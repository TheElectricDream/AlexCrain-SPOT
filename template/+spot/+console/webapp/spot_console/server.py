"""The HTTP server: the page, the JSON API, events, start-up.

Starts either the lab console or the local console.

Part of the SPOT Console (webapp/app.py starts it).
"""

import argparse
import ipaddress
import json
import os
import platform
import queue
import re
import shutil
import socket
import subprocess
import sys
import threading
import webbrowser
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from socketserver import ThreadingMixIn
from urllib.parse import parse_qs, urlparse

from .common import ApiError, PROJECT_DIR, STATIC_DIR, parse_bool, parse_number
from .events import hub, settings
from .phasespace import (PLATFORM_DIR, RESERVED_PORTS, WSL_INFO, broadcast_loop, builder,
                         monitor, parse_frequency, read_base_destinations, read_led_owner,
                         save_tracker_positions, stream_summary, streamer, toolchains,
                         tracker_positions_path)
from .hardware import NUM_THRUSTERS, hardware, parse_platforms
from .experiment import experiment
from .interactive import interactive
from . import local   # local.lab: the link to the lab console, set when the local console starts
from .local import (LOCAL, LabLink, MATLAB_COMMANDS, build_runner, check_token, download_run,
                    matlab, sim_runner, write_ready_file)
from .files import browse, make_folder
from .design import design_store
from .calibration import assumed_defaults, calibrator, saved_runs, store as calibration_store
from . import vision
from .deploy import deployer


CONTENT_TYPES = {".html": "text/html; charset=utf-8", ".js": "text/javascript; charset=utf-8",
                 ".css": "text/css; charset=utf-8", ".svg": "image/svg+xml", ".png": "image/png",
                 ".txt": "text/plain; charset=utf-8", ".json": "application/json",
                 ".gz": "application/gzip"}

MAX_EXTRA_DESTINATIONS = 20
EVENT_KEEPALIVE = 15       # [s] a comment line when no event has come
EVENTS_PER_WRITE = 200     # events sent together when they queue up
DEFAULT_LAB_URL = "http://192.168.1.116:8080"


# State and settings


def features():
    """Return what this server offers.

    The lab console (TX2) runs PhaseSpace and the hardware; the local
    console (started by SPOT.m) runs simulations through the user's
    MATLAB.
    """
    if LOCAL["enabled"]:
        return {"mode": "local", "phasespace": False, "hardware": False, "simulation": True,
                "experiment": True, "results": True, "designer": True, "calibration": True,
                "vision": False}
    return {"mode": "lab", "phasespace": True, "hardware": True, "simulation": False,
            "experiment": False, "results": False, "designer": True, "calibration": True,
            "vision": True}


def app_state(client_ip=None):
    """Return everything a page needs when it opens (GET /api/state).

    Args:
        client_ip: The address the browser connects from ("Add this
            computer").
    """
    # The local console: MATLAB, its jobs, and the lab link.
    if LOCAL["enabled"]:
        return {"features": features(), "client_ip": client_ip, "project_dir": LOCAL["root"],
                "host": {"os": platform.system(), "name": socket.gethostname(), "wsl": WSL_INFO},
                "logs": hub.history(), "matlab": matlab.status(), "sim": sim_runner.status(),
                "build": build_runner.status(), "lab_url": local.lab.url,
                "lab": local.lab.status(), "design": design_store.get(),
                "calibration": calibration_store.get(),
                "deploy": deployer.status(local.lab.status()["reachable"])}

    # The lab console: streaming, hardware, experiments, calibration.
    return {
        "features": features(),
        "client_ip": client_ip,
        "base_destinations": read_base_destinations(),
        "host": {"os": platform.system(), "name": socket.gethostname(), "wsl": WSL_INFO},
        "project_dir": str(PROJECT_DIR),
        "settings": settings.all(),
        "platform_dir": PLATFORM_DIR,
        "toolchains": toolchains.get(),
        "build": builder.status(),
        "stream": streamer.status(),
        "monitor": monitor.status(),
        "logs": hub.history(),
        "led_owner": read_led_owner(),
        "hardware": hardware.status(),
        "experiment": experiment.status(),
        "interactive": interactive.status(),
        "interactive_info": interactive.info(),
        "design": design_store.get(),
        "calibration": calibration_store.get(),
        "calibration_run": calibrator.status(),
        "calibration_assumed": assumed_defaults(),
    }


