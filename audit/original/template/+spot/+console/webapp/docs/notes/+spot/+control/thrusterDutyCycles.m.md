## RotateFrame

The rotation $C_{bI}(\theta)$ from the table frame to the body frame, for a platform at attitude θ ([The Thruster Mixer](page:math-mixer)). A verbatim copy of the diagram's *Rotate Forces to Body/Create Rotation Matrix* block.

## MakeH

The 3×8 matrix that maps the eight duty cycles to the body-frame force and torque: each thruster's direction, its lever arm about the centre of gravity (mm, converted to m) and half its thrust. Its pseudo-inverse gives the mixer's first guess. A verbatim copy of the diagram's *Generate Initial Guess* block.
