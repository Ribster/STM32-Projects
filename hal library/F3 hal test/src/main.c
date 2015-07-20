//
// This file is part of the GNU ARM Eclipse Plug-ins project.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "diag/Trace.h"
#include "Timer.h"

// ----------------------------------------------------------------------------
//
// Print a greeting message on the trace device and enter a loop
// to count seconds.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//
// ----------------------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main (int argc, char* argv[])
{
  // Normally at this stage most of the microcontroller subsystems, including
  // the clock, were initialised by the CMSIS SystemInit() function invoked
  // from the startup file, before calling main().
  // (see system/src/cortexm/_initialize_hardware.c)
  // If further initialisations are required, customise __initialize_hardware()
  // or add the additional initialisation here, for example:
  //
  // HAL_Init();

  // In this sample the SystemInit() function is just a placeholder,
  // if you do not add the real one, the clock will remain configured with
  // the reset value, usually a relatively low speed RC clock (8-12MHz).

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %uHz\n", SystemCoreClock);

  timer_start ();

  int seconds = 0;

  // Infinite loop
  while (1)
    {
      timer_sleep (TIMER_FREQUENCY_HZ);

      ++seconds;

      // Count seconds on the trace device.
      trace_printf ("Second %d\n", seconds);
    }
  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
