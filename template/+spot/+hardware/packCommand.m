function packet = packCommand(duty, pucks)
    %PACKCOMMAND Command packet for a platform's Teensy (firmware: teensy/teensy_pwm_control.ino).
    %   packet = spot.hardware.packCommand(duty, pucks). The Teensy switches everything off when
    %   no packet has arrived for TEENSY_WATCHDOG (Constants).
    %
    %   Inputs
    %     duty    8 thruster duty cycles [%] (0-100)
    %     pucks   air bearings, on if >= 0.5
    %
    %   Outputs
    %     packet  37-byte uint8 column: 0xAA, then the 9 values as little-endian single
    %
    %   See also spot.hardware.udpSend.

    %% Constants
    HEADER = Constants.TEENSY_SYNC_BYTE;        % 0xAA, start of packet
    NUM_THRUSTERS = Constants.NUM_THRUSTERS;

    if numel(duty) ~= NUM_THRUSTERS
        error('spot:hardware:packCommand', 'Give %d thruster duty cycles.', NUM_THRUSTERS);
    end

    % Values as single bytes; MATLAB runs little-endian on all its platforms.
    values = single([duty(:); pucks]);
    bytes = typecast(values, 'uint8');
    packet = [uint8(HEADER); bytes(:)];
end
