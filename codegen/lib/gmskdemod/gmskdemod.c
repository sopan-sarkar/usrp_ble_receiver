/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: gmskdemod.c
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 29-Aug-2019 03:51:24
 */

/* Include Files */
#include <math.h>
#include "gmskdemod.h"
#include "gmskdemod_emxutil.h"
#include "sum.h"
#include "sign.h"

/* Function Declarations */
static int div_s32_floor(int numerator, int denominator);
static double rt_roundd_snf(double u);

/* Function Definitions */

/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
static int div_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }

      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }

  return quotient;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * GMSKDEMOD Gaussian minimum shift keying demodulation
 *
 *    Note: This is an internal undocumented function and its API and/or
 *    functionality may change in subsequent releases.
 *
 *    Z = gmskdemod(Y,NSAMP) demodulates the complex envelope Y of a signal
 *    using the Gaussian minimum shift keying method and produces soft
 *    output Z. NSAMP denotes the number of samples per symbol and must be a
 *    positive integer.
 * Arguments    : const creal32_T y[375000]
 *                double nSamp
 *                emxArray_real32_T *z
 * Return Type  : void
 */
void gmskdemod(const creal32_T y[375000], double nSamp, emxArray_real32_T *z)
{
  emxArray_creal32_T *b_y;
  double nBits;
  int i0;
  int loop_ub;
  emxArray_real_T *t;
  int i1;
  double delta1;
  int imax;
  emxArray_real_T *h;
  int nx;
  emxArray_creal32_T *yConv;
  int ma;
  int mpad;
  emxArray_real_T *arg;
  int ic;
  int ia;
  emxArray_real_T *x;
  float im;
  float re;
  emxArray_creal32_T *filtSig;
  double Istart;
  double Qstart;
  double Iend;
  double Qend;
  double d0;
  emxArray_real32_T *A;
  emxArray_real32_T *b_A;
  emxArray_real32_T *zImagDec;
  emxInit_creal32_T(&b_y, 1);

  /*    Copyright 2018-2019 The MathWorks, Inc. */
  /*  signal length in samples */
  nBits = 375000.0 / nSamp;

  /*  signal length in bits */
  /*  Prepend the state */
  i0 = b_y->size[0];
  loop_ub = (int)nSamp;
  b_y->size[0] = loop_ub + 375000;
  emxEnsureCapacity_creal32_T(b_y, i0);
  for (i0 = 0; i0 < loop_ub; i0++) {
    b_y->data[i0].re = 0.0F;
    b_y->data[i0].im = 0.0F;
  }

  for (i0 = 0; i0 < 375000; i0++) {
    b_y->data[i0 + loop_ub] = y[i0];
  }

  emxInit_real_T(&t, 2);

  /*  Perform gaussian matched filtering */
  i0 = t->size[0] * t->size[1];
  t->size[0] = 1;
  t->size[1] = (int)(2.0 * nSamp + 1.0);
  emxEnsureCapacity_real_T(t, i0);
  t->data[t->size[1] - 1] = 1.0;
  if (t->size[1] >= 2) {
    t->data[0] = -1.0;
    if (t->size[1] >= 3) {
      delta1 = 2.0 / ((double)t->size[1] - 1.0);
      i0 = t->size[1];
      for (imax = 0; imax <= i0 - 3; imax++) {
        t->data[1 + imax] = -1.0 + (1.0 + (double)imax) * delta1;
      }
    }
  }

  i0 = t->size[0] * t->size[1];
  i1 = t->size[0] * t->size[1];
  t->size[0] = 1;
  emxEnsureCapacity_real_T(t, i1);
  loop_ub = i0 - 1;
  for (i0 = 0; i0 <= loop_ub; i0++) {
    t->data[i0] = t->data[i0] * 3.1415926535897931 / 1.1774100225154747;
  }

  emxInit_real_T(&h, 2);
  i0 = h->size[0] * h->size[1];
  h->size[0] = 1;
  h->size[1] = t->size[1];
  emxEnsureCapacity_real_T(h, i0);
  nx = t->size[1];
  for (imax = 0; imax < nx; imax++) {
    h->data[imax] = t->data[imax] * t->data[imax];
  }

  emxFree_real_T(&t);
  i0 = h->size[0] * h->size[1];
  i1 = h->size[0] * h->size[1];
  h->size[0] = 1;
  emxEnsureCapacity_real_T(h, i1);
  loop_ub = i0 - 1;
  for (i0 = 0; i0 <= loop_ub; i0++) {
    h->data[i0] = -h->data[i0];
  }

  nx = h->size[1];
  for (imax = 0; imax < nx; imax++) {
    h->data[imax] = exp(h->data[imax]);
  }

  i0 = h->size[0] * h->size[1];
  i1 = h->size[0] * h->size[1];
  h->size[0] = 1;
  emxEnsureCapacity_real_T(h, i1);
  loop_ub = i0 - 1;
  for (i0 = 0; i0 <= loop_ub; i0++) {
    h->data[i0] *= 1.5053836955785049;
  }

  nx = h->size[1];
  delta1 = h->data[0];
  for (imax = 2; imax <= nx; imax++) {
    delta1 += h->data[imax - 1];
  }

  i0 = h->size[0] * h->size[1];
  i1 = h->size[0] * h->size[1];
  h->size[0] = 1;
  emxEnsureCapacity_real_T(h, i1);
  loop_ub = i0 - 1;
  for (i0 = 0; i0 <= loop_ub; i0++) {
    h->data[i0] /= delta1;
  }

  emxInit_creal32_T(&yConv, 1);
  ma = b_y->size[0];
  i0 = yConv->size[0];
  yConv->size[0] = b_y->size[0];
  emxEnsureCapacity_creal32_T(yConv, i0);
  loop_ub = b_y->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    yConv->data[i0].re = 0.0F;
    yConv->data[i0].im = 0.0F;
  }

  mpad = (h->size[1] >> 1) + (h->size[1] & 1);
  i0 = h->size[1] - 1;
  for (loop_ub = 0; loop_ub <= i0; loop_ub++) {
    delta1 = h->data[(h->size[1] - loop_ub) - 1];
    if (loop_ub < mpad - 1) {
      nx = mpad - loop_ub;
      imax = ma;
    } else {
      nx = 1;
      imax = ((ma - loop_ub) + mpad) - 1;
    }

    ic = nx - 1;
    ia = (nx + loop_ub) - mpad;
    i1 = imax - nx;
    for (nx = 0; nx <= i1; nx++) {
      yConv->data[ic].re += (float)delta1 * b_y->data[ia].re;
      yConv->data[ic].im += (float)delta1 * b_y->data[ia].im;
      ic++;
      ia++;
    }
  }

  emxFree_real_T(&h);
  emxInit_real_T(&arg, 1);

  /*  Multiply the received GMSK signal by cos(pi*t/2T) and sin(pi*t/2T), where */
  /*  T is the symbol time. The phase offsets are different for the cos and */
  /*  sin multipliers to account for the circular shift in the I channel. */
  /*  These cos() and sin() multipliers are matched to the I and Q pulse */
  /*  shapes, respectively. Then perform receiver filtering. Assume that the */
  /*  received signal already is properly synchronized. */
  delta1 = 2.0 * nSamp;
  i0 = arg->size[0];
  loop_ub = (int)floor((375000.0 + nSamp) - 1.0);
  arg->size[0] = loop_ub + 1;
  emxEnsureCapacity_real_T(arg, i0);
  for (i0 = 0; i0 <= loop_ub; i0++) {
    arg->data[i0] = 3.1415926535897931 * (double)i0 / delta1;
  }

  emxInit_real_T(&x, 1);
  i0 = x->size[0];
  x->size[0] = arg->size[0];
  emxEnsureCapacity_real_T(x, i0);
  loop_ub = arg->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    x->data[i0] = arg->data[i0] + 1.5707963267948966;
  }

  nx = x->size[0];
  for (imax = 0; imax < nx; imax++) {
    x->data[imax] = cos(x->data[imax]);
  }

  i0 = arg->size[0];
  emxEnsureCapacity_real_T(arg, i0);
  loop_ub = arg->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    arg->data[i0] -= 1.5707963267948966;
  }

  nx = arg->size[0];
  for (imax = 0; imax < nx; imax++) {
    arg->data[imax] = sin(arg->data[imax]);
  }

  i0 = yConv->size[0];
  yConv->size[0] = x->size[0];
  emxEnsureCapacity_creal32_T(yConv, i0);
  loop_ub = x->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    im = yConv->data[i0].re;
    re = yConv->data[i0].im;
    yConv->data[i0].re = (float)x->data[i0] * im;
    yConv->data[i0].im = (float)arg->data[i0] * re;
  }

  emxFree_real_T(&x);

  /*  Perform ideal integration */
  delta1 = (double)yConv->size[0] - 2.0 * nSamp;
  if (1.0 > delta1) {
    loop_ub = 0;
  } else {
    loop_ub = (int)delta1;
  }

  emxInit_creal32_T(&filtSig, 1);
  i0 = filtSig->size[0];
  filtSig->size[0] = yConv->size[0];
  emxEnsureCapacity_creal32_T(filtSig, i0);
  nx = yConv->size[0];
  for (i0 = 0; i0 < nx; i0++) {
    filtSig->data[i0] = yConv->data[i0];
  }

  i0 = yConv->size[0];
  for (imax = 0; imax <= i0 - 2; imax++) {
    filtSig->data[imax + 1].re += filtSig->data[imax].re;
    filtSig->data[imax + 1].im += filtSig->data[imax].im;
  }

  nx = (int)(2.0 * nSamp);
  i0 = b_y->size[0];
  b_y->size[0] = nx + loop_ub;
  emxEnsureCapacity_creal32_T(b_y, i0);
  for (i0 = 0; i0 < nx; i0++) {
    b_y->data[i0].re = 0.0F;
    b_y->data[i0].im = 0.0F;
  }

  for (i0 = 0; i0 < loop_ub; i0++) {
    b_y->data[i0 + nx] = yConv->data[i0];
  }

  emxFree_creal32_T(&yConv);
  if ((b_y->size[0] != 0) && (b_y->size[0] != 1)) {
    i0 = b_y->size[0];
    for (imax = 0; imax <= i0 - 2; imax++) {
      b_y->data[imax + 1].re += b_y->data[imax].re;
      b_y->data[imax + 1].im += b_y->data[imax].im;
    }
  }

  i0 = filtSig->size[0];
  emxEnsureCapacity_creal32_T(filtSig, i0);
  loop_ub = filtSig->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    re = filtSig->data[i0].re - b_y->data[i0].re;
    im = filtSig->data[i0].im - b_y->data[i0].im;
    if (im == 0.0F) {
      re /= (float)nSamp;
      im = 0.0F;
    } else if (re == 0.0F) {
      re = 0.0F;
      im /= (float)nSamp;
    } else {
      re /= (float)nSamp;
      im /= (float)nSamp;
    }

    filtSig->data[i0].re = re;
    filtSig->data[i0].im = im;
  }

  emxFree_creal32_T(&b_y);

  /*  Make I start from 2*nSamp and Q start from 3*nSamp. Account for the fact */
  /*  that the signal vector has been augmented in length. */
  Istart = 2.0 * nSamp;
  Qstart = 3.0 * nSamp;
  delta1 = 2.0 * rt_roundd_snf(nBits / 2.0);
  if (delta1 == nBits) {
    Iend = 375000.0;
    Qend = 375000.0 + nSamp;
  } else {
    Iend = 375000.0 + nSamp;
    Qend = 375000.0;
  }

  /*  Soft GMSK demodulation */
  d0 = 2.0 * nSamp;
  if ((d0 == 0.0) || ((d0 > 0.0) && (Istart > Iend))) {
    i0 = 0;
    i1 = 1;
    nx = -1;
  } else {
    i0 = (int)Istart - 1;
    i1 = (int)d0;
    nx = (int)Iend - 1;
  }

  emxInit_real32_T(&A, 1);
  imax = A->size[0];
  loop_ub = div_s32_floor(nx - i0, i1);
  A->size[0] = loop_ub + 1;
  emxEnsureCapacity_real32_T(A, imax);
  for (nx = 0; nx <= loop_ub; nx++) {
    A->data[nx] = filtSig->data[i0 + i1 * nx].re;
  }

  d0 = 2.0 * nSamp;
  if ((d0 == 0.0) || ((d0 > 0.0) && (Qstart > Qend))) {
    i0 = 0;
    i1 = 1;
    nx = -1;
  } else {
    i0 = (int)Qstart - 1;
    i1 = (int)d0;
    nx = (int)Qend - 1;
  }

  emxInit_real32_T(&b_A, 1);
  imax = b_A->size[0];
  loop_ub = div_s32_floor(nx - i0, i1);
  b_A->size[0] = loop_ub + 1;
  emxEnsureCapacity_real32_T(b_A, imax);
  for (nx = 0; nx <= loop_ub; nx++) {
    b_A->data[nx] = filtSig->data[i0 + i1 * nx].im;
  }

  /*  Upsample (i.e. repeat) the I and Q channels by 2, and shift the Q channel */
  /*  by 1 sample relative to the I channel. This operation allows for 2-bit */
  /*  decisions to be made over each I or Q symbol. Also, set the threshold */
  /*  for a suboptimal decision on the last bit. */
  ma = A->size[0];
  nx = A->size[0] << 1;
  i0 = z->size[0];
  z->size[0] = nx;
  emxEnsureCapacity_real32_T(z, i0);
  nx = -1;
  for (imax = 0; imax < ma; imax++) {
    nx++;
    z->data[nx] = A->data[imax];
    nx++;
    z->data[nx] = A->data[imax];
  }

  emxInit_real32_T(&zImagDec, 1);
  ma = b_A->size[0];
  nx = b_A->size[0] << 1;
  i0 = zImagDec->size[0];
  zImagDec->size[0] = nx;
  emxEnsureCapacity_real32_T(zImagDec, i0);
  nx = -1;
  for (imax = 0; imax < ma; imax++) {
    nx++;
    zImagDec->data[nx] = b_A->data[imax];
    nx++;
    zImagDec->data[nx] = b_A->data[imax];
  }

  if (delta1 == nBits) {
    delta1 = ((double)filtSig->size[0] - nSamp) + 1.0;
    if (delta1 > filtSig->size[0]) {
      i0 = 1;
      i1 = 0;
    } else {
      i0 = (int)delta1;
      i1 = filtSig->size[0];
    }

    if (2 > z->size[0]) {
      nx = 0;
      imax = 0;
    } else {
      nx = 1;
      imax = z->size[0];
    }

    mpad = A->size[0];
    loop_ub = i1 - i0;
    A->size[0] = loop_ub + 1;
    emxEnsureCapacity_real32_T(A, mpad);
    for (i1 = 0; i1 <= loop_ub; i1++) {
      A->data[i1] = filtSig->data[(i0 + i1) - 1].re;
    }

    b_sign(A);
    i0 = arg->size[0];
    arg->size[0] = A->size[0];
    emxEnsureCapacity_real_T(arg, i0);
    loop_ub = A->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      arg->data[i0] = A->data[i0];
    }

    im = filtSig->data[filtSig->size[0] - 1].re - (float)(0.5 * sum(arg) / nSamp);
    if (im < 0.0F) {
      im = -1.0F;
    } else if (im > 0.0F) {
      im = 1.0F;
    } else {
      if (im == 0.0F) {
        im = 0.0F;
      }
    }

    i0 = A->size[0];
    loop_ub = imax - nx;
    A->size[0] = loop_ub + 1;
    emxEnsureCapacity_real32_T(A, i0);
    for (i0 = 0; i0 < loop_ub; i0++) {
      A->data[i0] = z->data[nx + i0];
    }

    A->data[loop_ub] = im;
    i0 = z->size[0];
    z->size[0] = A->size[0];
    emxEnsureCapacity_real32_T(z, i0);
    loop_ub = A->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      z->data[i0] = A->data[i0];
    }
  } else {
    if (2 > z->size[0]) {
      i0 = 0;
      i1 = 0;
    } else {
      i0 = 1;
      i1 = z->size[0];
    }

    nx = b_A->size[0];
    loop_ub = i1 - i0;
    b_A->size[0] = loop_ub;
    emxEnsureCapacity_real32_T(b_A, nx);
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_A->data[i1] = z->data[i0 + i1];
    }

    i0 = z->size[0];
    z->size[0] = b_A->size[0];
    emxEnsureCapacity_real32_T(z, i0);
    loop_ub = b_A->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      z->data[i0] = b_A->data[i0];
    }

    delta1 = ((double)filtSig->size[0] - nSamp) + 1.0;
    if (delta1 > filtSig->size[0]) {
      i0 = 1;
      i1 = 0;
    } else {
      i0 = (int)delta1;
      i1 = filtSig->size[0];
    }

    nx = A->size[0];
    loop_ub = i1 - i0;
    A->size[0] = loop_ub + 1;
    emxEnsureCapacity_real32_T(A, nx);
    for (i1 = 0; i1 <= loop_ub; i1++) {
      A->data[i1] = filtSig->data[(i0 + i1) - 1].im;
    }

    b_sign(A);
    i0 = arg->size[0];
    arg->size[0] = A->size[0];
    emxEnsureCapacity_real_T(arg, i0);
    loop_ub = A->size[0];
    for (i0 = 0; i0 < loop_ub; i0++) {
      arg->data[i0] = A->data[i0];
    }

    im = filtSig->data[filtSig->size[0] - 1].im - (float)(0.5 * sum(arg) / nSamp);
    if (im < 0.0F) {
      im = -1.0F;
    } else if (im > 0.0F) {
      im = 1.0F;
    } else {
      if (im == 0.0F) {
        im = 0.0F;
      }
    }

    i0 = zImagDec->size[0];
    i1 = zImagDec->size[0];
    zImagDec->size[0] = i0 + 1;
    emxEnsureCapacity_real32_T(zImagDec, i1);
    zImagDec->data[i0] = im;
  }

  emxFree_real32_T(&b_A);
  emxFree_real32_T(&A);
  emxFree_creal32_T(&filtSig);
  emxFree_real_T(&arg);

  /*  Now multiply the real and imaginary parts together to find the signed */
  /*  data values */
  i0 = z->size[0];
  emxEnsureCapacity_real32_T(z, i0);
  loop_ub = z->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    z->data[i0] = -z->data[i0] * zImagDec->data[i0];
  }

  emxFree_real32_T(&zImagDec);
}

/*
 * File trailer for gmskdemod.c
 *
 * [EOF]
 */
