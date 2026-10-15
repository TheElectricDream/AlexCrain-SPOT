# common.py

**File:** `+spot/+console/webapp/spot_console/common.py` · **Part of:** [[the console's server|Console-Server]]

<!-- source: +spot/+console/webapp/spot_console/common.py -->

## What it is for

The small things every other server module needs: where the files are, the platforms' display names, the error class that becomes an HTTP 400, and checks for values that come from the page.

## Function by function

**Lines 13–20.** Paths: `APP_DIR` is `webapp/`, `STATIC_DIR` the page's files, `PROJECT_DIR` the console's folder (`+spot/+console` here; `~/PhaseSpace_Server_SendAll` on the TX2), `PHASESPACE_DIR` where StreamData's source and Makefile are.

**Lines 26–35.** `PLATFORM_NAMES` and `shown` / `shown_list`: `RED` → "Chaser" and so on for text meant to be read; the API, settings and files keep the keys.

**Lines 38–44.** `ApiError`: a message for the user (the server answers 400 with it), optionally with `details` (e.g. a MATLAB error's chain, blocks and report) the page shows on request.

**Lines 47–56.** `decode`: process output bytes to text, trying UTF-8 then the file-system encoding (and dropping NULs: `wsl.exe` prints UTF-16).

**Lines 59–64.** `finite` (a float or `None`, for JSON which has no NaN) and `quote_cmd` (a command list as a shell-quoted line for the log).

**Lines 70–85.** `parse_bool` and `parse_number` (finite, within bounds) with the setting's name in the error.

**Lines 88–91.** Patterns for a host name and a Unix user name, used by the Hardware tab's address settings.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/common.py</b> (91 lines; click to show)</summary>

```python
   1  """Shared helpers: project paths, the API error, value parsing.
   2  
   3  Part of the SPOT Console (webapp/app.py starts it).
   4  """
   5  
   6  import math
   7  import re
   8  import shlex
   9  import sys
  10  from pathlib import Path
  11  
  12  
  13  APP_DIR = Path(__file__).resolve().parent.parent   # webapp/ (this file is in webapp/spot_console)
  14  
  15  
  16  STATIC_DIR = APP_DIR / "static"
  17  
  18  
  19  PROJECT_DIR = APP_DIR.parent   # the console's folder: +spot/+console here, ~/PhaseSpace_Server_SendAll on the TX2
  20  PHASESPACE_DIR = PROJECT_DIR / "phasespace"   # StreamData's source, Makefile and the PhaseSpace library
  21  # Kept in the home folder (not in Dropbox) so the laptop and the lab PC each keep their own paths
  22  
  23  
  24  # What people call the platforms: the API, the settings, the files and the platforms' own names
  25  # keep RED, BLACK and BLUE; messages meant to be read use these
  26  PLATFORM_NAMES = {"RED": "Chaser", "BLACK": "Target", "BLUE": "Obstacle"}
  27  
  28  
  29  def shown(name):
  30      """The name to show for a platform key ("RED" -> "Chaser"); other text unchanged."""
  31      return PLATFORM_NAMES.get(name, name)
  32  
  33  
  34  def shown_list(names):
  35      return ", ".join(shown(n) for n in names)
  36  
  37  
  38  class ApiError(Exception):
  39      """An error message meant for the user (sent back as HTTP 400). details: more about it for
  40      the page to show on request (MATLAB's errors: spot.sim.errorDetails), or None."""
  41  
  42      def __init__(self, message, details=None):
  43          super().__init__(message)
  44          self.details = details
  45  
  46  
  47  def decode(raw):
  48      """Decode process output; also handles wsl.exe's own UTF-16 messages."""
  49      if isinstance(raw, str):
  50          text = raw
  51      else:
  52          try:
  53              text = raw.decode("utf-8")
  54          except UnicodeDecodeError:
  55              text = raw.decode(sys.getfilesystemencoding() or "latin-1", errors="replace")
  56      return text.replace("\x00", "").rstrip("\r\n")
  57  
  58  
  59  def finite(v):
  60      return v if isinstance(v, float) and math.isfinite(v) else None
  61  
  62  
  63  def quote_cmd(cmd):
  64      return " ".join(shlex.quote(str(c)) for c in cmd)
  65  
  66  
  67  # --------------------------------------------------------------------------- settings / events
  68  
  69  
  70  def parse_bool(value, what):
  71      if value in (True, 1, "1", "true", "on", "yes"):
  72          return True
  73      if value in (False, 0, "0", "false", "off", "no"):
  74          return False
  75      raise ApiError("%s must be true or false." % what)
  76  
  77  
  78  def parse_number(value, what, low, high):
  79      try:
  80          v = float(value)
  81      except (TypeError, ValueError):
  82          raise ApiError("%s must be a number." % what)
  83      if not (math.isfinite(v) and low <= v <= high):
  84          raise ApiError("%s must be between %g and %g." % (what, low, high))
  85      return v
  86  
  87  
  88  HOST_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9.-]*$")
  89  
  90  
  91  USER_RE = re.compile(r"^[a-z_][a-z0-9_-]*$")
```

</details>
<!-- /listing -->
