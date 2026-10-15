# platforms.js and ratelock.js

Two small modules the others import.

<!-- source: +spot/+console/webapp/static/platforms.js -->
<!-- source: +spot/+console/webapp/static/ratelock.js -->

## `platforms.js`: names and colours

The code, the diagram, the settings, the data files and the API keep the keys `RED`, `BLACK`, `BLUE` (and `ARM`); only what is shown changes. `PLATFORM_KEYS`, `NAMES` (Chaser, Target, Obstacle, Arm) and `platformName(key)`; the body/swatch colours (`COLORS`: silver, gold, blue; the arm orange so it stands apart from the Chaser), brighter `LINE_COLORS` for lines on the dark table, `TEXT_COLORS` for labels, `LED_COLORS` by owner (`none` for an LED not on a platform); `swatch(key)`, a small coloured square as HTML. The MATLAB counterpart is [[spot.plotting.platformStyle|spot-plotting-platformStyle]].

## `ratelock.js`: the padlocks

Two settings should only change if you know why, so their fields are locked behind a padlock; each kind of lock is one for the whole page, and the page always starts locked:
- **`rate`**: the PhaseSpace data rate. The Simulation tab's Data Rate and the Experiment tab's camera Data Rate are the same setting (`dataRate`), and changing it changes the LQR gains (they are designed at the PhaseSpace period).
- **`pwm`**: the thrusters' PWM frequency (`pwmFrequency`), which must match the Teensy firmware's fixed 5 Hz.

- `KINDS` (lines 11–14): each kind's name in titles, its dialog and its *Unlock* button.
- `isUnlocked(kind)`, `onLockChange(fn)` (16–27): the state per kind and its listeners (called with the kind and the new state).
- `title`, `label` (30–32): the button's tooltip and accessible name.
- `lockButton(kind)` (35–40): the button's HTML (`data-ratelock="<kind>"`), for a field rendered by a module.
- `paintLocks(root)` (42–51): every `[data-ratelock]` button shows its lock's state (icon, title, `aria-pressed`).
- `handleLockClick(kind or event)` (54–60): locking is immediate; unlocking opens the kind's dialog (`#rateLockDialog`, `#pwmLockDialog`), which explains what the number is and why it is locked. It takes the kind, or the click event of a button (whose `data-ratelock` names it; empty means `rate`).
- (62–66): each dialog's *Unlock* button unlocks its kind; every change repaints the buttons.

