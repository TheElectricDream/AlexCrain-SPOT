// The platforms' CAD models for the 3D view (the "Use CAD" switch).
// models/spot_cad.bin.gz is made by build_cad_models.py (../designs/cad_tools) from the lab's
// SolidWorks assemblies in ../designs (the file format and what each model holds are described
// there): the Chaser (RED, with the arm's shoulder motor), the Target (BLACK, with the dock's
// cone), the Obstacle (BLUE), one arm link and the ZED2, each in its body frame (metres, z up,
// origin at the centre of the footprint on the table top), split by material. The panels get
// crinkled MLI foil made here, silver on the Chaser and gold on the Target; the Obstacle's are
// blue plexiglass over its aluminium frame and decks. Every model has its internal structure
// (decks, rods, brackets). The Spacecraft Designer's attachments (docking cones, solar panels)
// are separate models, placed on each platform from its design (spot_console/design.py) by
// platform().

import * as THREE from 'three';
import { constant } from './constants.js';

const MODEL_URL = '/static/models/spot_cad.bin.gz';
export const TABLE_URL = '/static/models/spot_table.bin.gz';   // the table's barrier (same format)
const GAUGE_URL = '/static/models/gauge.png';

// The solar panels, as the lab's (its photo): a white backsheet round the cell field, whose
// texture (9 x 8 black cells with silver lines between them) build_cad_models.py draws.
const SOLAR_BORDER = 0xeeeeea;
const SOLAR_CELLS_URL = '/static/models/solar_cells.png';

// The panels' finish on each platform: MLI on the Chaser (silver) and the Target (gold),
// transparent blue plexiglass on the Obstacle (its frame and decks show through).
const PANELS = {
  RED:   { mli: true, color: 0xe4e7eb },
  BLACK: { mli: true, color: 0xf0b25a },
  BLUE:  { mli: false, color: 0x3d8fe0, opacity: 0.24 },
};

// Copies that are not the run itself: the simulation laid over a live view ('sim') and loaded
// results laid over a replay ('overlay').
const OPACITY = { sim: 0.5, overlay: 0.14 };

// The CAD arm ends at the wrist motor, so the end effector is a stand-in: a bracket and a white
// tip, a3 + b3 long (ARM_A3 + ARM_B3) [m].
const eeLength = () => constant('ARM_A3') + constant('ARM_B3');

// The face a docking cone is on, as the angle of its outward normal about +z.
const FACE_ANGLE = { '+x': 0, '+y': Math.PI / 2, '-x': Math.PI, '-y': -Math.PI / 2 };

/**
 * Where a docking cone's mount frame sits on the body: its face (outward normal at angle a),
 * left or right of the face's vertical centre line as seen from outside (left is -y in the
 * mount frame), in the upper half.
 * @param {!Object} info The CAD's info; info.mount is the arm coupler's place (mirrored).
 * @param {{face: string, side: string}} slot The place.
 * @return {{x: number, y: number, z: number, rot: number}} Position [m] and angle [rad].
 */
export function mountPose(info, slot) {
  const m = info.mount;
  const a = FACE_ANGLE[slot.face] ?? 0;
  const lat = (slot.side === 'left' ? -1 : 1) * m.lateral;
  return {
    x: m.face * Math.cos(a) - lat * Math.sin(a), y: m.face * Math.sin(a) + lat * Math.cos(a),
    z: m.upper, rot: a,
  };
}

/**
 * The arm's joints in the body frame (shoulder, elbow, wrist, end effector tip), as
 * spot.plotting.armPosition and spot.arm.mountGeometry place them.
 * @param {!Object} info The CAD's info.
 * @param {{face: string, side: string}} place The arm's mount.
 * @param {!Array<number>} q Joint angles [rad], each 0 straight out (the shoulder out from its
 *     face).
 * @param {?Object=} geometry The settings' arm (Phi, b0 for the usual mount, a1..b3), or null
 *     for the CAD's.
 * @return {!Array<!Array<number>>} [[x, y] x 4] [m].
 */
