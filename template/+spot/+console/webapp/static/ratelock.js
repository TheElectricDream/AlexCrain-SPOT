// The page's padlocks on settings that should only change if you know why. Each kind is one
// lock for the whole page, shared by every button of that kind; unlocking shows its dialog
// first (what the number is and why it is locked), and the page always starts locked.
//   rate:   the PhaseSpace data rate (dataRate): the Simulation tab's Data Rate and the
//           Experiment tab's camera Data Rate are the same setting, locked together
//           (#rateLockDialog)
//   pwm:    the thrusters' PWM frequency (pwmFrequency), which must match the Teensy firmware
//           (#pwmLockDialog)
//   solver: the Simulation tab's solver settings, read from the diagram (no dialog)

const $ = (sel, el = document) => el.querySelector(sel);

// The two icons.
const LOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" ' +
    'd="M7 10V7a5 5 0 0 1 10 0v3h1a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 ' +
    '1-1zm2 0h6V7a3 3 0 0 0-6 0z"/></svg>';
const UNLOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" ' +
    'd="M7 10V7a5 5 0 0 1 9.6-2l-1.8.9A3 3 0 0 0 9 7v3h9a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 ' +
    '0 1-1-1v-9a1 1 0 0 1 1-1z"/></svg>';

// Each kind: what it locks, its warning dialog and the dialog's unlock button.
const KINDS = {
  rate: { what: 'the data rate', dialog: '#rateLockDialog', go: '#rateUnlockGo' },
  pwm: { what: 'the thruster PWM frequency', dialog: '#pwmLockDialog', go: '#pwmUnlockGo' },
  solver: { what: 'the solver' },
};

const unlocked = { rate: false, pwm: false, solver: false };
const listeners = new Set();
const kindOf = k => (KINDS[k] ? k : 'rate');

/**
 * Whether a lock is open.
 * @param {string=} kind 'rate', 'pwm' or 'solver'.
 * @return {boolean}
 */
export const isUnlocked = (kind = 'rate') => unlocked[kindOf(kind)];

/**
 * Calls fn(kind, unlocked) whenever a lock changes.
 * @param {function(string, boolean)} fn The listener.
 */
export function onLockChange(fn) {
  listeners.add(fn);
}

/**
 * Opens or closes a lock and tells the listeners.
 * @param {string} kind The lock.
 * @param {boolean} v Whether it is now open.
 */
function setUnlocked(kind, v) {
  unlocked[kind] = v;
  for (const fn of listeners) {
    fn(kind, v);
  }
}

// A lock button's tooltip and accessible label.
const title = k => (unlocked[k]
  ? `Unlocked: click to lock ${KINDS[k].what} again`
  : `Locked: click to unlock ${KINDS[k].what}` +
    `${KINDS[k].dialog ? ' (read the warning first)' : ''}`);
const label = k => `${unlocked[k] ? 'Lock' : 'Unlock'} ${KINDS[k].what}`;

/**
 * A lock button (HTML) for next to a locked field; wire it with handleLockClick.
 * @param {string=} kind The lock.
 * @return {string} HTML.
 */
export function lockButton(kind = 'rate') {
  const k = kindOf(kind);
  return `<button type="button" class="rate-lock${unlocked[k] ? ' open' : ''}" data-ratelock="${k}" aria-pressed="${unlocked[k]}"
    title="${title(k)}" aria-label="${label(k)}">${unlocked[k] ? UNLOCKED : LOCKED}</button>`;
}

/**
 * Shows every lock button as its lock is.
 * @param {!Element|!Document=} root Where to look.
 */
export function paintLocks(root = document) {
  for (const b of root.querySelectorAll('[data-ratelock]')) {
    const k = kindOf(b.dataset.ratelock);
    b.setAttribute('aria-pressed', String(unlocked[k]));
    b.classList.toggle('open', unlocked[k]);
    b.innerHTML = unlocked[k] ? UNLOCKED : LOCKED;
    b.title = title(k);
    b.setAttribute('aria-label', label(k));
  }
}

/**
 * Handles a click on a lock button: locks at once, or asks before unlocking.
 * @param {string|!Event} arg The lock's kind, or the click event.
 * @return {undefined}
 */
export function handleLockClick(arg) {
  const k = kindOf(typeof arg === 'string' ? arg : arg?.currentTarget?.dataset?.ratelock);
  if (unlocked[k]) {
    return setUnlocked(k, false);
  }

  // No warning dialog: open at once; else show the warning.
  const dialog = KINDS[k].dialog && $(KINDS[k].dialog);
  if (!dialog) {
    return setUnlocked(k, true);
  }
  dialog.showModal();
}

// Each warning dialog's unlock button opens its lock.
for (const [k, d] of Object.entries(KINDS)) {
  const dialog = d.dialog && $(d.dialog);
  if (dialog) {
    $(d.go).onclick = () => {
      dialog.close();
      setUnlocked(k, true);
    };
  }
}
onLockChange(() => paintLocks());