![The data rate's dialog](images/gui-rate-lock-dialog.png)

![The PWM's dialog](images/gui-pwm-lock-dialog.png)

## The code
<!-- listing -->
<details>
<summary><b>+spot/+console/webapp/static/platforms.js</b> (21 lines; click to show)</summary>

```javascript
   1  // The platforms as people see them: their names and colours. The code, the diagram, the settings,
   2  // the data files and the API keep the keys RED, BLACK and BLUE (and ARM); only what is shown changes.
   3  
   4  export const PLATFORM_KEYS = ['RED', 'BLACK', 'BLUE'];
   5  export const NAMES = { RED: 'Chaser', BLACK: 'Target', BLUE: 'Obstacle', ARM: 'Arm' };
   6  
   7  // What a key is called on screen ("RED" -> "Chaser"); anything else is returned unchanged
   8  export const platformName = key => NAMES[key] || key;
   9  
  10  // Body and swatch colours: the Chaser silver, the Target gold, the Obstacle blue (the arm's chip and
  11  // swatch keep their orange, so it stands apart from the Chaser it is mounted on)
  12  export const COLORS = { RED: '#aeb5bd', BLACK: '#d4a326', BLUE: '#1c7ed6', ARM: '#f08c00' };
  13  // Brighter versions for lines and outlines on the dark table
  14  export const LINE_COLORS = { RED: '#e9ecef', BLACK: '#fcc419', BLUE: '#4dabf7', ARM: '#e9ecef' };
  15  // Text on a label filled with the platform's colour
  16  export const TEXT_COLORS = { RED: '#15171a', BLACK: '#15171a', BLUE: '#ffffff', ARM: '#15171a' };
  17  // PhaseSpace LEDs by owner ("none": an LED that is not on a platform)
  18  export const LED_COLORS = { RED: '#f1f3f5', BLACK: '#ffd43b', BLUE: '#74c0fc', none: '#e599f7' };
  19  
  20  // A small square in the platform's colour
  21  export const swatch = key => `<i class="swatch" style="background:${COLORS[key] || '#868e96'}"></i>`;
```

</details>

<details>
<summary><b>+spot/+console/webapp/static/ratelock.js</b> (66 lines; click to show)</summary>

```javascript
   1  // The page's padlocks on settings that should only change if you know why. Each kind is one lock
   2  // for the whole page, shared by every button of that kind; unlocking shows its dialog first (what
   3  // the number is and why it is locked), and the page always starts locked.
   4  //   rate: the PhaseSpace data rate (dataRate): the Simulation tab's Data Rate and the Experiment
   5  //         tab's camera Data Rate are the same setting, locked together (#rateLockDialog)
   6  //   pwm:  the thrusters' PWM frequency (pwmFrequency), which must match the Teensy firmware (#pwmLockDialog)
   7  
   8  const $ = (sel, el = document) => el.querySelector(sel);
   9  const LOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 10 0v3h1a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1zm2 0h6V7a3 3 0 0 0-6 0z"/></svg>';
  10  const UNLOCKED = '<svg viewBox="0 0 24 24" aria-hidden="true"><path fill="currentColor" d="M7 10V7a5 5 0 0 1 9.6-2l-1.8.9A3 3 0 0 0 9 7v3h9a1 1 0 0 1 1 1v9a1 1 0 0 1-1 1H6a1 1 0 0 1-1-1v-9a1 1 0 0 1 1-1z"/></svg>';
  11  const KINDS = {
  12    rate: { what: 'the data rate', dialog: '#rateLockDialog', go: '#rateUnlockGo' },
  13    pwm: { what: 'the thruster PWM frequency', dialog: '#pwmLockDialog', go: '#pwmUnlockGo' },
  14  };
  15  
  16  const unlocked = { rate: false, pwm: false };
  17  const listeners = new Set();
  18  const kindOf = k => (KINDS[k] ? k : 'rate');
  19  
  20  export const isUnlocked = (kind = 'rate') => unlocked[kindOf(kind)];
  21  
  22  // Call fn(kind, unlocked) whenever a lock changes
  23  export function onLockChange(fn) { listeners.add(fn); }
  24  
  25  function setUnlocked(kind, v) {
  26    unlocked[kind] = v;
  27    for (const fn of listeners) fn(kind, v);
  28  }
  29  
  30  const title = k => (unlocked[k] ? `Unlocked: click to lock ${KINDS[k].what} again`
  31                                  : `Locked: click to unlock ${KINDS[k].what} (read the warning first)`);
  32  const label = k => `${unlocked[k] ? 'Lock' : 'Unlock'} ${KINDS[k].what}`;
  33  
  34  // A lock button (HTML) for next to a locked field; wire it with handleLockClick
  35  export function lockButton(kind = 'rate') {
  36    const k = kindOf(kind);
  37    return `<button type="button" class="rate-lock${unlocked[k] ? ' open' : ''}" data-ratelock="${k}" aria-pressed="${unlocked[k]}"
  38      title="${title(k)}" aria-label="${label(k)}">${unlocked[k] ? UNLOCKED : LOCKED}</button>`;
  39  }
  40  
  41  // Show every lock button as its lock is
  42  export function paintLocks(root = document) {
  43    for (const b of root.querySelectorAll('[data-ratelock]')) {
  44      const k = kindOf(b.dataset.ratelock);
  45      b.setAttribute('aria-pressed', String(unlocked[k]));
  46      b.classList.toggle('open', unlocked[k]);
  47      b.innerHTML = unlocked[k] ? UNLOCKED : LOCKED;
  48      b.title = title(k);
  49      b.setAttribute('aria-label', label(k));
  50    }
  51  }
  52  
  53  // A click on a lock button (its kind, or the click event): lock at once, or ask before unlocking
  54  export function handleLockClick(arg) {
  55    const k = kindOf(typeof arg === 'string' ? arg : arg?.currentTarget?.dataset?.ratelock);
  56    if (unlocked[k]) return setUnlocked(k, false);
  57    const dialog = $(KINDS[k].dialog);
  58    if (!dialog) return setUnlocked(k, true);
  59    dialog.showModal();
  60  }
  61  
  62  for (const [k, d] of Object.entries(KINDS)) {
  63    const dialog = $(d.dialog);
  64    if (dialog) $(d.go).onclick = () => { dialog.close(); setUnlocked(k, true); };
  65  }
  66  onLockChange(() => paintLocks());
```

</details>
<!-- /listing -->
