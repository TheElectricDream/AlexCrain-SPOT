// example_rigid1.cc -*- C++ -*-
// simple rigid tracking program

/***
Copyright (c) PhaseSpace, Inc 2017

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL PHASESPACE, INC
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***/

/* Builds on Windows (MSVC, links ws2_32.lib) and Linux (g++). See CMakeLists.txt. */
//#include "stdafx.h"

//!!!!!!!!!NETWORK CODE!!!!!!!!!!!!!!!
#ifdef _WIN32
#pragma comment (lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#ifndef WIN32
#define WIN32 /* owl.hpp keys dllimport and its "#undef ERROR" off WIN32, not _WIN32 */
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define closesocket close
#endif
#include <time.h>
#include <chrono>
#include <csignal>
#include <string>
#include <mutex>
#include <thread>
#define BUFFER_SIZE 4096

/* Platform wrappers: Winsock needs explicit start-up/clean-up, POSIX sockets do not. */
static bool networkStartup()
{
#ifdef _WIN32
    WSADATA w;
    return WSAStartup(MAKEWORD(2, 2), &w) == 0;
#else
    return true;
#endif
}

static void networkCleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

static void setNonBlocking(SOCKET sd)
{
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(sd, FIONBIO, &mode);
#else
    fcntl(sd, F_SETFL, fcntl(sd, F_GETFL, 0) | O_NONBLOCK);
#endif
}

/* Every destination gets its own UDP socket. With a single shared socket, packets waiting
   for a computer that is switched off (its address never resolves, which takes ~3 s to time
   out) filled the shared send buffer, and the sends to every other computer then failed
   silently: in testing, 99.7% of packets to a live destination were lost that way. */
struct Destination {
    std::string address;
    SOCKET sd;
    unsigned long failed; /* packets not handed to the network since the last status line */
    bool extra;           /* added with --dest or "dest add" (can be removed again) */
};

/* Dotted IPv4 address such as 192.168.1.43 */
static bool isIPv4(const std::string &s)
{
    unsigned a, b, c, d;
    char end;
    return sscanf(s.c_str(), "%u.%u.%u.%u%c", &a, &b, &c, &d, &end) == 4 && a < 256 && b < 256 && c < 256 && d < 256;
}

static SOCKET openDestination(const std::string &address, unsigned short port, unsigned short source_port)
{
    SOCKET sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == INVALID_SOCKET)
        return sd;

    int on = 1, bufferSize = 1 << 20;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char*)&bufferSize, sizeof(bufferSize));

    /* Keep sending from the same source port as before, in case a receiver filters on it */
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(source_port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sd, (struct sockaddr*)&local, sizeof(local));

    struct sockaddr_in remote;
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    remote.sin_addr.s_addr = inet_addr(address.c_str());
    connect(sd, (struct sockaddr*)&remote, sizeof(remote));

    setNonBlocking(sd);
    return sd;
}
//!!!!!!!!!NETWORK CODE!!!!!!!!!!!!!!!/

#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>

#include "libowl-src-410/owl.hpp" /* PhaseSpace OWL C++ API v2.2, same source as libowlsock.so */

//#include <ctime>

using namespace std;

typedef std::chrono::high_resolution_clock Clock;

/* UDP packet layout (every element is a PacketScalar, 0-based indices):
     [0]                   time since streaming started (s)
     [1..3]                RED   x, y, attitude
     [4..6]                BLACK x, y, attitude
     [7..9]                BLUE  x, y, attitude
     [10..12]              RED   vx, vy, angular rate
     [13..15]              BLACK vx, vy, angular rate
     [16..18]              BLUE  vx, vy, angular rate
     [19+3k .. 21+3k]      LED k x, y, z for k = 0..NUM_LEDS-1 (NaN if LED k is not visible this frame)
     [112]                 (LED 31's x) the experiment clock instead: NaN when no experiment is armed,
                           negative while counting down to the agreed start, then seconds since the start
   Calculations are done in double; the packet is only converted to PacketScalar when sent.
   Change PacketScalar to double for full precision at twice the packet size. */
