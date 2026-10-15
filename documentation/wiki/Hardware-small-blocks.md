# Data Logger, Debugging, WhoAmI (C++)

Three small C++ files behind three System object blocks ([[spot.hardware.CustomDataLogger|spot-hardware-CustomDataLogger]], [[spot.hardware.Debugging|spot-hardware-Debugging]], [[spot.hardware.IdentifyPlatform|spot-hardware-IdentifyPlatform]]). They only run in the program built for a platform.

<!-- source: +spot/+hardware/csrc/data_logger/include/custom_data_logger.h -->
<!-- source: +spot/+hardware/csrc/data_logger/src/custom_data_logger.cpp -->
<!-- source: +spot/+hardware/csrc/debugging/include/debugging_header.h -->
<!-- source: +spot/+hardware/csrc/debugging/src/debugging_function.cpp -->
<!-- source: +spot/+hardware/csrc/identify_platform/include/resource_headers.h -->
<!-- source: +spot/+hardware/csrc/identify_platform/src/resource_functions.cpp -->

## `custom_data_logger.cpp`: the ExpLog file

The *Data Logger* block writes one line per step: the values of the *Data Logging Bus*, comma-separated, time first. [[spot.experiment.decodeData|spot-experiment-decodeData]] reads these files back.

- `createFile()` (lines 12–37): find an unused name in the program's working folder, `ExpLog`, `ExpLog1`, `ExpLog2`, … (so a second run on the platform doesn't overwrite the first), and create it empty. `newName` remembers it.
- `appendDataToFile(data, n)` (lines 43–57): open the file for appending, write the `n` values separated by commas and end the line, close it. Opening and closing every step is slow but simple, and keeps the file complete if the program is killed.

## `debugging_function.cpp`: print a value

`display_input(x)` (lines 12–17) is `printf("Input Data: %f\n", x)`. Seen only when the program is run by hand on the platform (the lab console starts experiments in the background and their output goes to `run.log`, see [[spot_agent.sh|Hardware-Deploy-Scripts]]).

## `resource_functions.cpp`: which platform am I?

- `getComputerIdentifier(hostname)` (lines 7–21): `spot-red` → 1, `spot-black` → 2, `spot-blue` → 3, `spot-spare` → 4, `spot-vision` → 5, anything else 0.
- `WhoAmI()` (lines 23–34): `gethostname`, then the above; if the hostname can't be read it prints an error and returns 1 (the Chaser).

The diagram stores the result in the `WhoAmI` data store and every platform-specific subsystem is enabled by it, which is how the same program behaves differently on each platform.

## The code
<!-- listing -->
<details>
<summary><b>+spot/+hardware/csrc/data_logger/include/custom_data_logger.h</b> (8 lines; click to show)</summary>

```c
   1  #include <string.h>
   2  #include <fstream>
   3  using namespace std;
   4  
   5  #include "rtwtypes.h"
   6  
   7  void createFile();
   8  void appendDataToFile(double exp_data[], int num_params);
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/data_logger/src/custom_data_logger.cpp</b> (57 lines; click to show)</summary>

```cpp
   1          #include <iostream>
   2          #include <iomanip>
   3          #include <fstream>
   4          #include <ctime>
   5          
   6          #include "custom_data_logger.h"
   7          
   8          using namespace std;
   9          
  10          string newName = ""; 
  11          
  12          void createFile() {
  13              
  14              string name = "ExpLog";
  15              int i = 1;
  16              newName = name;
  17              while(true){
  18              
  19                  ifstream file(newName);
  20                  if(file.good()){
  21                   
  22                      newName = name + to_string(i);
  23                      i++;
  24                      
  25                  } else {
  26                  
  27                      break;
  28                      
  29                  }
  30                         
  31                  
  32              }
  33              
  34              ofstream file(newName);
  35              file.close();
  36             
  37          }
  38          
  39          // This function opens a text file and appends data into it.
  40          // The function will accept an array of unknown size, and will
  41          // loop through the array and append the data to the file.
  42          
  43          void appendDataToFile(double exp_data[], int num_params)
  44          {
  45              
  46              ofstream myfile;
  47              myfile.open (newName, ios::app);
  48              
  49              for (int i = 0; i < num_params-1; i++) {
  50                  myfile << exp_data[i] << ","; 
  51              }
  52              
  53              myfile << exp_data[num_params-1];
  54              myfile << endl;
  55              
  56              myfile.close();
  57          }
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/debugging/include/debugging_header.h</b> (3 lines; click to show)</summary>

```c
   1  #include "rtwtypes.h"
   2  
   3  double display_input(double input_data);
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/debugging/src/debugging_function.cpp</b> (17 lines; click to show)</summary>

```cpp
   1  // io/read-file-sum.cpp - Read integers from file and print sum.
   2  // Fred Swartz 2003-08-20
   3  
   4  #include <iostream>
   5  #include <iomanip>
   6  #include <fstream>
   7  
   8  #include "debugging_header.h"
   9  
  10  using namespace std;
  11  
  12  double display_input(double input_data)
  13  {
  14    
  15    printf("Input Data: %f\n",input_data);
  16  
  17  }
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/identify_platform/include/resource_headers.h</b> (4 lines; click to show)</summary>

```c
   1  #include "rtwtypes.h"
   2  
   3  int WhoAmI();
   4  int getComputerIdentifier(const char* hostname);
```

</details>

<details>
<summary><b>+spot/+hardware/csrc/identify_platform/src/resource_functions.cpp</b> (34 lines; click to show)</summary>

```cpp
   1      #include <iostream>
   2      #include <unistd.h>
   3      #include <cstring>
   4      
   5      #include "resource_headers.h"
   6      
   7      int getComputerIdentifier(const char* hostname) {
   8          if (std::strcmp(hostname, "spot-red") == 0) {
   9              return 1;
  10          } else if (std::strcmp(hostname, "spot-black") == 0) {
  11              return 2;
  12          } else if (std::strcmp(hostname, "spot-blue") == 0) {
  13              return 3;
  14          } else if (std::strcmp(hostname, "spot-spare") == 0) {
  15              return 4;
  16          } else if (std::strcmp(hostname, "spot-vision") == 0) {
  17              return 5;
  18          } else {
  19              return 0;  // No match found
  20          }
  21      }
  22      
  23      int WhoAmI() {
  24          char hostname[256];
  25          if (gethostname(hostname, sizeof(hostname)) == 0) {
  26              int identifier = getComputerIdentifier(hostname);
  27              return identifier;
  28          } else {
  29              std::cerr << "Failed to get the hostname." << std::endl;
  30              return 1;
  31          }
  32      
  33          return 0;
  34      }
```

</details>
<!-- /listing -->