def update_settings(body):
    """Check and save a settings change, and apply it right away.

    Args:
        body: The settings to change: output_dir, frequency,
            monitor_port, extra_destinations, transport, platforms,
            vision.

    Returns:
        All the settings.

    Raises:
        ApiError: for a value that isn't valid.
    """
    changes = {}

    # The output folder: a full path.
    if "output_dir" in body:
        out_dir = str(body["output_dir"]).strip()
        if not out_dir:
            raise ApiError("Choose an output folder.")
        out_path = Path(out_dir).expanduser()
        if not out_path.is_absolute():
            raise ApiError("The output folder must be a full path.")
        changes["output_dir"] = str(out_path)

    if "frequency" in body:
        changes["frequency"] = parse_frequency(body["frequency"])

    # The viewer port: not SPOT's, and not while streaming.
    if "monitor_port" in body:
        try:
            port = int(body["monitor_port"])
        except (TypeError, ValueError):
            raise ApiError("Viewer port must be a number.")
        if not 1024 <= port <= 65535 or port in RESERVED_PORTS:
            raise ApiError("Viewer port must be 1024-65535 and not 31534/31535 (used by SPOT).")
        if port != settings.get("monitor_port") and streamer.status()["running"]:
            raise ApiError("Stop streaming before changing the viewer port.")
        changes["monitor_port"] = port

    # Extra destinations: IPv4 addresses, not in the fixed list, no
    # repeats.
    if "extra_destinations" in body:
        value = body["extra_destinations"]
        if not isinstance(value, list):
            raise ApiError("extra_destinations must be a list of IP addresses.")
        base, extras = read_base_destinations(), []
        for item in value:
            try:
                ip = str(ipaddress.IPv4Address(str(item).strip()))
            except ValueError:
                raise ApiError("%r is not an IPv4 address (e.g. 192.168.1.43)." % item)
            if ip in base:
                raise ApiError("%s is already in StreamData's fixed list." % ip)
            if ip not in extras:
                extras.append(ip)
        if len(extras) > MAX_EXTRA_DESTINATIONS:
            raise ApiError("At most 20 extra destinations.")
        changes["extra_destinations"] = extras

    if "transport" in body:
        if body["transport"] not in ("tcp", "udp"):
            raise ApiError("Transport must be tcp or udp.")
        changes["transport"] = body["transport"]
    if "platforms" in body:
        changes["platforms"] = parse_platforms(body["platforms"])
    if "vision" in body:
        changes["vision"] = vision.parse_vision(body["vision"])

    # Save.
    old_extras, old_platforms = settings.get("extra_destinations"), settings.get("platforms")
    data = settings.update(changes)

    # Apply: destinations to a running stream, the viewer port, and
    # platforms whose address changed.
    if "extra_destinations" in changes:
        for ip in old_extras:
            if ip not in changes["extra_destinations"]:
                streamer.command("dest remove " + ip)
        for ip in changes["extra_destinations"]:
            if ip not in old_extras:
                streamer.command("dest add " + ip)
    if "monitor_port" in changes:
        monitor.ensure(changes["monitor_port"])
    for name, cfg in changes.get("platforms", {}).items():
        if cfg != old_platforms.get(name):
            hardware.get(name).reconfigure()
    hub.publish("settings", data)
    hub.publish("stream_status", streamer.status())
    return data


# Requests


