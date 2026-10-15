# spot.experiment.connect

**File:** `+spot/+experiment/connect.m` · **Package:** [[spot.experiment|spot-experiment]]

<!-- source: +spot/+experiment/connect.m -->

## What it is for

The Experiment tab's *Connect*: tell the Jetson support package which board to build on. With a password it connects afresh (MATLAB then remembers the credentials); without one it reuses the last connection, provided that was the TX2.

## How to call it

```matlab
info = spot.experiment.connect(projectFolder, 'the TX2 password');
info = spot.experiment.connect(projectFolder);    % the saved credentials
```

## Line by line

**Line 6.** The board.

**Lines 7–15.** With a password: `jetson(address, user, password)`. Without: `jetson()` returns the last board MATLAB connected to; if its address isn't the TX2's, say so (connect once with the password).

**Lines 16–17.** Record the TX2 as the default board in the support package's preferences.

**Line 18.** The connection's details.

<!-- xref -->
- **Used by:** `+spot/+experiment/build.m`, `+spot/+worker/Worker.m`
- **Uses:** [[spot.experiment.buildBoard|spot-experiment-buildBoard]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+experiment/connect.m</b> (19 lines; click to show)</summary>

```matlab
   1  function info = connect(root, password)
   2  %CONNECT Connect MATLAB to the build board (the TX2), as jetson() does for a platform.
   3  %   info = spot.experiment.connect(projectFolder) reuses the credentials MATLAB saved the last time
   4  %   it connected to the TX2; spot.experiment.connect(projectFolder, password) connects with a
   5  %   password (MATLAB then remembers it). Needs the MATLAB Coder Support Package for NVIDIA Jetson.
   6  b = spot.experiment.buildBoard(root);
   7  if nargin > 1 && ~isempty(password)
   8      hw = jetson(b.address, b.user, password);
   9  else
  10      hw = jetson();   % the last board MATLAB connected to, with its saved password
  11      if ~strcmp(hw.DeviceAddress, b.address)
  12          error('spot:experiment:connect', ['MATLAB was last connected to %s, not the TX2 (%s). ' ...
  13              'Connect once with the TX2''s password.'], hw.DeviceAddress, b.address);
  14      end
  15  end
  16  setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultIpAddress', b.address);
  17  setpref('Hardware_Connectivity_Installer_NVIDIAJetson', 'DefaultUserName', b.user);
  18  info = struct('address', hw.DeviceAddress, 'user', b.user, 'board', b);
  19  end
```

</details>
<!-- /listing -->
