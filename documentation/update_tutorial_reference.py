"""Refresh the function reference (Appendix A and B) of SPOT_Tutorial.tex from the code.

Reads every function in +spot (its signature, the optional inputs from its nargin checks, and its help
line) and rewrites the part of SPOT_Tutorial.tex between the "BEGIN/END generated function reference"
markers. Run it after adding or changing functions, then rebuild the PDF:

    python3 documentation/update_tutorial_reference.py
    cd documentation && latexmk -pdf SPOT_Tutorial.tex
"""
import glob, os, re, sys
root = sys.argv[1] if len(sys.argv) > 1 else os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(root)

PKG = [('arm', 'The Simulated Arm: Servos, Friction, Calibration'),
       ('console', 'Starting and Stopping the Local Console'),
       ('control', 'Controller Design'),
       ('data', 'Results: Saved Runs, Signals, 3D Playback, Figures'),
       ('experiment', 'Experiments on the Platforms'),
       ('hardware', 'Code the Spacecraft Program Uses to Talk to Its Hardware'),
       ('massprops', 'Mass Properties'),
       ('params', 'Settings'),
       ('plotting', 'Drawing'),
       ('sim', 'Simulations'),
       ('worker', "The MATLAB Worker That Runs the Console's Commands"),
       ('workspace', "The Console's Workspace Panel and Command Window")]

# One-sentence purposes where the help line is too long or written for a block
PURPOSE = {
 'packCommand': "The 37-byte command packet for a platform's Teensy (8 thruster duty cycles and the pucks).",
 'PhaseSpaceUdp': "Adds the PhaseSpace receiver's C code to the generated program and calls it (used by phasespaceLatest).",
 'phasespaceLatest': "The newest PhaseSpace packet every model step, for the diagram's ``Receive PhaseSpace Data'' block.",
 'CustomDataLogger': "Appends each step's logged values to the platform's ExpLog file (the diagram's Data Logger block).",
 'Debugging': "Prints its input on the platform's console while an experiment runs.",
 'ArmTorque': "The arm's three Dynamixel servos in current control: reads their encoders and torques before the controllers, sends the joint torques at the end of the step (the Dynamixel Arm (Torque) block).",
 'IdentifyPlatform': "Which platform the program runs on (1 RED, 2 BLACK, 3 BLUE), from its hostname (the WhoAmI block).",
 'Plant': "The Chaser with its arm and three Dynamixel servos in current control (with friction), integrated at 1 ms within each diagram step (the RED + ARM Plant block).",
 'PositionControl': "The servos' own position control law (profile, PID), run in the diagram and giving joint torques (the Default Position Control (ARM) block).",
 'VelocityControl': "The servos' own velocity control law (PI), run in the diagram and giving joint torques (the Default Velocity Control (ARM) block).",
 'Worker': "Runs the console's MATLAB commands in this session (the commands are listed in Appendix~B).",
 'reset': "The template's default settings, keeping the diagram, after backing up the settings file.",
 'update': "New settings from the console: the defaults overlaid with what the page sent, re-reading the solver for a new diagram.",
}
# How a block is used: its inputs and outputs (MATLAB System blocks in the diagram)
BLOCKS = {
 'ArmTorque': 'block: (torque) $\\rightarrow$ (q, dq, torque, status)',
 'CustomDataLogger': 'block: (data, size) $\\rightarrow$ (none)',
 'Debugging': 'block: (value) $\\rightarrow$ (none)',
 'IdentifyPlatform': 'block: () $\\rightarrow$ platformId',
 'PhaseSpaceUdp': 'spot.hardware.PhaseSpaceUdp.open(port, senderIP, bytes); [packet, n, age] = ...read(n)',
 'Plant': "p = spot.arm.Plant('Model', armModel[, 'FixedBase', true]); [pos, vel, torque] = p(F_RED, jointTorques)",
 'PositionControl': "c = spot.arm.PositionControl('Motor', armModel.motor, 'StepTime', baseRate); tau = c(goal, [q; dq])",
 'VelocityControl': "c = spot.arm.VelocityControl('Motor', armModel.motor, 'StepTime', baseRate); tau = c(goalSpeeds, [q; dq])",
 'Worker': 'w = spot.worker.Worker(root); result = w.dispatch(cmd, args); w = spot.worker.Worker.instance()',
}

def tex(s):
    s = s.replace('\\', '\\textbackslash{}')
    for a, b in [('_', '\\_'), ('%', '\\%'), ('&', '\\&'), ('#', '\\#'), ('{', '\\{'), ('}', '\\}'), ('~', '\\textasciitilde{}'), ('^', '\\^{}')]:
        s = s.replace(a, b)
    s = s.replace('\\textbackslash\\{\\}', '\\textbackslash{}')
    return re.sub(r'"([^"]*)"', r"``\1''", s)

