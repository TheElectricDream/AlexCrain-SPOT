/* spot_udp.c -- see spot_udp.h. */

#include "spot_udp.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
typedef HANDLE thread_t;
typedef CRITICAL_SECTION lock_t;
#define lock_init(l)    InitializeCriticalSection(l)
#define lock_free(l)    DeleteCriticalSection(l)
#define lock_take(l)    EnterCriticalSection(l)
#define lock_give(l)    LeaveCriticalSection(l)
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define closesocket close
typedef pthread_t thread_t;
typedef pthread_mutex_t lock_t;
#define lock_init(l)    pthread_mutex_init(l, NULL)
#define lock_free(l)    pthread_mutex_destroy(l)
#define lock_take(l)    pthread_mutex_lock(l)
#define lock_give(l)    pthread_mutex_unlock(l)
#endif

#define MAX_PACKET 65536
#define MAX_RX 8
#define MAX_TX 16

typedef struct {
    int open;
    int port, min_bytes, max_bytes;
    unsigned long allowed;      /* sender address in network byte order, 0 = anyone */
    char allowed_text[64];
    SOCKET sd;
    thread_t thread;
    lock_t lock;
    volatile int stop;

    /* protected by lock */
    unsigned char latest[MAX_PACKET];
    int latest_len;
    double latest_time;         /* spot_udp_now() when it arrived, < 0 = nothing yet */
    unsigned long arrived;      /* packets accepted so far */
    unsigned long handed_out;   /* value of 'arrived' at the previous read */
} receiver;

typedef struct {
    int open;
    char ip[64];
    int port;
    SOCKET sd;
} sender;

static receiver rx[MAX_RX];
static sender tx[MAX_TX];
static int network_started;

static int network_startup(void)
{
#ifdef _WIN32
    if (!network_started) {
        WSADATA w;
        if (WSAStartup(MAKEWORD(2, 2), &w) != 0) return -1;
    }
#endif
    network_started = 1;
    return 0;
}

double spot_udp_now(void)
{
#ifdef _WIN32
    static LARGE_INTEGER freq;
    LARGE_INTEGER t;
    if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
#endif
}

/* ------------------------------------------------------------------ receiving */

/* Background thread for one port: read every packet the moment it arrives, keep the newest. */
#ifdef _WIN32
static DWORD WINAPI receive_loop(LPVOID arg)
#else
static void *receive_loop(void *arg)
#endif
{
    receiver *r = (receiver *)arg;
    static unsigned char buffers[MAX_RX][MAX_PACKET];
    unsigned char *buffer = buffers[r - rx];
    while (!r->stop) {
        struct sockaddr_in from;
#ifdef _WIN32
        int fromlen = sizeof(from);
#else
        socklen_t fromlen = sizeof(from);
#endif
        int n = recvfrom(r->sd, (char *)buffer, MAX_PACKET, 0, (struct sockaddr *)&from, &fromlen);
        if (n <= 0)
            continue;  /* timeout (lets the loop see r->stop) or a transient error */
        if (r->allowed && from.sin_addr.s_addr != r->allowed)
            continue;  /* not from the expected sender */
        if ((r->min_bytes && n < r->min_bytes) || (r->max_bytes && n > r->max_bytes))
            continue;  /* wrong format, e.g. an old program still sending another layout */

        lock_take(&r->lock);
        memcpy(r->latest, buffer, (size_t)n);
        r->latest_len = n;
        r->latest_time = spot_udp_now();
        r->arrived++;
        lock_give(&r->lock);
    }
    return 0;
}

static void rx_close(receiver *r)
{
    if (!r->open) return;
    r->stop = 1;
#ifdef _WIN32
    WaitForSingleObject(r->thread, INFINITE);
    CloseHandle(r->thread);
#else
    pthread_join(r->thread, NULL);
#endif
    closesocket(r->sd);
    lock_free(&r->lock);
    r->open = 0;
}

