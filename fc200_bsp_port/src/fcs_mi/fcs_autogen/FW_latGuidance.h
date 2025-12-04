/*
 * File: FW_latGuidance.h
 *
 * Code generated for Simulink model 'FW_latGuidance'.
 *
 * Model version                  : 2.81
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:05:32 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_latGuidance_h_
#define FW_latGuidance_h_
#ifndef FW_latGuidance_COMMON_INCLUDES_
#define FW_latGuidance_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_latGuidance_COMMON_INCLUDES_ */

#include "FW_latGuidance_types.h"

extern void FW_latGuidance(const real_T *rtu_FW_TECS_switcher_roll_cmd, const
  std_sensor_t *rtu_Sensor, real_T *rty_rollCmd, real_T *rty_rollCmd_FW_guidance);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S3>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<Root>/Saturation2' : Unused code path elimination
 * Block '<S2>/Reshape' : Reshape block reduction
 * Block '<Root>/Constant' : Unused code path elimination
 * Block '<S1>/Constant' : Unused code path elimination
 * Block '<S1>/Constant1' : Unused code path elimination
 * Block '<S1>/Constant2' : Unused code path elimination
 * Block '<S1>/Delay' : Unused code path elimination
 * Block '<S1>/Gain' : Unused code path elimination
 * Block '<S1>/Gain2' : Unused code path elimination
 * Block '<S3>/AND' : Unused code path elimination
 * Block '<S3>/Lower Limit' : Unused code path elimination
 * Block '<S3>/Lower Test' : Unused code path elimination
 * Block '<S3>/Upper Limit' : Unused code path elimination
 * Block '<S3>/Upper Test' : Unused code path elimination
 * Block '<S1>/Kp_chi' : Unused code path elimination
 * Block '<S1>/Logical Operator' : Unused code path elimination
 * Block '<S1>/Logical Operator1' : Unused code path elimination
 * Block '<S1>/Product' : Unused code path elimination
 * Block '<S1>/Product1' : Unused code path elimination
 * Block '<S1>/Product2' : Unused code path elimination
 * Block '<S1>/Saturation' : Unused code path elimination
 * Block '<S1>/Sum' : Unused code path elimination
 * Block '<S1>/Sum2' : Unused code path elimination
 * Block '<S1>/Switch' : Unused code path elimination
 * Block '<S1>/Switch1' : Unused code path elimination
 * Block '<S1>/Trigonometric Function' : Unused code path elimination
 * Block '<S1>/Trigonometric Function1' : Unused code path elimination
 * Block '<S4>/Add3' : Unused code path elimination
 * Block '<S4>/Add4' : Unused code path elimination
 * Block '<S4>/Constant2' : Unused code path elimination
 * Block '<S4>/Constant3' : Unused code path elimination
 * Block '<S4>/Mod2' : Unused code path elimination
 * Block '<S4>/Mod3' : Unused code path elimination
 * Block '<S4>/Sum' : Unused code path elimination
 * Block '<S4>/Sum1' : Unused code path elimination
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
 * '<Root>' : 'FW_latGuidance'
 * '<S1>'   : 'FW_latGuidance/FW_CoG_tracking'
 * '<S2>'   : 'FW_latGuidance/L1_guidance'
 * '<S3>'   : 'FW_latGuidance/FW_CoG_tracking/Interval Test'
 * '<S4>'   : 'FW_latGuidance/FW_CoG_tracking/wrapToPi'
 * '<S5>'   : 'FW_latGuidance/L1_guidance/MATLAB Function'
 * '<S6>'   : 'FW_latGuidance/L1_guidance/dist_from_LLAs1'
 * '<S7>'   : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/ECEF2NED'
 * '<S8>'   : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF'
 * '<S9>'   : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF1'
 * '<S10>'  : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/ECEF2NED/MATLAB Function1'
 * '<S11>'  : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2'
 * '<S12>'  : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF/Subsystem2/zero_division_protection'
 * '<S13>'  : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2'
 * '<S14>'  : 'FW_latGuidance/L1_guidance/dist_from_LLAs1/LLA2ECEF1/Subsystem2/zero_division_protection'
 */

/*-
 * Requirements for '<Root>': FW_latGuidance

 */
#endif                                 /* FW_latGuidance_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
