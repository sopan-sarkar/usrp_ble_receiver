/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sign.c
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 29-Aug-2019 03:51:24
 */

/* Include Files */
#include "gmskdemod.h"
#include "sign.h"

/* Function Definitions */

/*
 * Arguments    : emxArray_real32_T *x
 * Return Type  : void
 */
void b_sign(emxArray_real32_T *x)
{
  int nx;
  int k;
  float b_x;
  nx = x->size[0];
  for (k = 0; k < nx; k++) {
    b_x = x->data[k];
    if (x->data[k] < 0.0F) {
      b_x = -1.0F;
    } else if (x->data[k] > 0.0F) {
      b_x = 1.0F;
    } else {
      if (x->data[k] == 0.0F) {
        b_x = 0.0F;
      }
    }

    x->data[k] = b_x;
  }
}

/*
 * File trailer for sign.c
 *
 * [EOF]
 */