export function armPoints(info, place, q, geometry = null) {
  const g = geometry || {
    a1: 0, b1: info.arm.linkLength, a2: 0, b2: info.arm.linkLength, a3: 0,
    b3: info.arm.eeLength || eeLength(),
  };

  // The shoulder on its mount: the CAD's, or the settings' (given in the usual mount's frame,
  // the +y face, left).
  let sm = info.arm.shoulderMount || [0.09, 0];
  if (geometry && geometry.b0 != null && geometry.Phi != null) {
    const o = mountPose(info, { face: '+y', side: 'left' });
    const sx = geometry.b0 * Math.cos(geometry.Phi) - o.x;
    const sy = geometry.b0 * Math.sin(geometry.Phi) - o.y;
    sm = [sy, -sx];
  }

  // The shoulder on the body, then each link from the joint before it.
  const p = mountPose(info, place);
  const c = Math.cos(p.rot);
  const s = Math.sin(p.rot);
  const pts = [[p.x + c * sm[0] - s * sm[1], p.y + s * sm[0] + c * sm[1]]];
  const lengths = [g.a1 + g.b1, g.a2 + g.b2, g.a3 + g.b3];
  let dir = p.rot;
  for (let k = 0; k < 3; k++) {
    dir += q[k];
    const [x, y] = pts[k];
    pts.push([x + lengths[k] * Math.cos(dir), y + lengths[k] * Math.sin(dir)]);
  }
  return pts;
}

/**
 * Loads a CAD model file.
 * @param {string=} url The gzipped file.
 * @return {!Promise<!CadLibrary>}
 */
export async function loadCad(url = MODEL_URL) {
  const res = await fetch(url, { cache: 'no-cache' });
  if (!res.ok) {
    throw new Error(`CAD models not found (HTTP ${res.status}); ` +
      'they are built by build_cad_models.py in designs/cad_tools');
  }
  if (typeof DecompressionStream === 'undefined') {
    throw new Error('This browser cannot unpack the CAD models (no DecompressionStream)');
  }
  const buffer =
    await new Response(res.body.pipeThrough(new DecompressionStream('gzip'))).arrayBuffer();
  const gauge = await new THREE.TextureLoader().loadAsync(GAUGE_URL).catch(() => null);
  return new CadLibrary(buffer, gauge);
}

// The models of a CAD file, and the materials they are drawn with.
class CadLibrary {
  // Reads the file: 'SPOTCAD1', the header's length (uint32), the header (JSON: info and each
  // model's meshes), then the meshes' arrays.
  constructor(buffer, gauge) {
    const text = new TextDecoder();
    if (text.decode(new Uint8Array(buffer, 0, 8)) !== 'SPOTCAD1') {
      throw new Error('Not a SPOT CAD model file');
    }
    const n = new DataView(buffer).getUint32(8, true);
    const header = JSON.parse(text.decode(new Uint8Array(buffer, 12, n)));
    const base = 12 + n;
    this.info = header.info;

    // Each mesh: positions, normals (int8), texture coordinates (if any) and triangles.
    this.models = {};
    for (const [name, model] of Object.entries(header.models)) {
      this.models[name] = model.meshes.map(m => {
        const g = new THREE.BufferGeometry();
        g.setAttribute('position', new THREE.BufferAttribute(
          new Float32Array(buffer, base + m.position, 3 * m.vertices), 3));
        g.setAttribute('normal', new THREE.BufferAttribute(
          new Int8Array(buffer, base + m.normal, 3 * m.vertices), 3, true));
        if (m.uv != null) {
          g.setAttribute('uv', new THREE.BufferAttribute(
            new Float32Array(buffer, base + m.uv, 2 * m.vertices), 2));
        }
        const Index = m.indexType === 'uint16' ? Uint16Array : Uint32Array;
        g.setIndex(new THREE.BufferAttribute(
          new Index(buffer, base + m.index, 3 * m.triangles), 1));
        g.computeBoundingSphere();
        return { material: m.material, geometry: g };
      });
    }
    if (gauge) {
      gauge.colorSpace = THREE.SRGBColorSpace;
    }
    this.gauge = gauge;
    this.materials = new Map();
    this.textures = null;
  }

  // Models
  // Each returns a group in the body frame; its meshes share the geometry (don't dispose it).
  // ghost: false (solid), 'sim' (the simulation's see-through copy) or 'overlay' (a loaded
  // result).

