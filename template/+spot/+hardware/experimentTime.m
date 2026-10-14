function [t, state] = experimentTime(packet, age)
%EXPERIMENTTIME  The experiment's time, the same on every platform, from the PhaseSpace stream.
%
%   All platforms receive the same PhaseSpace packets from the lab console (TX2). When the lab
%   console starts an experiment, it waits until every platform's program is running, then
%   tells StreamData to start the experiment a few seconds later. From then on every packet
%   carries the experiment clock in slot 113 (LED 31's x, which PhaseSpace never uses):
%     NaN        no experiment armed
%     negative   counting down to the agreed start
%     >= 0       seconds since the start
%   So every platform starts at the same moment, whenever its own program was started, and the
%   phases switch together. Use it in a MATLAB Function block, on the packet and age from
%   spot.hardware.phasespaceLatest:
%
%       function [t, state] = fcn(packet, age)
%       [t, state] = spot.hardware.experimentTime(packet, age);
%
%   t       experiment time [s]: 0 until the start, then the shared clock. Between packets
%           it is advanced by the packet's age, so it moves smoothly at any stream rate.
%   state   0 waiting (no experiment armed yet), 1 counting down, 2 running on the shared
%           clock, 3 running on this computer's clock (the shared clock was lost or jumped
%           back after the start: t carries on from where it was instead)
%
%   Safety: a program that was started after the countdown (it never saw the clock negative)
%   stays at t = 0, rather than jumping into the middle of an experiment.
%#codegen

EXP_CLOCK = 113;     % 1-based slot of the experiment clock in the PhaseSpace packet
MAX_BACKWARDS = 0.5; % [s] a shared clock this far behind t counts as lost

persistent armed started lastT lastNow
if isempty(armed)
    armed = false;
    started = false;
    lastT = 0;
    lastNow = 0;
end

now = 0;
if coder.target('Rtw')
    now = spot.hardware.Udp.now();
end
c = double(packet(EXP_CLOCK));
valid = age >= 0 && ~isnan(c);
if valid
    c = c + age;   % the clock when the newest packet left, plus the time since it arrived
end

t = 0;
state = 0;
if ~started
    if valid && c < 0
        armed = true;
        state = 1;
    elseif valid && armed
        started = true;
        t = c;
        state = 2;
    end
else
    if valid && c > lastT - MAX_BACKWARDS
        t = max(c, lastT);   % never backwards
        state = 2;
    else
        t = lastT + (now - lastNow);
        state = 3;
    end
end
if started
    lastT = t;
    lastNow = now;
end
end
