// The page's padlocks on settings that should only change if you know why. Each kind is one lock
// for the whole page, shared by every button of that kind; unlocking shows its dialog first (what
// the number is and why it is locked), and the page always starts locked.
//   rate: the PhaseSpace data rate (dataRate): the Simulation tab's Data Rate and the Experiment
//         tab's camera Data Rate are the same setting, locked together (#rateLockDialog)
//   pwm:  the thrusters' PWM frequency (pwmFrequency), which must match the Teensy firmware (#pwmLockDialog)

const $ = (sel, el = document) => el.querySelector(sel);
const LOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 10 0v3h1a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1zm2 0h6V7a3 3 0 0 0-6 0z"/></svg>';
const UNLOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 9.6-2l-1.8.9A3 3 0 0 0 9 7v3h9a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1z"/></svg>';
const KINDS = {
  rate: { what: 'the data rate', dialog: '#rateLockDialog', go: '#rateUnlockGo' },
  pwm: { what: 'the thruster PWM frequency', dialog: '#pwmLockDialog', go: '#pwmUnlockGo' },
};

const unlocked = { rate: false, pwm: false };
const listeners = new Set();
const kindOf = k => (KINDS[k] ? k : 'rate');

export const isUnlocked = (kind = 'rate') => unlocked[kindOf(kind)];

// Call fn(kind, unlocked) whenever a lock changes
export function onLockChange(fn) { listeners.add(fn); }

function setUnlocked(kind, v) {
  unlocked[kind] = v;
  for (const fn of listeners) fn(kind, v);
}

const title = k => (unlocked[k] ? `Unlocked: click to lock ${KINDS[k].what} again`
                                : `Locked: click to unlock ${KINDS[k].what} (read the warning first)`);
const label = k => `${unlocked[k] ? 'Lock' : 'Unlock'} ${KINDS[k].what}`;

// A lock button (HTML) for next to a locked field; wire it with handleLockClick
export function lockButton(kind = 'rate') {
  const k = kindOf(kind);
  return `<button type="button" class="rate-lock${unlocked[k] ? ' open' : ''}" data-ratelock="${k}" aria-pressed="${unlocked[k]}"
    title="${title(k)}" aria-label="${label(k)}">${unlocked[k] ? UNLOCKED : LOCKED}</button>`;
}

// Show every lock button as its lock is
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

// A click on a lock button (its kind, or the click event): lock at once, or ask before unlocking
export function handleLockClick(arg) {
  const k = kindOf(typeof arg === 'string' ? arg : arg?.currentTarget?.dataset?.ratelock);
  if (unlocked[k]) return setUnlocked(k, false);
  const dialog = $(KINDS[k].dialog);
  if (!dialog) return setUnlocked(k, true);
  dialog.showModal();
}

for (const [k, d] of Object.entries(KINDS)) {
  const dialog = $(d.dialog);
  if (dialog) $(d.go).onclick = () => { dialog.close(); setUnlocked(k, true); };
}
onLockChange(() => paintLocks());
