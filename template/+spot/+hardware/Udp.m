classdef Udp < coder.ExternalDependency
%UDP  Connects csrc/udp/spot_udp.c to the generated code (used by the UDP functions here).
%   Used by spot.hardware.udpReceive, udpSend, phasespaceLatest and experimentTime. When a
%   diagram that uses them is built, Simulink calls updateBuildInfo below (as ordinary MATLAB),
%   which adds spot_udp.c and .h to the build. The other methods are the C calls; use the
%   functions named above rather than these.
%#codegen

    methods (Static)
        function name = getDescriptiveName(~)
            name = 'spot_udp';
        end

        function supported = isSupportedContext(~)
            supported = true;
        end

        function updateBuildInfo(buildInfo, ~)
            here = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'udp');
            buildInfo.addIncludePaths(here);
            buildInfo.addSourceFiles('spot_udp.c', here);
            buildInfo.addLinkFlags('-lpthread');  % the receive threads (Linux targets such as the Jetson)
        end

        function h = rxOpen(port, senderIP, minBytes, maxBytes)
            % Listen on a port, keeping the newest packet (see spot_udp.h)
            coder.cinclude('spot_udp.h');
            ip = [spot.hardware.ipText(senderIP) char(0)];   % text, or four numbers
            h = int32(-1);
            h = coder.ceval('spot_udp_rx_open', int32(port), coder.rref(ip), int32(minBytes), int32(maxBytes));
        end

        function [data, newPackets, age] = rxRead(h, n, type)
            % Newest packet as n values of class type, packets since the last read, its age in s
            coder.cinclude('spot_udp.h');
            data = zeros(n, 1, type);
            age = -1;
            newPackets = int32(0);
            newPackets = coder.ceval('spot_udp_rx_read', int32(h), coder.wref(data), ...
                int32(n * spot.hardware.Udp.bytesOf(type)), coder.wref(age));
        end

        function h = txOpen(ip, port)
            % A socket that sends to ip:port
            coder.cinclude('spot_udp.h');
            dest = [spot.hardware.ipText(ip) char(0)];        % text, or four numbers
            h = int32(-1);
            h = coder.ceval('spot_udp_tx_open', coder.rref(dest), int32(port));
        end

        function txSend(h, data)
            % data as one packet, in memory order (little-endian), without waiting
            coder.cinclude('spot_udp.h');
            coder.ceval('spot_udp_tx_send', int32(h), coder.rref(data), ...
                int32(numel(data) * spot.hardware.Udp.bytesOf(class(data))));
        end

        function t = now()
            % A steady clock in seconds (for intervals)
            coder.cinclude('spot_udp.h');
            t = 0;
            t = coder.ceval('spot_udp_now');
        end

        function b = bytesOf(type)
            % Bytes per value of a numeric class
            switch type
                case {'double', 'int64', 'uint64'}
                    b = 8;
                case {'single', 'int32', 'uint32'}
                    b = 4;
                case {'int16', 'uint16'}
                    b = 2;
                otherwise   % int8, uint8, logical, char
                    b = 1;
            end
        end
    end
end
