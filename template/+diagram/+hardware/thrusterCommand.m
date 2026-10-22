function frame = thrusterCommand(bus, whoAmI)
    %THRUSTERCOMMAND The frame for this platform's Teensy: its own duty cycles and air bearings.
    %   frame = diagram.hardware.thrusterCommand(bus, whoAmI)
    %
    %   The same program runs on every platform, and every platform computes all three
    %   platforms' duty cycles. This picks out the ones for the platform it runs on and
    %   packs them for the Teensy (firmware: +spot/+hardware/teensy/teensy_pwm_control.ino),
    %   ready for a Serial Write block. On a computer that is not one of the three
    %   platforms (whoAmI 0, a simulation, or 4 and 5, the spare and vision computers),
    %   everything is off.
    %
    %   The frame is the one spot.hardware.packCommand builds: 0xAA, then the 8 duty cycles
    %   in percent and the air bearings (1 on, 0 off), each a little-endian single. The
    %   Teensy switches everything off when no frame has arrived for 2 s.
    %
    %   Inputs
    %     bus      The diagram's bus (Bus: SpotBus), as it leaves the mixers
    %     whoAmI   Platform the program runs on (spot.hardware.IdentifyPlatform):
    %              1 RED, 2 BLACK, 3 BLUE, anything else none
    %
    %   Outputs
    %     frame    37 bytes for the Teensy (uint8)
    %
    %   See also spot.hardware.packCommand, spot.hardware.IdentifyPlatform.

    %#codegen

    arguments (Input)
        bus     (1,1) struct
        whoAmI  (1,1) {mustBeNumeric}
    end

    HEADER = uint8(170);    % 0xAA, start of a frame

    % This platform's duty cycles and air bearings; everything off elsewhere
    switch whoAmI
        case 1
            duty     = bus.RED.duty;
            floating = bus.RED.command.floating;
        case 2
            duty     = bus.BLACK.duty;
            floating = bus.BLACK.command.floating;
        case 3
            duty     = bus.BLUE.duty;
            floating = bus.BLUE.command.floating;
        otherwise
            duty     = zeros(8, 1);
            floating = false;
    end

    % In percent, as the Teensy expects; anything outside 0 to 100 (or NaN) is
    % clipped by the firmware too, but is kept out of the frame here
    percent = min(max(duty, 0), 1) * 100;
    percent(isnan(percent)) = 0;

    % Header, then the nine values as single-precision bytes
    values = single([percent; double(floating)]);
    frame  = [HEADER; typecast(values, 'uint8')];
end
