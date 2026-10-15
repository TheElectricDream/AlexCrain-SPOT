# Archive

Everything that used to live in the project folder but is not needed to run simulations or experiments now sits beside the template, in `../archive/` (git-ignored, so not on GitHub). Nothing in the project refers to it at run time; it is kept for history and for anyone who needs the old GUI. The lab's CAD is in its own folder beside it, `../designs/` (also git-ignored), with `cad_tools/`: `build_cad_models.py` (CAD → the console's `spot_cad.bin.gz`, `spot_table.bin.gz`, `solar_cells.png`; run `python3 designs/cad_tools/build_cad_models.py` from the top folder after a part changes) and `step_mesh.py` (STEP → triangles).

| Folder | What it is | When you would open it |
|---|---|---|
| `Legacy/` | The old App Designer GUI (`SPOT_GUI.mlapp` and friends), `Custom_Library`, `Run_Initializer.m`, the earlier template diagrams (`original_diagram/`), the old `Saved Data` layout | To read how something was done before the console; `spot.params.fromLegacyState` can still load its saved GUI states |
| `Pictures/` | Photographs of the lab and the platforms | Documentation |
| `Resources_unused/` | Images and files the old GUI used that nothing uses now | – |
| `Manuals/` | Datasheets and manuals (Dynamixel, PhaseSpace, Teensy, …) | Hardware questions |
| `Saved Data/` | Runs saved with earlier versions | Old results |
| `camera/` | The ZED2 camera block and code (`+spot/+hardware/camera` before the move) | If the camera is brought back into the diagram |
| `old_arm_blocks/` | The earlier Dynamixel blocks (`Dynamixel_Controller.m`, `ReadArm_Load.m`, `ReadArm_Position_Rates.m`) and their C sources (`csrc_dynamixel/`): position/velocity control done in the servos, read at ~2 Hz | Replaced by [[spot.hardware.ArmTorque|spot-hardware-ArmTorque]] (torque control, encoders read every step) |
| `New_SPOT_Project.py`, `.bat` | The Python project creator and its Windows launcher | Replaced by [[New_SPOT_Project|Tool-New-SPOT-Project]] (MATLAB) |
| `SPOT_5_0_0_Pre_Release_1_Patch_Notes.pdf` | The notes of the 5.0.0 pre-release | History |

The `README.md` in that folder says the same in more detail.

## Why the split

A test user needs the template (what is on GitHub) and nothing else; the CAD alone is 200 MB and the old saved data another 300 MB. Keeping the project small also keeps `New_SPOT_Project` fast and the git history clean.
