# Live Data Streams

**Priority 73.** Three **Live Stream** blocks that send whatever is wired into them to the console's **Live Plot** tab, every step: during a simulation (live while it runs, if Real Time is on) and during an experiment (over the network, from each platform).

![Live Data Streams](images/sub_Live_Data_Streams.png)

**Reads:** in the template, `RED_Measured_States`, `BLACK_Measured_States`, `BLUE_Measured_States` (one per block), plus `Univ_Time`, `WhoAmI`, `isSim` inside each block.

Each **Live Stream n** is a masked subsystem with one input *u* (any length):

![Live Stream 1](images/sub_Live_Data_Streams_Live_Stream_1.png)

- **Stream**: `spot.hardware.liveStream(streamLabel, t, u, whoAmI, isSim, labConsoleIP)` (`spot.hardware.liveStream`); `streamLabel` is the mask's parameter, filled by `spot.hardware.streamName` from the name of what is connected (a Data Store Read's store, a signal's label) unless you type a name into the block (double-click it).
- **on a platform**: a UDP packet to the lab console (RED 55011, BLACK 55012, BLUE 55013) with the name, the time and the values; the lab console keeps them and the Experiment tab's Live Plot draws them ([experiment.py](file:experiment.py), [liveplot.js](file:liveplot.js)).
- **in a simulation**: the samples are handed to MATLAB (`spot.sim.liveData`), which forwards them to the console as it runs and as a whole at the end.

To plot something else, connect it to a block's input (a Data Store Read of `RED_Controls`, a signal out of your controller) or copy a block: the console shows a tick box per stream and a chip per element. The blocks read `Univ_Time` for the time stamp, so they run after everything else (73): the streams show the step's final values, and Simulink's *read-before-write* diagnostics stay quiet.
