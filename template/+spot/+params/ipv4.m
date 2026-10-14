function ip = ipv4(address)
%IPV4 An IPv4 address as four numbers: [192 168 1 116] from '192.168.1.116'.
%   ip = spot.params.ipv4(address). The diagram's UDP blocks take the lab console's address this
%   way (labConsoleIP: a block parameter can't be text); spot.hardware.ipText turns it back.
parts = regexp(strtrim(char(address)), '^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$', 'tokens', 'once');
if isempty(parts)
    error('spot:params:ipv4', 'The lab console address must be an IPv4 address such as 192.168.1.116 (it is "%s").', char(address));
end
ip = cellfun(@str2double, parts);
if any(ip > 255)
    error('spot:params:ipv4', 'The lab console address %s has a number above 255.', char(address));
end
end
