/*
 * File: FW_throttle_private.h
 *
 * Code generated for Simulink model 'FW_throttle'.
 *
 * Model version                  : 2.3
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:37 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_throttle_private_h_
#define FW_throttle_private_h_
#include "rtwtypes.h"
#include "FW_throttle_types.h"

/* Block states (default storage) for model 'FW_throttle' */
typedef struct {
  real_T PrevY;                        /* '<Root>/100%_per_sec' */
} FW_throttle_TDW;

/* Block states (default storage) */
extern FW_throttle_TDW FW_throttle_DW;

#endif                                 /* FW_throttle_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
