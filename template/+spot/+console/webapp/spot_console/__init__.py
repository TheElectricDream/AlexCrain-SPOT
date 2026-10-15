"""The SPOT Console's server, one module per area.

Start it with webapp/app.py.

    common       project paths, the API error, checking request values
    events       console settings, the event hub that feeds every
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
    files        the folder browser
    server       the HTTP API and start-up (lab or local console)

Python 3.6 standard library only (the lab console runs on the TX2's
Python 3.6.9).
"""
