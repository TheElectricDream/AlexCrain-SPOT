function sendExperimentTime(t, whoAmI, labConsoleIP)
    %SENDEXPERIMENTTIME This platform's experiment time, to the lab console, every step.
    %   spot.hardware.sendExperimentTime(t, whoAmI, labConsoleIP)
    %
    %   The lab console starts an experiment's shared countdown only once every selected
    %   platform's program is sending its clock (still 0 while it waits), and shows each
    %   platform's time on the Experiment tab (+interface/+console/webapp/spot_console/
    %   experiment.py). Each platform sends one little-endian double to its own port:
    %   RED 48720, BLACK 48721, BLUE 48722. Without it, the countdown never starts and the
    %   platforms stay in phase 0.
    %
    %   Only the platform the program runs on sends. Nothing is sent from any other computer
    %   (whoAmI 0, a simulation, or 4 and 5, the spare and vision computers), and nothing is
    %   sent in a desktop simulation (spot.hardware.udpSend only sends from generated code).
    %
    %   Inputs
    %     t             Experiment time [s] (spot.hardware.experimentTime)
    %     whoAmI        Platform the program runs on (spot.hardware.IdentifyPlatform):
    %                   1 RED, 2 BLACK, 3 BLUE, anything else none
    %     labConsoleIP  Lab console's address (the workspace's labConsoleIP)
    %
    %   See also spot.hardware.udpSend, spot.hardware.experimentTime.

    %#codegen

    arguments (Input)
        t               (1,1) double
        whoAmI          (1,1) {mustBeNumeric}
        labConsoleIP    % four numbers or text (spot.hardware.ipText)
    end

    % udpSend needs its port as a constant, so one call per platform
    switch whoAmI
        case 1
            spot.hardware.udpSend(labConsoleIP, 48720, t);
        case 2
            spot.hardware.udpSend(labConsoleIP, 48721, t);
        case 3
            spot.hardware.udpSend(labConsoleIP, 48722, t);
    end
end