  // The platform with its attachments from design ({dock, solar}, see design.py); without a
  // design, as built (the Chaser with its arm mount).
  platform(name, ghost, design) {
    const group = this._group(name, name, ghost);
    const dock = design && design.dock;
    const solar = design && design.solar;
    const docked = dock && dock.type !== 'none';

    // The side panels: each face's has the docking cone's cutout only where the cone is (nothing
    // is left at the cone's usual place when it moves; the arm's coupler and shoulder aren't
    // drawn).
    for (const face of Object.keys(FACE_ANGLE)) {
      const variant = docked && dock.face === face ? dock.side : 'none';
      const key = `${name}_PANEL_${face}_${variant}`;
      if (!this.models[key]) {
        continue;
      }
      const panel = this._group(key, name, ghost);
      panel.userData.attachment = 'panel';
      group.add(panel);
    }

    // The docking cone; the arm sits on the female cup's plate (its links and cone end effector
    // follow its joints).
    if (docked && this.info.mount) {
      const model = {
        male: 'DOCK_MALE', female: 'DOCK_FEMALE',
        arm: this.models.ARM_MOUNT ? 'ARM_MOUNT' : 'DOCK_FEMALE',
      }[dock.type];
      const g = this._group(model || 'DOCK_FEMALE', name, ghost);
      const p = mountPose(this.info, dock);
      g.position.set(p.x, p.y, p.z);
      g.rotation.z = p.rot;
      g.userData.attachment = 'dock';
      group.add(g);
    }

    // The solar panels, along x or y.
    if (solar && solar.on && this.models.SOLAR) {
      const g = this._group('SOLAR', name, ghost);
      g.rotation.z = solar.axis === 'y' ? Math.PI / 2 : 0;
      g.userData.attachment = 'solar';
      group.add(g);
    }
    return group;
  }

  // The ZED2 camera.
  zed(ghost) {
    return this._group('ZED', 'RED', ghost);
  }

  // An arm link, from the proximal joint along +x; scale x to the link's length over
  // userData.length.
  armLink(ghost) {
    const g = this._group('ARM_LINK', 'RED', ghost);
    g.userData.length = this.info.arm.linkLength;
    return g;
  }

  // The male docking cone (ARM_EE), from the wrist joint along +x at the joints' height, on a
  // bracket from the wrist to the cone's flange; without the model, a stand-in (see eeLength).
  endEffector(ghost) {
    const g = new THREE.Group();
    const h = this.info.arm.jointHeight;
    if (this.models.ARM_EE) {
      const start = this.info.arm.coneStart;
      g.add(this._group('ARM_EE', 'RED', ghost));
      if (start > 0.004) {
        const bracket = new THREE.Mesh(new THREE.BoxGeometry(start, 0.03, 0.034),
          this.material('black', 'RED', ghost));
        bracket.position.set(start / 2, 0, h);
        Object.assign(bracket.userData, { material: 'black', platform: 'RED' });
        bracket.castShadow = bracket.receiveShadow = !ghost;
        g.add(bracket);
      }
      g.userData.length = this.info.arm.eeLength;
      return g;
    }

    // The stand-in: a black bracket and a white tip.
    const length = eeLength();
    const split = 0.04;
    const bracket = new THREE.Mesh(new THREE.BoxGeometry(split, 0.032, 0.036),
      this.material('black', 'RED', ghost));
    bracket.position.set(split / 2, 0, h);
    bracket.userData.material = 'black';
    const tip = new THREE.Mesh(new THREE.BoxGeometry(length - split, 0.024, 0.03),
      this.material('white', 'RED', ghost));
    tip.position.set((split + length) / 2, 0, h);
    tip.userData.material = 'white';
    for (const m of [bracket, tip]) {
      m.castShadow = !ghost;
      m.receiveShadow = !ghost;
      m.userData.platform = 'RED';
      g.add(m);
    }
    g.userData.length = length;
    return g;
  }

  // A model as a group of meshes, each with its material for the platform and ghost.
  _group(model, platform, ghost) {
    const group = new THREE.Group();
    for (const { material, geometry } of this.models[model] || []) {
      const mesh = new THREE.Mesh(geometry, this.material(material, platform, ghost));
      mesh.castShadow = !ghost && !clear(mesh.material);
      mesh.receiveShadow = !ghost && !NO_SHADOW.has(material);
      Object.assign(mesh.userData, { sharedGeometry: true, material, platform });
      group.add(mesh);
    }
    return group;
  }

  // Draws a model made by one of the functions above as another ghost (or solid: ghost false).
  restyle(group, ghost) {
    group.traverse(o => {
      if (!o.isMesh || !o.userData.material) {
        return;
      }
      o.material = this.material(o.userData.material, o.userData.platform, ghost);
      o.castShadow = !ghost && !clear(o.material);
      o.receiveShadow = !ghost && !NO_SHADOW.has(o.userData.material);
    });
  }

  // Materials

