function share = thrustShare(duty, decayPerValve)
    %THRUSTSHARE Fraction of its own thrust each thruster gives, with others open at once.
    %   share = diagram.control.thrustShare(duty, decayPerValve)
    %
    %   The thrusters share one air supply, so each gives less thrust while other valves
    %   are open: with n open at once, each gives 1 - decayPerValve*(n - 1) of its thrust
    %   alone. The Teensy opens every valve at the start of each PWM period and closes
    %   each one after its duty cycle, so the number open at each moment follows from the
    %   duty cycles. Each thruster's share is that fraction averaged over its own opening.
    %
    %   Inputs
    %     duty            Duty cycle of each thruster, 0 to 1
    %     decayPerValve   Thrust lost per extra valve open (thrust_decay_<PLATFORM>)
    %
    %   Outputs
    %     share           Fraction of its own thrust each thruster gives, 0 to 1 (1 for a
    %                     thruster that is off)
    %
    %   See also diagram.control.allocateThrusters, diagram.dynamics.thrusterForces.

    %#codegen

    arguments (Input)
        duty            (8,1) double
        decayPerValve   (1,1) double {mustBeNonnegative}
    end

    duty    = max(duty, 0);
    closing = sort(duty);           % when each valve closes, as a fraction of the period
    share   = ones(8, 1);

    for i = 1:8
        if duty(i) > 0
            % Step through the moments valves close, up to when this one does: between
            % them, the valves still open are the ones with a longer duty cycle
            total = 0;
            t0    = 0;
            for k = 1:8
                t1 = min(closing(k), duty(i));
                if t1 > t0
                    open  = sum(duty >= t1);
                    total = total + (t1 - t0) * max(0, 1 - decayPerValve * (open - 1));
                    t0    = t1;
                end
            end
            share(i) = total / duty(i);
        end
    end
end
