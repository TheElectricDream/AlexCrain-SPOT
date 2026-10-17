    #include <iostream>
    #include <unistd.h>
    #include <cstring>
    
    #include "resource_headers.h"
    
    // Each platform computer's number from its hostname (Constants.USER_RED etc.): the
    // WHO_AM_I_* values of Constants.m, which C can't read; 0 means a simulation.
    int getComputerIdentifier(const char* hostname) {
        if (std::strcmp(hostname, "spot-red") == 0) {
            return 1;  // Constants.WHO_AM_I_RED
        } else if (std::strcmp(hostname, "spot-black") == 0) {
            return 2;  // Constants.WHO_AM_I_BLACK
        } else if (std::strcmp(hostname, "spot-blue") == 0) {
            return 3;  // Constants.WHO_AM_I_BLUE
        } else if (std::strcmp(hostname, "spot-spare") == 0) {
            return 4;  // Constants.WHO_AM_I_SPARE
        } else if (std::strcmp(hostname, "spot-vision") == 0) {
            return 5;  // Constants.WHO_AM_I_VISION
        } else {
            return 0;  // No match found: Constants.WHO_AM_I_SIMULATION
        }
    }
    
    int WhoAmI() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            int identifier = getComputerIdentifier(hostname);
            return identifier;
        } else {
            std::cerr << "Failed to get the hostname." << std::endl;
            return 1;
        }
    
        return 0;
    }
