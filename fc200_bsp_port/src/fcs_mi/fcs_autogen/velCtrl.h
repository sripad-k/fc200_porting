/*
 * File: velCtrl.h
 *
 * Code generated for Simulink model 'velCtrl'.
 *
 * Model version                  : 2.9
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:20 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef velCtrl_h_
#define velCtrl_h_
#ifndef velCtrl_COMMON_INCLUDES_
#define velCtrl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* velCtrl_COMMON_INCLUDES_ */

#include "velCtrl_types.h"
#include <string.h>

extern void velCtrl_Init(void);
extern void velCtrl(const std_sensor_t *rtu_Sensor, const real_T
                    *rtu_ctrlIF_vel_velCmdH_x, const real_T
                    *rtu_ctrlIF_vel_velCmdH_y, const boolean_T
                    *rtu_ctrlIF_vel_init_reset_vel, busControllerIF_att
                    *rty_attAltCmd, busControllerVelCtrl *rty_controllerVelCtrl);

/* Model reference registration function */
extern void velCtrl_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S10>/Reshape' : Reshape block reduction
 * Block '<S17>/Reshape' : Reshape block reduction
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
 * '<Root>' : 'velCtrl'
 * '<S1>'   : 'velCtrl/EC'
 * '<S2>'   : 'velCtrl/kinematicInversion'
 * '<S3>'   : 'velCtrl/rot_2D'
 * '<S4>'   : 'velCtrl/signedEllipticalLimit'
 * '<S5>'   : 'velCtrl/EC/AW_clamping'
 * '<S6>'   : 'velCtrl/EC/AW_clamping/Compare To Constant'
 * '<S7>'   : 'velCtrl/EC/AW_clamping/Compare To Zero'
 * '<S8>'   : 'velCtrl/kinematicInversion/axay_to_phitheta'
 * '<S9>'   : 'velCtrl/kinematicInversion/castToDouble'
 * '<S10>'  : 'velCtrl/kinematicInversion/ellipticalLimit'
 * '<S11>'  : 'velCtrl/kinematicInversion/ellipticalLimit/Compare To Constant'
 * '<S12>'  : 'velCtrl/kinematicInversion/ellipticalLimit/zero_division_protection'
 * '<S13>'  : 'velCtrl/kinematicInversion/ellipticalLimit/zero_division_protection1'
 * '<S14>'  : 'velCtrl/rot_2D/MATLAB Function'
 * '<S15>'  : 'velCtrl/signedEllipticalLimit/Compare To Constant'
 * '<S16>'  : 'velCtrl/signedEllipticalLimit/Compare To Constant1'
 * '<S17>'  : 'velCtrl/signedEllipticalLimit/ellipticalLimit'
 * '<S18>'  : 'velCtrl/signedEllipticalLimit/ellipticalLimit/Compare To Constant'
 * '<S19>'  : 'velCtrl/signedEllipticalLimit/ellipticalLimit/zero_division_protection'
 * '<S20>'  : 'velCtrl/signedEllipticalLimit/ellipticalLimit/zero_division_protection1'
 */

/*-
 * Requirements for '<Root>': velCtrl

 */
#endif                                 /* velCtrl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
