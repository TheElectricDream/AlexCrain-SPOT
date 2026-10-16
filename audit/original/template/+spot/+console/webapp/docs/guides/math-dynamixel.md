# Dynamixel Units

The MX-64(2.0) (protocol 2.0) speaks in its own integer units. Everywhere in the code (the hardware block, the plant, the controllers, the calibration script) the same conversions appear; here they are in one place.

| Quantity | Servo unit | Conversion | In the code |
|---|---|---|---|
| Position | pulse, 4096 per revolution; 2048 = the joint straight out | rad = (pulses − 2048) · 2π/4096 | `PULSES = 4096/(2π)` pulses per rad |
| Goal position (old SPOT code) | pulse | pulses = round(651.74 · rad + 2048) | `CMD_PULSES = 651.74` (≈ 4096/2π = 651.90; the old hardware code's constant, kept so goals mean what they always meant) |
| Velocity | 0.229 rev/min | rad/s = units · 0.229 · 2π/60 | `VEL_UNIT = 0.229·2π/60` ≈ 0.02398 rad/s |
| Goal velocity (old SPOT code) | 0.229 rev/min | units = round(rad/s · 9.5493 · 4.3668) | `9.549296585513702 * 4.366756307` (rad/s → rpm → units) |
| Current (not used) | 3.36 mA | A = units · 0.00336 | – (too coarse at the arm's torques; PWM control doesn't need it) |
| Profile acceleration | 214.577 rev/min² | rad/s² = units · 214.577 · 2π/3600 | `ACC_UNIT` |
| PWM (the arm's command, `ARM_Controls`) | 885 = 100 % | V = u/885 · supply | `PWM_FULL = 885` |
| Torque at the joint | – | N m = g (V − K_e ω), g = stall torque / 12 V = 0.5 N m/V; at rest ≈ 0.0068 N m per PWM unit at 12 V | `armModel.motor.torquePerVolt` |
| Back-EMF at the joint | – | K_e = 12 V / no-load speed (63 rpm) ≈ 1.82 V s/rad | `armModel.motor.backEMF` |

Limits used by SPOT:

| Register | Value | Meaning |
|---|---|---|
| Min / Max Position Limit | 1024 / 3072 pulses | ±90° |
| Velocity Limit | 1023 | ≈ 24.5 rad/s (the servo's maximum) |
| PWM Limit | 885 | 100 % (set by the hardware block; the default controllers have their own, 80 for position control) |
| Position P / I / D Gain | 400 / 0 / 200 | the servos' own (the diagram's controller uses retuned values at 20 Hz) |
| Velocity P / I Gain | 100 / 1920 | the servos' own |
| Operating Mode | 16 | PWM (voltage) control, set by the hardware block; 0 current control was its mode until October 2026; 1 velocity, 3 position were the old blocks' modes |
| Return Delay Time | 250 (500 µs) by default; 0 with `ZeroReturnDelay` | how long a servo waits before replying |

The control-table addresses and the protocol framing are in [arm_pwm.cpp](file:arm_pwm.cpp).
