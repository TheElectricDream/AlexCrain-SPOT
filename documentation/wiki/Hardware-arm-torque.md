# arm_torque.cpp

**Files:** `+spot/+hardware/csrc/arm/src/arm_torque.cpp`, `include/arm_torque.h` · **MATLAB side:** [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]]

<!-- source: +spot/+hardware/csrc/arm/include/arm_torque.h -->
<!-- source: +spot/+hardware/csrc/arm/src/arm_torque.cpp -->

## What it is for

The driver for the arm's three Dynamixel MX-64(2.0) servos (IDs 1–3 on the U2D2 adapter, protocol 2.0, 1 Mbps) in current control. It is self-contained: it speaks the protocol itself and does not use the Dynamixel SDK. The reasons, found when the old blocks' encoder reads came in at about 2 Hz although the servos update their registers every millisecond:

1. **The USB adapter's latency timer.** The U2D2's FTDI chip holds what it receives for its *latency timer* (16 ms by default on Linux) before passing it on, and the SDK gave up on a reply after 2 × 8 + 2 = 18.6 ms (it assumes 8 ms). Replies arrived within a fraction of a millisecond of that deadline, and any extra USB or scheduling delay lost the read. Here the adapter is asked for a 1 ms timer (`ASYNC_LOW_LATENCY`, and the sysfs `latency_timer` when permitted; [[setup_arm_serial.sh|Hardware-Serial-Setup]] makes it permanent), the timer is read back, and the reply deadline is the timer plus a margin.
2. **All or nothing.** One servo's missing reply threw away all three (the SDK's group read), and the old block then repeated the last angles and reported zero speed without saying so. Here each servo's reply stands on its own, a missing one keeps its last values, and the status counts what was missed.
3. **Busy-waiting.** The SDK spins on the port; this waits in `poll()`, so the rest of the program and the kernel's USB work keep running while the servos answer.

On the stand-in bus used in development, all three servos were read at the diagram's 20 Hz without misses (and up to 250 Hz).

## The API (`arm_torque.h`)

| | |
|---|---|
| `arm_torque_open(port, currentLimit, zeroReturnDelay)` | open the port, set every servo up (torque off, current mode, Current Limit, Return Delay Time 0 if asked, goal 0, torque on); 1 if all answered |
| `arm_torque_read(pos, vel, cur, status)` | one sync read of Present Current, Velocity, Position; a servo that didn't answer keeps its last values; `status` = [answered, read time ms, latency timer ms, missed so far, alert] |
| `arm_torque_write(goal)` | one sync write of the three Goal Currents (3.36 mA units), no reply |
| `arm_torque_close()` | goals 0, torque off, port closed |

## Function by function (`arm_torque.cpp`)

**Lines 36–52.** The IDs; the control-table addresses (Return Delay Time 9, Operating Mode 11, Current Limit 38, Torque Enable 64, Goal Current 102, Present Current 126 which is followed in the table by Present Velocity and Present Position, so one 10-byte read gets all three); the instruction codes (write 0x03, sync read 0x82, sync write 0x83, status 0x55) and the broadcast ID 0xFE.

**Lines 54–62.** State: the file descriptor, the latency timer, how long to wait for replies, the current limit, the number of missed reads, the last values per servo, and a receive buffer.

**Lines 64–69.** `nowMs`: a monotonic clock in milliseconds.

**Lines 71–80.** `crc16`: the protocol's CRC-16 (polynomial 0x8005), bitwise.