def h1(src):
    for line in src.split('\n'):
        m = re.match(r'\s*%([A-Za-z_][A-Za-z0-9_]*)\s+(.*)', line)
        if m:
            return m.group(2).strip()
    return ''

def call(pkg, name, src):
    m = re.search(r'^\s*function\s+(?:(\[[^\]]*\]|\w+)\s*=\s*)?(\w+)\s*\(([^)]*)\)', src, re.M)
    if not m:
        m2 = re.search(r'^\s*function\s+(?:(\[[^\]]*\]|\w+)\s*=\s*)?(\w+)\s*$', src, re.M)
        out, args = (m2.group(1) if m2 else None), []
    else:
        out, args = m.group(1), [a.strip() for a in m.group(3).split(',') if a.strip()]
    # optional inputs, from the function's own nargin checks
    req = len(args)
    for k in re.findall(r'nargin\s*<\s*(\d+)', src):
        req = min(req, int(k) - 1)
    for k in re.findall(r'nargin\s*>\s*(\d+)', src):
        req = min(req, int(k))
    shown = ', '.join(args[:req])
    if req < len(args):
        shown += ('[, ' if req else '[') + ', '.join(args[req:]) + ']'
    lhs = (out.replace(' ', '').replace(',', ', ') + ' = ') if out else ''
    return '%sspot.%s.%s(%s)' % (lhs, pkg, name, shown)

out = []
total = 0
for pkg, title in PKG:
    files = sorted(glob.glob('+spot/+%s/*.m' % pkg), key=lambda f: os.path.basename(f).lower())
    files = [f for f in files if not f.endswith('Contents.m')]
    out.append('\\subsection*{spot.%s \\textmd{\\textendash\\ %s}}' % (pkg, tex(title)))
    out.append('\\begin{xltabular}{\\linewidth}{@{}>{\\raggedright\\arraybackslash\\ttfamily\\footnotesize}p{0.47\\linewidth} >{\\raggedright\\arraybackslash\\small}X@{}}')
    out.append('\\toprule \\normalfont\\small\\textbf{Call} & \\textbf{Purpose} \\\\ \\midrule \\endhead')
    for f in files:
        name = os.path.basename(f)[:-2]
        src = open(f, encoding='utf-8', errors='replace').read()
        c = BLOCKS.get(name)
        if c is None:
            c = tex(call(pkg, name, src))
        elif c.startswith('block:'):
            c = '\\textrm{MATLAB System block} spot.%s.%s %s' % (pkg, tex(name), c[len('block:'):].replace('_', '\\_'))
        else:
            c = tex(c)
        purpose = PURPOSE.get(name) or tex(h1(src))
        if not purpose.endswith('.'):
            purpose += '.'
        out.append('%s%s & %s \\\\' % ('{}' if c.startswith('[') else '', c, purpose))
        total += 1
    out.append('\\bottomrule\\end{xltabular}')

# The console's commands (what the page asks MATLAB to do)
CMDS = [('ping, info', 'Check that MATLAB answers; its release and toolboxes.'),
 ('settings.get, settings.set', 'Read or replace the settings (spot.params.update), then apply them.'),
 ('settings.reset', 'Back to the defaults (spot.params.reset).'),
 ('settings.open, settings.saveAs', 'Load or save settings through MATLAB\'s file dialogs.'),
 ('diagram.open', 'Open the diagram in Simulink.'),
 ('sim.run, sim.stop', 'Run the simulation (spot.sim.run), or stop it.'),
 ('experiment.plan, .connect, .build', 'spot.experiment.plan, connect and build.'),
 ('experiment.decode', 'Convert collected ExpLog files (spot.experiment.decodeData).'),
 ('data.list, data.load, data.unload', 'Saved Data, and the datasets loaded for the Results tab (spot.data.listSaved, load).'),
 ('data.series, data.trajectory', 'A signal to plot, or a dataset\'s motion for the 3D view (spot.data.series, trajectory).'),
 ('data.saveSimulation, data.figure', 'Save the last simulation; draw or export the plots (spot.data.toFigure, exportFigure).'),
 ('massprops.writeTracker', 'Write the PhaseSpace marker calibration (spot.massprops.writeTrackerPositions).'),
 ('workspace.list, .inspect, .eval', 'The Workspace panel and the command window (spot.workspace.*).')]
out.append('\\section{The Console\'s Commands and the Non-MATLAB Parts}')
out.append('The local console asks MATLAB for everything through \\fn{spot.worker.Worker.dispatch(cmd, args)}; each command calls the functions above.')
out.append('\\begin{xltabular}{\\linewidth}{@{}>{\\raggedright\\arraybackslash\\ttfamily\\footnotesize}p{0.36\\linewidth} >{\\raggedright\\arraybackslash\\small}X@{}}')
out.append('\\caption*{Commands of the local console (\\fn{dispatch}).}\\\\')
out.append('\\toprule \\normalfont\\small\\textbf{Command} & \\textbf{Purpose} \\\\ \\midrule \\endhead')
for c, p in CMDS:
    out.append('%s & %s \\\\' % (tex(c), tex(p)))
