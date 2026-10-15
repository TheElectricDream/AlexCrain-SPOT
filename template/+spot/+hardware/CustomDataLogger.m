classdef CustomDataLogger < matlab.System ...
        & coder.ExternalDependency
    %CUSTOMDATALOGGER Appends each step's logged values to the platform's ExpLog text file.
    %   The diagram's Data Logger block. In generated code it creates the file at setup and
    %   appends each step's values; in a simulation it does nothing.
    %   C/C++ code: +spot/+hardware/csrc/data_logger (was
    %   Custom_Library/Resources/CustomDataLogger/CustomDataLogger.m).
    %
    %   Inputs
    %     u1  the values to log
    %     u2  their count
    %
    %   Based on a MathWorks System object template (Copyright 2016 The MathWorks, Inc.).
    %
    %   See also spot.experiment.decodeData.

    %#codegen
    %#ok<*EMCA>

    methods
        function obj = CustomDataLogger(varargin)
            % Name-value pair arguments.
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj)
            % Generated code: create the log file.
            if ~isempty(coder.target)
                coder.cinclude('custom_data_logger.h')
                coder.ceval('createFile');
            end
        end

        function stepImpl(obj, u1, u2)
            % Generated code: append this step's values.
            if ~isempty(coder.target)
                coder.cinclude('custom_data_logger.h')
                coder.ceval('appendDataToFile', u1, u2);
            end
        end

        function releaseImpl(obj) %#ok<MANU>
            % Nothing to release.
        end
    end

    methods (Access = protected)
        %% Simulink block interface
        function num = getNumInputsImpl(~)
            num = 2;
        end

        function num = getNumOutputsImpl(~)
            num = 0;
        end

        function varargout = isInputFixedSizeImpl(~, ~)
            varargout{1} = true;
        end

        function flag = isInputComplexityMutableImpl(~, ~)
            flag = false;   % always real
        end

        function icon = getIconImpl(~)
            icon = 'CustomDataLogger';
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
                'Title', 'CustomDataLogger Block', ...
                'Text', ['This simulink block takes an input array and the size of the array ' ...
                         'and appends the data to a text file.' newline]);
        end
    end

    methods (Static)
        %% coder.ExternalDependency interface
        function name = getDescriptiveName()
            name = 'CustomDataLogger';
        end

        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end

        function updateBuildInfo(buildInfo, context)
            % Add the logger's C++ source and header to the build.
            if context.isCodeGenTarget('rtw')
                srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'data_logger', 'src');
                includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'data_logger', ...
                    'include');
                addIncludePaths(buildInfo, includeDir);
                addSourceFiles(buildInfo, 'custom_data_logger.cpp', srcDir);
                addIncludeFiles(buildInfo, 'custom_data_logger.h', includeDir);
            end
        end
    end
end
