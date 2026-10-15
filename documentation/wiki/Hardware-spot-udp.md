# spot_udp.c

**Files:** `+spot/+hardware/csrc/udp/spot_udp.c`, `spot_udp.h` · **MATLAB side:** [[spot.hardware.Udp|spot-hardware-Udp]]

<!-- source: +spot/+hardware/csrc/udp/spot_udp.h -->
<!-- source: +spot/+hardware/csrc/udp/spot_udp.c -->

## What it is for

All the UDP in the program that runs on a platform: the PhaseSpace stream in, the Orin link in and out, the clocks, messages and live streams out to the lab console. Two ideas:

- **Receiving keeps only the newest packet.** Simulink's own *UDP Receive* block returns the *oldest* queued packet each step, so once the sender is faster than the model the queue fills and the model reads stale data for the rest of the run. Here a background thread per port reads every packet the moment it arrives and keeps the latest, with its arrival time.
- **Sending never waits.** One socket per destination, non-blocking sends, so a computer that is off the network can't hold up the model or the other destinations.

Opening is idempotent: calling an open function again with the same arguments returns the same handle, so the MATLAB side simply calls it every step. Linux (the Jetsons) and Windows.

## The API (`spot_udp.h`)

| Function | |
|---|---|
| `spot_udp_rx_open(port, allowed_ip, min_bytes, max_bytes)` | listen on a port, accepting only that sender (`""`/`"0.0.0.0"`: any) and only packets within the size limits (0 = no limit; min = max = 460 accepts exactly the PhaseSpace layout); returns a handle or −1 |
| `spot_udp_rx_read(handle, out, nbytes, &age)` | copy the newest packet's first `nbytes` into `out` (zeros where nothing arrived), set its age in seconds (−1: none yet), return how many packets arrived since the last read |
| `spot_udp_tx_open(ip, port)` | a socket that sends to `ip:port` (broadcast allowed) |
| `spot_udp_tx_send(handle, data, nbytes)` | one packet, without waiting; returns bytes sent or −1 |
| `spot_udp_now()` | a steady clock in seconds (intervals, not time of day) |
| `spot_udp_close_all()` | stop every thread and close every socket |

## Function by function (`spot_udp.c`)

**Lines 8–36.** Platform shims: Winsock vs POSIX sockets, Windows threads/critical sections vs pthreads/mutexes, behind the same macro names (`lock_take`, `thread_t`, …).

**Lines 38–69.** Limits: 64 KB packets, 8 receivers, 16 senders. A `receiver` holds its socket, thread, lock and settings, and under the lock the latest packet, its length and arrival time, and two counters (`arrived`, `handed_out`) whose difference is "packets since the last read". A `sender` is an address, port and socket. Static tables of both.

**Lines 71–81.** `network_startup`: Winsock initialisation, once; nothing on Linux.

**Lines 83–96.** `spot_udp_now`: the performance counter on Windows, `CLOCK_MONOTONIC` on Linux.

**Lines 100–133.** `receive_loop`, one thread per port: `recvfrom` with a 200 ms timeout (so the loop notices `stop`); drop packets from the wrong sender or of the wrong size; under the lock copy the packet, stamp the time, count it.

**Lines 135–148.** `rx_close`: ask the thread to stop, join it, close the socket, free the lock.

