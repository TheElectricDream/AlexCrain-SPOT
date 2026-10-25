// 3D view of the granite table, the spacecraft and the LEDs (three.js, z axis up, metres).
// Table frame: (0, 0, 0) is one corner of the table top, +x runs along the 3.51155 m side,
// +y along the 2.41935 m side, +z up. PhaseSpace data arrives in this frame (converted mm -> m
// by app.py), and so do simulation results (the diagrams' Px, Py, Rz).
//
// One viewer is shared by all pages (app.js moves it into the page shown), with three layers:
//   live    the platforms and LEDs PhaseSpace sees
//   sim     a simulation replayed (see-through platforms, their paths, the arm), and loaded
//           results laid over it (outlined platforms, dashed paths); a run's arm only while
//           the Designer's Chaser carries it, or where the person chose to see it anyway
//   setup   the initial-condition poses, which can be dragged (Shift+drag rotates)
// and, on the lab console's PhaseSpace tab, Interactive Mode's goals (dragged the same way) and
// the thrusters it fires, drawn as plumes on the live platform.

import * as THREE from 'three';
import { OrbitControls } from './vendor/OrbitControls.js';
import { CSS2DRenderer, CSS2DObject } from './vendor/CSS2DRenderer.js';
// Wide lines (WebGL draws ordinary lines 1 px wide whatever width is asked for).
import { Line2 } from './vendor/lines/Line2.js';
import { LineGeometry } from './vendor/lines/LineGeometry.js';
import { LineSegments2 } from './vendor/lines/LineSegments2.js';
import { LineSegmentsGeometry } from './vendor/lines/LineSegmentsGeometry.js';
import { LineMaterial } from './vendor/lines/LineMaterial.js';
import { armPoints, loadCad, mountPose, TABLE_URL } from './cad.js';
import { COLORS, LINE_COLORS, LED_COLORS, TEXT_COLORS, platformName } from './platforms.js';
import { heightCanvas, tickMarks, TABLE as TABLE_TOP } from './calviz.js';

// The granite: the top's sides (measured) and its thickness (0.33 m, its CAD).
export const TABLE = { x: TABLE_TOP.x, y: TABLE_TOP.y, thickness: 0.33 };
export const CRAFT_SIZE = 0.3;
const BODY_AXIS = 0.34;   // length of each body axis from the cube's centre [m]

// Paths trail the platforms' centre of mass, at mid-height of the body.
const PATH_Z = CRAFT_SIZE / 2;
const PATH_WIDTH = 3.5;   // [px] paths: traced behind the platforms as they move

// The platforms' colours (platforms.js): the Chaser (RED) silver, the Target (BLACK) gold, the
// Obstacle (BLUE) blue.
export const CRAFT_COLORS = { RED: COLORS.RED, BLACK: COLORS.BLACK, BLUE: COLORS.BLUE };
export { LINE_COLORS, LED_COLORS };
const NUM_LED_SLOTS = 32;

// Dash and gap [m] of each overlaid result's paths, in the order they were added.
const OVERLAY_DASHES = [[0.06, 0.035], [0.02, 0.025], [0.12, 0.04], [0.04, 0.06]];

// RED's ZED2 camera (the "ZED2 Viewpoint" window), in RED's body frame [m]: on the front (+x)
// face, its right lens 0.08 m right of the centre of geometry (right when looking along +x, so
// -y) and 0.28 m up, the left lens 120 mm to its left; both look along +x, level with the table.
// The front of its housing is 4 mm proud of the panel. With the CAD loaded, the lens positions
// come from the ZED 2i model (info.zed.lenses); these are the same numbers.
const ZED_LENSES = { right: [0.154, -0.08, 0.28], left: [0.154, 0.04, 0.28] };
const ZED_BOX = [0.043, 0.1753, 0.0303];   // the housing [m]: depth, width, height (ZED 2 / 2i)

// The default ZED2 intrinsics at HD720 (1280 x 720, rectified): fx = fy ~ 528 px, the centre in
// the middle (each unit's factory calibration differs slightly). Vertical field of view
// 2 atan(360/528).
const ZED_INTRINSICS = { width: 1280, height: 720, fx: 528, fy: 528, cx: 640, cy: 360 };
const ZED_VFOV = 2 * Math.atan(ZED_INTRINSICS.cy / ZED_INTRINSICS.fy) * 180 / Math.PI;   // 68.6 deg

// The 8 thrusters in the body frame (docs/images/ThrusterConfig.png; the mixer's H matrix in
// spot.control.thrusterDutyCycles): nozzle [x, y] [m], 81 mm in from the edges, and the
// direction the gas leaves in (the platform is pushed the other way).
const THRUSTER_NOZZLES = [
  [[0.1547, 0.069], [1, 0]], [[0.1547, -0.069], [1, 0]],       // T1, T2 push along -x
  [[0.069, -0.1547], [0, -1]], [[-0.069, -0.1547], [0, -1]],   // T3, T4 push along +y
  [[-0.1547, -0.069], [-1, 0]], [[-0.1547, 0.069], [-1, 0]],   // T5, T6 push along +x
  [[-0.069, 0.1547], [0, 1]], [[0.069, 0.1547], [0, 1]],       // T7, T8 push along -y (nozzle tips)
];
// [m] above the table: the MJ4 nozzles, in the side panels' bottom notches.
const NOZZLE_Z = 0.0276;

// A text label in the 3D view (a CSS2D object), with its own background and colour if given.
function label(text, className, background, color) {
  const el = document.createElement('div');
  el.className = 'label3d ' + className;
  el.textContent = text;
  if (background) {
    el.style.background = background;
  }
  if (color) {
    el.style.color = color;
  }
  return new CSS2DObject(el);
}

// The table top's texture: dark granite, drawn at random.
function graniteTexture(renderer) {
  const w = 2048;
  const h = Math.round(w * TABLE.y / TABLE.x);
  const canvas = document.createElement('canvas');
  canvas.width = w;
  canvas.height = h;
  const g = canvas.getContext('2d');
  g.fillStyle = '#3a3d43';
  g.fillRect(0, 0, w, h);

  // Soft cloudy variation in the stone.
  for (let i = 0; i < 320; i++) {
    const x = Math.random() * w;
    const y = Math.random() * h;
    const r = 30 + Math.random() * 160;
    const grad = g.createRadialGradient(x, y, 0, x, y, r);
    grad.addColorStop(0, Math.random() < 0.5 ? 'rgba(125,129,137,0.10)' : 'rgba(8,8,10,0.16)');
    grad.addColorStop(1, 'rgba(0,0,0,0)');
    g.fillStyle = grad;
    g.fillRect(x - r, y - r, 2 * r, 2 * r);
  }

  // Mineral grains: mostly dark, some grey, a few light flecks ([colour, share]).
  const palette = [
    ['#111214', 0.42], ['#24262a', 0.22], ['#55595f', 0.18], ['#7d828a', 0.12], ['#aeb2b8', 0.05],
    ['#d9d4ca', 0.01],
  ];
  const pick = () => {
    let r = Math.random();
    for (const [c, p] of palette) {
      if ((r -= p) <= 0) {
        return c;
      }
    }
    return palette[0][0];
  };
  const grains = Math.round(w * h * 0.055);
  for (let i = 0; i < grains; i++) {
    const x = Math.random() * w;
    const y = Math.random() * h;
    g.globalAlpha = 0.5 + Math.random() * 0.5;
    g.fillStyle = pick();
    if (Math.random() < 0.88) {
      const s = 0.8 + Math.random() * 1.6;
      g.fillRect(x, y, s, s);
    } else {
      // A larger irregular crystal.
      const r = 1.5 + Math.random() * 3.2;
      const sides = 5 + (Math.random() * 3 | 0);
      const rot = Math.random() * Math.PI;
      g.beginPath();
      for (let k = 0; k < sides; k++) {
        const a = rot + k / sides * Math.PI * 2;
        const rr = r * (0.6 + Math.random() * 0.5);
        k ? g.lineTo(x + Math.cos(a) * rr, y + Math.sin(a) * rr)
          : g.moveTo(x + Math.cos(a) * rr, y + Math.sin(a) * rr);
      }
      g.closePath();
      g.fill();
    }
  }
  g.globalAlpha = 1;

  const tex = new THREE.CanvasTexture(canvas);
  tex.colorSpace = THREE.SRGBColorSpace;
  tex.anisotropy = renderer.capabilities.getMaxAnisotropy();
  return tex;
}

/** The 3D view: the table, the platforms and their layers, and the ZED2 Viewpoint window. */
export class Viewer {
  /**
   * Builds the view in CONTAINER.
   * @param {!Element} container
   * @param {{capture: boolean}=} opts capture: keep the drawing buffer so the view can be saved
   *     or screenshotted (a little slower).
   */
  constructor(container, { capture = false } = {}) {
    THREE.Object3D.DEFAULT_UP.set(0, 0, 1);
    this.container = container;
    this.dropLines = true;

    // The renderer (with shadows) and the labels' renderer over it.
    this.renderer = new THREE.WebGLRenderer({ antialias: true, preserveDrawingBuffer: capture });
    this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    container.prepend(this.renderer.domElement);
    this.labels = new CSS2DRenderer();
    Object.assign(this.labels.domElement.style,
      { position: 'absolute', top: '0', left: '0', pointerEvents: 'none', zIndex: '1' });
    container.prepend(this.labels.domElement);

    // The scene (the floor fades out instead of showing its edge), the camera and its controls.
    this.scene = new THREE.Scene();
    this.scene.background = new THREE.Color(0x0e1014);
    this.scene.fog = new THREE.Fog(0x0e1014, 9, 24);
    this.camera = new THREE.PerspectiveCamera(42, 1, 0.01, 200);
    this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    this.controls.enableDamping = true;
    this.controls.dampingFactor = 0.08;
    this.controls.minDistance = 0.3;
    this.controls.maxDistance = 25;

    // ZED2 Viewpoint: a peek window in the corner that shows what a camera on RED sees (its own
    // small renderer, made when first switched on).
    this.redCam = {
      on: false, lens: 'left', renderer: null, note: '', onClose: null, onLens: null,
      cameras: {
        left: new THREE.PerspectiveCamera(ZED_VFOV, 16 / 9, 0.01, 200),
        right: new THREE.PerspectiveCamera(ZED_VFOV, 16 / 9, 0.01, 200),
      },
    };
    this._buildRedCamWindow();

    this._buildLights();
    this._buildTable();
    this._buildAxes();
    this.layers = { live: true, sim: false, setup: false };
    // Every platform's and marker's axes (the Axes switch).
    this.bodyAxes = { on: true, groups: new Set() };

    // Use CAD: the platforms (and replayed arms) drawn from the lab's CAD instead of cubes and
    // lines.
    this.cad = { on: false, lib: null, loading: null, crafts: new Set(), arms: new Set() };

    // Spacecraft Designer: what is mounted on each platform (drawn with the CAD), and the page's
    // close-up of one platform with the 16 docking-cone places to click.
    this.design = null;
    // A replayed arm shows only while the Designer's Chaser carries it, or for the results the
    // person chose to see it on (shown); asked: results the page was told about (onHidden).
    this.armChoice = { shown: new WeakSet(), asked: new WeakSet(), onHidden: null };
    this.designer = {
      active: false, group: null, slots: [], platform: null, onSlot: null, selected: null,
      savedCam: null, hover: null,
    };

    // The live platforms, the LEDs, the live paths and the other layers.
    this.crafts = {};
    for (const name of Object.keys(CRAFT_COLORS)) {
      this.crafts[name] = this._buildCraft(name);
    }
    this._buildLeds();
    this._buildLivePaths();
    this._buildSimLayer();
    this._buildSetupLayer();
    this._buildGoalLayer();
    this._bindDrag();

    // Follow the container's size; draw every frame (and the ZED2 Viewpoint when on).
    new ResizeObserver(() => this._resize()).observe(container);
    this._resize();
    this.setView('3d');
    this.renderer.setAnimationLoop(() => {
      this.controls.update();
      this.renderer.render(this.scene, this.camera);
      this.labels.render(this.scene, this.camera);
      if (this.redCam.on) {
        this._renderRedCam();
      }
    });
  }

