classdef IdentifyPlatform < realtime.internal.SourceSampleTime & ...
        coder.ExternalDependency
    %IDENTIFYPLATFORM Which platform the program runs on, from the computer's hostname.
    %   The diagram's WhoAmI block: the generated code calls getComputerIdentifier(). In a
    %   simulation it outputs 0.
    %   C/C++ code: +spot/+hardware/csrc/identify_platform (was
    %   Custom_Library/Resources/IdentifyPlatform/IdentifyPlatform.m).
    %
    %   Outputs
    %     Platform_ID  1 spot-red, 2 spot-black, 3 spot-blue, 4 spot-spare, 5 spot-vision,
    %                  0 otherwise
    %
    %   Based on a MathWorks System object template (Copyright 2016 The MathWorks, Inc.).
    %
    %   See also spot.hardware.debugMessages.

    %#codegen
    %#ok<*EMCA>

    methods
        function obj = IdentifyPlatform(varargin)
            % Name-value pair arguments.
            setProperties(obj, nargin, varargin{:});
        end
    end

    methods (Access = protected)
        function setupImpl(obj) %#ok<MANU>
            % Nothing to set up.
        end

        function Platform_ID = stepImpl(obj)   %#ok<MANU>
            % Generated code: the platform's number from its hostname; 0 in a simulation.
            Platform_ID = 0;
            if ~isempty(coder.target)
                coder.cinclude('resource_headers.h');
                Platform_ID = coder.ceval('WhoAmI');
            end
        end

        function releaseImpl(obj) %#ok<MANU>
            % Nothing to release.
        end
    end

    methods (Access = protected)
        %% Simulink block interface
        function num = getNumInputsImpl(~)
            num = 0;
        end

        function num = getNumOutputsImpl(~)
            num = 1;
        end

        function flag = isOutputSizeLockedImpl(~, ~)
            flag = true;
        end

        function varargout = isOutputFixedSizeImpl(~, ~)
            varargout{1} = true;
        end

        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
        end

        function varargout = getOutputSizeImpl(~)
            varargout{1} = [1, 1];
        end

        function varargout = getOutputDataTypeImpl(~)
            varargout{1} = 'double';
        end

        function icon = getIconImpl(~)
            icon = 'IdentifyPlatform';
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
                'Title', 'Determine Identity of Platform', ...
                'Text', ['This code is a C++ program that retrieves the hostname of the ' ...
                         'computer running on the Jetson Xavier device. It compares the ' ...
                         'retrieved hostname with these specific strings: "spot-red", ' ...
                         '"spot-black", "spot-blue", "spot-spare", and "spot-vision", and ' ...
                         'returns corresponding integer identifiers.' newline newline ...
                         'The getComputerIdentifier() function takes the retrieved hostname as ' ...
                         'input and compares it with the specified strings using ' ...
                         'std::strcmp(). If a match is found, the function returns the ' ...
                         'corresponding identifier (1 for "spot-red", 2 for "spot-black", 3 ' ...
                         'for "spot-blue", 4 for "spot-spare", and 5 for "spot-vision"). If ' ...
                         'there is no match, the function returns 0 as the default ' ...
                         'value. ' newline newline ...
                         'In the whoAmI() function, the code retrieves the hostname using ' ...
                         'gethostname() and calls getComputerIdentifier() to obtain the ' ...
                         'identifier based on the matched hostname. The identifier is returned ' ...
                         'as an integer and is used in identifying the active platform.']);
        end
    end

    methods (Static)
        %% coder.ExternalDependency interface
        function name = getDescriptiveName()
            name = 'IdentifyPlatform';
        end

        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end

        function updateBuildInfo(buildInfo, context)
            % Add the C++ source and header to the build.
            if context.isCodeGenTarget('rtw')
                srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', ...
                    'identify_platform', 'src');
                includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', ...
                    'identify_platform', 'include');
                addIncludePaths(buildInfo, includeDir);
                addIncludeFiles(buildInfo, 'resource_headers.h', includeDir);
                addSourceFiles(buildInfo, 'resource_functions.cpp', srcDir);
            end
        end
    end
end