typedef float PacketScalar;
const int NUM_RIGID_FIELDS = 19;
const int NUM_LEDS = 32;
/* The PhaseSpace server rejects marker ids >= 24 ("assignMarker: invalid marker id=24"),
   so only ids below this are put in the point tracker. Their packet slots stay NaN. */
const int NUM_SERVER_MARKERS = 24;
const int PACKET_LENGTH = NUM_RIGID_FIELDS + 3 * NUM_LEDS;
/* The experiment clock: when the lab console starts an experiment it waits until every platform's
   program is running, then sends "exp start <seconds>" on standard input. From then on every
   packet carries the time since that agreed start in this slot (LED 31's x; the server never
   reports LED 31, see NUM_SERVER_MARKERS), so all platforms start and switch phases together,
   whenever each program was started (spot.hardware.experimentTime reads it). "exp clear" ends it. */
const int EXP_CLOCK_INDEX = NUM_RIGID_FIELDS + 3 * 31;

/* Set by Ctrl+C (or the webapp's Stop button) so the main loop can shut down cleanly. */
static volatile std::sig_atomic_t stopRequested = 0;
static void requestStop(int) { stopRequested = 1; }

int main(int argc, const char **argv)
{
    /* Usage: StreamData <frequency> [monitor_port] [--udp] [--dest IP ...]
         monitor_port  also send a copy of every packet to this port on 127.0.0.1, for the
                       webapp viewer or Python/udp_listener.py (e.g. "StreamData 100 31540")
         --udp         receive frames from PhaseSpace over UDP instead of TCP: a late frame is
                       dropped instead of holding up the frames behind it
         --dest IP     also send to this computer (port 31534); can be repeated
       While running, lines on standard input add or remove extra destinations, and start or end
       the experiment clock (see EXP_CLOCK_INDEX):
         dest add 192.168.1.43        dest remove 192.168.1.43
         exp start 3                  exp clear */
    std::vector<std::string> positional, extra_addresses;
    bool useUdp = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--udp") == 0) useUdp = true;
        else if (strcmp(argv[i], "--dest") == 0 && i + 1 < argc) {
            if (isIPv4(argv[++i])) extra_addresses.push_back(argv[i]);
            else fprintf(stderr, "Ignoring --dest %s: not an IPv4 address.\n", argv[i]);
        }
        else positional.push_back(argv[i]);
    }
	if (positional.empty()) {
		// print error message if no value is provided
		std::cerr << "Error: no frequency value provided" << std::endl;
		std::cerr << "Usage: StreamData <frequency> [monitor_port] [--udp] [--dest IP ...]" << std::endl;
		return 1; // exit program with error code
	}
    int frequency = atoi(positional[0].c_str());
    int monitor_port = (positional.size() >= 2) ? atoi(positional[1].c_str()) : 0;

    setvbuf(stdout, NULL, _IONBF, 0); // print immediately, even when the output goes to the webapp
    std::signal(SIGINT, requestStop);
    std::signal(SIGTERM, requestStop);
#ifdef SIGBREAK
    std::signal(SIGBREAK, requestStop); // Ctrl+Break, used by the webapp's Stop button on Windows
