// Saves a 3D model (a three.js group, such as the Spacecraft Designer's platform) as one file
// that other programs open:
//   .glb  binary glTF 2.0 (Blender, Windows' 3D Viewer, web viewers, game engines): each part
//         by name with its colours, in metres; y up, as glTF has it (importers turn it back);
//   .stl  binary STL (CAD programs, slicers): the surface only, in millimetres, z up.
// Both keep the model's own frame (the body frame: origin at the centre of the footprint on the
// table top). Textures are left out: a textured material keeps the colour it averages to.

import * as THREE from 'three';

// The colour of a material whose look is its texture (cad.js's keys).
const TEXTURE_COLORS = { solar: 0x1a1f28 };
// A roughness map's (the crinkled MLI's) usual value.
const MAPPED_ROUGHNESS = 0.35;
const MM = 1000;   // [mm/m] STL
const STL_HEADER = 80;   // [bytes]
const STL_TRIANGLE = 50;   // [bytes] normal, 3 vertices, attribute
const GLB_MAGIC = 0x46546c67;   // 'glTF'
const GLB_JSON = 0x4e4f534a;   // 'JSON'
const GLB_BIN = 0x004e4942;   // 'BIN\0'
const FLOAT = 5126;   // glTF component types and buffer targets
const UINT = 5125;
const ARRAY_BUFFER = 34962;
const ELEMENT_ARRAY_BUFFER = 34963;

/**
 * The model's visible meshes in its frame, by part: each mesh goes to its nearest named
 * ancestor (the group itself if none).
 * @param {!THREE.Object3D} root The model.
 * @return {!Map<string, !Array<{material: !THREE.Material, key: string,
 *     position: !Float32Array, normal: !Float32Array, index: !Uint32Array}>>}
 */
function collect(root) {
  root.updateMatrixWorld(true);
  const toRoot = root.matrixWorld.clone().invert();
  const parts = new Map();
  root.traverseVisible(o => {
    if (!o.isMesh || !o.geometry.attributes.position) {
      return;
    }
    let owner = o;
    while (owner !== root && !owner.name) {
      owner = owner.parent;
    }
    const part = owner.name || 'Model';

    // Its vertices and normals in the model's frame.
    const m = new THREE.Matrix4().multiplyMatrices(toRoot, o.matrixWorld);
    const nm = new THREE.Matrix3().getNormalMatrix(m);
    const pos = o.geometry.attributes.position;
    const nrm = o.geometry.attributes.normal;
    const position = new Float32Array(3 * pos.count);
    const normal = new Float32Array(3 * pos.count);
    const v = new THREE.Vector3();
    for (let i = 0; i < pos.count; i++) {
      v.fromBufferAttribute(pos, i).applyMatrix4(m).toArray(position, 3 * i);
      if (nrm) {
        v.fromBufferAttribute(nrm, i).applyMatrix3(nm).normalize().toArray(normal, 3 * i);
      }
    }

    // Its triangles, turned round if the transform mirrors it (so they still face out).
    const index = o.geometry.index
      ? Uint32Array.from(o.geometry.index.array)
      : Uint32Array.from({ length: pos.count }, (_, i) => i);
    if (m.determinant() < 0) {
      for (let t = 0; t + 2 < index.length; t += 3) {
        [index[t + 1], index[t + 2]] = [index[t + 2], index[t + 1]];
      }
    }
    fillNormals(position, normal, index);
    const material = Array.isArray(o.material) ? o.material[0] : o.material;
    if (!parts.has(part)) {
      parts.set(part, []);
    }
    parts.get(part).push({
      material, key: o.userData.material || material.name || 'material', position, normal,
      index,
    });
  });
  return parts;
}

/**
 * Gives each vertex without a normal (a few in the CAD have none) the normal of the triangles
 * round it, so every normal is a unit vector as glTF requires.
 * @param {!Float32Array} position The vertices.
 * @param {!Float32Array} normal Their normals, filled in where zero.
 * @param {!Uint32Array} index The triangles.
 */