out.append('\\bottomrule\\end{xltabular}')

FILES = [
 ('+spot/+console/webapp/app.py', 'Starts the console\'s web server: \\texttt{--local} for the local console (SPOT does this), otherwise the hardware console (the TX2\'s service).'),
 ('webapp/spot_console/common.py', 'Paths, the API error, checking values from requests.'),
 ('webapp/spot_console/events.py', 'The console\'s own settings, the event stream to every open page, running commands.'),
 ('webapp/spot_console/phasespace.py', 'Compiling and running StreamData, decoding its packets for the 3D view, marker calibration.'),
 ('webapp/spot_console/hardware.py', 'Teensy packets and the SSH links to the platforms: pucks, thrusters, thruster checks.'),
 ('webapp/spot_console/control.py', 'The diagram\'s default LQR law and thruster mixer in Python (spot.control.lqrCommand and thrusterDutyCycles, ported line by line), for Interactive Mode.'),
 ('webapp/spot_console/interactive.py', 'Interactive Mode: flies one or more platforms to their goals from the PhaseSpace stream, each with its own loop, controller process and safety stops.'),
 ('webapp/spot_console/experiment.py', 'Deploying, starting, stopping and collecting experiments on the platforms.'),
 ('webapp/spot_console/local.py', 'The local console: the MATLAB link, simulation and build runs, the link to the lab console.'),
 ('webapp/spot_console/files.py', 'The folder browser.'),
 ('webapp/spot_console/design.py', 'The Spacecraft Designer\'s design (docking cone and solar panels on each platform): checked and kept in a JSON file.'),
 ('webapp/spot_console/calibration.py', 'The Calibration tab: the calibration file (the lab\'s, and each project\'s copy), and the calibrations, which drive a platform (the table\'s slope from drifts).'),
 ('webapp/spot_console/calfit.py', 'The calibrations\' fits (linear least squares on the measured positions): the table\'s height surface and drag, each thruster\'s thrust.'),
 ('webapp/spot_console/server.py', 'The web API and start-up.'),
 ('webapp/static/', 'The page: app.js (common), sim.js, experiment.js, results.js, hardware.js, interactive.js (Interactive Mode), designer.js (Spacecraft Designer), calibration.js and calviz.js (Calibration), viewer.js (3D), cad.js (Use CAD), data.js.'),
 ('+spot/+console/phasespace/StreamData.cc', 'Reads PhaseSpace and streams positions to the platforms (UDP 31534); built with its Makefile or build_linux.sh.'),
 ('+spot/+console/deploy/deploy_tx2.sh', 'Installs the console on the TX2 and (re)starts its service (spot-phasespace-console.service).'),
 ('+spot/+console/deploy/setup_hardware_link.sh', 'Installs the platform agent on RED/BLACK/BLUE and gives the TX2 access to it.'),
 ('+spot/+console/deploy/spot_agent.sh', 'The only thing the TX2 may run on a platform: status, stream, exp-receive, exp-start, exp-stop, exp-status, exp-data, exp-clean.'),
 ('+spot/+hardware/csrc/', 'C/C++ behind the hardware blocks (arm/arm_torque.cpp for the arm, WhoAmI, Data Logger, Debugging, PhaseSpace receiver).'),
 ('+spot/+hardware/arm/', 'setup_arm_serial.sh: on RED, the U2D2 adapter\'s 1 ms latency timer and permissions (a udev rule).'),
 ('+spot/+hardware/teensy/', 'The Teensy firmware (teensy_pwm_control.ino) and setup_teensy_serial.sh.'),
]
out.append('\\begin{xltabular}{\\linewidth}{@{}>{\\raggedright\\arraybackslash\\ttfamily\\footnotesize}p{0.40\\linewidth} >{\\raggedright\\arraybackslash\\small}X@{}}')
out.append('\\caption*{The non-MATLAB parts.}\\\\')
out.append('\\toprule \\normalfont\\small\\textbf{File} & \\textbf{Purpose} \\\\ \\midrule \\endhead')
for f, p in FILES:
    pp = p if '\\texttt' in p else tex(p)
    out.append('%s & %s \\\\' % (tex(f).replace('/', '/\\allowbreak{}'), pp))
out.append('\\bottomrule\\end{xltabular}')

body = '\n'.join(out)
TEX = os.path.join('documentation', 'SPOT_Tutorial.tex')
t = open(TEX).read()
begin, end = '% BEGIN generated function reference', '% END generated function reference'
if '%%FUNCTIONS%%' in t:
    t = t.replace('%%FUNCTIONS%%', begin + '\n' + body + '\n' + end)
else:
    a, b = t.index(begin), t.index(end) + len(end)
    t = t[:a] + begin + '\n' + body + '\n' + end + t[b:]
open(TEX, 'w').write(t)
print('functions:', total)
