# spot.sim.debugLog

**File:** `+spot/+sim/debugLog.m` · **Package:** [[spot.sim|spot-sim]]

<!-- source: +spot/+sim/debugLog.m -->

## What it is for

The platforms show a debug message (the text the diagram writes to `RED_Logger`, `BLACK_Logger`, `BLUE_Logger`, e.g. "Chaser: Running Phase 1"). In a simulation, [[spot.hardware.debugMessages|spot-hardware-debugMessages]] calls this whenever a platform's message changes, and the console's Messages panel shows the record, live and in the replay.

## How to call it

```matlab
spot.sim.debugLog('reset')
spot.sim.debugLog(k, t, codes)     % platform k (1 RED, 2 BLACK, 3 BLUE) now shows the text in codes, at t
log = spot.sim.debugLog('get')     % log.RED.t, log.RED.text (and BLACK, BLUE)
new = spot.sim.debugLog('new')     % entries since the last 'new': platform, t, text
```

## Line by line

**Lines 12–17.** Persistent `entries` (a struct array in arrival order) and `delivered` (how many the last `'new'` returned); the platform names in index order.

**Lines 19–38.** Text commands: `reset` empties the record; `get` splits the entries per platform into a column of times and a cell column of texts; `new` returns the tail since the last call and moves the mark.

**Lines 40–41.** A block call: the text arrives as character codes padded with zeros; keep the non-zero ones, make text, trim spaces.

**Lines 42–47.** An empty message is only worth recording when the platform previously showed something (it means "stopped showing a message"); otherwise append the entry.

<!-- xref -->
- **Used by:** `+spot/+hardware/debugMessages.m`, `+spot/+sim/run.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.hardware.debugMessages|spot-hardware-debugMessages]], [[spot.sim.run|spot-sim-run]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+sim/debugLog.m</b> (48 lines; click to show)</summary>

```matlab
   1  function out = debugLog(platform, t, codes)
   2  %DEBUGLOG The platforms' debug messages recorded during a simulation (by spot.hardware.debugMessages).
   3  %   spot.sim.debugLog('reset')          start a new record (spot.sim.run does this)
   4  %   spot.sim.debugLog(k, t, codes)      platform k (1 RED, 2 BLACK, 3 BLUE) changed its message to
   5  %                                       the text in codes (uint8 character codes) at time t
   6  %   log = spot.sim.debugLog('get')      the record: struct with RED, BLACK and BLUE, each with t
   7  %                                       (column, s) and text (cell array), in the order they came
   8  %   new = spot.sim.debugLog('new')      the messages added since the previous 'new' (struct array
   9  %                                       with platform, t and text), for the console while it runs
  10  %   An empty message (all zeros) is recorded as "" only after a non-empty one, i.e. when a
  11  %   platform stops showing a message.
  12  persistent entries delivered
  13  if isempty(entries)
  14      entries = struct('platform', {}, 't', {}, 'text', {});
  15      delivered = 0;
  16  end
  17  names = {'RED', 'BLACK', 'BLUE'};
  18  
  19  if ischar(platform) || isstring(platform)
  20      switch char(platform)
  21          case 'reset'
  22              entries = struct('platform', {}, 't', {}, 'text', {});
  23              delivered = 0;
  24              out = [];
  25          case 'get'
  26              out = struct();
  27              for k = 1:3
  28                  mine = entries(strcmp({entries.platform}, names{k}));
  29                  out.(names{k}) = struct('t', reshape([mine.t], [], 1), 'text', {reshape({mine.text}, [], 1)});
  30              end
  31          case 'new'
  32              out = entries(delivered + 1:end);
  33              delivered = numel(entries);
  34          otherwise
  35              error('spot:sim:debugLog', 'Use ''reset'', ''get'' or ''new''.');
  36      end
  37      return
  38  end
  39  
  40  codes = double(codes(:)');
  41  text = strtrim(char(codes(codes > 0)));
  42  name = names{platform};
  43  previous = entries(strcmp({entries.platform}, name));
  44  if isempty(text) && (isempty(previous) || isempty(previous(end).text))
  45      return   % nothing to show yet
  46  end
  47  entries(end + 1) = struct('platform', name, 't', double(t), 'text', text);
  48  end
```

</details>
<!-- /listing -->
