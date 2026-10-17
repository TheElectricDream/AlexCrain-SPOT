// SPOT's constants (Constants.m) as the server sends them with the page's state
// (state.constants: {NAME: value}, each a number, a string, an array for a row vector or an
// array of arrays for a matrix; per-platform values end in _RED, _BLACK or _BLUE). Nothing in the
// page types a parameter itself: a module asks here, inside the function that uses the value,
// since the modules load before the state arrives (app.js waits for the first state before it
// starts the page). Units are SI (m, s) unless Constants.m's comment says otherwise; packet
// indices are 1-based there.

let constants = null;

/**
 * Keeps the constants the state brought (app.js, on every state).
 * @param {?Object} values state.constants, {NAME: value}.
 */
export function setConstants(values) {
  if (values && typeof values === 'object') {
    constants = values;
  }
}

/**
 * A constant by its MATLAB name: constant('TABLE_X').
 * @param {string} name UPPER_SNAKE_CASE, as in Constants.m.
 * @return {*} Its value (a number, string or array).
 */
export function constant(name) {
  if (!constants) {
    throw new Error(`Constants.${name} isn't here: the page's state brought no constants`);
  }
  if (!(name in constants)) {
    throw new Error(`Constants has no ${name} (is Constants.m up to date?)`);
  }
  return constants[name];
}

/**
 * A per-platform constant: constantPlatform('MASS', 'RED') is MASS_RED.
 * @param {string} name The constant's name without the platform.
 * @param {string} platform RED, BLACK or BLUE.
 * @return {*} Its value.
 */
export function constantPlatform(name, platform) {
  return constant(`${name}_${platform}`);
}
