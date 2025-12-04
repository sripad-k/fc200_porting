/*
 * File: altCtrl.h
 *
 * Code generated for Simulink model 'altCtrl'.
 *
 * Model version                  : 2.16
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:06:12 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef altCtrl_h_
#define altCtrl_h_
#ifndef altCtrl_COMMON_INCLUDES_
#define altCtrl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* altCtrl_COMMON_INCLUDES_ */

#include "altCtrl_types.h"
#include <string.h>

extern void altCtrl_Init(void);
extern void altCtrl(const real_T *rtu_ctrlIF_vel_hRateCmd, const real_T
                    *rtu_ctrlIF_vel_hCmd, const boolean_T *rtu_ctrlIF_vel_hHold,
                    const boolean_T *rtu_ctrlIF_vel_init_reset_hdot, const
                    std_sensor_t *rtu_Sensor, const real_T
                    *rtu_busControllerCA_c_erp2, const real_T
                    rtu_busControllerCA_nu_des[4], const real_T
                    rtu_busControllerCA_nu_allocated[4], busControllerAltCtrl
                    *rty_controllerAltCtrl);

/* Model reference registration function */
extern void altCtrl_initialize(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S8>/Data Type Duplicate' : Unused code path elimination
 * Block '<S8>/Data Type Propagation' : Unused code path elimination
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
 * '<Root>' : 'altCtrl'
 * '<S1>'   : 'altCtrl/EC1'
 * '<S2>'   : 'altCtrl/height_ctrl'
 * '<S3>'   : 'altCtrl/kinematicInversion'
 * '<S4>'   : 'altCtrl/EC1/AW_clamping'
 * '<S5>'   : 'altCtrl/EC1/derivativeFilter'
 * '<S6>'   : 'altCtrl/EC1/AW_clamping/Compare To Constant'
 * '<S7>'   : 'altCtrl/EC1/AW_clamping/Compare To Zero'
 * '<S8>'   : 'altCtrl/height_ctrl/Saturation Dynamic3'
 */

/*-
 * Requirements for '<Root>': altCtrl

 */
#endif                                 /* altCtrl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
