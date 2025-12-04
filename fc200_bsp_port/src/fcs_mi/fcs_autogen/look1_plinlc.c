/*
 * File: look1_plinlc.c
 *
 * Code generated for Simulink model 'FW_CA'.
 *
 * Model version                  : 2.108
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:04:17 2025
 */

#include "look1_plinlc.h"
#include "rtwtypes.h"

real_T look1_plinlc(real_T u0, const real_T bp0[], const real_T table[],
                    uint32_T prevIndex[], uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T bpIdx;

  /* Column-major Lookup 1-D
     Search method: 'linear'
     Use previous index: 'on'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    bpIdx = 0U;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex]) {
    /* Linear Search */
    for (bpIdx = prevIndex[0U]; u0 < bp0[bpIdx]; bpIdx--) {
    }

    while (u0 >= bp0[bpIdx + 1U]) {
      bpIdx++;
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1U] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex - 1U;
    frac = 1.0;
  }

  prevIndex[0U] = bpIdx;

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'wrapping'
   */
  yL_0d0 = table[bpIdx];
  return ((table[bpIdx + 1U] - yL_0d0) * frac) + yL_0d0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
