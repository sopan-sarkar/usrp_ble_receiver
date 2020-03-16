/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: gmskdemod_emxutil.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 29-Aug-2019 03:51:24
 */

#ifndef GMSKDEMOD_EMXUTIL_H
#define GMSKDEMOD_EMXUTIL_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "gmskdemod_types.h"

/* Function Declarations */
extern void emxEnsureCapacity_creal32_T(emxArray_creal32_T *emxArray, int
  oldNumel);
extern void emxEnsureCapacity_real32_T(emxArray_real32_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxFree_creal32_T(emxArray_creal32_T **pEmxArray);
extern void emxFree_real32_T(emxArray_real32_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_creal32_T(emxArray_creal32_T **pEmxArray, int numDimensions);
extern void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for gmskdemod_emxutil.h
 *
 * [EOF]
 */
