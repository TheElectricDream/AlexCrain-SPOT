# spot.sim.errorDetails

**File:** `+spot/+sim/errorDetails.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/errorDetails.m -->

## What it is for

A Simulink error is a chain: "Error in simulating X" → "caused by: block Y failed" → "caused by: Variable 'K_RED' does not exist", with hyperlinks and handles inside the messages. This takes any error and returns the parts the console shows: a one-line root cause with the block it is in, the chain of causes, the blocks named (so *Show Block* can open them), where in *your* code it happened, and the full report.

## How to call it

```matlab
d = spot.sim.errorDetails(err)
d = spot.sim.errorDetails(err, projectFolder)    % also fills d.where
d.summary    % 'Variable ''K_RED'' does not exist. (in 4 - Control/Default LQR Control (RED))'
d.chain      % struct array: message, identifier, depth, blocks
d.blocks     % full block paths
d.where      % 'SPOT_User_Parameters.m, line 12' when the error is in the project's own code
d.output     % what Simulink printed before failing (SimulationFailed only)
d.report     % getReport(err), without hyperlinks
```

## Line by line

**Lines 15–19.** Default root; empty output and report.

**Lines 20–34.** A [[SimulationFailed|spot-sim-SimulationFailed]] carries Simulink's original error and the captured output: take both, make the report from the original, and continue with the original. Any other error: its report and its stack. `getReport` is in `try` because some diagnostic objects can't produce one.

**Lines 35–38.** Strip hyperlinks from the report (`plain`, lines 113–118); if there is no report, the message itself serves.

**Lines 40–42.** Walk the chain of causes (`walk`, lines 59–77): depth-first, recording each distinct message with its identifier, depth and the blocks it names (`blockPaths`), skipping messages already seen (Simulink repeats the root message at several levels). `blocks` is every block named, in order, once.

**Lines 44–53.** The summary: the deepest message is the root cause; the block named closest to it (searching back from it) is appended as "(in <path without the model name>)" (`shortPath`, lines 120–126).

**Lines 55–56.** Pack the result; `where` (lines 136–150) scans the stack for the first frame inside the project folder that is not in `+spot`: that is the user's own code (`SPOT_User_Parameters.m`, a custom block's helper), reported as path and line.

**Lines 79–111.** `blockPaths`: blocks are found two ways. The regular expression picks the block paths out of Simulink's `open_and_hilite_hyperlink('...')` links in the message (undoubling quotes). Then the error object's `handles` and `paths` properties (when it has them) are turned into full names with `getfullname`. Only paths with a `/` are kept (a block inside the model, not the model itself), unique, in order.

**Lines 128–134.** `stackOf`: the error's stack, or empty if it has none.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/spot_console/common.py`, `+spot/+console/webapp/static/app.js`, `+spot/+sim/SimulationFailed.m`, `+spot/+sim/run.m`, `+spot/+sim/showBlock.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.sim.SimulationFailed|spot-sim-SimulationFailed]], [[spot.sim.showBlock|spot-sim-showBlock]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/errorDetails.m</b> (150 lines; click to show)</summary>

```matlab
   1  function d = errorDetails(err, root)
   2  %ERRORDETAILS An error's root cause, the blocks involved and the full report, as plain text.
   3  %   d = spot.sim.errorDetails(err) takes any MATLAB error (an MException, Simulink's
   4  %   MSLException, a spot.sim.SimulationFailed or an MSLDiagnostic) and returns a struct with
   5  %     summary  one line: the root cause, and the block it is in (if any)
   6  %     chain    the messages from the outermost to the root cause, each with its identifier,
   7  %              depth and the blocks it names (struct array: message, identifier, depth, blocks)
   8  %     blocks   every block named, as full paths (for spot.sim.showBlock)
   9  %     where    'file.m, line N' when the error comes from the project's own code (e.g.
  10  %              SPOT_User_Parameters.m), otherwise ''
  11  %     output   what Simulink printed before the error (spot.sim.SimulationFailed only)
  12  %     report   MATLAB's full report (getReport), without hyperlinks
  13  %   d = spot.sim.errorDetails(err, projectFolder) also looks for 'where' in that folder.
  14  %   Simulink's messages carry hyperlinks; they are removed everywhere.
  15  if nargin < 2
  16      root = '';
  17  end
  18  output = '';
  19  report = '';
  20  if isa(err, 'spot.sim.SimulationFailed')
  21      output = err.Output;
  22      try
  23          report = getReport(err.Original, 'extended', 'hyperlinks', 'off');
  24      catch
  25      end
  26      stack = stackOf(err.Original);
  27      err = err.Original;
  28  else
  29      try
  30          report = getReport(err, 'extended', 'hyperlinks', 'off');
  31      catch
  32      end
  33      stack = stackOf(err);
  34  end
  35  report = plain(report);   % (Simulink's messages carry their hyperlinks into the report)
  36  if isempty(report)
  37      report = plain(err.message);
  38  end
  39  
  40  chain = struct('message', {}, 'identifier', {}, 'depth', {}, 'blocks', {});
  41  chain = walk(err, 0, chain);
  42  blocks = unique([chain.blocks], 'stable');
  43  
  44  % The root cause: the deepest message, with the deepest block named on the way to it
  45  summary = 'Unknown error.';
  46  if ~isempty(chain)
  47      [~, k] = max([chain.depth]);
  48      summary = chain(k).message;
  49      named = find(~cellfun(@isempty, {chain(1:k).blocks}), 1, 'last');
  50      if ~isempty(named)
  51          summary = sprintf('%s (in %s)', summary, shortPath(chain(named).blocks{1}));
  52      end
  53  end
  54  
  55  d = struct('summary', summary, 'chain', chain, 'blocks', {blocks}, 'where', where(stack, root), ...
  56             'output', output, 'report', strtrim(report));
  57  end
  58  
  59  function chain = walk(e, depth, chain)
  60  % Depth first through the causes, skipping messages already listed
  61  msg = plain(e.message);
  62  if ~isempty(msg) && ~any(strcmp({chain.message}, msg))
  63      chain(end + 1) = struct('message', msg, 'identifier', char(e.identifier), 'depth', depth, ...
  64                              'blocks', {blockPaths(e)});
  65  end
  66  causes = {};
  67  try
  68      causes = e.cause;
  69  catch
  70  end
  71  if ~iscell(causes)
  72      causes = num2cell(causes);
  73  end
  74  for k = 1:numel(causes)
  75      chain = walk(causes{k}, depth + 1, chain);
  76  end
  77  end
  78  
  79  function paths = blockPaths(e)
  80  % Blocks named in Simulink's hyperlinks, and the handles/paths the error carries
  81  paths = {};
  82  tok = regexp(e.message, 'open_and_hilite_hyperlink\s*\(\s*''((?:[^'']|'''')*)''', 'tokens');
  83  for k = 1:numel(tok)
  84      paths{end + 1} = strrep(tok{k}{1}, '''''', ''''); %#ok<AGROW>
  85  end
  86  for name = {'handles', 'paths'}
  87      try
  88          h = e.(name{1});
  89      catch
  90          continue
  91      end
  92      if ~iscell(h)
  93          h = num2cell(h);
  94      end
  95      for k = 1:numel(h)
  96          try
  97              if ischar(h{k}) || isstring(h{k})
  98                  p = cellstr(h{k});
  99              else
 100                  p = cellstr(getfullname(h{k}));   % (several handles give several names)
 101              end
 102              p = p(~cellfun(@isempty, p));
 103              paths = [paths, reshape(p, 1, [])]; %#ok<AGROW>
 104          catch
 105          end
 106      end
 107  end
 108  % Blocks only (a path inside the model), not the model itself; block names may contain line
 109  % breaks, which are kept as they are
 110  paths = unique(paths(contains(paths, '/')), 'stable');
 111  end
 112  
 113  function text = plain(text)
 114  % Without Simulink's hyperlinks
 115  text = char(text);
 116  text = regexprep(text, '<a\s[^>]*>|</a>', '');
 117  text = strtrim(text);
 118  end
 119  
 120  function p = shortPath(p)
 121  % A block path without the model's name in front
 122  slash = find(p == '/', 1);
 123  if ~isempty(slash)
 124      p = p(slash + 1:end);
 125  end
 126  end
 127  
 128  function stack = stackOf(e)
 129  stack = struct('file', {}, 'name', {}, 'line', {});
 130  try
 131      stack = e.stack;
 132  catch
 133  end
 134  end
 135  
 136  function w = where(stack, root)
 137  % The first frame in the project's own code (not the console's +spot package)
 138  w = '';
 139  if isempty(root)
 140      return
 141  end
 142  pkg = [filesep '+spot' filesep];
 143  for k = 1:numel(stack)
 144      f = stack(k).file;
 145      if startsWith(f, [root filesep]) && ~contains(f, pkg)
 146          w = sprintf('%s, line %d', f(numel(root) + 2:end), stack(k).line);
 147          return
 148      end
 149  end
 150  end
```

</details>
<!-- /listing -->
