# spot.hardware.Debugging

**File:** `+spot/+hardware/Debugging.m` · **Package:** [[spot.hardware|spot-hardware]] · **C++:** [[debugging_function.cpp|Hardware-small-blocks]]

<!-- source: +spot/+hardware/Debugging.m -->

## What it is for

A sink block that prints its scalar input on the platform's terminal, for debugging a program run by hand (`ssh` to the platform, run the `.elf`, watch the numbers). Nothing in a simulation. The *Debugger* subsystem's messages to the console are a different mechanism ([[debugMessages|spot-hardware-debugMessages]]).

## Line by line

**Lines 50–58.** Step: in generated code, `display_input(u)` prints the value.

**Lines 72–102.** One scalar double input (validated in simulation), no outputs, the icon.

**Lines 107–121.** Interpreted execution, the help text (with the note about running the executable manually).

**Lines 126–152.** Code generation: add `csrc/debugging/src/debugging_function.cpp` and its header.

<!-- xref -->
- **Used by:** nothing in the code (called by you, from the command window, or by the diagram)
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/Debugging.m</b> (154 lines; click to show)</summary>

```matlab
   1  classdef Debugging < matlab.System ...
   2          & coder.ExternalDependency
   3  %DEBUGGING Prints its input on the platform's console, for debugging a running experiment
   4  %   (run the program by hand to see the output).
   5  %   C/C++ code: +spot/+hardware/csrc/debugging. (Was Custom_Library/Resources/Debugging/Debugging.m.)
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
  35          function obj = Debugging(varargin)
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
  47              end
  48          end
  49          
  50          function stepImpl(obj,u)  
  51              if isempty(coder.target)
  52                  % Place simulation output code here 
  53              else
  54                  % Call C-function implementing device output
  55                   coder.cinclude('debugging_header.h')
  56                   coder.ceval('display_input',u);
  57              end
  58          end
  59          
  60          function releaseImpl(obj) %#ok<MANU>
  61              if isempty(coder.target)
  62                  % Place simulation termination code here
  63              else
  64                  % Call C-function implementing device termination
  65                  %coder.ceval('sink_terminate');
  66              end
  67          end
  68      end
  69      
  70      methods (Access=protected)
  71          %% Define input properties
  72          function num = getNumInputsImpl(~)
  73              num = 1;
  74          end
  75          
  76          function num = getNumOutputsImpl(~)
  77              num = 0;
  78          end
  79          
  80          function flag = isInputSizeMutableImpl(~,~)
  81              flag = false;   % the input size never changes
  82          end
  83          
  84          function varargout = isInputFixedSizeImpl(~,~)
  85              varargout{1} = true;
  86          end
  87          
  88          function flag = isInputComplexityMutableImpl(~,~)
  89              flag = false;   % always real
  90          end
  91          
  92          function validateInputsImpl(~, u)
  93              if isempty(coder.target)
  94                  % Run input validation only in Simulation
  95                  validateattributes(u,{'double'},{'scalar'},'','u');
  96              end
  97          end
  98          
  99          function icon = getIconImpl(~)
 100              % Define a string as the icon for the System block in Simulink.
 101              icon = 'Debugging';
 102          end  
 103          
 104      end
 105      
 106      methods (Static, Access=protected)
 107          function simMode = getSimulateUsingImpl(~)
 108              simMode = 'Interpreted execution';
 109          end
 110          
 111          function isVisible = showSimulateUsingImpl
 112              isVisible = false;
 113          end
 114          
 115          function header = getHeaderImpl
 116              header = matlab.system.display.Header(mfilename('class'),'Title',...
 117                  'Debugging Block','Text',...
 118                  ['This simulink block prints the input to the Linux command prompt for debugging. '...
 119                   ' To view the output for this function, you must run the generated executable manually '...
 120                   '(see user manual).' newline]);
 121          end
 122          
 123      end
 124      
 125      methods (Static)
 126          function name = getDescriptiveName()
 127              name = 'Debugging';
 128          end
 129          
 130          function b = isSupportedContext(context)
 131              b = context.isCodeGenTarget('rtw');
 132          end
 133          
 134          function updateBuildInfo(buildInfo, context)
 135              if context.isCodeGenTarget('rtw')
 136                  % Update buildInfo
 137                  srcDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'debugging', 'src');
 138                  includeDir = fullfile(fileparts(mfilename('fullpath')), 'csrc', 'debugging', 'include');
 139                  addIncludePaths(buildInfo,includeDir);
 140                  
 141                  % Add all SOURCE files for compiling robotis software
 142                  addSourceFiles(buildInfo,'debugging_function.cpp', srcDir);
 143                  
 144                  % Add all INCLUDE files for compiling robotis software
 145                  addIncludeFiles(buildInfo,'debugging_header.h',includeDir);
 146  
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
