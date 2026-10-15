# cad.js

**File:** `+spot/+console/webapp/static/cad.js` · **Models built by:** `designs/cad_tools/build_cad_models.py` (beside the CAD it reads)

<!-- source: +spot/+console/webapp/static/cad.js -->

## What it is for

The platforms' CAD models for the 3D view's *Use CAD* switch. `models/spot_cad.bin.gz` holds, from the lab's SolidWorks assemblies: the Chaser (with the arm's shoulder motor), the Target (with the dock's cone), the Obstacle, one arm link, the ZED2, each in its body frame (metres, z up, origin at the centre of the footprint on the table top), split by material, with their internal structure; plus the Designer's attachments (docking cones, the arm mount, the solar array) and per-face side panels with or without the cone's cutout. The panels get crinkled MLI foil made here (silver on the Chaser, gold on the Target); the Obstacle's are blue plexiglass over its frame.

## Function by function

**Lines 14–36.** The model files; the solar panels' backsheet colour; each platform's panel finish; the opacity of ghost copies (the simulation over a live view, loaded results over a replay); the stand-in end-effector length; the faces' outward angles.

**Lines 41–45.** `mountPose(info, slot)`: where a docking cone's mount frame sits on the body for a face and side (from the CAD's measured coupler place, mirrored).

**Lines 51–70.** `armPoints(info, place, q, geometry)`: the arm's four joint points in the body frame for a mount place and joint angles, placing the shoulder exactly as [[spot.plotting.armPosition|spot-plotting-armPosition]] and [[spot.arm.mountGeometry|spot-arm-mountGeometry]] do (the settings' `Phi`, `b0` translated into the usual mount's frame).

**Lines 72–79.** `loadCad`: fetch and gunzip the file (`DecompressionStream`), load the gauge texture, build the library.

**Lines 81–106.** `CadLibrary` constructor: check the `SPOTCAD1` magic, parse the JSON header, and make a `BufferGeometry` per mesh pointing straight into the binary (positions, byte normals, optional uv, 16- or 32-bit indices).

**Lines 112–145.** `platform(name, ghost, design)`: the body, the side panels with the cutout only where the cone is, the dock model (male cone, female cup, or the arm mount) at its mount pose, and the solar array turned for its axis.

