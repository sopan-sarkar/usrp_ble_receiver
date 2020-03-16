//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 29-Aug-2019 03:51:15
//

//***********************************************************************
// This automatically generated example C++ main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "gmskdemod.h"
#include "main.h"
#include "gmskdemod_terminate.h"
#include "gmskdemod_emxAPI.h"
#include "gmskdemod_initialize.h"

// Function Declarations
static void argInit_375000x1_creal32_T(creal32_T result[375000]);
static creal32_T argInit_creal32_T();
static float argInit_real32_T();
static double argInit_real_T();
static void main_gmskdemod();

// Function Definitions

//
// Arguments    : creal32_T result[375000]
// Return Type  : void
//
static void argInit_375000x1_creal32_T(creal32_T result[375000])
{
  int idx0;

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < 375000; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_creal32_T();
  }
}

//
// Arguments    : void
// Return Type  : creal32_T
//
static creal32_T argInit_creal32_T()
{
  creal32_T result;
  float result_tmp;

  // Set the value of the complex variable.
  // Change this value to the value that the application requires.
  result_tmp = argInit_real32_T();
  result.re = result_tmp;
  result.im = result_tmp;
  return result;
}

//
// Arguments    : void
// Return Type  : float
//
static float argInit_real32_T()
{
  return 0.0F;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_gmskdemod()
{
  emxArray_real32_T *z;
  static creal32_T fcv0[375000];
  emxInitArray_real32_T(&z, 1);

  // Initialize function 'gmskdemod' input arguments.
  // Initialize function input argument 'y'.
  // Call the entry-point 'gmskdemod'.
  argInit_375000x1_creal32_T(fcv0);
  gmskdemod(fcv0, argInit_real_T(), z);
  emxDestroyArray_real32_T(z);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  gmskdemod_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_gmskdemod();

  // Terminate the application.
  // You do not need to do this more than one time.
  gmskdemod_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
