function s = ipText(ip)
    %IPTEXT An IPv4 address as text for the C code: '192.168.1.116' from [192 168 1 116].
    %   s = spot.hardware.ipText(ip)
    %
    %   Inputs
    %     ip  address as text (returned unchanged) or four numbers (e.g. the workspace's
    %         labConsoleIP, a block parameter)
    %
    %   Outputs
    %     s   1x16 char: at most 15 characters padded with NUL, so the generated code has a
    %         fixed-size string
    %
    %   See also spot.hardware.udpSend, spot.hardware.Udp.

    %#codegen

    %% Constants
    ZERO_CODE = 48;   % character code of '0'

    if ischar(ip)
        s = ip;
        return
    end

    %% Write out the four numbers, dot-separated
    s = char(zeros(1, 16));
    k = 0;
    for part = 1:4

        % Decimal digits, without leading zeros.
        octet = round(double(ip(part)));
        digits = [floor(octet / 100), mod(floor(octet / 10), 10), mod(octet, 10)];
        first = 1 + (octet < 100) + (octet < 10);
        for j = first:3
            k = k + 1;
            s(k) = char(ZERO_CODE + digits(j));
        end

        % Separator.
        if part < 4
            k = k + 1;
            s(k) = '.';
        end
    end
end