  // A material, made once per key, platform and ghost; a ghost's is see-through.
  material(key, platform, ghost) {
    const id = `${key}|${platform}|${ghost || ''}`;
    let m = this.materials.get(id);
    if (!m) {
      m = this._material(key, platform);
      if (ghost) {
        const opacity = OPACITY[ghost] ?? 0.5;
        Object.assign(m, {
          opacity: m.transparent ? Math.min(m.opacity, opacity) : opacity, transparent: true,
          depthWrite: !m.transparent && ghost !== 'overlay',
        });
      }
      this.materials.set(id, m);
    }
    return m;
  }

  // A new material for a mesh's material key (the CAD file's names).
  _material(key, platform) {
    const t = this.textures ||= { ...mliTextures(1024), env: studioEnvironment() };
    const std = o => new THREE.MeshStandardMaterial({ envMap: t.env, ...o });
    switch (key) {
      case 'panel': {
        const p = PANELS[platform] || PANELS.BLUE;
        return p.mli
          ? std({
            color: p.color, metalness: 1, roughness: 1, roughnessMap: t.rough,
            normalMap: t.normal, envMapIntensity: 1.25,
          })
          : std({
            color: p.color, metalness: 0, roughness: 0.04, envMapIntensity: 1.6,
            transparent: true, opacity: p.opacity, depthWrite: false, side: THREE.DoubleSide,
            userData: { clear: true },
          });
      }
      case 'aluminium':
        return std({ color: 0xc6cad0, metalness: 1, roughness: 0.42, envMapIntensity: 0.9 });
      // The Obstacle's frame (corner brackets, top ring), seen through clear panels: less
      // mirror-like than 'aluminium', so its sides facing the dark half of the room stay light.
      case 'frame':
        return std({ color: 0xd3d7dd, metalness: 0.55, roughness: 0.32, envMapIntensity: 1.0 });
      // Blue anodised.
      case 'puck':
        return std({ color: 0x2862c8, metalness: 0.6, roughness: 0.35, envMapIntensity: 0.8 });
      case 'dark':
        return std({ color: 0x1a1c20, metalness: 0.3, roughness: 0.3, envMapIntensity: 0.8 });
      case 'black':
        return std({ color: 0x2a2c31, metalness: 0.5, roughness: 0.55, envMapIntensity: 0.6 });
      // 3D print.
      case 'white':
        return std({ color: 0xf0efea, metalness: 0, roughness: 0.65, envMapIntensity: 0.4 });
      case 'gauge':
        return std({
          color: 0xffffff, map: this.gauge, metalness: 0, roughness: 0.25, envMapIntensity: 0.5,
        });
      // The solar panels on the frames (the CAD has the frames only): real cells, from a
      // texture.
      case 'solar':
        return std({
          map: this._solarCells(), color: 0xffffff, metalness: 0.15, roughness: 0.28,
          envMapIntensity: 1.2,
        });
      // The panels' white backsheet round the cells (a little self-lit, so it reads white on the
      // panels' vertical faces under the ceiling light).
      case 'solarback':
        return std({
          color: SOLAR_BORDER, emissive: 0x9c9c98, metalness: 0, roughness: 0.45,
          envMapIntensity: 0.8,
        });
      // 3D-printed black PLA (the thrusters' inserts, the LED mounts).
      case 'pla':
        return std({ color: 0x141518, metalness: 0, roughness: 0.78, envMapIntensity: 0.3 });
      // Stainless steel (1.4404): the Silvent MJ4 nozzles.
      case 'steel':
        return std({ color: 0xd2d5da, metalness: 1, roughness: 0.22, envMapIntensity: 1.1 });
      // The screws: countersunk Phillips, matte black, flush with the surface, with their cross
      // recess.
      case 'screw':
        return std({ color: 0x1a1b1e, metalness: 0.3, roughness: 0.62, envMapIntensity: 0.4 });
      case 'recess':
        return std({ color: 0x030304, metalness: 0, roughness: 0.95, envMapIntensity: 0.1 });
      // The solar array's metal (bracket, standoffs, frames, eyelets): aluminium, as the
      // Obstacle's frame.
      case 'solarframe':
        return std({ color: 0xd3d7dd, metalness: 0.55, roughness: 0.32, envMapIntensity: 1.0 });
      // The table's barrier, painted glossy black (its drawing).
      case 'paint':
        return std({ color: 0x15171a, metalness: 0.1, roughness: 0.2, envMapIntensity: 0.9 });
      // The ZED2: its graphite housing and the black glass over its lenses.
      case 'zedcase':
        return std({ color: 0x33373d, metalness: 0.55, roughness: 0.42, envMapIntensity: 0.8 });
      case 'glass':
        return std({ color: 0x050607, metalness: 0, roughness: 0.04, envMapIntensity: 1.6 });
      default:
        return std({ color: 0x9aa0a8, metalness: 0.5, roughness: 0.5 });
    }
  }
}