#endif


    unsigned short port_number = 31535;	/* Port the packets are sent from */
    unsigned short client_port = 31534;	/* Port the packets are sent to */
    double dataPacket[PACKET_LENGTH] = { 0 };		/* Create array for data storage */
    PacketScalar sendPacket[PACKET_LENGTH];			/* dataPacket converted to the wire format */
    const double LED_NOT_VISIBLE = std::numeric_limits<double>::quiet_NaN();
    const double TWO_PI = 2.0 * 3.14159265358979323846;
    const double PI = 3.14159265358979323846;

    // Set up the previous values for velocity
    double prevServX = 0.0;
    double prevServY = 0.0;
    double prevServAtt = 0.0;

    double prevClientX = 0.0;
    double prevClientY = 0.0;
    double prevClientAtt = 0.0;

    double prevObstX = 0.0;
    double prevObstY = 0.0;
    double prevObstAtt = 0.0;

    // Time of each body's previous valid frame (-1 = none yet, so no velocity is computed)
    double prevTimeRED = -1.0;
    double prevTimeBLACK = -1.0;
    double prevTimeBLUE = -1.0;


    /* Open network connection (Winsock start-up on Windows, no-op on Linux) */
    if (!networkStartup())
    {
        //fprintf(stderr, "Could not open Windows connection.\n");
        ::exit(0);
    }

    /* Below is the IP address for the UDP client. */
    std::vector<std::string> client_addresses = { "192.168.1.110", "192.168.1.111", "192.168.1.112", "192.168.1.104", "192.168.1.115"};

    /* static: also used by the command thread below, which may outlive main's locals at exit */
    static std::vector<Destination> destinations;
    static std::mutex destinationsLock;
    /* The experiment clock's pending command, from the command thread to the frame loop */
    static std::mutex expLock;
    static int expCommand = 0;          /* 1 = start in expDelay s, 2 = clear */
    static double expDelay = 0.0;
    for (const auto& address : client_addresses)
        destinations.push_back({ address, openDestination(address, client_port, port_number), 0, false });
    if (monitor_port > 0) /* local copy for the viewer */
        destinations.push_back({ "127.0.0.1:" + std::to_string(monitor_port),
                                 openDestination("127.0.0.1", (unsigned short)monitor_port, port_number), 0, false });
    for (const auto& address : extra_addresses)
    {
        bool known = false;
        for (const auto& d : destinations) known = known || d.address == address;
        if (!known)
            destinations.push_back({ address, openDestination(address, client_port, port_number), 0, true });
    }
    for (const auto& d : destinations)
    {
        if (d.sd == INVALID_SOCKET)
        {
            fprintf(stderr, "Could not create a socket for %s.\n", d.address.c_str());
            networkCleanup();
            ::exit(0);
        }
    }

    /* Print out server information */
    printf("Sending from port %u to port %u on:", (unsigned)port_number, (unsigned)client_port);
    for (const auto& address : client_addresses)
        printf(" %s", address.c_str());
    printf("\n");
    for (const auto& d : destinations)
        if (d.extra) printf("Also sending to %s (added destination).\n", d.address.c_str());
    if (monitor_port > 0)
        printf("Also sending a copy of every packet to 127.0.0.1:%d (viewer).\n", monitor_port);

    /* Commands on standard input while streaming (the webapp uses these, so nothing has to
       restart): add or remove extra destinations ("dest add IP" / "dest remove IP"), and start
       or end the experiment clock ("exp start SECONDS" / "exp clear"). */
    std::thread([client_port, port_number]() {
        std::string line;
        while (std::getline(std::cin, line))
        {
            std::istringstream words(line);
            std::string cmd, action, ip;
            words >> cmd >> action;
            if (cmd == "exp")
            {
                double delay = -1.0;
                if (action == "start" && (words >> delay) && delay >= 0.0 && delay <= 600.0)
                {
                    std::lock_guard<std::mutex> guard(expLock);
                    expCommand = 1;
                    expDelay = delay;
                }
                else if (action == "clear")
                {
                    std::lock_guard<std::mutex> guard(expLock);
                    expCommand = 2;
                }
                else
                    printf("Ignored command: %s (expected: exp start <0-600 s> | exp clear)\n", line.c_str());
                continue;
            }
            words >> ip;
            if (cmd != "dest" || (action != "add" && action != "remove") || !isIPv4(ip))
            {
                if (!line.empty()) printf("Ignored command: %s (expected: dest add|remove <IPv4>, exp start <s>, exp clear)\n", line.c_str());
                continue;
            }
            std::lock_guard<std::mutex> guard(destinationsLock);
            auto it = std::find_if(destinations.begin(), destinations.end(),
                                   [&ip](const Destination &d) { return d.address == ip; });
            if (action == "add")
            {
                if (it != destinations.end()) { printf("Already sending to %s.\n", ip.c_str()); continue; }
                SOCKET sd = openDestination(ip, client_port, port_number);
                if (sd == INVALID_SOCKET) { printf("Could not create a socket for %s.\n", ip.c_str()); continue; }
                destinations.push_back({ ip, sd, 0, true });
                printf("Now also sending to %s.\n", ip.c_str());
            }
            else
            {
                if (it == destinations.end() || !it->extra) { printf("%s is not an added destination.\n", ip.c_str()); continue; }
                closesocket(it->sd);
                destinations.erase(it);
                printf("Stopped sending to %s.\n", ip.c_str());
            }
        }
    }).detach();

    printf("Use the ON/OFF Toggle in the SPOT Application to shut down cleanly. This console can be minimized.\n");
    printf("Sending %d x %u-byte values (%u bytes) per frame.\n",
        PACKET_LENGTH, (unsigned)sizeof(PacketScalar), (unsigned)(PACKET_LENGTH * sizeof(PacketScalar)));

    /* This is where the PhaseSpace code begins. */
    string address = "192.168.1.109";
    OWL::Context owl;
    OWL::Markers markers;
    OWL::Rigids rigids;

    std::string phaseSpaceOptions;
    std::string tracker_id_RED_5_pos_string, tracker_id_RED_3_pos_string;
    std::string tracker_id_RED_7_pos_string, tracker_id_RED_4_pos_string;
    std::string tracker_id_BLACK_13_pos_string, tracker_id_BLACK_11_pos_string;
    std::string tracker_id_BLACK_9_pos_string, tracker_id_BLACK_15_pos_string;
    std::string tracker_id_BLUE_8_pos_string, tracker_id_BLUE_14_pos_string;
    std::string tracker_id_BLUE_12_pos_string, tracker_id_BLUE_10_pos_string;
    phaseSpaceOptions = "profile=default";

    std::unordered_map<std::string, std::string> tracker_positions;

    // Default values
    tracker_positions["tracker_id_RED_5_pos_string"] = "pos=125.509767,143.875167,0";
    tracker_positions["tracker_id_RED_3_pos_string"] = "pos=125.509767,-135.624833,0";
    tracker_positions["tracker_id_RED_7_pos_string"] = "pos=-154.990233,-135.624833,0";
    tracker_positions["tracker_id_RED_4_pos_string"] = "pos=-153.490233,144.375167,0";
    tracker_positions["tracker_id_BLACK_13_pos_string"] = "pos=130.251807,141.800150,0";
    tracker_positions["tracker_id_BLACK_11_pos_string"] = "pos=130.751807,-135.699850,0";
    tracker_positions["tracker_id_BLACK_9_pos_string"] = "pos=-146.748193,-135.199850,0";
    tracker_positions["tracker_id_BLACK_15_pos_string"] = "pos=-146.748193,143.300150,0";
    tracker_positions["tracker_id_BLUE_8_pos_string"] = "pos=140.000177,152.096588,0";
    tracker_positions["tracker_id_BLUE_14_pos_string"] = "pos=140.500177,-125.403412,0";
    tracker_positions["tracker_id_BLUE_12_pos_string"] = "pos=-136.999823,-124.903412,0";
    tracker_positions["tracker_id_BLUE_10_pos_string"] = "pos=-136.999823,153.596588,0";

    // Attempt to read from file
    std::ifstream file("tracker_positions_phasespace.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string id, x, y, z;
            getline(iss, id, ',');
            getline(iss, x, ',');
            getline(iss, y, ',');
            getline(iss, z);
            tracker_positions[id] = "pos=" + x + "," + y + "," + z;
        }
        file.close();
    }

    // Output the tracker positions (from file if possible, otherwise defaults)
    for (const auto& pair : tracker_positions) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }

    tracker_id_RED_5_pos_string = tracker_positions["tracker_id_RED_5_pos_string"];
    tracker_id_RED_3_pos_string = tracker_positions["tracker_id_RED_3_pos_string"];
    tracker_id_RED_7_pos_string = tracker_positions["tracker_id_RED_7_pos_string"];
    tracker_id_RED_4_pos_string = tracker_positions["tracker_id_RED_4_pos_string"];

    tracker_id_BLACK_13_pos_string = tracker_positions["tracker_id_BLACK_13_pos_string"];
    tracker_id_BLACK_11_pos_string = tracker_positions["tracker_id_BLACK_11_pos_string"];
    tracker_id_BLACK_9_pos_string = tracker_positions["tracker_id_BLACK_9_pos_string"];
    tracker_id_BLACK_15_pos_string = tracker_positions["tracker_id_BLACK_15_pos_string"];

    tracker_id_BLUE_8_pos_string = tracker_positions["tracker_id_BLUE_8_pos_string"];
    tracker_id_BLUE_14_pos_string = tracker_positions["tracker_id_BLUE_14_pos_string"];
    tracker_id_BLUE_12_pos_string = tracker_positions["tracker_id_BLUE_12_pos_string"];
    tracker_id_BLUE_10_pos_string = tracker_positions["tracker_id_BLUE_10_pos_string"];

    const std::string myoptions = phaseSpaceOptions;


    if (owl.open(address) <= 0 || owl.initialize() <= 0)
    {
        fprintf(stderr, "Could not connect to the PhaseSpace server at %s: %s\n", address.c_str(), owl.lastError().c_str());
        return 0;
    }

    /* Report frame times in microseconds; the packet time and the velocities use PhaseSpace's
       own frame timestamps instead of when this computer happened to receive each frame
       (frames often arrive in bursts, and dividing by those tiny gaps made velocities spike). */
    owl.timeBase(1, 1000000);

    uint32_t tracker_id_RED = 0;
    owl.createTracker(tracker_id_RED, "rigid", "RED_rigid");

    /* Assign markers to the rigid body and indicate their positions
        w.r.t the centre of mass (obtained from calibration text file) */
    owl.assignMarker(tracker_id_RED, 4, "4", tracker_id_RED_5_pos_string); // top left
    owl.assignMarker(tracker_id_RED, 2, "2", tracker_id_RED_3_pos_string); // top right
    owl.assignMarker(tracker_id_RED, 6, "6", tracker_id_RED_7_pos_string); // bottom right
    owl.assignMarker(tracker_id_RED, 0, "0", tracker_id_RED_4_pos_string); // bottom left (this LED reports as id 0, not 5)

    uint32_t tracker_id_BLACK = 2;
    owl.createTracker(tracker_id_BLACK, "rigid", "BLACK_rigid");

    /* Assign markers to the rigid body and indicate their positions
        w.r.t the centre of mass (obtained from calibration text file) */
    owl.assignMarker(tracker_id_BLACK, 10, "10", tracker_id_BLACK_13_pos_string); // top left
    owl.assignMarker(tracker_id_BLACK, 14, "14", tracker_id_BLACK_11_pos_string); // top right
    owl.assignMarker(tracker_id_BLACK, 13, "13", tracker_id_BLACK_9_pos_string); // bottom right (this LED reports as id 13, not 8)
    owl.assignMarker(tracker_id_BLACK, 12, "12", tracker_id_BLACK_15_pos_string); // bottom left

    uint32_t tracker_id_BLUE = 3;

    owl.createTracker(tracker_id_BLUE, "rigid", "BLUE_rigid");

    /* Assign markers to the rigid body and indicate their positions
        w.r.t the centre of mass (obtained from calibration text file) */
    owl.assignMarker(tracker_id_BLUE, 19, "19", tracker_id_BLUE_8_pos_string); // top left
    owl.assignMarker(tracker_id_BLUE, 21, "21", tracker_id_BLUE_14_pos_string); // top right
    owl.assignMarker(tracker_id_BLUE, 23, "23", tracker_id_BLUE_12_pos_string); // bottom right
    owl.assignMarker(tracker_id_BLUE, 17, "17", tracker_id_BLUE_10_pos_string); // bottom left

    /* Put every LED that is not part of a rigid body into a point tracker so that its
       position is also reported in the "markers" list. Keep this list in sync with the
       assignMarker calls above. */
    const uint32_t rigid_led_ids[] = { 4, 2, 6, 0, 10, 14, 13, 12, 19, 21, 23, 17 };
    uint32_t tracker_id_POINTS = 4;
    owl.createTracker(tracker_id_POINTS, "point", "LED_points");
    for (uint32_t led = 0; led < NUM_SERVER_MARKERS; led++)
    {
        if (std::find(std::begin(rigid_led_ids), std::end(rigid_led_ids), led) == std::end(rigid_led_ids))
            owl.assignMarker(tracker_id_POINTS, led, std::to_string(led));
    }


    owl.frequency(frequency);

    // start streaming (1 = TCP, 2 = UDP)
    owl.streaming(useUdp ? 2 : 1);

    auto t1 = Clock::now();
    double firstFrameTime = 0.0;
    bool havePhaseSpaceTime = false, firstFrame = true;
    double lastTime = 0.0;                                           /* the previous frame's packet time */
    double expStart = std::numeric_limits<double>::quiet_NaN();     /* packet time of the agreed start */

    // Status line every few seconds: frame rate actually received, and undelivered packets
    const double STATUS_INTERVAL = 5.0;
    auto statusStart = Clock::now();
    unsigned long framesSinceStatus = 0, trackedSinceStatus[3] = { 0, 0, 0 };

    // main loop
    while (!stopRequested && owl.isOpen() && owl.property<int>("initialized"))
    {
        const OWL::Event *event = owl.nextEvent(1000);
        if (!event) continue;

        if (event->type_id() == OWL::Type::ERROR)
        {
            // Print the error and only stop on fatal errors (non-fatal ones, e.g. a rejected
            // tracker or marker assignment, would otherwise silently stop all UDP output)
            std::cerr << "OWL error: " << event->name() << " " << event->str() << std::endl;
            if (strcmp(event->name(), "fatal") == 0) break;
        }
        else if (event->type_id() == OWL::Type::FRAME)
        {
            /* Seconds since streaming started, from the PhaseSpace frame timestamp. Falls back
               to this computer's clock if the server has not reported a time base. */
            const int *tb = owl.timeBase();
            bool psTime = tb && tb[0] > 0 && tb[1] > 0 && event->time() > 0;
            double frameTime = psTime ? (double)event->time() * tb[0] / tb[1]
                : std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - t1).count() * 1e-9;
            if (firstFrame || psTime != havePhaseSpaceTime)
            {
                /* (Re)start the clock; also when the time source changes, so it never jumps.
                   The packet time restarts at 0, so an armed experiment clock is shifted with it. */
                if (!firstFrame) expStart -= lastTime;
                firstFrameTime = frameTime;
                havePhaseSpaceTime = psTime;
                prevTimeRED = prevTimeBLACK = prevTimeBLUE = -1.0;
            }
            if (firstFrame)
            {
                firstFrame = false;
                float maxFrequency = owl.property<float>("maxfrequency");
                printf("First frame received. PhaseSpace frequency %.1f Hz (requested %d Hz", owl.frequency(), frequency);
                if (maxFrequency > 0) printf(", system maximum %.0f Hz", maxFrequency);
                printf("), streaming over %s, timestamps from %s.\n", useUdp ? "UDP" : "TCP",
                       psTime ? "PhaseSpace" : "this computer's clock");
            }
            double currentTime = frameTime - firstFrameTime;
            dataPacket[0] = currentTime;
            lastTime = currentTime;
            framesSinceStatus++;

            if (event->find("rigids", rigids) > 0)
            {
                for (OWL::Rigids::iterator r = rigids.begin(); r != rigids.end(); r++)
                {
                    if (r->cond > 0)
                    {
                        if (r->id == 0 || r->id == 2 || r->id == 3)
                            trackedSinceStatus[r->id == 0 ? 0 : r->id == 2 ? 1 : 2]++;

                        if (r->id == 0)
                        {
                            
                            // Assume prevRawAngle0 and accumulatedAngle0 are declared and initialized elsewhere
                            static double prevRawAngle0 = 0.0;
                            static double accumulatedAngle0 = 0.0;

                            dataPacket[1] = r->pose[0];
                            dataPacket[2] = r->pose[1];

                            // Compute the raw angle
                            double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
                                + 2 * r->pose[3] * r->pose[6],
                                2 * r->pose[3] * r->pose[3] - 1
                                + 2 * r->pose[4] * r->pose[4]);

                            // Compute the difference between the current and the previous raw angle
                            double angleDifference = rawAngle - prevRawAngle0;

                            // Normalize the angle difference to the interval [-π, π]
                            if (angleDifference <= -PI)
                                angleDifference += TWO_PI;
                            else if (angleDifference > PI)
                                angleDifference -= TWO_PI;

                            // Accumulate the angle difference
                            accumulatedAngle0 += angleDifference;

                            // Store the current raw angle for the next iteration
                            prevRawAngle0 = rawAngle;

                            // Store the accumulated angle in the data packet
                            dataPacket[3] = accumulatedAngle0;

                            // Calculate the velocity (only once there is an earlier frame to compare with)
                            if (prevTimeRED >= 0.0 && currentTime > prevTimeRED)
                            {
                                dataPacket[10] = (dataPacket[1] - prevServX) / (currentTime - prevTimeRED);
                                dataPacket[11] = (dataPacket[2] - prevServY) / (currentTime - prevTimeRED);
                                dataPacket[12] = (dataPacket[3] - prevServAtt) / (currentTime - prevTimeRED);
                            }

                            // Store the current values for the next iteration
                            prevServX = dataPacket[1];
                            prevServY = dataPacket[2];
                            prevServAtt = dataPacket[3];


                            //std::cout << dataPacket[1] << std::endl;

                            prevTimeRED = currentTime;
                        }
                        else if (r->id == 2)
                        {
                            // Assume prevRawAngle2 and accumulatedAngle2 are declared and initialized elsewhere
                            static double prevRawAngle2 = 0.0;
                            static double accumulatedAngle2 = 0.0;

                            dataPacket[4] = r->pose[0];
                            dataPacket[5] = r->pose[1];

                            // Compute the raw angle
                            double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
                                + 2 * r->pose[3] * r->pose[6],
                                2 * r->pose[3] * r->pose[3] - 1
                                + 2 * r->pose[4] * r->pose[4]);

                            // Compute the difference between the current and the previous raw angle
                            double angleDifference = rawAngle - prevRawAngle2;

                            // Normalize the angle difference to the interval [-π, π]
                            if (angleDifference <= -PI)
                                angleDifference += TWO_PI;
                            else if (angleDifference > PI)
                                angleDifference -= TWO_PI;

                            // Accumulate the angle difference
                            accumulatedAngle2 += angleDifference;

                            // Store the current raw angle for the next iteration
                            prevRawAngle2 = rawAngle;

                            // Store the accumulated angle in the data packet
                            dataPacket[6] = accumulatedAngle2;

                            // Calculate the velocity (only once there is an earlier frame to compare with)
                            if (prevTimeBLACK >= 0.0 && currentTime > prevTimeBLACK)
                            {
                                dataPacket[13] = (dataPacket[4] - prevClientX) / (currentTime - prevTimeBLACK);
                                dataPacket[14] = (dataPacket[5] - prevClientY) / (currentTime - prevTimeBLACK);
                                dataPacket[15] = (dataPacket[6] - prevClientAtt) / (currentTime - prevTimeBLACK);
                            }

                            // Store the current values for the next iteration
                            prevClientX = dataPacket[4];
                            prevClientY = dataPacket[5];
                            prevClientAtt = dataPacket[6];


                            prevTimeBLACK = currentTime;

                        }
                        else if (r->id == 3)
                        {
                            // Assume prevRawAngle3 and accumulatedAngle3 are declared and initialized elsewhere
                            static double prevRawAngle3 = 0.0;
                            static double accumulatedAngle3 = 0.0;

                            dataPacket[7] = r->pose[0];
                            dataPacket[8] = r->pose[1];

                            // Compute the raw angle
                            double rawAngle = atan2(2 * r->pose[4] * r->pose[5]
                                + 2 * r->pose[3] * r->pose[6],
                                2 * r->pose[3] * r->pose[3] - 1
                                + 2 * r->pose[4] * r->pose[4]);

                            // Compute the difference between the current and the previous raw angle
                            double angleDifference = rawAngle - prevRawAngle3;

                            // Normalize the angle difference to the interval [-π, π]
                            if (angleDifference <= -PI)
                                angleDifference += TWO_PI;
                            else if (angleDifference > PI)
                                angleDifference -= TWO_PI;

                            // Accumulate the angle difference
                            accumulatedAngle3 += angleDifference;

                            // Store the current raw angle for the next iteration
                            prevRawAngle3 = rawAngle;

                            // Store the accumulated angle in the data packet
                            dataPacket[9] = accumulatedAngle3;

                            // Calculate the velocity (only once there is an earlier frame to compare with)
                            if (prevTimeBLUE >= 0.0 && currentTime > prevTimeBLUE)
                            {
                                dataPacket[16] = (dataPacket[7] - prevObstX) / (currentTime - prevTimeBLUE);
                                dataPacket[17] = (dataPacket[8] - prevObstY) / (currentTime - prevTimeBLUE);
                                dataPacket[18] = (dataPacket[9] - prevObstAtt) / (currentTime - prevTimeBLUE);
                            }

                            // Store the current values for the next iteration
                            prevObstX = dataPacket[7];
                            prevObstY = dataPacket[8];
                            prevObstAtt = dataPacket[9];


                            prevTimeBLUE = currentTime;

                        }
                    }
                }
            }

            // Store the position of every visible LED (including those used by the rigid bodies)
            for (int i = NUM_RIGID_FIELDS; i < PACKET_LENGTH; i++)
                dataPacket[i] = LED_NOT_VISIBLE;

            if (event->find("markers", markers) > 0)
            {
                for (OWL::Markers::iterator m = markers.begin(); m != markers.end(); m++)
                {
                    if (m->cond > 0 && m->id < NUM_LEDS)
                    {
                        int index = NUM_RIGID_FIELDS + 3 * m->id;
                        dataPacket[index] = m->x;
                        dataPacket[index + 1] = m->y;
                        dataPacket[index + 2] = m->z;
                    }
                }
            }

            // The experiment clock (see EXP_CLOCK_INDEX), after the LEDs so it isn't overwritten
            {
                std::lock_guard<std::mutex> guard(expLock);
                if (expCommand == 1)
                {
                    expStart = currentTime + expDelay;
                    printf("Experiment clock: the experiment starts in %.3f s (packet time %.3f s).\n", expDelay, expStart);
                }
                else if (expCommand == 2 && !std::isnan(expStart))
                    printf("Experiment clock cleared (it was at %.3f s).\n", currentTime - expStart);
                if (expCommand == 2) expStart = std::numeric_limits<double>::quiet_NaN();
                expCommand = 0;
            }
            dataPacket[EXP_CLOCK_INDEX] = std::isnan(expStart) ? LED_NOT_VISIBLE : currentTime - expStart;

            for (int i = 0; i < PACKET_LENGTH; i++)
                sendPacket[i] = (PacketScalar)dataPacket[i];

            // send sendPacket array to every destination
            std::lock_guard<std::mutex> guard(destinationsLock);
            for (auto& d : destinations)
            {
                if (send(d.sd, (char*)sendPacket, sizeof(sendPacket), 0) != (int)sizeof(sendPacket))
                    d.failed++;
            }

            double sinceStatus = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - statusStart).count() * 1e-9;
            if (sinceStatus >= STATUS_INTERVAL)
            {
                printf("Status: %.1f frames/s from PhaseSpace (tracked per s: RED %.1f, BLACK %.1f, BLUE %.1f)",
                       framesSinceStatus / sinceStatus, trackedSinceStatus[0] / sinceStatus,
                       trackedSinceStatus[1] / sinceStatus, trackedSinceStatus[2] / sinceStatus);
                if (!std::isnan(expStart))
                    printf(". Experiment clock %.1f s", currentTime - expStart);
                bool anyFailed = false;
                for (auto& d : destinations)
                {
                    if (d.failed)
                    {
                        printf("%s %s: %lu", anyFailed ? "," : ". Not delivered:", d.address.c_str(), d.failed);
                        anyFailed = true;
                    }
                    d.failed = 0;
                }
                printf("\n");
                statusStart = Clock::now();
                framesSinceStatus = 0;
                trackedSinceStatus[0] = trackedSinceStatus[1] = trackedSinceStatus[2] = 0;
            }
        }
    } // while 

    if (stopRequested)
        printf("Stop requested, closing the PhaseSpace connection.\n");
    owl.done();
    owl.close();
    for (const auto& d : destinations)
        closesocket(d.sd);
    networkCleanup();
    return 0;
}