function fillNormals(position, normal, index) {
  const missing = new Uint8Array(position.length / 3);
  let any = false;
  for (let i = 0; i < missing.length; i++) {
    missing[i] = Math.hypot(normal[3 * i], normal[3 * i + 1], normal[3 * i + 2]) < 1e-6;
    any ||= missing[i];
  }
  if (!any) {
    return;
  }

  // Each triangle's normal (its length its area) added to its corners that have none.
  const a = new THREE.Vector3();
  const b = new THREE.Vector3();
  const c = new THREE.Vector3();
  for (let t = 0; t + 2 < index.length; t += 3) {
    const corners = [index[t], index[t + 1], index[t + 2]];
    if (!corners.some(k => missing[k])) {
      continue;
    }
    a.fromArray(position, 3 * corners[0]);
    b.fromArray(position, 3 * corners[1]);
    c.fromArray(position, 3 * corners[2]);
    c.sub(b).cross(a.sub(b));
    for (const k of corners.filter(k => missing[k])) {
      normal[3 * k] += c.x;
      normal[3 * k + 1] += c.y;
      normal[3 * k + 2] += c.z;
    }
  }

  // Each made a unit vector (up, for a vertex on no triangle with an area).
  const v = new THREE.Vector3();
  for (let k = 0; k < missing.length; k++) {
    if (missing[k]) {
      v.fromArray(normal, 3 * k);
      (v.lengthSq() > 0 ? v.normalize() : v.set(0, 0, 1)).toArray(normal, 3 * k);
    }
  }
}

/**
 * A glTF material from a three.js one: its colour, metalness, roughness and transparency.
 * @param {!THREE.Material} m The material.
 * @param {string} key Its name (cad.js's material key).
 * @return {!Object} The glTF material.
 */
function gltfMaterial(m, key) {
  const color = TEXTURE_COLORS[key] != null
    ? new THREE.Color(TEXTURE_COLORS[key])
    : m.color || new THREE.Color(1, 1, 1);
  const clear = m.transparent && m.opacity < 1;
  const out = {
    name: key,
    pbrMetallicRoughness: {
      baseColorFactor: [color.r, color.g, color.b, clear ? m.opacity : 1],
      metallicFactor: m.metalness ?? 0,
      roughnessFactor: m.roughnessMap ? MAPPED_ROUGHNESS : m.roughness ?? 1,
    },
  };
  if (clear) {
    out.alphaMode = 'BLEND';
  }
  if (m.side === THREE.DoubleSide) {
    out.doubleSided = true;
  }
  return out;
}

/**
 * The model as binary glTF: a node per part (one mesh, a primitive per material) under a root
 * node that turns z up into glTF's y up.
 * @param {!THREE.Object3D} root The model.
 * @param {string} title The root node's name.
 * @return {!ArrayBuffer} The .glb file.
 */
