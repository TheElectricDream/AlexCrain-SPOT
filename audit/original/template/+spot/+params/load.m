function s = load(file)
    %LOAD Read settings from a JSON file, filling anything missing with the defaults.
    %   s = spot.params.load(file). Old GUI state files (.mat) are converted with
    %   spot.params.fromLegacyState.
    %
    %   Inputs
    %     file  settings file (.json), or an old GUI state file (.mat)
    %
    %   Outputs
    %     s     settings
    %
    %   See also spot.params.save, spot.params.merge, spot.params.fromLegacyState.

    %% Constants
    MAX_DATA_RATE = 960;   % [Hz] PhaseSpace

    %% Old GUI state file
    [~, ~, ext] = fileparts(file);
    if strcmpi(ext, '.mat')
        s = spot.params.fromLegacyState(file);
        return
    end

    %% JSON settings
    saved = jsondecode(fileread(file));

    % Saved before the drop pose was removed: the arm's drop pose is now its initial pose (the
    % platforms' drop poses are simply dropped).
    try
        if isfield(saved.initial.ARM, 'drop') && ~isfield(saved.initial.ARM, 'initial')
            saved.initial.ARM.initial = saved.initial.ARM.drop;
        end
    catch
    end

    % Over the defaults, so newer fields get their default values.
    s = spot.params.merge(spot.params.defaults(), saved);

    % Saved before the PhaseSpace data rate was a setting: it was half the sample rate.
    if ~isfield(saved, 'dataRate') && isfield(saved, 'sampleRate') ...
            && isnumeric(saved.sampleRate) && isscalar(saved.sampleRate)
        s.dataRate = min(max(round(saved.sampleRate / 2), 1), MAX_DATA_RATE);
    end
end
