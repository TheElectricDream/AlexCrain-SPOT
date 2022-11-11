// io/read-file-sum.cpp - Read integers from file and print sum.
// Fred Swartz 2003-08-20

#include <iostream>
#include <iomanip>
#include <fstream>

#include "control_pwm_headers.h"

using namespace std;

double ControlPWM()
{
  double pwm_Signal = 0;
  std::ifstream pwm_textFile("/home/blue-spot/ExperimentSoftware/PWMDutyCycle");
  pwm_textFile >> pwm_Signal;
  return pwm_Signal;

}