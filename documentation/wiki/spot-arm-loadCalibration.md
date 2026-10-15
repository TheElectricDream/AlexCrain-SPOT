# spot.arm.loadCalibration

**File:** `+spot/+arm/loadCalibration.m` · **Package:** [[spot.arm|spot-arm]]

<!-- source: +spot/+arm/loadCalibration.m -->

## What it is for

Apply the file `calibrate_arm.py` writes ([[page|Tool-calibrate-arm]]) to an arm model: for each joint it has, the four friction parameters and the armature (given directly, or derived from a measured inertia about the joint at a known pose by subtracting the links' part from the mass matrix). Values the file lacks (or has as `null`) keep the model's. FILE may also be the lab's calibration, `resources/spot_calibration.json` (its `arm` section, which the [[Calibration tab|Calibration-Tab]]'s arm calibration writes), or the calibration itself as a struct.

```json
{ "joints": [ { "index": 1, "static": 0.31, "coulomb": 0.18, "stribeck": 0.04, "viscous": 0.08, "armature": 0.0032 }, ... ] }
```

## Line by line

**Lines 12–24.** A struct is the calibration itself (named "the arm calibration" in messages); otherwise read the JSON, and if it is the lab's calibration (`format` `spot-calibration`) take its `arm` section (an error if it has none) and name it `<file> (arm)`.

**Lines 25–33.** It must have `joints`; make it a cell array whether it decoded as a struct array or a single struct.

**Lines 34–45.** For each joint: its index must be 1, 2 or 3; each finite scalar friction field replaces the model's.

**Lines 46–60.** The armature: a positive `armature` is taken as is; otherwise, with `inertia` and a 3-element `pose`, the mass matrix at that pose (base fixed) gives the links' inertia about that joint, and what is left is the rotor's; if that isn't positive, warn and keep the model's.

**Line 62.** Remember the file's path (or name) in the model as `a.calibration`.

<!-- xref -->
- **Used by:** `+spot/+arm/compareSteps.m`, `+spot/+arm/parameters.m`, `resources/arm_calibration/calibrate_arm.py`
- **Uses:** [[spot.arm.defaults|spot-arm-defaults]], [[spot.arm.inertiaMatrix|spot-arm-inertiaMatrix]], [[spot.arm.parameters|spot-arm-parameters]]
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+arm/loadCalibration.m</b> (63 lines; click to show)</summary>

```matlab
   1  function a = loadCalibration(file, a)
   2  %LOADCALIBRATION Apply the arm calibration (resources/arm_calibration/calibrate_arm.py) to a model.
   3  %   a = spot.arm.loadCalibration(file, a) reads the JSON file the calibration script writes and
   4  %   replaces, for each joint it has, the friction (static, coulomb, stribeck, viscous) and, when
   5  %   measured, the armature, in the arm model a (spot.arm.defaults / spot.arm.parameters). A
   6  %   value the file doesn't have (or has as null) keeps the model's. FILE may also be the lab's
   7  %   calibration (resources/spot_calibration.json: its arm section, which the Calibration tab's arm
   8  %   calibration writes), or the calibration itself as a struct.
   9  %
  10  %   Example:
  11  %     armModel = spot.arm.loadCalibration('resources/arm_calibration.json', armModel);
  12  if isstruct(file)
  13      c = file;
  14      file = 'the arm calibration';
  15  else
  16      c = jsondecode(fileread(file));
  17      if isfield(c, 'format') && strcmp(c.format, 'spot-calibration')   % the lab's calibration: its arm section
  18          if ~isfield(c, 'arm') || ~isstruct(c.arm)
  19              error('spot:arm:loadCalibration', '%s has no arm calibration.', file);
  20          end
  21          c = c.arm;
  22          file = [file ' (arm)'];
  23      end
  24  end
  25  if ~isfield(c, 'joints')
  26      error('spot:arm:loadCalibration', '%s has no "joints" (not an arm calibration file?).', file);
  27  end
  28  joints = c.joints;
  29  if isstruct(joints) && ~isscalar(joints)
  30      joints = num2cell(joints);
  31  elseif isstruct(joints)
  32      joints = {joints};
  33  end
  34  fields = {'static', 'coulomb', 'stribeck', 'viscous'};
  35  for k = 1:numel(joints)
  36      jt = joints{k};
  37      j = jt.index;   % 1 shoulder, 2 elbow, 3 wrist
  38      if ~(isnumeric(j) && isscalar(j) && any(j == 1:3))
  39          error('spot:arm:loadCalibration', '%s: joint index must be 1, 2 or 3.', file);
  40      end
  41      for f = fields
  42          if isfield(jt, f{1}) && isnumeric(jt.(f{1})) && isscalar(jt.(f{1})) && isfinite(jt.(f{1}))
  43              a.friction.(f{1})(j) = jt.(f{1});
  44          end
  45      end
  46      if isfield(jt, 'armature') && isnumeric(jt.armature) && isscalar(jt.armature) && jt.armature > 0
  47          a.armature(j) = jt.armature;
  48      elseif isfield(jt, 'inertia') && isnumeric(jt.inertia) && isscalar(jt.inertia) && isfield(jt, 'pose') ...
  49              && numel(jt.pose) == 3 && isfield(a, 'IEE')
  50          % the inertia measured about the joint (base fixed, the other joints held) less the links'
  51          % part at that pose is the rotor's
  52          M = spot.arm.inertiaMatrix(a, [0; 0; 0; jt.pose(:); zeros(6, 1)]);
  53          rotor = jt.inertia - M(3 + j, 3 + j);
  54          if rotor > 0
  55              a.armature(j) = rotor;
  56          else
  57              warning('spot:arm:loadCalibration', ['%s: joint %d''s measured inertia (%.4g kg m^2) is below its links'' ' ...
  58                  '(%.4g); its armature is left at %.4g.'], file, j, jt.inertia, M(3 + j, 3 + j), a.armature(j));
  59          end
  60      end
  61  end
  62  a.calibration = file;
  63  end
```

</details>
<!-- /listing -->
