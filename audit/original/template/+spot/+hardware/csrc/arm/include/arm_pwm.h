#ifndef SPOT_ARM_PWM_H
#define SPOT_ARM_PWM_H
/* The arm's three Dynamixel MX-64(2.0) servos (IDs 1-3) in PWM (voltage) control, for
   spot.hardware.ArmPWM. Self-contained (protocol 2.0, Linux serial); see arm_pwm.cpp. The units
   below are Constants.CENTER_PULSE, VEL_UNIT_RPM and PWM_FULL (Constants.m). */
#ifdef __cplusplus
extern "C" {
#endif

/* Opens the port and sets the servos up (torque off, PWM control mode, PWM Limit, Return Delay
   Time 0 if zeroReturnDelay, Goal PWM 0, torque on). Returns 1 if every servo answered, 0 if
   the port or a servo failed (the other calls then do what they can). */
int arm_pwm_open(const char *port, double pwmLimit, double zeroReturnDelay);

/* One sync read of Present PWM, Velocity and Position from the three servos.
   pos [pulses, 2048 = straight out], vel [0.229 rev/min], pwm [885 = 100 %]: a servo that did
   not answer keeps its last values. status: [servos that answered (0-3), the read's time [ms],
   the adapter's latency timer [ms] (-1 unknown), servo reads missed since the start,
   1 if a servo's reply had its Alert bit (a hardware error, e.g. overload: see its Hardware
   Error Status)]. */
void arm_pwm_read(double pos[3], double vel[3], double pwm[3], double status[5]);

/* Sends each servo its Goal PWM [885 = 100 %], rounded and within the PWM Limit (one sync
   write, no reply). */
void arm_pwm_write(const double goal[3]);

/* Goal PWM 0, torque off, port closed. */
void arm_pwm_close(void);

#ifdef __cplusplus
}
#endif
#endif
