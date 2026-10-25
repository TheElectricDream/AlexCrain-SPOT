# How This Documentation Works

## The Reference: read from the code

When the Documentation tab opens, the console's server builds an index of the project's code ([docs.py](file:docs.py)), and builds it again whenever a file changes (it compares each file's size and time; only changed files are read again).

1. **Which files.** The project's top-level `.m` files, everything in `+spot` and `+interface` (except third-party code, the 3D models and caches), and the MATLAB Function blocks of the `.slx` diagrams at the top of the project. On the lab console, the copy of the project's code that `deploy_tx2.sh` puts on the TX2.
2. **Each file is parsed** ([docparse.py](file:docparse.py)) by a reader for its language: MATLAB (functions, local and nested functions, classes, their methods and properties), Python (its own syntax tree), JavaScript (a tokenizer that finds functions, arrow functions, methods, classes and handlers), C and C++, and shell scripts. A diagram's MATLAB Function blocks are read from the `.slx` file (a zip of XML), and blocks with the same code are documented once, listing every block that uses it.
3. **Its documentation** is its help text, docstring or JSDoc, or the comment directly above it, laid out in sections (inputs, outputs, behaviour, "see also"). Standard methods (`setupImpl`, `__init__`, ...) get a standard description, and helpers defined together under one comment share it.
4. **Its walkthrough** splits the body at its comments: each comment that starts a block, with the code under it. `%%` sections and one-line title comments become headings; long blocks are split again at the comments inside them; functions defined inside become links.
5. **Connections** are found by name: `spot.*` names in MATLAB, imports and `self.` in Python, scopes and imports in JavaScript, and the routes (`/api/...`) and MATLAB commands named in each language.

The index is sent to the page compressed, and the page keeps it: search, Find and the pages all run in the browser. Nothing is generated ahead of time, so the Reference can't go out of date.

## The guides and the notes

Hand-written, in `+interface/+console/webapp/docs/`:

```text
docs/
├── toc.json          the guides' sections and pages (id, file, summary)
├── guides/*.md       the guides
├── notes/<path>.md   notes on a source file's functions ("## name" sections)
├── notes/areas/<key>.md   an overview shown on a package's or folder's page (e.g. areas/spot.sim.md)
└── images/           pictures the guides use
```

[Using the Documentation Tab](page:tab-documentation) describes the Markdown they can use.

## Saving and checking

Saving goes through `docs.py`'s `save`: on the local console only (its requests carry the session token), only files the index shows or the documentation's own, never over a newer version on disk, and with a backup of the old one in `.spot_cache/docs_backups/`. The checks are [doclint.py](file:doclint.py) on the server (Python, MATLAB with the Code Analyzer through the worker's `code.lint` command, shell, C/C++, JSON, XML, the guides' links) and ESLint in the page for JavaScript ([docs.js](file:docs.js)).
