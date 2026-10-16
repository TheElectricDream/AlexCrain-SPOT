The platforms' data logger (`spot.hardware.CustomDataLogger`): each step's logged signals go to a text file, which **Collect** fetches after an experiment and `spot.experiment.decodeData` converts.

## createFile

Creates the run's log file in the program's folder: `ExpLog`, or `ExpLog1`, `ExpLog2`, ... if earlier ones exist, so a run never overwrites one that hasn't been collected. The name is kept for `appendDataToFile`.

## appendDataToFile

Appends one line to the log file: the `num_params` values of `exp_data`, separated by commas. The file is opened and closed for every line, so what was written survives a program that is stopped abruptly.
