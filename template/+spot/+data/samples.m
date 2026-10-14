function [t, y] = samples(ts)
%SAMPLES Time and data of a logged signal as plain columns.
%   [t, y] = spot.data.samples(ts) returns the time stamps (N x 1, each once, increasing) and the
%   data as an N x m double matrix, whether Simulink logged it as N x m or as a x b x N.
t = double(ts.Time(:));
y = double(ts.Data);
if ndims(y) == 3 || (size(y, 1) ~= numel(t) && size(y, ndims(y)) == numel(t))
    y = reshape(y, [], numel(t)).';
end
[t, keep] = unique(t);
y = y(keep, :);
end