**Lines 84–101.** `packet`: build an instruction packet: header `FF FF FD 00`, id, length (filled in after), instruction, parameters with **byte stuffing** (an extra `FD` after any `FF FF FD` inside the data, so the header pattern can't appear in it), then the CRC.

**Lines 103–125.** `send`: flush anything left in the input, write the packet (non-blocking port, so loop with `poll` for `POLLOUT` until it is all out, up to 20 ms).

**Lines 129–169.** `takeStatus`: pull the next complete, valid status packet out of the receive buffer: find the header, check the length is sane, wait for the whole packet, verify the CRC and that it is a status packet (skipping a byte and retrying otherwise), take the id and the error byte, copy the parameters **un-stuffing** the extra `FD`s, drop the packet from the buffer.

**Lines 174–212.** `collect`: wait until every wanted id has replied or the deadline passes. Each loop: take a status packet if there is one (record it for its id, note the Alert bit 0x80 of the error byte); otherwise `poll` for input until the deadline and read what arrived. Returns how many replied.

**Lines 215–232.** `writeReg`: write 1, 2 or 4 bytes to one register of one servo and wait for its reply, up to three tries.

**Lines 234–252.** `syncWriteCurrents`: one sync-write packet carrying each servo's Goal Current (NaN → 0, clipped to the limit, rounded to an `int16`); no replies to a broadcast.

**Lines 256–288.** `lowLatency`: set `ASYNC_LOW_LATENCY` through `TIOCSSERIAL` (allowed without root), try to write `1` to `/sys/class/tty/<port>/device/latency_timer` (needs root or the udev rule) and read back whatever it is; the reply deadline is twice the timer plus 8 ms, at most 30 ms (the first bytes can just miss a flush; but never most of a 50 ms step for a servo that doesn't answer).

**Lines 292–333.** `arm_torque_open`: reset the state; open the port non-blocking; raw 8N1 at 1 Mbps; the latency timer; then for each servo: torque off (the EEPROM registers need it), operating mode 0, Current Limit, optionally Return Delay 0, Goal Current 0, torque on; report on stderr.

**Lines 335–370.** `arm_torque_read`: a sync-read request for 10 bytes from address 126 of all three; collect the replies within the deadline; decode current (`int16`), velocity and position (`int32`) of those that answered into the "last" values; count the missed ones; return the last values and the status.

**Lines 372–387.** `arm_torque_write` and `arm_torque_close` as described above.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/csrc/arm/include/arm_torque.h</b> (31 lines; click to show)</summary>

```c
   1  #ifndef SPOT_ARM_TORQUE_H
   2  #define SPOT_ARM_TORQUE_H
   3  /* The arm's three Dynamixel MX-64(2.0) servos (IDs 1-3) in current control, for
   4     spot.hardware.ArmTorque. Self-contained (protocol 2.0, Linux serial); see arm_torque.cpp. */
   5  #ifdef __cplusplus
   6  extern "C" {
   7  #endif
   8  
   9  /* Opens the port and sets the servos up (torque off, current control mode, Current Limit,
  10     Return Delay Time 0 if zeroReturnDelay, goal current 0, torque on). Returns 1 if every servo
  11     answered, 0 if the port or a servo failed (the other calls then do what they can). */
  12  int arm_torque_open(const char *port, double currentLimit, double zeroReturnDelay);
  13  
  14  /* One sync read of Present Current, Velocity and Position from the three servos.
  15     pos [pulses, 2048 = straight out], vel [0.229 rev/min], cur [3.36 mA]: a servo that did not
  16     answer keeps its last values. status: [servos that answered (0-3), the read's time [ms],
  17     the adapter's latency timer [ms] (-1 unknown), servo reads missed since the start,
  18     1 if a servo's reply had its Alert bit (a hardware error, e.g. overload: see its Hardware
  19     Error Status)]. */
  20  void arm_torque_read(double pos[3], double vel[3], double cur[3], double status[5]);
  21  
  22  /* Sends each servo its Goal Current [3.36 mA units] (one sync write, no reply). */
  23  void arm_torque_write(const double goal[3]);
  24  
  25  /* Goal currents 0, torque off, port closed. */
  26  void arm_torque_close(void);
  27  
  28  #ifdef __cplusplus
  29  }
  30  #endif
  31  #endif
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/arm/src/arm_torque.cpp</b> (387 lines; click to show)</summary>

```cpp
   1  // The arm's three Dynamixel MX-64(2.0) servos (IDs 1-3, protocol 2.0, 1 Mbps) in current control,
   2  // for spot.hardware.ArmTorque. Self-contained: it does not use the Dynamixel SDK (the old arm
   3  // blocks did; they are in ../archive/old_arm_blocks), for these reasons, found when
   4  // the old encoder reads came in at a few Hz although the servos update their registers every
   5  // millisecond:
   6  //   - The U2D2's FTDI chip holds what it receives for its latency timer (16 ms by default on Linux)
   7  //     before passing it on, and the SDK gave up on a reply after 2 x 8 + 2 ms (it assumes 8 ms):
   8  //     replies came within a fraction of a millisecond of the deadline, and any extra USB or
   9  //     scheduling delay lost the read. Here the adapter is asked for a 1 ms latency timer
  10  //     (ASYNC_LOW_LATENCY, and the sysfs latency_timer when allowed), the timer is read back, and
  11  //     the deadline is the timer plus a margin.
  12  //   - One servo's missing reply threw away all three (the SDK's group read is all or nothing), and
  13  //     the old block then repeated the last angles and reported zero speed without saying so.
  14  //     Here each servo's reply stands on its own, a missing one keeps its last values, and the
  15  //     status output counts what was missed.
  16  //   - The SDK busy-waits on the port; this waits with poll(), so the rest of the program (and
  17  //     the kernel's USB work) keeps running while the servos answer.
  18  #include "arm_torque.h"
  19  
  20  #include <errno.h>
  21  #include <fcntl.h>
  22  #include <limits.h>
  23  #include <linux/serial.h>
  24  #include <poll.h>
  25  #include <stdint.h>
  26  #include <stdio.h>
  27  #include <stdlib.h>
  28  #include <string.h>
  29  #include <sys/ioctl.h>
  30  #include <termios.h>
  31  #include <time.h>
  32  #include <unistd.h>
  33  
  34  namespace {
  35  
  36  const int N = 3;
  37  const uint8_t IDS[N] = {1, 2, 3};
  38  
  39  // Control table addresses (MX-64(2.0), protocol 2.0)
  40  const uint16_t ADDR_RETURN_DELAY = 9;
  41  const uint16_t ADDR_OPERATING_MODE = 11;
  42  const uint16_t ADDR_CURRENT_LIMIT = 38;
  43  const uint16_t ADDR_TORQUE_ENABLE = 64;
  44  const uint16_t ADDR_GOAL_CURRENT = 102;
  45  const uint16_t ADDR_PRESENT_CURRENT = 126;   // 2 bytes, then Present Velocity (4) and Position (4)
  46  const uint16_t READ_LENGTH = 10;
  47  
  48  const uint8_t INST_WRITE = 0x03;
  49  const uint8_t INST_SYNC_READ = 0x82;
  50  const uint8_t INST_SYNC_WRITE = 0x83;
  51  const uint8_t INST_STATUS = 0x55;
  52  const uint8_t BROADCAST = 0xFE;
  53  
  54  int fd = -1;
  55  double latencyMs = -1;          // the adapter's latency timer, -1 if unknown
  56  double replyMs = 30;            // how long to wait for the replies to one request
  57  double currentLimit = 850;
  58  double missed = 0;              // servo reads missed since the start
  59  double lastPos[N], lastVel[N], lastCur[N];
  60  
  61  uint8_t rx[1024];
  62  int rxLen = 0;
  63  
  64  double nowMs()
  65  {
  66      struct timespec t;
  67      clock_gettime(CLOCK_MONOTONIC, &t);
  68      return t.tv_sec * 1e3 + t.tv_nsec * 1e-6;
  69  }
  70  
  71  uint16_t crc16(const uint8_t *d, int n)
  72  {
  73      uint16_t crc = 0;
  74      for (int i = 0; i < n; i++) {
  75          crc ^= (uint16_t)d[i] << 8;
  76          for (int b = 0; b < 8; b++)
  77              crc = (crc & 0x8000) ? (uint16_t)((crc << 1) ^ 0x8005) : (uint16_t)(crc << 1);
  78      }
  79      return crc;
  80  }
  81  
  82  // A packet for id: header, then the instruction and parameters with byte stuffing (an FD after
  83  // every FF FF FD), then the CRC. Returns its length.
  84  int packet(uint8_t *out, uint8_t id, uint8_t inst, const uint8_t *par, int n)
  85  {
  86      int k = 7;
  87      out[0] = 0xFF; out[1] = 0xFF; out[2] = 0xFD; out[3] = 0x00; out[4] = id;
  88      out[k++] = inst;
  89      for (int i = 0; i < n; i++) {
  90          out[k++] = par[i];
  91          if (k >= 10 && out[k - 3] == 0xFF && out[k - 2] == 0xFF && out[k - 1] == 0xFD)
  92              out[k++] = 0xFD;
  93      }
  94      int length = k - 7 + 2;
  95      out[5] = (uint8_t)(length & 0xFF);
  96      out[6] = (uint8_t)(length >> 8);
  97      uint16_t c = crc16(out, k);
  98      out[k++] = (uint8_t)(c & 0xFF);
  99      out[k++] = (uint8_t)(c >> 8);
 100      return k;
 101  }
 102  
 103  bool send(const uint8_t *p, int n)
 104  {
 105      if (fd < 0)
 106          return false;
 107      tcflush(fd, TCIFLUSH);   // nothing left over from an earlier reply
 108      rxLen = 0;
 109      int done = 0;
 110      double until = nowMs() + 20;
 111      while (done < n) {
 112          ssize_t w = write(fd, p + done, n - done);
 113          if (w > 0) {
 114              done += (int)w;
 115          } else if (w < 0 && errno != EAGAIN && errno != EINTR) {
 116              return false;
 117          } else {
 118              if (nowMs() > until)
 119                  return false;
 120              struct pollfd pf = {fd, POLLOUT, 0};
 121              poll(&pf, 1, 1);
 122          }
 123      }
 124      return true;
 125  }
 126  
 127  // Takes the next valid status packet out of rx. Returns its id (params unstuffed into par, their
 128  // count in *np, the error byte in *err), 0 if there is no complete packet yet.
 129  int takeStatus(uint8_t *par, int *np, uint8_t *err)
 130  {
 131      for (;;) {
 132          int i = 0;
 133          while (i + 3 < rxLen && !(rx[i] == 0xFF && rx[i + 1] == 0xFF && rx[i + 2] == 0xFD && rx[i + 3] == 0x00))
 134              i++;
 135          if (i > 0) {
 136              memmove(rx, rx + i, rxLen - i);
 137              rxLen -= i;
 138          }
 139          if (rxLen < 9)
 140              return 0;
 141          int length = rx[5] | (rx[6] << 8);
 142          if (length < 4 || 7 + length > (int)sizeof(rx)) {   // not a packet: skip the header
 143              memmove(rx, rx + 1, rxLen - 1);
 144              rxLen -= 1;
 145              continue;
 146          }
 147          if (rxLen < 7 + length)
 148              return 0;
 149          int total = 7 + length;
 150          uint16_t c = (uint16_t)(rx[total - 2] | (rx[total - 1] << 8));
 151          if (crc16(rx, total - 2) != c || rx[7] != INST_STATUS) {
 152              memmove(rx, rx + 1, rxLen - 1);
 153              rxLen -= 1;
 154              continue;
 155          }
 156          int id = rx[4];
 157          *err = rx[8];
 158          int n = 0;
 159          for (int k = 9; k < total - 2; k++) {
 160              if (k >= 12 && rx[k] == 0xFD && rx[k - 1] == 0xFD && rx[k - 2] == 0xFF && rx[k - 3] == 0xFF)
 161                  continue;   // a stuffed FD
 162              par[n++] = rx[k];
 163          }
 164          *np = n;
 165          memmove(rx, rx + total, rxLen - total);
 166          rxLen -= total;
 167          return id;
 168      }
 169  }
 170  
 171  // Waits (poll, not spinning) until every id in want[] has replied or the deadline passes.
 172  // got[k] is set for each reply, its parameters copied to data[k] (up to len bytes).
 173  // Returns the number of replies; *alert is set if any error byte had its Alert bit.
 174  int collect(const uint8_t *want, int nWant, int len, uint8_t data[][16], bool *got, bool *alert, double deadline)
 175  {
 176      int count = 0;
 177      for (int k = 0; k < nWant; k++)
 178          got[k] = false;
 179      while (count < nWant) {
 180          uint8_t par[64];
 181          int np = 0;
 182          uint8_t err = 0;
 183          int id = takeStatus(par, &np, &err);
 184          if (id > 0) {
 185              for (int k = 0; k < nWant; k++) {
 186                  if (want[k] == id && !got[k] && np >= len) {
 187                      memcpy(data[k], par, len);
 188                      got[k] = true;
 189                      count++;
 190                      if (err & 0x80)
 191                          *alert = true;
 192                  }
 193              }
 194              continue;
 195          }
 196          double left = deadline - nowMs();
 197          if (left <= 0)
 198              break;
 199          struct pollfd pf = {fd, POLLIN, 0};
 200          int r = poll(&pf, 1, (int)(left + 0.999));
 201          if (r > 0 && (pf.revents & POLLIN)) {
 202              ssize_t n = read(fd, rx + rxLen, sizeof(rx) - rxLen);
 203              if (n > 0)
 204                  rxLen += (int)n;
 205              if (rxLen == (int)sizeof(rx))
 206                  rxLen = 0;   // garbage: start again
 207          } else if (r < 0 && errno != EINTR) {
 208              break;
 209          }
 210      }
 211      return count;
 212  }
 213  
 214  // Writes n bytes to one servo's control table and waits for its reply (up to 3 tries)
 215  bool writeReg(uint8_t id, uint16_t addr, uint32_t value, int n)
 216  {
 217      uint8_t par[6] = {(uint8_t)(addr & 0xFF), (uint8_t)(addr >> 8)};
 218      for (int i = 0; i < n; i++)
 219          par[2 + i] = (uint8_t)(value >> (8 * i));
 220      uint8_t p[32];
 221      int len = packet(p, id, INST_WRITE, par, 2 + n);
 222      for (int attempt = 0; attempt < 3; attempt++) {
 223          if (!send(p, len))
 224              continue;
 225          uint8_t data[1][16];
 226          bool got[1];
 227          bool alert = false;
 228          if (collect(&id, 1, 0, data, got, &alert, nowMs() + replyMs) == 1)
 229              return true;
 230      }
 231      return false;
 232  }
 233  
 234  void syncWriteCurrents(const double goal[N])
 235  {
 236      uint8_t par[4 + 3 * N];
 237      int k = 0;
 238      par[k++] = ADDR_GOAL_CURRENT & 0xFF; par[k++] = ADDR_GOAL_CURRENT >> 8;
 239      par[k++] = 2; par[k++] = 0;
 240      for (int j = 0; j < N; j++) {
 241          double g = goal[j];
 242          if (!(g == g)) g = 0;   // NaN
 243          if (g > currentLimit) g = currentLimit;
 244          if (g < -currentLimit) g = -currentLimit;
 245          int16_t u = (int16_t)(g < 0 ? g - 0.5 : g + 0.5);
 246          par[k++] = IDS[j];
 247          par[k++] = (uint8_t)(u & 0xFF);
 248          par[k++] = (uint8_t)((uint16_t)u >> 8);
 249      }
 250      uint8_t p[64];
 251      send(p, packet(p, BROADCAST, INST_SYNC_WRITE, par, k));
 252  }
 253  
 254  // The adapter's latency timer: ask for 1 ms (allowed without root through TIOCSSERIAL; the sysfs
 255  // file needs root or a udev rule), then read what it is
 256  void lowLatency(const char *port)
 257  {
 258      struct serial_struct ss;
 259      if (ioctl(fd, TIOCGSERIAL, &ss) == 0) {
 260          ss.flags |= ASYNC_LOW_LATENCY;
 261          ioctl(fd, TIOCSSERIAL, &ss);
 262      }
 263      char real[PATH_MAX];
 264      const char *dev = realpath(port, real) ? real : port;
 265      const char *base = strrchr(dev, '/');
 266      base = base ? base + 1 : dev;
 267      char path[PATH_MAX + 64];
 268      snprintf(path, sizeof(path), "/sys/class/tty/%s/device/latency_timer", base);
 269      FILE *f = fopen(path, "w");
 270      if (f) {
 271          fputs("1", f);
 272          fclose(f);
 273      }
 274      f = fopen(path, "r");
 275      if (f) {
 276          int v = 0;
 277          if (fscanf(f, "%d", &v) == 1)
 278              latencyMs = v;
 279          fclose(f);
 280      }
 281      // the replies come within ~3 ms on the wire; allow for the latency timer twice (the first
 282      // bytes can just miss a flush) and a margin for USB and scheduling, but not most of a 50 ms
 283      // step for a servo that doesn't answer
 284      double lat = latencyMs >= 0 ? latencyMs : 16;
 285      replyMs = 2 * lat + 8;
 286      if (replyMs > 30)
 287          replyMs = 30;
 288  }
 289  
 290  }  // namespace
 291  
 292  extern "C" int arm_torque_open(const char *port, double limit, double zeroReturnDelay)
 293  {
 294      currentLimit = limit;
 295      missed = 0;
 296      for (int j = 0; j < N; j++)
 297          lastPos[j] = 2048, lastVel[j] = 0, lastCur[j] = 0;
 298      fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
 299      if (fd < 0) {
 300          fprintf(stderr, "[arm_torque] cannot open %s: %s\n", port, strerror(errno));
 301          return 0;
 302      }
 303      struct termios t;
 304      memset(&t, 0, sizeof(t));
 305      t.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;
 306      t.c_iflag = IGNPAR;
 307      t.c_cc[VMIN] = 0;
 308      t.c_cc[VTIME] = 0;
 309      cfsetispeed(&t, B1000000);
 310      cfsetospeed(&t, B1000000);
 311      tcflush(fd, TCIOFLUSH);
 312      tcsetattr(fd, TCSANOW, &t);
 313      lowLatency(port);
 314  
 315      int ok = 1;
 316      for (int j = 0; j < N; j++) {
 317          uint8_t id = IDS[j];
 318          bool s = writeReg(id, ADDR_TORQUE_ENABLE, 0, 1);         // the EEPROM area needs torque off
 319          s = s && writeReg(id, ADDR_OPERATING_MODE, 0, 1);        // current control
 320          s = s && writeReg(id, ADDR_CURRENT_LIMIT, (uint32_t)limit, 2);
 321          if (zeroReturnDelay != 0)
 322              s = s && writeReg(id, ADDR_RETURN_DELAY, 0, 1);
 323          s = s && writeReg(id, ADDR_GOAL_CURRENT, 0, 2);
 324          s = s && writeReg(id, ADDR_TORQUE_ENABLE, 1, 1);
 325          if (!s) {
 326              fprintf(stderr, "[arm_torque] servo %d did not answer its setup\n", id);
 327              ok = 0;
 328          }
 329      }
 330      fprintf(stderr, "[arm_torque] %s: latency timer %g ms, replies awaited %g ms, servos %s\n", port, latencyMs,
 331              replyMs, ok ? "ready" : "NOT all ready");
 332      return ok;
 333  }
 334  
 335  extern "C" void arm_torque_read(double pos[3], double vel[3], double cur[3], double status[5])
 336  {
 337      double t0 = nowMs();
 338      int count = 0;
 339      bool alert = false;
 340      if (fd >= 0) {
 341          uint8_t par[4 + N] = {ADDR_PRESENT_CURRENT & 0xFF, ADDR_PRESENT_CURRENT >> 8, READ_LENGTH, 0};
 342          for (int j = 0; j < N; j++)
 343              par[4 + j] = IDS[j];
 344          uint8_t p[32];
 345          if (send(p, packet(p, BROADCAST, INST_SYNC_READ, par, 4 + N))) {
 346              uint8_t data[N][16];
 347              bool got[N];
 348              count = collect(IDS, N, READ_LENGTH, data, got, &alert, nowMs() + replyMs);
 349              for (int j = 0; j < N; j++) {
 350                  if (!got[j])
 351                      continue;
 352                  const uint8_t *d = data[j];
 353                  lastCur[j] = (int16_t)(d[0] | (d[1] << 8));
 354                  lastVel[j] = (int32_t)((uint32_t)d[2] | ((uint32_t)d[3] << 8) | ((uint32_t)d[4] << 16) | ((uint32_t)d[5] << 24));
 355                  lastPos[j] = (int32_t)((uint32_t)d[6] | ((uint32_t)d[7] << 8) | ((uint32_t)d[8] << 16) | ((uint32_t)d[9] << 24));
 356              }
 357          }
 358      }
 359      missed += N - count;
 360      for (int j = 0; j < N; j++) {
 361          pos[j] = lastPos[j];
 362          vel[j] = lastVel[j];
 363          cur[j] = lastCur[j];
 364      }
 365      status[0] = count;
 366      status[1] = nowMs() - t0;
 367      status[2] = latencyMs;
 368      status[3] = missed;
 369      status[4] = alert ? 1 : 0;
 370  }
 371  
 372  extern "C" void arm_torque_write(const double goal[3])
 373  {
 374      syncWriteCurrents(goal);
 375  }
 376  
 377  extern "C" void arm_torque_close(void)
 378  {
 379      if (fd < 0)
 380          return;
 381      double zero[N] = {0, 0, 0};
 382      syncWriteCurrents(zero);
 383      for (int j = 0; j < N; j++)
 384          writeReg(IDS[j], ADDR_TORQUE_ENABLE, 0, 1);
 385      close(fd);
 386      fd = -1;
 387  }
```

</details>
<!-- /listing -->
