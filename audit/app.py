#!/usr/bin/env python3
"""SPOT Audit: a game for reading every chunk of the project.

Shows the project one chunk at a time ([comment] -> [code], or a
piece of a guide), in chapters: the documentation, the MATLAB code,
the Python code, the JavaScript and the page, the shell scripts, the
Arduino and C/C++ code, and the configuration files. For each chunk
you either accept it as it is or rewrite it in your own words; you
can flag anything to come back to. Progress, flags and achievements
are kept in audit/progress.json.

The game plays on a snapshot of the project taken at the first start
(audit/original/). Rewritten files are kept, whole, in audit/work/
with the same paths; the Export tab copies them over the project (with
a backup in audit/backup/) when you are ready.

    python3 audit/app.py                 # http://localhost:8090
    python3 audit/app.py --port 9000
    python3 audit/app.py --no-browser

Needs only Python 3.8 or newer. The page reuses the SPOT Console's
own editor and Markdown bundles (template/.../static/vendor).
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from audit_game.server import main  # noqa: E402

if __name__ == "__main__":
    main()
