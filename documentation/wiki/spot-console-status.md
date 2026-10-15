# spot.console.status

**File:** `+spot/+console/status.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/status.m -->

## What it is for

Is the console running for this project? The server writes `.spot_cache/console.json` (web port, worker port, token, pid, root) when it starts; this reads it and checks that the server still answers `/api/ping`.

## Line by line

**Lines 5–9.** Empty unless the file exists.

**Lines 10–15.** Decode it and ping the web port with a 2 s timeout; any failure (stale file, server gone) leaves the result empty.

<!-- xref -->
- **Used by:** `+spot/+console/open.m`, `+spot/+console/start.m`, `+spot/+console/stop.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/status.m</b> (16 lines; click to show)</summary>

```matlab
   1  function ready = status(root)
   2  %STATUS The project's running SPOT Console (ports and token), or [] if none answers.
   3  %   ready = spot.console.status(projectFolder) reads .spot_cache/console.json, written by the
   4  %   console's web server when it starts, and checks that the server still answers.
   5  ready = [];
   6  file = fullfile(root, '.spot_cache', 'console.json');
   7  if ~isfile(file)
   8      return
   9  end
  10  try
  11      info = jsondecode(fileread(file));
  12      webread(sprintf('http://127.0.0.1:%d/api/ping', info.web_port), weboptions('Timeout', 2));
  13      ready = info;
  14  catch
  15  end
  16  end
```

</details>
<!-- /listing -->
