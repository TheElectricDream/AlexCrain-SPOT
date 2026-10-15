# spot.hardware.IdentifyPlatform

**File:** `+spot/+hardware/IdentifyPlatform.m` · **Package:** [[spot.hardware|spot-hardware]] · **C++:** [[resource_functions.cpp|Hardware-small-blocks]]

<!-- source: +spot/+hardware/IdentifyPlatform.m -->

## What it is for

The diagram's *WhoAmI* block (in *Platform Identification*): the same program runs on every platform, so each one must find out which it is. On a platform the C++ `WhoAmI()` reads the computer's hostname and returns 1 for `spot-red`, 2 for `spot-black`, 3 for `spot-blue`, 4 for `spot-spare`, 5 for `spot-vision`, 0 otherwise. In a simulation the output is 0 (and [[spot.sim.run|spot-sim-run]] sets the workspace `WhoAmI = 0` too). The diagram stores it in the `WhoAmI` data store, and every platform-specific subsystem is gated on it.

## Line by line

**Lines 1–2.** A source block with its own sample time (`realtime.internal.SourceSampleTime`) and an external dependency.

**Lines 52–61.** Step: 0, or in generated code the C++ `WhoAmI()`.

**Lines 75–106.** No inputs, one scalar double output, fixed size, the icon.

**Lines 110–124.** Interpreted execution; the help text describes the C++.

**Lines 129–152.** Code generation: add `csrc/identify_platform/src/resource_functions.cpp` and its header.

<!-- xref -->
- **Used by:** nothing in the code (called by you, from the command window, or by the diagram)
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/IdentifyPlatform.m</b> (154 lines; click to show)</summary>

```matlab
   1  classdef IdentifyPlatform < realtime.internal.SourceSampleTime & ...
   2          coder.ExternalDependency
   3  %IDENTIFYPLATFORM Which platform the program runs on, from the computer's hostname (the diagram's
   4  %   WhoAmI block): the generated code calls getComputerIdentifier().
   5  %   C/C++ code: +spot/+hardware/csrc/identify_platform. (Was Custom_Library/Resources/IdentifyPlatform/IdentifyPlatform.m.)
   6      %
   7      % System object template for a source block.
   8      % 
   9      % This template includes most, but not all, possible properties,
  10      % attributes, and methods that you can implement for a System object in
  11      % Simulink.
  12      %
  13      % NOTE: When renaming the class name Source, the file name and
  14      % constructor name must be updated to use the class name.
  15      %
  16      
  17      % Copyright 2016 The MathWorks, Inc.
  18      %#codegen
  19      %#ok<*EMCA>
  20      
  21      properties
  22          % Public, tunable properties.
  23      end
  24      
  25      properties (Nontunable)
  26          % Public, non-tunable properties.
  27      end
  28      
  29      properties (Access = private)
  30          % Pre-computed constants.
  31      end
  32      
  33      methods
  34          % Constructor
  35          function obj = IdentifyPlatform(varargin)
  36              % Support name-value pair arguments when constructing the object.
  37              setProperties(obj,nargin,varargin{:});
  38          end
  39      end
  40      
  41      
  42      methods (Access=protected)
  43          function setupImpl(obj) %#ok<MANU>
  44              if isempty(coder.target)
  45                  % Place simulation setup code here
  46              else
  47                  % Call C-function implementing device initialization
  48  
  49              end
  50          end
  51          
  52          function Platform_ID = stepImpl(obj)   %#ok<MANU>
  53              Platform_ID = 0;
  54              if isempty(coder.target)
  55                  % Place simulation output code here
  56              else
  57                  % Call C-function implementing device output
  58                   coder.cinclude('resource_headers.h');
  59                   Platform_ID = coder.ceval('WhoAmI');
  60              end
  61          end
  62          
  63          function releaseImpl(obj) %#ok<MANU>
  64              if isempty(coder.target)
  65                  % Place simulation termination code here
  66              else
  67                  % Call C-function implementing device termination
  68                  %coder.ceval('source_terminate');
  69              end
  70          end
  71      end
  72      
  73      methods (Access=protected)
  74          %% Define output properties
  75          function num = getNumInputsImpl(~)
  76              num = 0;
  77          end
  78          
  79          function num = getNumOutputsImpl(~)
  80              num = 1;
  81          end
  82          
  83          function flag = isOutputSizeLockedImpl(~,~)
  84              flag = true;
  85          end
  86          
  87          function varargout = isOutputFixedSizeImpl(~,~)
  88              varargout{1} = true;
  89          end
  90          
  91          function varargout = isOutputComplexImpl(~)
  92              varargout{1} = false;
  93          end
  94          
  95          function varargout = getOutputSizeImpl(~)
  96              varargout{1} = [1,1];
  97          end
  98          
  99          function varargout = getOutputDataTypeImpl(~)
 100              varargout{1} = 'double';
 101          end
 102          
 103          function icon = getIconImpl(~)
 104              % Define a string as the icon for the System block in Simulink.
 105              icon = 'IdentifyPlatform';
 106          end    
 107      end
 108      
 109      methods (Static, Access=protected)
 110          function simMode = getSimulateUsingImpl(~)
 111              simMode = 'Interpreted execution';
 112          end
 113          
 114          function isVisible = showSimulateUsingImpl
 115              isVisible = false;
 116          end
 117          
 118          function header = getHeaderImpl
 119              header = matlab.system.display.Header(mfilename('class'),'Title',...
 120                  'Determine Identity of Platform','Text',...
 121                  ['This code is a C++ program that retrieves the hostname of the computer running on the Jetson Xavier device. It compares the retrieved hostname with these specific strings: "spot-red", "spot-black", "spot-blue", "spot-spare", and "spot-vision", and returns corresponding integer identifiers.' newline newline ...
 122                  'The getComputerIdentifier() function takes the retrieved hostname as input and compares it with the specified strings using std::strcmp(). If a match is found, the function returns the corresponding identifier (1 for "spot-red", 2 for "spot-black", 3 for "spot-blue", 4 for "spot-spare", and 5 for "spot-vision"). If there is no match, the function returns 0 as the default value. ' newline newline...
 123                  'In the whoAmI() function, the code retrieves the hostname using gethostname() and calls getComputerIdentifier() to obtain the identifier based on the matched hostname. The identifier is returned as an integer and is used in identifying the active platform.']);
 124          end
 125          
 126      end
 127      
 128      methods (Static)
 129          function name = getDescriptiveName()
 130              name = 'Source';
 131          end
 132          
 133          function b = isSupportedContext(context)
 134              b = context.isCodeGenTarget('rtw');
 135          end
 136          
 137          function updateBuildInfo(buildInfo, context)
 138              if context.isCodeGenTarget('rtw')
 139                  % Update buildInfo
 140                  srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'identify_platform', 'src'); 
 141                  includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'identify_platform', 'include');
 142                  addIncludePaths(buildInfo,includeDir);
 143                  % Use the following API's to add include files, sources and
 144                  % linker flags
 145                  addIncludeFiles(buildInfo,'resource_headers.h',includeDir);
 146                  addSourceFiles(buildInfo,'resource_functions.cpp',srcDir);
 147                  %addLinkFlags(buildInfo,{'-lSource'});
 148                  %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
 149                  %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
 150                  %addDefines(buildInfo,'MY_DEFINE_1')
 151              end
 152          end
 153      end
 154  end
```

</details>
<!-- /listing -->