export function toGlb(root, title) {
  const json = {
    asset: { version: '2.0', generator: 'SPOT Console' }, scene: 0, scenes: [{ nodes: [0] }],
    nodes: [{ name: title, rotation: [-Math.SQRT1_2, 0, 0, Math.SQRT1_2], children: [] }],
    meshes: [], materials: [], accessors: [], bufferViews: [], buffers: [],
  };
  const arrays = [];
  let length = 0;

  // An array in the binary chunk (each is 4-byte aligned: floats and uint32s only), and an
  // accessor on it.
  const accessor = (array, target, type, componentType, extra = {}) => {
    json.bufferViews.push({ buffer: 0, byteOffset: length, byteLength: array.byteLength, target });
    arrays.push(array);
    length += array.byteLength;
    json.accessors.push({
      bufferView: json.bufferViews.length - 1, componentType,
      count: array.length / { SCALAR: 1, VEC3: 3 }[type], type, ...extra,
    });
    return json.accessors.length - 1;
  };
  const materials = new Map();
  for (const [part, meshes] of collect(root)) {
    const primitives = meshes.map(({ material, key, position, normal, index }) => {
      if (!materials.has(material)) {
        materials.set(material, json.materials.length);
        json.materials.push(gltfMaterial(material, key));
      }
      const box = new THREE.Box3().setFromArray(position);
      return {
        attributes: {
          POSITION: accessor(position, ARRAY_BUFFER, 'VEC3', FLOAT,
            { min: box.min.toArray(), max: box.max.toArray() }),
          NORMAL: accessor(normal, ARRAY_BUFFER, 'VEC3', FLOAT),
        },
        indices: accessor(index, ELEMENT_ARRAY_BUFFER, 'SCALAR', UINT),
        material: materials.get(material),
      };
    });
    json.meshes.push({ name: part, primitives });
    json.nodes.push({ name: part, mesh: json.meshes.length - 1 });
    json.nodes[0].children.push(json.nodes.length - 1);
  }
  json.buffers.push({ byteLength: length });

  // The file: its header, the JSON chunk (padded with spaces) and the binary chunk.
  const text = new TextEncoder().encode(JSON.stringify(json));
  const jsonLength = Math.ceil(text.length / 4) * 4;
  const total = 12 + 8 + jsonLength + 8 + length;
  const out = new ArrayBuffer(total);
  const dv = new DataView(out);
  const bytes = new Uint8Array(out);
  dv.setUint32(0, GLB_MAGIC, true);
  dv.setUint32(4, 2, true);
  dv.setUint32(8, total, true);
  dv.setUint32(12, jsonLength, true);
  dv.setUint32(16, GLB_JSON, true);
  bytes.fill(0x20, 20, 20 + jsonLength);
  bytes.set(text, 20);
  let at = 20 + jsonLength;
  dv.setUint32(at, length, true);
  dv.setUint32(at + 4, GLB_BIN, true);
  at += 8;
  for (const a of arrays) {
    bytes.set(new Uint8Array(a.buffer, a.byteOffset, a.byteLength), at);
    at += a.byteLength;
  }
  return out;
}

/**
 * The model as binary STL, in millimetres.
 * @param {!THREE.Object3D} root The model.
 * @param {string} title For the file's header (ASCII, up to 80 characters).
 * @return {!ArrayBuffer} The .stl file.
 */
export function toStl(root, title) {
  const meshes = [...collect(root).values()].flat();
  const count = meshes.reduce((n, m) => n + Math.floor(m.index.length / 3), 0);
  const out = new ArrayBuffer(STL_HEADER + 4 + STL_TRIANGLE * count);
  const dv = new DataView(out);

  // The header (it mustn't start with "solid", which marks an ASCII STL), then the count.
  const header = `${title} (mm, z up)`.replace(/[^\x20-\x7e]/g, '?').slice(0, STL_HEADER);
  for (let i = 0; i < header.length; i++) {
    dv.setUint8(i, header.charCodeAt(i));
  }
  dv.setUint32(STL_HEADER, count, true);

  // Each triangle: its normal (from its corners), its corners [mm] and an empty attribute.
  const a = new THREE.Vector3();
  const b = new THREE.Vector3();
  const c = new THREE.Vector3();
  const n = new THREE.Vector3();
  const e = new THREE.Vector3();
  let at = STL_HEADER + 4;
  for (const { position, index } of meshes) {
    for (let t = 0; t + 2 < index.length; t += 3) {
      a.fromArray(position, 3 * index[t]);
      b.fromArray(position, 3 * index[t + 1]);
      c.fromArray(position, 3 * index[t + 2]);
      n.subVectors(c, b).cross(e.subVectors(a, b)).normalize();
      for (const v of [n, a.multiplyScalar(MM), b.multiplyScalar(MM), c.multiplyScalar(MM)]) {
        dv.setFloat32(at, v.x, true);
        dv.setFloat32(at + 4, v.y, true);
        dv.setFloat32(at + 8, v.z, true);
        at += 12;
      }
      dv.setUint16(at, 0, true);
      at += 2;
    }
  }
  return out;
}
