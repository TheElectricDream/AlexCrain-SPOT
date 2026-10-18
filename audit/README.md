# SPOT Audit

A game for reading every chunk of the project: the documentation, the MATLAB code, the Python
code, the JavaScript and the page, the shell scripts, the Arduino and C/C++ code, and the
configuration files. It looks like the SPOT Console and reuses its editor.

```
python3 audit/app.py            # opens http://localhost:8090
python3 audit/app.py --port 9000 --no-browser
```

## How it plays

- **A chunk** is a comment and the code under it (a `%%` section, a function and its help, a
  docstring and its body, a JSDoc comment and its function), or a heading's worth of a guide. Long
  ones are split at blank lines into parts. The chunks of a file join back into the file byte for
  byte.
- For each chunk, the original is on the left and your version on the right. **Accept As Is**
  (`A`) if it is right; edit it and **Save My Rewrite** (`Ctrl+Enter`) to own it in your own words;
  **Flag…** (`F`) with a note to come back to it; **Skip** (`S`) to leave it for later.
- The check line over the editor says whether a rewrite changed **only the comments** or **the
  code too** (comments and whitespace stripped, the tokens compared). For guides it just says
  "Rewritten".
- **Diff** shows your changes line by line; **Context** shows the lines around the chunk; the
  **Preview** button renders a Markdown chunk.
- The strip under the stage is the file's chunks: green accepted, blue rewritten, grey skipped, a
  dot for a flag. Click one to jump to it.
- **Next Pending** (`Enter`) goes to the next chunk still to do, in reading order: chapter by
  chapter, section by section, file by file. Auto-Advance does that after each accept or save.

## What is kept, and where

| Path | What |
|---|---|
| `audit/original/` | The snapshot of the project the game plays on (taken at the first start; delete it to take a new one). Gitignored. |
| `audit/work/` | Every file with a rewrite, whole, under its project path. Gitignored. |
| `audit/saves/autosave.json` | Statuses, rewrites, flags, the log, the achievements and the time played, written after every change. |
| `audit/saves/<name>.json` | Manual saves (**Save Game** in the top bar), the same content with a name. **Load Game** lists them all, the autosave first, with the time played and the progress of each. Loading replaces the current progress (and the autosave follows), so save a slot first to keep it. |
| `audit/backup/<date>/` | The project's copies of files replaced by **Apply** on the Export tab. Gitignored. |

The **Export** tab lists the rewritten files with their diffs. **Apply** copies one (or all) over
the project's copy, backing the old one up first; a file edited in the project since the snapshot
is marked so you look at it before applying. Chunk keys are hashes of the chunk's file and text,
so progress survives a new snapshot wherever the text is unchanged.

Not covered: third-party code (`static/vendor`, `libowl-src-410`), images, the diagram and other
binaries.

## Chapters

1. Documentation: the guides in the Documentation tab's order, the notes on the code, the project's
   README-type files, the release notes (`changes/patch-notes.tex`).
2. MATLAB: the project root (`SPOT.m`, `Constants.m`, `New_SPOT_Project.m`) and every `+spot`
   package.
3. Python: the console server, SPOT Vision, the arm calibration, the CAD tools.
4. JavaScript and the page: the console's scripts, `index.html`, the stylesheets, the editor bundle.
5. Shell scripts: deployment, the platforms' agent and serial links, builds, services.
6. Arduino and C/C++: the Teensy firmware, the platforms' C code, StreamData.
7. Configuration: the table-of-contents file and `package.json` (the settings are constants in
   `Constants.m`, audited with the MATLAB).

## Code

`audit_game/chunker.py` cuts the files and checks a rewrite's code; `catalog.py` takes the snapshot
and places every file in a chapter and section; `progress.py` keeps the progress file, the work
copy and the achievements; `server.py` serves the page and the JSON API (standard library only).
The page is `static/index.html`, `audit.css` and `audit.js`; CodeMirror, marked and KaTeX come from
the SPOT Console's `static/vendor` (served at `/vendor`).
