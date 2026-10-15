# spot.params.merge

**File:** `+spot/+params/merge.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/merge.m -->

## What it is for

Combine two settings structs: `base` (usually the defaults) with `changes` (a file, or what the page sent) laid over it. It walks into nested structs, so `changes.timing.phase2` replaces only that one number and leaves the rest of `timing` alone. Unknown fields in `changes` are ignored, which is what keeps a typo in a hand-edited file from becoming a setting.

## How to call it

```matlab
s = spot.params.merge(spot.params.defaults(), x);
```

## Line by line

**Line 5.** Start from a copy of `base`.

**Lines 6–8.** If `changes` is not a struct (e.g. an empty array from a bad file) there is nothing to overlay.

**Lines 9–13.** For every field name in `changes`: skip it unless `base` has the same field.

**Lines 14–15.** If both sides hold a scalar struct, recurse, so nested settings merge field by field too.

**Lines 16–23.** Otherwise take the value from `changes`. One correction on the way: JSON decoding gives column vectors, but the defaults use rows (e.g. `Q = [1 1 0.05 10 10 1]`), so a non-scalar numeric or logical column is transposed. Scalars and matrices are left as they are.

<!-- xref -->
- **Used by:** `+spot/+params/load.m`, `+spot/+params/update.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/merge.m</b> (25 lines; click to show)</summary>

```matlab
   1  function s = merge(base, changes)
   2  %MERGE Copy the fields of CHANGES over BASE, recursing into structs.
   3  %   s = spot.params.merge(base, changes). Fields only in BASE keep their value, so settings saved
   4  %   by an older version gain any new fields with their defaults. Fields not in BASE are ignored.
   5  s = base;
   6  if ~isstruct(changes)
   7      return
   8  end
   9  for f = fieldnames(changes).'
  10      name = f{1};
  11      if ~isfield(base, name)
  12          continue
  13      end
  14      if isstruct(base.(name)) && isscalar(base.(name)) && isstruct(changes.(name))
  15          s.(name) = spot.params.merge(base.(name), changes.(name));
  16      else
  17          value = changes.(name);
  18          % JSON arrays come back as columns; keep vectors as rows, like the defaults
  19          if (isnumeric(value) || islogical(value)) && iscolumn(value) && ~isscalar(value)
  20              value = value.';
  21          end
  22          s.(name) = value;
  23      end
  24  end
  25  end
```

</details>
<!-- /listing -->
