function cmd = ffmpegCommand(tool, args)
    %FFMPEGCOMMAND A shell command that runs ffmpeg (or ffprobe) with these arguments.
    %   cmd = ffmpegCommand(tool, args) for spot.camera's converters, which hand the video to
    %   ffmpeg (MATLAB's VideoWriter can't write .mkv).
    %
    %   Inputs
    %     tool  "ffmpeg" or "ffprobe"
    %     args  the arguments, a string array (quoted here for the shell)
    %
    %   Outputs
    %     cmd   the command, for system()
    %
    %   Behavior
    %     1. Uses ffmpeg on the PATH; on Linux without MATLAB's library path, which can break
    %        it. Inside a distrobox without ffmpeg, the host's (distrobox-host-exec).
    %     2. Errors with how to install it if neither works. The answer is kept per session.
    %
    %   See also spot.camera.thermalToMkv.

    %% Find ffmpeg once
    persistent prefix
    if isempty(prefix)
        prefix = findPrefix();
    end

    %% The command
    cmd = prefix + tool + " " + strjoin(arrayfun(@shellQuote, string(args)), " ");
end

function prefix = findPrefix()
    %FINDPREFIX What goes before "ffmpeg" in a command that works here.
    candidates = "";
    discard = " > NUL 2>&1";
    if isunix
        candidates = ["env -u LD_LIBRARY_PATH ", "distrobox-host-exec "];
        discard = " > /dev/null 2>&1";
    end
    for c = candidates
        if system(c + "ffmpeg -version" + discard) == 0
            prefix = c;
            return
        end
    end
    error('spot:camera:noFfmpeg', ...
        ['ffmpeg was not found. Install it (Linux: sudo apt install ffmpeg or sudo dnf ' ...
         'install ffmpeg; Windows: winget install ffmpeg) and make sure it is on the PATH.']);
end

function quoted = shellQuote(arg)
    %SHELLQUOTE One argument quoted for the system shell.
    if ispc
        quoted = """" + arg + """";
    else
        quoted = "'" + replace(arg, "'", "'\''") + "'";
    end
end
