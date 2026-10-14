/* spot_udp.h -- the UDP used by the spacecraft program: receive the newest packet, send without
 * blocking. Every UDP link in the diagram goes through here (spot.hardware.udpReceive,
 * spot.hardware.udpSend, spot.hardware.phasespaceLatest).
 *
 * Receiving: Simulink's UDP Receive block returns the OLDEST queued packet each model step. When
 * the sender is faster than the model (or the model once fell behind), the queue fills and the
 * model reads data many steps old for the rest of the run. Here a background thread per port
 * reads every packet as it arrives and keeps only the newest, with its age.
 *
 * Sending: one socket per destination, and a send never waits, so a destination that is off
 * the network can't hold up the model or the other destinations.
 *
 * Linux (the Jetsons) and Windows. Opening is idempotent: calling an open function again with the
 * same arguments returns the same handle, so the MATLAB side can simply call it every step.
 */
#ifndef SPOT_UDP_H
#define SPOT_UDP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Listen on UDP <port> (all interfaces) and keep the newest packet. Returns a handle >= 0, or -1.
 *   allowed_ip  only accept packets from this sender ("" or "0.0.0.0": any sender)
 *   min_bytes   ignore shorter packets (0: no limit)
 *   max_bytes   ignore longer packets (0: no limit); min = max = 460 accepts only that size */
int spot_udp_rx_open(int port, const char *allowed_ip, int min_bytes, int max_bytes);

/* Copy the newest packet's first nbytes into out (zeros where nothing has arrived yet, or past
 * the end of a shorter packet). age_seconds: time since it arrived, -1 if nothing yet.
 * Returns how many packets arrived since the previous read (0 = nothing new). */
int spot_udp_rx_read(int handle, void *out, int nbytes, double *age_seconds);

/* A socket that sends to ip:port (a broadcast address such as 255.255.255.255 is allowed).
 * Returns a handle >= 0, or -1. */
int spot_udp_tx_open(const char *ip, int port);

/* Send nbytes from data as one packet, without waiting. Returns the bytes sent, or -1. */
int spot_udp_tx_send(int handle, const void *data, int nbytes);

/* A steady clock in seconds (not the time of day; for measuring intervals). */
double spot_udp_now(void);

/* Stop every receive thread and close every socket. */
void spot_udp_close_all(void);

#ifdef __cplusplus
}
#endif

#endif /* SPOT_UDP_H */
