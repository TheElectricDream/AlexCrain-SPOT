# spot.workspace.inspect

**File:** `+spot/+workspace/inspect.m` · **Package:** [[spot.workspace|spot-workspace]]

<!-- source: +spot/+workspace/inspect.m -->

## What it is for

What the Workspace pane shows when you click a variable: a numeric matrix (up to 50×12 of it), a struct's fields with previews (click a field to go deeper), a timeseries as a small plot (resampled to 600 points), or anything else as MATLAB would display it. Only a variable name, with optional `.field` and `(index)` parts, is accepted, so nothing else can be executed through it.

## How to call it

```matlab
r = spot.workspace.inspect('dataClass.RED_Px_m');   % r.series.t, r.series.data
r = spot.workspace.inspect('K_RED');                % r.values (3x6)
```

## Line by line

**Lines 8–10.** The expression must match `name(.field|(number))*`; anything else is refused.

**Lines 11–13.** Evaluate it in the base workspace; record name, class and size.

**Lines 14–20.** A numeric or logical matrix: the top-left 50×12 as doubles, a flag if it was cut, real parts only for complex values.

**Lines 21–29.** A scalar struct: each field's name, class, size and one-line preview.

**Lines 30–39.** A timeseries: time as a column, data as doubles (3-D data flattened so samples run down), the first 6 columns, and up to 600 evenly spaced samples, with the units.

**Lines 40–46.** Anything else: `disp` captured, cut to 4000 characters.

<!-- xref -->
- **Used by:** `+spot/+worker/Worker.m`
- **Uses:** [[spot.workspace.preview|spot-workspace-preview]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+workspace/inspect.m</b> (47 lines; click to show)</summary>

```matlab
   1  function r = inspect(expr)
   2  %INSPECT Details of a workspace variable (or a field of one) for the Workspace panel.
   3  %   r = spot.workspace.inspect('dataClass.RED_Px_m') returns name, class, size and one of:
   4  %     values        numeric/logical matrices (up to 50 x 12 shown)
   5  %     fields        structs: name, class, size and preview of each field
   6  %     series        timeseries: time and data, resampled to at most 600 points
   7  %     text          anything else, as MATLAB displays it (shortened)
   8  if isempty(regexp(expr, '^[A-Za-z]\w*(\.[A-Za-z]\w*|\(\d+\))*$', 'once'))
   9      error('spot:workspace:inspect', '%s is not a variable name.', expr);
  10  end
  11  x = evalin('base', expr);
  12  r = struct('name', expr, 'class', class(x), ...
  13             'size', strjoin(arrayfun(@num2str, size(x), 'UniformOutput', false), 'x'));
  14  if (isnumeric(x) || islogical(x)) && ismatrix(x)
  15      r.values = double(x(1:min(end, 50), 1:min(end, 12)));
  16      r.truncated = any(size(x) > [50 12]);
  17      if ~isreal(x)
  18          r.values = real(r.values);
  19          r.note = 'complex: real parts shown';
  20      end
  21  elseif isstruct(x) && isscalar(x)
  22      f = fieldnames(x);
  23      r.fields = struct('name', f, 'class', '', 'size', '', 'value', '');
  24      for k = 1:numel(f)
  25          v = x.(f{k});
  26          r.fields(k).class = class(v);
  27          r.fields(k).size = strjoin(arrayfun(@num2str, size(v), 'UniformOutput', false), 'x');
  28          r.fields(k).value = spot.workspace.preview(v);
  29      end
  30  elseif isa(x, 'timeseries')
  31      t = x.Time(:);
  32      d = double(x.Data);
  33      if ndims(d) == 3
  34          d = reshape(d, [], size(d, 3)).';
  35      end
  36      d = d(:, 1:min(end, 6));
  37      n = numel(t);
  38      pick = unique(round(linspace(1, n, min(n, 600))));
  39      r.series = struct('t', t(pick).', 'data', d(pick, :).', 'units', char(x.DataInfo.Units));
  40  else
  41      text = evalc('disp(x)');
  42      if strlength(text) > 4000
  43          text = [text(1:4000) newline '...'];
  44      end
  45      r.text = text;
  46  end
  47  end
```

</details>
<!-- /listing -->