**Lines 150–222.** `spot_udp_rx_open`: if the port is already open with the same settings, return that handle; with different settings, close and reopen it; otherwise take a free slot. Make a UDP socket with address reuse, a 1 MB receive buffer and the 200 ms timeout; bind to the port on all interfaces (a clear message if the port is taken); fill in the settings (`allowed` is the sender's address in network byte order, 0 for anyone); start the thread.

**Lines 224–246.** `spot_udp_rx_read`: a bad handle gives zeros and −1. Under the lock: zero-fill if the caller wants more than arrived, copy the bytes (little-endian on both ends, so a `float` array can be copied straight into a MATLAB `single` array), compute the age, and return the fresh count while moving the mark.

**Lines 250–292.** `spot_udp_tx_open`: reuse a sender with the same address and port, else a free slot; a UDP socket with broadcast allowed (and non-blocking mode on Windows); `connect` it to the destination, so `send` needs no address and errors from that destination (e.g. "port unreachable") stay on that socket.

**Lines 294–302.** `spot_udp_tx_send`: `send` with `MSG_DONTWAIT` on Linux (the socket is non-blocking on Windows).

**Lines 304–316.** `spot_udp_close_all`: close everything, Winsock cleanup.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/csrc/udp/spot_udp.h</b> (51 lines; click to show)</summary>

```c
   1  /* spot_udp.h -- the UDP used by the spacecraft program: receive the newest packet, send without
   2   * blocking. Every UDP link in the diagram goes through here (spot.hardware.udpReceive,
   3   * spot.hardware.udpSend, spot.hardware.phasespaceLatest).
   4   *
   5   * Receiving: Simulink's UDP Receive block returns the OLDEST queued packet each model step. When
   6   * the sender is faster than the model (or the model once fell behind), the queue fills and the
   7   * model reads data many steps old for the rest of the run. Here a background thread per port
   8   * reads every packet as it arrives and keeps only the newest, with its age.
   9   *
  10   * Sending: one socket per destination, and a send never waits, so a destination that is off
  11   * the network can't hold up the model or the other destinations.
  12   *
  13   * Linux (the Jetsons) and Windows. Opening is idempotent: calling an open function again with the
  14   * same arguments returns the same handle, so the MATLAB side can simply call it every step.
  15   */
  16  #ifndef SPOT_UDP_H
  17  #define SPOT_UDP_H
  18  
  19  #ifdef __cplusplus
  20  extern "C" {
  21  #endif
  22  
  23  /* Listen on UDP <port> (all interfaces) and keep the newest packet. Returns a handle >= 0, or -1.
  24   *   allowed_ip  only accept packets from this sender ("" or "0.0.0.0": any sender)
  25   *   min_bytes   ignore shorter packets (0: no limit)
  26   *   max_bytes   ignore longer packets (0: no limit); min = max = 460 accepts only that size */
  27  int spot_udp_rx_open(int port, const char *allowed_ip, int min_bytes, int max_bytes);
  28  
  29  /* Copy the newest packet's first nbytes into out (zeros where nothing has arrived yet, or past
  30   * the end of a shorter packet). age_seconds: time since it arrived, -1 if nothing yet.
  31   * Returns how many packets arrived since the previous read (0 = nothing new). */
  32  int spot_udp_rx_read(int handle, void *out, int nbytes, double *age_seconds);
  33  
  34  /* A socket that sends to ip:port (a broadcast address such as 255.255.255.255 is allowed).
  35   * Returns a handle >= 0, or -1. */
  36  int spot_udp_tx_open(const char *ip, int port);
  37  
  38  /* Send nbytes from data as one packet, without waiting. Returns the bytes sent, or -1. */
  39  int spot_udp_tx_send(int handle, const void *data, int nbytes);
  40  
  41  /* A steady clock in seconds (not the time of day; for measuring intervals). */
  42  double spot_udp_now(void);
  43  
  44  /* Stop every receive thread and close every socket. */
  45  void spot_udp_close_all(void);
  46  
  47  #ifdef __cplusplus
  48  }
  49  #endif
  50  
  51  #endif /* SPOT_UDP_H */
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/udp/spot_udp.c</b> (316 lines; click to show)</summary>

```c
   1  /* spot_udp.c -- see spot_udp.h. */
   2  
   3  #include "spot_udp.h"
   4  
   5  #include <stdio.h>
   6  #include <string.h>
   7  
   8  #ifdef _WIN32
   9  #include <winsock2.h>
  10  #include <ws2tcpip.h>
  11  #include <windows.h>
  12  #pragma comment(lib, "ws2_32.lib")
  13  typedef HANDLE thread_t;
  14  typedef CRITICAL_SECTION lock_t;
  15  #define lock_init(l)    InitializeCriticalSection(l)
  16  #define lock_free(l)    DeleteCriticalSection(l)
  17  #define lock_take(l)    EnterCriticalSection(l)
  18  #define lock_give(l)    LeaveCriticalSection(l)
  19  #else
  20  #include <arpa/inet.h>
  21  #include <netinet/in.h>
  22  #include <pthread.h>
  23  #include <sys/socket.h>
  24  #include <sys/time.h>
  25  #include <time.h>
  26  #include <unistd.h>
  27  typedef int SOCKET;
  28  #define INVALID_SOCKET (-1)
  29  #define closesocket close
  30  typedef pthread_t thread_t;
  31  typedef pthread_mutex_t lock_t;
  32  #define lock_init(l)    pthread_mutex_init(l, NULL)
  33  #define lock_free(l)    pthread_mutex_destroy(l)
  34  #define lock_take(l)    pthread_mutex_lock(l)
  35  #define lock_give(l)    pthread_mutex_unlock(l)
  36  #endif
  37  
  38  #define MAX_PACKET 65536
  39  #define MAX_RX 8
  40  #define MAX_TX 16
  41  
  42  typedef struct {
  43      int open;
  44      int port, min_bytes, max_bytes;
  45      unsigned long allowed;      /* sender address in network byte order, 0 = anyone */
  46      char allowed_text[64];
  47      SOCKET sd;
  48      thread_t thread;
  49      lock_t lock;
  50      volatile int stop;
  51  
  52      /* protected by lock */
  53      unsigned char latest[MAX_PACKET];
  54      int latest_len;
  55      double latest_time;         /* spot_udp_now() when it arrived, < 0 = nothing yet */
  56      unsigned long arrived;      /* packets accepted so far */
  57      unsigned long handed_out;   /* value of 'arrived' at the previous read */
  58  } receiver;
  59  
  60  typedef struct {
  61      int open;
  62      char ip[64];
  63      int port;
  64      SOCKET sd;
  65  } sender;
  66  
  67  static receiver rx[MAX_RX];
  68  static sender tx[MAX_TX];
  69  static int network_started;
  70  
  71  static int network_startup(void)
  72  {
  73  #ifdef _WIN32
  74      if (!network_started) {
  75          WSADATA w;
  76          if (WSAStartup(MAKEWORD(2, 2), &w) != 0) return -1;
  77      }
  78  #endif
  79      network_started = 1;
  80      return 0;
  81  }
  82  
  83  double spot_udp_now(void)
  84  {
  85  #ifdef _WIN32
  86      static LARGE_INTEGER freq;
  87      LARGE_INTEGER t;
  88      if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
  89      QueryPerformanceCounter(&t);
  90      return (double)t.QuadPart / (double)freq.QuadPart;
  91  #else
  92      struct timespec t;
  93      clock_gettime(CLOCK_MONOTONIC, &t);
  94      return t.tv_sec + t.tv_nsec * 1e-9;
  95  #endif
  96  }
  97  
  98  /* ------------------------------------------------------------------ receiving */
  99  
 100  /* Background thread for one port: read every packet the moment it arrives, keep the newest. */
 101  #ifdef _WIN32
 102  static DWORD WINAPI receive_loop(LPVOID arg)
 103  #else
 104  static void *receive_loop(void *arg)
 105  #endif
 106  {
 107      receiver *r = (receiver *)arg;
 108      static unsigned char buffers[MAX_RX][MAX_PACKET];
 109      unsigned char *buffer = buffers[r - rx];
 110      while (!r->stop) {
 111          struct sockaddr_in from;
 112  #ifdef _WIN32
 113          int fromlen = sizeof(from);
 114  #else
 115          socklen_t fromlen = sizeof(from);
 116  #endif
 117          int n = recvfrom(r->sd, (char *)buffer, MAX_PACKET, 0, (struct sockaddr *)&from, &fromlen);
 118          if (n <= 0)
 119              continue;  /* timeout (lets the loop see r->stop) or a transient error */
 120          if (r->allowed && from.sin_addr.s_addr != r->allowed)
 121              continue;  /* not from the expected sender */
 122          if ((r->min_bytes && n < r->min_bytes) || (r->max_bytes && n > r->max_bytes))
 123              continue;  /* wrong format, e.g. an old program still sending another layout */
 124  
 125          lock_take(&r->lock);
 126          memcpy(r->latest, buffer, (size_t)n);
 127          r->latest_len = n;
 128          r->latest_time = spot_udp_now();
 129          r->arrived++;
 130          lock_give(&r->lock);
 131      }
 132      return 0;
 133  }
 134  
 135  static void rx_close(receiver *r)
 136  {
 137      if (!r->open) return;
 138      r->stop = 1;
 139  #ifdef _WIN32
 140      WaitForSingleObject(r->thread, INFINITE);
 141      CloseHandle(r->thread);
 142  #else
 143      pthread_join(r->thread, NULL);
 144  #endif
 145      closesocket(r->sd);
 146      lock_free(&r->lock);
 147      r->open = 0;
 148  }
 149  
 150  int spot_udp_rx_open(int port, const char *allowed_ip, int min_bytes, int max_bytes)
 151  {
 152      struct sockaddr_in local;
 153      int on = 1, bufsize = 1 << 20, i;
 154      const char *ip = allowed_ip ? allowed_ip : "";
 155      receiver *r = NULL;
 156  
 157      for (i = 0; i < MAX_RX; i++) {
 158          if (rx[i].open && rx[i].port == port) {
 159              if (rx[i].min_bytes == min_bytes && rx[i].max_bytes == max_bytes && strcmp(rx[i].allowed_text, ip) == 0)
 160                  return i;  /* already listening exactly like this */
 161              rx_close(&rx[i]);  /* same port, new settings: listen again */
 162              r = &rx[i];
 163              break;
 164          }
 165      }
 166      for (i = 0; !r && i < MAX_RX; i++)
 167          if (!rx[i].open) r = &rx[i];
 168      if (!r) {
 169          fprintf(stderr, "spot_udp: too many receive ports (at most %d)\n", MAX_RX);
 170          return -1;
 171      }
 172      if (network_startup() != 0) return -1;
 173  
 174      r->sd = socket(AF_INET, SOCK_DGRAM, 0);
 175      if (r->sd == INVALID_SOCKET) return -1;
 176      setsockopt(r->sd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
 177      setsockopt(r->sd, SOL_SOCKET, SO_RCVBUF, (const char *)&bufsize, sizeof(bufsize));
 178      {
 179          /* Wake up regularly so rx_close() can stop the thread */
 180  #ifdef _WIN32
 181          DWORD timeout = 200;
 182  #else
 183          struct timeval timeout = { 0, 200000 };
 184  #endif
 185          setsockopt(r->sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
 186      }
 187  
 188      memset(&local, 0, sizeof(local));
 189      local.sin_family = AF_INET;
 190      local.sin_port = htons((unsigned short)port);
 191      local.sin_addr.s_addr = htonl(INADDR_ANY);
 192      if (bind(r->sd, (struct sockaddr *)&local, sizeof(local)) != 0) {
 193          fprintf(stderr, "spot_udp: cannot listen on UDP port %d (is another program using it?)\n", port);
 194          closesocket(r->sd);
 195          return -1;
 196      }
 197  
 198      r->port = port;
 199      r->min_bytes = min_bytes;
 200      r->max_bytes = max_bytes;
 201      strncpy(r->allowed_text, ip, sizeof(r->allowed_text) - 1);
 202      r->allowed_text[sizeof(r->allowed_text) - 1] = '\0';
 203      r->allowed = (ip[0] && strcmp(ip, "0.0.0.0") != 0) ? inet_addr(ip) : 0;
 204      r->latest_len = 0;
 205      r->latest_time = -1.0;
 206      r->arrived = r->handed_out = 0;
 207      r->stop = 0;
 208      lock_init(&r->lock);
 209  
 210  #ifdef _WIN32
 211      r->thread = CreateThread(NULL, 0, receive_loop, r, 0, NULL);
 212      if (!r->thread) {
 213  #else
 214      if (pthread_create(&r->thread, NULL, receive_loop, r) != 0) {
 215  #endif
 216          lock_free(&r->lock);
 217          closesocket(r->sd);
 218          return -1;
 219      }
 220      r->open = 1;
 221      return (int)(r - rx);
 222  }
 223  
 224  int spot_udp_rx_read(int handle, void *out, int nbytes, double *age_seconds)
 225  {
 226      receiver *r;
 227      int count = nbytes, fresh;
 228  
 229      if (handle < 0 || handle >= MAX_RX || !rx[handle].open) {
 230          memset(out, 0, (size_t)nbytes);
 231          if (age_seconds) *age_seconds = -1.0;
 232          return 0;
 233      }
 234      r = &rx[handle];
 235      lock_take(&r->lock);
 236      if (count > r->latest_len) {
 237          memset(out, 0, (size_t)nbytes);  /* zeros where nothing (or a shorter packet) arrived */
 238          count = r->latest_len;
 239      }
 240      memcpy(out, r->latest, (size_t)count);  /* little-endian on both ends */
 241      if (age_seconds) *age_seconds = r->latest_time < 0 ? -1.0 : spot_udp_now() - r->latest_time;
 242      fresh = (int)(r->arrived - r->handed_out);
 243      r->handed_out = r->arrived;
 244      lock_give(&r->lock);
 245      return fresh;
 246  }
 247  
 248  /* ------------------------------------------------------------------ sending */
 249  
 250  int spot_udp_tx_open(const char *ip, int port)
 251  {
 252      struct sockaddr_in to;
 253      int on = 1, i;
 254      sender *s = NULL;
 255  
 256      for (i = 0; i < MAX_TX; i++)
 257          if (tx[i].open && tx[i].port == port && strcmp(tx[i].ip, ip) == 0)
 258              return i;
 259      for (i = 0; !s && i < MAX_TX; i++)
 260          if (!tx[i].open) s = &tx[i];
 261      if (!s) {
 262          fprintf(stderr, "spot_udp: too many destinations (at most %d)\n", MAX_TX);
 263          return -1;
 264      }
 265      if (network_startup() != 0) return -1;
 266  
 267      s->sd = socket(AF_INET, SOCK_DGRAM, 0);
 268      if (s->sd == INVALID_SOCKET) return -1;
 269      setsockopt(s->sd, SOL_SOCKET, SO_BROADCAST, (const char *)&on, sizeof(on));
 270  #ifdef _WIN32
 271      {
 272          u_long nonblocking = 1;
 273          ioctlsocket(s->sd, FIONBIO, &nonblocking);
 274      }
 275  #endif
 276      memset(&to, 0, sizeof(to));
 277      to.sin_family = AF_INET;
 278      to.sin_port = htons((unsigned short)port);
 279      to.sin_addr.s_addr = inet_addr(ip);
 280      /* A connected socket: send() needs no address, and errors from this destination
 281         (e.g. "port unreachable") stay on this socket */
 282      if (connect(s->sd, (struct sockaddr *)&to, sizeof(to)) != 0) {
 283          fprintf(stderr, "spot_udp: cannot send to %s:%d\n", ip, port);
 284          closesocket(s->sd);
 285          return -1;
 286      }
 287      strncpy(s->ip, ip, sizeof(s->ip) - 1);
 288      s->ip[sizeof(s->ip) - 1] = '\0';
 289      s->port = port;
 290      s->open = 1;
 291      return (int)(s - tx);
 292  }
 293  
 294  int spot_udp_tx_send(int handle, const void *data, int nbytes)
 295  {
 296      if (handle < 0 || handle >= MAX_TX || !tx[handle].open) return -1;
 297  #ifdef _WIN32
 298      return send(tx[handle].sd, (const char *)data, nbytes, 0);
 299  #else
 300      return (int)send(tx[handle].sd, data, (size_t)nbytes, MSG_DONTWAIT);
 301  #endif
 302  }
 303  
 304  void spot_udp_close_all(void)
 305  {
 306      int i;
 307      for (i = 0; i < MAX_RX; i++) rx_close(&rx[i]);
 308      for (i = 0; i < MAX_TX; i++) {
 309          if (tx[i].open) closesocket(tx[i].sd);
 310          tx[i].open = 0;
 311      }
 312  #ifdef _WIN32
 313      if (network_started) WSACleanup();
 314  #endif
 315      network_started = 0;
 316  }
```

</details>
<!-- /listing -->
