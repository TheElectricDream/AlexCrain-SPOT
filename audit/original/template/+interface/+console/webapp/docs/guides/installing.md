# Setting Up Your Computer

## What you need

| | |
|---|---|
| **MATLAB** | **R2026a or newer**: the template diagram is saved in R2026a, and older releases can't open it. |
| for simulations | Simulink, the DSP System Toolbox, and the Control System Toolbox (for the LQR gains) |
| for experiments | also MATLAB Coder, Simulink Coder, Embedded Coder, and the *MATLAB Coder Support Package for NVIDIA Jetson and NVIDIA DRIVE Platforms* (MATLAB: Home › Add-Ons) |
| **Python** | 3.6 or newer, standard library only (nothing to `pip install`). It runs the local console's small web server. If it is missing, `SPOT` offers to install it. |
| **A web browser** | any current browser (Chrome, Firefox, Edge, Brave, Safari) |
| for experiments | a connection to the lab network (the lab's Wi-Fi, or a cable in the lab), and the TX2's password from a lab administrator (needed once, to build) |

## Windows

1. Install MATLAB R2026a with the toolboxes above.
2. Python: nothing to do beforehand. If `SPOT` can't find one, it says so and offers **Install Python**, which installs it for your user only (with `winget`, or python.org's installer; no administrator rights needed). To install it yourself, use [python.org](https://www.python.org/downloads/) and tick *Add python.exe to PATH*. SPOT tries the Python MATLAB is set up to use (`pyenv`), then `py -3`, `python`, `python3`, then where the installers put it (`interface.console.findPython`).
3. Make your project ([Making a New Project](page:new-project)). Avoid folders synced by OneDrive while building experiments: syncing can lock files.
4. In MATLAB, `cd` into the project folder and type `SPOT`.
5. If Windows Firewall asks whether Python may use the network, *private networks* is enough: the local console only listens on your own computer (127.0.0.1).

> [!NOTE]
> As of Pre-Release.3, SPOT has not yet been tested on Windows, including starting the console and installing Python from SPOT. Please report what you find.

## Linux

1. Install MATLAB R2026a with the toolboxes above.
2. Python 3 comes with most distributions. If it is missing, `SPOT` offers **Install Python**, which uses your package manager (apt, dnf, yum, zypper or pacman) and asks for your password in a system dialog; without a desktop it prints the command instead (for example `sudo apt install python3`).
3. The local console opens your default browser with `xdg-open`.
4. Make your project, `cd` into it in MATLAB, and type `SPOT`.

> [!TIP]
> If MATLAB runs in a container (for example with `distrobox`), nothing extra is needed: SPOT fixes the shell MATLAB uses for `system` when the container doesn't set one (`interface.console.ensureShell`), and opens the browser on the host.

## First start

```matlab
SPOT          % start the console and open it in your browser
SPOT open     % open the page again (after closing the tab), and print its address
SPOT stop     % stop the console
```

MATLAB starts the console's web server, connects its **worker** to it, and opens *SPOT Local Simulations*. The address it prints carries a session key: the page only takes commands from a browser that opened that link (the key is then kept for that tab). Keep MATLAB open while you use the console: simulations, builds and the page's MATLAB command window all run in it. The **MATLAB** badge at the bottom of the Simulation tab says whether it is connected and what it is doing.

| If... | then... |
|---|---|
| "needs Python 3.6 or newer" | click **Install Python**, run `interface.console.installPython`, install it yourself, or point MATLAB at one: `pyenv('Version', 'C:\...\python.exe')` |
| the page says it can't talk to MATLAB | open it with the link MATLAB printed, or run `SPOT open` |
| the browser doesn't open | copy the address `SPOT open` prints into your browser |
| **Lab Disconnected** (top right) | you are not on the lab network. Simulations and results still work; experiments and the live view need the lab network |
| the console doesn't start | its log is `.spot_cache/console.log` in the project folder |
