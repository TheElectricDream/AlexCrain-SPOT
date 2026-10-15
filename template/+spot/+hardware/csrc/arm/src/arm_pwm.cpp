// The arm's three Dynamixel MX-64(2.0) servos (IDs 1-3, protocol 2.0, 1 Mbps) in PWM (voltage)
// control, for spot.hardware.ArmPWM. The current readings are not used: at the arm's small
// torques they are a few units of 3.36 mA, too coarse and noisy to control or measure with.
// Self-contained: it does not use the Dynamixel SDK (the old arm
// blocks did; they are in ../archive/old_arm_blocks), for these reasons, found when
// the old encoder reads came in at a few Hz although the servos update their registers every
// millisecond:
//   - The U2D2's FTDI chip holds what it receives for its latency timer (16 ms by default on Linux)
//     before passing it on, and the SDK gave up on a reply after 2 x 8 + 2 ms (it assumes 8 ms):
//     replies came within a fraction of a millisecond of the deadline, and any extra USB or
//     scheduling delay lost the read. Here the adapter is asked for a 1 ms latency timer
//     (ASYNC_LOW_LATENCY, and the sysfs latency_timer when allowed), the timer is read back, and
//     the deadline is the timer plus a margin.
//   - One servo's missing reply threw away all three (the SDK's group read is all or nothing), and
//     the old block then repeated the last angles and reported zero speed without saying so.
//     Here each servo's reply stands on its own, a missing one keeps its last values, and the
//     status output counts what was missed.
//   - The SDK busy-waits on the port; this waits with poll(), so the rest of the program (and
//     the kernel's USB work) keeps running while the servos answer.
#include "arm_pwm.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/serial.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

namespace {

const int N = 3;
const uint8_t IDS[N] = {1, 2, 3};

// Control table addresses (MX-64(2.0), protocol 2.0)
const uint16_t ADDR_RETURN_DELAY = 9;
const uint16_t ADDR_OPERATING_MODE = 11;
const uint16_t ADDR_PWM_LIMIT = 36;
const uint16_t ADDR_TORQUE_ENABLE = 64;
const uint16_t ADDR_GOAL_PWM = 100;
const uint16_t ADDR_PRESENT_PWM = 124;   // 2 bytes, then Present Current (2), Velocity (4), Position (4)
const uint16_t READ_LENGTH = 12;
const uint8_t MODE_PWM = 16;             // PWM (voltage) control

const uint8_t INST_WRITE = 0x03;
const uint8_t INST_SYNC_READ = 0x82;
const uint8_t INST_SYNC_WRITE = 0x83;
const uint8_t INST_STATUS = 0x55;
const uint8_t BROADCAST = 0xFE;

int fd = -1;
double latencyMs = -1;          // the adapter's latency timer, -1 if unknown
double replyMs = 30;            // how long to wait for the replies to one request
double pwmLimit = 885;
double missed = 0;              // servo reads missed since the start
double lastPos[N], lastVel[N], lastPwm[N];

uint8_t rx[1024];
int rxLen = 0;

double nowMs()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e3 + t.tv_nsec * 1e-6;
}

uint16_t crc16(const uint8_t *d, int n)
{
    uint16_t crc = 0;
    for (int i = 0; i < n; i++) {
        crc ^= (uint16_t)d[i] << 8;
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000) ? (uint16_t)((crc << 1) ^ 0x8005) : (uint16_t)(crc << 1);
    }
    return crc;
}

