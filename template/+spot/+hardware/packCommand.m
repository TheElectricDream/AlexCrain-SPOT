function packet = packCommand(duty, pucks)
%PACKCOMMAND Command packet for a platform's Teensy (firmware: +spot/+hardware/teensy/teensy_pwm_control.ino).
%   packet = spot.hardware.packCommand(duty, pucks) with DUTY the 8 thruster duty cycles in
%   percent (0-100) and PUCKS the air bearings (on if >= 0.5). Returns the 37-byte packet as a
%   uint8 column: 0xAA, then the 9 values as little-endian single.
%   The Teensy switches everything off when no packet has arrived for 2 s.
if numel(duty) ~= 8
    error('spot:hardware:packCommand', 'Give 8 thruster duty cycles.');
end
values = single([duty(:); pucks]);
bytes = typecast(values, 'uint8');           % MATLAB runs little-endian on all its platforms
packet = [uint8(170); bytes(:)];
end
