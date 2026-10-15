# Quick Start: An Experiment in the Lab

Running your diagram on the platforms. Simulate it first ([Quick Start: Your First Simulation](page:quick-start-simulation)), and check the trajectory and the thruster duty cycles on the Results tab before you take it to the table.

## Before you start

- You are on the **lab network** (the lab's Wi-Fi, or a cable in the lab). The local console's badge says **Lab Connected** instead of *Lab Disconnected*.
- You have the TX2's password (from a lab administrator), the first time you build.
- The table and the platforms are prepared, with charged batteries and full air tanks ([Preparing the Lab](page:lab-procedures)).
- You know where the **E-stop** is (on the ground station table) and how to use it ([Safety and Emergencies](page:lab-safety)).

## 1. Check the platforms

Open the lab console (the top bar's **Lab Console ↗**, or `http://192.168.1.116:8080`). On its **Hardware** tab, each platform taking part should show **Ready**. Switch the pucks on and off, and run a thruster check, to hear every valve open ([The Hardware Tab](page:tab-hardware)).

## 2. Start the cameras

On the local console's **Experiment** tab ([The Experiment Tab](page:tab-experiment)), the **Lab** card: **Start Cameras** sends this project's marker file and starts PhaseSpace at the Data Rate. The 3D view shows the platforms where the cameras see them. Check that each platform is tracked (its LEDs visible, its pose steady).

## 3. Build

In the **Build** card:

1. **Check**: what the build will do, with any warnings. Nothing is compiled.
2. **Connect**: MATLAB's connection to the TX2 (enter the TX2's password the first time).
3. **Build**: generates the code for each platform taking part and compiles it on the TX2. It takes several minutes; the log follows in the card.

Build again whenever you change the diagram or a setting.

## 4. Deploy and run

1. Put the platforms on the table near their initial positions (the pucks off).
2. **Deploy** copies the programs to the platforms.
3. **Start Experiment**: each platform's program starts, and once they all report their clocks the lab console counts them in, so they leave phase 0 together. The 3D view shows them live, the Messages panel shows what each is doing, and the Live Plot shows your Live Stream signals.
4. Watch, with a hand near the E-stop. **Stop** ends it at any time; the programs end by themselves after phase 5.

## 5. Collect the data

**Collect** fetches each platform's log, saves it in `Saved Data/`, and converts it in MATLAB. The run appears in the Results tab's **Saved Data…** list next to your simulations, with the same signal names: plot the two together, or replay the experiment over the simulation with **Compare**.

## At the end of the session

Follow [Preparing the Lab](page:lab-procedures)'s shutdown steps: platforms off the table, air released, batteries charging, table covered.
