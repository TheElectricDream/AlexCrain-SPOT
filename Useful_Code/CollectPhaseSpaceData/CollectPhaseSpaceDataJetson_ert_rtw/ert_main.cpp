//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
//
// Code generated for Simulink model 'CollectPhaseSpaceDataJetson'.
//
// Model version                  : 5.2
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Sun Jul 17 13:19:08 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Emulation hardware selection:
//    Differs from embedded hardware (MATLAB Host)
// Code generation objective: Execution efficiency
// Validation result: Not run
//
#include <stdio.h>
#include <stdlib.h>
#include "CollectPhaseSpaceDataJetson.h"
#include "CollectPhaseSpaceDataJetson_private.h"
#include "rtwtypes.h"
#include "limits.h"
#include "MW_raspi_init.h"
#include "MW_Pyserver_control.h"
#include "linuxinitialize.h"
#define UNUSED(x)                      x = x
#define NAMELEN                        16

// Function prototype declaration
void exitFcn(int sig);
void *terminateTask(void *arg);
void *baseRateTask(void *arg);
void *subrateTask(void *arg);
volatile boolean_T stopRequested = false;
volatile boolean_T runModel = true;
sem_t stopSem;
sem_t baserateTaskSem;
pthread_t schedulerThread;
pthread_t baseRateThread;
void *threadJoinStatus;
int terminatingmodel = 0;
void *baseRateTask(void *arg)
{
  runModel = (rtmGetErrorStatus(CollectPhaseSpaceDataJetson_M) == (NULL)) &&
    !rtmGetStopRequested(CollectPhaseSpaceDataJetson_M);
  while (runModel) {
    sem_wait(&baserateTaskSem);
    CollectPhaseSpaceDataJetson_step();

    // Get model outputs here
    stopRequested = !((rtmGetErrorStatus(CollectPhaseSpaceDataJetson_M) == (NULL))
                      && !rtmGetStopRequested(CollectPhaseSpaceDataJetson_M));
  }

  runModel = 0;
  terminateTask(arg);
  pthread_exit((void *)0);
  return NULL;
}

void exitFcn(int sig)
{
  UNUSED(sig);
  rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, "stopping the model");
  runModel = 0;
}

void *terminateTask(void *arg)
{
  UNUSED(arg);
  terminatingmodel = 1;

  {
    runModel = 0;
  }

  MW_killPyserver();
  mwRaspiTerminate();

  // Terminate model
  CollectPhaseSpaceDataJetson_terminate();
  sem_post(&stopSem);
  return NULL;
}

int main(int argc, char **argv)
{
  UNUSED(argc);
  UNUSED(argv);
  mwRaspiInit();
  MW_launchPyserver();
  rtmSetErrorStatus(CollectPhaseSpaceDataJetson_M, 0);

  // Initialize model
  CollectPhaseSpaceDataJetson_initialize();

  // Call RTOS Initialization function
  myRTOSInit(0.05, 0);

  // Wait for stop semaphore
  sem_wait(&stopSem);

#if (MW_NUMBER_TIMER_DRIVEN_TASKS > 0)

  {
    int i;
    for (i=0; i < MW_NUMBER_TIMER_DRIVEN_TASKS; i++) {
      CHECK_STATUS(sem_destroy(&timerTaskSem[i]), 0, "sem_destroy");
    }
  }

#endif

  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
