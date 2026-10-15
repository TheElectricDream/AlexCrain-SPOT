## network_startup

Starts Windows' socket library the first time a socket is opened; nothing to do on Linux.

## spot_udp_now

A steady clock [s] for measuring intervals (a packet's age): the performance counter on Windows, `CLOCK_MONOTONIC` on Linux. Its zero is arbitrary.

## rx_close

Stops a receiving port: tells its thread to stop, waits for it (the socket's 0.2 s timeout lets it notice), closes the socket and frees its lock.
