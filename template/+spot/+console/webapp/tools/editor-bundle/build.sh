#!/usr/bin/env bash
# Rebuilds the Documentation tab's third-party bundles in ../../static/vendor from the exact
# versions in package.json. Needs Node.js and npm (only to rebuild; the console doesn't need them).
#   ./build.sh
set -euo pipefail
cd "$(dirname "$0")"
VENDOR=../../static/vendor
npm install --no-audit --no-fund
mkdir -p "$VENDOR/codemirror" "$VENDOR/eslint" "$VENDOR/markdown" "$VENDOR/katex/fonts"

# The editor and highlighter, ESLint, and Markdown with KaTeX: one ES module each, minified.
bundle() {
    npx esbuild "$1" --bundle --format=esm --minify --legal-comments=eof --outfile="$2"
}
bundle cm-entry.js "$VENDOR/codemirror/codemirror.js"
bundle eslint-entry.js "$VENDOR/eslint/eslint.js"
bundle markdown-entry.js "$VENDOR/markdown/markdown.js"

# KaTeX's stylesheet and fonts (woff2: every current browser reads it).
cp node_modules/katex/dist/katex.min.css "$VENDOR/katex/"
cp node_modules/katex/dist/fonts/*.woff2 "$VENDOR/katex/fonts/"
cp node_modules/katex/LICENSE "$VENDOR/katex/LICENSE.txt"
echo "Done: $VENDOR"
