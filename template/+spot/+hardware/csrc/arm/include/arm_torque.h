#ifndef SPOT_ARM_TORQUE_H
#define SPOT_ARM_TORQUE_H
/* The arm's three Dynamixel MX-64(2.0) servos (IDs 1-3) in current control, for
   spot.hardware.ArmTorque. Self-contained (protocol 2.0, Linux serial); see arm_torque.cpp. */
#ifdef __cplusplus
extern "C" {
#endif

/* Opens the port and sets the servos up (torque off, current control mode, Current Limit,
   Return Delay Time 0 if zeroReturnDelay, goal current 0, torque on). Returns 1 if every servo
   answered, 0 if the port or a servo failed (the other calls then do what they can). */
int arm_torque_open(const char *port, double currentLimit, double zeroReturnDelay);

/* One sync read of Present Current, Velocity and Position from the three servos.
   pos [pulses, 2048 = straight out], vel [0.229 rev/min], cur [3.36 mA]: a servo that did not
   answer keeps its last values. status: [servos that answered (0-3), the read's time [ms],
   the adapter's latency timer [ms] (-1 unknown), servo reads missed since the start,
   1 if a servo's reply had its Alert bit (a hardware error, e.g. overload: see its Hardware
   Error Status)]. */
void arm_torque_read(double pos[3], double vel[3], double cur[3], double status[5]);

/* Sends each servo its Goal Current [3.36 mA units] (one sync write, no reply). */
void arm_torque_write(const double goal[3]);

/* Goal currents 0, torque off, port closed. */
void arm_torque_close(void);

#ifdef __cplusplus
}
#endif
#endif
