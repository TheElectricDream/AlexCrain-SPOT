# spot.workspace.preview

**File:** `+spot/+workspace/preview.m` · **Package:** [[spot.workspace|spot-workspace]]

<!-- source: +spot/+workspace/preview.m -->

## What it is for

One line of text for a value, as the Value column of MATLAB's Workspace browser would show it: a number, a small matrix, a quoted string, "3x1 struct (4 fields)", "timeseries, 5501 samples", or the size and class when there is too much to show.

## How to call it

```matlab
spot.workspace.preview([1 2 3])          % '[1 2 3]'
spot.workspace.preview(dataClass)        % '1x1 struct (108 fields)'
```

## Line by line

**Lines 4–5.** Empty by default; the size as text.

**Lines 6–15.** Numeric or logical matrices: `[]`, a scalar with 6 significant digits, up to 10 elements with `mat2str`, otherwise size and class.

**Lines 16–19.** A char row in single quotes; a string scalar in double quotes.

**Lines 20–29.** Structs with their field count, timeseries with their length, cells and other classes with their size, function handles as text.

**Lines 31–33.** Cut to 80 characters.

<!-- xref -->
- **Used by:** `+spot/+workspace/inspect.m`, `+spot/+workspace/list.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+workspace/preview.m</b> (34 lines; click to show)</summary>

```matlab
   1  function t = preview(x)
   2  %PREVIEW Short text for a value, like the Value column of MATLAB's Workspace browser.
   3  %   t = spot.workspace.preview(x) returns '' when the value is too large to show in a line.
   4  t = '';
   5  sz = strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x');
   6  if (isnumeric(x) || islogical(x)) && ismatrix(x)
   7      if isempty(x)
   8          t = '[]';
   9      elseif isscalar(x)
  10          t = num2str(x, 6);
  11      elseif numel(x) <= 10
  12          t = mat2str(x, 5);
  13      else
  14          t = sprintf('%s %s', sz, class(x));
  15      end
  16  elseif ischar(x) && (isrow(x) || isempty(x))
  17      t = ['''' x ''''];
  18  elseif isstring(x) && isscalar(x)
  19      t = ['"' char(x) '"'];
  20  elseif isstruct(x)
  21      t = sprintf('%s struct (%d fields)', sz, numel(fieldnames(x)));
  22  elseif isa(x, 'timeseries')
  23      t = sprintf('timeseries, %d samples', x.Length);
  24  elseif iscell(x)
  25      t = sprintf('%s cell', sz);
  26  elseif isa(x, 'function_handle')
  27      t = func2str(x);
  28  else
  29      t = sprintf('%s %s', sz, class(x));
  30  end
  31  if strlength(t) > 80
  32      t = [t(1:77) '...'];
  33  end
  34  end
```

</details>
<!-- /listing -->
