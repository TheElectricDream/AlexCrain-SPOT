# Welcome to SPOT

SPOT, the **Spacecraft Proximity Operations Testbed**, is the testbed of Carleton University's Spacecraft Robotics and Control Laboratory: three spacecraft platforms that float on air bearings over a 3.5 m × 2.4 m granite table, so they move as spacecraft do in a plane, without friction. You design a guidance and control law in a Simulink diagram, simulate it on your own computer, then build it and run it on the platforms.

| Name in the code | Name on screen | What it is |
|---|---|---|
| `RED` | **Chaser** | the active spacecraft; it carries the robotic arm and a ZED2 stereo camera |
| `BLACK` | **Target** | the spacecraft being approached; it carries the docking cup |
| `BLUE` | **Obstacle** | a third platform that gets in the way |
| `ARM` | **Arm** | the Chaser's three-joint arm (Dynamixel servos) |

The consoles show the names and colours (Chaser silver, Target gold, Obstacle blue); the code, the diagram, the settings and the data keep `RED`, `BLACK` and `BLUE`, so `RED_Px_m` is the Chaser's x position.

Each platform has eight cold-gas thrusters fed by an air tank, three air bearings ("pucks"), a Jetson computer that runs the program built from your diagram, and a Teensy micro-controller that opens the thruster valves. A PhaseSpace motion-capture system tracks LEDs on each platform, and the **lab console**, a Jetson TX2, streams their positions to the platforms.

## The two consoles

You use SPOT through two web pages, called **consoles**:

- **The local console** (*SPOT Local Simulations*) runs on your computer; type `SPOT` in MATLAB, in your project folder, to start it. Its tabs are Simulation, Experiment, Results, Designer and Documentation. Everything it does is done by your MATLAB session.
- **The lab console** (*SPOT Hardware Interfaces*) runs on the TX2 at `http://192.168.1.116:8080`, on the lab network. Its tabs are PhaseSpace, Hardware, Designer, Calibration, Vision and Documentation. The local console's Experiment tab talks to it for you.

[How It Fits Together](page:architecture) draws the whole system.

## Where to start

1. [Setting Up Your Computer](page:installing) and [Making a New Project](page:new-project).
2. [Quick Start: Your First Simulation](page:quick-start-simulation).
3. [Quick Start: An Experiment in the Lab](page:quick-start-experiment), after [Preparing the Lab](page:lab-procedures).
4. The diagram you will edit: [Overview and Step Order](page:diagram-overview), then [Writing Your Own Guidance and Control](page:diagram-your-code).

## This documentation

It has two halves:

- **Guides** (these pages): how to use SPOT and the lab, the console tab by tab, the Simulink diagram, and the maths behind the controllers. They are Markdown files in `+interface/+console/webapp/docs/guides`, so they travel with every project.
- **Reference**: a page for every function in the project, in MATLAB, Python, JavaScript, C/C++ and shell, and for every MATLAB Function block in your diagrams. These pages are read from the code itself each time it changes, so they always match it: what the function is for (its help text, docstring or JSDoc comment), then its code, block by block, under the comments that explain each block, with what it calls and what calls it.

Search (press `/`) finds functions by name, by what their documentation says, and by any word in their code; **Find** (Ctrl+F) highlights words on the page. On the local console you can edit any file from its page and save it; the editor checks the code as you type. [Using the Documentation Tab](page:tab-documentation) has the details.
