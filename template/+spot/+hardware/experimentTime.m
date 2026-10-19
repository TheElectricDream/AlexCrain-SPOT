function [t, state] = experimentTime(packet, age)
    %EXPERIMENTTIME  The experiment's time, the same on every platform, from the PhaseSpace stream.
    %   [t, state] = spot.hardware.experimentTime(packet, age). When the lab console starts an
    %   experiment, it waits until every platform's program runs, then tells StreamData to start
    %   a few seconds later. From then on every packet carries the experiment clock in slot 113
    %   (LED 31's x, which PhaseSpace never uses):
    %     NaN        no experiment armed
    %     negative   counting down to the agreed start
    %     >= 0       seconds since the start
    %   So every platform starts at the same moment and the phases switch together.
    %
    %   Safety: a program started after the countdown (it never saw the clock negative) stays at
    %   t = 0, rather than jumping into the middle of an experiment.
    %
    %   Inputs
    %     packet  the packet from spot.hardware.phasespaceLatest
    %     age     its age [s] (-1 = nothing received yet)
    %
    %   Outputs
    %     t       experiment time [s]: 0 until the start, then the shared clock, advanced by
    %             the packet's age between packets so it moves smoothly at any stream rate
    %     state   0 waiting (no experiment armed), 1 counting down, 2 running on the shared
    %             clock, 3 running on this computer's clock (the shared clock was lost or jumped
    %             back after the start: t carries on from where it was)
    %
    %   Example (in a MATLAB Function block)
    %     function [t, state] = fcn(packet, age)
    %     [t, state] = spot.hardware.experimentTime(packet, age);
    %
    %   See also spot.hardware.phasespaceLatest.

    %#codegen

    %% Constants
    EXP_CLOCK = 113;     % 1-based slot of the experiment clock in the PhaseSpace packet
    MAX_BACKWARDS = 0.5; % [s] a shared clock this far behind t counts as lost

    %% State between steps
    persistent armed started lastT lastNow
    if isempty(armed)
        armed = false;
        started = false;
        lastT = 0;
        lastNow = 0;
    end

    %% Clocks
    % This computer's clock (generated code only).
    localNow = 0;
    if coder.target('Rtw')
        localNow = spot.hardware.Udp.now();
    end

    % The shared clock when the newest packet left, plus the time since it arrived.
    sharedT = double(packet(EXP_CLOCK));
    valid = age >= 0 && ~isnan(sharedT);
    if valid
        sharedT = sharedT + age;
    end

    %% Experiment time
    t = 0;
    state = 0;
    if ~started

        % Armed by a countdown; started when the clock reaches 0 after one.
        if valid && sharedT < 0
            armed = true;
            state = 1;
        elseif valid && armed
            started = true;
            t = sharedT;
            state = 2;
        end
    else

        % Running: the shared clock, never backwards; this computer's clock if it is lost.
        if valid && sharedT > lastT - MAX_BACKWARDS
            t = max(sharedT, lastT);
            state = 2;
        else
            t = lastT + (localNow - lastNow);
            state = 3;
        end
    end
    if started
        lastT = t;
        lastNow = localNow;
    end
end
