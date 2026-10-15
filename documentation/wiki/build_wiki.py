#!/usr/bin/env python3
"""Refresh the generated parts of the SPOT wiki (documentation/wiki): the sidebar, each page's
source listing and its cross-references.

    python3 documentation/wiki/build_wiki.py          # from the project folder (or anywhere)

The pages are written by hand. Three kinds of markers in a page are filled in by this script, so
that a page never shows code that has changed, and never names a caller that no longer exists:

    <!-- source: +spot/+sim/run.m -->      at the top of a page about one file (or several, one
                                            marker each): the file the page documents
    <!-- xref -->   ...   <!-- /xref -->   replaced with "Used by" (every file that names this
                                            function) and "Uses" (every spot.* function it names)
    <!-- listing -->  ...  <!-- /listing --> replaced with the file's full text, numbered, in a
                                            collapsed <details> block (the line numbers the
                                            page's walkthrough refers to)

_Sidebar.md is generated from TOC below. Pages are plain GitHub-wiki Markdown: file name = page
name (spaces as -), [[links]] by page name. Images live in images/.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJECT = os.path.dirname(os.path.dirname(HERE))

# The sidebar: (section, [(page title, page file name without .md)])
TOC = [
    ("Start here", [("Home", "Home"), ("Getting Started", "Getting-Started"), ("How It Fits Together", "Architecture"),
                    ("The Project Folder", "Project-Folder"), ("The Settings File", "Settings-File"),
                    ("Workspace Variables", "Workspace-Variables"), ("Logged Signals", "Logged-Signals"),
                    ("Data Stores", "Data-Stores"), ("Archive", "Archive")]),
    ("The console (GUI)", [("Simulation Tab", "Simulation-Tab"), ("Experiment Tab", "Experiment-Tab"),
                           ("Results Tab", "Results-Tab"), ("Spacecraft Designer", "Designer-Tab"),
                           ("Lab Console: PhaseSpace Tab", "Lab-Console-PhaseSpace-Tab"),
                           ("Lab Console: Hardware Tab", "Lab-Console-Hardware-Tab"), ("Interactive Mode", "Interactive-Mode"),
                           ("Calibration Tab", "Calibration-Tab")]),
    ("The maths", [("Platform Dynamics and LQR", "Math-Platform-Dynamics-and-LQR"), ("The Thruster Mixer", "Math-Thruster-Mixer"),
                   ("Mass Properties", "Math-Mass-Properties"), ("The Arm: Dynamics, Friction, Servos", "Math-Arm"),
                   ("Dynamixel Units", "Math-Dynamixel-Units"), ("The Calibration Fits", "Math-Calibration")]),
    ("The Simulink diagram", [("Overview and Step Order", "Diagram-Overview"),
                              ("Simulation Status, Platform Identification", "Diagram-Status-and-Identification"),
                              ("PhaseSpace Camera Code", "Diagram-PhaseSpace-Camera-Code"),
                              ("Robotic Arm Code", "Diagram-Robotic-Arm-Code"),
                              ("Receive Data from Orin", "Diagram-Receive-Data-from-Orin"),
                              ("Simulate Plant Dynamics", "Diagram-Simulate-Plant-Dynamics"),
                              ("1 - Experiment Logic", "Diagram-1-Experiment-Logic"),
                              ("2 - Navigation", "Diagram-2-Navigation"), ("3 - Guidance", "Diagram-3-Guidance"),
                              ("4 - Control", "Diagram-4-Control"),
                              ("Thruster & Float Control Code", "Diagram-Thruster-and-Float-Control-Code"),
                              ("Arm Torques (After Control)", "Diagram-Arm-Torques-After-Control"),
                              ("Send Data to Orin, Check Connection", "Diagram-Outgoing-Links"),
                              ("Debugger", "Diagram-Debugger"), ("Live Data Streams", "Diagram-Live-Data-Streams"),
                              ("Data Logger Subsystem", "Diagram-Data-Logger-Subsystem")]),
]
# One section per MATLAB package; its functions are listed from the files
PACKAGES = ["params", "massprops", "control", "sim", "experiment", "data", "plotting", "workspace", "worker",
            "console", "hardware", "arm"]
PACKAGE_TITLES = {"params": "spot.params: settings", "massprops": "spot.massprops: mass properties",
                  "control": "spot.control: controller design", "sim": "spot.sim: simulations",
                  "experiment": "spot.experiment: building for the platforms", "data": "spot.data: results",
                  "plotting": "spot.plotting: drawing", "workspace": "spot.workspace: the Workspace panel",
                  "worker": "spot.worker: the MATLAB worker", "console": "spot.console: starting the console",
                  "hardware": "spot.hardware: the blocks and the hardware", "arm": "spot.arm: the simulated arm"}
TOC_AFTER = [
    ("The console's server (Python)", [("Overview: server.py", "Console-Server"), ("common.py", "Console-common"),
                                       ("events.py", "Console-events"), ("local.py", "Console-local"),
                                       ("phasespace.py", "Console-phasespace"), ("hardware.py", "Console-hardware"),
                                       ("experiment.py", "Console-experiment"), ("interactive.py", "Console-interactive"),
                                       ("control.py", "Console-control"), ("design.py", "Console-design"),
                                       ("calibration.py", "Console-calibration"), ("calfit.py", "Console-calfit"),
                                       ("files.py", "Console-files")]),
    ("The console's page (JavaScript)", [("Overview: index.html, app.js", "Page-app-js"), ("sim.js", "Page-sim-js"),
                                         ("experiment.js", "Page-experiment-js"), ("results.js and data.js", "Page-results-js"),
                                         ("viewer.js", "Page-viewer-js"), ("cad.js", "Page-cad-js"),
                                         ("designer.js", "Page-designer-js"), ("hardware.js", "Page-hardware-js"),
                                         ("interactive.js", "Page-interactive-js"), ("liveplot.js", "Page-liveplot-js"),
                                         ("messages.js", "Page-messages-js"), ("calibration.js and calviz.js", "Page-calibration-js"),
                                         ("platforms.js and ratelock.js", "Page-small-modules")]),
    ("The hardware code", [("spot_udp.c", "Hardware-spot-udp"), ("arm_torque.cpp", "Hardware-arm-torque"),
                           ("Data Logger, Debugging, WhoAmI (C++)", "Hardware-small-blocks"),
                           ("Teensy Firmware", "Hardware-Teensy-Firmware"), ("StreamData.cc (PhaseSpace)", "Hardware-StreamData"),
                           ("Deploy Scripts and Agent", "Hardware-Deploy-Scripts"),
                           ("Serial Setup Scripts", "Hardware-Serial-Setup")]),
    ("Tools", [("calibrate_arm.py", "Tool-calibrate-arm"), ("New_SPOT_Project", "Tool-New-SPOT-Project"),
               ("demo_execution_order.m", "Tool-demo-execution-order"), ("The Tutorial and This Wiki", "Tool-Documentation")]),
]


def package_functions(pkg):
    folder = os.path.join(PROJECT, "+spot", "+" + pkg)
    names = sorted(f[:-2] for f in os.listdir(folder) if f.endswith(".m") and f != "Contents.m")
    return names


def page_for(pkg, name):
    return "spot-%s-%s" % (pkg, name)


def sidebar():
    out = ["**[[Home]]**", ""]
    for section, pages in TOC:
        out.append("**%s**" % section)
        for title, page in pages:
            if page == "Home":
                continue
            out.append("- [[%s|%s]]" % (title, page))
        out.append("")
    out.append("**MATLAB: the +spot packages**")
    for pkg in PACKAGES:
        out.append("- [[%s|spot-%s]]" % (PACKAGE_TITLES[pkg], pkg))
        for name in package_functions(pkg):
            out.append("  - [[%s|%s]]" % (name, page_for(pkg, name)))
    out.append("")
    for section, pages in TOC_AFTER:
        out.append("**%s**" % section)
        for title, page in pages:
            out.append("- [[%s|%s]]" % (title, page))
        out.append("")
    return "\n".join(out).rstrip() + "\n"


# ---------------------------------------------------------------------------- cross-references

CALL_RE = re.compile(r"\bspot\.(\w+)\.(\w+)\b")
SCAN_DIRS = ["+spot", "documentation/demo_execution_order.m", "SPOT.m", "SPOT_User_Parameters.m", "../New_SPOT_Project.m",
             "resources/arm_calibration/calibrate_arm.py"]
SCAN_EXT = (".m", ".py", ".js", ".html", ".cpp", ".c", ".h", ".sh", ".ino", ".cc")


def scan_files():
    files = []
    for d in SCAN_DIRS:
        p = os.path.join(PROJECT, d)
        if os.path.isfile(p):
            files.append(p)
            continue
        for root, dirs, names in os.walk(p):
            dirs[:] = [x for x in dirs if x not in ("vendor", "models", "__pycache__", "libowl-src-410")]
            for n in names:
                if n.endswith(SCAN_EXT):
                    files.append(os.path.join(root, n))
    return files


def references(files):
    """{(pkg, name): set of files naming spot.pkg.name}, and {file: set of (pkg, name) it names}."""
    used_by, uses = {}, {}
    for f in files:
        try:
            text = open(f, encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        found = set(CALL_RE.findall(text))
        uses[f] = found
        for key in found:
            used_by.setdefault(key, set()).add(f)
    return used_by, uses


def rel(path):
    return os.path.relpath(path, PROJECT).replace(os.sep, "/")


def own_key(source):
    m = re.match(r"\+spot/\+(\w+)/(\w+)\.m$", source)
    return (m.group(1), m.group(2)) if m else None


def link_for(key):
    pkg, name = key
    if os.path.isfile(os.path.join(PROJECT, "+spot", "+" + pkg, name + ".m")):
        return "[[spot.%s.%s|%s]]" % (pkg, name, page_for(pkg, name))
    return "`spot.%s.%s`" % (pkg, name)


def xref_text(sources, used_by, uses):
    lines = []
    for source in sources:
        key = own_key(source)
        if key is None:
            continue
        callers = sorted(rel(f) for f in used_by.get(key, ()) if rel(f) != source)
        callees = sorted(k for k in uses.get(os.path.join(PROJECT, source.replace("/", os.sep)), ()) if k != key)
        if len(sources) > 1:
            lines.append("**%s**" % source)
        lines.append("- **Used by:** " + (", ".join("`%s`" % c for c in callers) if callers else "nothing in the code (called by you, from the command window, or by the diagram)"))
        lines.append("- **Uses:** " + (", ".join(link_for(k) for k in callees) if callees else "no other spot function"))
        lines.append("")
    return "\n".join(lines).rstrip() + "\n"


def listing_text(sources):
    out = []
    for source in sources:
        path = os.path.join(PROJECT, source.replace("/", os.sep))
        try:
            text = open(path, encoding="utf-8", errors="replace").read().rstrip("\n")
        except OSError:
            out.append("*(%s is missing)*" % source)
            continue
        lang = {".m": "matlab", ".py": "python", ".js": "javascript", ".c": "c", ".cpp": "cpp", ".cc": "cpp", ".h": "c",
                ".sh": "bash", ".ino": "cpp", ".html": "html", ".css": "css"}.get(os.path.splitext(source)[1], "")
        numbered = "\n".join("%4d  %s" % (i + 1, l) for i, l in enumerate(text.split("\n")))
        out.append("<details>\n<summary><b>%s</b> (%d lines; click to show)</summary>\n\n```%s\n%s\n```\n\n</details>"
                   % (source, text.count("\n") + 1, lang, numbered))
    return "\n\n".join(out) + "\n"


def refresh_page(path, used_by, uses):
    text = open(path, encoding="utf-8").read()
    sources = re.findall(r"^<!-- source: ([^>]+?) -->$", text, flags=re.M)
    if not sources:
        return False
    sources = [s.strip() for s in sources]
    new = text
    if re.search(r"^<!-- xref -->$", new, flags=re.M):
        new = re.sub(r"^<!-- xref -->$.*?^<!-- /xref -->$", lambda m: "<!-- xref -->\n" + xref_text(sources, used_by, uses) + "<!-- /xref -->", new, flags=re.S | re.M)
    if re.search(r"^<!-- listing -->$", new, flags=re.M):
        new = re.sub(r"^<!-- listing -->$.*?^<!-- /listing -->$", lambda m: "<!-- listing -->\n" + listing_text(sources) + "<!-- /listing -->", new, flags=re.S | re.M)
    if new != text:
        open(path, "w", encoding="utf-8").write(new)
        return True
    return False


def main():
    os.chdir(HERE)
    open("_Sidebar.md", "w", encoding="utf-8").write(sidebar())
    used_by, uses = references(scan_files())
    changed = 0
    for name in sorted(os.listdir(HERE)):
        if name.endswith(".md") and not name.startswith("_"):
            changed += refresh_page(os.path.join(HERE, name), used_by, uses)
    # every function has a page?
    missing = [page_for(p, n) for p in PACKAGES for n in package_functions(p) if not os.path.isfile(page_for(p, n) + ".md")]
    print("sidebar written; %d page(s) refreshed; %d function page(s) missing%s" % (changed, len(missing), ": " + ", ".join(missing) if missing else ""))
    return 1 if missing and "--strict" in sys.argv else 0


if __name__ == "__main__":
    sys.exit(main())
