# spot.control: controller design

`+spot/+control/` has two halves.

**Design**: the platform model and the LQR gains the diagram's default controller uses, computed from the settings every time they change:

```matlab
sys = spot.control.stateSpace(mass, inertia, Ts);   % discrete double integrator at Ts
K   = spot.control.lqrGains(sys, Q, R);             % 3x6, u = -K x
F   = spot.control.nominalForces(0.2825);           % 8x1 thrust of each thruster
```

**The diagram's controller as plain functions**: the LQR law and the thruster mixer, copied *verbatim* from the diagram's MATLAB Function blocks and wired together, so they can be read, run at the command line, and checked. The lab console's Interactive Mode runs the same two functions in Python ([[control.py|Console-control]]); [[verifyConsoleController|spot-control-verifyConsoleController]] checks the three against each other.

| Function | One line |
|---|---|
| [[stateSpace|spot-control-stateSpace]] | zero-order-hold discretisation of the planar rigid body |
| [[lqrGains|spot-control-lqrGains]] | `dlqr` with diagonal weights |
| [[nominalForces|spot-control-nominalForces]] | one thrust value → the eight thrusters |
| [[lqrCommand|spot-control-lqrCommand]] | the block "4 - Control/Default LQR Control": forces and torque for one step |
| [[thrusterDutyCycles|spot-control-thrusterDutyCycles]] | the block "Thruster & Float Control Code/Change … Behavior": eight duty cycles |
| [[verifyConsoleController|spot-control-verifyConsoleController]] | four checks: block code, simulation, Python port, defaults |

The maths: [[Platform Dynamics and LQR|Math-Platform-Dynamics-and-LQR]], [[The Thruster Mixer|Math-Thruster-Mixer]].
