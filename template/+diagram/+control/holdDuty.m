function dutyHeld = holdDuty(duty, previousDuty, time, PWMFreq, baseRate)
    %HOLDDUTY Hold the duty cycles for a whole PWM period, as the thrusters do.
    %   dutyHeld = diagram.control.holdDuty(duty, previousDuty, time, PWMFreq, baseRate)
    %
    %   New duty cycles take effect only on the first step of each PWM period (every
    %   1/PWMFreq seconds); on the other steps the previous ones are kept. The memory
    %   is outside this function: feed dutyHeld back as previousDuty through a Unit
    %   Delay (initial condition zeros(8,1)).
    %
    %   Inputs
    %     duty          Duty cycles from the mixer this step, 0 to 1
    %     previousDuty  Duty cycles held on the previous step, 0 to 1
    %     time          Simulation or experiment time [s]
    %     PWMFreq       Thrusters' PWM frequency [Hz]
    %     baseRate      Diagram's time step [s]
    %
    %   Outputs
    %     dutyHeld      Duty cycles the thrusters apply this step, 0 to 1
    %
    %   See also diagram.control.allocateThrusters.

    %#codegen

    arguments (Input)
        duty            (8,1) double
        previousDuty    (8,1) double
        time            (1,1) double
        PWMFreq         (1,1) double {mustBePositive}
        baseRate        (1,1) double {mustBePositive}
    end

    % A new PWM period starts on this step if the previous step was in an earlier
    % one (the small offset avoids rounding down at an exact boundary)
    period         = floor(time * PWMFreq + 1e-9);
    previousPeriod = floor((time - baseRate) * PWMFreq + 1e-9);

    if period > previousPeriod
        dutyHeld = duty;            % new period: take the new duty cycles
    else
        dutyHeld = previousDuty;    % same period: keep the held ones
    end
end
