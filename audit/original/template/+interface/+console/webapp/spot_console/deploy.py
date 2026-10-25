"""Lab Setup (local console): the deployment scripts, run from the page.

The scripts of +interface/+console/deploy run from the topbar's Lab Setup
(next to Lab Console) instead of a terminal, one at a time; what each
prints goes to the page as it runs (the "deploy" log). They need this
computer on the lab network (the lab console reachable) and the SSH
key the scripts use (~/.ssh/id_ed25519_spot, accepted by the TX2, the
Orin and the platforms).

    tx2        deploy_tx2.sh           the lab console: copies it to
                                       the TX2, builds StreamData,
                                       restarts it
    orin       deploy_orin.sh          SPOT Vision: copies it to the
                                       Orin and restarts it
    platforms  setup_hardware_link.sh  links the TX2 to the chosen
                                       platforms (their agent, its
                                       key). Its power-saving step
                                       needs a terminal (sudo asks for
                                       a password), so it is skipped
                                       here (NO_POWER=1).

Part of the SPOT Console (webapp/app.py starts it).
"""

import os
import shutil
import signal
import subprocess
import threading
import time
from pathlib import Path

from .common import PROJECT_DIR, ApiError, decode
from .events import hub

DEPLOY_DIR = PROJECT_DIR / "deploy"
SSH_KEY = Path.home() / ".ssh" / "id_ed25519_spot"
BOARDS = ("red", "black", "blue")

# What the page shows for each script, and the script itself.
ACTIONS = {
    "tx2": {
        "title": "Update the Lab Console", "script": "deploy_tx2.sh",
        "target": "TX2 · 192.168.1.116", "minutes": 2,
        "what": "Copies this project's lab console (the webapp, StreamData and the arm "
                "calibration script) to the TX2, builds StreamData there and restarts the "
                "console. Do it after updating SPOT, so the lab console has the same version as "
                "this project.",
        "careful": "It restarts the lab console: PhaseSpace streaming, Interactive Mode and any "
                   "calibration stop, and its pages reconnect after about 10 s. Don't run it "
                   "during an experiment."},
    "orin": {
        "title": "Update SPOT Vision", "script": "deploy_orin.sh",
        "target": "Orin · 192.168.1.115", "minutes": 1,
        "what": "Copies SPOT Vision (the service that streams the Orin's cameras to the lab "
                "console's Vision tab) to the Jetson Orin and restarts it.",
        "careful": "Any Vision tab stream stops for a few seconds; the cameras' settings are "
                   "kept."},
    "platforms": {
        "title": "Link the Platforms", "script": "setup_hardware_link.sh",
        "target": "Chaser, Target, Obstacle", "minutes": 1,
        "what": "Lets the lab console drive the chosen platforms: installs their agent "
                "(spot_agent.sh) and the TX2's key, then checks each answers. Do it for a new or "
                "reinstalled platform, or after the agent changes (e.g. the arm calibration's "
                "servo check).",
        "careful": "Switching off the platforms' Wi-Fi power saving and suspend needs their "
                   "password: run +interface/+console/deploy/setup_hardware_link.sh in a terminal "
                   "for that step."},
}


