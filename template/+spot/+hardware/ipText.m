function s = ipText(ip)
%IPTEXT An IPv4 address as text for the C code: '192.168.1.116' from [192 168 1 116].
%   s = spot.hardware.ipText(ip): ip as text is returned as it is; as four numbers (e.g. the
%   workspace's labConsoleIP, a block parameter) it is written out as 15 characters at most,
%   padded with NUL characters, so the generated code has a fixed-size string.
%#codegen
if ischar(ip)
    s = ip;
    return
end
s = char(zeros(1, 16));
k = 0;
for i = 1:4
    v = round(double(ip(i)));
    digits = [floor(v / 100), mod(floor(v / 10), 10), mod(v, 10)];
    first = 1 + (v < 100) + (v < 10);
    for j = first:3
        k = k + 1;
        s(k) = char(48 + digits(j));
    end
    if i < 4
        k = k + 1;
        s(k) = '.';
    end
end
end
