/*
 * File: FW_TECS.h
 *
 * Code generated for Simulink model 'FW_TECS'.
 *
 * Model version                  : 2.61
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:04:29 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_TECS_h_
#define FW_TECS_h_
#ifndef FW_TECS_COMMON_INCLUDES_
#define FW_TECS_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_TECS_COMMON_INCLUDES_ */

#include "FW_TECS_types.h"
#include <string.h>

extern void FW_TECS_Init(void);
extern void FW_TECS(const real_T *rtu_FW_TECS_switcher_h_dot_cmd, const real_T
                    *rtu_FW_TECS_switcher_h_cmd, const real_T
                    *rtu_FW_TECS_switcher_V_cmd, const boolean_T
                    *rtu_FW_TECS_switcher_TECS_reset, const boolean_T
                    *rtu_FW_TECS_switcher_h_hold, const std_sensor_t *rtu_Sensor,
                    const real_T *rtu_engineCmd, const busTECS_Data_Processing
                    *rtu_sTECS_Data_Processing, busControllerTECS
                    *rty_controllerTECS);

/* Model reference registration function */
extern void FW_TECS_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S6>/Data Type Duplicate' : Unused code path elimination
 * Block '<S6>/Data Type Propagation' : Unused code path elimination
 * Block '<S11>/Data Type Duplicate' : Unused code path elimination
 * Block '<S11>/Data Type Propagation' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'FW_TECS'
 * '<S1>'   : 'FW_TECS/ '
 * '<S2>'   : 'FW_TECS/TECS_inner'
 * '<S3>'   : 'FW_TECS/TECS_outer'
 * '<S4>'   : 'FW_TECS/TECS_inner/AW_theta'
 * '<S5>'   : 'FW_TECS/TECS_inner/AW_thrust'
 * '<S6>'   : 'FW_TECS/TECS_inner/Saturation Dynamic'
 * '<S7>'   : 'FW_TECS/TECS_inner/Subsystem'
 * '<S8>'   : 'FW_TECS/TECS_inner/zero_division_protection1'
 * '<S9>'   : 'FW_TECS/TECS_inner/AW_theta/Compare To Constant'
 * '<S10>'  : 'FW_TECS/TECS_inner/AW_thrust/Compare To Constant'
 * '<S11>'  : 'FW_TECS/TECS_outer/Saturation Dynamic3'
 */

/*-
 * Requirements for '<Root>': FW_TECS

 */
#endif                                 /* FW_TECS_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