  // The renderers and the camera at the container's size.
  _resize() {
    const w = this.container.clientWidth;
    const h = this.container.clientHeight;
    if (!w || !h) {
      return;
    }
    this.renderer.setSize(w, h);
    this.labels.setSize(w, h);
    this.camera.aspect = w / h;
    this.camera.updateProjectionMatrix();
  }

  // The lab's ceiling lights: one straight above the table casts the shadows (so a platform's
  // shadow falls right under it, and the table shades the floor), with soft fill from the room.
  _buildLights() {
    this.scene.add(new THREE.HemisphereLight(0xdfe7ff, 0x16181c, 1.1));
    const ceiling = new THREE.DirectionalLight(0xffffff, 2.3);
    ceiling.position.set(TABLE.x / 2, TABLE.y / 2, 6);
    ceiling.target.position.set(TABLE.x / 2, TABLE.y / 2, 0);
    ceiling.castShadow = true;
    ceiling.shadow.mapSize.set(2048, 2048);
    Object.assign(ceiling.shadow.camera,
      { left: -2.4, right: 2.4, top: 1.9, bottom: -1.9, near: 2, far: 8 });
    ceiling.shadow.bias = -0.0003;
    ceiling.shadow.normalBias = 0.002;
    this.scene.add(ceiling, ceiling.target);

    // Two more panels off to the sides, without shadows, so the platforms' sides aren't flat.
    for (const [dx, dy, i] of [[-1.4, -1.0, 0.55], [1.4, 1.0, 0.45]]) {
      const fill = new THREE.DirectionalLight(0xf2f5ff, i);
      fill.position.set(TABLE.x / 2 + dx, TABLE.y / 2 + dy, 5);
      fill.target.position.set(TABLE.x / 2, TABLE.y / 2, 0);
      this.scene.add(fill, fill.target);
    }
  }

