function duty = dropShortPulses(duty, PWMFreq)
    %DROPSHORTPULSES Turn off thrusters whose pulse is shorter than a valve can open.
    %   duty = diagram.control.dropShortPulses(duty, PWMFreq)
    %
    %   A solenoid valve needs about 7 ms to open, so a pulse shorter than that gives no
    %   useful thrust. Duty cycles at or below 7 ms of the PWM period (3.5 % at 5 Hz) are
    %   set to 0. 
    %
    %   Inputs
    %     duty      Duty cycle of each thruster, 0 to 1
    %     PWMFreq   Thrusters' PWM frequency [Hz]
    %
    %   Outputs
    %     duty      The same, with pulses too short for the valves set to 0
    %
    %   See also diagram.control.allocateThrusters, diagram.control.holdDuty.

    %#codegen

    arguments (Input)
        duty        (8,1) double
        PWMFreq     (1,1) double {mustBePositive}
    end

    VALVE_TIME = 0.007;     % shortest pulse a valve responds to [s] 

    duty(duty <= VALVE_TIME * PWMFreq) = 0;
end
