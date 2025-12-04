/*
 * File: rt_modd.c
 *
 * Code generated for Simulink model 'FW_TECS_switcher'.
 *
 * Model version                  : 2.179
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:02 2025
 */

#include "rtwtypes.h"
#include "rt_modd.h"
#include <math.h>
#include <float.h>

real_T rt_modd(real_T u0, real_T u1)
{
  real_T q;
  real_T y;
  boolean_T yEq;
  y = u0;
  if (u1 == 0.0) {
    if (u0 == 0.0) {
      y = u1;
    }
  } else if (u0 == 0.0) {
    y = 0.0 / u1;
  } else {
    y = fmod(u0, u1);
    yEq = (y == 0.0);
    if ((!yEq) && (u1 > floor(u1))) {
      q = fabs(u0 / u1);
      yEq = (fabs(q - floor(q + 0.5)) <= (DBL_EPSILON * q));
    }

    if (yEq) {
      y = 0.0;
    } else if ((u0 < 0.0) != (u1 < 0.0)) {
      y += u1;
    } else {
      /* no actions */
    }
  }

  return y;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
