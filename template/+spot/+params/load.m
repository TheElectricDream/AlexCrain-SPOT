function s = load(file)
%LOAD Read settings from a JSON file, filling anything missing with the defaults.
%   s = spot.params.load(file). Old GUI state files (.mat) are converted with
%   spot.params.fromLegacyState.
[~, ~, ext] = fileparts(file);
if strcmpi(ext, '.mat')
    s = spot.params.fromLegacyState(file);
    return
end
x = jsondecode(fileread(file));
% Settings saved before the drop pose was removed: the arm's angles were its drop pose, and are
% now its initial pose (the platforms' drop poses are simply dropped)
try
    if isfield(x.initial.ARM, 'drop') && ~isfield(x.initial.ARM, 'initial')
        x.initial.ARM.initial = x.initial.ARM.drop;
    end
catch
end
s = spot.params.merge(spot.params.defaults(), x);
% Settings saved before the PhaseSpace data rate was a setting: it was half the sample rate
if ~isfield(x, 'dataRate') && isfield(x, 'sampleRate') && isnumeric(x.sampleRate) && isscalar(x.sampleRate)
    s.dataRate = min(max(round(x.sampleRate / 2), 1), 960);
end
end