**Lines 147–184.** `zed`, `armLink` (scaled to the link's length by the viewer) and `endEffector` (the male cone model on a bracket from the wrist, or a stand-in bracket and white tip).

**Lines 186–206.** `_group` (meshes sharing the geometry, with shadows unless ghost or clear) and `restyle` (redraw a model as another ghost level).

**Lines 210–267.** Materials, cached per key/platform/ghost: panel (MLI with the normal and roughness maps, or clear plexiglass), aluminium, frame, puck, dark, black, white, gauge, solar cells (textured), solar backsheet (slightly self-lit), PLA, steel nozzles, screws and their recesses, the solar frame, the table's paint, the ZED2's case and glass.

**Lines 269–284.** The solar-cell texture loaded once; which materials take no shadows (the panels, which the ceiling light would streak); clear materials cast none.

**Lines 288–377.** The MLI textures: a seeded random generator; tilted Voronoi facets at three scales plus 320 creases (mostly short, some long) accumulated into slopes; a normal map and a roughness map as canvas textures, tileable.

**Lines 379–404.** `studioEnvironment`: an equirectangular picture of a dim lab with soft boxes, what the metals reflect.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/cad.js</b> (404 lines; click to show)</summary>

```javascript
   1  // The platforms' CAD models for the 3D view (the "Use CAD" switch).
   2  // models/spot_cad.bin.gz is made by build_cad_models.py (../designs/cad_tools) from the lab's SolidWorks
   3  // assemblies in ../designs (the file format and what each model holds are described there): the Chaser (RED,
   4  // with the arm's shoulder motor), the Target (BLACK, with the dock's cone), the Obstacle (BLUE),
   5  // one arm link and the ZED2, each in its body frame (metres, z up, origin at the centre of the
   6  // footprint on the table top), split by material. The panels get crinkled MLI foil made here,
   7  // silver on the Chaser and gold on the Target; the Obstacle's are blue plexiglass over its
   8  // aluminium frame and decks. Every model has its internal structure (decks, rods, brackets).
   9  // The Spacecraft Designer's attachments (docking cones, solar panels) are separate models, placed on
  10  // each platform from its design (spot_console/design.py) by platform().
  11  
  12  import * as THREE from 'three';
  13  
  14  const MODEL_URL = '/static/models/spot_cad.bin.gz';
  15  export const TABLE_URL = '/static/models/spot_table.bin.gz';   // the table's barrier (same format)
  16  const GAUGE_URL = '/static/models/gauge.png';
  17  // The solar panels, as the lab's (its photo): a white backsheet round the cell field, whose texture
  18  // (9 x 8 black cells with silver lines between them) build_cad_models.py draws
  19  const SOLAR_BORDER = 0xeeeeea;
  20  const SOLAR_CELLS_URL = '/static/models/solar_cells.png';
  21  
  22  // The panels' finish on each platform: MLI on the Chaser (silver) and the Target (gold), transparent
  23  // blue plexiglass on the Obstacle (its frame and decks show through)
  24  const PANELS = {
  25    RED:   { mli: true, color: 0xe4e7eb },
  26    BLACK: { mli: true, color: 0xf0b25a },
  27    BLUE:  { mli: false, color: 0x3d8fe0, opacity: 0.24 },
  28  };
  29  // Copies that are not the run itself: the simulation laid over a live view ('sim') and loaded
  30  // results laid over a replay ('overlay')
  31  const OPACITY = { sim: 0.5, overlay: 0.14 };
  32  // The CAD arm ends at the wrist motor, so the end effector is a stand-in: a bracket and a white
  33  // tip, a3 + b3 long (spot.params.defaults)
  34  const EE_LENGTH = 0.06210 + 0.02515;
  35  // The face a docking cone is on, as the angle of its outward normal about +z
  36  const FACE_ANGLE = { '+x': 0, '+y': Math.PI / 2, '-x': Math.PI, '-y': -Math.PI / 2 };
  37  
  38  // Where a docking cone's mount frame sits on the body: its face (outward normal at angle a), left or
  39  // right of the face's vertical centre line as seen from outside (left is -y in the mount frame), in
  40  // the upper half. info.mount comes from the CAD (the arm coupler's place, mirrored).
  41  export function mountPose(info, slot) {
  42    const m = info.mount, a = FACE_ANGLE[slot.face] ?? 0;
  43    const lat = (slot.side === 'left' ? -1 : 1) * m.lateral;
  44    return { x: m.face * Math.cos(a) - lat * Math.sin(a), y: m.face * Math.sin(a) + lat * Math.cos(a), z: m.upper, rot: a };
  45  }
  46  
  47  // The arm's joints in the body frame, [[x, y] x 4] (shoulder, elbow, wrist, end effector tip), for the
  48  // arm on its mount at place {face, side}, at joint angles q [rad] (each 0 straight out, the shoulder
  49  // out from its face), as spot.plotting.armPosition and spot.arm.mountGeometry place it. geometry: the
  50  // settings' arm (Phi, b0 for the usual mount, a1..b3), or null for the CAD's
  51  export function armPoints(info, place, q, geometry = null) {
  52    const g = geometry || { a1: 0, b1: info.arm.linkLength, a2: 0, b2: info.arm.linkLength, a3: 0, b3: info.arm.eeLength || EE_LENGTH };
  53    let sm = info.arm.shoulderMount || [0.09, 0];
  54    if (geometry && geometry.b0 != null && geometry.Phi != null) {
  55      // the shoulder in the usual mount's frame (+y face, left)
  56      const o = mountPose(info, { face: '+y', side: 'left' });
  57      const sx = geometry.b0 * Math.cos(geometry.Phi) - o.x, sy = geometry.b0 * Math.sin(geometry.Phi) - o.y;
  58      sm = [sy, -sx];
  59    }
  60    const p = mountPose(info, place), c = Math.cos(p.rot), s = Math.sin(p.rot);
  61    const pts = [[p.x + c * sm[0] - s * sm[1], p.y + s * sm[0] + c * sm[1]]];
  62    const lengths = [g.a1 + g.b1, g.a2 + g.b2, g.a3 + g.b3];
  63    let dir = p.rot;
  64    for (let k = 0; k < 3; k++) {
  65      dir += q[k];
  66      const [x, y] = pts[k];
  67      pts.push([x + lengths[k] * Math.cos(dir), y + lengths[k] * Math.sin(dir)]);
  68    }
  69    return pts;
  70  }
  71  
  72  export async function loadCad(url = MODEL_URL) {
  73    const res = await fetch(url, { cache: 'no-cache' });
  74    if (!res.ok) throw new Error(`CAD models not found (HTTP ${res.status}); they are built by build_cad_models.py in designs/cad_tools`);
  75    if (typeof DecompressionStream === 'undefined') throw new Error('This browser cannot unpack the CAD models (no DecompressionStream)');
  76    const buffer = await new Response(res.body.pipeThrough(new DecompressionStream('gzip'))).arrayBuffer();
  77    const gauge = await new THREE.TextureLoader().loadAsync(GAUGE_URL).catch(() => null);
  78    return new CadLibrary(buffer, gauge);
  79  }
  80  
  81  class CadLibrary {
  82    constructor(buffer, gauge) {
  83      const text = new TextDecoder();
  84      if (text.decode(new Uint8Array(buffer, 0, 8)) !== 'SPOTCAD1') throw new Error('Not a SPOT CAD model file');
  85      const n = new DataView(buffer).getUint32(8, true);
  86      const header = JSON.parse(text.decode(new Uint8Array(buffer, 12, n)));
  87      const base = 12 + n;
  88      this.info = header.info;
  89      this.models = {};
  90      for (const [name, model] of Object.entries(header.models)) {
  91        this.models[name] = model.meshes.map(m => {
  92          const g = new THREE.BufferGeometry();
  93          g.setAttribute('position', new THREE.BufferAttribute(new Float32Array(buffer, base + m.position, 3 * m.vertices), 3));
  94          g.setAttribute('normal', new THREE.BufferAttribute(new Int8Array(buffer, base + m.normal, 3 * m.vertices), 3, true));
  95          if (m.uv != null) g.setAttribute('uv', new THREE.BufferAttribute(new Float32Array(buffer, base + m.uv, 2 * m.vertices), 2));
  96          const Index = m.indexType === 'uint16' ? Uint16Array : Uint32Array;
  97          g.setIndex(new THREE.BufferAttribute(new Index(buffer, base + m.index, 3 * m.triangles), 1));
  98          g.computeBoundingSphere();
  99          return { material: m.material, geometry: g };
 100        });
 101      }
 102      if (gauge) gauge.colorSpace = THREE.SRGBColorSpace;
 103      this.gauge = gauge;
 104      this.materials = new Map();
 105      this.textures = null;
 106    }
 107  
 108    // ---------------------------------------------------------------- models
 109    // Each returns a group in the body frame; its meshes share the geometry (don't dispose it).
 110    // ghost: false (solid), 'sim' (the simulation's see-through copy) or 'overlay' (a loaded result)
 111  
 112    platform(name, ghost, design) {
 113      // The platform with its attachments from design ({dock, solar}, see design.py); without a design,
 114      // as built (the Chaser with its arm mount)
 115      const group = this._group(name, name, ghost);
 116      const dock = design && design.dock, solar = design && design.solar;
 117      const docked = dock && dock.type !== 'none';
 118      // The side panels: each face's has the docking cone's cutout only where the cone is (nothing is
 119      // left at the cone's usual place when it moves; the arm's coupler and shoulder aren't drawn)
 120      for (const face of Object.keys(FACE_ANGLE)) {
 121        const variant = docked && dock.face === face ? dock.side : 'none';
 122        const key = `${name}_PANEL_${face}_${variant}`;
 123        if (!this.models[key]) continue;
 124        const panel = this._group(key, name, ghost);
 125        panel.userData.attachment = 'panel';
 126        group.add(panel);
 127      }
 128      if (docked && this.info.mount) {
 129        // the arm sits on the female cup's plate (its links and cone end effector follow its joints)
 130        const model = { male: 'DOCK_MALE', female: 'DOCK_FEMALE', arm: this.models.ARM_MOUNT ? 'ARM_MOUNT' : 'DOCK_FEMALE' }[dock.type];
 131        const g = this._group(model || 'DOCK_FEMALE', name, ghost);
 132        const p = mountPose(this.info, dock);
 133        g.position.set(p.x, p.y, p.z);
 134        g.rotation.z = p.rot;
 135        g.userData.attachment = 'dock';
 136        group.add(g);
 137      }
 138      if (solar && solar.on && this.models.SOLAR) {
 139        const g = this._group('SOLAR', name, ghost);
 140        g.rotation.z = solar.axis === 'y' ? Math.PI / 2 : 0;
 141        g.userData.attachment = 'solar';
 142        group.add(g);
 143      }
 144      return group;
 145    }
 146  
 147    zed(ghost) { return this._group('ZED', 'RED', ghost); }
 148  
 149    armLink(ghost) {
 150      // From the proximal joint along +x; scale x to the link's length over userData.length
 151      const g = this._group('ARM_LINK', 'RED', ghost);
 152      g.userData.length = this.info.arm.linkLength;
 153      return g;
 154    }
 155  
 156    endEffector(ghost) {
 157      // The male docking cone (ARM_EE), from the wrist joint along +x at the joints' height, on a bracket
 158      // from the wrist to the cone's flange; without the model, a stand-in (see EE_LENGTH)
 159      const g = new THREE.Group();
 160      const h = this.info.arm.jointHeight;
 161      if (this.models.ARM_EE) {
 162        const start = this.info.arm.coneStart;
 163        g.add(this._group('ARM_EE', 'RED', ghost));
 164        if (start > 0.004) {
 165          const bracket = new THREE.Mesh(new THREE.BoxGeometry(start, 0.03, 0.034), this.material('black', 'RED', ghost));
 166          bracket.position.set(start / 2, 0, h);
 167          Object.assign(bracket.userData, { material: 'black', platform: 'RED' });
 168          bracket.castShadow = bracket.receiveShadow = !ghost;
 169          g.add(bracket);
 170        }
 171        g.userData.length = this.info.arm.eeLength;
 172        return g;
 173      }
 174      const split = 0.04;
 175      const bracket = new THREE.Mesh(new THREE.BoxGeometry(split, 0.032, 0.036), this.material('black', 'RED', ghost));
 176      bracket.position.set(split / 2, 0, h);
 177      bracket.userData.material = 'black';
 178      const tip = new THREE.Mesh(new THREE.BoxGeometry(EE_LENGTH - split, 0.024, 0.03), this.material('white', 'RED', ghost));
 179      tip.position.set((split + EE_LENGTH) / 2, 0, h);
 180      tip.userData.material = 'white';
 181      for (const m of [bracket, tip]) { m.castShadow = !ghost; m.receiveShadow = !ghost; m.userData.platform = 'RED'; g.add(m); }
 182      g.userData.length = EE_LENGTH;
 183      return g;
 184    }
 185  
 186    _group(model, platform, ghost) {
 187      const group = new THREE.Group();
 188      for (const { material, geometry } of this.models[model] || []) {
 189        const mesh = new THREE.Mesh(geometry, this.material(material, platform, ghost));
 190        mesh.castShadow = !ghost && !clear(mesh.material);
 191        mesh.receiveShadow = !ghost && !NO_SHADOW.has(material);
 192        Object.assign(mesh.userData, { sharedGeometry: true, material, platform });
 193        group.add(mesh);
 194      }
 195      return group;
 196    }
 197  
 198    restyle(group, ghost) {
 199      // Draws a model made by one of the functions above as another ghost (or solid: ghost false)
 200      group.traverse(o => {
 201        if (!o.isMesh || !o.userData.material) return;
 202        o.material = this.material(o.userData.material, o.userData.platform, ghost);
 203        o.castShadow = !ghost && !clear(o.material);
 204        o.receiveShadow = !ghost && !NO_SHADOW.has(o.userData.material);
 205      });
 206    }
 207  
 208    // ---------------------------------------------------------------- materials
 209  
 210    material(key, platform, ghost) {
 211      const id = `${key}|${platform}|${ghost || ''}`;
 212      let m = this.materials.get(id);
 213      if (!m) {
 214        m = this._material(key, platform);
 215        if (ghost) {
 216          const opacity = OPACITY[ghost] ?? 0.5;
 217          Object.assign(m, { opacity: m.transparent ? Math.min(m.opacity, opacity) : opacity, transparent: true,
 218                             depthWrite: !m.transparent && ghost !== 'overlay' });
 219        }
 220        this.materials.set(id, m);
 221      }
 222      return m;
 223    }
 224  
 225    _material(key, platform) {
 226      const t = this.textures ||= { ...mliTextures(1024), env: studioEnvironment() };
 227      const std = o => new THREE.MeshStandardMaterial({ envMap: t.env, ...o });
 228      switch (key) {
 229        case 'panel': {
 230          const p = PANELS[platform] || PANELS.BLUE;
 231          return p.mli
 232            ? std({ color: p.color, metalness: 1, roughness: 1, roughnessMap: t.rough, normalMap: t.normal, envMapIntensity: 1.25 })
 233            : std({ color: p.color, metalness: 0, roughness: 0.04, envMapIntensity: 1.6, transparent: true,
 234                    opacity: p.opacity, depthWrite: false, side: THREE.DoubleSide, userData: { clear: true } });
 235        }
 236        case 'aluminium': return std({ color: 0xc6cad0, metalness: 1, roughness: 0.42, envMapIntensity: 0.9 });
 237        // The Obstacle's frame (corner brackets, top ring), seen through clear panels: less mirror-like
 238        // than 'aluminium', so its sides facing the dark half of the room stay light
 239        case 'frame': return std({ color: 0xd3d7dd, metalness: 0.55, roughness: 0.32, envMapIntensity: 1.0 });
 240        case 'puck': return std({ color: 0x2862c8, metalness: 0.6, roughness: 0.35, envMapIntensity: 0.8 });   // blue anodised
 241        case 'dark': return std({ color: 0x1a1c20, metalness: 0.3, roughness: 0.3, envMapIntensity: 0.8 });
 242        case 'black': return std({ color: 0x2a2c31, metalness: 0.5, roughness: 0.55, envMapIntensity: 0.6 });
 243        case 'white': return std({ color: 0xf0efea, metalness: 0, roughness: 0.65, envMapIntensity: 0.4 });   // 3D print
 244        case 'gauge': return std({ color: 0xffffff, map: this.gauge, metalness: 0, roughness: 0.25, envMapIntensity: 0.5 });
 245        // The solar panels on the frames (the CAD has the frames only): real cells, from a texture
 246        case 'solar': return std({ map: this._solarCells(), color: 0xffffff, metalness: 0.15, roughness: 0.28, envMapIntensity: 1.2 });
 247        // the panels' white backsheet round the cells
 248        // (a little self-lit, so it reads white on the panels' vertical faces under the ceiling light)
 249        case 'solarback': return std({ color: SOLAR_BORDER, emissive: 0x9c9c98, metalness: 0, roughness: 0.45, envMapIntensity: 0.8 });
 250        // 3D-printed black PLA (the thrusters' inserts, the LED mounts)
 251        case 'pla': return std({ color: 0x141518, metalness: 0, roughness: 0.78, envMapIntensity: 0.3 });
 252        // stainless steel (1.4404): the Silvent MJ4 nozzles
 253        case 'steel': return std({ color: 0xd2d5da, metalness: 1, roughness: 0.22, envMapIntensity: 1.1 });
 254        // the screws: countersunk Phillips, matte black, flush with the surface, with their cross recess
 255        case 'screw': return std({ color: 0x1a1b1e, metalness: 0.3, roughness: 0.62, envMapIntensity: 0.4 });
 256        case 'recess': return std({ color: 0x030304, metalness: 0, roughness: 0.95, envMapIntensity: 0.1 });
 257        // the solar array's metal (bracket, standoffs, frames, eyelets): aluminium, as the Obstacle's frame
 258        case 'solarframe': return std({ color: 0xd3d7dd, metalness: 0.55, roughness: 0.32, envMapIntensity: 1.0 });
 259        // The table's barrier, painted glossy black (its drawing)
 260        case 'paint': return std({ color: 0x15171a, metalness: 0.1, roughness: 0.2, envMapIntensity: 0.9 });
 261        // The ZED2: its graphite housing and the black glass over its lenses
 262        case 'zedcase': return std({ color: 0x33373d, metalness: 0.55, roughness: 0.42, envMapIntensity: 0.8 });
 263        case 'glass': return std({ color: 0x050607, metalness: 0, roughness: 0.04, envMapIntensity: 1.6 });
 264        default: return std({ color: 0x9aa0a8, metalness: 0.5, roughness: 0.5 });
 265      }
 266    }
 267  }
 268  
 269  CadLibrary.prototype._solarCells = function () {
 270    // loaded once, shared by every panel (the model's uv spans the cell field, 0..1)
 271    if (!this.solar) {
 272      this.solar = new THREE.TextureLoader().load(SOLAR_CELLS_URL);
 273      this.solar.colorSpace = THREE.SRGBColorSpace;
 274      this.solar.anisotropy = 8;
 275    }
 276    return this.solar;
 277  };
 278  
 279  // The solar panels don't take shadows: the ceiling light grazes them, and the frame's rail and brace
 280  // just above would streak them (shadow-map artefacts) and grey them out
 281  const NO_SHADOW = new Set(['solar', 'solarback']);
 282  
 283  // See-through plexiglass lets the light through, so it casts no shadow
 284  const clear = m => Boolean(m.userData.clear);
 285  
 286  // ---------------------------------------------------------------- textures
 287  
 288  function random(seed) {
 289    // Small seeded generator (mulberry32), so the foil looks the same every time
 290    return () => {
 291      seed = (seed + 0x6D2B79F5) | 0;
 292      let t = Math.imul(seed ^ (seed >>> 15), 1 | seed);
 293      t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
 294      return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
 295    };
 296  }
 297  
 298  function mliTextures(size) {
 299    // Crinkled foil, tileable (one tile per 0.3 m of panel): mostly folds (creases 1-15 cm long:
 300    // the surface slopes one way on one side and the other way on the other), over gently tilted
 301    // facets of three sizes (Voronoi cells). Returns the normal map and a roughness map (green).
 302    const rnd = random(20261009);
 303    const gauss = () => Math.sqrt(-2 * Math.log(rnd() + 1e-12)) * Math.cos(2 * Math.PI * rnd());
 304    const N = size * size;
 305    const sx = new Float32Array(N), sy = new Float32Array(N), rough = new Float32Array(N).fill(0.3);
 306  
 307    for (const [cells, tilt, sheen] of [[5, 0.07, true], [16, 0.05, false], [44, 0.02, false]]) {
 308      const n2 = cells * cells;
 309      const px = new Float32Array(n2), py = new Float32Array(n2), ax = new Float32Array(n2), ay = new Float32Array(n2), r = new Float32Array(n2);
 310      for (let k = 0; k < n2; k++) {
 311        px[k] = (k % cells + rnd()) / cells;
 312        py[k] = (Math.floor(k / cells) + rnd()) / cells;
 313        ax[k] = gauss() * tilt; ay[k] = gauss() * tilt; r[k] = rnd();
 314      }
 315      for (let j = 0; j < size; j++) {
 316        const v = (j + 0.5) / size, cy = Math.floor(v * cells);
 317        for (let i = 0; i < size; i++) {
 318          const u = (i + 0.5) / size, cx = Math.floor(u * cells);
 319          let best = Infinity, bk = 0;
 320          for (let dy = -1; dy <= 1; dy++) {
 321            let gy = cy + dy, oy = 0;
 322            if (gy < 0) { gy += cells; oy = -1; } else if (gy >= cells) { gy -= cells; oy = 1; }
 323            for (let dx = -1; dx <= 1; dx++) {
 324              let gx = cx + dx, ox = 0;
 325              if (gx < 0) { gx += cells; ox = -1; } else if (gx >= cells) { gx -= cells; ox = 1; }
 326              const k = gy * cells + gx, ex = px[k] + ox - u, ey = py[k] + oy - v, d = ex * ex + ey * ey;
 327              if (d < best) { best = d; bk = k; }
 328            }
 329          }
 330          const p = j * size + i;
 331          sx[p] += ax[bk]; sy[p] += ay[bk];
 332          if (sheen) rough[p] = 0.24 + 0.14 * r[bk];
 333        }
 334      }
 335    }
 336  
 337    for (let f = 0; f < 320; f++) {
 338      // many short creases, fewer long wrinkles
 339      const long = rnd() < 0.25;
 340      const cx = rnd(), cy = rnd(), a = rnd() * Math.PI, half = long ? 0.12 + rnd() * 0.25 : 0.02 + rnd() * 0.08;
 341      const width = long ? 0.006 + rnd() * 0.02 : 0.002 + rnd() * 0.006;
 342      const amp = (long ? 0.12 + rnd() * 0.2 : 0.1 + rnd() * 0.25) * (rnd() < 0.5 ? -1 : 1);
 343      const dx = Math.cos(a), dy = Math.sin(a);
 344      const ex = Math.abs(dx) * half + 4 * width, ey = Math.abs(dy) * half + 4 * width;
 345      const i0 = Math.floor((cx - ex) * size), i1 = Math.ceil((cx + ex) * size);
 346      const j0 = Math.floor((cy - ey) * size), j1 = Math.ceil((cy + ey) * size);
 347      for (let j = j0; j <= j1; j++) {
 348        const v = (j + 0.5) / size - cy, row = ((j % size) + size) % size * size;
 349        for (let i = i0; i <= i1; i++) {
 350          const u = (i + 0.5) / size - cx;
 351          const s = u * dx + v * dy, t = -u * dy + v * dx;
 352          if (Math.abs(s) >= half) continue;
 353          const slope = amp * (1 - (s / half) ** 2) * Math.sign(t) * Math.exp(-Math.abs(t) / width);
 354          const p = row + ((i % size) + size) % size;
 355          sx[p] += -dy * slope; sy[p] += dx * slope;
 356        }
 357      }
 358    }
 359  
 360    const make = fill => {
 361      const c = document.createElement('canvas');
 362      c.width = c.height = size;
 363      const g = c.getContext('2d'), img = g.createImageData(size, size), d = img.data;
 364      for (let p = 0; p < N; p++) fill(p, d, 4 * p);
 365      g.putImageData(img, 0, 0);
 366      const tex = new THREE.CanvasTexture(c);
 367      tex.wrapS = tex.wrapT = THREE.RepeatWrapping;
 368      tex.anisotropy = 8;
 369      return tex;
 370    };
 371    const normal = make((p, d, o) => {
 372      const x = -sx[p], y = -sy[p], len = Math.hypot(x, y, 1);
 373      d[o] = (x / len * 0.5 + 0.5) * 255; d[o + 1] = (y / len * 0.5 + 0.5) * 255; d[o + 2] = (1 / len * 0.5 + 0.5) * 255; d[o + 3] = 255;
 374    });
 375    const roughMap = make((p, d, o) => { d[o] = d[o + 1] = d[o + 2] = rough[p] * 255; d[o + 3] = 255; });
 376    return { normal, rough: roughMap };
 377  }
 378  
 379  function studioEnvironment() {
 380    // What the metal reflects: a dim lab with a few bright soft boxes (an equirectangular picture;
 381    // each renderer prefilters it itself, so the ZED2 Viewpoint's renderer can use it too)
 382    const w = 1024, h = 512;
 383    const c = document.createElement('canvas');
 384    c.width = w; c.height = h;
 385    const g = c.getContext('2d');
 386    const sky = g.createLinearGradient(0, 0, 0, h);
 387    sky.addColorStop(0, '#c3ccd8'); sky.addColorStop(0.3, '#9ba3ae'); sky.addColorStop(0.5, '#848a94');
 388    sky.addColorStop(0.56, '#5b6068'); sky.addColorStop(1, '#2c2f34');
 389    g.fillStyle = sky;
 390    g.fillRect(0, 0, w, h);
 391    g.filter = 'blur(8px)';
 392    // ceiling lights, and lit walls around the table (what the side panels see)
 393    for (const [x, y, bw, bh, color] of [[70, 40, 230, 110, '#ffffff'], [560, 25, 320, 90, '#fff3e0'], [380, 140, 120, 60, '#e3edff'],
 394                                         [880, 120, 110, 90, '#ffffff'], [0, 200, 180, 70, '#dfe5ee'], [300, 215, 160, 55, '#f4f1ea'],
 395                                         [620, 205, 200, 65, '#e8edf5'], [900, 215, 124, 50, '#f0ece4']]) {
 396      g.fillStyle = color;
 397      g.fillRect(x, y, bw, bh);
 398    }
 399    g.filter = 'none';
 400    const tex = new THREE.CanvasTexture(c);
 401    tex.mapping = THREE.EquirectangularReflectionMapping;
 402    tex.colorSpace = THREE.SRGBColorSpace;
 403    return tex;
 404  }
```

</details>
<!-- /listing -->
