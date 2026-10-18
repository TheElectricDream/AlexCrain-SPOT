"""The SPOT Console's server, one module per area.

Start it with webapp/app.py.

    common       project paths, the API error, checking request values
    constants    Constants (Constants.m): from the user's MATLAB session
                 through the MATLAB Engine (the local console), or parsed
                 from the file (the lab console, no MATLAB)
    events       the lab console's settings, the event hub that feeds every
                 open page, running commands
    phasespace   compiling and running StreamData, the packet
                 monitor, marker calibration
    hardware     Teensy packets, SSH links to the platforms' agents,
                 pucks/thrusters/checks
    control      the template diagram's default LQR and thruster
                 mixer, ported from +spot/+control
    interactive  Interactive Mode: flying platforms to poses set on
                 the PhaseSpace tab, with control
    experiment   deploying, starting, stopping and collecting
                 experiments on the platforms
    local        the local console: the MATLAB worker bridge,
                 simulation/build runs, the lab link
    project      the local console's design and calibration: views over
                 the constants, written through the MATLAB worker
    design       the Spacecraft Designer's design (the lab console's file)
    calibration  the Calibration tab's runs and the lab's calibration file
    files        the folder browser
    docs         the Documentation tab: the index of the code, the
                 guides and notes, saving edited files
    docparse     reading the code: functions, their documentation,
                 their blocks, what calls what
    doclint      checking edited code (Python, MATLAB, shell, C/C++,
                 JSON, XML, the guides' links)
    server       the HTTP API and start-up (lab or local console)

Python 3.6 standard library only (the lab console runs on the TX2's
Python 3.6.9); the local console adds the MATLAB Engine for Python.
"""