class Deployer:
    """Runs one deployment script at a time and follows its output."""

    def __init__(self):
        """Start with nothing running."""
        self._lock = threading.Lock()
        self.proc = None
        self.last = None   # {"action", "started", "finished", "code", "ok", "by"}

    def checks(self):
        """Return why the scripts can't run here, or None."""
        # Bash scripts: not on native Windows.
        if os.name == "nt":
            return ("The deployment scripts are bash scripts: run them from WSL or Git Bash "
                    "(+interface/+console/deploy).")

        # The programs they call.
        if not shutil.which("bash") or not shutil.which("ssh") or not shutil.which("rsync"):
            missing = [c for c in ("bash", "ssh", "rsync") if not shutil.which(c)]
            return "This computer has no %s, which the scripts need." % ", ".join(missing)

        # The lab key they log in with.
        if not SSH_KEY.exists():
            return ("This computer has no lab key (%s): the scripts log in to the TX2, the Orin "
                    "and the platforms with it." % SSH_KEY)
        return None

    def status(self, lab_reachable=None):
        """Return the actions, current and last runs, any problem."""
        with self._lock:
            running = self.proc is not None and self.proc.poll() is None
            return {"actions": [dict(v, id=k) for k, v in ACTIONS.items()],
                    "boards": list(BOARDS), "running": running,
                    "current": self.last if running else None, "last": self.last,
                    "problem": self.checks(), "lab_reachable": lab_reachable}

    def run(self, action, boards, requester, lab_reachable):
        """Start a deployment script.

        Args:
            action: A key of ACTIONS.
            boards: For "platforms": the platforms to link ("red",
                "black", "blue").
            requester: Who asked (shown with the run).
            lab_reachable: Whether the lab console answers.

        Returns:
            The new status.

        Raises:
            ApiError: if the action is unknown, the lab isn't
                reachable, the scripts can't run here, no platform was
                chosen, or another script is running.
        """
        # Can it run?
        if action not in ACTIONS:
            raise ApiError("Unknown action %r." % action)
        if not lab_reachable:
            raise ApiError("The lab console isn't reachable: the deployments need this "
                           "computer on the lab network.")
        problem = self.checks()
        if problem:
            raise ApiError(problem)

        # Arguments: the chosen platforms, without the sudo step (it
        # needs a terminal).
        args, env = [], dict(os.environ)
        if action == "platforms":
            boards = [b for b in (boards or []) if b in BOARDS]
            if not boards:
                raise ApiError("Choose at least one platform.")
            args = boards
            env["NO_POWER"] = "1"
        script = DEPLOY_DIR / ACTIONS[action]["script"]
        if not script.exists():
            raise ApiError("%s is missing." % script)

        with self._lock:
            if self.proc is not None and self.proc.poll() is None:
                raise ApiError("%s is already running; wait for it, or cancel it."
                               % ACTIONS[self.last["action"]]["title"])

            # Its own process group, so Cancel stops the ssh and rsync
            # it runs too; no input, so a prompt fails at once.
            self.proc = subprocess.Popen(["bash", str(script)] + args, cwd=str(DEPLOY_DIR),
                                         env=env, stdin=subprocess.DEVNULL,
                                         stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                         start_new_session=True)
            self.last = {"action": action, "title": ACTIONS[action]["title"], "args": args,
                         "started": time.time(), "finished": None, "code": None, "ok": None,
                         "by": requester}
            proc = self.proc

        # Follow its output in the background.
        hub.log("deploy", "=== %s (%s%s) ===" % (ACTIONS[action]["title"],
                                                ACTIONS[action]["script"],
                                                " " + " ".join(args) if args else ""))
        threading.Thread(target=self._follow, args=(proc,), daemon=True).start()
        self._publish()
        return self.status(lab_reachable)

    def cancel(self):
        """Stop the running script and everything it started."""
        with self._lock:
            proc = self.proc
        if proc is None or proc.poll() is not None:
            return
        try:
            os.killpg(proc.pid, signal.SIGTERM)
        except OSError:
            pass
        hub.log("deploy", "Cancelled.")

    def _follow(self, proc):
        """Log a script's output until it ends; record the result."""
        for raw in proc.stdout:
            hub.log("deploy", decode(raw).rstrip("\n"))
        code = proc.wait()
        with self._lock:
            if self.last is not None:
                self.last.update(finished=time.time(), code=code, ok=code == 0)
        hub.log("deploy", "=== %s (code %d) ===" % ("Done" if code == 0 else "Failed", code))
        self._publish()

    def _publish(self):
        """Send the status to the pages."""
        hub.publish("deploy", self.status())


deployer = Deployer()
