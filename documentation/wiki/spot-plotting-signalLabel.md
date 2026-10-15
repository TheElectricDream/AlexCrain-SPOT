# spot.plotting.signalLabel

**File:** `+spot/+plotting/signalLabel.m` · **Package:** [[spot.plotting|spot-plotting]]

<!-- source: +spot/+plotting/signalLabel.m -->

## What it is for

The diagram logs signals with names like `BLACK_Vx_mpers` or `ARM_Rz_Shoulder_rad`: `<PLATFORM>_<quantity>_<unit>`. This turns such a name into its parts for axis labels and legends: platform `BLACK`, quantity "X-Velocity", unit `m/s`, label "X-Velocity [m/s]". Names that don't follow the pattern just get their underscores replaced by spaces.

## How to call it

```matlab
L = spot.plotting.signalLabel('ARM_RzD_Elbow_radpers');   % platform 'ARM', quantity 'Elbow Rate', unit 'rad/s'
```

## Line by line

**Lines 6–12.** Empty result; peel off a leading platform name.

**Lines 13–21.** Try the unit suffixes from the most specific (`_mpers2`) to the least (`_m`, `_s`); the first match sets the unit and is removed.

**Lines 22–29.** Split the rest on `_`; tables for the base quantities (`Px` → X-Position, `RzD` → Angular Rate, …), the prefixes (`Desired`, `Path`, `Sat` → Saturated, `IMU`) and the joint names.

**Lines 30–43.** Sort each word into: the main quantity (first base word), a prefix, a joint, or "other".

**Lines 44–51.** With a joint: "Angular Rate" becomes "Rate", "Attitude" becomes "Angle", and the joint's name goes in front ("Shoulder Angle"); if there was no base word, the other words serve as the quantity.

**Lines 52–60.** Join prefixes, main and others; collapse spaces; fall back to the raw name; the label is the quantity with the unit in brackets when there is one.

<!-- xref -->
- **Used by:** `+spot/+console/webapp/static/results.js`, `+spot/+data/summary.m`, `+spot/+data/toFigure.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+plotting/signalLabel.m</b> (61 lines; click to show)</summary>

```matlab
   1  function L = signalLabel(name)
   2  %SIGNALLABEL Readable name and unit of a logged signal, for figure labels.
   3  %   L = spot.plotting.signalLabel('BLACK_Vx_mpers') returns platform ('BLACK'), quantity
   4  %   ('X-Velocity'), unit ('m/s', TeX) and label ('X-Velocity [m/s]'). Names that don't follow
   5  %   the diagrams' <PLATFORM>_<quantity>_<unit> pattern get their underscores turned to spaces.
   6  L = struct('platform', '', 'quantity', '', 'unit', '', 'label', '');
   7  rest = name;
   8  tok = regexp(rest, '^(RED|BLACK|BLUE|ARM)_(.*)$', 'tokens', 'once');
   9  if ~isempty(tok)
  10      L.platform = tok{1};
  11      rest = tok{2};
  12  end
  13  units = {'_mpers2$', 'm/s^2'; '_radpers2$', 'rad/s^2'; '_mpers$', 'm/s'; '_radpers$', 'rad/s'; ...
  14           '_Nm$', 'N m'; '_N$', 'N'; '_rad$', 'rad'; '_m$', 'm'; '_s$', 's'};
  15  for k = 1:size(units, 1)
  16      if ~isempty(regexp(rest, units{k, 1}, 'once'))
  17          L.unit = units{k, 2};
  18          rest = regexprep(rest, units{k, 1}, '');
  19          break
  20      end
  21  end
  22  words = strsplit(rest, '_');
  23  base = containers.Map( ...
  24      {'Px', 'Py', 'Rz', 'Vx', 'Vy', 'RzD', 'Ax', 'Ay', 'RzDD', 'Fx', 'Fy', 'Tz', 'Gz', 'Time'}, ...
  25      {'X-Position', 'Y-Position', 'Attitude', 'X-Velocity', 'Y-Velocity', 'Angular Rate', ...
  26       'X-Acceleration', 'Y-Acceleration', 'Angular Acceleration', 'X-Force', 'Y-Force', 'Torque', ...
  27       'Angular Rate', 'Time'});
  28  prefix = containers.Map({'Desired', 'Path', 'Sat', 'IMU'}, {'Desired', 'Path', 'Saturated', 'IMU'});
  29  joints = {'Shoulder', 'Elbow', 'Wrist'};
  30  pre = {}; main = ''; joint = ''; other = {};
  31  for w = words
  32      if isempty(w{1})
  33          continue
  34      elseif isKey(base, w{1}) && isempty(main)
  35          main = base(w{1});
  36      elseif isKey(prefix, w{1})
  37          pre{end + 1} = prefix(w{1}); %#ok<AGROW>
  38      elseif ismember(w{1}, joints)
  39          joint = w{1};
  40      else
  41          other{end + 1} = w{1}; %#ok<AGROW>
  42      end
  43  end
  44  if ~isempty(joint)   % ARM_Rz_Shoulder_rad -> Shoulder Angle, ARM_RzD_Elbow_radpers -> Elbow Rate
  45      main = strrep(strrep(strrep(main, 'Angular Acceleration', 'Acceleration'), 'Angular Rate', 'Rate'), 'Attitude', 'Angle');
  46      if isempty(main)
  47          main = strjoin(other, ' ');
  48          other = {};
  49      end
  50      main = strtrim([joint ' ' main]);
  51  end
  52  L.quantity = strjoin([pre, {main}, other], ' ');
  53  L.quantity = strtrim(regexprep(L.quantity, '\s+', ' '));
  54  if isempty(L.quantity)
  55      L.quantity = strrep(name, '_', ' ');
  56  end
  57  L.label = L.quantity;
  58  if ~isempty(L.unit)
  59      L.label = sprintf('%s [%s]', L.quantity, L.unit);
  60  end
  61  end
```

</details>
<!-- /listing -->
