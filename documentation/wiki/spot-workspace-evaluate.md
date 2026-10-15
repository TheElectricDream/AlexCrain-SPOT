# spot.workspace.evaluate

**File:** `+spot/+workspace/evaluate.m` · **Package:** [[spot.workspace|spot-workspace]]

<!-- source: +spot/+workspace/evaluate.m -->

## What it is for

The console's MATLAB command window: run what you typed in the base workspace (so it sees and can change the same variables as the diagram) and hand back what it printed, or the error message. Commands that wait for the keyboard (`input`, `keyboard`, `pause`) would block MATLAB and should not be sent.

## How to call it

```matlab
r = spot.workspace.evaluate('K_RED');      % r.output = 'K_RED = ...', r.error = ''
```

## Line by line

**Line 6.** Empty output and error.

**Lines 7–8.** `evalin('base', command)` runs the command in the base workspace; `evalc` around it captures the text it prints.

**Lines 9–13.** On error, the message; if the error came from inside a function (the first stack frame isn't this one), add where.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+workspace/evaluate.m</b> (15 lines; click to show)</summary>

```matlab
   1  function r = evaluate(command)
   2  %EVALUATE Run a command in the base workspace and capture what it prints.
   3  %   r = spot.workspace.evaluate('x = magic(3)') returns output (the printed text) and error
   4  %   (the error message, '' if none), like typing the command in MATLAB's Command Window.
   5  %   Commands that wait for keyboard input (input, keyboard, pause) would block MATLAB.
   6  r = struct('output', '', 'error', '');
   7  try
   8      r.output = evalc('evalin(''base'', command);');
   9  catch err
  10      r.error = err.message;
  11      if ~isempty(err.stack) && ~strcmp(err.stack(1).name, 'evaluate')
  12          r.error = sprintf('%s\n(in %s, line %d)', err.message, err.stack(1).name, err.stack(1).line);
  13      end
  14  end
  15  end
```

</details>
<!-- /listing -->
