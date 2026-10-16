The agent runs on each platform whenever the lab console logs in with its own key; the key only allows this script, and the command comes in `$SSH_ORIGINAL_COMMAND`. [Lab Administration](page:lab-admin) lists every command it accepts.

## need_name

Checks the experiment name given with a command (letters, digits, `_`, `.` and `-`, at most 100 characters, no `..`), and sets `DIR` to its folder under `~/ExperimentSoftware/spot`. Exits with an error otherwise, so no command can reach outside that folder.

## running_pid

Prints the process ID in `$DIR/run.pid` if that program is still running (and returns failure if it isn't), so commands know whether an experiment or the arm calibration is under way.

## users_of

Lists `<pid> <name>` for every program of this user that has the device `$1` open (the Teensy's port, or the arm's U2D2), by looking through `/proc/*/fd`. The lab console shows them as the reason a platform is **In Use**.
