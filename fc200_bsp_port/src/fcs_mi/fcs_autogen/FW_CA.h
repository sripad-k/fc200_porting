/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FW_CA.h
 *
 * Code generated for Simulink model 'FW_CA'.
 *
 * Model version                  : 2.106
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:50:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef FW_CA_h_
#define FW_CA_h_
#ifndef FW_CA_COMMON_INCLUDES_
#define FW_CA_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FW_CA_COMMON_INCLUDES_ */

#include "FW_CA_types.h"
#include <string.h>

extern void FW_CA_Init(void);
extern void FW_CA(const vom_t *rtu_vom_status, const real_T *rtu_elevator, const
                  real_T *rtu_aileron, const real_T *rtu_rudder, const real_T
                  *rtu_engineCmd, const std_sensor_t *rtu_Sensor, const real_T
                  *rtu_pilot_roll_ch, const real_T *rtu_pilot_pitch_ch, const
                  real_T *rtu_pilot_yaw_ch, const real_T *rtu_pilot_engine_ch,
                  busControllerCA *rty_controllerCA);

/* Model reference registration function */
extern void FW_CA_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Reshape1' : Reshape block reduction
 * Block '<Root>/Reshape3' : Reshape block reduction
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
 * '<Root>' : 'FW_CA'
 * '<S1>'   : 'FW_CA/ACS_and_Engine_Numbering_Variant'
 * '<S2>'   : 'FW_CA/CA_aileron'
 * '<S3>'   : 'FW_CA/CA_elevator'
 * '<S4>'   : 'FW_CA/CA_engine'
 * '<S5>'   : 'FW_CA/CA_rudder'
 * '<S6>'   : 'FW_CA/Filter_forEach'
 * '<S7>'   : 'FW_CA/PilotServoTestEnableFlag'
 * '<S8>'   : 'FW_CA/Subsystem'
 * '<S9>'   : 'FW_CA/outputFilter'
 * '<S10>'  : 'FW_CA/sPilotCmd'
 * '<S11>'  : 'FW_CA/ACS_and_Engine_Numbering_Variant/SS'
 * '<S12>'  : 'FW_CA/CA_aileron/MATLAB Function'
 * '<S13>'  : 'FW_CA/CA_elevator/MATLAB Function'
 * '<S14>'  : 'FW_CA/CA_rudder/MATLAB Function'
 * '<S15>'  : 'FW_CA/Filter_forEach/outputFilter'
 * '<S16>'  : 'FW_CA/Filter_forEach/outputFilter/noFilter'
 * '<S17>'  : 'FW_CA/PilotServoTestEnableFlag/Compare To Constant'
 * '<S18>'  : 'FW_CA/PilotServoTestEnableFlag/Compare To Constant1'
 * '<S19>'  : 'FW_CA/Subsystem/Compare To Constant'
 * '<S20>'  : 'FW_CA/Subsystem/Compare To Constant1'
 * '<S21>'  : 'FW_CA/Subsystem/Compare To Constant2'
 * '<S22>'  : 'FW_CA/Subsystem/Compare To Constant3'
 * '<S23>'  : 'FW_CA/Subsystem/Compare To Constant4'
 * '<S24>'  : 'FW_CA/Subsystem/Subsystem'
 * '<S25>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant1'
 * '<S26>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant3'
 * '<S27>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant4'
 * '<S28>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant5'
 * '<S29>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant6'
 * '<S30>'  : 'FW_CA/Subsystem/Subsystem/Compare To Constant7'
 * '<S31>'  : 'FW_CA/outputFilter/noFilter'
 * '<S32>'  : 'FW_CA/sPilotCmd/LPF1'
 * '<S33>'  : 'FW_CA/sPilotCmd/LPF2'
 * '<S34>'  : 'FW_CA/sPilotCmd/LPF3'
 */

/*-
 * Requirements for '<Root>': FW_CA

 */
#endif                                 /* FW_CA_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
