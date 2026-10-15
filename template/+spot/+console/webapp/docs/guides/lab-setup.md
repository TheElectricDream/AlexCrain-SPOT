# Lab Setup

The local console's **Lab Setup** button (in the top bar, next to **Lab Console**) runs the lab's deployment scripts from the page instead of a terminal. It opens a dialog with each script: what it does, what to mind, and **Run**. What the script prints follows below as it runs, and the last run's result is kept. The code is [labsetup.js](file:labsetup.js) and [deploy.py](file:deploy.py); the scripts themselves are described in [Lab Administration](page:lab-admin).

| Action | Script | What it does |
|---|---|---|
| **Update the Lab Console** | `deploy_tx2.sh` | Copies the console to the TX2, compiles StreamData there and restarts the lab console. Run it after you change the console's code (the server, the page, StreamData), or to bring an older lab console up to date (the Experiment tab then stops saying **Needs Update**). |
| **Update SPOT Vision** | `deploy_orin.sh` | Copies SPOT Vision to the Orin and restarts it (the Vision tab's cameras). |
| **Link the Platforms** | `setup_hardware_link.sh` | For the platforms you tick: installs their agent and lets the TX2 log in to drive their Teensy and run experiments. Run it for a new or reinstalled platform, or after `spot_agent.sh` changes. |

One script runs at a time; **Cancel** stops it.

## What it needs

- **The lab network.** The button is disabled, and the server refuses, while the lab console can't be reached.
- **The lab's SSH key** on this computer, `~/.ssh/id_ed25519_spot`, which the TX2, the Orin and the platforms accept.
- `bash`, `ssh` and `rsync` on this computer: the scripts are bash scripts, and the dialog says if one is missing. On Windows, Lab Setup can't run them: run the scripts yourself from WSL or Git Bash (`+spot/+console/deploy`).

> [!NOTE]
> The platform script's power-saving step (Wi-Fi power saving off, no suspend) needs `sudo`, which asks for a password, so Lab Setup skips it. Run `./setup_hardware_link.sh` in a terminal once for each new platform to do that step.