  // The granite table, its stand and barrier, the floor, and the table top's grid and ticks.
  _buildTable() {
    // The slab (BoxGeometry faces: +x, -x, +y, -y, +z (top), -z); it shades the floor under it.
    const top = new THREE.MeshStandardMaterial({
      map: graniteTexture(this.renderer), roughness: 0.5, metalness: 0.02,
    });
    const side =
      new THREE.MeshStandardMaterial({ color: 0x2b2d32, roughness: 0.55, metalness: 0.05 });
    const slab = new THREE.Mesh(new THREE.BoxGeometry(TABLE.x, TABLE.y, TABLE.thickness),
      [side, side, side, side, top, side]);
    slab.position.set(TABLE.x / 2, TABLE.y / 2, -TABLE.thickness / 2);
    slab.receiveShadow = true;
    slab.castShadow = true;
    this.scene.add(slab);

    // The steel stand: a frame under the slab, a leg under each corner, stretchers near the
    // floor, and a bracket wrapping each corner of the frame, all painted as the barrier (its
    // very material, reflections included, once the barrier has loaded).
    const legH = 0.55;
    const floorZ = -TABLE.thickness - legH;
    const steel =
      new THREE.MeshStandardMaterial({ color: 0x15171a, metalness: 0.1, roughness: 0.2 });
    const stand = [];
    const add = (w, d, h, x, y, z, mat = steel) => {
      const m = new THREE.Mesh(new THREE.BoxGeometry(w, d, h), mat);
      m.position.set(x, y, z);
      m.castShadow = m.receiveShadow = true;
      this.scene.add(m);
      stand.push(m);
      return m;
    };
    const leg = 0.1;
    const apron = 0.1;
    const under = -TABLE.thickness;
    const corners = [[0, 0], [TABLE.x, 0], [0, TABLE.y], [TABLE.x, TABLE.y]];
    for (const [cx, cy] of corners) {
      // Toward the table's inside.
      const sx = cx ? -1 : 1;
      const sy = cy ? -1 : 1;
      add(leg, leg, legH, cx + sx * leg / 2, cy + sy * leg / 2, under - legH / 2);

      // The corner bracket: two plates round the outside of the corner, over the frame and the
      // leg's top.
      const t = 0.006;
      const reach = 0.2;
      const bh = 0.18;
      add(reach, t, bh, cx + sx * reach / 2, cy - sy * t / 2, under - bh / 2);
      add(t, reach, bh, cx - sx * t / 2, cy + sy * reach / 2, under - bh / 2);
    }

    // The frame right under the slab, along all four edges.
    add(TABLE.x - 2 * leg, leg * 0.8, apron, TABLE.x / 2, leg * 0.4, under - apron / 2);
    add(TABLE.x - 2 * leg, leg * 0.8, apron, TABLE.x / 2, TABLE.y - leg * 0.4, under - apron / 2);
    add(leg * 0.8, TABLE.y - 2 * leg, apron, leg * 0.4, TABLE.y / 2, under - apron / 2);
    add(leg * 0.8, TABLE.y - 2 * leg, apron, TABLE.x - leg * 0.4, TABLE.y / 2, under - apron / 2);

    // Stretchers near the floor.
    const low = floorZ + 0.14;
    add(TABLE.x - 2 * leg, 0.06, 0.06, TABLE.x / 2, leg / 2, low);
    add(TABLE.x - 2 * leg, 0.06, 0.06, TABLE.x / 2, TABLE.y - leg / 2, low);
    add(0.06, TABLE.y - 2 * leg, 0.06, leg / 2, TABLE.y / 2, low);
    add(0.06, TABLE.y - 2 * leg, 0.06, TABLE.x - leg / 2, TABLE.y / 2, low);

    // The barrier round the table's edges (rails, corner brackets, hinges; from its CAD, loaded
    // in the background); the stand takes its paint.
    loadCad(TABLE_URL).then(lib => {
      const g = lib._group('BARRIER', 'table', false);
      g.traverse(o => {
        if (o.isMesh) {
          o.castShadow = o.receiveShadow = true;
        }
      });
      const paint = lib.material('paint', 'table', false);
      for (const m of stand) {
        m.material = paint;
      }
      steel.dispose();
      this.barrier = g;
      this.scene.add(g);
    }).catch(e => console.warn('Table barrier not loaded:', e.message));

    // The floor, with a faint grid.
    const floor = new THREE.Mesh(new THREE.CircleGeometry(40, 64),
      new THREE.MeshStandardMaterial({ color: 0x111317, roughness: 0.95 }));
    floor.position.set(TABLE.x / 2, TABLE.y / 2, floorZ);
    floor.receiveShadow = true;
    this.scene.add(floor);
    const floorGrid = new THREE.GridHelper(40, 80, 0x20242c, 0x1a1d23);
    floorGrid.rotation.x = Math.PI / 2;
    floorGrid.position.set(TABLE.x / 2, TABLE.y / 2, floorZ + 0.001);
    this.scene.add(floorGrid);

    // 0.5 m grid and outline on the table top.
    const pts = [];
    for (let x = 0.5; x < TABLE.x - 1e-6; x += 0.5) {
      pts.push(x, 0, 0.0008, x, TABLE.y, 0.0008);
    }
    for (let y = 0.5; y < TABLE.y - 1e-6; y += 0.5) {
      pts.push(0, y, 0.0008, TABLE.x, y, 0.0008);
    }
    const gridGeo = new THREE.BufferGeometry();
    gridGeo.setAttribute('position', new THREE.Float32BufferAttribute(pts, 3));
    this.scene.add(new THREE.LineSegments(gridGeo,
      new THREE.LineBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.09 })));
    const outline = new THREE.LineLoop(new THREE.BufferGeometry().setFromPoints([
      new THREE.Vector3(0, 0, 0.001), new THREE.Vector3(TABLE.x, 0, 0.001),
      new THREE.Vector3(TABLE.x, TABLE.y, 0.001), new THREE.Vector3(0, TABLE.y, 0.001)]),
    new THREE.LineBasicMaterial({ color: 0xffffff, transparent: true, opacity: 0.3 }));
    this.scene.add(outline);

    // Distance ticks along the two edges that meet at the origin (every 0.5 m, then the side).
    for (const x of tickMarks(TABLE.x).filter(v => v > 0)) {
      const t = label(x === TABLE.x ? `${x.toFixed(2)} m` : x.toFixed(1), 'tick-label');
      t.position.set(x, -0.09, 0);
      this.scene.add(t);
    }
    for (const y of tickMarks(TABLE.y).filter(v => v > 0)) {
      const t = label(y === TABLE.y ? `${y.toFixed(2)} m` : y.toFixed(1), 'tick-label');
      t.position.set(-0.11, y, 0);
      this.scene.add(t);
    }
  }

  // The table's axes at the origin, a little above the table so the arrows clear the barrier
  // along its edges (6.5 mm high), and the origin's label just outside the corner.
  _buildAxes() {
    const o = new THREE.Vector3(0, 0, 0.015);
    const axes = [
      [new THREE.Vector3(1, 0, 0), 0xff6b6b, '+X'],
      [new THREE.Vector3(0, 1, 0), 0x51cf66, '+Y'],
      [new THREE.Vector3(0, 0, 1), 0x4dabf7, '+Z'],
    ];
    for (const [dir, color, text] of axes) {
      this.scene.add(new THREE.ArrowHelper(dir, o, 0.45, color, 0.08, 0.045));
      const l = label(text, 'axis-label');
      l.element.style.color = '#' + color.toString(16).padStart(6, '0');
      l.position.copy(dir.clone().multiplyScalar(0.55)).add(o);
      this.scene.add(l);
    }
    const origin = label('(0, 0, 0)', 'origin-label');
    origin.position.set(0, 0, 0);
    origin.center.set(1.08, -0.15);
    this.scene.add(origin);
  }

  // A platform: a cube with its edges, body axes and name tag (or its CAD model), and on RED the
  // ZED2. ghost: 'sim' for the simulation's see-through copy, 'overlay' for a loaded result's
  // outline (tagged TAGTEXT).
  _buildCraft(name, ghost = false, tagText = '') {
    const color = CRAFT_COLORS[name];
    const group = new THREE.Group();
    const geo = new THREE.BoxGeometry(CRAFT_SIZE, CRAFT_SIZE, CRAFT_SIZE);
    const opacity = ghost === 'overlay' ? 0.1 : ghost ? 0.45 : 1;
    const metal = name !== 'BLUE';   // the Chaser silver, the Target gold
    const box = new THREE.Mesh(geo, new THREE.MeshStandardMaterial({
      color, roughness: metal ? 0.35 : 0.5, metalness: metal ? 0.45 : 0.15,
      transparent: Boolean(ghost), opacity, depthWrite: !ghost }));
    box.position.z = CRAFT_SIZE / 2;
    box.castShadow = !ghost;
    box.receiveShadow = true;
    const edgeMat = ghost === 'overlay'
      ? new THREE.LineDashedMaterial({ color: LINE_COLORS[name], dashSize: 0.03, gapSize: 0.02 })
      : new THREE.LineBasicMaterial({
        color: ghost ? LINE_COLORS[name] : 0xffffff, transparent: true,
        opacity: ghost ? 0.95 : 0.4 });
    const edges = new THREE.LineSegments(new THREE.EdgesGeometry(geo), edgeMat);
    if (ghost === 'overlay') {
      edges.computeLineDistances();
    }
    edges.position.z = CRAFT_SIZE / 2;

    // Body axes from the centre of the cube (x red, y green, z blue), so the attitude is visible.
    const axes = this._bodyAxes(ghost === 'overlay' ? 0.7 : 1);
    axes.position.z = CRAFT_SIZE / 2;

    // The name tag, above the +Z label.
    const shown = platformName(name);
    const text = ghost === 'overlay' ? `${shown} · ${tagText}` : ghost ? `${shown} · Sim` : shown;
    const tag = ghost === 'overlay'
      ? label(text, 'craft-label', null, LINE_COLORS[name])
      : label(text, 'craft-label', color, TEXT_COLORS[name]);
    if (ghost) {
      tag.element.classList.add(ghost === 'overlay' ? 'overlay' : 'ghost');
    }
    tag.position.z = CRAFT_SIZE / 2 + BODY_AXIS + (ghost === 'overlay' ? 0.2 : 0.12);
    tag.center.set(0.5, 1);

    // The axes, the tag and the ZED2 housing are hidden while the ZED2 Viewpoint looks out from
    // this platform (they would sit in front of the lens).
    group.add(box, edges, axes, tag);
    Object.assign(group.userData,
      { craft: name, ghost, box, edges, cad: null, ownParts: [axes, tag] });
    if (name === 'RED') {
      const cam = this._buildRedCameraModel(opacity);
      group.add(cam);
      group.userData.ownParts.push(cam);
      group.userData.housing = cam;
    }
    this.cad.crafts.add(group);
    this._applyCadCraft(group);
    group.visible = false;
    this.scene.add(group);
    return group;
  }

  // The ZED2 on RED's front face as a box of its size (the cube view; the CAD has the real one),
  // with its two lenses, where the ZED2 Viewpoint looks from.
  _buildRedCameraModel(opacity = 1) {
    const group = new THREE.Group();
    const see = opacity < 1
      ? { transparent: true, opacity: Math.max(opacity, 0.35), depthWrite: false }
      : {};
    const [front, ly, lz] = ZED_LENSES.right;
    const proud = front - CRAFT_SIZE / 2;   // the part ahead of the cube's face
    const housing = new THREE.Mesh(new THREE.BoxGeometry(proud, ZED_BOX[1], ZED_BOX[2]),
      new THREE.MeshStandardMaterial({ color: 0x2a2d32, roughness: 0.45, metalness: 0.4, ...see }));
    housing.position.set(CRAFT_SIZE / 2 + proud / 2,
      (ZED_LENSES.right[1] + ZED_LENSES.left[1]) / 2, lz);
    housing.castShadow = opacity === 1;
    group.add(housing);
    for (const [, y] of [ZED_LENSES.right, ZED_LENSES.left]) {
      const lens = new THREE.Mesh(new THREE.CylinderGeometry(0.008, 0.008, 0.002, 24),
        new THREE.MeshStandardMaterial({
          color: 0x0b0d10, roughness: 0.05, metalness: 0.2, emissive: 0x0b2a44, ...see,
        }));
      lens.rotation.z = -Math.PI / 2;   // cylinder axis (y) along +x
      lens.position.set(front + 0.001, y, lz);
      group.add(lens);
    }
    return group;
  }

  // Three arrows from the origin, long enough to stick out of the cube (whose half-size is
  // CRAFT_SIZE / 2): +x red, +y green, +z blue, with +X/+Y/+Z beyond the tips, as the table's
  // own axes (the labels follow the Labels switch).
  _bodyAxes(opacity = 1) {
    const group = new THREE.Group();
    group.visible = this.bodyAxes.on;
    this.bodyAxes.groups.add(group);
    for (const [dir, color, text] of [
      [[1, 0, 0], 0xff6b6b, '+X'], [[0, 1, 0], 0x51cf66, '+Y'], [[0, 0, 1], 0x4dabf7, '+Z'],
    ]) {
      const d = new THREE.Vector3(...dir);
      const arrow =
        new THREE.ArrowHelper(d, new THREE.Vector3(0, 0, 0), BODY_AXIS, color, 0.08, 0.05);
      for (const m of [arrow.line.material, arrow.cone.material]) {
        m.transparent = opacity < 1;
        m.opacity = opacity;
      }
      const l = label(text, 'axis-label body-axis-label');
      l.element.style.color = '#' + color.toString(16).padStart(6, '0');
      l.element.style.opacity = opacity;
      l.position.copy(d.multiplyScalar(BODY_AXIS + 0.055));
      group.add(arrow, l);
    }
    return group;
  }

  // NUM_LED_SLOTS LEDs, each a dot with a halo, a drop line to the table (scaled to reach it)
  // and its number.
  _buildLeds() {
    const core = new THREE.SphereGeometry(0.012, 20, 14);
    const halo = new THREE.SphereGeometry(0.03, 20, 14);
    const drop = new THREE.BufferGeometry()
      .setFromPoints([new THREE.Vector3(0, 0, 0), new THREE.Vector3(0, 0, -1)]);
    this.leds = [];
    for (let k = 0; k < NUM_LED_SLOTS; k++) {
      const coreMat = new THREE.MeshBasicMaterial({ color: LED_COLORS.none });
      const haloMat = new THREE.MeshBasicMaterial({
        color: LED_COLORS.none, transparent: true, opacity: 0.25,
        blending: THREE.AdditiveBlending, depthWrite: false,
      });
      const lineMat =
        new THREE.LineBasicMaterial({ color: LED_COLORS.none, transparent: true, opacity: 0.35 });
      const group = new THREE.Group();
      const line = new THREE.Line(drop, lineMat);
      const tag = label(String(k), 'led-label');
      tag.position.z = 0.028;
      tag.center.set(0.5, 1);
      group.add(new THREE.Mesh(core, coreMat), new THREE.Mesh(halo, haloMat), line, tag);
      group.visible = false;
      this.scene.add(group);
      this.leds.push({ group, line, mats: [coreMat, haloMat, lineMat], tag });
    }
  }

  /**
   * Colours each LED by the platform it is on.
   * @param {!Object<number, string>} owner LED number -> platform key.
   */
  setLedOwner(owner) {
    this.leds.forEach((led, k) => {
      const color = LED_COLORS[owner[k]] || LED_COLORS.none;
      for (const m of led.mats) {
        m.color.set(color);
      }
      led.tag.element.style.color = color;
    });
  }

  /**
   * A PhaseSpace frame: the live platforms (and their paths), the goal lines and the LEDs.
   * @param {!Object} frame {crafts: {NAME: {tracked, x, y, att}}, leds: [[k, x, y, z], ...]}.
   */
  update(frame) {
    this.lastFrame = frame;
    for (const [name, c] of Object.entries(frame.crafts || {})) {
      const g = this.crafts[name];
      if (!g) {
        continue;
      }
      g.visible = Boolean(this.layers.live && c.tracked && c.x != null && c.y != null);
      if (g.visible) {
        g.position.set(c.x, c.y, 0);
        g.rotation.z = c.att || 0;
        this._extendLivePath(name, c.x, c.y);
      }
    }
    this._updateGoalLine();

    // The LEDs seen (with their drop lines, if on); the others are hidden.
    const seen = new Set();
    for (const [k, x, y, z] of this.layers.live ? frame.leds || [] : []) {
      const led = this.leds[k];
      if (!led) {
        continue;
      }
      seen.add(k);
      led.group.visible = true;
      led.group.position.set(x, y, z);
      led.line.scale.z = Math.max(z, 1e-4);
      led.line.visible = this.dropLines && z > 0;
    }
    this.leds.forEach((led, k) => {
      if (!seen.has(k)) {
        led.group.visible = false;
      }
    });
  }

  // Layers

  /**
   * Shows or hides layers (the simulation's CAD copy: solid alone, see-through over live).
   * @param {{live: boolean, sim: boolean, setup: boolean}} layers Those to change.
   */
  setLayers(layers) {
    Object.assign(this.layers, layers);
    if (this.lastFrame) {
      this.update(this.lastFrame);
    } else {
      for (const g of Object.values(this.crafts)) {
        g.visible = false;
      }
    }
    this.sim.group.visible = this.layers.sim;
    this.setup.group.visible = this.layers.setup;
    this._checkArms();
    for (const p of Object.values(this.live.paths)) {
      p.line.visible = this.live.on && this.layers.live;
    }
    if (this.cad.lib) {
      this._applyCad();
    }
  }

  // Live paths (where each platform has been)

  // Each path is a wide line of up to max segments, written in place as the platform moves.
  _buildLivePaths() {
    this.live = { paths: {}, on: false, max: 20000 };
    for (const name of Object.keys(CRAFT_COLORS)) {
      const geo = new LineSegmentsGeometry();
      geo.setPositions(new Float32Array(this.live.max * 6));
      geo.instanceCount = 0;
      const line = new LineSegments2(geo, new LineMaterial({
        color: new THREE.Color(LINE_COLORS[name]), linewidth: PATH_WIDTH, transparent: true,
        opacity: 0.85 }));
      line.frustumCulled = false;
      line.visible = false;
      this.scene.add(line);
      this.live.paths[name] = { line, seg: geo.attributes.instanceStart.data, n: 0, last: null };
    }
  }

  // Adds the platform's move to its path, only when it has moved (3 mm), so a platform at rest
  // adds nothing; a full path keeps its newer half.
  _extendLivePath(name, x, y) {
    const p = this.live.paths[name];
    if (!p || !this.live.on) {
      return;
    }
    if (!p.last) {
      p.last = [x, y];
      return;
    }
    const dx = x - p.last[0];
    const dy = y - p.last[1];
    if (dx * dx + dy * dy < 9e-6) {
      return;
    }
    const arr = p.seg.array;
    if (p.n >= this.live.max) {
      const half = this.live.max / 2;
      arr.copyWithin(0, 6 * half, 6 * this.live.max);
      p.n = half;
    }
    arr.set([p.last[0], p.last[1], PATH_Z, x, y, PATH_Z], 6 * p.n);
    p.n++;
    p.last = [x, y];
    p.seg.needsUpdate = true;
    p.line.geometry.instanceCount = p.n;
  }

  /**
   * Traces the live platforms' paths, or not.
   * @param {boolean} on
   */
  setLivePaths(on) {
    this.live.on = on;
    for (const p of Object.values(this.live.paths)) {
      p.line.visible = on && this.layers.live;
    }
  }

  /** Clears the live platforms' paths. */
  clearLivePaths() {
    for (const p of Object.values(this.live.paths)) {
      p.n = 0;
      p.last = null;
      p.line.geometry.instanceCount = 0;
    }
  }

  // Simulation layer
  // Replays of logged motion ("tracks"): the last simulation (the primary track: see-through
  // platforms, solid paths) and any loaded results laid over it (outlined platforms, dashed
  // paths, named after the dataset), each with its own time offset.

  _buildSimLayer() {
    this.sim = { group: new THREE.Group(), traces: true, overlays: new Map(), t: 0 };
    this.sim.group.visible = false;
    this.scene.add(this.sim.group);
    this.sim.primary = this._buildTrack({ ghost: 'sim' });
    this.sim.primary.shown = true;
  }

  // A track: a copy of each platform and the arm on RED (links and joints, at mid-height of the
  // body). ghost: 'sim' (the simulation's see-through copy) or 'overlay' (outline only).
  _buildTrack({ ghost, label = '', dash = null }) {
    const track = {
      group: new THREE.Group(), crafts: {}, paths: {}, result: null, offset: 0, dash, shown: true,
      label,
    };
    for (const name of Object.keys(CRAFT_COLORS)) {
      const g = this._buildCraft(name, ghost, label);
      g.userData.track = track;
      this.scene.remove(g);
      track.group.add(g);
      track.crafts[name] = g;
    }
    const armMat = dash
      ? new THREE.LineDashedMaterial(
        { color: LINE_COLORS.RED, dashSize: dash[0] / 2, gapSize: dash[1] / 2 })
      : new THREE.LineBasicMaterial({ color: LINE_COLORS.RED });
    track.arm = this._buildArm(armMat, ghost);
    track.group.add(track.arm.group);
    this.sim.group.add(track.group);
    return track;
  }

  // Removes a track and frees what it alone uses (ArrowHelper's line and cone geometries are
  // shared by every arrow, and the CAD models' by every copy: keep them; CSS2D labels go).
  _disposeTrack(track) {
    this._setTrackResult(track, null);
    this.sim.group.remove(track.group);
    this.cad.arms.delete(track.arm);
    track.group.traverse(o => {
      this.bodyAxes.groups.delete(o);
      this.cad.crafts.delete(o);
      if (o.geometry && o.parent?.type !== 'ArrowHelper' && !o.userData.sharedGeometry) {
        o.geometry.dispose();
      }
      if (o.element) {
        o.element.remove();
      }
    });
  }

  // An arm: a line through its joints, with a ball at each (or its CAD links). ghost: the
  // track's ('sim' or 'overlay'), for its CAD links; null for the initial-condition arm, which
  // stays a line.
  _buildArm(material, ghost = null) {
    const group = new THREE.Group();
    const geo = new THREE.BufferGeometry();
    geo.setAttribute('position', new THREE.Float32BufferAttribute(new Float32Array(12), 3));
    const line = new THREE.Line(geo, material);
    const joint = new THREE.SphereGeometry(0.018, 14, 10);
    const jointMat = new THREE.MeshStandardMaterial({ color: 0xf1f3f5, roughness: 0.4 });
    const joints = [0, 1, 2, 3].map(() => new THREE.Mesh(joint, jointMat));
    group.add(line, ...joints);
    group.visible = false;
    const arm = { group, line, joints, ghost, cad: null, pts: null };
    if (ghost) {
      this.cad.arms.add(arm);
      this._applyCadArm(arm);
    }
    return arm;
  }

  // Places an arm. pts: [[x, y] x 4] shoulder, elbow, wrist, end effector. The CAD links each
  // run from their joint toward the next, stretched to the arm's own link length.
  _setArm(arm, pts) {
    arm.pts = pts;
    const z = CRAFT_SIZE * 0.55;
    const pos = arm.line.geometry.attributes.position;
    pts.forEach(([x, y], k) => {
      pos.setXYZ(k, x, y, z);
      arm.joints[k].position.set(x, y, z);
    });
    pos.needsUpdate = true;
    arm.line.geometry.computeBoundingSphere();
    if (arm.line.material.isLineDashedMaterial) {
      arm.line.computeLineDistances();
    }
    arm.cad?.links.forEach((link, k) => {
      const [x0, y0] = pts[k];
      const dx = pts[k + 1][0] - x0;
      const dy = pts[k + 1][1] - y0;
      link.position.set(x0, y0, 0);
      link.rotation.z = Math.atan2(dy, dx);
      link.scale.x = Math.hypot(dx, dy) / link.userData.length;
    });
  }

  // Replayed arms and the design. A run's arm is part of the Chaser: while the Designer's Chaser
  // has none, a replayed arm is hidden (its links would hang off whatever is mounted instead)
  // unless the person chose to see it for that result; then its Chaser also carries the arm's
  // mount, where the run had it.

  // The simulation and the loaded results.
  _tracks() {
    return [this.sim.primary, ...this.sim.overlays.values()];
  }

  // Whether the Designer's Chaser carries the arm (true while no design is known).
  _armDesigned() {
    const red = this._designOf('RED');
    return !red || !red.dock || red.dock.type === 'arm';
  }

  // Whether a track's arm is drawn.
  _armShown(track) {
    const r = track.result;
    return Boolean(r && r.arm) && (this._armDesigned() || this.armChoice.shown.has(r));
  }

  // The design a platform is drawn with: a replayed Chaser whose arm is shown although the
  // Designer's has none carries the arm's mount at the run's place.
  _craftDesign(u) {
    const design = this._designOf(u.craft);
    const track = u.craft === 'RED' ? u.track : null;
    if (!track || this._armDesigned() || !this._armShown(track)) {
      return design;
    }
    const mount = this._armMount(track.result);
    return mount ? { ...design, dock: { type: 'arm', ...mount } } : design;
  }

  // Where a result's arm was mounted: the place whose shoulder is nearest the run's, in the
  // Chaser's body frame at the first sample with both; null if unknown (or no CAD loaded).
  _armMount(r) {
    const info = this.cad.lib && this.cad.lib.info;
    const c = r && r.arm && r.crafts.RED;
    if (!info || !info.mount || !c) {
      return null;
    }
    const [sx, sy] = r.arm[0];
    const k = c.x.findIndex((x, i) =>
      x != null && c.y[i] != null && c.att[i] != null && sx[i] != null && sy[i] != null);
    if (k < 0) {
      return null;
    }
    const dx = sx[k] - c.x[k];
    const dy = sy[k] - c.y[k];
    const ca = Math.cos(c.att[k]);
    const sa = Math.sin(c.att[k]);
    const bx = ca * dx + sa * dy;
    const by = -sa * dx + ca * dy;
    let best = null;
    let bestD = Infinity;
    for (const face of info.mount.faces || ['+x', '+y', '-y']) {
      for (const side of ['left', 'right']) {
        const [x, y] = armPoints(info, { face, side }, [0, 0, 0])[0];
        const d = Math.hypot(x - bx, y - by);
        if (d < bestD) {
          best = { face, side };
          bestD = d;
        }
      }
    }
    return best;
  }

  // Every replayed arm (and its Chaser's mount) as the design and the person's choice have it.
  _refreshArms() {
    for (const track of this._tracks()) {
      this._applyCadCraft(track.crafts.RED);
      track.arm.group.visible = this._armShown(track);
      this._setTrackTime(track, this.sim.t);
    }
    this._checkArms();
  }

  // Tells the page about results in view whose arm is hidden by the design, once each:
  // armChoice.onHidden(labels, show), labels the tracks' ('' for the simulation), show() draws
  // their arms.
  _checkArms() {
    if (!this.armChoice.onHidden || !this.layers.sim || this._armDesigned()) {
      return;
    }
    const fresh = this._tracks().filter(t => t.shown && t.result && t.result.arm &&
      !this.armChoice.shown.has(t.result) && !this.armChoice.asked.has(t.result));
    if (!fresh.length) {
      return;
    }
    const results = fresh.map(t => t.result);
    results.forEach(r => this.armChoice.asked.add(r));
    this.armChoice.onHidden(fresh.map(t => t.label), () => {
      results.forEach(r => this.armChoice.shown.add(r));
      this._refreshArms();
    });
  }

  /**
   * Forgets which results the page was told had their arm hidden, and tells it again about
   * those in view (the Designer's Ask Again).
   */
  askArmsAgain() {
    this.armChoice.asked = new WeakSet();
    this._checkArms();
  }

  // CAD models (Use CAD)

  /**
   * Draws the platforms from the lab's CAD (loaded the first time).
   * @param {boolean} on
   * @return {!Promise} Rejects if the models can't be loaded.
   */
  async setUseCad(on) {
    this.cad.on = on;
    if (on && !this.cad.lib) {
      this.cad.loading ||= loadCad();
      try {
        this.cad.lib = await this.cad.loading;
      } catch (e) {
        this.cad.loading = null;
        this.cad.on = false;
        throw e;
      }
    }
    this._applyCad();
  }

  // Every platform and arm as Use CAD has it.
  _applyCad() {
    for (const g of this.cad.crafts) {
      this._applyCadCraft(g);
    }
    for (const a of this.cad.arms) {
      this._applyCadArm(a);
    }
  }

  // Spacecraft Designer

  // A platform's design ({dock, solar}), or null.
  _designOf(name) {
    return this.design ? this.design[name] || null : null;
  }

  /**
   * What is mounted on each platform; every CAD platform is rebuilt with it.
   * @param {?Object} platforms { RED: {dock, solar}, ... } (design.py).
   */
  setDesign(platforms) {
    this.design = platforms || null;
    for (const g of this.cad.crafts) {
      const u = g.userData;
      if (u.cad) {
        g.remove(u.cad);
        u.cad = null;
      }
      this._applyCadCraft(g);
    }
    this._refreshArms();
    if (this.designer.active) {
      this._buildDesigner();
    }
  }

  /**
   * Shows one platform's CAD model close up at the table's centre, with a marker on each of the
   * 6 docking-cone places (clicking one calls opts.onSlot({face, side})); null leaves it (the
   * camera goes back).
   * @param {?{platform: string, onSlot: function(!Object), selected: ?Object, arm: ?Object}} opts
   *     selected: the dock's place, or null when it has none; arm: { q: joint angles [rad],
   *     geometry: the settings' arm } for the Chaser's arm, or null.
   * @return {!Promise} Rejects if the CAD models can't be loaded.
   */
  async setDesigner(opts) {
    const d = this.designer;
    if (!opts) {
      if (!d.active) {
        return;
      }
      d.active = false;
      this._dropDesigner();
      d.slots = [];
      this.goal.group.visible = true;
      if (d.savedCam) {
        this.camera.position.copy(d.savedCam.pos);
        this.controls.target.copy(d.savedCam.target);
        this.controls.update();
        d.savedCam = null;
      }
      return;
    }

    // Shown (the camera saved the first time), with the CAD loaded; unless left while loading.
    const first = !d.active;
    const moved = d.platform !== opts.platform;
    if (first) {
      d.savedCam = { pos: this.camera.position.clone(), target: this.controls.target.clone() };
    }
    Object.assign(d, opts, { active: true });
    this.goal.group.visible = false;
    if (!this.cad.lib) {
      this.cad.loading ||= loadCad();
      try {
        this.cad.lib = await this.cad.loading;
      } catch (e) {
        this.cad.loading = null;
        throw e;
      }
      if (this.cad.on) {
        this._applyCad();
      }
    }
    if (!d.active) {
      return;
    }
    this._buildDesigner();

    // Look at the face with the docking cone (or the front), from a little to its side and above.
    if (first || moved) {
      const face = (d.selected || { face: '+x' }).face;
      const az = { '+x': 0, '+y': Math.PI / 2, '-x': Math.PI, '-y': -Math.PI / 2 }[face] - 0.6;
      const c = new THREE.Vector3(TABLE.x / 2, TABLE.y / 2, 0.15);
      const r = 1.05;
      this.controls.target.copy(c);
      this.camera.position.set(c.x + r * Math.cos(az) * 0.85, c.y + r * Math.sin(az) * 0.85,
        c.z + r * 0.55);
      this.controls.update();
    }
  }

  // The close-up: the platform with its design (and ZED2), the arm at its initial pose, and the
  // places to click.
  _buildDesigner() {
    const d = this.designer;
    const lib = this.cad.lib;
    if (!lib || !d.active) {
      return;
    }
    this._dropDesigner();
    const group = new THREE.Group();
    group.position.set(TABLE.x / 2, TABLE.y / 2, 0);
    const design = this._designOf(d.platform);
    group.add(lib.platform(d.platform, false, design));
    if (d.platform === 'RED') {
      group.add(lib.zed(false));
    }

    // The platform's body frame from the centre of its body, as on the other pages (the Axes
    // switch shows or hides it).
    const axes = this._bodyAxes();
    axes.position.z = CRAFT_SIZE / 2;
    group.add(axes);

    // The arm on its mount, at its initial pose: links and the cone end effector, joint to joint.
    if (d.platform === 'RED' && design && design.dock && design.dock.type === 'arm' &&
        lib.models.ARM_LINK) {
      const q = (d.arm && d.arm.q) || [Math.PI / 2, Math.PI / 2, 0];
      const pts = armPoints(lib.info, design.dock, q, d.arm && d.arm.geometry);
      [lib.armLink(false), lib.armLink(false), lib.endEffector(false)].forEach((link, k) => {
        const [x0, y0] = pts[k];
        const dx = pts[k + 1][0] - x0;
        const dy = pts[k + 1][1] - y0;
        link.position.set(x0, y0, 0);
        link.rotation.z = Math.atan2(dy, dx);
        link.scale.x = Math.hypot(dx, dy) / link.userData.length;
        group.add(link);
      });
    }

    // The 6 places: a disc on the face at each (blocked red) with a white ring, just off the
    // face (clear of the panel and the coupler's plate); the chosen one hidden under its cone.
    d.slots = [];
    for (const face of (lib.info.mount && lib.info.mount.faces) || ['+x', '+y', '-y']) {
      for (const side of ['left', 'right']) {
        const slot = { face, side };
        const p = mountPose(lib.info, slot);
        const holder = new THREE.Group();
        holder.position.set(p.x, p.y, p.z);
        holder.rotation.z = p.rot;
        const blocked = ((lib.info.blocked || {})[d.platform] || {})[`${face}|${side}`];
        const chosen = d.selected && d.selected.face === face && d.selected.side === side;
        const disc = new THREE.Mesh(new THREE.CircleGeometry(0.03, 40),
          new THREE.MeshBasicMaterial({
            color: blocked ? 0xff6b6b : 0x74c0fc, transparent: true,
            opacity: d.selected ? 0.7 : 0.4, side: THREE.DoubleSide, depthWrite: false }));
        disc.rotation.y = Math.PI / 2;
        disc.position.x = 0.012;
        disc.visible = !chosen;
        disc.userData.slot = slot;
        const ring = new THREE.Mesh(new THREE.RingGeometry(0.03, 0.036, 40),
          new THREE.MeshBasicMaterial({
            color: 0xffffff, transparent: true, opacity: 0.8, side: THREE.DoubleSide,
            depthWrite: false }));
        ring.rotation.y = Math.PI / 2;
        ring.position.x = 0.0121;
        ring.visible = !chosen;
        holder.add(disc, ring);
        group.add(holder);
        d.slots.push(disc);
      }
    }
    d.group = group;
    this.scene.add(group);
  }

  // Takes the close-up out of the scene, with its body frame's labels (CSS2D elements stay in the
  // page otherwise).
  _dropDesigner() {
    const d = this.designer;
    if (!d.group) {
      return;
    }
    this.scene.remove(d.group);
    d.group.traverse(o => {
      this.bodyAxes.groups.delete(o);
      if (o.element) {
        o.element.remove();
      }
    });
    d.group = null;
  }

  // CAD models are solid, except copies that are not the run being looked at: the simulation
  // laid over a live view (see-through) and loaded results (faint).
  _cadGhost(ghost) {
    return ghost === 'sim' && !this.layers.live ? false : ghost;
  }

  // The cube (and the Chaser's camera housing) or the CAD model (and the ZED2). Copies that are
  // not the run keep the cube's outline over their model: the simulation laid over a live view a
  // solid one (see-through model), a loaded result a dashed one (faint model).
  _applyCadCraft(group) {
    const u = group.userData;
    const on = Boolean(this.cad.on && this.cad.lib);
    const ghost = this._cadGhost(u.ghost);
    const design = this._craftDesign(u);
    const key = JSON.stringify(design);
    if (u.cad && u.cadDesign !== key) {
      group.remove(u.cad);
      u.cad = null;
    }
    if (on && !u.cad) {
      u.cad = this.cad.lib.platform(u.craft, ghost, design);
      u.cadDesign = key;
      group.add(u.cad);
      if (u.housing && !u.zed) {
        u.zed = this.cad.lib.zed(ghost);
        group.add(u.zed);
        u.ownParts.push(u.zed);
      }
      if (u.zed && u.cadGhost !== ghost) {
        this.cad.lib.restyle(u.zed, ghost);
      }
      u.cadGhost = ghost;
    } else if (u.cad && u.cadGhost !== ghost) {
      this.cad.lib.restyle(u.cad, ghost);
      if (u.zed) {
        this.cad.lib.restyle(u.zed, ghost);
      }
      u.cadGhost = ghost;
    }
    if (u.cad) {
      u.cad.visible = on;
    }
    if (u.zed) {
      u.zed.visible = on;
    }
    if (u.housing) {
      u.housing.visible = !on;
    }
    u.box.visible = !on;
    u.edges.visible = !on || Boolean(ghost);
  }

  // The line and joint balls, or the CAD links (shoulder to elbow, elbow to wrist, and the end
  // effector); a loaded result keeps its dashed line.
  _applyCadArm(arm) {
    const on = Boolean(this.cad.on && this.cad.lib);
    const ghost = this._cadGhost(arm.ghost);
    if (on && !arm.cad) {
      const lib = this.cad.lib;
      arm.cad = {
        group: new THREE.Group(),
        links: [lib.armLink(ghost), lib.armLink(ghost), lib.endEffector(ghost)], ghost,
      };
      arm.cad.group.add(...arm.cad.links);
      arm.group.add(arm.cad.group);
      if (arm.pts) {
        this._setArm(arm, arm.pts);
      }
    } else if (arm.cad && arm.cad.ghost !== ghost) {
      for (const link of arm.cad.links) {
        this.cad.lib.restyle(link, ghost);
      }
      arm.cad.ghost = ghost;
    }
    if (arm.cad) {
      arm.cad.group.visible = on;
    }
    arm.line.visible = !on || arm.ghost === 'overlay';
    for (const j of arm.joints) {
      j.visible = !on;
    }
  }

  // A track's motion. result: t, crafts.{NAME}.{x, y, att} (null where unknown), arm [4][2][N];
  // or null.
  _setTrackResult(track, result) {
    track.result = result;
    for (const p of Object.values(track.paths)) {
      track.group.remove(p.done);
      p.done.geometry.dispose();
      p.done.material.dispose();
    }
    track.paths = {};
    for (const [name, g] of Object.entries(track.crafts)) {
      g.visible = Boolean(result && result.crafts[name]);
    }
    track.arm.group.visible = this._armShown(track);
    this._applyCadCraft(track.crafts.RED);
    if (!result) {
      return;
    }
    for (const [name, c] of Object.entries(result.crafts)) {
      // The path skips samples without a position; upto[i] = points drawn up to sample i.
      const pts = [];
      const upto = new Int32Array(c.x.length);
      c.x.forEach((x, i) => {
        if (x != null && c.y[i] != null) {
          pts.push(x, c.y[i], PATH_Z);
        }
        upto[i] = pts.length / 3;
      });
      if (pts.length < 6) {
        continue;
      }
      const make = (opacity, width) => {
        const geo = new LineGeometry();
        geo.setPositions(pts);
        const line = new Line2(geo, new LineMaterial({
          color: new THREE.Color(LINE_COLORS[name]), linewidth: width, transparent: true, opacity,
          dashed: Boolean(track.dash), dashSize: track.dash ? track.dash[0] : 1,
          gapSize: track.dash ? track.dash[1] : 0 }));
        if (track.dash) {
          line.computeLineDistances();
        }
        line.frustumCulled = false;
        return line;
      };

      // Only the path travelled so far: it is traced behind the platform as the replay runs.
      const done = make(0.95, PATH_WIDTH);
      done.visible = this.sim.traces;
      track.group.add(done);
      track.paths[name] = { done, upto };
    }
    this._setTrackTime(track, this.sim.t);
  }

  // A track at time T (less its offset): each platform between its samples, its path up to
  // there, and the arm at the nearer sample.
  _setTrackTime(track, t) {
    const r = track.result;
    if (!r || !track.shown) {
      return;
    }
    t -= track.offset;
    const ts = r.t;
    const n = ts.length;

    // Index of the last sample at or before t (binary search), and how far to the next.
    let lo = 0;
    let hi = n - 1;
    if (t <= ts[0]) {
      hi = 0;
    } else if (t >= ts[n - 1]) {
      lo = hi = n - 1;
    } else {
      while (hi - lo > 1) {
        const mid = (lo + hi) >> 1;
        if (ts[mid] <= t) {
          lo = mid;
        } else {
          hi = mid;
        }
      }
    }
    const i = lo;
    const j = Math.min(hi, n - 1);
    const f = j > i ? (t - ts[i]) / (ts[j] - ts[i]) : 0;
    const lerp = (a, k) => (a[j] == null ? a[i] : a[i] + (a[j] - a[i]) * k);
    for (const [name, c] of Object.entries(r.crafts)) {
      const g = track.crafts[name];
      g.visible = c.x[i] != null && c.y[i] != null;
      if (g.visible) {
        g.position.set(lerp(c.x, f), lerp(c.y, f), 0);
        g.rotation.z = lerp(c.att, f) || 0;
      }
      // Segments up to sample i.
      const p = track.paths[name];
      if (p) {
        p.done.geometry.instanceCount = Math.max(0, p.upto[i] - 1);
      }
    }
    if (r.arm) {
      const k = f < 0.5 ? i : j;
      const pts = [0, 1, 2, 3].map(q => [r.arm[q][0][k], r.arm[q][1][k]]);
      track.arm.group.visible =
        this._armShown(track) && pts.every(([x, y]) => x != null && y != null);
      if (track.arm.group.visible) {
        this._setArm(track.arm, pts);
      }
    }
  }

  /**
   * The primary track: the last simulation.
   * @param {?Object} result interface.results.simulation, or null.
   */
  setSimulation(result) {
    this._setTrackResult(this.sim.primary, result);
    this._checkArms();
  }

  /**
   * Shows or hides the last simulation without dropping it (the overlays stay).
   * @param {boolean} on
   */
  setPrimaryShown(on) {
    const p = this.sim.primary;
    p.shown = on;
    p.group.visible = on;
    if (on) {
      this._setTrackTime(p, this.sim.t);
      this._checkArms();
    }
  }

  /**
   * Loaded results replayed with the simulation: tracks no longer listed (or with a new result)
   * are dropped, new ones built, each with its dash and offset.
   * @param {!Array<{id: string, result: !Object, label: string, offset: number}>} list
   */
  setOverlays(list) {
    const keep = new Set(list.map(o => o.id));
    for (const [id, track] of this.sim.overlays) {
      if (!keep.has(id)) {
        this._disposeTrack(track);
        this.sim.overlays.delete(id);
      }
    }
    list.forEach((o, k) => {
      let track = this.sim.overlays.get(o.id);
      if (track && track.result !== o.result) {
        this._disposeTrack(track);
        track = null;
      }
      if (!track) {
        track = this._buildTrack({
          ghost: 'overlay', label: o.label, dash: OVERLAY_DASHES[k % OVERLAY_DASHES.length],
        });
        this.sim.overlays.set(o.id, track);
        track.offset = o.offset || 0;
        this._setTrackResult(track, o.result);
      }
      track.offset = o.offset || 0;
      this._setTrackTime(track, this.sim.t);
    });
    this._checkArms();
  }

  /**
   * Shows the replayed paths, or not.
   * @param {boolean} on
   */
  setTraces(on) {
    this.sim.traces = on;
    for (const track of this._tracks()) {
      for (const p of Object.values(track.paths)) {
        p.done.visible = on;
      }
    }
  }

  /**
   * Moves every track to time T.
   * @param {number} t [s]
   */
  setSimTime(t) {
    this.sim.t = t;
    this._setTrackTime(this.sim.primary, t);
    for (const track of this.sim.overlays.values()) {
      this._setTrackTime(track, t);
    }
  }

  // Setup layer (initial conditions)

  // A pose on the table: a filled square with its outline and body axes (x red, y green, z blue,
  // as on the platforms), and a level name tag. The name stays level just off the marker's -y
  // side (below it seen from the top, in front of it in 3D), clear of the axes: the outer group
  // moves, only the body inside rotates.
  _makeMarker(name, text, opacity = 0.22) {
    const h = CRAFT_SIZE / 2;
    const color = LINE_COLORS[name];
    const group = new THREE.Group();
    const fill = new THREE.Mesh(new THREE.PlaneGeometry(CRAFT_SIZE, CRAFT_SIZE),
      new THREE.MeshBasicMaterial({ color, transparent: true, opacity, depthWrite: false }));
    fill.position.z = 0.002;
    const outline = new THREE.LineLoop(new THREE.BufferGeometry().setFromPoints([
      new THREE.Vector3(-h, -h, 0.003), new THREE.Vector3(h, -h, 0.003),
      new THREE.Vector3(h, h, 0.003), new THREE.Vector3(-h, h, 0.003)]),
    new THREE.LineBasicMaterial({ color }));
    const axes = this._bodyAxes(0.9);
    axes.position.z = 0.005;
    const tag = label(text, 'craft-label setup', CRAFT_COLORS[name], TEXT_COLORS[name]);
    tag.position.set(0, -h - 0.04, 0.01);
    tag.center.set(0.5, 0);
    const body = new THREE.Group();
    body.add(fill, outline, axes);
    group.add(body, tag);
    group.visible = false;
    return { group, body, fill, tag, att: 0 };
  }

  // A marker per platform, and the arm.
  _buildSetupLayer() {
    this.setup = { group: new THREE.Group(), markers: {}, editable: false, onDrag: null };
    this.setup.group.visible = false;
    this.scene.add(this.setup.group);
    for (const name of Object.keys(CRAFT_COLORS)) {
      const m = this._makeMarker(name, platformName(name));
      this.setup.group.add(m.group);
      this.setup.markers[name] = m;
    }
    this.setup.arm = this._buildArm(
      new THREE.LineBasicMaterial({ color: LINE_COLORS.RED, transparent: true, opacity: 0.8 }));
    this.setup.group.add(this.setup.arm.group);
  }

  /**
   * The initial-condition poses (a marker being dragged keeps its own).
   * @param {{poses: !Object, kind: string, arm: ?Array, editable: boolean}=} opts poses:
   *     { NAME: [x, y, thetaDeg] } for the platforms to show; kind: their tags' suffix; arm:
   *     4 x [x, y] or null; editable: whether they can be dragged.
   */
  setSetup({ poses = {}, kind = '', arm = null, editable = false } = {}) {
    this.setup.editable = editable;
    for (const [name, m] of Object.entries(this.setup.markers)) {
      const p = poses[name];
      m.group.visible = Boolean(p);
      if (!p || (this.drag && this.drag.m === m)) {
        continue;
      }
      m.group.position.set(p[0], p[1], 0);
      m.att = p[2] * Math.PI / 180;
      m.body.rotation.z = m.att;
      m.tag.element.textContent = kind ? `${platformName(name)} · ${kind}` : platformName(name);
    }
    this.setup.arm.group.visible = Boolean(arm);
    if (arm) {
      this._setArm(this.setup.arm, arm);
    }
  }

  // Goal layer (Interactive Mode)

  // Where Interactive Mode sends the live platforms: one marker per platform, dragged like the
  // initial conditions, each with a dashed line from its platform to it. A click on a live
  // platform calls onPick(name) while pickable.
  _buildGoalLayer() {
    this.goal = {
      group: new THREE.Group(), markers: {}, lines: {}, editable: {}, pickable: false,
      margin: CRAFT_SIZE / 2, onDrag: null, onPick: null,
    };
    this.scene.add(this.goal.group);
    for (const name of Object.keys(CRAFT_COLORS)) {
      const m = this._makeMarker(name, `${platformName(name)} · Goal`, 0.34);
      m.tag.element.classList.add('goal');
      this.goal.group.add(m.group);
      this.goal.markers[name] = m;
      const geo =
        new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(), new THREE.Vector3()]);
      const line = new THREE.Line(geo, new THREE.LineDashedMaterial(
        {
          color: LINE_COLORS[name], dashSize: 0.035, gapSize: 0.03, transparent: true,
          opacity: 0.7,
        }));
      line.visible = false;
      line.frustumCulled = false;
      this.goal.group.add(line);
      this.goal.lines[name] = line;
    }
  }

  /**
   * Interactive Mode's goals (a marker being dragged keeps its own).
   * @param {{goals: !Object, pickable: boolean, margin: ?number}=} opts goals: { NAME: { pose:
   *     [x, y, thetaDeg], editable } } for the platforms whose goal to show (editable: it can be
   *     dragged); the others are hidden. pickable: a click on a live platform calls
   *     goal.onPick(name). margin: how far inside the edges a goal stays [m].
   */
  setGoals({ goals = {}, pickable = false, margin = null } = {}) {
    const t = this.goal;
    t.pickable = pickable;
    if (margin != null) {
      t.margin = margin;
    }
    t.editable = {};
    for (const [n, m] of Object.entries(t.markers)) {
      const g = goals[n];
      m.group.visible = Boolean(g && g.pose);
      if (!m.group.visible) {
        continue;
      }
      t.editable[n] = Boolean(g.editable);
      if (this.drag && this.drag.m === m) {
        continue;
      }
      m.group.position.set(g.pose[0], g.pose[1], 0);
      m.att = g.pose[2] * Math.PI / 180;
      m.body.rotation.z = m.att;
    }
    this._updateGoalLine();
  }

  // Each dashed line from a live platform to its goal (both shown).
  _updateGoalLine() {
    const t = this.goal;
    for (const [n, line] of Object.entries(t.lines)) {
      const m = t.markers[n];
      const g = this.crafts[n];
      line.visible = Boolean(m.group.visible && g && g.visible);
      if (!line.visible) {
        continue;
      }
      const pos = line.geometry.attributes.position;
      pos.setXYZ(0, g.position.x, g.position.y, 0.004);
      pos.setXYZ(1, m.group.position.x, m.group.position.y, 0.004);
      pos.needsUpdate = true;
      line.geometry.computeBoundingSphere();
      line.computeLineDistances();
    }
  }

  /**
   * The duty cycles being sent to a live platform, drawn as plumes from its nozzles (longer for
   * a larger duty cycle). Each cone's apex is at the nozzle, opening along its -y, which is
   * turned along the gas's direction.
   * @param {string} name The platform.
   * @param {?Array<number>} duty The 8 duty cycles [%]; null: none.
   */
  setThrust(name, duty) {
    const g = this.crafts[name];
    if (!g) {
      return;
    }
    let plumes = g.userData.plumes;
    if (!plumes && duty) {
      plumes = new THREE.Group();
      const geo = new THREE.ConeGeometry(0.022, 1, 18, 1, true);
      geo.translate(0, -0.5, 0);
      const mat = new THREE.MeshBasicMaterial({
        color: 0xcfe8ff, transparent: true, opacity: 0.6, blending: THREE.AdditiveBlending,
        depthWrite: false, side: THREE.DoubleSide,
      });
      for (const [[x, y], [dx, dy]] of THRUSTER_NOZZLES) {
        const cone = new THREE.Mesh(geo, mat);
        cone.position.set(x, y, NOZZLE_Z);
        cone.rotation.z = Math.atan2(dx, -dy);
        cone.visible = false;
        plumes.add(cone);
      }
      g.add(plumes);
      g.userData.plumes = plumes;
    }
    if (!plumes) {
      return;
    }
    plumes.visible = Boolean(duty);
    if (duty) {
      plumes.children.forEach((c, k) => {
        const d = Math.min(100, Math.max(0, duty[k] || 0));
        c.visible = d > 0.5;
        c.scale.set(1, 0.05 + 0.25 * d / 100, 1);
      });
    }
  }

  // Drag a setup marker or Interactive Mode's goal across the table (Shift+drag rotates it).
  // Runs before OrbitControls (capture phase) so that grabbing a marker doesn't also orbit the
  // camera. Also: the Designer's places, and picking a live platform.
  _bindDrag() {
    const el = this.renderer.domElement;
    const ray = new THREE.Raycaster();
    const ndc = new THREE.Vector2();
    const plane = new THREE.Plane(new THREE.Vector3(0, 0, 1), 0);

    // The ray under the mouse, and where it meets the table top.
    const aim = ev => {
      const r = el.getBoundingClientRect();
      ndc.set(((ev.clientX - r.left) / r.width) * 2 - 1,
        -((ev.clientY - r.top) / r.height) * 2 + 1);
      ray.setFromCamera(ndc, this.camera);
    };
    const onTable = () => ray.ray.intersectPlane(plane, new THREE.Vector3());

    // The draggable marker under the mouse: { kind: 'setup' or 'goal', name, m }.
    const hitMarker = () => {
      const list = [];
      if (this.layers.setup && this.setup.editable) {
        for (const [name, m] of Object.entries(this.setup.markers)) {
          if (m.group.visible) {
            list.push({ kind: 'setup', name, m });
          }
        }
      }
      for (const [name, m] of Object.entries(this.goal.markers)) {
        if (this.goal.editable[name] && m.group.visible) {
          list.push({ kind: 'goal', name, m });
        }
      }
      const hit = list.length ? ray.intersectObjects(list.map(e => e.m.fill), false)[0] : null;
      return hit ? list.find(e => e.m.fill === hit.object) : null;
    };

    // The dragged marker's pose to its layer's onDrag (done: let go).
    const report = done => {
      const { kind, name, m } = this.drag;
      const onDrag = kind === 'goal' ? this.goal.onDrag : this.setup.onDrag;
      onDrag?.(name, m.group.position.x, m.group.position.y, m.att * 180 / Math.PI, done);
    };

    // Pressing on a marker grabs it (moving, or with Shift rotating).
    let down = null;
    el.addEventListener('pointerdown', ev => {
      down = [ev.clientX, ev.clientY];
      if (ev.button !== 0) {
        return;
      }
      aim(ev);
      const e = hitMarker();
      const p = e && onTable();
      if (!p) {
        return;
      }
      ev.stopImmediatePropagation();
      const m = e.m;
      this.drag = {
        kind: e.kind, name: e.name, m, rotate: ev.shiftKey, offset: p.clone().sub(m.group.position),
        att0: m.att, angle0: Math.atan2(p.y - m.group.position.y, p.x - m.group.position.x),
      };
      el.setPointerCapture(ev.pointerId);
      el.style.cursor = 'grabbing';
    }, { capture: true });

    // The Designer's place under the mouse.
    const hitSlot = () => {
      const d = this.designer;
      if (!d.active || !d.slots.length) {
        return null;
      }
      const hit = ray.intersectObjects(d.slots.filter(s => s.visible), false)[0];
      return hit ? hit.object : null;
    };

    // Moving: the Designer's places light up under the mouse; a marker shows it can be grabbed;
    // a grabbed one follows (goals stay as far inside the edges as the lab console keeps them).
    el.addEventListener('pointermove', ev => {
      aim(ev);
      if (this.designer.active) {
        const s = hitSlot();
        const d = this.designer;
        if (d.hover !== s) {
          if (d.hover) {
            d.hover.scale.setScalar(1);
          }
          if (s) {
            s.scale.setScalar(1.25);
          }
          d.hover = s;
        }
        el.style.cursor = s ? 'pointer' : '';
        return;
      }
      if (!this.drag) {
        el.style.cursor = hitMarker() ? 'grab' : '';
        return;
      }
      const p = onTable();
      if (!p) {
        return;
      }
      const m = this.drag.m;
      if (this.drag.rotate) {
        m.att = this.drag.att0 +
          Math.atan2(p.y - m.group.position.y, p.x - m.group.position.x) - this.drag.angle0;
        m.body.rotation.z = m.att;
      } else {
        const h = this.drag.kind === 'goal' ? this.goal.margin : CRAFT_SIZE / 2;
        m.group.position.x = Math.min(Math.max(p.x - this.drag.offset.x, h), TABLE.x - h);
        m.group.position.y = Math.min(Math.max(p.y - this.drag.offset.y, h), TABLE.y - h);
      }
      if (this.drag.kind === 'goal') {
        this._updateGoalLine();
      }
      report(false);
    });

    // A click (not a drag): in the Spacecraft Designer, on one of the 16 places; else, on a live
    // platform, picks it for Interactive Mode.
    el.addEventListener('click', ev => {
      if (!down || Math.hypot(ev.clientX - down[0], ev.clientY - down[1]) > 4) {
        return;
      }
      if (this.designer.active) {
        aim(ev);
        const s = hitSlot();
        if (s) {
          this.designer.onSlot?.({ ...s.userData.slot });
        }
        return;
      }
      if (!this.goal.pickable) {
        return;
      }
      aim(ev);
      const shown = Object.values(this.crafts).filter(g => g.visible);
      const hit = shown.length ? ray.intersectObjects(shown, true)[0] : null;
      for (let o = hit && hit.object; o; o = o.parent) {
        if (o.userData.craft && !o.userData.ghost) {
          this.goal.onPick?.(o.userData.craft);
          return;
        }
      }
    });

    // Letting go reports the final pose.
    const end = ev => {
      if (!this.drag) {
        return;
      }
      report(true);
      this.drag = null;
      el.style.cursor = '';
      if (el.hasPointerCapture(ev.pointerId)) {
        el.releasePointerCapture(ev.pointerId);
      }
    };
    el.addEventListener('pointerup', end);
    el.addEventListener('pointercancel', end);
  }

  // Display options

  /** @param {boolean} on Show the LEDs' numbers. */
  setLedLabels(on) {
    this.container.classList.toggle('hide-led-labels', !on);
  }

  /** @param {boolean} on Show the platforms' names. */
  setNames(on) {
    this.container.classList.toggle('hide-names', !on);
  }

  /**
   * The platforms' (and initial-condition markers') own axes; the table's frame stays.
   * @param {boolean} on
   */
  setBodyAxes(on) {
    this.bodyAxes.on = on;
    for (const g of this.bodyAxes.groups) {
      g.visible = on;
    }
  }

  /** @param {boolean} on Draw a line from each LED down to the table. */
  setDropLines(on) {
    this.dropLines = on;
    for (const led of this.leds) {
      led.line.visible = on && led.group.position.z > 0;
    }
  }

  /**
   * The table slope calibration: its height map, jet coloured, laid on the table top (under the
   * grid lines, at 0.0008 m) when shown (setTableSlopeShown); rebuilt only when it changes.
   * @param {?Object} cal The calibration (calibration.js), or null for none.
   */
  setTableSlope(cal) {
    const key = cal ? JSON.stringify([cal.height, cal.centre]) : null;
    if (key === this._slopeKey) {
      return;
    }
    this._slopeKey = key;
    if (this.slopeMesh) {
      this.scene.remove(this.slopeMesh);
      this.slopeMesh.material.map.dispose();
      this.slopeMesh.material.dispose();
      this.slopeMesh.geometry.dispose();
      this.slopeMesh = null;
    }
    if (!cal) {
      return;
    }
    const tex = new THREE.CanvasTexture(heightCanvas(cal));
    tex.colorSpace = THREE.SRGBColorSpace;
    const mat =
      new THREE.MeshBasicMaterial({
        map: tex, transparent: true, opacity: 0.62, depthWrite: false,
      });
    this.slopeMesh = new THREE.Mesh(new THREE.PlaneGeometry(TABLE.x, TABLE.y), mat);
    this.slopeMesh.position.set(TABLE.x / 2, TABLE.y / 2, 0.0005);
    this.slopeMesh.visible = Boolean(this.slopeShown);
    this.scene.add(this.slopeMesh);
  }

  /** @param {boolean} on Show the table slope. */
  setTableSlopeShown(on) {
    this.slopeShown = on;
    if (this.slopeMesh) {
      this.slopeMesh.visible = on;
    }
  }

  // The camera distance that fits a W x H area in view.
  _fitDistance(w, h) {
    const v = THREE.MathUtils.degToRad(this.camera.fov);
    const hfov = 2 * Math.atan(Math.tan(v / 2) * this.camera.aspect);
    return Math.max((h / 2) / Math.tan(v / 2), (w / 2) / Math.tan(hfov / 2));
  }

  // ZED2 Viewpoint (peek window)

  // A small framed window in the corner of the view: title (which RED it rides on), the lens
  // choice, close button, and the camera's picture, 16:9 like the ZED's HD720.
  _buildRedCamWindow() {
    const win = document.createElement('div');
    win.className = 'cam-window';
    win.hidden = true;
    win.innerHTML = '<div class="cam-head"><span class="cam-title"></span>' +
      '<div class="segmented seg-sm cam-lens" role="group" aria-label="Lens">' +
      '<button type="button" data-lens="left" ' +
      'title="The left lens (the ZED SDK\'s reference image)">Left</button>' +
      '<button type="button" data-lens="right" title="The right lens">Right</button>' +
      '<button type="button" data-lens="both" ' +
      'title="Both lenses side by side, left then right">Both</button></div>' +
      '<button type="button" class="cam-close" title="Close the ZED2 Viewpoint" ' +
      'aria-label="Close">✕</button></div>' +
      '<div class="cam-frame"><div class="cam-missing">The Chaser is not on the table</div></div>';
    this.container.append(win);
    Object.assign(this.redCam, {
      win, title: win.querySelector('.cam-title'), frame: win.querySelector('.cam-frame'),
      missing: win.querySelector('.cam-missing'),
    });
    win.querySelector('.cam-close').onclick = () => {
      this.setRedCam(false);
      this.redCam.onClose?.();
    };
    win.querySelector('.cam-lens').onclick = ev => {
      const b = ev.target.closest('[data-lens]');
      if (b) {
        this.setRedCamLens(b.dataset.lens);
        this.redCam.onLens?.(b.dataset.lens);
      }
    };
    this.setRedCamLens('left');
    new ResizeObserver(() => this._resizeRedCam()).observe(this.redCam.frame);
  }

  // The window's renderer at its size; each image keeps the ZED2's 16:9 and vertical field of
  // view (two side by side for both lenses).
  _resizeRedCam() {
    const r = this.redCam.renderer;
    const w = this.redCam.frame.clientWidth;
    const h = this.redCam.frame.clientHeight;
    if (!r || !w || !h) {
      return;
    }
    r.setSize(w, h);
    const aspect = (this.redCam.lens === 'both' ? w / 2 : w) / h;
    for (const c of Object.values(this.redCam.cameras)) {
      c.aspect = aspect;
      c.updateProjectionMatrix();
    }
  }

  /**
   * The lens the ZED2 Viewpoint shows.
   * @param {string} lens 'left', 'right' or 'both' (side by side).
   */
  setRedCamLens(lens) {
    const rc = this.redCam;
    rc.lens = ['left', 'right', 'both'].includes(lens) ? lens : 'left';
    rc.win.classList.toggle('both', rc.lens === 'both');
    for (const b of rc.win.querySelectorAll('[data-lens]')) {
      b.classList.toggle('active', b.dataset.lens === rc.lens);
    }
    rc.note = '';
    this._resizeRedCam();
  }

  /**
   * Shows or hides the ZED2 Viewpoint window (its renderer is made the first time).
   * @param {boolean} on
   */
  setRedCam(on) {
    const rc = this.redCam;
    rc.on = on;
    rc.win.hidden = !on;
    if (on && !rc.renderer) {
      rc.renderer = new THREE.WebGLRenderer({
        antialias: true,
        preserveDrawingBuffer: this.renderer.getContextAttributes().preserveDrawingBuffer,
      });
      rc.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
      rc.renderer.shadowMap.enabled = true;
      rc.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
      rc.frame.prepend(rc.renderer.domElement);
      this._resizeRedCam();
    }
  }

  // The RED that is shown, in order: live (PhaseSpace), the last simulation, a laid-over result,
  // then the initial-condition marker. Returns the object whose frame is RED's body frame, and
  // where it comes from.
  _redCameraCarrier() {
    const shown = o => {
      for (; o; o = o.parent) {
        if (!o.visible) {
          return false;
        }
      }
      return true;
    };
    const candidates = [
      [this.crafts.RED, 'Live'], [this.sim.primary.crafts.RED, 'Simulation'],
      ...[...this.sim.overlays.values()].map(t => [t.crafts.RED, t.label || 'Results']),
      [this.setup.markers.RED.body,
        this.setup.markers.RED.tag.element.textContent.split(' · ')[1] || 'Initial Conditions'],
    ];
    for (const [obj, source] of candidates) {
      if (shown(obj)) {
        return { obj, source };
      }
    }
    return null;
  }

  // Places the camera at RED's mount point and draws its picture into the window. RED's own
  // axes, tag and camera housing are hidden meanwhile (they would sit in front of the lens).
  _renderRedCam() {
    const rc = this.redCam;
    if (!rc.renderer || !rc.frame.clientWidth) {
      return;
    }

    // The title: which RED, which lens; nothing to show without a RED.
    const carrier = this._redCameraCarrier();
    const which = { left: 'Left Lens', right: 'Right Lens', both: 'Both Lenses' }[rc.lens];
    const note = carrier
      ? `ZED2 Viewpoint · ${platformName('RED')} ${carrier.source} · ${which}`
      : 'ZED2 Viewpoint';
    if (note !== rc.note) {
      rc.note = note;
      rc.title.textContent = note;
      rc.missing.hidden = Boolean(carrier);
      rc.renderer.domElement.style.visibility = carrier ? '' : 'hidden';
    }
    if (!carrier) {
      return;
    }

    // Each lens's camera at the lens's centre (just ahead of the glass), looking along RED's
    // +x, z up.
    const { obj } = carrier;
    obj.updateWorldMatrix(true, false);
    const lenses =
      (this.cad.lib && this.cad.lib.info.zed && this.cad.lib.info.zed.lenses) || ZED_LENSES;
    for (const side of ['left', 'right']) {
      const [x, y, z] = lenses[side];
      const cam = rc.cameras[side];
      cam.position.copy(obj.localToWorld(new THREE.Vector3(x + 0.001, y, z)));
      cam.up.set(0, 0, 1).applyQuaternion(obj.getWorldQuaternion(new THREE.Quaternion()));
      cam.lookAt(obj.localToWorld(new THREE.Vector3(x + 1, y, z)));
    }

    // Draw one lens, or both side by side.
    const hide = (obj.userData.ownParts || []).filter(o => o.visible);
    for (const o of hide) {
      o.visible = false;
    }
    const r = rc.renderer;
    const size = r.getSize(new THREE.Vector2());
    if (rc.lens === 'both') {
      r.setScissorTest(true);
      for (const [k, side] of [[0, 'left'], [1, 'right']]) {
        r.setViewport(k * size.x / 2, 0, size.x / 2, size.y);
        r.setScissor(k * size.x / 2, 0, size.x / 2, size.y);
        r.render(this.scene, rc.cameras[side]);
      }
      r.setScissorTest(false);
      r.setViewport(0, 0, size.x, size.y);
    } else {
      r.render(this.scene, rc.cameras[rc.lens]);
    }
    for (const o of hide) {
      o.visible = true;
    }
  }

  /**
   * Moves the camera to a view of the table.
   * @param {string} name 'top' (straight down; with +z up and a right-handed frame, +x points
   *     right and +y up the screen), 'side', or else 3D (from the y = 0 side, raised and a
   *     little toward the origin, so the origin corner is in view).
   */
  setView(name) {
    const target = new THREE.Vector3(TABLE.x / 2, TABLE.y / 2, 0);
    if (name === 'top') {
      this.camera.position.set(target.x, target.y - 0.001,
        this._fitDistance(TABLE.x + 0.7, TABLE.y + 0.7));
    } else if (name === 'side') {
      target.z = 0.15;
      this.camera.position.set(target.x,
        target.y - TABLE.y / 2 - this._fitDistance(TABLE.x + 0.8, 1.4), 0.4);
    } else {
      target.set(TABLE.x / 2 - 0.1, TABLE.y / 2 - 0.15, -0.15);
      const dir = new THREE.Vector3(-0.3, -0.8, 0.62).normalize();
      this.camera.position.copy(target)
        .addScaledVector(dir, this._fitDistance(TABLE.x + 0.6, TABLE.y + 0.6) * 1.4);
    }
    this.controls.target.copy(target);
    this.controls.update();
  }
}