// A packet for id: header, then the instruction and parameters with byte stuffing (an FD after
// every FF FF FD), then the CRC. Returns its length.
int packet(uint8_t *out, uint8_t id, uint8_t inst, const uint8_t *par, int n)
{
    int k = 7;
    out[0] = 0xFF; out[1] = 0xFF; out[2] = 0xFD; out[3] = 0x00; out[4] = id;
    out[k++] = inst;
    for (int i = 0; i < n; i++) {
        out[k++] = par[i];
        if (k >= 10 && out[k - 3] == 0xFF && out[k - 2] == 0xFF && out[k - 1] == 0xFD)
            out[k++] = 0xFD;
    }
    int length = k - 7 + 2;
    out[5] = (uint8_t)(length & 0xFF);
    out[6] = (uint8_t)(length >> 8);
    uint16_t c = crc16(out, k);
    out[k++] = (uint8_t)(c & 0xFF);
    out[k++] = (uint8_t)(c >> 8);
    return k;
}

bool send(const uint8_t *p, int n)
{
    if (fd < 0)
        return false;
    tcflush(fd, TCIFLUSH);   // nothing left over from an earlier reply
    rxLen = 0;
    int done = 0;
    double until = nowMs() + 20;
    while (done < n) {
        ssize_t w = write(fd, p + done, n - done);
        if (w > 0) {
            done += (int)w;
        } else if (w < 0 && errno != EAGAIN && errno != EINTR) {
            return false;
        } else {
            if (nowMs() > until)
                return false;
            struct pollfd pf = {fd, POLLOUT, 0};
            poll(&pf, 1, 1);
        }
    }
    return true;
}

// Takes the next valid status packet out of rx. Returns its id (params unstuffed into par, their
// count in *np, the error byte in *err), 0 if there is no complete packet yet.
int takeStatus(uint8_t *par, int *np, uint8_t *err)
{
    for (;;) {
        int i = 0;
        while (i + 3 < rxLen && !(rx[i] == 0xFF && rx[i + 1] == 0xFF && rx[i + 2] == 0xFD && rx[i + 3] == 0x00))
            i++;
        if (i > 0) {
            memmove(rx, rx + i, rxLen - i);
            rxLen -= i;
        }
        if (rxLen < 9)
            return 0;
        int length = rx[5] | (rx[6] << 8);
        if (length < 4 || 7 + length > (int)sizeof(rx)) {   // not a packet: skip the header
            memmove(rx, rx + 1, rxLen - 1);
            rxLen -= 1;
            continue;
        }
        if (rxLen < 7 + length)
            return 0;
        int total = 7 + length;
        uint16_t c = (uint16_t)(rx[total - 2] | (rx[total - 1] << 8));
        if (crc16(rx, total - 2) != c || rx[7] != INST_STATUS) {
            memmove(rx, rx + 1, rxLen - 1);
            rxLen -= 1;
            continue;
        }
        int id = rx[4];
        *err = rx[8];
        int n = 0;
        for (int k = 9; k < total - 2; k++) {
            if (k >= 12 && rx[k] == 0xFD && rx[k - 1] == 0xFD && rx[k - 2] == 0xFF && rx[k - 3] == 0xFF)
                continue;   // a stuffed FD
            par[n++] = rx[k];
        }
        *np = n;
        memmove(rx, rx + total, rxLen - total);
        rxLen -= total;
        return id;
    }
}

// Waits (poll, not spinning) until every id in want[] has replied or the deadline passes.
// got[k] is set for each reply, its parameters copied to data[k] (up to len bytes).
// Returns the number of replies; *alert is set if any error byte had its Alert bit.
int collect(const uint8_t *want, int nWant, int len, uint8_t data[][16], bool *got, bool *alert, double deadline)
{
    int count = 0;
    for (int k = 0; k < nWant; k++)
        got[k] = false;
    while (count < nWant) {
        uint8_t par[64];
        int np = 0;
        uint8_t err = 0;
        int id = takeStatus(par, &np, &err);
        if (id > 0) {
            for (int k = 0; k < nWant; k++) {
                if (want[k] == id && !got[k] && np >= len) {
                    memcpy(data[k], par, len);
                    got[k] = true;
                    count++;
                    if (err & 0x80)
                        *alert = true;
                }
            }
            continue;
        }
        double left = deadline - nowMs();
        if (left <= 0)
            break;
        struct pollfd pf = {fd, POLLIN, 0};
        int r = poll(&pf, 1, (int)(left + 0.999));
        if (r > 0 && (pf.revents & POLLIN)) {
            ssize_t n = read(fd, rx + rxLen, sizeof(rx) - rxLen);
            if (n > 0)
                rxLen += (int)n;
            if (rxLen == (int)sizeof(rx))
                rxLen = 0;   // garbage: start again
        } else if (r < 0 && errno != EINTR) {
            break;
        }
    }
    return count;
}

