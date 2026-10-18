# The Spacecraft Designer

What is mounted on each platform: the docking cone, the arm, the solar panels, and where. It changes two things: how the platforms look in the 3D view (with **Use CAD**) and, through the Arm, what the simulation includes. The code is [designer.js](file:designer.js) in the page and [design.py](file:design.py) in the server; the design is the `DOCK_*` and `SOLAR_*` constants in the project's `Constants.m`, written through MATLAB, next to the settings.

## The sidebar

- **Platform**: Chaser, Target or Obstacle. The 3D view shows that platform's CAD model close up, from the side with the attachment.
- **Docking Cone** (on the Chaser: **Docking Cone or Arm**): none, the **male** cone, the **female** cone, or (Chaser only) the **arm**. Then its **place**: the four faces drawn as seen from outside, two slots each (left / right); the back face is the control panel and has no slots. The slots a part would collide with are marked, and so is the arm's usual place (+Y face, left), which is where the arm's geometry in the settings assumes it (`spot.arm.mountGeometry` moves it with the mount).
- **Solar Panels**: include them, along X or along Y; a warning if the bracket would cross the ZED2's view.
- **Lab Console**: whether the lab console's design is the same as this one, with **Send to Lab Console** and **Use Lab Console's Design**, so the lab's 3D view shows the same hardware.
- Where the design is saved.

## The arm and the simulation

On the local console, the Chaser's **Arm** and the Simulation tab's **Arm** platform are one choice: picking the arm here ticks the Arm in the Scenario and sets its mount in the settings (`mass.arm.mount`); ticking the Arm there mounts it here (replacing the cone). Dropping it puts the male cone back.

## On the lab console

The same tab, without the simulation link: it decides what the lab console's 3D view draws when **Use CAD** is on. Keep it in step with yours with the Lab Console card above.

The CAD models themselves (`static/models/*.bin.gz`) are built from the lab's SolidWorks assembly by a tool kept with the CAD, outside the repository. The page loads them only when **Use CAD** is first switched on ([cad.js](file:cad.js)). The design does not change the mass properties or the 2D figures yet.
