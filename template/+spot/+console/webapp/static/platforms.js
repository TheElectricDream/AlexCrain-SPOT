// The platforms as people see them: their names and colours. The code, the diagram, the settings,
// the data files and the API keep the keys RED, BLACK and BLUE (and ARM); only what is shown changes.

export const PLATFORM_KEYS = ['RED', 'BLACK', 'BLUE'];
export const NAMES = { RED: 'Chaser', BLACK: 'Target', BLUE: 'Obstacle', ARM: 'Arm' };

// What a key is called on screen ("RED" -> "Chaser"); anything else is returned unchanged
export const platformName = key => NAMES[key] || key;

// Body and swatch colours: the Chaser silver, the Target gold, the Obstacle blue (the arm's chip and
// swatch keep their orange, so it stands apart from the Chaser it is mounted on)
export const COLORS = { RED: '#aeb5bd', BLACK: '#d4a326', BLUE: '#1c7ed6', ARM: '#f08c00' };
// Brighter versions for lines and outlines on the dark table
export const LINE_COLORS = { RED: '#e9ecef', BLACK: '#fcc419', BLUE: '#4dabf7', ARM: '#e9ecef' };
// Text on a label filled with the platform's colour
export const TEXT_COLORS = { RED: '#15171a', BLACK: '#15171a', BLUE: '#ffffff', ARM: '#15171a' };
// PhaseSpace LEDs by owner ("none": an LED that is not on a platform)
export const LED_COLORS = { RED: '#f1f3f5', BLACK: '#ffd43b', BLUE: '#74c0fc', none: '#e599f7' };

// A small square in the platform's colour
export const swatch = key => `<i class="swatch" style="background:${COLORS[key] || '#868e96'}"></i>`;