// Writes n bytes to one servo's control table and waits for its reply (up to 3 tries)
bool writeReg(uint8_t id, uint16_t addr, uint32_t value, int n)
{
    uint8_t par[6] = {(uint8_t)(addr & 0xFF), (uint8_t)(addr >> 8)};
    for (int i = 0; i < n; i++)
        par[2 + i] = (uint8_t)(value >> (8 * i));
    uint8_t p[32];
    int len = packet(p, id, INST_WRITE, par, 2 + n);
    for (int attempt = 0; attempt < 3; attempt++) {
        if (!send(p, len))
            continue;
        uint8_t data[1][16];
        bool got[1];
        bool alert = false;
        if (collect(&id, 1, 0, data, got, &alert, nowMs() + replyMs) == 1)
            return true;
    }
    return false;
}

// Each servo's Goal PWM, rounded and within the PWM Limit, in one sync write
void syncWritePwm(const double goal[N])
{
    uint8_t par[4 + 3 * N];
    int k = 0;
    par[k++] = ADDR_GOAL_PWM & 0xFF; par[k++] = ADDR_GOAL_PWM >> 8;
    par[k++] = 2; par[k++] = 0;
    for (int j = 0; j < N; j++) {
        double g = goal[j];
        if (!(g == g)) g = 0;   // NaN
        if (g > pwmLimit) g = pwmLimit;
        if (g < -pwmLimit) g = -pwmLimit;
        int16_t u = (int16_t)(g < 0 ? g - 0.5 : g + 0.5);
        par[k++] = IDS[j];
        par[k++] = (uint8_t)(u & 0xFF);
        par[k++] = (uint8_t)((uint16_t)u >> 8);
    }
    uint8_t p[64];
    send(p, packet(p, BROADCAST, INST_SYNC_WRITE, par, k));
}

// The adapter's latency timer: ask for 1 ms (allowed without root through TIOCSSERIAL; the sysfs
// file needs root or a udev rule), then read what it is
void lowLatency(const char *port)
{
    struct serial_struct ss;
    if (ioctl(fd, TIOCGSERIAL, &ss) == 0) {
        ss.flags |= ASYNC_LOW_LATENCY;
        ioctl(fd, TIOCSSERIAL, &ss);
    }
    char real[PATH_MAX];
    const char *dev = realpath(port, real) ? real : port;
    const char *base = strrchr(dev, '/');
    base = base ? base + 1 : dev;
    char path[PATH_MAX + 64];
    snprintf(path, sizeof(path), "/sys/class/tty/%s/device/latency_timer", base);
    FILE *f = fopen(path, "w");
    if (f) {
        fputs("1", f);
        fclose(f);
    }
    f = fopen(path, "r");
    if (f) {
        int v = 0;
        if (fscanf(f, "%d", &v) == 1)
            latencyMs = v;
        fclose(f);
    }
    // the replies come within ~3 ms on the wire; allow for the latency timer twice (the first
    // bytes can just miss a flush) and a margin for USB and scheduling, but not most of a 50 ms
    // step for a servo that doesn't answer
    double lat = latencyMs >= 0 ? latencyMs : 16;
    replyMs = 2 * lat + 8;
    if (replyMs > 30)
        replyMs = 30;
}

}  // namespace

