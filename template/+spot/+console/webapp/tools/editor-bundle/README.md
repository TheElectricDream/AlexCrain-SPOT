# Documentation tab: third-party bundles

The Documentation tab (`static/docs.js`) uses four third-party libraries, served from
`static/vendor` so the console works without the internet:

| Bundle | Contents | Used for |
|---|---|---|
| `vendor/codemirror/codemirror.js` | CodeMirror 6 and its languages (MATLAB through a mode of our own in cm-entry.js, Python, JavaScript, C/C++, shell, CSS, HTML, JSON, Markdown, XML) | the editor, and highlighting code on every page |
| `vendor/eslint/eslint.js` | ESLint 9's `Linter` (browser build) and the browser globals | checking JavaScript as it is edited |
| `vendor/markdown/markdown.js` | marked 12 and KaTeX | the guides and notes, with their equations |
| `vendor/katex/` | KaTeX's stylesheet and fonts | the equations |

`cm-entry.js`, `eslint-entry.js` and `markdown-entry.js` say what each bundle exports, and
`package.json` pins every version. To rebuild them (for example to upgrade one), run `./build.sh`
with Node.js installed. The console itself needs neither Node.js nor these files.

The bundles are minified and keep their licence notices at the end; each vendor folder also has
the licences as `LICENSE.txt`. All are MIT-licensed.