class Handler(BaseHTTPRequestHandler):
    """One HTTP request: the page, the API and the event stream."""

    server_version = "SPOTConsole/1.0"

    def log_message(self, fmt, *args):
        """Log nothing (keep the terminal quiet)."""
        pass

    def _local_host_ok(self):
        """Return whether the request is addressed to the loopback name.

        Local console only: a web page whose own domain is pointed at
        127.0.0.1 (DNS rebinding) is turned away.
        """
        host = (self.headers.get("Host") or "").rsplit(":", 1)[0].strip("[]").lower()
        return host in ("127.0.0.1", "localhost", "::1")

    def _cookie_name(self):
        """Return the token cookie's name (one per port)."""
        return "spot_token_%d" % self.server.server_address[1]

    def do_GET(self):
        """Serve a GET: the page, static files, and the API's reads."""
        url = urlparse(self.path)
        qs = parse_qs(url.query)
        try:
            if LOCAL["enabled"] and not self._local_host_ok():
                return self._send(403, b"Forbidden", "text/plain")

            # The page; the local console remembers the token in this
            # browser, so the plain address works from now on.
            if url.path == "/":
                extra = None
                if LOCAL["enabled"] and check_token(qs.get("token", [""])[0]):
                    extra = [("Set-Cookie", "%s=%s; Path=/; HttpOnly; SameSite=Strict"
                              % (self._cookie_name(), LOCAL["token"]))]
                return self._file(STATIC_DIR / "index.html", extra)

            # Static files, only from STATIC_DIR.
            if url.path.startswith("/static/"):
                target = (STATIC_DIR / url.path[len("/static/"):]).resolve()
                if STATIC_DIR not in target.parents:
                    return self._send(404, b"Not found", "text/plain")
                return self._file(target)

            # State.
            if url.path == "/api/ping":
                return self._json({"ok": True, "mode": features()["mode"]})
            if url.path == "/api/state":
                return self._json(app_state(self.client_address[0]))

            # The simulation's result file (token required).
            if url.path == "/api/sim/result":
                if not self._authorized():
                    return self._json({"error": "Not allowed."}, 403)
                path = sim_runner.result_path()
                if path is None:
                    return self._json({"error": "No simulation result yet."}, 404)
                return self._file(path)

            # Streaming and design.
            if url.path == "/api/tracker_positions":
                path = tracker_positions_path()
                return self._json({"path": str(path),
                                   "text": path.read_text(encoding="utf-8")
                                   if path.exists() else None})
            if url.path == "/api/stream":
                return self._json(stream_summary())
            if url.path == "/api/design":
                return self._json(design_store.get())

            # Calibration.
            if url.path == "/api/calibration/arm/check" and not LOCAL["enabled"]:
                return self._json(calibrator.arm_check())
            if url.path in ("/api/calibration/table/runs", "/api/calibration/thrusters/runs",
                            "/api/calibration/decay/runs") and not LOCAL["enabled"]:
                return self._json({"runs": saved_runs(url.path.split("/")[3])})
            if url.path == "/api/calibration":
                return self._json(dict(calibration_store.get(),
                                       run=None if LOCAL["enabled"] else calibrator.status(),
                                       assumed=assumed_defaults()))

            # Hardware, vision and Interactive Mode (lab console).
            if url.path == "/api/hardware":
                return self._json(hardware.status())
            if url.path == "/api/vision" and not LOCAL["enabled"]:
                return self._json(vision.status())
            if url.path == "/api/vision/params" and not LOCAL["enabled"]:
                try:
                    return self._json(vision.get_params(qs.get("camera", [""])[0]))
                except ApiError as e:
                    return self._json({"error": str(e)}, 400)
            if url.path == "/api/interactive" and not LOCAL["enabled"]:
                return self._json(dict(interactive.status(), controller=interactive.info()))

            # Experiments (lab console).
            if url.path == "/api/experiment" and not LOCAL["enabled"]:
                return self._json(experiment.status())
            if url.path == "/api/experiment/live" and not LOCAL["enabled"]:
                return self._json({"streams": experiment.live_data()})
            if url.path == "/api/experiment/build" and not LOCAL["enabled"]:
                try:
                    return self._json({"build": experiment.build_info(qs.get("model", [""])[0])})
                except ApiError as e:
                    return self._json({"error": str(e)}, 400)
            if url.path == "/api/experiment/download" and not LOCAL["enabled"]:
                try:
                    data = experiment.archive(qs.get("run", [""])[0])
                except ApiError as e:
                    return self._json({"error": str(e)}, 404)
                return self._send(200, data, "application/gzip")

            # The folder browser and the event stream.
            if url.path == "/api/browse":
                return self._json(browse(qs.get("path", [""])[0]))
            if url.path == "/api/events":
                return self._events()
            self._send(404, b"Not found", "text/plain")
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError):
            pass

    def do_POST(self):
        """Serve a POST: the API's actions.

        Requiring the JSON content type stops other web pages from
        triggering actions through the browser. Values may come in the
        JSON body or the URL (?frequency=100, easier from scripts).
        """
        if not self.headers.get("Content-Type", "").startswith("application/json"):
            return self._json({"error": "Send the header \"Content-Type: application/json\"."},
                              415)
        try:
            # The body, with the URL's values as defaults.
            length = int(self.headers.get("Content-Length") or 0)
            body = json.loads(self.rfile.read(length) or b"{}") if length else {}
            if not isinstance(body, dict):
                raise ValueError
            url = urlparse(self.path)
            for key, values in parse_qs(url.query).items():
                body.setdefault(key, values[-1])
            path = url.path

            # Who asks: the address, and the page or the local console
            # it came through.
            requester = self.client_address[0] + (
                " (webapp)" if self.headers.get("X-SPOT-Client") == "webapp" else "")
            via = re.sub(r"[^\w .,()@-]", "", self.headers.get("X-SPOT-Requester", ""))[:80]
            if via:
                requester = "%s (%s)" % (self.client_address[0], via)

            # The local console has its own actions.
            if LOCAL["enabled"] and not self._local_host_ok():
                return self._json({"error": "Forbidden"}, 403)
            if LOCAL["enabled"]:
                return self._json(*self._local_post(path, body, requester))

            # Settings and design.
            if path == "/api/settings":
                result = update_settings(body)
            elif path == "/api/design":
                result = design_store.set(body.get("design"), requester)

            # Calibration.
            elif path == "/api/calibration/table/start":
                result = calibrator.start_table(body, requester)
            elif path == "/api/calibration/table/load":
                result = calibrator.load_table(body, requester)
            elif path == "/api/calibration/thrusters/load":
                result = calibrator.load_thrusters(body, requester)
            elif path == "/api/calibration/thrusters/start":
                result = calibrator.start_thrusters(body, requester)
            elif path == "/api/calibration/decay/start":
                result = calibrator.start_decay(body, requester)
            elif path == "/api/vision/params":
                result = vision.set_params(body.get("camera"), body)
            elif path == "/api/calibration/decay/load":
                result = calibrator.load_decay(body, requester)
            elif path == "/api/calibration/arm/start":
                result = calibrator.start_arm(body, requester)
            elif path == "/api/calibration/stop":
                result = calibrator.stop(requester)
            elif path == "/api/calibration/clear":
                # Remove a section (and a platform's, for thrusters)
                # from the lab's calibration.
                section = str(body.get("section", ""))
                if section not in ("table", "thrusters", "arm"):
                    raise ApiError("section must be table, thrusters or arm.")
                if calibrator.active():
                    raise ApiError("A calibration is running; wait for it to finish.")
                plat_key = str(body.get("platform") or "").upper() or None
                if section == "thrusters" and plat_key not in ("RED", "BLACK", "BLUE"):
                    raise ApiError("Say which platform's thruster calibration to remove.")
                hub.log("hardware", "Calibration: %s%s removed (by %s)"
                        % (section, " " + plat_key if plat_key else "", requester))
                result = calibration_store.set_section(section, None, plat_key)

            # Building and streaming.
            elif path == "/api/build":
                builder.start()
                result = builder.status()
            elif path == "/api/stream/start":
                freq = parse_frequency(body.get("frequency", settings.get("frequency")))
                streamer.start(freq, requester)
                update_settings({"frequency": freq})  # remembered as the page's default
                result = stream_summary()
            elif path == "/api/stream/stop":
                streamer.stop(requester)
                result = stream_summary()
            elif path in ("/api/destinations/add", "/api/destinations/remove"):
                # ip=self means the computer sending this request (e.g.
                # the SPOT GUI's own machine).
                ip = str(body.get("ip", "")).strip()
                if ip == "self":
                    ip = self.client_address[0]
                extras = list(settings.get("extra_destinations"))
                if ip in read_base_destinations():
                    # Always sent to already (e.g. the ground station):
                    # nothing to add or remove.
                    result = {"extra_destinations": extras, "ip": ip,
                              "note": "%s is one of the fixed destinations" % ip}
                else:
                    if path.endswith("add") and ip not in extras:
                        extras.append(ip)
                    elif path.endswith("remove"):
                        extras = [e for e in extras if e != ip]
                    result = {"extra_destinations": update_settings(
                                  {"extra_destinations": extras})["extra_destinations"],
                              "ip": ip}
            elif path == "/api/tracker_positions":
                result = save_tracker_positions(body.get("text"), requester)
            elif path == "/api/toolchains/refresh":
                toolchains.refresh()
                result = toolchains.get()
            elif path == "/api/mkdir":
                result = make_folder(body)

            # Experiments.
            elif path == "/api/experiment/deploy":
                result = experiment.deploy(str(body.get("model", "")), body.get("platforms"),
                                           requester)
            elif path == "/api/experiment/start":
                result = experiment.start(body.get("platforms"), requester)
            elif path == "/api/experiment/stop":
                result = experiment.stop(requester)
            elif path == "/api/experiment/fetch":
                result = experiment.fetch(requester)

            # Interactive Mode.
            elif path == "/api/interactive/start":
                # Fly platforms (each to its goal, or holding where it
                # is): {"platforms": {"RED": {"x", "y", "att"} or null,
                # ...}}, or one "platform" and its "goal".
                goals = body.get("platforms")
                if goals is None and body.get("platform"):
                    goals = {body.get("platform"): body.get("goal")}
                result = interactive.start(goals, str(body.get("client") or ""), requester)
            elif path == "/api/interactive/goal":
                result = interactive.set_goal(body.get("platform"), body,
                                              str(body.get("client") or ""),
                                              final=parse_bool(body.get("final", True), "final"))
            elif path == "/api/interactive/heartbeat":
                result = interactive.heartbeat(str(body.get("client") or ""))
            elif path == "/api/interactive/stop":
                # One "platform", or all of them.
                result = interactive.stop(requester, body.get("platform"))

            # Hardware.
            elif path == "/api/hardware/off":
                # No platform (or "all"): every platform.
                if str(body.get("platform") or "all").lower() == "all":
                    hardware.all_off(requester)
                    result = hardware.status()
                else:
                    plat = hardware.get(body.get("platform"))
                    plat.all_off(requester)
                    result = plat.status()
            elif path.startswith("/api/hardware/"):
                plat = hardware.get(body.get("platform"))
                if path == "/api/hardware/pucks":
                    plat.set_pucks(parse_bool(body.get("on"), "on"), requester)
                elif path == "/api/hardware/thruster":
                    thruster = parse_number(body.get("thruster"), "thruster", 1, NUM_THRUSTERS)
                    if thruster != int(thruster):
                        raise ApiError("thruster must be 1-%d." % NUM_THRUSTERS)
                    plat.set_thruster(int(thruster) - 1,
                                      parse_number(body.get("duty", 100), "duty", 0, 100),
                                      requester)
                elif path == "/api/hardware/check":
                    plat.start_check(parse_number(body.get("duration", 0.5), "duration", 0.2, 10),
                                     requester)
                elif path == "/api/hardware/check/cancel":
                    plat.cancel_check(requester)
                else:
                    return self._json({"error": "Not found"}, 404)
                result = plat.status()
            else:
                return self._json({"error": "Not found"}, 404)
            self._json(result)
        except ApiError as e:
            self._json({"error": str(e), "details": e.details} if e.details
                       else {"error": str(e)}, 400)
        except ValueError:
            self._json({"error": "Invalid request."}, 400)
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError):
            pass

    def _authorized(self):
        """Return whether the request has the token (header, cookie)."""
        if check_token(self.headers.get("X-SPOT-Token")):
            return True
        for part in (self.headers.get("Cookie") or "").split(";"):
            name, _, value = part.strip().partition("=")
            if name == self._cookie_name() and check_token(value):
                return True
        return False

    def _local_post(self, path, body, requester):
        """Serve the local console's POST requests.

        Returns:
            (data, HTTP status).

        Raises:
            ApiError: for a request that isn't valid now.
        """
        if not self._authorized():
            return {"error": "This page can't talk to MATLAB: open it with the link SPOT printed "
                             "in MATLAB (or run SPOT open there)."}, 403

        # A MATLAB command, with its own time limit.
        if path == "/api/matlab":
            cmd = str(body.get("cmd", ""))
            if cmd not in MATLAB_COMMANDS:
                raise ApiError("Unknown MATLAB command %r." % cmd)
            args = body.get("args") or {}
            if not isinstance(args, dict):
                raise ApiError("args must be an object.")
            if cmd == "workspace.eval":
                hub.log("matlab", ">> %s" % str(args.get("command", ""))[:500])
            if cmd == "experiment.connect" and not local.lab.status()["reachable"]:
                raise ApiError("The lab console isn't reachable: connect to the lab network "
                               "first.")
            return {"result": matlab.request(cmd, args, timeout=MATLAB_COMMANDS[cmd])}, 200

        # A simulation.
        if path == "/api/sim/run":
            return sim_runner.start(requester), 200

        # Lab Setup: a deployment script (deploy.py).
        if path == "/api/deploy/run":
            boards = body.get("boards") or []
            if not isinstance(boards, list):
                raise ApiError("boards must be a list (red, black, blue).")
            return deployer.run(str(body.get("action", "")), [str(b) for b in boards], requester,
                                local.lab.status()["reachable"]), 200
        if path == "/api/deploy/cancel":
            deployer.cancel()
            return deployer.status(local.lab.status()["reachable"]), 200

        # An experiment build: on the lab network, with a recent lab
        # console.
        if path == "/api/experiment/build":
            if not local.lab.status()["reachable"]:
                raise ApiError("The lab console isn't reachable: experiments can only be built "
                               "on the lab network.")
            if not local.lab.status()["experiments"]:
                raise ApiError("The lab console runs an older version without experiments: "
                               "update it with deploy_tx2.sh.")
            return build_runner.start(requester), 200

        # Stop a simulation. MATLAB doesn't read the connection while
        # Simulink runs, but its progress timer checks for this file
        # (see Worker.m, sim.run).
        if path == "/api/sim/stop":
            if not sim_runner.status()["running"]:
                return {"stopping": False}, 200
            stop_file = Path(LOCAL["root"]) / ".spot_cache" / "stop_request"
            stop_file.write_text("stop")
            hub.log("matlab", "Stop requested (by %s)" % requester)
            return {"stopping": True}, 200

        # A lab action from the Experiment tab, forwarded to the lab
        # console; tracker_positions sends this project's marker
        # calibration.
        if path == "/api/lab":
            action = str(body.get("path", ""))
            if action.split("?", 1)[0] in LabLink.READS:
                return {"result": local.lab.request("GET", action)}, 200
            if action.split("?", 1)[0] not in LabLink.ACTIONS:
                raise ApiError("Not a lab action: %r" % action)
            payload = body.get("body") or {}
            if action == "tracker_positions":
                tracker_file = (Path(LOCAL["root"]) / "resources"
                                / "tracker_positions_phasespace.txt")
                if not tracker_file.is_file():
                    raise ApiError("This project has no "
                                   "resources/tracker_positions_phasespace.txt.")
                payload = {"text": tracker_file.read_text(encoding="utf-8")}
            return {"result": local.lab.request("POST", action, payload)}, 200

        # The Experiment tab's lab console address (the settings'
        # labConsoleAddress).
        if path == "/api/lab/address":
            return {"lab": local.lab.set_address(body.get("address", "")),
                    "lab_url": local.lab.url}, 200

        if path == "/api/design":
            return design_store.set(body.get("design"), requester), 200

        # The project's copy of the calibration becomes the lab
        # console's (the page's Update).
        if path == "/api/calibration/take":
            lab_cal = local.lab.request("GET", "calibration").get("calibration")
            if not lab_cal or not lab_cal.get("updated"):
                raise ApiError("The lab console has no calibration yet.")
            hub.log("matlab", "Calibration updated from the lab console (%s)"
                    % lab_cal.get("updated"))
            return calibration_store.take(lab_cal), 200

        # Whether to offer the lab's newer calibration: {"ask": false}
        # (Never Ask Again), or {"declined": "<the lab's updated>"}
        # (Not Now: not again for that calibration).
        if path == "/api/calibration/sync":
            return calibration_store.set_sync(ask=body.get("ask"),
                                              declined=body.get("declined")), 200

        if path == "/api/lab/download":
            return download_run(str(body.get("run", ""))), 200
        if path == "/api/shutdown":
            threading.Thread(target=LOCAL["server"].shutdown, daemon=True).start()
            return {"ok": True}, 200
        return {"error": "Not available in the local console."}, 404

    def _send(self, code, body, ctype, extra_headers=None):
        """Send a reply: status, headers (not cached) and body."""
        self.send_response(code)
        for name, value in extra_headers or ():
            self.send_header(name, value)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        self.wfile.write(body)

    def _json(self, data, code=200):
        """Send data as JSON."""
        self._send(code, json.dumps(data, allow_nan=False).encode("utf-8"), "application/json")

    def _file(self, path, extra_headers=None):
        """Send a file, typed by its extension (404 if unreadable)."""
        try:
            body = path.read_bytes()
        except OSError:
            return self._send(404, b"Not found", "text/plain")
        self._send(200, body, CONTENT_TYPES.get(path.suffix, "application/octet-stream"),
                   extra_headers)

    def _events(self):
        """Stream server-sent events until the page goes away."""
        # Subscribe before the reply starts: whoever sees the headers
        # misses no event after them.
        events = hub.subscribe()
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()
        try:
            self.wfile.write(b"retry: 2000\n\n")
            self.wfile.flush()
            while True:
                # The next event, or a keep-alive comment; with whatever
                # else is already waiting, in the same write.
                try:
                    chunks = [events.get(timeout=EVENT_KEEPALIVE)]
                except queue.Empty:
                    chunks = [": keep-alive\n\n"]
                else:
                    try:
                        while len(chunks) < EVENTS_PER_WRITE:
                            chunks.append(events.get_nowait())
                    except queue.Empty:
                        pass
                self.wfile.write("".join(chunks).encode("utf-8"))
                self.wfile.flush()
        except (BrokenPipeError, ConnectionResetError, ConnectionAbortedError, OSError):
            pass
        finally:
            hub.unsubscribe(events)


