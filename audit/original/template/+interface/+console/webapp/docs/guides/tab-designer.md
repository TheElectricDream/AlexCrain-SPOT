# The Spacecraft Designer

What is mounted on each platform: the docking cone, the arm, the solar panels, and where. It changes two things: how the platforms look in the 3D view (with **Use CAD**) and, through the Arm, what the simulation includes. The code is [designer.js](file:designer.js) in the page and [design.py](file:design.py) in the server; the design is saved in `spot_design.json` next to the settings.

## The sidebar

- **Platform**: Chaser, Target or Obstacle. The 3D view shows that platform's CAD model close up, from the side with the attachment, with its body axes at its centre of gravity (the point its pose is, as on the other pages).
- **Docking Cone** (on the Chaser: **Docking Cone or Arm**): none, the **male** cone, the **female** cone, or (Chaser only) the **arm**. Then its **place**: the four faces drawn as seen from outside, two slots each (left / right); the back face is the control panel and has no slots. The slots a part would collide with are marked, and so is the arm's usual place (+Y face, left), which is where the arm's geometry in the settings assumes it (`spot.arm.mountGeometry` moves it with the mount).
- **Solar Panels**: include them, along X or along Y; a warning if the bracket would cross the ZED2's view.
- **Export**: saves the platform as shown, as one file another program can open (see [Exporting a platform](#exporting-a-platform)).
- **Lab Console**: whether the lab console's design is the same as this one, with **Send to Lab Console** and **Use Lab Console's Design**, so the lab's 3D view shows the same hardware.
- Where the design is saved.

## Exporting a platform

**Export GLB** and **Export STL** save the platform on show, with what is mounted on it (its docking cone or arm, its solar panels and, on the Chaser, the ZED2), as a single file in your browser's downloads. The file is named after the design, e.g. `SPOT_Target_Female-+Y-Left_Solar-Y.glb`. Both are in the platform's body frame from its centre, not its CG: the origin is at the centre of its footprint on the table, with z up and x out of the front (+X) face. An arm is drawn at the simulation's initial pose, as on the page.

- **GLB** (binary glTF 2.0, in metres): each part is a separate, named object (the platform, each side panel, the docking cone, the solar panels, the ZED2, the arm's links and end effector), with its colours. It opens in Blender, Windows' 3D Viewer, online glTF viewers and game engines. glTF is y-up, so the file turns the model to suit, and importers turn it back.
- **STL** (binary, in millimetres, z up): the surface only, as one body without colours. It opens in CAD programs (SolidWorks, Fusion, FreeCAD, Onshape) and slicers.

Both are meshes made from the models the page draws, not solid CAD bodies, so a CAD program imports them as a mesh or graphics body. For editable solids, use the SolidWorks/STEP assemblies the models were built from. Textures are left out: the solar cells keep their dark colour, and the MLI foil is plain gold or silver. The code is [export3d.js](file:export3d.js); [viewer.js](file:viewer.js)'s `designerModel` gives it the platform without the places to click or the body axes.

## The arm and the simulation

On the local console, the Chaser's **Arm** and the Simulation tab's **Arm** platform are one choice: picking the arm here ticks the Arm in the Scenario and sets its mount in the settings (`mass.arm.mount`); ticking the Arm there mounts it here (replacing the cone). Dropping it puts the male cone back.

## Runs flown with the arm

A run keeps its arm after the design changes: the last simulation, a loaded result or an experiment may have flown the arm while the Chaser here now carries a cone. The 3D view draws the Chaser as designed, so it hides such a run's arm (its links would otherwise hang off the cone), and the first time the run is replayed (on the Simulation or Experiment tab) the page asks whether that was intended:

- **Show the Arm**: draws the arm on these runs anyway, with its mount at the place the run had it (the nearest of the six places to the run's shoulder). The design doesn't change.
- **Not Now**: keeps it hidden on these runs; the next run flown with the arm asks again.
- **Never Ask Again**: always hides it, without asking, on this browser (`spot.designer.armNeverAsk` in its local storage). While that is set, the Chaser's card here has **Ask Again**, which turns asking back on and asks about the runs on show.

The prompt comes from [viewer.js](file:viewer.js), which hides the arm and reports each run once (`armChoice`), and [designer.js](file:designer.js), which asks.

## On the lab console

The same tab, without the simulation link: it decides what the lab console's 3D view draws when **Use CAD** is on. Keep it in step with yours with the Lab Console card above.

The CAD models themselves (`static/models/*.bin.gz`) are built from the lab's SolidWorks assembly by a tool kept with the CAD, outside the repository. The page loads them only when **Use CAD** is first switched on ([cad.js](file:cad.js)). The design does not change the mass properties or the 2D figures yet.