int spot_udp_rx_open(int port, const char *allowed_ip, int min_bytes, int max_bytes)
{
    struct sockaddr_in local;
    int on = 1, bufsize = 1 << 20, i;
    const char *ip = allowed_ip ? allowed_ip : "";
    receiver *r = NULL;

    for (i = 0; i < MAX_RX; i++) {
        if (rx[i].open && rx[i].port == port) {
            if (rx[i].min_bytes == min_bytes && rx[i].max_bytes == max_bytes && strcmp(rx[i].allowed_text, ip) == 0)
                return i;  /* already listening exactly like this */
            rx_close(&rx[i]);  /* same port, new settings: listen again */
            r = &rx[i];
            break;
        }
    }
    for (i = 0; !r && i < MAX_RX; i++)
        if (!rx[i].open) r = &rx[i];
    if (!r) {
        fprintf(stderr, "spot_udp: too many receive ports (at most %d)\n", MAX_RX);
        return -1;
    }
    if (network_startup() != 0) return -1;

    r->sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (r->sd == INVALID_SOCKET) return -1;
    setsockopt(r->sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
    setsockopt(r->sd, SOL_SOCKET, SO_RCVBUF, (const char *)&bufsize, sizeof(bufsize));
    {
        /* Wake up regularly so rx_close() can stop the thread */
#ifdef _WIN32
        DWORD timeout = 200;
#else
        struct timeval timeout = { 0, 200000 };
#endif
        setsockopt(r->sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    }

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons((unsigned short)port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(r->sd, (struct sockaddr *)&local, sizeof(local)) != 0) {
        fprintf(stderr, "spot_udp: cannot listen on UDP port %d (is another program using it?)\n", port);
        closesocket(r->sd);
        return -1;
    }

    r->port = port;
    r->min_bytes = min_bytes;
    r->max_bytes = max_bytes;
    strncpy(r->allowed_text, ip, sizeof(r->allowed_text) - 1);
    r->allowed_text[sizeof(r->allowed_text) - 1] = '\0';
    r->allowed = (ip[0] && strcmp(ip, "0.0.0.0") != 0) ? inet_addr(ip) : 0;
    r->latest_len = 0;
    r->latest_time = -1.0;
    r->arrived = r->handed_out = 0;
    r->stop = 0;
    lock_init(&r->lock);

#ifdef _WIN32
    r->thread = CreateThread(NULL, 0, receive_loop, r, 0, NULL);
    if (!r->thread) {
#else
    if (pthread_create(&r->thread, NULL, receive_loop, r) != 0) {
#endif
        lock_free(&r->lock);
        closesocket(r->sd);
        return -1;
    }
    r->open = 1;
    return (int)(r - rx);
}

int spot_udp_rx_read(int handle, void *out, int nbytes, double *age_seconds)
{
    receiver *r;
    int count = nbytes, fresh;

    if (handle < 0 || handle >= MAX_RX || !rx[handle].open) {
        memset(out, 0, (size_t)nbytes);
        if (age_seconds) *age_seconds = -1.0;
        return 0;
    }
    r = &rx[handle];
    lock_take(&r->lock);
    if (count > r->latest_len) {
        memset(out, 0, (size_t)nbytes);  /* zeros where nothing (or a shorter packet) arrived */
        count = r->latest_len;
    }
    memcpy(out, r->latest, (size_t)count);  /* little-endian on both ends */
    if (age_seconds) *age_seconds = r->latest_time < 0 ? -1.0 : spot_udp_now() - r->latest_time;
    fresh = (int)(r->arrived - r->handed_out);
    r->handed_out = r->arrived;
    lock_give(&r->lock);
    return fresh;
}

/* ------------------------------------------------------------------ sending */

int spot_udp_tx_open(const char *ip, int port)
{
    struct sockaddr_in to;
    int on = 1, i;
    sender *s = NULL;

    for (i = 0; i < MAX_TX; i++)
        if (tx[i].open && tx[i].port == port && strcmp(tx[i].ip, ip) == 0)
            return i;
    for (i = 0; !s && i < MAX_TX; i++)
        if (!tx[i].open) s = &tx[i];
    if (!s) {
        fprintf(stderr, "spot_udp: too many destinations (at most %d)\n", MAX_TX);
        return -1;
    }
    if (network_startup() != 0) return -1;

    s->sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (s->sd == INVALID_SOCKET) return -1;
    setsockopt(s->sd, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));
#ifdef _WIN32
    {
        u_long nonblocking = 1;
        ioctlsocket(s->sd, FIONBIO, &nonblocking);
    }
#endif
    memset(&to, 0, sizeof(to));
    to.sin_family = AF_INET;
    to.sin_port = htons((unsigned short)port);
    to.sin_addr.s_addr = inet_addr(ip);
    /* A connected socket: send() needs no address, and errors from this destination
       (e.g. "port unreachable") stay on this socket */
    if (connect(s->sd, (struct sockaddr *)&to, sizeof(to)) != 0) {
        fprintf(stderr, "spot_udp: cannot send to %s:%d\n", ip, port);
        closesocket(s->sd);
        return -1;
    }
    strncpy(s->ip, ip, sizeof(s->ip) - 1);
    s->ip[sizeof(s->ip) - 1] = '\0';
    s->port = port;
    s->open = 1;
    return (int)(s - tx);
}

int spot_udp_tx_send(int handle, const void *data, int nbytes)
{
    if (handle < 0 || handle >= MAX_TX || !tx[handle].open) return -1;
#ifdef _WIN32
    return send(tx[handle].sd, (const char *)data, nbytes, 0);
#else
    return (int)send(tx[handle].sd, data, (size_t)nbytes, MSG_DONTWAIT);
#endif
}

void spot_udp_close_all(void)
{
    int i;
    for (i = 0; i < MAX_RX; i++) rx_close(&rx[i]);
    for (i = 0; i < MAX_TX; i++) {
        if (tx[i].open) closesocket(tx[i].sd);
        tx[i].open = 0;
    }
#ifdef _WIN32
    if (network_started) WSACleanup();
#endif
    network_started = 0;
}
