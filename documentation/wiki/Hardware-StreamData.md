# StreamData.cc (PhaseSpace)

**Files:** `+spot/+console/phasespace/StreamData.cc`, `Makefile`, `build_linux.sh`, `libowl-src-410/` (PhaseSpace's client library source)

<!-- source: +spot/+console/phasespace/StreamData.cc -->
<!-- source: +spot/+console/phasespace/Makefile -->
<!-- source: +spot/+console/phasespace/build_linux.sh -->

## What it is for

The program the lab console runs to turn PhaseSpace's motion capture into the packets the platforms consume. It connects to the PhaseSpace server (192.168.1.109), defines the three rigid bodies from the LED calibration file, receives frames at the requested rate, computes each platform's position, attitude (unwrapped) and velocity, adds every LED's position and the experiment clock, and sends one UDP packet per frame to every destination: the three platforms (`192.168.1.110–112`), the fixed extras, a local copy for the console's 3D view, and any address added at run time.

Started by the lab console's *Start Streaming* ([[phasespace.py|Console-phasespace]]); compiled by its *Compile* button or `build_linux.sh`.

## The packet

115 × `float` (460 bytes), sent from port 31535 to port 31534:

| Index (0-based) | |
|---|---|
| 0 | time since streaming started [s] |
| 1–3, 4–6, 7–9 | RED, BLACK, BLUE: x [mm], y [mm], attitude [rad, unwrapped] |
| 10–12, 13–15, 16–18 | RED, BLACK, BLUE: ẋ, ẏ, angular rate |
| 19 + 3k … 21 + 3k | LED k's x, y, z (NaN when not seen), k = 0…31 |
| 112 (LED 31's x) | the experiment clock: NaN (not armed), negative (counting down), ≥ 0 (seconds since the start) |

[[spot.hardware.phasespaceLatest|spot-hardware-phasespaceLatest]] receives it; [[spot.hardware.experimentTime|spot-hardware-experimentTime]] reads slot 113 (1-based).

## Usage

```
StreamData <frequency> [monitor_port] [--udp] [--dest IP ...]
```

and, on standard input while running: `dest add 192.168.1.43`, `dest remove …`, `exp start 3` (the experiment starts in 3 s), `exp clear`.

## Section by section

**Lines 19–127.** Network wrappers for Windows and Linux. `Destination` (85–90) is one UDP socket per receiver: with a single shared socket, packets for a computer that is off (whose address never resolves, ~3 s timeout each) filled the buffer and 99.7 % of packets to the live destinations were lost. `openDestination` (100–127): a socket with a 1 MB send buffer, bound to the fixed source port 31535 (in case a receiver filters on it), connected to the destination, non-blocking.

**Lines 149–174.** The packet layout above as constants; marker ids ≥ 24 are refused by the server, so only 0–23 go into the point tracker; `EXP_CLOCK_INDEX` = 19 + 3·31 = 112.

**Lines 180–216.** `main`: parse the arguments; unbuffered stdout (the console reads it live); Ctrl+C / SIGTERM (and SIGBREAK on Windows) request a clean stop.

**Lines 219–293.** The destinations: the three platforms plus two fixed extras, the local monitor copy if a port was given, and the `--dest` extras; fail if any socket can't be made; print where packets go.

**Lines 298–348.** The command thread: reads stdin lines; `exp start <0–600 s>` / `exp clear` set a pending command for the frame loop (under a mutex); `dest add/remove <IPv4>` open or close an extra destination (under the destinations mutex).

**Lines 355–421.** PhaseSpace setup: the server address, the OWL context, and the LED calibration: defaults for all twelve LEDs, then `tracker_positions_phasespace.txt` in the working folder overrides them (the Experiment tab sends this file before starting the cameras; [[spot.massprops.trackerPositions|spot-massprops-trackerPositions]] writes it).

**Lines 424–433.** Connect and initialise; frame timestamps in microseconds from the server's own clock (frames arrive in bursts, and dividing by the arrival gaps made the velocities spike).

**Lines 435–476.** The rigid bodies: tracker 0 "RED_rigid" with LEDs 4, 2, 6, 0 (two LEDs report under different ids than their labels, as the comments note), tracker 2 "BLACK_rigid" with 10, 14, 13, 12, tracker 3 "BLUE_rigid" with 19, 21, 23, 17, each LED at its calibrated offset from the CG; and a point tracker with every other LED below 24, so single LEDs are reported too.

**Lines 479–482.** The frame rate, and streaming over TCP (default) or UDP (`--udp`: a late frame is dropped instead of delaying the ones behind it).

**Lines 484–493.** Clocks and counters for the status line.

**Lines 496–771.** The main loop, one iteration per event:
- 501–507: an OWL error is printed; only a fatal one stops the program (a rejected marker assignment used to stop all output silently).
- 510–536: the frame time from PhaseSpace's timestamp (or this computer's clock if there is none), restarted at 0 on the first frame or when the time source changes (an armed experiment clock is shifted along); the first frame prints the actual rate.
- 539–702: for each tracked rigid body: x, y; the yaw from the quaternion (`atan2(2(q_y q_z + q_w q_x), 2q_w² − 1 + 2q_x²)`, the server's axis convention); **unwrapping**: the change from the previous raw angle is brought into (−π, π] and accumulated, so the attitude is continuous through full turns; the velocities as finite differences over the frame time (only once there is an earlier frame); the previous values are kept per body.
- 704–720: every LED's slot is set to NaN, then the visible ones get their coordinates.
- 722–735: the experiment clock: a pending `start` sets the agreed start time to now + delay; `clear` ends it; the slot carries NaN or the time since the start.
- 737–746: convert to `float` and send to every destination, counting failures.
- 748–770: every 5 s a status line: frames per second from PhaseSpace, how often each body was tracked, the experiment clock, and undelivered packet counts per destination.

**Lines 774–781.** Clean shutdown: close the PhaseSpace connection and the sockets.

## Building

`Makefile`: builds `libowlsock.so` from PhaseSpace's sources in `libowl-src-410` and links `StreamData` against it with `$ORIGIN` as the run path, in `~/.cache/streamdata-build/<arch>` (outside Dropbox and free of spaces), copying the results to `../output/linux-<arch>/` (or `OUT=`). `build_linux.sh` runs `make -j`. Needs only `g++` and `make`; works on x86 and the TX2's ARM.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/phasespace/StreamData.cc</b> (782 lines; click to show)</summary>

```cpp
   1  ﻿// example_rigid1.cc -*- C++ -*-
   2  // simple rigid tracking program
   3  
   4  /***
   5  Copyright (c) PhaseSpace, Inc 2017
   6  
   7  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   8  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
   9  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL PHASESPACE, INC
  10  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  11  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  12  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  13  ***/
  14  
  15  /* Builds on Windows (MSVC, links ws2_32.lib) and Linux (g++). See CMakeLists.txt. */
  16  //#include "stdafx.h"
  17  
  18  //!!!!!!!!!NETWORK CODE!!!!!!!!!!!!!!!
  19  #ifdef _WIN32
  20  #pragma comment (lib, "ws2_32.lib")
  21  #define _CRT_SECURE_NO_WARNINGS
  22  #ifndef WIN32
  23  #define WIN32 /* owl.hpp keys dllimport and its "#undef ERROR" off WIN32, not _WIN32 */
  24  #endif
  25  #endif
  26  #include <stdio.h>
  27  #include <stdlib.h>
  28  #include <string.h>
  29  #include <stdint.h>
  30  #ifdef _WIN32
  31  #include <winsock2.h>
  32  #include <ws2tcpip.h>
  33  #include <windows.h>
  34  #else
  35  #include <sys/types.h>
  36  #include <sys/socket.h>
  37  #include <netinet/in.h>
  38  #include <arpa/inet.h>
  39  #include <fcntl.h>
  40  #include <unistd.h>
  41  typedef int SOCKET;
  42  #define INVALID_SOCKET (-1)
  43  #define closesocket close
  44  #endif
  45  #include <time.h>
  46  #include <chrono>
  47  #include <csignal>
  48  #include <string>
  49  #include <mutex>
  50  #include <thread>
  51  #define BUFFER_SIZE 4096
  52  
  53  /* Platform wrappers: Winsock needs explicit start-up/clean-up, POSIX sockets do not. */
  54  static bool networkStartup()
  55  {
  56  #ifdef _WIN32
  57      WSADATA w;
  58      return WSAStartup(MAKEWORD(2, 2), &w) == 0;
  59  #else
  60      return true;
  61  #endif
  62  }
  63  
  64  static void networkCleanup()
  65  {
  66  #ifdef _WIN32
  67      WSACleanup();
  68  #endif
  69  }
  70  
  71  static void setNonBlocking(SOCKET sd)
  72  {
  73  #ifdef _WIN32
  74      u_long mode = 1;
  75      ioctlsocket(sd, FIONBIO, &mode);
  76  #else
  77      fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK);
  78  #endif
  79  }
  80  
  81  /* Every destination gets its own UDP socket. With a single shared socket, packets waiting
  82     for a computer that is switched off (its address never resolves, which takes ~3 s to time
  83     out) filled the shared send buffer, and the sends to every other computer then failed
  84     silently: in testing, 99.7% of packets to a live destination were lost that way. */
  85  struct Destination {
  86      std::string address;
  87      SOCKET sd;
  88      unsigned long failed; /* packets not handed to the network since the last status line */
  89      bool extra;           /* added with --dest or "dest add" (can be removed again) */
  90  };
  91  
  92  /* Dotted IPv4 address such as 192.168.1.43 */
  93  static bool isIPv4(const std::string &s)
  94  {
  95      unsigned a, b, c, d;
  96      char end;
  97      return sscanf(s.c_str(), "%u.%u.%u.%u%c", &a, &b, &c, &d, &end) == 4 && a < 256 && b < 256 && c < 256 && d < 256;
  98  }
  99  
 100  static SOCKET openDestination(const std::string &address, unsigned short port, unsigned short source_port)
 101  {
 102      SOCKET sd = socket(AF_INET, SOCK_DGRAM, 0);
 103      if (sd == INVALID_SOCKET)
 104          return sd;
 105  
 106      int on = 1, bufferSize = 1 << 20;
 107      setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
 108      setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char*)&bufferSize, sizeof(bufferSize));
 109  
 110      /* Keep sending from the same source port as before, in case a receiver filters on it */
 111      struct sockaddr_in local;
 112      memset(&local, 0, sizeof(local));
 113      local.sin_family = AF_INET;
 114      local.sin_port = htons(source_port);
 115      local.sin_addr.s_addr = htonl(INADDR_ANY);
 116      bind(sd, (struct sockaddr*)&local, sizeof(local));
 117  
 118      struct sockaddr_in remote;
 119      memset(&remote, 0, sizeof(remote));
 120      remote.sin_family = AF_INET;
 121      remote.sin_port = htons(port);
 122      remote.sin_addr.s_addr = inet_addr(address.c_str());
 123      connect(sd, (struct sockaddr*)&remote, sizeof(remote));
 124  
 125      setNonBlocking(sd);
 126      return sd;
 127  }
 128  //!!!!!!!!!NETWORK CODE!!!!!!!!!!!!!!!/
 129  
 130  #include <iostream>
 131  #include <vector>
 132  #include <cstring>
 133  #include <unordered_map>
 134  #include <fstream>
 135  #include <sstream>
 136  #include <cmath>
 137  #include <limits>
 138  #include <algorithm>
 139  #include <iterator>
 140  
 141  #include "libowl-src-410/owl.hpp" /* PhaseSpace OWL C++ API v2.2, same source as libowlsock.so */
 142  
 143  //#include <ctime>
 144  
 145  using namespace std;
 146  
 147  typedef std::chrono::high_resolution_clock Clock;
 148  
 149  /* UDP packet layout (every element is a PacketScalar, 0-based indices):
 150       [0]                   time since streaming started (s)
 151       [1..3]                RED   x, y, attitude
 152       [4..6]                BLACK x, y, attitude
 153       [7..9]                BLUE  x, y, attitude
 154       [10..12]              RED   vx, vy, angular rate
 155       [13..15]              BLACK vx, vy, angular rate
 156       [16..18]              BLUE  vx, vy, angular rate
 157       [19+3k .. 21+3k]      LED k x, y, z for k = 0..NUM_LEDS-1 (NaN if LED k is not visible this frame)
 158       [112]                 (LED 31's x) the experiment clock instead: NaN when no experiment is armed,
 159                             negative while counting down to the agreed start, then seconds since the start
 160     Calculations are done in double; the packet is only converted to PacketScalar when sent.
 161     Change PacketScalar to double for full precision at twice the packet size. */
 162  typedef float PacketScalar;
 163  const int NUM_RIGID_FIELDS = 19;
 164  const int NUM_LEDS = 32;
 165  /* The PhaseSpace server rejects marker ids >= 24 ("assignMarker: invalid marker id=24"),
 166     so only ids below this are put in the point tracker. Their packet slots stay NaN. */
 167  const int NUM_SERVER_MARKERS = 24;
 168  const int PACKET_LENGTH = NUM_RIGID_FIELDS + 3 * NUM_LEDS;
 169  /* The experiment clock: when the lab console starts an experiment it waits until every platform's
 170     program is running, then sends "exp start <seconds>" on standard input. From then on every
 171     packet carries the time since that agreed start in this slot (LED 31's x; the server never
 172     reports LED 31, see NUM_SERVER_MARKERS), so all platforms start and switch phases together,
 173     whenever each program was started (spot.hardware.experimentTime reads it). "exp clear" ends it. */
 174  const int EXP_CLOCK_INDEX = NUM_RIGID_FIELDS + 3 * 31;
 175  
 176  /* Set by Ctrl+C (or the webapp's Stop button) so the main loop can shut down cleanly. */
 177  static volatile std::sig_atomic_t stopRequested = 0;
 178  static void requestStop(int) { stopRequested = 1; }
 179  
 180  int main(int argc, const char **argv)
 181  {
 182      /* Usage: StreamData <frequency> [monitor_port] [--udp] [--dest IP ...]
 183           monitor_port  also send a copy of every packet to this port on 127.0.0.1, for the
 184                         webapp viewer or Python/udp_listener.py (e.g. "StreamData 100 31540")
 185           --udp         receive frames from PhaseSpace over UDP instead of TCP: a late frame is
 186                         dropped instead of holding up the frames behind it
 187           --dest IP     also send to this computer (port 31534); can be repeated
 188         While running, lines on standard input add or remove extra destinations, and start or end
 189         the experiment clock (see EXP_CLOCK_INDEX):
 190           dest add 192.168.1.43        dest remove 192.168.1.43
 191           exp start 3                  exp clear */
 192      std::vector<std::string> positional, extra_addresses;
 193      bool useUdp = false;
 194      for (int i = 1; i < argc; i++) {
 195          if (strcmp(argv[i], "--udp") == 0) useUdp = true;
 196          else if (strcmp(argv[i], "--dest") == 0 && i + 1 < argc) {
 197              if (isIPv4(argv[++i])) extra_addresses.push_back(argv[i]);
 198              else fprintf(stderr, "Ignoring --dest %s: not an IPv4 address.\n", argv[i]);
 199          }
 200          else positional.push_back(argv[i]);
 201      }
 202  	if (positional.empty()) {
 203  		// print error message if no value is provided
 204  		std::cerr << "Error: no frequency value provided" << std::endl;
 205  		std::cerr << "Usage: StreamData <frequency> [monitor_port] [--udp] [--dest IP ...]" << std::endl;
 206  		return 1; // exit program with error code
 207  	}
 208      int frequency = atoi(positional[0].c_str());
 209      int monitor_port = (positional.size() >= 2) ? atoi(positional[1].c_str()) : 0;
 210  
 211      setvbuf(stdout, NULL, _IONBF, 0); // print immediately, even when the output goes to the webapp
 212      std::signal(SIGINT, requestStop);
 213      std::signal(SIGTERM, requestStop);
 214  #ifdef SIGBREAK
 215      std::signal(SIGBREAK, requestStop); // Ctrl+Break, used by the webapp's Stop button on Windows
 216  #endif
 217  
 218  
 219      unsigned short port_number = 31535;	/* Port the packets are sent from */
 220      unsigned short client_port = 31534;	/* Port the packets are sent to */
 221      double dataPacket[PACKET_LENGTH] = { 0 };		/* Create array for data storage */
 222      PacketScalar sendPacket[PACKET_LENGTH];			/* dataPacket converted to the wire format */
 223      const double LED_NOT_VISIBLE = std::numeric_limits<double>::quiet_NaN();
 224      const double TWO_PI = 2.0 * 3.14159265358979323846;
 225      const double PI = 3.14159265358979323846;
 226  
 227      // Set up the previous values for velocity
 228      double prevServX = 0.0;
 229      double prevServY = 0.0;
 230      double prevServAtt = 0.0;
 231  
 232      double prevClientX = 0.0;
 233      double prevClientY = 0.0;
 234      double prevClientAtt = 0.0;
 235  
 236      double prevObstX = 0.0;
 237      double prevObstY = 0.0;
 238      double prevObstAtt = 0.0;
 239  
 240      // Time of each body's previous valid frame (-1 = none yet, so no velocity is computed)
 241      double prevTimeRED = -1.0;
 242      double prevTimeBLACK = -1.0;
 243      double prevTimeBLUE = -1.0;
 244  
 245  
 246      /* Open network connection (Winsock start-up on Windows, no-op on Linux) */
 247      if (!networkStartup())
 248      {
 249          //fprintf(stderr, "Could not open Windows connection.\n");
 250          ::exit(0);
 251      }
 252  
 253      /* Below is the IP address for the UDP client. */
 254      std::vector<std::string> client_addresses = { "192.168.1.110", "192.168.1.111", "192.168.1.112", "192.168.1.104", "192.168.1.115"};
 255  
 256      /* static: also used by the command thread below, which may outlive main's locals at exit */
 257      static std::vector<Destination> destinations;
 258      static std::mutex destinationsLock;
 259      /* The experiment clock's pending command, from the command thread to the frame loop */
 260      static std::mutex expLock;
 261      static int expCommand = 0;          /* 1 = start in expDelay s, 2 = clear */
 262      static double expDelay = 0.0;
 263      for (const auto& address : client_addresses)
 264          destinations.push_back({ address, openDestination(address, client_port, port_number), 0, false });
 265      if (monitor_port > 0) /* local copy for the viewer */
 266          destinations.push_back({ "127.0.0.1:" + std::to_string(monitor_port),
 267                                   openDestination("127.0.0.1", (unsigned short)monitor_port, port_number), 0, false });
 268      for (const auto& address : extra_addresses)
 269      {
 270          bool known = false;
 271          for (const auto& d : destinations) known = known || d.address == address;
 272          if (!known)
 273              destinations.push_back({ address, openDestination(address, client_port, port_number), 0, true });
 274      }
 275      for (const auto& d : destinations)
 276      {
 277          if (d.sd == INVALID_SOCKET)
 278          {
 279              fprintf(stderr, "Could not create a socket for %s.\n", d.address.c_str());
 280              networkCleanup();
 281              ::exit(0);
 282          }
 283      }
 284  
 285      /* Print out server information */
 286      printf("Sending from port %u to port %u on:", (unsigned)port_number, (unsigned)client_port);
 287      for (const auto& address : client_addresses)
 288          printf(" %s", address.c_str());
 289      printf("\n");
 290      for (const auto& d : destinations)
 291          if (d.extra) printf("Also sending to %s (added destination).\n", d.address.c_str());
 292      if (monitor_port > 0)
 293          printf("Also sending a copy of every packet to 127.0.0.1:%d (viewer).\n", monitor_port);
 294  
 295      /* Commands on standard input while streaming (the webapp uses these, so nothing has to
 296         restart): add or remove extra destinations ("dest add IP" / "dest remove IP"), and start
 297         or end the experiment clock ("exp start SECONDS" / "exp clear"). */
 298      std::thread([client_port, port_number]() {
 299          std::string line;
 300          while (std::getline(std::cin, line))
 301          {
 302              std::istringstream words(line);
 303              std::string cmd, action, ip;
 304              words >> cmd >> action;
 305              if (cmd == "exp")
 306              {
 307                  double delay = -1.0;
 308                  if (action == "start" && (words >> delay) && delay >= 0.0 && delay <= 600.0)
 309                  {
 310                      std::lock_guard<std::mutex> guard(expLock);
 311                      expCommand = 1;
 312                      expDelay = delay;
 313                  }
 314                  else if (action == "clear")
 315                  {
 316                      std::lock_guard<std::mutex> guard(expLock);
 317                      expCommand = 2;
 318                  }
 319                  else
 320                      printf("Ignored command: %s (expected: exp start <0-600 s> | exp clear)\n", line.c_str());
 321                  continue;
 322              }
 323              words >> ip;
 324              if (cmd != "dest" || (action != "add" && action != "remove") || !isIPv4(ip))
 325              {
 326                  if (!line.empty()) printf("Ignored command: %s (expected: dest add|remove <IPv4>, exp start <s>, exp clear)\n", line.c_str());
 327                  continue;
 328              }
 329              std::lock_guard<std::mutex> guard(destinationsLock);
 330              auto it = std::find_if(destinations.begin(), destinations.end(),
 331                                     [&ip](const Destination &d) { return d.address == ip; });
 332              if (action == "add")
 333              {
 334                  if (it != destinations.end()) { printf("Already sending to %s.\n", ip.c_str()); continue; }
 335                  SOCKET sd = openDestination(ip, client_port, port_number);
 336                  if (sd == INVALID_SOCKET) { printf("Could not create a socket for %s.\n", ip.c_str()); continue; }
 337                  destinations.push_back({ ip, sd, 0, true });
 338                  printf("Now also sending to %s.\n", ip.c_str());
 339              }
 340              else
 341              {
 342                  if (it == destinations.end() || !it->extra) { printf("%s is not an added destination.\n", ip.c_str()); continue; }
 343                  closesocket(it->sd);
 344                  destinations.erase(it);
 345                  printf("Stopped sending to %s.\n", ip.c_str());
 346              }
 347          }
 348      }).detach();
 349  
 350      printf("Use the ON/OFF Toggle in the SPOT Application to shut down cleanly. This console can be minimized.\n");
 351      printf("Sending %d x %u-byte values (%u bytes) per frame.\n",
 352          PACKET_LENGTH, (unsigned)sizeof(PacketScalar), (unsigned)(PACKET_LENGTH * sizeof(PacketScalar)));
 353  
 354      /* This is where the PhaseSpace code begins. */
 355      string address = "192.168.1.109";
 356      OWL::Context owl;
 357      OWL::Markers markers;
 358      OWL::Rigids rigids;
 359  
 360      std::string phaseSpaceOptions;
 361      std::string tracker_id_RED_5_pos_string, tracker_id_RED_3_pos_string;
 362      std::string tracker_id_RED_7_pos_string, tracker_id_RED_4_pos_string;
 363      std::string tracker_id_BLACK_13_pos_string, tracker_id_BLACK_11_pos_string;
 364      std::string tracker_id_BLACK_9_pos_string, tracker_id_BLACK_15_pos_string;
 365      std::string tracker_id_BLUE_8_pos_string, tracker_id_BLUE_14_pos_string;
 366      std::string tracker_id_BLUE_12_pos_string, tracker_id_BLUE_10_pos_string;
 367      phaseSpaceOptions = "profile=default";
 368  
 369      std::unordered_map<std::string, std::string> tracker_positions;
 370  
 371      // Default values
 372      tracker_positions["tracker_id_RED_5_pos_string"] = "pos=125.509767,143.875167,0";
 373      tracker_positions["tracker_id_RED_3_pos_string"] = "pos=125.509767,-135.624833,0";
 374      tracker_positions["tracker_id_RED_7_pos_string"] = "pos=-154.990233,-135.624833,0";
 375      tracker_positions["tracker_id_RED_4_pos_string"] = "pos=-153.490233,144.375167,0";
 376      tracker_positions["tracker_id_BLACK_13_pos_string"] = "pos=130.251807,141.800150,0";
 377      tracker_positions["tracker_id_BLACK_11_pos_string"] = "pos=130.751807,-135.699850,0";
 378      tracker_positions["tracker_id_BLACK_9_pos_string"] = "pos=-146.748193,-135.199850,0";
 379      tracker_positions["tracker_id_BLACK_15_pos_string"] = "pos=-146.748193,143.300150,0";
 380      tracker_positions["tracker_id_BLUE_8_pos_string"] = "pos=140.000177,152.096588,0";
 381      tracker_positions["tracker_id_BLUE_14_pos_string"] = "pos=140.500177,-125.403412,0";
 382      tracker_positions["tracker_id_BLUE_12_pos_string"] = "pos=-136.999823,-124.903412,0";
 383      tracker_positions["tracker_id_BLUE_10_pos_string"] = "pos=-136.999823,153.596588,0";
 384  
 385      // Attempt to read from file
 386      std::ifstream file("tracker_positions_phasespace.txt");
 387      if (file.is_open()) {
 388          std::string line;
 389          while (getline(file, line)) {
 390              std::istringstream iss(line);
 391              std::string id, x, y, z;
 392              getline(iss, id, ',');
 393              getline(iss, x, ',');
 394              getline(iss, y, ',');
 395              getline(iss, z);
 396              tracker_positions[id] = "pos=" + x + "," + y + "," + z;
 397          }
 398          file.close();
 399      }
 400  
 401      // Output the tracker positions (from file if possible, otherwise defaults)
 402      for (const auto& pair : tracker_positions) {
 403          std::cout << pair.first << " = " << pair.second << std::endl;
 404      }
 405  
 406      tracker_id_RED_5_pos_string = tracker_positions["tracker_id_RED_5_pos_string"];
 407      tracker_id_RED_3_pos_string = tracker_positions["tracker_id_RED_3_pos_string"];
 408      tracker_id_RED_7_pos_string = tracker_positions["tracker_id_RED_7_pos_string"];
 409      tracker_id_RED_4_pos_string = tracker_positions["tracker_id_RED_4_pos_string"];
 410  
 411      tracker_id_BLACK_13_pos_string = tracker_positions["tracker_id_BLACK_13_pos_string"];
 412      tracker_id_BLACK_11_pos_string = tracker_positions["tracker_id_BLACK_11_pos_string"];
 413      tracker_id_BLACK_9_pos_string = tracker_positions["tracker_id_BLACK_9_pos_string"];
 414      tracker_id_BLACK_15_pos_string = tracker_positions["tracker_id_BLACK_15_pos_string"];
 415  
 416      tracker_id_BLUE_8_pos_string = tracker_positions["tracker_id_BLUE_8_pos_string"];
 417      tracker_id_BLUE_14_pos_string = tracker_positions["tracker_id_BLUE_14_pos_string"];
 418      tracker_id_BLUE_12_pos_string = tracker_positions["tracker_id_BLUE_12_pos_string"];
 419      tracker_id_BLUE_10_pos_string = tracker_positions["tracker_id_BLUE_10_pos_string"];
 420  
 421      const std::string myoptions = phaseSpaceOptions;
 422  
 423  
 424      if (owl.open(address) <= 0 || owl.initialize() <= 0)
 425      {
 426          fprintf(stderr, "Could not connect to the PhaseSpace server at %s: %s\n", address.c_str(), owl.lastError().c_str());
 427          return 0;
 428      }
 429  
 430      /* Report frame times in microseconds; the packet time and the velocities use PhaseSpace's
 431         own frame timestamps instead of when this computer happened to receive each frame
 432         (frames often arrive in bursts, and dividing by those tiny gaps made velocities spike). */
 433      owl.timeBase(1, 1000000);
 434  
 435      uint32_t tracker_id_RED = 0;
 436      owl.createTracker(tracker_id_RED, "rigid", "RED_rigid");
 437  
 438      /* Assign markers to the rigid body and indicate their positions
 439          w.r.t the centre of mass (obtained from calibration text file) */
 440      owl.assignMarker(tracker_id_RED, 4, "4", tracker_id_RED_5_pos_string); // top left
 441      owl.assignMarker(tracker_id_RED, 2, "2", tracker_id_RED_3_pos_string); // top right
 442      owl.assignMarker(tracker_id_RED, 6, "6", tracker_id_RED_7_pos_string); // bottom right
 443      owl.assignMarker(tracker_id_RED, 0, "0", tracker_id_RED_4_pos_string); // bottom left (this LED reports as id 0, not 5)
 444  
 445      uint32_t tracker_id_BLACK = 2;
 446      owl.createTracker(tracker_id_BLACK, "rigid", "BLACK_rigid");
 447  
 448      /* Assign markers to the rigid body and indicate their positions
 449          w.r.t the centre of mass (obtained from calibration text file) */
 450      owl.assignMarker(tracker_id_BLACK, 10, "10", tracker_id_BLACK_13_pos_string); // top left
 451      owl.assignMarker(tracker_id_BLACK, 14, "14", tracker_id_BLACK_11_pos_string); // top right
 452      owl.assignMarker(tracker_id_BLACK, 13, "13", tracker_id_BLACK_9_pos_string); // bottom right (this LED reports as id 13, not 8)
 453      owl.assignMarker(tracker_id_BLACK, 12, "12", tracker_id_BLACK_15_pos_string); // bottom left
 454  
 455      uint32_t tracker_id_BLUE = 3;
 456  
 457      owl.createTracker(tracker_id_BLUE, "rigid", "BLUE_rigid");
 458  
 459      /* Assign markers to the rigid body and indicate their positions
 460          w.r.t the centre of mass (obtained from calibration text file) */
 461      owl.assignMarker(tracker_id_BLUE, 19, "19", tracker_id_BLUE_8_pos_string); // top left
 462      owl.assignMarker(tracker_id_BLUE, 21, "21", tracker_id_BLUE_14_pos_string); // top right
 463      owl.assignMarker(tracker_id_BLUE, 23, "23", tracker_id_BLUE_12_pos_string); // bottom right
 464      owl.assignMarker(tracker_id_BLUE, 17, "17", tracker_id_BLUE_10_pos_string); // bottom left
 465  
 466      /* Put every LED that is not part of a rigid body into a point tracker so that its
 467         position is also reported in the "markers" list. Keep this list in sync with the
 468         assignMarker calls above. */
 469      const uint32_t rigid_led_ids[] = { 4, 2, 6, 0, 10, 14, 13, 12, 19, 21, 23, 17 };
 470      uint32_t tracker_id_POINTS = 4;
 471      owl.createTracker(tracker_id_POINTS, "point", "LED_points");
 472      for (uint32_t led = 0; led < NUM_SERVER_MARKERS; led++)
 473      {
 474          if (std::find(std::begin(rigid_led_ids), std::end(rigid_led_ids), led) == std::end(rigid_led_ids))
 475              owl.assignMarker(tracker_id_POINTS, led, std::to_string(led));
 476      }
 477  
 478  
 479      owl.frequency(frequency);
 480  
 481      // start streaming (1 = TCP, 2 = UDP)
 482      owl.streaming(useUdp ? 2 : 1);
 483  
 484      auto t1 = Clock::now();
 485      double firstFrameTime = 0.0;
 486      bool havePhaseSpaceTime = false, firstFrame = true;
 487      double lastTime = 0.0;                                           /* the previous frame's packet time */
 488      double expStart = std::numeric_limits<double>::quiet_NaN();     /* packet time of the agreed start */
 489  
 490      // Status line every few seconds: frame rate actually received, and undelivered packets
 491      const double STATUS_INTERVAL = 5.0;
 492      auto statusStart = Clock::now();
 493      unsigned long framesSinceStatus = 0, trackedSinceStatus[3] = { 0, 0, 0 };
 494  
 495      // main loop
 496      while (!stopRequested && owl.isOpen() && owl.property<int>("initialized"))
 497      {
 498          const OWL::Event *event = owl.nextEvent(1000);
 499          if (!event) continue;
 500  
 501          if (event->type_id() == OWL::Type::ERROR)
 502          {
 503              // Print the error and only stop on fatal errors (non-fatal ones, e.g. a rejected
 504              // tracker or marker assignment, would otherwise silently stop all UDP output)
 505              std::cerr << "OWL error: " << event->name() << " " << event->str() << std::endl;
 506              if (strcmp(event->name(), "fatal") == 0) break;
 507          }
 508          else if (event->type_id() == OWL::Type::FRAME)
 509          {
 510              /* Seconds since streaming started, from the PhaseSpace frame timestamp. Falls back
 511                 to this computer's clock if the server has not reported a time base. */
 512              const int *tb = owl.timeBase();
 513              bool psTime = tb && tb[0] > 0 && tb[1] > 0 && event->time() > 0;
 514              double frameTime = psTime ? (double)event->time() * tb[0] / tb[1]
 515                  : std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - t1).count() * 1e-9;
 516              if (firstFrame || psTime != havePhaseSpaceTime)
 517              {
 518                  /* (Re)start the clock; also when the time source changes, so it never jumps.
 519                     The packet time restarts at 0, so an armed experiment clock is shifted with it. */
 520                  if (!firstFrame) expStart -= lastTime;
 521                  firstFrameTime = frameTime;
 522                  havePhaseSpaceTime = psTime;
 523                  prevTimeRED = prevTimeBLACK = prevTimeBLUE = -1.0;
 524              }
 525              if (firstFrame)
 526              {
 527                  firstFrame = false;
 528                  float maxFrequency = owl.property<float>("maxfrequency");
 529                  printf("First frame received. PhaseSpace frequency %.1f Hz (requested %d Hz", owl.frequency(), frequency);
 530                  if (maxFrequency > 0) printf(", system maximum %.0f Hz", maxFrequency);
 531                  printf("), streaming over %s, timestamps from %s.\n", useUdp ? "UDP" : "TCP",
 532                         psTime ? "PhaseSpace" : "this computer's clock");
 533              }
 534              double currentTime = frameTime - firstFrameTime;
 535              dataPacket[0] = currentTime;
 536              lastTime = currentTime;
 537              framesSinceStatus++;
 538  
 539              if (event->find("rigids", rigids) > 0)
 540              {
 541                  for (OWL::Rigids::iterator r = rigids.begin(); r != rigids.end(); r++)
 542                  {
 543                      if (r->cond > 0)
 544                      {
 545                          if (r->id == 0 || r->id == 2 || r->id == 3)
 546                              trackedSinceStatus[r->id == 0 ? 0 : r->id == 2 ? 1 : 2]++;
 547  
 548                          if (r->id == 0)
 549                          {
 550                              
 551                              // Assume prevRawAngle0 and accumulatedAngle0 are declared and initialized elsewhere
 552                              static double prevRawAngle0 = 0.0;
 553                              static double accumulatedAngle0 = 0.0;
 554  
 555                              dataPacket[1] = r->pose[0];
 556                              dataPacket[2] = r->pose[1];
 557  
 558                              // Compute the raw angle
 559                              double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
 560                                  + 2 * r->pose[3] * r->pose[6],
 561                                  2 * r->pose[3] * r->pose[3] - 1
 562                                  + 2 * r->pose[4] * r->pose[4]);
 563  
 564                              // Compute the difference between the current and the previous raw angle
 565                              double angleDifference = rawAngle - prevRawAngle0;
 566  
 567                              // Normalize the angle difference to the interval [-π, π]
 568                              if (angleDifference <= -PI)
 569                                  angleDifference += TWO_PI;
 570                              else if (angleDifference > PI)
 571                                  angleDifference -= TWO_PI;
 572  
 573                              // Accumulate the angle difference
 574                              accumulatedAngle0 += angleDifference;
 575  
 576                              // Store the current raw angle for the next iteration
 577                              prevRawAngle0 = rawAngle;
 578  
 579                              // Store the accumulated angle in the data packet
 580                              dataPacket[3] = accumulatedAngle0;
 581  
 582                              // Calculate the velocity (only once there is an earlier frame to compare with)
 583                              if (prevTimeRED >= 0.0 && currentTime > prevTimeRED)
 584                              {
 585                                  dataPacket[10] = (dataPacket[1] - prevServX) / (currentTime - prevTimeRED);
 586                                  dataPacket[11] = (dataPacket[2] - prevServY) / (currentTime - prevTimeRED);
 587                                  dataPacket[12] = (dataPacket[3] - prevServAtt) / (currentTime - prevTimeRED);
 588                              }
 589  
 590                              // Store the current values for the next iteration
 591                              prevServX = dataPacket[1];
 592                              prevServY = dataPacket[2];
 593                              prevServAtt = dataPacket[3];
 594  
 595  
 596                              //std::cout << dataPacket[1] << std::endl;
 597  
 598                              prevTimeRED = currentTime;
 599                          }
 600                          else if (r->id == 2)
 601                          {
 602                              // Assume prevRawAngle2 and accumulatedAngle2 are declared and initialized elsewhere
 603                              static double prevRawAngle2 = 0.0;
 604                              static double accumulatedAngle2 = 0.0;
 605  
 606                              dataPacket[4] = r->pose[0];
 607                              dataPacket[5] = r->pose[1];
 608  
 609                              // Compute the raw angle
 610                              double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
 611                                  + 2 * r->pose[3] * r->pose[6],
 612                                  2 * r->pose[3] * r->pose[3] - 1
 613                                  + 2 * r->pose[4] * r->pose[4]);
 614  
 615                              // Compute the difference between the current and the previous raw angle
 616                              double angleDifference = rawAngle - prevRawAngle2;
 617  
 618                              // Normalize the angle difference to the interval [-π, π]
 619                              if (angleDifference <= -PI)
 620                                  angleDifference += TWO_PI;
 621                              else if (angleDifference > PI)
 622                                  angleDifference -= TWO_PI;
 623  
 624                              // Accumulate the angle difference
 625                              accumulatedAngle2 += angleDifference;
 626  
 627                              // Store the current raw angle for the next iteration
 628                              prevRawAngle2 = rawAngle;
 629  
 630                              // Store the accumulated angle in the data packet
 631                              dataPacket[6] = accumulatedAngle2;
 632  
 633                              // Calculate the velocity (only once there is an earlier frame to compare with)
 634                              if (prevTimeBLACK >= 0.0 && currentTime > prevTimeBLACK)
 635                              {
 636                                  dataPacket[13] = (dataPacket[4] - prevClientX) / (currentTime - prevTimeBLACK);
 637                                  dataPacket[14] = (dataPacket[5] - prevClientY) / (currentTime - prevTimeBLACK);
 638                                  dataPacket[15] = (dataPacket[6] - prevClientAtt) / (currentTime - prevTimeBLACK);
 639                              }
 640  
 641                              // Store the current values for the next iteration
 642                              prevClientX = dataPacket[4];
 643                              prevClientY = dataPacket[5];
 644                              prevClientAtt = dataPacket[6];
 645  
 646  
 647                              prevTimeBLACK = currentTime;
 648  
 649                          }
 650                          else if (r->id == 3)
 651                          {
 652                              // Assume prevRawAngle3 and accumulatedAngle3 are declared and initialized elsewhere
 653                              static double prevRawAngle3 = 0.0;
 654                              static double accumulatedAngle3 = 0.0;
 655  
 656                              dataPacket[7] = r->pose[0];
 657                              dataPacket[8] = r->pose[1];
 658  
 659                              // Compute the raw angle
 660                              double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
 661                                  + 2 * r->pose[3] * r->pose[6],
 662                                  2 * r->pose[3] * r->pose[3] - 1
 663                                  + 2 * r->pose[4] * r->pose[4]);
 664  
 665                              // Compute the difference between the current and the previous raw angle
 666                              double angleDifference = rawAngle - prevRawAngle3;
 667  
 668                              // Normalize the angle difference to the interval [-π, π]
 669                              if (angleDifference <= -PI)
 670                                  angleDifference += TWO_PI;
 671                              else if (angleDifference > PI)
 672                                  angleDifference -= TWO_PI;
 673  
 674                              // Accumulate the angle difference
 675                              accumulatedAngle3 += angleDifference;
 676  
 677                              // Store the current raw angle for the next iteration
 678                              prevRawAngle3 = rawAngle;
 679  
 680                              // Store the accumulated angle in the data packet
 681                              dataPacket[9] = accumulatedAngle3;
 682  
 683                              // Calculate the velocity (only once there is an earlier frame to compare with)
 684                              if (prevTimeBLUE >= 0.0 && currentTime > prevTimeBLUE)
 685                              {
 686                                  dataPacket[16] = (dataPacket[7] - prevObstX) / (currentTime - prevTimeBLUE);
 687                                  dataPacket[17] = (dataPacket[8] - prevObstY) / (currentTime - prevTimeBLUE);
 688                                  dataPacket[18] = (dataPacket[9] - prevObstAtt) / (currentTime - prevTimeBLUE);
 689                              }
 690  
 691                              // Store the current values for the next iteration
 692                              prevObstX = dataPacket[7];
 693                              prevObstY = dataPacket[8];
 694                              prevObstAtt = dataPacket[9];
 695  
 696  
 697                              prevTimeBLUE = currentTime;
 698  
 699                          }
 700                      }
 701                  }
 702              }
 703  
 704              // Store the position of every visible LED (including those used by the rigid bodies)
 705              for (int i = NUM_RIGID_FIELDS; i < PACKET_LENGTH; i++)
 706                  dataPacket[i] = LED_NOT_VISIBLE;
 707  
 708              if (event->find("markers", markers) > 0)
 709              {
 710                  for (OWL::Markers::iterator m = markers.begin(); m != markers.end(); m++)
 711                  {
 712                      if (m->cond > 0 && m->id < NUM_LEDS)
 713                      {
 714                          int index = NUM_RIGID_FIELDS + 3 * m->id;
 715                          dataPacket[index] = m->x;
 716                          dataPacket[index + 1] = m->y;
 717                          dataPacket[index + 2] = m->z;
 718                      }
 719                  }
 720              }
 721  
 722              // The experiment clock (see EXP_CLOCK_INDEX), after the LEDs so it isn't overwritten
 723              {
 724                  std::lock_guard<std::mutex> guard(expLock);
 725                  if (expCommand == 1)
 726                  {
 727                      expStart = currentTime + expDelay;
 728                      printf("Experiment clock: the experiment starts in %.3f s (packet time %.3f s).\n", expDelay, expStart);
 729                  }
 730                  else if (expCommand == 2 && !std::isnan(expStart))
 731                      printf("Experiment clock cleared (it was at %.3f s).\n", currentTime - expStart);
 732                  if (expCommand == 2) expStart = std::numeric_limits<double>::quiet_NaN();
 733                  expCommand = 0;
 734              }
 735              dataPacket[EXP_CLOCK_INDEX] = std::isnan(expStart) ? LED_NOT_VISIBLE : currentTime - expStart;
 736  
 737              for (int i = 0; i < PACKET_LENGTH; i++)
 738                  sendPacket[i] = (PacketScalar)dataPacket[i];
 739  
 740              // send sendPacket array to every destination
 741              std::lock_guard<std::mutex> guard(destinationsLock);
 742              for (auto& d : destinations)
 743              {
 744                  if (send(d.sd, (char*)sendPacket, sizeof(sendPacket), 0) != (int)sizeof(sendPacket))
 745                      d.failed++;
 746              }
 747  
 748              double sinceStatus = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - statusStart).count() * 1e-9;
 749              if (sinceStatus >= STATUS_INTERVAL)
 750              {
 751                  printf("Status: %.1f frames/s from PhaseSpace (tracked per s: RED %.1f, BLACK %.1f, BLUE %.1f)",
 752                         framesSinceStatus / sinceStatus, trackedSinceStatus[0] / sinceStatus,
 753                         trackedSinceStatus[1] / sinceStatus, trackedSinceStatus[2] / sinceStatus);
 754                  if (!std::isnan(expStart))
 755                      printf(". Experiment clock %.1f s", currentTime - expStart);
 756                  bool anyFailed = false;
 757                  for (auto& d : destinations)
 758                  {
 759                      if (d.failed)
 760                      {
 761                          printf("%s %s: %lu", anyFailed ? "," : ". Not delivered:", d.address.c_str(), d.failed);
 762                          anyFailed = true;
 763                      }
 764                      d.failed = 0;
 765                  }
 766                  printf("\n");
 767                  statusStart = Clock::now();
 768                  framesSinceStatus = 0;
 769                  trackedSinceStatus[0] = trackedSinceStatus[1] = trackedSinceStatus[2] = 0;
 770              }
 771          }
 772      } // while 
 773  
 774      if (stopRequested)
 775          printf("Stop requested, closing the PhaseSpace connection.\n");
 776      owl.done();
 777      owl.close();
 778      for (const auto& d : destinations)
 779          closesocket(d.sd);
 780      networkCleanup();
 781      return 0;
 782  }
```

</details>

<details>
<summary><b>+spot/+console/phasespace/Makefile</b> (52 lines; click to show)</summary>

```
   1  # Builds StreamData and the PhaseSpace client library (libowlsock.so, compiled from the
   2  # PhaseSpace source in libowl-src-410) for whatever Linux machine this runs on: x86 laptop,
   3  # WSL on Windows, or ARM boards such as the Jetson TX2. Needs only g++ and make.
   4  #
   5  #   make                      -> ../output/linux-<arch>/StreamData + libowlsock.so (the console's folder)
   6  #   make OUT=/some/folder     build into another folder (the webapp does this)
   7  #   make clean                remove the build folder and the output folder's files
   8  #
   9  # Everything is built in ~/.cache (outside Dropbox, and free of spaces, which make cannot handle
  10  # in file names) so rebuilds only recompile what changed; the result is then copied to OUT.
  11  
  12  ARCH  := $(shell uname -m)
  13  OUT   ?= ../output/linux-$(ARCH)
  14  BUILD ?= $(HOME)/.cache/streamdata-build/$(ARCH)
  15  OWL   := libowl-src-410
  16  
  17  CXX      ?= g++
  18  LIBFLAGS := -fPIC -O3 -funroll-loops -Wall -Wno-terminate -MMD -MP
  19  APPFLAGS := -std=c++11 -O2 -Wall -MMD -MP -pthread
  20  
  21  LIB_SRC := owl.cc libowl.cc owl_rpd.cc libowl_socket.cc socket.cc
  22  LIB_OBJ := $(addprefix $(BUILD)/owl/,$(LIB_SRC:.cc=.o))
  23  APP_OBJ := $(BUILD)/StreamData.o
  24  LIB     := $(BUILD)/libowlsock.so
  25  APP     := $(BUILD)/StreamData
  26  
  27  .PHONY: all clean
  28  all: $(APP) $(LIB)
  29  	@mkdir -p "$(OUT)"
  30  	cp -f $(APP) $(LIB) "$(OUT)/"
  31  
  32  $(LIB): $(LIB_OBJ)
  33  	$(CXX) -shared $^ -lrt -lpthread -o $@
  34  
  35  # StreamData finds libowlsock.so in its own folder ($ORIGIN), wherever that folder is moved
  36  $(APP): $(APP_OBJ) $(LIB)
  37  	$(CXX) $(APP_OBJ) -pthread -L$(BUILD) -lowlsock -lpthread -Wl,-rpath,'$$ORIGIN' -o $@
  38  
  39  $(BUILD)/owl/libowl.o $(BUILD)/owl/libowl_socket.o: LIBFLAGS += -DLIBOWL_REV=410
  40  
  41  $(BUILD)/owl/%.o: $(OWL)/%.cc
  42  	@mkdir -p $(@D)
  43  	$(CXX) $(LIBFLAGS) -c $< -o $@
  44  
  45  $(BUILD)/StreamData.o: StreamData.cc
  46  	@mkdir -p $(@D)
  47  	$(CXX) $(APPFLAGS) -c $< -o $@
  48  
  49  clean:
  50  	rm -rf $(BUILD) "$(OUT)/StreamData" "$(OUT)/libowlsock.so"
  51  
  52  -include $(LIB_OBJ:.o=.d) $(APP_OBJ:.o=.d)
```

</details>

<details>
<summary><b>+spot/+console/phasespace/build_linux.sh</b> (7 lines; click to show)</summary>

```bash
   1  #!/usr/bin/env bash
   2  # Builds ../output/linux-<arch>/StreamData and libowlsock.so (output/ in the console's folder) (from the PhaseSpace source in
   3  # libowl-src-410). Needs g++ and make (Ubuntu/WSL: sudo apt install g++ make; Fedora: sudo dnf install gcc-c++ make).
   4  set -euo pipefail
   5  cd "$(dirname "$0")"
   6  make -j"$(nproc)" "$@"
   7  echo "Built ../output/linux-$(uname -m)/StreamData  (run: ../output/linux-$(uname -m)/StreamData <frequency>)"
```

</details>
<!-- /listing -->
