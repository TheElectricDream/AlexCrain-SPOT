# spot.console.open

**File:** `+spot/+console/open.m` · **Package:** [[spot.console|spot-console]]

<!-- source: +spot/+console/open.m -->

## What it is for

`SPOT open`: print the running console's address (with this session's token: the server only accepts requests from pages opened with it) as a clickable link, and open it in the browser.

## Line by line

**Lines 6–12.** Default: show it. No running console is an error.

**Line 13.** The URL: local host, the web port, the token, and `#simulation` so the page opens on the Simulation tab.

**Line 14.** Print a `matlab:` hyperlink that calls [[openBrowser|spot-console-openBrowser]], and the plain address to paste.

**Lines 15–17.** Open the browser; say so if none could be started.

<!-- xref -->
- **Used by:** `+spot/+console/start.m`, `SPOT.m`
- **Uses:** [[spot.console.openBrowser|spot-console-openBrowser]], [[spot.console.status|spot-console-status]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/open.m</b> (18 lines; click to show)</summary>

```matlab
   1  function url = open(root, show)
   2  %OPEN Open (or just show the address of) the project's running SPOT Console.
   3  %   url = spot.console.open(projectFolder) opens the console in the browser and prints its
   4  %   address; spot.console.open(projectFolder, false) only prints it. The address includes this
   5  %   session's token: the console only accepts requests from pages opened with it.
   6  if nargin < 2
   7      show = true;
   8  end
   9  ready = spot.console.status(root);
  10  if isempty(ready)
  11      error('spot:console:open', 'The SPOT Console is not running. Start it with SPOT.');
  12  end
  13  url = sprintf('http://127.0.0.1:%d/?token=%s#simulation', ready.web_port, ready.token);
  14  fprintf('SPOT Console: <a href="matlab:spot.console.openBrowser(''%s'');">open in the browser</a>  (or paste: %s )\n', url, url);
  15  if show && ~spot.console.openBrowser(url)
  16      fprintf('Could not start a browser: paste the address above into one.\n');
  17  end
  18  end
```

</details>
<!-- /listing -->
