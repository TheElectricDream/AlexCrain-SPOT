function text = calibrationRecord(newText)
    %CALIBRATIONRECORD The lab calibration's full record, as the project took it.
    %   text = spot.params.calibrationRecord() returns the lab console's calibration as it was
    %   when the project last took it (Update from Lab), as the JSON text the lab console sent:
    %   the numbers (which the project keeps in Constants.LAB_*) with the measurements behind
    %   them, the table's drift paths, each thruster's burns and spread, the arm's sweeps and
    %   steps, and what each run assumed. '' when the project has none.
    %   spot.params.calibrationRecord(text) keeps a new one (the worker, on Update from Lab).
    %
    %   The record is measurement data, like a run in Saved Data, not configuration: nothing
    %   reads a value from it. The Calibration pane draws it (the local console's Python reads
    %   it through the MATLAB Engine), and only where its section's time matches the LAB_*
    %   constants', so a value changed in Constants.m never shows beside an old record. It
    %   lives in Saved Data/Calibration/lab_calibration.mat (the variable record), next to the
    %   project's runs; the Results page leaves that folder out.
    %
    %   Inputs
    %     newText  the lab console's calibration, as JSON text (optional)
    %
    %   Outputs
    %     text     the record, as JSON text; jsondecode(text) gives it as a struct
    %
    %   See also spot.params.labCalibration, spot.worker.labConstants.

    %% Where
    folder = fullfile(fileparts(Constants.file()), 'Saved Data', 'Calibration');
    file = fullfile(folder, 'lab_calibration.mat');

    %% Keep a new one
    if nargin > 0
        if ~isfolder(folder)
            mkdir(folder);
        end
        record = char(newText);   % saved by name
        save(file, 'record');
        text = record;
        return
    end

    %% Read
    text = '';
    if isfile(file)
        saved = load(file, 'record');
        if isfield(saved, 'record') && ischar(saved.record)
            text = saved.record;
        end
    end
end
