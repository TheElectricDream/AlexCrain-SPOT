function ip = ipv4(address)
    %IPV4 An IPv4 address as four numbers: [192 168 1 116] from '192.168.1.116'.
    %   ip = spot.params.ipv4(address). The diagram's UDP blocks take the lab console's address
    %   this way (labConsoleIP: a block parameter can't be text); spot.hardware.ipText turns it
    %   back.
    %
    %   Inputs
    %     address  dotted IPv4 address as text
    %
    %   Outputs
    %     ip       1x4 numbers
    %
    %   See also spot.hardware.ipText.

    % Four dot-separated numbers of 1-3 digits.
    parts = regexp(strtrim(char(address)), '^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$', ...
        'tokens', 'once');
    if isempty(parts)
        error('spot:params:ipv4', ...
            ['The lab console address must be an IPv4 address such as 192.168.1.116 (it is ' ...
             '"%s").'], char(address));
    end

    % Each at most 255.
    ip = cellfun(@str2double, parts);
    if any(ip > 255)
        error('spot:params:ipv4', 'The lab console address %s has a number above 255.', ...
            char(address));
    end
end
