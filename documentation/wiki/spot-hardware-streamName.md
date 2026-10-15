# spot.hardware.streamName

**File:** `+spot/+hardware/streamName.m` · **Package:** [[spot.hardware|spot-hardware]]

<!-- source: +spot/+hardware/streamName.m -->

## What it is for

The name a Live Stream block shows in the Live Plot, worked out by the block's mask when the diagram is updated or built: the name typed into the block if any, otherwise the data store read by the block connected to its input (e.g. `RED_Measured_States`), else the signal line's name, else the source block's name. At most 40 printable characters.

## Line by line

**Lines 8–10.** The typed name, trimmed; `stream` as the fallback.

**Lines 11–25.** Follow the block's input port to its line and the line's source block: a Data Store Read gives its store's name; a named line its name; else the block's name. Any failure (nothing connected yet) keeps the fallback.

**Lines 26–28.** Collapse whitespace, keep printable ASCII, cut to 40.

<!-- xref -->
- **Used by:** `+spot/+hardware/liveStream.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/streamName.m</b> (29 lines; click to show)</summary>

```matlab
   1  function label = streamName(block, name)
   2  %STREAMNAME The name a Live Stream block shows in the SPOT Console's Live Plot.
   3  %   label = spot.hardware.streamName(block, name) is called by the Live Stream block's mask (in
   4  %   MATLAB, when the diagram is updated or built). With a name typed into the block, that name;
   5  %   otherwise the name of what is connected to its input: a Data Store Read's data store (e.g.
   6  %   RED_Measured_States), else the signal's name, else the source block's name.
   7  %   At most 40 printable characters.
   8  label = strtrim(char(name));
   9  if isempty(label)
  10      label = 'stream';
  11      try
  12          ph = get_param(block, 'PortHandles');
  13          line = get_param(ph.Inport(1), 'Line');
  14          src = get_param(line, 'SrcBlockHandle');
  15          if strcmp(get_param(src, 'BlockType'), 'DataStoreRead')
  16              label = get_param(src, 'DataStoreName');
  17          elseif ~isempty(strtrim(get_param(line, 'Name')))
  18              label = get_param(line, 'Name');
  19          else
  20              label = get_param(src, 'Name');
  21          end
  22      catch
  23          % nothing connected yet
  24      end
  25  end
  26  label = regexprep(label, '\s+', ' ');
  27  label = label(label >= 32 & label < 127);
  28  label = label(1:min(end, 40));
  29  end
```

</details>
<!-- /listing -->
