# Using the Documentation Tab

This tab, on both consoles: guides to SPOT and the lab, and a page for every function in the project, read from the code itself.

## Finding things

- **Search** (the box at the top left; press `/` or Ctrl+K to go there). It finds functions by their name, by what their documentation and comments say, and by any word in their code, and guides by their titles and text. Suggestions appear as you type (↑↓ and Enter to open one); Enter on its own opens all the results.
  - All the words must match. `"a phrase"` matches the words together.
  - Filters: `lang:matlab` (or `python`, `javascript`, `cpp`, `shell`, `diagram`), `kind:method` (or `function`, `class`, `local`, `nested`, `block`, ...), `in:code` (only lines of code), `in:guides`, `in:docs` (only documentation, not code), `file:sim.js`.
- **Find** (Ctrl+F, or the **Find** button) highlights a word on the page you are reading, including inside the code: Enter and Shift+Enter go to the next and previous match, **Aa** matches case. When you open a result from a search, Find opens with your search's longest word.
- **Contents** (left): **Guides**, or the **Reference**: the code's groups (MATLAB, the console's server and page, the lab programs, the Simulink diagrams), then each package or folder, file and function. **Filter the list** narrows the Reference to names containing a word. ☰ hides the contents.
- **On This Page** (right) lists the page's sections; the one you are reading is marked.
- **API Routes and MATLAB Commands** and **Every Function, A to Z** (under Reference Tools) are generated from the code too.

## A function's page

- Its **name**, language and kind (function, local function, method, class, ...), its file and lines, and what it is defined inside.
- **What it does**: the code's own documentation (its MATLAB help text, Python docstring or JSDoc comment), laid out as a summary, its inputs and outputs, and its other sections. A standard method (a System object's `stepImpl`, a Python `__init__`) is described by what calls it. A helper defined in a group under one comment shares that comment.
- A **Note**, if someone wrote one (below).
- **The Code, Block by Block**: the function's code, block by block, each under the comment that explains it, as written in the code. Functions defined inside it are links to their own pages. Click a line number to see that line in the whole file.
- **Connections**: what it calls and what calls it, found by name in the code, with the line of each call; and the console routes or MATLAB commands it handles or sends.

Every page is rebuilt from the code whenever a file changes, so it always matches the code in this project. A function the code doesn't describe says so: a note can fill the gap.

## Editing code

**Edit** on a function's or file's page opens the file in the editor, at that function (marked). The editor highlights the language, folds blocks, finds and replaces (Ctrl+F in the editor), offers completions where the language supports them, and **checks the code as you type**. Problems are underlined, listed under **Problems** on the right (click one to go to it), and counted in the editor's bar; the line at the bottom says which checks ran. A faint line marks the coding standard's line width.

| Language | Checks |
|---|---|
| JavaScript | ESLint, in the page: mistakes (undefined names, unused variables, unreachable code, ...) and the coding standard's Google-style rules |
| Python | syntax; Python 3.6 syntax and library for the console's code (the TX2 runs 3.6); undefined and unused names (pyflakes, if installed, else the console's own check); PEP 8 widths, one statement per line, docstrings, shadowed built-ins |
| MATLAB | MATLAB's Code Analyzer (on the local console, with MATLAB connected and free), and the coding standard: width, one statement per line, shadowed functions, the H1 line and `Contents.m`, the verbatim sections the controller check compares |
| shell | `bash -n`, and ShellCheck if installed |
| C, C++ | the compiler's syntax check (local console, with g++ or gcc installed) |
| JSON, XML | whether they parse |
| Markdown (these guides, notes) | links to guides, functions and pictures that don't exist |

**Save** (Ctrl+S) writes the file to this project, **on the local console only**. It refuses if the file changed on disk since you opened it (then **Reload from Disk**, or **Save Anyway**), and it keeps the previous version in `.spot_cache/docs_backups/`. After saving:

- a MATLAB file is used from MATLAB's next call (the console asks MATLAB to `rehash`);
- the console's Python takes effect when the console restarts (`SPOT stop`, then `SPOT`);
- the page's JavaScript and CSS take effect when you reload the page;
- the lab's programs (StreamData, the Teensy firmware, the deployment scripts) need deploying ([Lab Setup](page:lab-setup)).

**Revert** goes back to the saved version; **Download** saves the text as a file. On the lab console the editor checks your changes but can't save: download the file instead, and save it from the local console. The diagram's MATLAB Function blocks are shown read-only: edit them in Simulink (**Show in Simulink** opens the diagram at the block, on the local console).

> [!WARNING]
> Saving changes the project's code. The same rules apply as for any edit: simulate before taking a change to the lab. Keep the verbatim sections of `spot.control.lqrCommand` and `spot.control.thrusterDutyCycles` identical to the diagram's blocks: `spot.control.verifyConsoleController` compares them.

## Notes

**Add Note** (or **Edit Note**) on a function's page opens its notes file in the editor, at the function's section. Notes are Markdown, kept in `+interface/+console/webapp/docs/notes/`, one file per source file (`notes/<path of the file>.md`). The text before the first heading is about the file; each `## name` section is about the function of that name (as the function's page shows it under its file, e.g. `Handler.do_GET` or `cleanMessages`). Notes are for what the code's comments don't say: why it is done this way, what to watch for, how it is used in the lab. They are kept with the project and show on the function's page above its code.

## Writing guides

The guides are Markdown files in `+interface/+console/webapp/docs/guides/`, listed in `docs/toc.json` (its sections, and each page's `id`, `file` and a short `summary`). **Edit Page** on a guide opens it in the editor, with **Preview** beside it. Besides ordinary Markdown (headings, lists, tables, code blocks with a language), they can use:

| Write | For |
|---|---|
| `[text](page:quick-start-simulation)`, `[text](page:tab-simulation#running)` | a link to a guide, or a heading in it |
| `[text](ref:spot.sim.run)` | a link to a function's page (its full name) |
| `[text](file:server.py)`, `[text](file:+spot/+sim/run.m:40)` | a link to a file's page (a file name is enough if only one file has it), at a line |
| `` `spot.sim.run` ``, `` `server.py` `` | inline code that names a function or a file becomes a link by itself |
| `$x^2$`, `$$ ... $$` | maths (KaTeX) |
| `> [!NOTE]`, `> [!TIP]`, `> [!WARNING]`, `> [!CAUTION]`, `> [!IMPORTANT]` | a highlighted box |
| `![caption](images/name.png)` | a picture from `docs/images/` |

The editor's link check finds links to pages, functions and pictures that don't exist. [How This Documentation Works](page:dev-documentation) explains how the Reference is built.
