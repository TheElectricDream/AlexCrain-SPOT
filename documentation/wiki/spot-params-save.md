# spot.params.save

**File:** `+spot/+params/save.m` · **Package:** [[spot.params|spot-params]]

<!-- source: +spot/+params/save.m -->

## What it is for

Write a settings struct to a JSON file, pretty-printed (one key per line) so that a change shows as a small diff in version control.

## How to call it

```matlab
spot.params.save(s, 'spot_settings.json');
```

## Line by line

**Line 4.** `jsonencode` with `PrettyPrint` turns the struct into indented JSON text. Row vectors become JSON arrays; nested structs become objects.

**Lines 5–8.** Open the file for writing; a negative file id means it couldn't be opened (a read-only folder, a bad path), which becomes an error with the path in it.

**Line 9.** `onCleanup` closes the file when the function ends, whether normally or by an error.

**Line 10.** Write the text as characters.

<!-- xref -->
- **Used by:** `+spot/+data/saveSimulation.m`, `+spot/+params/defaults.m`, `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+params/save.m</b> (11 lines; click to show)</summary>

```matlab
   1  function save(s, file)
   2  %SAVE Write settings to a JSON file (readable, and diff-friendly in version control).
   3  %   spot.params.save(s, file)
   4  text = jsonencode(s, 'PrettyPrint', true);
   5  fid = fopen(file, 'w');
   6  if fid < 0
   7      error('spot:params:save', 'Cannot write %s.', file);
   8  end
   9  cleanup = onCleanup(@() fclose(fid));
  10  fwrite(fid, text, 'char');
  11  end
```

</details>
<!-- /listing -->
