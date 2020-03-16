/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_gmskdemod_api.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 29-Aug-2019 03:51:24
 */

#ifndef _CODER_GMSKDEMOD_API_H
#define _CODER_GMSKDEMOD_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_gmskdemod_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real32_T
#define struct_emxArray_real32_T

struct emxArray_real32_T
{
  real32_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real32_T*/

#ifndef typedef_emxArray_real32_T
#define typedef_emxArray_real32_T

typedef struct emxArray_real32_T emxArray_real32_T;

#endif                                 /*typedef_emxArray_real32_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void gmskdemod(creal32_T y[375000], real_T nSamp, emxArray_real32_T *z);
extern void gmskdemod_api(const mxArray * const prhs[2], int32_T nlhs, const
  mxArray *plhs[1]);
extern void gmskdemod_atexit(void);
extern void gmskdemod_initialize(void);
extern void gmskdemod_terminate(void);
extern void gmskdemod_xil_shutdown(void);
extern void gmskdemod_xil_terminate(void);

#endif

/*
 * File trailer for _coder_gmskdemod_api.h
 *
 * [EOF]
 */