extern "C" int arm_pwm_open(const char *port, double limit, double zeroReturnDelay)
{
    pwmLimit = limit;
    missed = 0;
    for (int j = 0; j < N; j++)
        lastPos[j] = 2048, lastVel[j] = 0, lastPwm[j] = 0;
    fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        fprintf(stderr, "[arm_pwm] cannot open %s: %s\n", port, strerror(errno));
        return 0;
    }
    struct termios t;
    memset(&t, 0, sizeof(t));
    t.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
    t.c_iflag = IGNPAR;
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    cfsetispeed(&t, B1000000);
    cfsetospeed(&t, B1000000);
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &t);
    lowLatency(port);

    int ok = 1;
    for (int j = 0; j < N; j++) {
        uint8_t id = IDS[j];
        bool s = writeReg(id, ADDR_TORQUE_ENABLE, 0, 1);         // the EEPROM area needs torque off
        s = s && writeReg(id, ADDR_OPERATING_MODE, MODE_PWM, 1);  // PWM control
        s = s && writeReg(id, ADDR_PWM_LIMIT, (uint32_t)limit, 2);
        if (zeroReturnDelay != 0)
            s = s && writeReg(id, ADDR_RETURN_DELAY, 0, 1);
        s = s && writeReg(id, ADDR_GOAL_PWM, 0, 2);
        s = s && writeReg(id, ADDR_TORQUE_ENABLE, 1, 1);
        if (!s) {
            fprintf(stderr, "[arm_pwm] servo %d did not answer its setup\n", id);
            ok = 0;
        }
    }
    fprintf(stderr, "[arm_pwm] %s: latency timer %g ms, replies awaited %g ms, servos %s\n", port, latencyMs,
            replyMs, ok ? "ready" : "NOT all ready");
    return ok;
}

extern "C" void arm_pwm_read(double pos[3], double vel[3], double pwm[3], double status[5])
{
    double t0 = nowMs();
    int count = 0;
    bool alert = false;
    if (fd >= 0) {
        uint8_t par[4 + N] = {ADDR_PRESENT_PWM & 0xFF, ADDR_PRESENT_PWM >> 8, READ_LENGTH, 0};
        for (int j = 0; j < N; j++)
            par[4 + j] = IDS[j];
        uint8_t p[32];
        if (send(p, packet(p, BROADCAST, INST_SYNC_READ, par, 4 + N))) {
            uint8_t data[N][16];
            bool got[N];
            count = collect(IDS, N, READ_LENGTH, data, got, &alert, nowMs() + replyMs);
            for (int j = 0; j < N; j++) {
                if (!got[j])
                    continue;
                const uint8_t *d = data[j];
                lastPwm[j] = (int16_t)(d[0] | (d[1] << 8));   // d[2..3]: Present Current, not used
                lastVel[j] = (int32_t)((uint32_t)d[4] | ((uint32_t)d[5] << 8) | ((uint32_t)d[6] << 16) | ((uint32_t)d[7] << 24));
                lastPos[j] = (int32_t)((uint32_t)d[8] | ((uint32_t)d[9] << 8) | ((uint32_t)d[10] << 16) | ((uint32_t)d[11] << 24));
            }
        }
    }
    missed += N - count;
    for (int j = 0; j < N; j++) {
        pos[j] = lastPos[j];
        vel[j] = lastVel[j];
        pwm[j] = lastPwm[j];
    }
    status[0] = count;
    status[1] = nowMs() - t0;
    status[2] = latencyMs;
    status[3] = missed;
    status[4] = alert ? 1 : 0;
}

extern "C" void arm_pwm_write(const double goal[3])
{
    syncWritePwm(goal);
}

extern "C" void arm_pwm_close(void)
{
    if (fd < 0)
        return;
    double zero[N] = {0, 0, 0};
    syncWritePwm(zero);
    for (int j = 0; j < N; j++)
        writeReg(IDS[j], ADDR_TORQUE_ENABLE, 0, 1);
    close(fd);
    fd = -1;
}
