// CodeMirror 6 for the SPOT Console's Documentation tab: the editor, its languages, linting,
// search, and a function that highlights code as static HTML (the same classes as the editor).
export { EditorState, Compartment, StateEffect, StateField, RangeSetBuilder } from '@codemirror/state';
export {
  EditorView, keymap, lineNumbers, highlightActiveLine, highlightActiveLineGutter,
  highlightSpecialChars, drawSelection, dropCursor, rectangularSelection, crosshairCursor,
  Decoration, ViewPlugin, WidgetType, showPanel, highlightTrailingWhitespace, highlightWhitespace,
} from '@codemirror/view';
export {
  defaultKeymap, history, historyKeymap, indentWithTab, toggleComment, undo, redo,
} from '@codemirror/commands';
export {
  searchKeymap, search, openSearchPanel, closeSearchPanel, highlightSelectionMatches,
  SearchQuery, setSearchQuery, findNext, findPrevious,
} from '@codemirror/search';
export {
  autocompletion, completionKeymap, closeBrackets, closeBracketsKeymap,
} from '@codemirror/autocomplete';
export {
  linter, lintGutter, lintKeymap, setDiagnostics, forEachDiagnostic, openLintPanel,
  closeLintPanel, nextDiagnostic, diagnosticCount, forceLinting,
} from '@codemirror/lint';
export {
  syntaxHighlighting, indentOnInput, bracketMatching, foldGutter, foldKeymap, indentUnit,
  StreamLanguage, LanguageSupport, syntaxTree,
} from '@codemirror/language';
export { classHighlighter, highlightTree, tags } from '@lezer/highlight';

import { javascript } from '@codemirror/lang-javascript';
import { python } from '@codemirror/lang-python';
import { cpp } from '@codemirror/lang-cpp';
import { css } from '@codemirror/lang-css';
import { html } from '@codemirror/lang-html';
import { json, jsonParseLinter } from '@codemirror/lang-json';
import { markdown } from '@codemirror/lang-markdown';
import { xml } from '@codemirror/lang-xml';
import { StreamLanguage } from '@codemirror/language';
import { shell } from '@codemirror/legacy-modes/mode/shell';
import { properties } from '@codemirror/legacy-modes/mode/properties';
import { highlightTree, classHighlighter } from '@lezer/highlight';

export { jsonParseLinter };

// MATLAB, as SPOT writes it: keywords (with classdef's blocks), %% sections, %{ %} block
// comments, ... continuations, 'strings' told apart from transposes, "strings", numbers, field
// names, and the name a function line defines.
const M_KEYWORDS = new Set([
  'break', 'case', 'catch', 'classdef', 'continue', 'else', 'elseif', 'end', 'for', 'function',
  'global', 'if', 'otherwise', 'parfor', 'persistent', 'return', 'spmd', 'switch', 'try', 'while',
  'arguments', 'properties', 'methods', 'events', 'enumeration', 'import',
]);
const M_ATOMS = new Set(['true', 'false', 'pi', 'Inf', 'inf', 'NaN', 'nan', 'eps']);
const M_BUILTINS = new Set([
  'zeros', 'ones', 'eye', 'size', 'numel', 'length', 'isempty', 'isfield', 'isfile', 'isfolder',
  'error', 'warning', 'fprintf', 'sprintf', 'disp', 'struct', 'cell', 'fieldnames', 'cellfun',
  'arrayfun', 'strcmp', 'strcmpi', 'contains', 'startsWith', 'endsWith', 'strrep', 'strsplit',
  'strjoin', 'regexp', 'regexprep', 'num2str', 'str2double', 'jsonencode', 'jsondecode',
  'fullfile', 'fileparts', 'exist', 'nargin', 'nargout', 'max', 'min', 'sum', 'abs', 'sqrt',
  'round', 'floor', 'ceil', 'mod', 'rem', 'any', 'all', 'find', 'sort', 'unique', 'numel',
  'isnumeric', 'ischar', 'isstruct', 'iscell', 'islogical', 'isa', 'class', 'double', 'single',
  'int32', 'uint8', 'logical', 'char', 'string', 'cos', 'sin', 'tan', 'atan2', 'exp', 'log',
  'norm', 'inv', 'pinv', 'diag', 'repmat', 'reshape', 'cat', 'horzcat', 'vertcat', 'mean',
  'std', 'interp1', 'isrow', 'iscolumn', 'nan', 'inf', 'setfield', 'getfield', 'rmfield',
  'evalin', 'assignin', 'evalc', 'feval', 'tic', 'toc', 'onCleanup', 'clear', 'rethrow',
  'MException', 'containers', 'datestr', 'datetime', 'fopen', 'fclose', 'fwrite', 'fread',
  'mkdir', 'rmdir', 'delete', 'copyfile', 'movefile', 'dir', 'open_system', 'get_param',
  'set_param', 'sim', 'load_system', 'timeseries', 'coder', 'height', 'width',
]);