// The solar cells' texture, loaded once and shared by every panel (the model's uv spans the
// cell field, 0..1).
CadLibrary.prototype._solarCells = function () {
  if (!this.solar) {
    this.solar = new THREE.TextureLoader().load(SOLAR_CELLS_URL);
    this.solar.colorSpace = THREE.SRGBColorSpace;
    this.solar.anisotropy = 8;
  }
  return this.solar;
};

// The solar panels don't take shadows: the ceiling light grazes them, and the frame's rail and
// brace just above would streak them (shadow-map artefacts) and grey them out.
const NO_SHADOW = new Set(['solar', 'solarback']);

// See-through plexiglass lets the light through, so it casts no shadow.
const clear = m => Boolean(m.userData.clear);

// Textures

// Small seeded generator (mulberry32), so the foil looks the same every time.
function random(seed) {
  return () => {
    seed = (seed + 0x6D2B79F5) | 0;
    let t = Math.imul(seed ^ (seed >>> 15), 1 | seed);
    t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
    return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
  };
}

// Crinkled foil, tileable (one tile per 0.3 m of panel): mostly folds (creases 1-15 cm long: the
// surface slopes one way on one side and the other way on the other), over gently tilted facets
// of three sizes (Voronoi cells). Returns the normal map and a roughness map (green).
function mliTextures(size) {
  const rnd = random(20261009);
  const gauss = () => Math.sqrt(-2 * Math.log(rnd() + 1e-12)) * Math.cos(2 * Math.PI * rnd());
  const N = size * size;
  const sx = new Float32Array(N);
  const sy = new Float32Array(N);
  const rough = new Float32Array(N).fill(0.3);

  // Facets: for each size, random cell centres, each with a random tilt (and, for the largest,
  // a roughness); each pixel takes its nearest centre's (wrapping round the tile).
  for (const [cells, tilt, sheen] of [[5, 0.07, true], [16, 0.05, false], [44, 0.02, false]]) {
    const n2 = cells * cells;
    const px = new Float32Array(n2);
    const py = new Float32Array(n2);
    const ax = new Float32Array(n2);
    const ay = new Float32Array(n2);
    const r = new Float32Array(n2);
    for (let k = 0; k < n2; k++) {
      px[k] = (k % cells + rnd()) / cells;
      py[k] = (Math.floor(k / cells) + rnd()) / cells;
      ax[k] = gauss() * tilt;
      ay[k] = gauss() * tilt;
      r[k] = rnd();
    }
    for (let j = 0; j < size; j++) {
      const v = (j + 0.5) / size;
      const cy = Math.floor(v * cells);
      for (let i = 0; i < size; i++) {
        const u = (i + 0.5) / size;
        const cx = Math.floor(u * cells);
        let best = Infinity;
        let bk = 0;
        for (let dy = -1; dy <= 1; dy++) {
          let gy = cy + dy;
          let oy = 0;
          if (gy < 0) {
            gy += cells;
            oy = -1;
          } else if (gy >= cells) {
            gy -= cells;
            oy = 1;
          }
          for (let dx = -1; dx <= 1; dx++) {
            let gx = cx + dx;
            let ox = 0;
            if (gx < 0) {
              gx += cells;
              ox = -1;
            } else if (gx >= cells) {
              gx -= cells;
              ox = 1;
            }
            const k = gy * cells + gx;
            const ex = px[k] + ox - u;
            const ey = py[k] + oy - v;
            const d = ex * ex + ey * ey;
            if (d < best) {
              best = d;
              bk = k;
            }
          }
        }
        const p = j * size + i;
        sx[p] += ax[bk];
        sy[p] += ay[bk];
        if (sheen) {
          rough[p] = 0.24 + 0.14 * r[bk];
        }
      }
    }
  }

  // Folds: many short creases, fewer long wrinkles; each slopes the surface across its line,
  // fading with the distance from it and towards its ends.
  for (let f = 0; f < 320; f++) {
    const long = rnd() < 0.25;
    const cx = rnd();
    const cy = rnd();
    const a = rnd() * Math.PI;
    const half = long ? 0.12 + rnd() * 0.25 : 0.02 + rnd() * 0.08;
    const width = long ? 0.006 + rnd() * 0.02 : 0.002 + rnd() * 0.006;
    const amp = (long ? 0.12 + rnd() * 0.2 : 0.1 + rnd() * 0.25) * (rnd() < 0.5 ? -1 : 1);
    const dx = Math.cos(a);
    const dy = Math.sin(a);

    // The pixels it can reach (wrapping round the tile).
    const ex = Math.abs(dx) * half + 4 * width;
    const ey = Math.abs(dy) * half + 4 * width;
    const i0 = Math.floor((cx - ex) * size);
    const i1 = Math.ceil((cx + ex) * size);
    const j0 = Math.floor((cy - ey) * size);
    const j1 = Math.ceil((cy + ey) * size);
    for (let j = j0; j <= j1; j++) {
      const v = (j + 0.5) / size - cy;
      const row = ((j % size) + size) % size * size;
      for (let i = i0; i <= i1; i++) {
        const u = (i + 0.5) / size - cx;
        const s = u * dx + v * dy;
        const t = -u * dy + v * dx;
        if (Math.abs(s) >= half) {
          continue;
        }
        const slope = amp * (1 - (s / half) ** 2) * Math.sign(t) * Math.exp(-Math.abs(t) / width);
        const p = row + ((i % size) + size) % size;
        sx[p] += -dy * slope;
        sy[p] += dx * slope;
      }
    }
  }

  // The maps: a repeating texture filled pixel by pixel.
  const make = fill => {
    const c = document.createElement('canvas');
    c.width = c.height = size;
    const g = c.getContext('2d');
    const img = g.createImageData(size, size);
    const d = img.data;
    for (let p = 0; p < N; p++) {
      fill(p, d, 4 * p);
    }
    g.putImageData(img, 0, 0);
    const tex = new THREE.CanvasTexture(c);
    tex.wrapS = tex.wrapT = THREE.RepeatWrapping;
    tex.anisotropy = 8;
    return tex;
  };
  const normal = make((p, d, o) => {
    const x = -sx[p];
    const y = -sy[p];
    const len = Math.hypot(x, y, 1);
    d[o] = (x / len * 0.5 + 0.5) * 255;
    d[o + 1] = (y / len * 0.5 + 0.5) * 255;
    d[o + 2] = (1 / len * 0.5 + 0.5) * 255;
    d[o + 3] = 255;
  });
  const roughMap = make((p, d, o) => {
    d[o] = d[o + 1] = d[o + 2] = rough[p] * 255;
    d[o + 3] = 255;
  });
  return { normal, rough: roughMap };
}

