# spot.plotting: drawing

`+spot/+plotting/` is the geometry the 3D view, the figures and the Results tab share: where the arm's joints are for a given pose, the outlines of the platforms and the arm's links for publication figures, the platforms' display names and colours, and readable names and units for the logged signals.

| Function | One line |
|---|---|
| [[armPosition|spot-plotting-armPosition]] | the arm's four joint points on the table from the Chaser's pose and the joint angles |
| [[drawArm|spot-plotting-drawArm]] | outlines of the three links for `patch` |
| [[drawSpacecraft|spot-plotting-drawSpacecraft]] | a platform's outline (with cone, grapple or panels) for `patch` |
| [[platformStyle|spot-plotting-platformStyle]] | `'RED'` → `'Chaser'` and its colour |
| [[signalLabel|spot-plotting-signalLabel]] | `'BLACK_Vx_mpers'` → "X-Velocity [m/s]", platform BLACK |
