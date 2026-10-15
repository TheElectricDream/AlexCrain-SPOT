classdef Debugging < matlab.System ...
        & coder.ExternalDependency
    %DEBUGGING Prints its input on the platform's console, for debugging a running experiment.
    %   Run the program by hand to see the output. In a simulation it does nothing.
    %   C/C++ code: +spot/+hardware/csrc/debugging (was
    %   Custom_Library/Resources/Debugging/Debugging.m).
    %
    %   Inputs
    %     u  a double scalar
    %
    %   Based on a MathWorks System object template (Copyright 2016 The MathWorks, Inc.).
    %
    %   See also spot.hardware.debugMessages.

    %#codegen
    %#ok<*EMCA>

    methods
        function obj = Debugging(varargin)
            % Name-value pair arguments.
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            % Nothing to set up.
        end

        function stepImpl(obj, u)
            % Generated code: print the input.
            if ~isempty(coder.target)
                coder.cinclude('debugging_header.h')
                coder.ceval('display_input', u);
            end
        end

        function releaseImpl(obj) %#ok<MANU>
            % Nothing to release.
        end
    end

    methods (Access = protected)
        %% Simulink block interface
        function num = getNumInputsImpl(~)
            num = 1;
        end

        function num = getNumOutputsImpl(~)
            num = 0;
        end

        function flag = isInputSizeMutableImpl(~, ~)
            flag = false;   % the input size never changes
        end

        function varargout = isInputFixedSizeImpl(~, ~)
            varargout{1} = true;
        end

        function flag = isInputComplexityMutableImpl(~, ~)
            flag = false;   % always real
        end

        function validateInputsImpl(~, u)
            % Validate the input in simulation only.
            if isempty(coder.target)
                validateattributes(u, {'double'}, {'scalar'}, '', 'u');
            end
        end

        function icon = getIconImpl(~)
            icon = 'Debugging';
        end
    end

    methods (Static, Access = protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end

        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end

        function header = getHeaderImpl
            header = matlab.system.display.Header(mfilename('class'), ...
                'Title', 'Debugging Block', ...
                'Text', ['This simulink block prints the input to the Linux command prompt ' ...
                         'for debugging.  To view the output for this function, you must run ' ...
                         'the generated executable manually (see user manual).' newline]);
        end
    end

    methods (Static)
        %% coder.ExternalDependency interface
        function name = getDescriptiveName()
            name = 'Debugging';
        end

        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end

        function updateBuildInfo(buildInfo, context)
            % Add the C++ source and header to the build.
            if context.isCodeGenTarget('rtw')
                srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'debugging', 'src');
                includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'debugging', ...
                    'include');
                addIncludePaths(buildInfo, includeDir);
                addSourceFiles(buildInfo, 'debugging_function.cpp', srcDir);
                addIncludeFiles(buildInfo, 'debugging_header.h', includeDir);
            end
        end
    end
end
