## nowMs

The time [ms] from a monotonic clock, for timing the servos' replies.

## crc16

The CRC-16 that Dynamixel protocol 2.0 packets end with (polynomial 0x8005), computed over the packet so far.

## send

Writes one packet to the serial port (after discarding anything left over from an earlier reply), waiting with `poll` if the port is busy, for at most 20 ms. Returns false if it couldn't.

## syncWriteCurrents

Sends the three servos their goal currents in one broadcast *Sync Write* (no reply), each rounded to whole current units and limited to the Current Limit. A goal that isn't a number is sent as 0.
