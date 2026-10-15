# spot.massprops.bifilarInertia

**File:** `+spot/+massprops/bifilarInertia.m` · **Package:** [[spot.massprops|spot-massprops]]

<!-- source: +spot/+massprops/bifilarInertia.m -->

## What it is for

The moment of inertia about the vertical axis from a bifilar (two-string) pendulum test: hang the platform from two strings of length $L$ a distance $D$ apart, twist it, time the period $T$.

$$I = \frac{T^2 M g D^2}{16 \pi^2 L}$$

## How to call it

```matlab
I = spot.massprops.bifilarInertia(13.75, 3.00, 2.56, 0.28)    % kg, s, m, m -> 0.2354 kg m^2
```

## Line by line

**Line 5.** $g = 9.81$ m/s².

**Line 6.** The formula above.

<!-- xref -->
- **Used by:** `+spot/+massprops/compute.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+massprops/bifilarInertia.m</b> (7 lines; click to show)</summary>

```matlab
   1  function I = bifilarInertia(mass, T, L, D)
   2  %BIFILARINERTIA Moment of inertia from a bifilar (two-string) pendulum test.
   3  %   I = spot.massprops.bifilarInertia(mass, T, L, D) with the mass [kg], period of oscillation
   4  %   T [s], string length L [m] and string spacing D [m]. Returns I in kg*m^2.
   5  g = 9.81;
   6  I = T^2 * mass * g * D^2 / (16 * pi^2 * L);
   7  end
```

</details>
<!-- /listing -->
