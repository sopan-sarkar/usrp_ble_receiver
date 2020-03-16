/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: gmskdemod_emxAPI.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 29-Aug-2019 03:51:24
 */

#ifndef GMSKDEMOD_EMXAPI_H
#define GMSKDEMOD_EMXAPI_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "gmskdemod_types.h"

/* Function Declarations */
extern emxArray_real32_T *emxCreateND_real32_T(int numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapperND_real32_T(float *data, int
  numDimensions, int *size);
extern emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows, int
  cols);
extern emxArray_real32_T *emxCreate_real32_T(int rows, int cols);
extern void emxDestroyArray_real32_T(emxArray_real32_T *emxArray);
extern void emxInitArray_real32_T(emxArray_real32_T **pEmxArray, int
  numDimensions);

#endif

/*
 * File trailer for gmskdemod_emxAPI.h
 *
 * [EOF]
 */
