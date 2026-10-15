# spot.workspace.list

**File:** `+spot/+workspace/list.m` · **Package:** [[spot.workspace|spot-workspace]]

<!-- source: +spot/+workspace/list.m -->

## What it is for

The rows of the Workspace pane: every variable in the base workspace with its size, class, bytes and a short preview, like MATLAB's own Workspace browser.

## How to call it

```matlab
vars = spot.workspace.list();   % struct array: name, size ('3x1'), class, bytes, value
```

## Line by line

**Line 5.** `whos` run in the base workspace.

**Lines 6–15.** For each entry, a preview of its value ([[preview|spot-workspace-preview]]; any failure leaves it empty) and the size as text (`sizeText`, 18–20, joins the dimensions with `x`).

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.workspace.preview|spot-workspace-preview]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+workspace/list.m</b> (20 lines; click to show)</summary>

```matlab
   1  function vars = list()
   2  %LIST The base workspace's variables, as MATLAB's Workspace browser shows them.
   3  %   vars = spot.workspace.list() returns a struct array with name, size (e.g. '3x1'), class,
   4  %   bytes and value (a short preview, or '' when too large to show).
   5  w = evalin('base', 'whos');
   6  vars = struct('name', {}, 'size', {}, 'class', {}, 'bytes', {}, 'value', {});
   7  for k = 1:numel(w)
   8      preview = '';
   9      try
  10          preview = spot.workspace.preview(evalin('base', w(k).name));
  11      catch
  12      end
  13      vars(end + 1) = struct('name', w(k).name, 'size', sizeText(w(k).size), ...
  14          'class', w(k).class, 'bytes', w(k).bytes, 'value', preview); %#ok<AGROW>
  15  end
  16  end
  17  
  18  function t = sizeText(sz)
  19  t = strjoin(arrayfun(@num2str, sz, 'UniformOutput', false), 'x');
  20  end
```

</details>
<!-- /listing -->
