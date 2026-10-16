// ESLint's Linter (browser build) and the browser globals, for the Documentation tab's editor.
export { Linter } from 'eslint-linter-browserify';
import globals from 'globals';
export const browserGlobals = globals.browser;