// What the metal reflects: a dim lab with a few bright soft boxes (an equirectangular picture;
// each renderer prefilters it itself, so the ZED2 Viewpoint's renderer can use it too).
function studioEnvironment() {
  const w = 1024;
  const h = 512;
  const c = document.createElement('canvas');
  c.width = w;
  c.height = h;

  // The room: light above, dark below.
  const g = c.getContext('2d');
  const sky = g.createLinearGradient(0, 0, 0, h);
  sky.addColorStop(0, '#c3ccd8');
  sky.addColorStop(0.3, '#9ba3ae');
  sky.addColorStop(0.5, '#848a94');
  sky.addColorStop(0.56, '#5b6068');
  sky.addColorStop(1, '#2c2f34');
  g.fillStyle = sky;
  g.fillRect(0, 0, w, h);

  // Ceiling lights, and lit walls around the table (what the side panels see): [x, y, width,
  // height, colour].
  g.filter = 'blur(8px)';
  for (const [x, y, bw, bh, color] of [
    [70, 40, 230, 110, '#ffffff'], [560, 25, 320, 90, '#fff3e0'], [380, 140, 120, 60, '#e3edff'],
    [880, 120, 110, 90, '#ffffff'], [0, 200, 180, 70, '#dfe5ee'], [300, 215, 160, 55, '#f4f1ea'],
    [620, 205, 200, 65, '#e8edf5'], [900, 215, 124, 50, '#f0ece4'],
  ]) {
    g.fillStyle = color;
    g.fillRect(x, y, bw, bh);
  }
  g.filter = 'none';
  const tex = new THREE.CanvasTexture(c);
  tex.mapping = THREE.EquirectangularReflectionMapping;
  tex.colorSpace = THREE.SRGBColorSpace;
  return tex;
}
