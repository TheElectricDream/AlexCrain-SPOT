function [t, y] = samples(ts)
    %SAMPLES Time and data of a logged signal as plain columns.
    %   [t, y] = spot.data.samples(ts)
    %
    %   Inputs
    %     ts  logged timeseries; Simulink logs data as N x m or as a x b x N
    %
    %   Outputs
    %     t   N x 1 time stamps [s], each once, increasing
    %     y   N x m double data
    %
    %   See also interface.results.series, spot.data.flatten.

    t = double(ts.Time(:));
    y = double(ts.Data);

    % a x b x N (time last): one row per sample.
    if ndims(y) == 3 || (size(y, 1) ~= numel(t) && size(y, ndims(y)) == numel(t))
        y = reshape(y, [], numel(t)).';
    end

    % Sorted, with repeated time stamps dropped.
    [t, keep] = unique(t);
    y = y(keep, :);
end
