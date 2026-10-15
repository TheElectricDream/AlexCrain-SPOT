# viewer.js

**File:** `+spot/+console/webapp/static/viewer.js` (three.js) · **Models:** [[cad.js|Page-cad-js]]

<!-- source: +spot/+console/webapp/static/viewer.js -->

## What it is for

The 3D view of the granite table, the platforms and the LEDs, shared by every page (`app.js` moves it into the page shown). The table frame has its origin at one corner of the table top, +x along the 3.5 m side, +y along the 2.4 m side, +z up; PhaseSpace data (converted to metres by the server) and simulation results (the diagram's `Px`, `Py`, `Rz`) are both in it. Three layers: **live** (what PhaseSpace sees), **sim** (a replayed simulation as see-through platforms with paths and the arm, and loaded results laid over it as outlines with dashed paths), **setup** (the initial-condition markers, draggable; Shift+drag rotates); plus Interactive Mode's goals and thruster plumes, the Spacecraft Designer's close-up, the table slope calibration's height map, and the ZED2 Viewpoint window.

## Function by function

**Lines 27–59.** Constants: the table (3.5 × 2.4 × 0.33 m), the platform size (0.3 m), the body-axis length, the path height and width, the colours, 32 LED slots, the overlays' dash patterns, the ZED2's lens positions, housing and HD720 intrinsics (68.6° vertical field of view), the eight nozzles' positions and gas directions, the nozzle height.

**Lines 61–116.** `label` (a CSS2D text label) and `graniteTexture` (a procedural granite: cloudy variation, mineral grains, a few crystals).

**Lines 119–182, the constructor.** z up; a WebGL renderer with soft shadows (and a kept drawing buffer in capture mode); the label renderer; the scene with fog; the camera and orbit controls; the ZED2 window; lights, table, axes; the layers; the body-axes and CAD state; the Designer state; the three platforms; LEDs, live paths, the sim, setup and goal layers; dragging; resize; the default view; the render loop.

**Lines 193–213.** Lights: a hemisphere fill, a ceiling light above the table casting shadows, two side fills.

**Lines 215–305.** The table: the granite slab, the steel stand (legs, corner brackets, frame, stretchers, painted like the barrier once its CAD loads), the barrier from `spot_table.bin.gz`, the floor with a grid, the 0.5 m grid and outline on the top, the distance ticks.

**Lines 307–326.** The table's axes and the origin label.

**Lines 328–420.** `_buildCraft`: a platform as a cube (metallic for the Chaser and Target), its edges (dashed for overlays), body axes, a name tag, and on the Chaser the ZED2 housing with its two lenses; registered for the CAD switch. `_bodyAxes`: three arrows with labels.

**Lines 422–450.** The LEDs: 32 slots of core + halo + drop line + id label; `setLedOwner` colours them by platform.

**Lines 452–476.** `update(frame)`: each tracked platform's position and attitude (extending its live path), the goal lines, and each visible LED's position with its drop line scaled to its height.

**Lines 480–488.** `setLayers`: show/hide the live platforms, the sim group, the setup group, the live paths; restyle the CAD copies.

**Lines 492–535.** Live paths: a wide line of up to 20 000 segments per platform, extended only when the platform moved 3 mm, keeping the newer half when full; on/off; clear.

**Lines 542–618.** The sim layer and **tracks**: `_buildTrack` (the three ghost platforms and an arm as a line with joint balls, or CAD links), `_disposeTrack`, `_buildArm`, `_setArm` (the four joint points at mid-height; CAD links placed from joint to joint and stretched to the link length).

**Lines 622–641.** `setUseCad` (load the models the first time; reject if impossible) and `_applyCad`.

**Lines 645–656.** `setDesign`: rebuild every CAD platform with its attachments.

**Lines 658–756.** The Spacecraft Designer: `setDesigner` shows one platform's CAD model at the table's centre (loading the CAD if needed, saving and restoring the camera, looking at the face with the cone); `_buildDesigner` places the platform, the ZED2, the arm at its pose, and a disc on each of the six places (orange for the arm coupler, red when blocked, hidden under the chosen cone).

**Lines 758–807.** `_cadGhost` (a simulation alone is drawn solid; over a live view see-through; results faint), `_applyCadCraft` (swap the cube for the model and back, keep the outline for copies), `_applyCadArm`.

**Lines 809–878.** `_setTrackResult` (the paths as wide lines with a per-sample "drawn up to" index) and `_setTrackTime` (binary search for the sample at a time, linear interpolation of position and attitude, the path drawn only up to that sample, the arm's points).

**Lines 880–924.** `setSimulation`, `setPrimaryShown`, `setOverlays` (one track per loaded result with its dash pattern and offset), `setTraces`, `setSimTime`.

**Lines 928–982.** The setup layer: a marker is a translucent square with outline, body axes and a level name tag; `setSetup` places the markers (not the one being dragged) and the arm.

**Lines 986–1073.** The goal layer: a marker per platform with a dashed line to the live platform; `setGoals`; `_updateGoalLine`; `setThrust` (a cone plume at each nozzle, scaled by the duty cycle).

**Lines 1075–1183.** `_bindDrag`: pointer handling in the capture phase (so dragging a marker doesn't orbit): pick a draggable marker (setup or goal) under the pointer, drag it on the table plane (or rotate with Shift), clamp goals inside the margin, report to the module's callback; in Designer mode hover and click the slot discs; a click (not a drag) on a live platform picks it for Interactive Mode.

**Lines 1187–1231.** Display options (LED labels, names, body axes, drop lines); the table slope (1199–1225): `setTableSlope(cal)` lays the table slope calibration's height map (`heightCanvas` from `calviz.js`, jet coloured, see [[calibration.js|Page-calibration-js]]) on the table top as a see-through plane at z = 0.0005 m, under the grid lines, rebuilding it only when the calibration changes (`null` removes it), and `setTableSlopeShown(on)` shows or hides it (the 3D view's Table Slope toggle); and the camera distance that fits a width and height.

**Lines 1233–1347.** The ZED2 Viewpoint: a framed window with Left/Right/Both lens buttons and a close button, its own small renderer; `_redCameraCarrier` picks the Chaser that is shown (live, the simulation, an overlay, or the initial-condition marker); `_renderRedCam` places a camera at each lens looking along the Chaser's +x, hides the Chaser's own axes/tag/housing, and renders one lens or both side by side.

**Lines 1349–1365.** `setView`: the 3D, Top and Side cameras.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/viewer.js</b> (1366 lines; click to show)</summary>

```javascript
   1  // 3D view of the granite table, the spacecraft and the LEDs (three.js, z axis up, metres).
   2  // Table frame: (0, 0, 0) is one corner of the table top, +x runs along the 3.5 m side,
   3  // +y along the 2.4 m side, +z up. PhaseSpace data arrives in this frame (converted mm -> m by app.py),
   4  // and so do simulation results (the diagrams' Px, Py, Rz).
   5  //
   6  // One viewer is shared by all pages (app.js moves it into the page shown), with three layers:
   7  //   live    the platforms and LEDs PhaseSpace sees
   8  //   sim     a simulation replayed (see-through platforms, their paths, the arm), and loaded
   9  //           results laid over it (outlined platforms, dashed paths)
  10  //   setup   the initial-condition poses, which can be dragged (Shift+drag rotates)
  11  // and, on the lab console's PhaseSpace tab, Interactive Mode's goals (dragged the same way) and the
  12  // thrusters it fires, drawn as plumes on the live platform.
  13  
  14  import * as THREE from 'three';
  15  import { OrbitControls } from './vendor/OrbitControls.js';
  16  import { CSS2DRenderer, CSS2DObject } from './vendor/CSS2DRenderer.js';
  17  // Wide lines (WebGL draws ordinary lines 1 px wide whatever width is asked for)
  18  import { Line2 } from './vendor/lines/Line2.js';
  19  import { LineGeometry } from './vendor/lines/LineGeometry.js';
  20  import { LineSegments2 } from './vendor/lines/LineSegments2.js';
  21  import { LineSegmentsGeometry } from './vendor/lines/LineSegmentsGeometry.js';
  22  import { LineMaterial } from './vendor/lines/LineMaterial.js';
  23  import { armPoints, loadCad, mountPose, TABLE_URL } from './cad.js';
  24  import { COLORS, LINE_COLORS, LED_COLORS, TEXT_COLORS, platformName } from './platforms.js';
  25  import { heightCanvas } from './calviz.js';
  26  
  27  export const TABLE = { x: 3.5, y: 2.4, thickness: 0.33 };   // the granite (0.33 m thick, its CAD)
  28  export const CRAFT_SIZE = 0.3;
  29  const BODY_AXIS = 0.27;   // length of each body axis from the cube's centre [m]
  30  // Paths trail the platforms' centre of mass, at mid-height of the body
  31  const PATH_Z = CRAFT_SIZE / 2;
  32  const PATH_WIDTH = 3.5;   // [px] paths: traced behind the platforms as they move
  33  // The platforms' colours (platforms.js): the Chaser (RED) silver, the Target (BLACK) gold, the Obstacle (BLUE) blue
  34  export const CRAFT_COLORS = { RED: COLORS.RED, BLACK: COLORS.BLACK, BLUE: COLORS.BLUE };
  35  export { LINE_COLORS, LED_COLORS };
  36  const NUM_LED_SLOTS = 32;
  37  // Dash and gap [m] of each overlaid result's paths, in the order they were added
  38  const OVERLAY_DASHES = [[0.06, 0.035], [0.02, 0.025], [0.12, 0.04], [0.04, 0.06]];
  39  // RED's ZED2 camera (the "ZED2 Viewpoint" window), in RED's body frame [m]: on the front (+x) face,
  40  // its right lens 0.08 m right of the centre of geometry (right when looking along +x, so -y) and
  41  // 0.28 m up, the left lens 120 mm to its left; both look along +x, level with the table. The front
  42  // of its housing is 4 mm proud of the panel. With the CAD loaded, the lens positions come from the
  43  // ZED 2i model (info.zed.lenses); these are the same numbers.
  44  const ZED_LENSES = { right: [0.154, -0.08, 0.28], left: [0.154, 0.04, 0.28] };
  45  const ZED_BOX = [0.043, 0.1753, 0.0303];   // the housing [m]: depth, width, height (ZED 2 / 2i)
  46  // The default ZED2 intrinsics at HD720 (1280 x 720, rectified): fx = fy ~ 528 px, the centre in the
  47  // middle (each unit's factory calibration differs slightly). Vertical field of view 2 atan(360/528).
  48  const ZED_INTRINSICS = { width: 1280, height: 720, fx: 528, fy: 528, cx: 640, cy: 360 };
  49  const ZED_VFOV = 2 * Math.atan(ZED_INTRINSICS.cy / ZED_INTRINSICS.fy) * 180 / Math.PI;   // 68.6 deg
  50  // The 8 thrusters in the body frame (resources/ThrusterConfig.png; the mixer's H matrix in
  51  // spot.control.thrusterDutyCycles): nozzle [x, y] [m], 81 mm in from the edges, and the direction
  52  // the gas leaves in (the platform is pushed the other way)
  53  const THRUSTER_NOZZLES = [
  54    [[0.1547, 0.069], [1, 0]], [[0.1547, -0.069], [1, 0]],       // T1, T2 push along -x
  55    [[0.069, -0.1547], [0, -1]], [[-0.069, -0.1547], [0, -1]],   // T3, T4 push along +y
  56    [[-0.1547, -0.069], [-1, 0]], [[-0.1547, 0.069], [-1, 0]],   // T5, T6 push along +x
  57    [[-0.069, 0.1547], [0, 1]], [[0.069, 0.1547], [0, 1]],       // T7, T8 push along -y (nozzle tips)
  58  ];
  59  const NOZZLE_Z = 0.0276;   // [m] above the table: the MJ4 nozzles, in the side panels' bottom notches
  60  
  61  function label(text, className, background, color) {
  62    const el = document.createElement('div');
  63    el.className = 'label3d ' + className;
  64    el.textContent = text;
  65    if (background) el.style.background = background;
  66    if (color) el.style.color = color;
  67    return new CSS2DObject(el);
  68  }
  69  
  70  function graniteTexture(renderer) {
  71    const w = 2048, h = Math.round(w * TABLE.y / TABLE.x);
  72    const canvas = document.createElement('canvas');
  73    canvas.width = w; canvas.height = h;
  74    const g = canvas.getContext('2d');
  75    g.fillStyle = '#3a3d43';
  76    g.fillRect(0, 0, w, h);
  77  
  78    // Soft cloudy variation in the stone
  79    for (let i = 0; i < 320; i++) {
  80      const x = Math.random() * w, y = Math.random() * h, r = 30 + Math.random() * 160;
  81      const grad = g.createRadialGradient(x, y, 0, x, y, r);
  82      grad.addColorStop(0, Math.random() < 0.5 ? 'rgba(125,129,137,0.10)' : 'rgba(8,8,10,0.16)');
  83      grad.addColorStop(1, 'rgba(0,0,0,0)');
  84      g.fillStyle = grad;
  85      g.fillRect(x - r, y - r, 2 * r, 2 * r);
  86    }
  87  
  88    // Mineral grains: mostly dark, some grey, a few light flecks
  89    const palette = [['#111214', 0.42], ['#24262a', 0.22], ['#55595f', 0.18], ['#7d828a', 0.12], ['#aeb2b8', 0.05], ['#d9d4ca', 0.01]];
  90    const pick = () => { let r = Math.random(); for (const [c, p] of palette) { if ((r -= p) <= 0) return c; } return palette[0][0]; };
  91    const grains = Math.round(w * h * 0.055);
  92    for (let i = 0; i < grains; i++) {
  93      const x = Math.random() * w, y = Math.random() * h;
  94      g.globalAlpha = 0.5 + Math.random() * 0.5;
  95      g.fillStyle = pick();
  96      if (Math.random() < 0.88) {
  97        const s = 0.8 + Math.random() * 1.6;
  98        g.fillRect(x, y, s, s);
  99      } else {  // a larger irregular crystal
 100        const r = 1.5 + Math.random() * 3.2, sides = 5 + (Math.random() * 3 | 0), rot = Math.random() * Math.PI;
 101        g.beginPath();
 102        for (let k = 0; k < sides; k++) {
 103          const a = rot + k / sides * Math.PI * 2, rr = r * (0.6 + Math.random() * 0.5);
 104          k ? g.lineTo(x + Math.cos(a) * rr, y + Math.sin(a) * rr) : g.moveTo(x + Math.cos(a) * rr, y + Math.sin(a) * rr);
 105        }
 106        g.closePath();
 107        g.fill();
 108      }
 109    }
 110    g.globalAlpha = 1;
 111  
 112    const tex = new THREE.CanvasTexture(canvas);
 113    tex.colorSpace = THREE.SRGBColorSpace;
 114    tex.anisotropy = renderer.capabilities.getMaxAnisotropy();
 115    return tex;
 116  }
 117  
 118  export class Viewer {
 119    constructor(container, { capture = false } = {}) {
 120      // capture: keep the drawing buffer so the view can be saved or screenshotted (a little slower)
 121      THREE.Object3D.DEFAULT_UP.set(0, 0, 1);
 122      this.container = container;
 123      this.dropLines = true;
 124  
 125      this.renderer = new THREE.WebGLRenderer({ antialias: true, preserveDrawingBuffer: capture });
 126      this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
 127      this.renderer.shadowMap.enabled = true;
 128      this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
 129      container.prepend(this.renderer.domElement);
 130  
 131      this.labels = new CSS2DRenderer();
 132      Object.assign(this.labels.domElement.style, { position: 'absolute', top: '0', left: '0', pointerEvents: 'none', zIndex: '1' });
 133      container.prepend(this.labels.domElement);
 134  
 135      this.scene = new THREE.Scene();
 136      this.scene.background = new THREE.Color(0x0e1014);
 137      this.scene.fog = new THREE.Fog(0x0e1014, 9, 24);  // fade the floor out instead of showing its edge
 138      this.camera = new THREE.PerspectiveCamera(42, 1, 0.01, 200);
 139  
 140      this.controls = new OrbitControls(this.camera, this.renderer.domElement);
 141      this.controls.enableDamping = true;
 142      this.controls.dampingFactor = 0.08;
 143      this.controls.minDistance = 0.3;
 144      this.controls.maxDistance = 25;
 145      // ZED2 Viewpoint: a peek window in the corner that shows what a camera on RED sees (its own small
 146      // renderer, made when first switched on)
 147      this.redCam = { on: false, lens: 'left', renderer: null, note: '', onClose: null, onLens: null,
 148                      cameras: { left: new THREE.PerspectiveCamera(ZED_VFOV, 16 / 9, 0.01, 200),
 149                                 right: new THREE.PerspectiveCamera(ZED_VFOV, 16 / 9, 0.01, 200) } };
 150      this._buildRedCamWindow();
 151  
 152      this._buildLights();
 153      this._buildTable();
 154      this._buildAxes();
 155      this.layers = { live: true, sim: false, setup: false };
 156      this.bodyAxes = { on: true, groups: new Set() };   // every platform's and marker's axes (Axes switch)
 157      // Use CAD: the platforms (and replayed arms) drawn from the lab's CAD instead of cubes and lines
 158      this.cad = { on: false, lib: null, loading: null, crafts: new Set(), arms: new Set() };
 159      // Spacecraft Designer: what is mounted on each platform (drawn with the CAD), and the page's
 160      // close-up of one platform with the 16 docking-cone places to click
 161      this.design = null;
 162      this.designer = { active: false, group: null, slots: [], platform: null, onSlot: null, selected: null,
 163                        savedCam: null, hover: null };
 164      this.crafts = {};
 165      for (const name of Object.keys(CRAFT_COLORS)) this.crafts[name] = this._buildCraft(name);
 166      this._buildLeds();
 167      this._buildLivePaths();
 168      this._buildSimLayer();
 169      this._buildSetupLayer();
 170      this._buildGoalLayer();
 171      this._bindDrag();
 172  
 173      new ResizeObserver(() => this._resize()).observe(container);
 174      this._resize();
 175      this.setView('3d');
 176      this.renderer.setAnimationLoop(() => {
 177        this.controls.update();
 178        this.renderer.render(this.scene, this.camera);
 179        this.labels.render(this.scene, this.camera);
 180        if (this.redCam.on) this._renderRedCam();
 181      });
 182    }
 183  
 184    _resize() {
 185      const w = this.container.clientWidth, h = this.container.clientHeight;
 186      if (!w || !h) return;
 187      this.renderer.setSize(w, h);
 188      this.labels.setSize(w, h);
 189      this.camera.aspect = w / h;
 190      this.camera.updateProjectionMatrix();
 191    }
 192  
 193    _buildLights() {
 194      // The lab's ceiling lights: one straight above the table casts the shadows (so a platform's
 195      // shadow falls right under it, and the table shades the floor), with soft fill from the room
 196      this.scene.add(new THREE.HemisphereLight(0xdfe7ff, 0x16181c, 1.1));
 197      const ceiling = new THREE.DirectionalLight(0xffffff, 2.3);
 198      ceiling.position.set(TABLE.x / 2, TABLE.y / 2, 6);
 199      ceiling.target.position.set(TABLE.x / 2, TABLE.y / 2, 0);
 200      ceiling.castShadow = true;
 201      ceiling.shadow.mapSize.set(2048, 2048);
 202      Object.assign(ceiling.shadow.camera, { left: -2.4, right: 2.4, top: 1.9, bottom: -1.9, near: 2, far: 8 });
 203      ceiling.shadow.bias = -0.0003;
 204      ceiling.shadow.normalBias = 0.002;
 205      this.scene.add(ceiling, ceiling.target);
 206      // two more panels off to the sides, without shadows, so the platforms' sides aren't flat
 207      for (const [dx, dy, i] of [[-1.4, -1.0, 0.55], [1.4, 1.0, 0.45]]) {
 208        const fill = new THREE.DirectionalLight(0xf2f5ff, i);
 209        fill.position.set(TABLE.x / 2 + dx, TABLE.y / 2 + dy, 5);
 210        fill.target.position.set(TABLE.x / 2, TABLE.y / 2, 0);
 211        this.scene.add(fill, fill.target);
 212      }
 213    }
 214  
 215    _buildTable() {
 216      const top = new THREE.MeshStandardMaterial({ map: graniteTexture(this.renderer), roughness: 0.5, metalness: 0.02 });
 217      const side = new THREE.MeshStandardMaterial({ color: 0x2b2d32, roughness: 0.55, metalness: 0.05 });
 218      // BoxGeometry faces: +x, -x, +y, -y, +z (top), -z
 219      const slab = new THREE.Mesh(new THREE.BoxGeometry(TABLE.x, TABLE.y, TABLE.thickness), [side, side, side, side, top, side]);
 220      slab.position.set(TABLE.x / 2, TABLE.y / 2, -TABLE.thickness / 2);
 221      slab.receiveShadow = true;
 222      slab.castShadow = true;   // so the floor under the table is in its shade
 223      this.scene.add(slab);
 224  
 225      // The steel stand: a frame under the slab, a leg under each corner, stretchers near the floor, and
 226      // a bracket wrapping each corner of the frame, all painted as the barrier (its very material,
 227      // reflections included, once the barrier has loaded)
 228      const legH = 0.55, floorZ = -TABLE.thickness - legH;
 229      const steel = new THREE.MeshStandardMaterial({ color: 0x15171a, metalness: 0.1, roughness: 0.2 });
 230      const stand = [];
 231      const add = (w, d, h, x, y, z, mat = steel) => {
 232        const m = new THREE.Mesh(new THREE.BoxGeometry(w, d, h), mat);
 233        m.position.set(x, y, z);
 234        m.castShadow = m.receiveShadow = true;
 235        this.scene.add(m);
 236        stand.push(m);
 237        return m;
 238      };
 239      const leg = 0.1, apron = 0.1, under = -TABLE.thickness;
 240      const corners = [[0, 0], [TABLE.x, 0], [0, TABLE.y], [TABLE.x, TABLE.y]];
 241      for (const [cx, cy] of corners) {
 242        const sx = cx ? -1 : 1, sy = cy ? -1 : 1;   // toward the table's inside
 243        add(leg, leg, legH, cx + sx * leg / 2, cy + sy * leg / 2, under - legH / 2);
 244        // the corner bracket: two plates round the outside of the corner, over the frame and the leg's top
 245        const t = 0.006, reach = 0.2, bh = 0.18;
 246        add(reach, t, bh, cx + sx * reach / 2, cy - sy * t / 2, under - bh / 2);
 247        add(t, reach, bh, cx - sx * t / 2, cy + sy * reach / 2, under - bh / 2);
 248      }
 249      // the frame right under the slab, along all four edges
 250      add(TABLE.x - 2 * leg, leg * 0.8, apron, TABLE.x / 2, leg * 0.4, under - apron / 2);
 251      add(TABLE.x - 2 * leg, leg * 0.8, apron, TABLE.x / 2, TABLE.y - leg * 0.4, under - apron / 2);
 252      add(leg * 0.8, TABLE.y - 2 * leg, apron, leg * 0.4, TABLE.y / 2, under - apron / 2);
 253      add(leg * 0.8, TABLE.y - 2 * leg, apron, TABLE.x - leg * 0.4, TABLE.y / 2, under - apron / 2);
 254      // stretchers near the floor
 255      const low = floorZ + 0.14;
 256      add(TABLE.x - 2 * leg, 0.06, 0.06, TABLE.x / 2, leg / 2, low);
 257      add(TABLE.x - 2 * leg, 0.06, 0.06, TABLE.x / 2, TABLE.y - leg / 2, low);
 258      add(0.06, TABLE.y - 2 * leg, 0.06, leg / 2, TABLE.y / 2, low);
 259      add(0.06, TABLE.y - 2 * leg, 0.06, TABLE.x - leg / 2, TABLE.y / 2, low);
 260  
 261      // The barrier round the table's edges (rails, corner brackets, hinges; from its CAD, loaded
 262      // in the background)
 263      loadCad(TABLE_URL).then(lib => {
 264        const g = lib._group('BARRIER', 'table', false);
 265        g.traverse(o => { if (o.isMesh) { o.castShadow = o.receiveShadow = true; } });
 266        const paint = lib.material('paint', 'table', false);
 267        for (const m of stand) m.material = paint;
 268        steel.dispose();
 269        this.barrier = g;
 270        this.scene.add(g);
 271      }).catch(e => console.warn('Table barrier not loaded:', e.message));
 272      const floor = new THREE.Mesh(new THREE.CircleGeometry(40, 64), new THREE.MeshStandardMaterial({ color: 0x111317, roughness: 0.95 }));
 273      floor.position.set(TABLE.x / 2, TABLE.y / 2, floorZ);
 274      floor.receiveShadow = true;
 275      this.scene.add(floor);
 276      const floorGrid = new THREE.GridHelper(40, 80, 0x20242c, 0x1a1d23);
 277      floorGrid.rotation.x = Math.PI / 2;
 278      floorGrid.position.set(TABLE.x / 2, TABLE.y / 2, floorZ + 0.001);
 279      this.scene.add(floorGrid);
 280  
 281      // 0.5 m grid and outline on the table top
 282      const pts = [];
 283      for (let x = 0.5; x < TABLE.x - 1e-6; x += 0.5) pts.push(x, 0, 0.0008, x, TABLE.y, 0.0008);
 284      for (let y = 0.5; y < TABLE.y - 1e-6; y += 0.5) pts.push(0, y, 0.0008, TABLE.x, y, 0.0008);
 285      const gridGeo = new THREE.BufferGeometry();
 286      gridGeo.setAttribute('position', new THREE.Float32BufferAttribute(pts, 3));
 287      this.scene.add(new THREE.LineSegments(gridGeo, new THREE.LineBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.09 })));
 288      const outline = new THREE.LineLoop(new THREE.BufferGeometry().setFromPoints([
 289        new THREE.Vector3(0, 0, 0.001), new THREE.Vector3(TABLE.x, 0, 0.001),
 290        new THREE.Vector3(TABLE.x, TABLE.y, 0.001), new THREE.Vector3(0, TABLE.y, 0.001)]),
 291        new THREE.LineBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.3 }));
 292      this.scene.add(outline);
 293  
 294      // Distance ticks along the two edges that meet at the origin
 295      for (let x = 0.5; x <= TABLE.x + 1e-6; x += 0.5) {
 296        const t = label(x >= TABLE.x - 1e-6 ? `${TABLE.x} m` : x.toFixed(1), 'tick-label');
 297        t.position.set(x, -0.09, 0);
 298        this.scene.add(t);
 299      }
 300      for (const y of [0.5, 1.0, 1.5, 2.0, TABLE.y]) {
 301        const t = label(y === TABLE.y ? `${TABLE.y} m` : y.toFixed(1), 'tick-label');
 302        t.position.set(-0.11, y, 0);
 303        this.scene.add(t);
 304      }
 305    }
 306  
 307    _buildAxes() {
 308      // a little above the table, so the arrows clear the barrier along its edges (6.5 mm high)
 309      const o = new THREE.Vector3(0, 0, 0.015);
 310      const axes = [
 311        [new THREE.Vector3(1, 0, 0), 0xff6b6b, '+X'],
 312        [new THREE.Vector3(0, 1, 0), 0x51cf66, '+Y'],
 313        [new THREE.Vector3(0, 0, 1), 0x4dabf7, '+Z'],
 314      ];
 315      for (const [dir, color, text] of axes) {
 316        this.scene.add(new THREE.ArrowHelper(dir, o, 0.45, color, 0.08, 0.045));
 317        const l = label(text, 'axis-label');
 318        l.element.style.color = '#' + color.toString(16).padStart(6, '0');
 319        l.position.copy(dir.clone().multiplyScalar(0.55)).add(o);
 320        this.scene.add(l);
 321      }
 322      const origin = label('(0, 0, 0)', 'origin-label');
 323      origin.position.set(0, 0, 0);
 324      origin.center.set(1.08, -0.15);  // sit just outside the corner
 325      this.scene.add(origin);
 326    }
 327  
 328    _buildCraft(name, ghost = false, tagText = '') {
 329      // ghost: 'sim' for the simulation's see-through copy, 'overlay' for a loaded result's outline
 330      const color = CRAFT_COLORS[name];
 331      const group = new THREE.Group();
 332      const geo = new THREE.BoxGeometry(CRAFT_SIZE, CRAFT_SIZE, CRAFT_SIZE);
 333      const opacity = ghost === 'overlay' ? 0.1 : ghost ? 0.45 : 1;
 334      const metal = name !== 'BLUE';   // the Chaser silver, the Target gold
 335      const box = new THREE.Mesh(geo, new THREE.MeshStandardMaterial({
 336        color, roughness: metal ? 0.35 : 0.5, metalness: metal ? 0.45 : 0.15,
 337        transparent: Boolean(ghost), opacity, depthWrite: !ghost }));
 338      box.position.z = CRAFT_SIZE / 2;
 339      box.castShadow = !ghost;
 340      box.receiveShadow = true;
 341      const edgeMat = ghost === 'overlay'
 342        ? new THREE.LineDashedMaterial({ color: LINE_COLORS[name], dashSize: 0.03, gapSize: 0.02 })
 343        : new THREE.LineBasicMaterial({
 344          color: ghost ? LINE_COLORS[name] : 0xffffff, transparent: true, opacity: ghost ? 0.95 : 0.4 });
 345      const edges = new THREE.LineSegments(new THREE.EdgesGeometry(geo), edgeMat);
 346      if (ghost === 'overlay') edges.computeLineDistances();
 347      edges.position.z = CRAFT_SIZE / 2;
 348  
 349      // Body axes from the centre of the cube (x red, y green, z blue), so the attitude is visible
 350      const axes = this._bodyAxes(ghost === 'overlay' ? 0.7 : 1);
 351      axes.position.z = CRAFT_SIZE / 2;
 352  
 353      const shown = platformName(name);
 354      const text = ghost === 'overlay' ? `${shown} · ${tagText}` : ghost ? `${shown} · Sim` : shown;
 355      const tag = ghost === 'overlay' ? label(text, 'craft-label', null, LINE_COLORS[name])
 356                                      : label(text, 'craft-label', color, TEXT_COLORS[name]);
 357      if (ghost) tag.element.classList.add(ghost === 'overlay' ? 'overlay' : 'ghost');
 358      tag.position.z = CRAFT_SIZE / 2 + BODY_AXIS + (ghost === 'overlay' ? 0.2 : 0.12);   // above the +Z label
 359      tag.center.set(0.5, 1);
 360  
 361      group.add(box, edges, axes, tag);
 362      // Hidden while the ZED2 Viewpoint looks out from this platform (they would sit in front of the lens)
 363      Object.assign(group.userData, { craft: name, ghost, box, edges, cad: null, ownParts: [axes, tag] });
 364      if (name === 'RED') {
 365        const cam = this._buildRedCameraModel(opacity);
 366        group.add(cam);
 367        group.userData.ownParts.push(cam);
 368        group.userData.housing = cam;
 369      }
 370      this.cad.crafts.add(group);
 371      this._applyCadCraft(group);
 372      group.visible = false;
 373      this.scene.add(group);
 374      return group;
 375    }
 376  
 377    _buildRedCameraModel(opacity = 1) {
 378      // The ZED2 on RED's front face as a box of its size (the cube view; the CAD has the real one),
 379      // with its two lenses, where the ZED2 Viewpoint looks from
 380      const group = new THREE.Group();
 381      const see = opacity < 1 ? { transparent: true, opacity: Math.max(opacity, 0.35), depthWrite: false } : {};
 382      const [front, ly, lz] = ZED_LENSES.right;
 383      const proud = front - CRAFT_SIZE / 2;   // the part ahead of the cube's face
 384      const housing = new THREE.Mesh(new THREE.BoxGeometry(proud, ZED_BOX[1], ZED_BOX[2]),
 385        new THREE.MeshStandardMaterial({ color: 0x2a2d32, roughness: 0.45, metalness: 0.4, ...see }));
 386      housing.position.set(CRAFT_SIZE / 2 + proud / 2, (ZED_LENSES.right[1] + ZED_LENSES.left[1]) / 2, lz);
 387      housing.castShadow = opacity === 1;
 388      group.add(housing);
 389      for (const [, y] of [ZED_LENSES.right, ZED_LENSES.left]) {
 390        const lens = new THREE.Mesh(new THREE.CylinderGeometry(0.008, 0.008, 0.002, 24),
 391          new THREE.MeshStandardMaterial({ color: 0x0b0d10, roughness: 0.05, metalness: 0.2, emissive: 0x0b2a44, ...see }));
 392        lens.rotation.z = -Math.PI / 2;   // cylinder axis (y) along +x
 393        lens.position.set(front + 0.001, y, lz);
 394        group.add(lens);
 395      }
 396      return group;
 397    }
 398  
 399    _bodyAxes(opacity = 1) {
 400      // Three arrows from the origin, long enough to stick out of the cube (whose half-size is
 401      // CRAFT_SIZE / 2): +x red, +y green, +z blue, with +X/+Y/+Z beyond the tips, as the table's
 402      // own axes (the labels follow the Labels switch)
 403      const group = new THREE.Group();
 404      group.visible = this.bodyAxes.on;
 405      this.bodyAxes.groups.add(group);
 406      for (const [dir, color, text] of [[[1, 0, 0], 0xff6b6b, '+X'], [[0, 1, 0], 0x51cf66, '+Y'], [[0, 0, 1], 0x4dabf7, '+Z']]) {
 407        const d = new THREE.Vector3(...dir);
 408        const arrow = new THREE.ArrowHelper(d, new THREE.Vector3(0, 0, 0), BODY_AXIS, color, 0.08, 0.05);
 409        for (const m of [arrow.line.material, arrow.cone.material]) {
 410          m.transparent = opacity < 1;
 411          m.opacity = opacity;
 412        }
 413        const l = label(text, 'axis-label body-axis-label');
 414        l.element.style.color = '#' + color.toString(16).padStart(6, '0');
 415        l.element.style.opacity = opacity;
 416        l.position.copy(d.multiplyScalar(BODY_AXIS + 0.055));
 417        group.add(arrow, l);
 418      }
 419      return group;
 420    }
 421  
 422    _buildLeds() {
 423      const core = new THREE.SphereGeometry(0.012, 20, 14);
 424      const halo = new THREE.SphereGeometry(0.03, 20, 14);
 425      const drop = new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(0, 0, 0), new THREE.Vector3(0, 0, -1)]);
 426      this.leds = [];
 427      for (let k = 0; k < NUM_LED_SLOTS; k++) {
 428        const coreMat = new THREE.MeshBasicMaterial({ color: LED_COLORS.none });
 429        const haloMat = new THREE.MeshBasicMaterial({ color: LED_COLORS.none, transparent: true, opacity: 0.25,
 430                                                      blending: THREE.AdditiveBlending, depthWrite: false });
 431        const lineMat = new THREE.LineBasicMaterial({ color: LED_COLORS.none, transparent: true, opacity: 0.35 });
 432        const group = new THREE.Group();
 433        const line = new THREE.Line(drop, lineMat);  // scaled to reach the table top
 434        const tag = label(String(k), 'led-label');
 435        tag.position.z = 0.028;
 436        tag.center.set(0.5, 1);
 437        group.add(new THREE.Mesh(core, coreMat), new THREE.Mesh(halo, haloMat), line, tag);
 438        group.visible = false;
 439        this.scene.add(group);
 440        this.leds.push({ group, line, mats: [coreMat, haloMat, lineMat], tag });
 441      }
 442    }
 443  
 444    setLedOwner(owner) {
 445      this.leds.forEach((led, k) => {
 446        const color = LED_COLORS[owner[k]] || LED_COLORS.none;
 447        for (const m of led.mats) m.color.set(color);
 448        led.tag.element.style.color = color;
 449      });
 450    }
 451  
 452    update(frame) {
 453      this.lastFrame = frame;
 454      for (const [name, c] of Object.entries(frame.crafts || {})) {
 455        const g = this.crafts[name];
 456        if (!g) continue;
 457        g.visible = Boolean(this.layers.live && c.tracked && c.x != null && c.y != null);
 458        if (g.visible) {
 459          g.position.set(c.x, c.y, 0);
 460          g.rotation.z = c.att || 0;
 461          this._extendLivePath(name, c.x, c.y);
 462        }
 463      }
 464      this._updateGoalLine();
 465      const seen = new Set();
 466      for (const [k, x, y, z] of this.layers.live ? frame.leds || [] : []) {
 467        const led = this.leds[k];
 468        if (!led) continue;
 469        seen.add(k);
 470        led.group.visible = true;
 471        led.group.position.set(x, y, z);
 472        led.line.scale.z = Math.max(z, 1e-4);
 473        led.line.visible = this.dropLines && z > 0;
 474      }
 475      this.leds.forEach((led, k) => { if (!seen.has(k)) led.group.visible = false; });
 476    }
 477  
 478    // ---------------------------------------------------------------- layers
 479  
 480    setLayers(layers) {
 481      Object.assign(this.layers, layers);
 482      if (this.lastFrame) this.update(this.lastFrame);
 483      else for (const g of Object.values(this.crafts)) g.visible = false;
 484      this.sim.group.visible = this.layers.sim;
 485      this.setup.group.visible = this.layers.setup;
 486      for (const p of Object.values(this.live.paths)) p.line.visible = this.live.on && this.layers.live;
 487      if (this.cad.lib) this._applyCad();   // the simulation's CAD copy: solid alone, see-through over live
 488    }
 489  
 490    // ---------------------------------------------------------------- live paths (where each platform has been)
 491  
 492    _buildLivePaths() {
 493      // Each path is a wide line of up to max segments, written in place as the platform moves
 494      this.live = { paths: {}, on: false, max: 20000 };
 495      for (const name of Object.keys(CRAFT_COLORS)) {
 496        const geo = new LineSegmentsGeometry();
 497        geo.setPositions(new Float32Array(this.live.max * 6));
 498        geo.instanceCount = 0;
 499        const line = new LineSegments2(geo, new LineMaterial({
 500          color: new THREE.Color(LINE_COLORS[name]), linewidth: PATH_WIDTH, transparent: true, opacity: 0.85 }));
 501        line.frustumCulled = false;
 502        line.visible = false;
 503        this.scene.add(line);
 504        this.live.paths[name] = { line, seg: geo.attributes.instanceStart.data, n: 0, last: null };
 505      }
 506    }
 507  
 508    _extendLivePath(name, x, y) {
 509      const p = this.live.paths[name];
 510      if (!p || !this.live.on) return;
 511      if (!p.last) { p.last = [x, y]; return; }
 512      // only when it has moved (3 mm), so a platform at rest adds nothing
 513      const dx = x - p.last[0], dy = y - p.last[1];
 514      if (dx * dx + dy * dy < 9e-6) return;
 515      const arr = p.seg.array;
 516      if (p.n >= this.live.max) {   // full: keep the newer half
 517        const half = this.live.max / 2;
 518        arr.copyWithin(0, 6 * half, 6 * this.live.max);
 519        p.n = half;
 520      }
 521      arr.set([p.last[0], p.last[1], PATH_Z, x, y, PATH_Z], 6 * p.n);
 522      p.n++;
 523      p.last = [x, y];
 524      p.seg.needsUpdate = true;
 525      p.line.geometry.instanceCount = p.n;
 526    }
 527  
 528    setLivePaths(on) {
 529      this.live.on = on;
 530      for (const p of Object.values(this.live.paths)) p.line.visible = on && this.layers.live;
 531    }
 532  
 533    clearLivePaths() {
 534      for (const p of Object.values(this.live.paths)) { p.n = 0; p.last = null; p.line.geometry.instanceCount = 0; }
 535    }
 536  
 537    // ---------------------------------------------------------------- simulation layer
 538    // Replays of logged motion ("tracks"): the last simulation (the primary track: see-through
 539    // platforms, solid paths) and any loaded results laid over it (outlined platforms, dashed
 540    // paths, named after the dataset), each with its own time offset.
 541  
 542    _buildSimLayer() {
 543      this.sim = { group: new THREE.Group(), traces: true, overlays: new Map(), t: 0 };
 544      this.sim.group.visible = false;
 545      this.scene.add(this.sim.group);
 546      this.sim.primary = this._buildTrack({ ghost: 'sim' });
 547      this.sim.primary.shown = true;
 548    }
 549  
 550    _buildTrack({ ghost, label = '', dash = null }) {
 551      // ghost: 'sim' (the simulation's see-through copy) or 'overlay' (outline only)
 552      const track = { group: new THREE.Group(), crafts: {}, paths: {}, result: null, offset: 0, dash, shown: true, label };
 553      for (const name of Object.keys(CRAFT_COLORS)) {
 554        const g = this._buildCraft(name, ghost, label);
 555        this.scene.remove(g);
 556        track.group.add(g);
 557        track.crafts[name] = g;
 558      }
 559      // The arm on RED: links and joints, at mid-height of the body
 560      const armMat = dash ? new THREE.LineDashedMaterial({ color: LINE_COLORS.RED, dashSize: dash[0] / 2, gapSize: dash[1] / 2 })
 561                          : new THREE.LineBasicMaterial({ color: LINE_COLORS.RED });
 562      track.arm = this._buildArm(armMat, ghost);
 563      track.group.add(track.arm.group);
 564      this.sim.group.add(track.group);
 565      return track;
 566    }
 567  
 568    _disposeTrack(track) {
 569      this._setTrackResult(track, null);
 570      this.sim.group.remove(track.group);
 571      this.cad.arms.delete(track.arm);
 572      track.group.traverse(o => {
 573        this.bodyAxes.groups.delete(o);
 574        this.cad.crafts.delete(o);
 575        // (ArrowHelper's line and cone geometries are shared by every arrow, and the CAD models'
 576        // by every copy: keep them)
 577        if (o.geometry && o.parent?.type !== 'ArrowHelper' && !o.userData.sharedGeometry) o.geometry.dispose();
 578        if (o.element) o.element.remove();   // CSS2D labels
 579      });
 580    }
 581  
 582    _buildArm(material, ghost = null) {
 583      // ghost: the track's ('sim' or 'overlay'), for its CAD links; null for the initial-condition
 584      // arm, which stays a line
 585      const group = new THREE.Group();
 586      const geo = new THREE.BufferGeometry();
 587      geo.setAttribute('position', new THREE.Float32BufferAttribute(new Float32Array(12), 3));
 588      const line = new THREE.Line(geo, material);
 589      const joint = new THREE.SphereGeometry(0.018, 14, 10);
 590      const jointMat = new THREE.MeshStandardMaterial({ color: 0xf1f3f5, roughness: 0.4 });
 591      const joints = [0, 1, 2, 3].map(() => new THREE.Mesh(joint, jointMat));
 592      group.add(line, ...joints);
 593      group.visible = false;
 594      const arm = { group, line, joints, ghost, cad: null, pts: null };
 595      if (ghost) {
 596        this.cad.arms.add(arm);
 597        this._applyCadArm(arm);
 598      }
 599      return arm;
 600    }
 601  
 602    _setArm(arm, pts) {
 603      // pts: [[x, y] x 4] shoulder, elbow, wrist, end effector
 604      arm.pts = pts;
 605      const z = CRAFT_SIZE * 0.55;
 606      const pos = arm.line.geometry.attributes.position;
 607      pts.forEach(([x, y], k) => { pos.setXYZ(k, x, y, z); arm.joints[k].position.set(x, y, z); });
 608      pos.needsUpdate = true;
 609      arm.line.geometry.computeBoundingSphere();
 610      if (arm.line.material.isLineDashedMaterial) arm.line.computeLineDistances();
 611      // CAD links: each from its joint toward the next, stretched to the arm's own link length
 612      arm.cad?.links.forEach((link, k) => {
 613        const [x0, y0] = pts[k], dx = pts[k + 1][0] - x0, dy = pts[k + 1][1] - y0;
 614        link.position.set(x0, y0, 0);
 615        link.rotation.z = Math.atan2(dy, dx);
 616        link.scale.x = Math.hypot(dx, dy) / link.userData.length;
 617      });
 618    }
 619  
 620    // ---------------------------------------------------------------- CAD models (Use CAD)
 621  
 622    async setUseCad(on) {
 623      // Draws the platforms from the lab's CAD (loaded the first time); rejects if it can't be loaded
 624      this.cad.on = on;
 625      if (on && !this.cad.lib) {
 626        this.cad.loading ||= loadCad();
 627        try {
 628          this.cad.lib = await this.cad.loading;
 629        } catch (e) {
 630          this.cad.loading = null;
 631          this.cad.on = false;
 632          throw e;
 633        }
 634      }
 635      this._applyCad();
 636    }
 637  
 638    _applyCad() {
 639      for (const g of this.cad.crafts) this._applyCadCraft(g);
 640      for (const a of this.cad.arms) this._applyCadArm(a);
 641    }
 642  
 643    // ---------------------------------------------------------------- Spacecraft Designer
 644  
 645    _designOf(name) { return this.design ? this.design[name] || null : null; }
 646  
 647    setDesign(platforms) {
 648      // platforms: { RED: {dock, solar}, ... } (design.py); every CAD platform is rebuilt with it
 649      this.design = platforms || null;
 650      for (const g of this.cad.crafts) {
 651        const u = g.userData;
 652        if (u.cad) { g.remove(u.cad); u.cad = null; }
 653        this._applyCadCraft(g);
 654      }
 655      if (this.designer.active) this._buildDesigner();
 656    }
 657  
 658    async setDesigner(opts) {
 659      // Shows one platform's CAD model close up at the table's centre, with a marker on each of the
 660      // 6 docking-cone places (clicking one calls opts.onSlot({face, side})); null leaves it.
 661      // opts: { platform, onSlot, selected (the dock's place, or null when it has none),
 662      //         arm: { q: joint angles [rad], geometry: the settings' arm } for the Chaser's arm, or null }
 663      const d = this.designer;
 664      if (!opts) {
 665        if (!d.active) return;
 666        d.active = false;
 667        if (d.group) { this.scene.remove(d.group); d.group = null; }
 668        d.slots = [];
 669        this.goal.group.visible = true;
 670        if (d.savedCam) {
 671          this.camera.position.copy(d.savedCam.pos);
 672          this.controls.target.copy(d.savedCam.target);
 673          this.controls.update();
 674          d.savedCam = null;
 675        }
 676        return;
 677      }
 678      const first = !d.active, moved = d.platform !== opts.platform;
 679      if (first) d.savedCam = { pos: this.camera.position.clone(), target: this.controls.target.clone() };
 680      Object.assign(d, opts, { active: true });
 681      this.goal.group.visible = false;
 682      if (!this.cad.lib) {
 683        this.cad.loading ||= loadCad();
 684        try { this.cad.lib = await this.cad.loading; } catch (e) { this.cad.loading = null; throw e; }
 685        if (this.cad.on) this._applyCad();
 686      }
 687      if (!d.active) return;   // left while loading
 688      this._buildDesigner();
 689      if (first || moved) {
 690        // Look at the face with the docking cone (or the front), from a little to its side and above
 691        const face = (d.selected || { face: '+x' }).face;
 692        const az = { '+x': 0, '+y': Math.PI / 2, '-x': Math.PI, '-y': -Math.PI / 2 }[face] - 0.6;
 693        const c = new THREE.Vector3(TABLE.x / 2, TABLE.y / 2, 0.15), r = 1.05;
 694        this.controls.target.copy(c);
 695        this.camera.position.set(c.x + r * Math.cos(az) * 0.85, c.y + r * Math.sin(az) * 0.85, c.z + r * 0.55);
 696        this.controls.update();
 697      }
 698    }
 699  
 700    _buildDesigner() {
 701      const d = this.designer, lib = this.cad.lib;
 702      if (!lib || !d.active) return;
 703      if (d.group) this.scene.remove(d.group);
 704      const group = new THREE.Group();
 705      group.position.set(TABLE.x / 2, TABLE.y / 2, 0);
 706      const design = this._designOf(d.platform);
 707      group.add(lib.platform(d.platform, false, design));
 708      if (d.platform === 'RED') group.add(lib.zed(false));
 709      // The arm on its mount, at its initial pose: links and the cone end effector, joint to joint
 710      if (d.platform === 'RED' && design && design.dock && design.dock.type === 'arm' && lib.models.ARM_LINK) {
 711        const q = (d.arm && d.arm.q) || [Math.PI / 2, Math.PI / 2, 0];
 712        const pts = armPoints(lib.info, design.dock, q, d.arm && d.arm.geometry);
 713        [lib.armLink(false), lib.armLink(false), lib.endEffector(false)].forEach((link, k) => {
 714          const [x0, y0] = pts[k], dx = pts[k + 1][0] - x0, dy = pts[k + 1][1] - y0;
 715          link.position.set(x0, y0, 0);
 716          link.rotation.z = Math.atan2(dy, dx);
 717          link.scale.x = Math.hypot(dx, dy) / link.userData.length;
 718          group.add(link);
 719        });
 720      }
 721      // The 6 places: a disc on the face at each (the arm coupler's in orange on the Chaser), the
 722      // chosen one hidden under its cone
 723      d.slots = [];
 724      const arm = lib.info.mount && lib.info.mount.armMount;
 725      for (const face of (lib.info.mount && lib.info.mount.faces) || ['+x', '+y', '-y']) {
 726        for (const side of ['left', 'right']) {
 727          {
 728            const slot = { face, side };
 729            const p = mountPose(lib.info, slot);
 730            const holder = new THREE.Group();
 731            holder.position.set(p.x, p.y, p.z);
 732            holder.rotation.z = p.rot;
 733            const isArm = d.platform === 'RED' && arm && arm.face === face && arm.side === side;
 734            const blocked = ((lib.info.blocked || {})[d.platform] || {})[`${face}|${side}`];
 735            const chosen = d.selected && d.selected.face === face && d.selected.side === side;
 736            const disc = new THREE.Mesh(new THREE.CircleGeometry(0.03, 40), new THREE.MeshBasicMaterial({
 737              color: blocked ? 0xff6b6b : isArm ? 0xff922b : 0x74c0fc, transparent: true, opacity: d.selected ? 0.7 : 0.4,
 738              side: THREE.DoubleSide, depthWrite: false }));
 739            disc.rotation.y = Math.PI / 2;
 740            disc.position.x = 0.012;   // just off the face (clear of the panel and the coupler's plate)
 741            disc.visible = !chosen;
 742            disc.userData.slot = slot;
 743            const ring = new THREE.Mesh(new THREE.RingGeometry(0.03, 0.036, 40), new THREE.MeshBasicMaterial({
 744              color: 0xffffff, transparent: true, opacity: 0.8, side: THREE.DoubleSide, depthWrite: false }));
 745            ring.rotation.y = Math.PI / 2;
 746            ring.position.x = 0.0121;
 747            ring.visible = !chosen;
 748            holder.add(disc, ring);
 749            group.add(holder);
 750            d.slots.push(disc);
 751          }
 752        }
 753      }
 754      d.group = group;
 755      this.scene.add(group);
 756    }
 757  
 758    _cadGhost(ghost) {
 759      // CAD models are solid, except copies that are not the run being looked at: the simulation
 760      // laid over a live view (see-through) and loaded results (faint)
 761      return ghost === 'sim' && !this.layers.live ? false : ghost;
 762    }
 763  
 764    _applyCadCraft(group) {
 765      // The cube (and the Chaser's camera housing) or the CAD model (and the ZED2). Copies that are
 766      // not the run keep the cube's outline over their model: the simulation laid over a live view a
 767      // solid one (see-through model), a loaded result a dashed one (faint model)
 768      const u = group.userData, on = Boolean(this.cad.on && this.cad.lib), ghost = this._cadGhost(u.ghost);
 769      if (on && !u.cad) {
 770        u.cad = this.cad.lib.platform(u.craft, ghost, this._designOf(u.craft));
 771        group.add(u.cad);
 772        if (u.housing) {
 773          u.zed = this.cad.lib.zed(ghost);
 774          group.add(u.zed);
 775          u.ownParts.push(u.zed);
 776        }
 777        u.cadGhost = ghost;
 778      } else if (u.cad && u.cadGhost !== ghost) {
 779        this.cad.lib.restyle(u.cad, ghost);
 780        if (u.zed) this.cad.lib.restyle(u.zed, ghost);
 781        u.cadGhost = ghost;
 782      }
 783      if (u.cad) u.cad.visible = on;
 784      if (u.zed) u.zed.visible = on;
 785      if (u.housing) u.housing.visible = !on;
 786      u.box.visible = !on;
 787      u.edges.visible = !on || Boolean(ghost);
 788    }
 789  
 790    _applyCadArm(arm) {
 791      // The line and joint balls, or the CAD links (shoulder to elbow, elbow to wrist, and the
 792      // end effector); a loaded result keeps its dashed line
 793      const on = Boolean(this.cad.on && this.cad.lib), ghost = this._cadGhost(arm.ghost);
 794      if (on && !arm.cad) {
 795        const lib = this.cad.lib;
 796        arm.cad = { group: new THREE.Group(), links: [lib.armLink(ghost), lib.armLink(ghost), lib.endEffector(ghost)], ghost };
 797        arm.cad.group.add(...arm.cad.links);
 798        arm.group.add(arm.cad.group);
 799        if (arm.pts) this._setArm(arm, arm.pts);
 800      } else if (arm.cad && arm.cad.ghost !== ghost) {
 801        for (const link of arm.cad.links) this.cad.lib.restyle(link, ghost);
 802        arm.cad.ghost = ghost;
 803      }
 804      if (arm.cad) arm.cad.group.visible = on;
 805      arm.line.visible = !on || arm.ghost === 'overlay';
 806      for (const j of arm.joints) j.visible = !on;
 807    }
 808  
 809    _setTrackResult(track, result) {
 810      // result: t, crafts.{NAME}.{x, y, att} (null where unknown), arm [4][2][N]; or null
 811      track.result = result;
 812      for (const p of Object.values(track.paths)) {
 813        track.group.remove(p.done);
 814        p.done.geometry.dispose(); p.done.material.dispose();
 815      }
 816      track.paths = {};
 817      for (const [name, g] of Object.entries(track.crafts)) g.visible = Boolean(result && result.crafts[name]);
 818      track.arm.group.visible = Boolean(result && result.arm);
 819      if (!result) return;
 820      for (const [name, c] of Object.entries(result.crafts)) {
 821        // The path skips samples without a position; upto[i] = points drawn up to sample i
 822        const pts = [], upto = new Int32Array(c.x.length);
 823        c.x.forEach((x, i) => {
 824          if (x != null && c.y[i] != null) pts.push(x, c.y[i], PATH_Z);
 825          upto[i] = pts.length / 3;
 826        });
 827        if (pts.length < 6) continue;
 828        const make = (opacity, width) => {
 829          const geo = new LineGeometry();
 830          geo.setPositions(pts);
 831          const line = new Line2(geo, new LineMaterial({
 832            color: new THREE.Color(LINE_COLORS[name]), linewidth: width, transparent: true, opacity,
 833            dashed: Boolean(track.dash), dashSize: track.dash ? track.dash[0] : 1, gapSize: track.dash ? track.dash[1] : 0 }));
 834          if (track.dash) line.computeLineDistances();
 835          line.frustumCulled = false;
 836          return line;
 837        };
 838        // Only the path travelled so far: it is traced behind the platform as the replay runs
 839        const done = make(0.95, PATH_WIDTH);
 840        done.visible = this.sim.traces;
 841        track.group.add(done);
 842        track.paths[name] = { done, upto };
 843      }
 844      this._setTrackTime(track, this.sim.t);
 845    }
 846  
 847    _setTrackTime(track, t) {
 848      const r = track.result;
 849      if (!r || !track.shown) return;
 850      t -= track.offset;
 851      const ts = r.t, n = ts.length;
 852      // index of the last sample at or before t (binary search), and how far to the next
 853      let lo = 0, hi = n - 1;
 854      if (t <= ts[0]) hi = 0;
 855      else if (t >= ts[n - 1]) lo = hi = n - 1;
 856      else {
 857        while (hi - lo > 1) { const mid = (lo + hi) >> 1; if (ts[mid] <= t) lo = mid; else hi = mid; }
 858      }
 859      const i = lo, j = Math.min(hi, n - 1);
 860      const f = j > i ? (t - ts[i]) / (ts[j] - ts[i]) : 0;
 861      const lerp = (a, k) => (a[j] == null ? a[i] : a[i] + (a[j] - a[i]) * k);
 862      for (const [name, c] of Object.entries(r.crafts)) {
 863        const g = track.crafts[name];
 864        g.visible = c.x[i] != null && c.y[i] != null;
 865        if (g.visible) {
 866          g.position.set(lerp(c.x, f), lerp(c.y, f), 0);
 867          g.rotation.z = lerp(c.att, f) || 0;
 868        }
 869        const p = track.paths[name];
 870        if (p) p.done.geometry.instanceCount = Math.max(0, p.upto[i] - 1);   // segments up to sample i
 871      }
 872      if (r.arm) {
 873        const k = f < 0.5 ? i : j;
 874        const pts = [0, 1, 2, 3].map(q => [r.arm[q][0][k], r.arm[q][1][k]]);
 875        track.arm.group.visible = pts.every(([x, y]) => x != null && y != null);
 876        if (track.arm.group.visible) this._setArm(track.arm, pts);
 877      }
 878    }
 879  
 880    setSimulation(result) {
 881      // The primary track: the last simulation (spot.sim.results), or null
 882      this._setTrackResult(this.sim.primary, result);
 883    }
 884  
 885    setPrimaryShown(on) {
 886      // Show or hide the last simulation without dropping it (the overlays stay)
 887      const p = this.sim.primary;
 888      p.shown = on;
 889      p.group.visible = on;
 890      if (on) this._setTrackTime(p, this.sim.t);
 891    }
 892  
 893    setOverlays(list) {
 894      // list: [{ id, result, label, offset }]: loaded results replayed with the simulation
 895      const keep = new Set(list.map(o => o.id));
 896      for (const [id, track] of this.sim.overlays) {
 897        if (!keep.has(id)) { this._disposeTrack(track); this.sim.overlays.delete(id); }
 898      }
 899      list.forEach((o, k) => {
 900        let track = this.sim.overlays.get(o.id);
 901        if (track && track.result !== o.result) { this._disposeTrack(track); track = null; }
 902        if (!track) {
 903          track = this._buildTrack({ ghost: 'overlay', label: o.label, dash: OVERLAY_DASHES[k % OVERLAY_DASHES.length] });
 904          this.sim.overlays.set(o.id, track);
 905          track.offset = o.offset || 0;
 906          this._setTrackResult(track, o.result);
 907        }
 908        track.offset = o.offset || 0;
 909        this._setTrackTime(track, this.sim.t);
 910      });
 911    }
 912  
 913    setTraces(on) {
 914      this.sim.traces = on;
 915      for (const track of [this.sim.primary, ...this.sim.overlays.values()]) {
 916        for (const p of Object.values(track.paths)) p.done.visible = on;
 917      }
 918    }
 919  
 920    setSimTime(t) {
 921      this.sim.t = t;
 922      this._setTrackTime(this.sim.primary, t);
 923      for (const track of this.sim.overlays.values()) this._setTrackTime(track, t);
 924    }
 925  
 926    // ---------------------------------------------------------------- setup layer (initial conditions)
 927  
 928    _makeMarker(name, text, opacity = 0.22) {
 929      // A pose on the table: a filled square with its outline and body axes, and a level name tag
 930      const h = CRAFT_SIZE / 2;
 931      const color = LINE_COLORS[name];
 932      const group = new THREE.Group();
 933      const fill = new THREE.Mesh(new THREE.PlaneGeometry(CRAFT_SIZE, CRAFT_SIZE),
 934        new THREE.MeshBasicMaterial({ color, transparent: true, opacity, depthWrite: false }));
 935      fill.position.z = 0.002;
 936      const outline = new THREE.LineLoop(new THREE.BufferGeometry().setFromPoints([
 937        new THREE.Vector3(-h, -h, 0.003), new THREE.Vector3(h, -h, 0.003),
 938        new THREE.Vector3(h, h, 0.003), new THREE.Vector3(-h, h, 0.003)]),
 939        new THREE.LineBasicMaterial({ color }));
 940      // Body axes from the centre of the marker, as on the platforms (x red, y green, z blue)
 941      const axes = this._bodyAxes(0.9);
 942      axes.position.z = 0.005;
 943      // The name stays level just off the marker's -y side (below it seen from the top, in front
 944      // of it in 3D), clear of the axes: the outer group moves, only the body inside rotates
 945      const tag = label(text, 'craft-label setup', CRAFT_COLORS[name], TEXT_COLORS[name]);
 946      tag.position.set(0, -h - 0.04, 0.01);
 947      tag.center.set(0.5, 0);
 948      const body = new THREE.Group();
 949      body.add(fill, outline, axes);
 950      group.add(body, tag);
 951      group.visible = false;
 952      return { group, body, fill, tag, att: 0 };
 953    }
 954  
 955    _buildSetupLayer() {
 956      this.setup = { group: new THREE.Group(), markers: {}, editable: false, onDrag: null };
 957      this.setup.group.visible = false;
 958      this.scene.add(this.setup.group);
 959      for (const name of Object.keys(CRAFT_COLORS)) {
 960        const m = this._makeMarker(name, platformName(name));
 961        this.setup.group.add(m.group);
 962        this.setup.markers[name] = m;
 963      }
 964      this.setup.arm = this._buildArm(new THREE.LineBasicMaterial({ color: LINE_COLORS.RED, transparent: true, opacity: 0.8 }));
 965      this.setup.group.add(this.setup.arm.group);
 966    }
 967  
 968    setSetup({ poses = {}, kind = '', arm = null, editable = false } = {}) {
 969      // poses: { NAME: [x, y, thetaDeg] } for the platforms to show; arm: 4 x [x, y] or null
 970      this.setup.editable = editable;
 971      for (const [name, m] of Object.entries(this.setup.markers)) {
 972        const p = poses[name];
 973        m.group.visible = Boolean(p);
 974        if (!p || (this.drag && this.drag.m === m)) continue;
 975        m.group.position.set(p[0], p[1], 0);
 976        m.att = p[2] * Math.PI / 180;
 977        m.body.rotation.z = m.att;
 978        m.tag.element.textContent = kind ? `${platformName(name)} · ${kind}` : platformName(name);
 979      }
 980      this.setup.arm.group.visible = Boolean(arm);
 981      if (arm) this._setArm(this.setup.arm, arm);
 982    }
 983  
 984    // ---------------------------------------------------------------- goal layer (Interactive Mode)
 985  
 986    _buildGoalLayer() {
 987      // Where Interactive Mode sends the live platforms: one marker per platform, dragged like the
 988      // initial conditions, each with a dashed line from its platform to it. A click on a live
 989      // platform calls onPick(name) while pickable.
 990      this.goal = { group: new THREE.Group(), markers: {}, lines: {}, editable: {}, pickable: false,
 991                    margin: CRAFT_SIZE / 2, onDrag: null, onPick: null };
 992      this.scene.add(this.goal.group);
 993      for (const name of Object.keys(CRAFT_COLORS)) {
 994        const m = this._makeMarker(name, `${platformName(name)} · Goal`, 0.34);
 995        m.tag.element.classList.add('goal');
 996        this.goal.group.add(m.group);
 997        this.goal.markers[name] = m;
 998        const geo = new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(), new THREE.Vector3()]);
 999        const line = new THREE.Line(geo, new THREE.LineDashedMaterial(
1000          { color: LINE_COLORS[name], dashSize: 0.035, gapSize: 0.03, transparent: true, opacity: 0.7 }));
1001        line.visible = false;
1002        line.frustumCulled = false;
1003        this.goal.group.add(line);
1004        this.goal.lines[name] = line;
1005      }
1006    }
1007  
1008    setGoals({ goals = {}, pickable = false, margin = null } = {}) {
1009      // goals: { NAME: { pose: [x, y, thetaDeg], editable } } for the platforms whose goal to show
1010      // (editable: it can be dragged); the others are hidden. pickable: a click on a live platform
1011      // calls goal.onPick(name).
1012      const t = this.goal;
1013      t.pickable = pickable;
1014      if (margin != null) t.margin = margin;
1015      t.editable = {};
1016      for (const [n, m] of Object.entries(t.markers)) {
1017        const g = goals[n];
1018        m.group.visible = Boolean(g && g.pose);
1019        if (!m.group.visible) continue;
1020        t.editable[n] = Boolean(g.editable);
1021        if (this.drag && this.drag.m === m) continue;
1022        m.group.position.set(g.pose[0], g.pose[1], 0);
1023        m.att = g.pose[2] * Math.PI / 180;
1024        m.body.rotation.z = m.att;
1025      }
1026      this._updateGoalLine();
1027    }
1028  
1029    _updateGoalLine() {
1030      const t = this.goal;
1031      for (const [n, line] of Object.entries(t.lines)) {
1032        const m = t.markers[n], g = this.crafts[n];
1033        line.visible = Boolean(m.group.visible && g && g.visible);
1034        if (!line.visible) continue;
1035        const pos = line.geometry.attributes.position;
1036        pos.setXYZ(0, g.position.x, g.position.y, 0.004);
1037        pos.setXYZ(1, m.group.position.x, m.group.position.y, 0.004);
1038        pos.needsUpdate = true;
1039        line.geometry.computeBoundingSphere();
1040        line.computeLineDistances();
1041      }
1042    }
1043  
1044    setThrust(name, duty) {
1045      // duty: the 8 duty cycles [%] being sent to the live platform NAME, drawn as plumes from its
1046      // nozzles (longer for a larger duty cycle); null: none
1047      const g = this.crafts[name];
1048      if (!g) return;
1049      let plumes = g.userData.plumes;
1050      if (!plumes && duty) {
1051        plumes = new THREE.Group();
1052        const geo = new THREE.ConeGeometry(0.022, 1, 18, 1, true);
1053        geo.translate(0, -0.5, 0);   // apex at the nozzle, opening along -y
1054        const mat = new THREE.MeshBasicMaterial({ color: 0xcfe8ff, transparent: true, opacity: 0.6,
1055                                                  blending: THREE.AdditiveBlending, depthWrite: false, side: THREE.DoubleSide });
1056        for (const [[x, y], [dx, dy]] of THRUSTER_NOZZLES) {
1057          const cone = new THREE.Mesh(geo, mat);
1058          cone.position.set(x, y, NOZZLE_Z);
1059          cone.rotation.z = Math.atan2(dx, -dy);   // its -y along the gas's direction
1060          cone.visible = false;
1061          plumes.add(cone);
1062        }
1063        g.add(plumes);
1064        g.userData.plumes = plumes;
1065      }
1066      if (!plumes) return;
1067      plumes.visible = Boolean(duty);
1068      if (duty) plumes.children.forEach((c, k) => {
1069        const d = Math.min(100, Math.max(0, duty[k] || 0));
1070        c.visible = d > 0.5;
1071        c.scale.set(1, 0.05 + 0.25 * d / 100, 1);
1072      });
1073    }
1074  
1075    _bindDrag() {
1076      // Drag a setup marker or Interactive Mode's goal across the table (Shift+drag rotates it).
1077      // Runs before OrbitControls (capture phase) so that grabbing a marker doesn't also orbit the
1078      // camera.
1079      const el = this.renderer.domElement;
1080      const ray = new THREE.Raycaster(), ndc = new THREE.Vector2();
1081      const plane = new THREE.Plane(new THREE.Vector3(0, 0, 1), 0);
1082      const aim = ev => {
1083        const r = el.getBoundingClientRect();
1084        ndc.set(((ev.clientX - r.left) / r.width) * 2 - 1, -((ev.clientY - r.top) / r.height) * 2 + 1);
1085        ray.setFromCamera(ndc, this.camera);
1086      };
1087      const onTable = () => ray.ray.intersectPlane(plane, new THREE.Vector3());
1088      const hitMarker = () => {
1089        // The draggable markers under the mouse: { kind: 'setup' or 'goal', name, m }
1090        const list = [];
1091        if (this.layers.setup && this.setup.editable) {
1092          for (const [name, m] of Object.entries(this.setup.markers)) if (m.group.visible) list.push({ kind: 'setup', name, m });
1093        }
1094        for (const [name, m] of Object.entries(this.goal.markers)) {
1095          if (this.goal.editable[name] && m.group.visible) list.push({ kind: 'goal', name, m });
1096        }
1097        const hit = list.length ? ray.intersectObjects(list.map(e => e.m.fill), false)[0] : null;
1098        return hit ? list.find(e => e.m.fill === hit.object) : null;
1099      };
1100      const report = done => {
1101        const { kind, name, m } = this.drag;
1102        const onDrag = kind === 'goal' ? this.goal.onDrag : this.setup.onDrag;
1103        onDrag?.(name, m.group.position.x, m.group.position.y, m.att * 180 / Math.PI, done);
1104      };
1105      let down = null;
1106      el.addEventListener('pointerdown', ev => {
1107        down = [ev.clientX, ev.clientY];
1108        if (ev.button !== 0) return;
1109        aim(ev);
1110        const e = hitMarker();
1111        const p = e && onTable();
1112        if (!p) return;
1113        ev.stopImmediatePropagation();
1114        const m = e.m;
1115        this.drag = { kind: e.kind, name: e.name, m, rotate: ev.shiftKey, offset: p.clone().sub(m.group.position),
1116                      att0: m.att, angle0: Math.atan2(p.y - m.group.position.y, p.x - m.group.position.x) };
1117        el.setPointerCapture(ev.pointerId);
1118        el.style.cursor = 'grabbing';
1119      }, { capture: true });
1120      const hitSlot = () => {
1121        const d = this.designer;
1122        if (!d.active || !d.slots.length) return null;
1123        const hit = ray.intersectObjects(d.slots.filter(s => s.visible), false)[0];
1124        return hit ? hit.object : null;
1125      };
1126      el.addEventListener('pointermove', ev => {
1127        aim(ev);
1128        if (this.designer.active) {
1129          const s = hitSlot(), d = this.designer;
1130          if (d.hover !== s) {
1131            if (d.hover) d.hover.scale.setScalar(1);
1132            if (s) s.scale.setScalar(1.25);
1133            d.hover = s;
1134          }
1135          el.style.cursor = s ? 'pointer' : '';
1136          return;
1137        }
1138        if (!this.drag) {
1139          el.style.cursor = hitMarker() ? 'grab' : '';
1140          return;
1141        }
1142        const p = onTable();
1143        if (!p) return;
1144        const m = this.drag.m;
1145        if (this.drag.rotate) {
1146          m.att = this.drag.att0 + Math.atan2(p.y - m.group.position.y, p.x - m.group.position.x) - this.drag.angle0;
1147          m.body.rotation.z = m.att;
1148        } else {
1149          // Goals stay as far inside the edges as the lab console keeps them
1150          const h = this.drag.kind === 'goal' ? this.goal.margin : CRAFT_SIZE / 2;
1151          m.group.position.x = Math.min(Math.max(p.x - this.drag.offset.x, h), TABLE.x - h);
1152          m.group.position.y = Math.min(Math.max(p.y - this.drag.offset.y, h), TABLE.y - h);
1153        }
1154        if (this.drag.kind === 'goal') this._updateGoalLine();
1155        report(false);
1156      });
1157      // A click (not a drag) on a live platform picks it for Interactive Mode
1158      el.addEventListener('click', ev => {
1159        if (!down || Math.hypot(ev.clientX - down[0], ev.clientY - down[1]) > 4) return;
1160        if (this.designer.active) {   // Spacecraft Designer: a click on one of the 16 places
1161          aim(ev);
1162          const s = hitSlot();
1163          if (s) this.designer.onSlot?.({ ...s.userData.slot });
1164          return;
1165        }
1166        if (!this.goal.pickable) return;
1167        aim(ev);
1168        const shown = Object.values(this.crafts).filter(g => g.visible);
1169        const hit = shown.length ? ray.intersectObjects(shown, true)[0] : null;
1170        for (let o = hit && hit.object; o; o = o.parent) {
1171          if (o.userData.craft && !o.userData.ghost) { this.goal.onPick?.(o.userData.craft); return; }
1172        }
1173      });
1174      const end = ev => {
1175        if (!this.drag) return;
1176        report(true);
1177        this.drag = null;
1178        el.style.cursor = '';
1179        if (el.hasPointerCapture(ev.pointerId)) el.releasePointerCapture(ev.pointerId);
1180      };
1181      el.addEventListener('pointerup', end);
1182      el.addEventListener('pointercancel', end);
1183    }
1184  
1185    // ---------------------------------------------------------------- display options
1186  
1187    setLedLabels(on) { this.container.classList.toggle('hide-led-labels', !on); }
1188    setNames(on) { this.container.classList.toggle('hide-names', !on); }
1189    setBodyAxes(on) {
1190      // The platforms' (and initial-condition markers') own axes; the table's frame stays
1191      this.bodyAxes.on = on;
1192      for (const g of this.bodyAxes.groups) g.visible = on;
1193    }
1194    setDropLines(on) {
1195      this.dropLines = on;
1196      for (const led of this.leds) led.line.visible = on && led.group.position.z > 0;
1197    }
1198  
1199    // The table slope calibration (calibration.js; null: none): its height map, jet coloured, laid on
1200    // the table top when shown (setTableSlopeShown)
1201    setTableSlope(cal) {
1202      const key = cal ? JSON.stringify([cal.height, cal.centre]) : null;
1203      if (key === this._slopeKey) return;
1204      this._slopeKey = key;
1205      if (this.slopeMesh) {
1206        this.scene.remove(this.slopeMesh);
1207        this.slopeMesh.material.map.dispose();
1208        this.slopeMesh.material.dispose();
1209        this.slopeMesh.geometry.dispose();
1210        this.slopeMesh = null;
1211      }
1212      if (!cal) return;
1213      const tex = new THREE.CanvasTexture(heightCanvas(cal));
1214      tex.colorSpace = THREE.SRGBColorSpace;
1215      const mat = new THREE.MeshBasicMaterial({ map: tex, transparent: true, opacity: 0.62, depthWrite: false });
1216      this.slopeMesh = new THREE.Mesh(new THREE.PlaneGeometry(TABLE.x, TABLE.y), mat);
1217      this.slopeMesh.position.set(TABLE.x / 2, TABLE.y / 2, 0.0005);   // under the grid lines (0.0008)
1218      this.slopeMesh.visible = Boolean(this.slopeShown);
1219      this.scene.add(this.slopeMesh);
1220    }
1221  
1222    setTableSlopeShown(on) {
1223      this.slopeShown = on;
1224      if (this.slopeMesh) this.slopeMesh.visible = on;
1225    }
1226  
1227    _fitDistance(w, h) {
1228      const v = THREE.MathUtils.degToRad(this.camera.fov);
1229      const hfov = 2 * Math.atan(Math.tan(v / 2) * this.camera.aspect);
1230      return Math.max((h / 2) / Math.tan(v / 2), (w / 2) / Math.tan(hfov / 2));
1231    }
1232  
1233    // ---------------------------------------------------------------- ZED2 Viewpoint (peek window)
1234  
1235    _buildRedCamWindow() {
1236      // A small framed window in the corner of the view: title (which RED it rides on), close button,
1237      // and the camera's picture, 16:9 like the ZED's HD720
1238      const win = document.createElement('div');
1239      win.className = 'cam-window';
1240      win.hidden = true;
1241      win.innerHTML = '<div class="cam-head"><span class="cam-title"></span>'
1242        + '<div class="segmented seg-sm cam-lens" role="group" aria-label="Lens">'
1243        + '<button type="button" data-lens="left" title="The left lens (the ZED SDK\'s reference image)">Left</button>'
1244        + '<button type="button" data-lens="right" title="The right lens">Right</button>'
1245        + '<button type="button" data-lens="both" title="Both lenses side by side, left then right">Both</button></div>'
1246        + '<button type="button" class="cam-close" title="Close the ZED2 Viewpoint" aria-label="Close">✕</button></div>'
1247        + '<div class="cam-frame"><div class="cam-missing">The Chaser is not on the table</div></div>';
1248      this.container.append(win);
1249      Object.assign(this.redCam, { win, title: win.querySelector('.cam-title'), frame: win.querySelector('.cam-frame'),
1250                                   missing: win.querySelector('.cam-missing') });
1251      win.querySelector('.cam-close').onclick = () => { this.setRedCam(false); this.redCam.onClose?.(); };
1252      win.querySelector('.cam-lens').onclick = ev => {
1253        const b = ev.target.closest('[data-lens]');
1254        if (b) { this.setRedCamLens(b.dataset.lens); this.redCam.onLens?.(b.dataset.lens); }
1255      };
1256      this.setRedCamLens('left');
1257      new ResizeObserver(() => this._resizeRedCam()).observe(this.redCam.frame);
1258    }
1259  
1260    _resizeRedCam() {
1261      const r = this.redCam.renderer, w = this.redCam.frame.clientWidth, h = this.redCam.frame.clientHeight;
1262      if (!r || !w || !h) return;
1263      r.setSize(w, h);
1264      // each image keeps the ZED2's 16:9 and vertical field of view (two side by side for both lenses)
1265      const aspect = (this.redCam.lens === 'both' ? w / 2 : w) / h;
1266      for (const c of Object.values(this.redCam.cameras)) { c.aspect = aspect; c.updateProjectionMatrix(); }
1267    }
1268  
1269    setRedCamLens(lens) {
1270      // 'left', 'right' or 'both' (side by side)
1271      const rc = this.redCam;
1272      rc.lens = ['left', 'right', 'both'].includes(lens) ? lens : 'left';
1273      rc.win.classList.toggle('both', rc.lens === 'both');
1274      for (const b of rc.win.querySelectorAll('[data-lens]')) b.classList.toggle('active', b.dataset.lens === rc.lens);
1275      rc.note = '';
1276      this._resizeRedCam();
1277    }
1278  
1279    setRedCam(on) {
1280      // Show or hide the ZED2 Viewpoint window
1281      const rc = this.redCam;
1282      rc.on = on;
1283      rc.win.hidden = !on;
1284      if (on && !rc.renderer) {
1285        rc.renderer = new THREE.WebGLRenderer({ antialias: true, preserveDrawingBuffer: this.renderer.getContextAttributes().preserveDrawingBuffer });
1286        rc.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
1287        rc.renderer.shadowMap.enabled = true;
1288        rc.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
1289        rc.frame.prepend(rc.renderer.domElement);
1290        this._resizeRedCam();
1291      }
1292    }
1293  
1294    _redCameraCarrier() {
1295      // The RED that is shown, in order: live (PhaseSpace), the last simulation, a laid-over result,
1296      // then the initial-condition marker. Returns the object whose frame is RED's body frame.
1297      const shown = o => { for (; o; o = o.parent) if (!o.visible) return false; return true; };
1298      const candidates = [[this.crafts.RED, 'Live'], [this.sim.primary.crafts.RED, 'Simulation'],
1299                          ...[...this.sim.overlays.values()].map(t => [t.crafts.RED, t.label || 'Results']),
1300                          [this.setup.markers.RED.body, this.setup.markers.RED.tag.element.textContent.split(' · ')[1] || 'Initial Conditions']];
1301      for (const [obj, source] of candidates) if (shown(obj)) return { obj, source };
1302      return null;
1303    }
1304  
1305    _renderRedCam() {
1306      // Places the camera at RED's mount point and draws its picture into the window. RED's own
1307      // axes, tag and camera housing are hidden meanwhile (they would sit in front of the lens).
1308      const rc = this.redCam;
1309      if (!rc.renderer || !rc.frame.clientWidth) return;
1310      const carrier = this._redCameraCarrier();
1311      const which = { left: 'Left Lens', right: 'Right Lens', both: 'Both Lenses' }[rc.lens];
1312      const note = carrier ? `ZED2 Viewpoint · ${platformName('RED')} ${carrier.source} · ${which}` : 'ZED2 Viewpoint';
1313      if (note !== rc.note) {
1314        rc.note = note;
1315        rc.title.textContent = note;
1316        rc.missing.hidden = Boolean(carrier);
1317        rc.renderer.domElement.style.visibility = carrier ? '' : 'hidden';
1318      }
1319      if (!carrier) return;
1320      const { obj } = carrier;
1321      obj.updateWorldMatrix(true, false);
1322      const lenses = (this.cad.lib && this.cad.lib.info.zed && this.cad.lib.info.zed.lenses) || ZED_LENSES;
1323      for (const side of ['left', 'right']) {
1324        // at the lens's centre (just ahead of the glass), looking along RED's +x, z up
1325        const [x, y, z] = lenses[side];
1326        const cam = rc.cameras[side];
1327        cam.position.copy(obj.localToWorld(new THREE.Vector3(x + 0.001, y, z)));
1328        cam.up.set(0, 0, 1).applyQuaternion(obj.getWorldQuaternion(new THREE.Quaternion()));
1329        cam.lookAt(obj.localToWorld(new THREE.Vector3(x + 1, y, z)));
1330      }
1331      const hide = (obj.userData.ownParts || []).filter(o => o.visible);
1332      for (const o of hide) o.visible = false;
1333      const r = rc.renderer, size = r.getSize(new THREE.Vector2());
1334      if (rc.lens === 'both') {
1335        r.setScissorTest(true);
1336        for (const [k, side] of [[0, 'left'], [1, 'right']]) {
1337          r.setViewport(k * size.x / 2, 0, size.x / 2, size.y);
1338          r.setScissor(k * size.x / 2, 0, size.x / 2, size.y);
1339          r.render(this.scene, rc.cameras[side]);
1340        }
1341        r.setScissorTest(false);
1342        r.setViewport(0, 0, size.x, size.y);
1343      } else {
1344        r.render(this.scene, rc.cameras[rc.lens]);
1345      }
1346      for (const o of hide) o.visible = true;
1347    }
1348  
1349    setView(name) {
1350      const target = new THREE.Vector3(TABLE.x / 2, TABLE.y / 2, 0);
1351      if (name === 'top') {
1352        // Straight down; with +z up and a right-handed frame, +x points right and +y up the screen
1353        this.camera.position.set(target.x, target.y - 0.001, this._fitDistance(TABLE.x + 0.7, TABLE.y + 0.7));
1354      } else if (name === 'side') {
1355        target.z = 0.15;
1356        this.camera.position.set(target.x, target.y - TABLE.y / 2 - this._fitDistance(TABLE.x + 0.8, 1.4), 0.4);
1357      } else {
1358        // From the y = 0 side, raised and a little toward the origin, so the origin corner is in view
1359        target.set(TABLE.x / 2 - 0.1, TABLE.y / 2 - 0.15, -0.15);
1360        const dir = new THREE.Vector3(-0.3, -0.8, 0.62).normalize();
1361        this.camera.position.copy(target).addScaledVector(dir, this._fitDistance(TABLE.x + 0.6, TABLE.y + 0.6) * 1.4);
1362      }
1363      this.controls.target.copy(target);
1364      this.controls.update();
1365    }
1366  }
```

</details>
<!-- /listing -->
