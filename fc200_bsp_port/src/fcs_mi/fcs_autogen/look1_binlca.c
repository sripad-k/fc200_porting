/*
 * File: look1_binlca.c
 *
 * Code generated for Simulink model 'att_ctrl_switcher'.
 *
 * Model version                  : 2.209
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:26 2025
 */

#include "look1_binlca.h"
#include "rtwtypes.h"

real_T look1_binlca(real_T u0, const real_T bp0[], const real_T table[],
                    uint32_T maxIndex)
{
  real_T frac;
  real_T y;
  real_T yL_0d0;
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = (maxIndex >> ((uint32_T)1U));
    iLeft = 0U;
    iRght = maxIndex;
    while ((iRght - iLeft) > ((uint32_T)1U)) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = ((iRght + iLeft) >> ((uint32_T)1U));
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex;
    frac = 0.0;
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'wrapping'
   */
  if (iLeft == maxIndex) {
    y = table[iLeft];
  } else {
    yL_0d0 = table[iLeft];
    y = ((table[iLeft + 1U] - yL_0d0) * frac) + yL_0d0;
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
