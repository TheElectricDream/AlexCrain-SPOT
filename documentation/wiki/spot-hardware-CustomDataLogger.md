# spot.hardware.CustomDataLogger

**File:** `+spot/+hardware/CustomDataLogger.m` · **Package:** [[spot.hardware|spot-hardware]] · **C++:** [[custom_data_logger.cpp|Hardware-small-blocks]]

<!-- source: +spot/+hardware/CustomDataLogger.m -->

## What it is for

The diagram's *Data Logger* block (inside *Data Logger Subsystem*): on a platform, append each step's logged values as one line of text to the platform's `ExpLog` file, which the lab console collects after the experiment and [[spot.experiment.decodeData|spot-experiment-decodeData]] turns into `.mat`. In a simulation it does nothing (Simulink's own logging gives `dataClass`). The file is created in `setupImpl`, so each run starts a new log.

It was MathWorks' sink-block template with two calls filled in; the template's empty sections are still there.

## Line by line

**Lines 42–50.** Setup: in generated code, `createFile()` opens a new `ExpLog` file.

**Lines 52–60.** Step: the two inputs are the array of values and its size; `appendDataToFile(u1, u2)` writes one line.

**Lines 62–69.** Release: nothing (the file is closed by the C++ code).

**Lines 74–93.** Two inputs, no outputs, fixed-size real inputs, the icon.

**Lines 98–110.** Interpreted execution (hidden), the block's help.

**Lines 115–141.** Code generation: `rtw` targets; add `csrc/data_logger/src/custom_data_logger.cpp` and its header.

<!-- xref -->
- **Used by:** nothing in the code (called by you, from the command window, or by the diagram)
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/CustomDataLogger.m</b> (143 lines; click to show)</summary>

```matlab
   1  classdef CustomDataLogger < matlab.System ...
   2          & coder.ExternalDependency
   3  %CUSTOMDATALOGGER Appends each step's logged values to the platform's ExpLog text file (the diagram's
   4  %   Data Logger block).
   5  %   C/C++ code: +spot/+hardware/csrc/data_logger. (Was Custom_Library/Resources/CustomDataLogger/CustomDataLogger.m.)
   6      %
   7      % System object template for a sink block.
   8      % 
   9      % This template includes most, but not all, possible properties,
  10      % attributes, and methods that you can implement for a System object in
  11      % Simulink.
  12      %
  13      % NOTE: When renaming the class name Sink, the file name and
  14      % constructor name must be updated to use the class name.
  15      %
  16      
  17      % Copyright 2016 The MathWorks, Inc.
  18      %#codegen
  19      %#ok<*EMCA>
  20      
  21      properties
  22          
  23      end
  24      
  25      properties (Nontunable)
  26  
  27      end
  28      
  29      properties (Access = private)
  30     
  31      end
  32      
  33      methods
  34          % Constructor
  35          function obj = CustomDataLogger(varargin)
  36              % Support name-value pair arguments when constructing the object.
  37              setProperties(obj,nargin,varargin{:});
  38          end
  39      end
  40      
  41      methods (Access=protected)
  42          function setupImpl(obj)
  43              if isempty(coder.target)
  44                  % Place simulation setup code here
  45              else
  46                  % Call C-function implementing device initialization
  47                   coder.cinclude('custom_data_logger.h')
  48                   coder.ceval('createFile');
  49              end
  50          end
  51          
  52          function stepImpl(obj,u1,u2)  
  53              if isempty(coder.target)
  54                  % Place simulation output code here 
  55              else
  56                  % Call C-function implementing device output
  57                   coder.cinclude('custom_data_logger.h')
  58                   coder.ceval('appendDataToFile',u1, u2);
  59              end
  60          end
  61          
  62          function releaseImpl(obj) %#ok<MANU>
  63              if isempty(coder.target)
  64                  % Place simulation termination code here
  65              else
  66                  % Call C-function implementing device termination
  67                  %coder.ceval('sink_terminate');
  68              end
  69          end
  70      end
  71      
  72      methods (Access=protected)
  73          %% Define input properties
  74          function num = getNumInputsImpl(~)
  75              num = 2;
  76          end
  77          
  78          function num = getNumOutputsImpl(~)
  79              num = 0;
  80          end
  81          
  82          function varargout = isInputFixedSizeImpl(~,~)
  83              varargout{1} = true;
  84          end
  85          
  86          function flag = isInputComplexityMutableImpl(~,~)
  87              flag = false;   % always real
  88          end
  89                
  90          function icon = getIconImpl(~)
  91              % Define a string as the icon for the System block in Simulink.
  92              icon = 'CustomDataLogger';
  93          end  
  94          
  95      end
  96      
  97      methods (Static, Access=protected)
  98          function simMode = getSimulateUsingImpl(~)
  99              simMode = 'Interpreted execution';
 100          end
 101          
 102          function isVisible = showSimulateUsingImpl
 103              isVisible = false;
 104          end
 105          
 106          function header = getHeaderImpl
 107              header = matlab.system.display.Header(mfilename('class'),'Title',...
 108                  'CustomDataLogger Block','Text',...
 109                  ['This simulink block takes an input array and the size of the array and appends the data to a text file.' newline]);
 110          end
 111          
 112      end
 113      
 114      methods (Static)
 115          function name = getDescriptiveName()
 116              name = 'CustomDataLogger';
 117          end
 118          
 119          function b = isSupportedContext(context)
 120              b = context.isCodeGenTarget('rtw');
 121          end
 122          
 123          function updateBuildInfo(buildInfo, context)
 124              if context.isCodeGenTarget('rtw')
 125                  % Update buildInfo
 126                  srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'data_logger', 'src');
 127                  includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'data_logger', 'include');
 128                  addIncludePaths(buildInfo,includeDir);
 129                  
 130                  % Add all SOURCE files for compiling robotis software
 131                  addSourceFiles(buildInfo,'custom_data_logger.cpp', srcDir);
 132                  
 133                  % Add all INCLUDE files for compiling robotis software
 134                  addIncludeFiles(buildInfo,'custom_data_logger.h',includeDir);
 135  
 136                  %addLinkFlags(buildInfo,{'-lSource'});
 137                  %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
 138                  %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
 139                  %addDefines(buildInfo,'MY_DEFINE_1')
 140              end
 141          end
 142      end
 143  end
```

</details>
<!-- /listing -->