# Start-up


def open_browser(url):
    """Open a URL in the browser (under WSL, the Windows one)."""
    if WSL_INFO and shutil.which("cmd.exe"):
        subprocess.Popen(["cmd.exe", "/c", "start", "", url], cwd="/mnt/c",
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    else:
        webbrowser.open(url)


class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
    """http.server.ThreadingHTTPServer, which is Python 3.7+ only."""

    daemon_threads = True


def main():
    """Start the lab console, or with --local the local one."""
    parser = argparse.ArgumentParser(description="SPOT Console")
    parser.add_argument("--host", default="127.0.0.1",
                        help="address to serve on (default 127.0.0.1 = this computer only; "
                             "0.0.0.0 = whole network)")
    parser.add_argument("--port", type=int, default=8080, help="web port (default 8080)")
    parser.add_argument("--no-browser", action="store_true", help="do not open a browser tab")
    parser.add_argument("--local", action="store_true",
                        help="local console for simulations, started by SPOT.m in MATLAB (this "
                             "computer only)")
    parser.add_argument("--worker-port", type=int, default=0,
                        help="local mode: port the MATLAB worker connects to (0 = any)")
    parser.add_argument("--root", help="local mode: the SPOT project folder")
    parser.add_argument("--ready-file",
                        help="local mode: where to write the ports and token for MATLAB")
    parser.add_argument("--lab-url", default=os.environ.get("SPOT_LAB_URL"),
                        help="local mode: the lab console (default: the project's "
                             "spot_settings.json labConsoleAddress, port 8080; else "
                             "$SPOT_LAB_URL or http://192.168.1.116:8080)")
    args = parser.parse_args()

    # The local console, or the lab console (Linux only).
    if args.local:
        return main_local(args)
    if platform.system() == "Windows":
        sys.exit("The lab console runs on Linux (the TX2, or WSL): python3 webapp/app.py. "
                 "On Windows, SPOT in MATLAB starts the local console (app.py --local).")

    # The server, and the design and calibration in the home folder.
    try:
        server = ThreadingHTTPServer((args.host, args.port), Handler)
    except OSError as e:
        sys.exit("Could not start the web server on %s:%d (%s). Is it already running? "
                 "Try --port 8081." % (args.host, args.port, e))
    server.daemon_threads = True
    design_store.open(Path.home() / ".spot_design.json")
    calibration_store.open(Path.home() / ".spot_calibration.json")

    # Compilers, the monitor and its broadcast, the hardware, and the
    # platforms' clocks, messages and streams (sent to the lab console).
    toolchains.refresh()
    monitor.ensure(settings.get("monitor_port"))
    threading.Thread(target=broadcast_loop, daemon=True).start()
    hardware.start()
    if not LOCAL["enabled"]:
        experiment.listen_clocks()
        experiment.listen_debug()
        experiment.listen_live()

    # Say where, and warn when the whole network can reach it.
    url = "http://%s:%d" % ("localhost" if args.host in ("127.0.0.1", "0.0.0.0") else args.host,
                            args.port)
    print("SPOT Console running at %s  (Ctrl+C to quit)" % url)
    if args.host not in ("127.0.0.1", "localhost"):
        print("WARNING: serving on %s, so anyone on the network can compile, start streaming, "
              "browse folders on this computer and switch the platforms' pucks and thrusters."
              % args.host)
    if not args.no_browser:
        threading.Timer(0.8, open_browser, args=(url,)).start()

    # Serve until Ctrl+C; then everything off.
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down...")
    finally:
        calibrator.shutdown()
        interactive.shutdown()
        hardware.shutdown()
        streamer.stop("the webapp shutting down")
        server.server_close()


def lab_url_from_settings(root):
    """Return the lab console's URL from the settings, or the usual."""
    try:
        address = json.loads((root / "spot_settings.json").read_text(encoding="utf-8")).get(
            "labConsoleAddress")
        if address:
            return LabLink.url_for(address)
    except (OSError, ValueError, ApiError, AttributeError):
        pass
    return DEFAULT_LAB_URL


def main_local(args):
    """Start the local console.

    Served to this computer only, with the MATLAB worker on another
    port; the ports and token go to the ready file for SPOT.m.
    """
    # The project, the session token, the design and calibration.
    LOCAL.update(enabled=True, root=str(Path(args.root or os.getcwd()).resolve()),
                 token=os.environ.get("SPOT_TOKEN") or os.urandom(24).hex())
    LOCAL["ready_file"] = args.ready_file or str(Path(LOCAL["root"]) / ".spot_cache"
                                                 / "console.json")
    design_store.open(Path(LOCAL["root"]) / "spot_design.json")
    calibration_store.open(Path(LOCAL["root"]) / "resources" / "spot_calibration.json",
                           local=True)

    # The server, the MATLAB worker's port and the lab link.
    try:
        server = ThreadingHTTPServer(("127.0.0.1", args.port), Handler)
    except OSError as e:
        sys.exit("Could not start the web server on port %d (%s)." % (args.port, e))
    server.daemon_threads = True
    LOCAL["server"] = server
    matlab.listen(args.worker_port)
    local.lab = LabLink(args.lab_url or lab_url_from_settings(Path(LOCAL["root"])))
    local.lab.start()

    # Tell SPOT.m, and open the page with the token.
    web_port = server.server_address[1]
    write_ready_file(web_port)
    print("SPOT Console (local) at http://127.0.0.1:%d, MATLAB worker port %d"
          % (web_port, matlab.port))
    sys.stdout.flush()
    if not args.no_browser:
        url = "http://127.0.0.1:%d/?token=%s#simulation" % (web_port, LOCAL["token"])
        threading.Timer(0.5, webbrowser.open, args=(url,)).start()

    # Serve until stopped; then remove the ready file, so SPOT.m doesn't
    # find a console that is gone.
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
        try:
            if json.loads(Path(LOCAL["ready_file"]).read_text()).get("pid") == os.getpid():
                Path(LOCAL["ready_file"]).unlink()
        except (OSError, ValueError):
            pass
