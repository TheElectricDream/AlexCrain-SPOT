# files.py

**File:** `+spot/+console/webapp/spot_console/files.py` · **Part of:** [[the console's server|Console-Server]]

<!-- source: +spot/+console/webapp/spot_console/files.py -->

## What it is for

The folder browser behind the PhaseSpace tab's *Browse…* (choosing StreamData's output folder).

## Function by function

**Lines 17–34.** `places`: the shortcuts in the dialog: the project's output folder, the project, home, (under WSL) the Windows home and each drive, Desktop and Documents if they exist, and the root.

**Lines 37–56.** `browse(path)`: the folder to show (the output folder by default; relative paths from home; the nearest existing parent if it doesn't exist), its subfolders (hidden and `$` ones skipped, sorted case-insensitively), its parent, the separator, any error, and the places.

**Lines 62–75.** `make_folder`: *New Folder*: a valid name (no path characters), under an existing folder; an existing folder of that name is fine.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/spot_console/files.py</b> (75 lines; click to show)</summary>

```python
   1  """The folder browser (choosing StreamData's output folder).
   2  
   3  Part of the SPOT Console (webapp/app.py starts it).
   4  """
   5  
   6  import os
   7  from pathlib import Path
   8  
   9  from .common import ApiError, PROJECT_DIR
  10  from .events import settings
  11  from .phasespace import WSL_INFO, windows_home
  12  
  13  
  14  _windows_home = []  # cached: [path or None]
  15  
  16  
  17  def places():
  18      out = [{"label": "Project Output", "path": str(PROJECT_DIR / "output")},
  19             {"label": "Project", "path": str(PROJECT_DIR)},
  20             {"label": "Home", "path": str(Path.home())}]
  21      if WSL_INFO:
  22          if not _windows_home:
  23              _windows_home.append(windows_home())
  24          if _windows_home[0]:
  25              out.append({"label": "Windows Home", "path": _windows_home[0]})
  26          for d in sorted(Path("/mnt").glob("[a-z]")):
  27              if d.is_dir():
  28                  out.append({"label": "%s: (Windows)" % d.name.upper(), "path": str(d), "drive": True})
  29      for name in ("Desktop", "Documents"):
  30          p = Path.home() / name
  31          if p.is_dir():
  32              out.append({"label": name, "path": str(p)})
  33      out.append({"label": "/", "path": "/", "drive": True})
  34      return out
  35  
  36  
  37  def browse(path):
  38      p = Path(path or settings.get("output_dir")).expanduser()
  39      if not p.is_absolute():
  40          p = Path.home() / p
  41      while not p.is_dir() and p.parent != p:  # fall back to the nearest folder that exists
  42          p = p.parent
  43      dirs, error = [], None
  44      try:
  45          with os.scandir(p) as it:
  46              for e in it:
  47                  try:
  48                      if e.is_dir() and not e.name.startswith((".", "$")):
  49                          dirs.append(e.name)
  50                  except OSError:
  51                      pass
  52      except OSError as e:
  53          error = str(e)
  54      dirs.sort(key=str.lower)
  55      return {"path": str(p), "parent": str(p.parent) if p.parent != p else None, "dirs": dirs,
  56              "sep": os.sep, "error": error, "places": places()}
  57  
  58  
  59  # --------------------------------------------------------------------------- HTTP
  60  
  61  
  62  def make_folder(body):
  63      parent, name = Path(str(body.get("parent", ""))), str(body.get("name", "")).strip()
  64      if not name or any(c in name for c in '/\\:*?"<>|') or name in (".", ".."):
  65          raise ApiError("That is not a valid folder name.")
  66      if not parent.is_dir():
  67          raise ApiError("%s is not a folder." % parent)
  68      target = parent / name
  69      try:
  70          target.mkdir()
  71      except FileExistsError:
  72          pass
  73      except OSError as e:
  74          raise ApiError("Could not create %s: %s" % (target, e))
  75      return {"path": str(target)}
```

</details>
<!-- /listing -->