const matlabMode = {
  name: 'matlab',
  startState: () => ({ block: 0, sig: 0, value: false, dot: false }),
  token(stream, state) {
    if (stream.sol()) {
      state.value = false;
      state.dot = false;
      if (!state.cont) {
        state.sig = 0;
      }
      state.cont = false;
    }

    // Block comments: %{ and %} alone on their lines.
    if (stream.sol() && stream.match(/^\s*%\{\s*$/)) {
      state.block++;
      return 'comment';
    }
    if (state.block) {
      if (stream.match(/^\s*%\}\s*$/)) {
        state.block--;
      } else {
        stream.skipToEnd();
      }
      return 'comment';
    }
    if (stream.eatSpace()) {
      return null;
    }

    // Comments, sections and continuations.
    if (stream.match('%%')) {
      stream.skipToEnd();
      return 'comment strong';
    }
    if (stream.peek() === '%') {
      stream.skipToEnd();
      return 'comment';
    }
    if (stream.match('...')) {
      stream.skipToEnd();
      state.cont = true;
      return 'comment';
    }

    // Strings, and the transpose (after a value).
    const ch = stream.peek();
    if (ch === "'" && state.value) {
      stream.next();
      return 'operator';
    }
    if (ch === "'" || ch === '"') {
      stream.next();
      while (!stream.eol()) {
        if (stream.next() === ch) {
          if (stream.peek() === ch) {
            stream.next();
            continue;
          }
          break;
        }
      }
      state.value = true;
      return 'string';
    }

    // Numbers (1, 2.5, .5, 1e-3, 2i).
    if (stream.match(/^(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?[ij]?/)) {
      state.value = true;
      return 'number';
    }

    // Names: a field, a keyword, the function being defined, a constant, a built-in.
    if (stream.match(/^[A-Za-z]\w*/)) {
      const word = stream.current();
      state.value = true;
      if (state.dot) {
        state.dot = false;
        return 'property';
      }
      if (M_KEYWORDS.has(word)) {
        state.value = word === 'end';
        if (word === 'function') {
          state.sig = 1;
        }
        return 'keyword';
      }
      if (state.sig) {
        if (stream.match(/^\s*[=,\]]/, false)) {
          return 'variable';
        }
        state.sig = 0;
        return 'def';
      }
      if (M_ATOMS.has(word)) {
        return 'atom';
      }
      return M_BUILTINS.has(word) ? 'builtin' : 'variable';
    }

    // Brackets and operators.
    if (ch === '.' && /[A-Za-z(]/.test(stream.string.charAt(stream.pos + 1))) {
      stream.next();
      state.dot = /[A-Za-z]/.test(stream.peek());
      state.value = false;
      return 'operator';
    }
    if (stream.match(/^[)\]}]/)) {
      state.value = true;
      return 'bracket';
    }
    if (stream.match(/^[(\[{]/)) {
      state.value = false;
      return 'bracket';
    }
    if (stream.match(/^(?:==|~=|<=|>=|&&|\|\||\.\*|\.\/|\.\^|\.\\|\.'|[-+*\/\\^<>=~&|!:@,;])/)) {
      state.value = stream.current() === ".'";
      return 'operator';
    }
    stream.next();
    state.value = false;
    return null;
  },
  languageData: { commentTokens: { line: '%', block: { open: '%{', close: '%}' } } },
};

const STREAM = {
  matlab: () => StreamLanguage.define(matlabMode),
  shell: () => StreamLanguage.define(shell),
  ini: () => StreamLanguage.define(properties),
};
const FULL = {
  javascript: () => javascript(),
  python: () => python(),
  cpp: () => cpp(),
  c: () => cpp(),
  arduino: () => cpp(),
  css: () => css(),
  html: () => html(),
  json: () => json(),
  markdown: () => markdown(),
  xml: () => xml(),
};
const cache = {};

/** The editor extension for a language name ([] for plain text). */
export function languageFor(name) {
  if (FULL[name]) {
    return FULL[name]();
  }
  if (STREAM[name]) {
    return STREAM[name]();
  }
  return [];
}

// The parser's language for static highlighting (cached).
function languageObject(name) {
  if (!(name in cache)) {
    let lang = null;
    if (FULL[name]) {
      lang = FULL[name]().language;
    } else if (STREAM[name]) {
      lang = STREAM[name]();
    }
    cache[name] = lang;
  }
  return cache[name];
}

const escapeHtml = s => s.replace(/[&<>]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;' }[c]));

/**
 * Highlights code as HTML, one string per line (tok-* classes, as in the editor).
 * @param {string} text The code.
 * @param {string} name The language name (see languageFor).
 * @return {!Array<string>} The lines, as HTML.
 */
export function highlightLines(text, name) {
  const lang = languageObject(name);
  const out = [];
  let line = '';
  const emit = (from, to, cls) => {
    const parts = text.slice(from, to).split('\n');
    parts.forEach((p, i) => {
      if (i > 0) {
        out.push(line);
        line = '';
      }
      if (p) {
        line += cls ? `<span class="${cls}">${escapeHtml(p)}</span>` : escapeHtml(p);
      }
    });
  };
  if (!lang) {
    emit(0, text.length, '');
  } else {
    const tree = lang.parser.parse(text);
    let pos = 0;
    highlightTree(tree, classHighlighter, (from, to, classes) => {
      if (from > pos) {
        emit(pos, from, '');
      }
      emit(from, to, classes);
      pos = to;
    });
    if (pos < text.length) {
      emit(pos, text.length, '');
    }
  }
  out.push(line);
  return out;
}
