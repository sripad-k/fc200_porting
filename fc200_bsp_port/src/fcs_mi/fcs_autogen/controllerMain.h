/*
 * File: controllerMain.h
 *
 * Code generated for Simulink model 'controllerMain'.
 *
 * Model version                  : 3.618
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:43 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef controllerMain_h_
#define controllerMain_h_
#ifndef controllerMain_COMMON_INCLUDES_
#define controllerMain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* controllerMain_COMMON_INCLUDES_ */

#include "controllerMain_types.h"
#include <string.h>
#include "zero_crossing_types.h"

/* Block signals (default storage) */
typedef struct {
  busMode_data mode_data;              /* '<Root>/system_state_machine' */
  std_sensor_t BusAssignment;          /* '<S4>/Bus Assignment' */
  pilot_ext_t BusAssignment_b4zvdvf4m4;/* '<S2>/Bus Assignment' */
  busControllerAltCtrl UnitDelay;      /* '<Root>/Unit Delay' */
  real_T dCapturedAlt_m;               /* '<Root>/system_state_machine' */
  vom_t vom_status;                    /* '<Root>/system_state_machine' */
  safety_state_t safety_state;         /* '<Root>/system_state_machine' */
  pic_t pic_status;                    /* '<Root>/system_state_machine' */
  lifter_state_t lifter_state;         /* '<Root>/system_state_machine' */
  uint8_T loiter_mode;                 /* '<Root>/system_state_machine' */
  uint8_T TECS_mode;                   /* '<Root>/system_state_machine' */
  uint8_T CoG_tracking;                /* '<Root>/system_state_machine' */
  boolean_T bInAirFlag;                /* '<Root>/system_state_machine' */
  boolean_T bGPSLoss10sec;             /* '<Root>/system_state_machine' */
  boolean_T bGPSLoss;                  /* '<Root>/system_state_machine' */
  boolean_T rampup_phase;              /* '<Root>/system_state_machine' */
  boolean_T bTelemtryLinkLoss1sec;     /* '<Root>/system_state_machine' */
  boolean_T ep_loss1sec;               /* '<Root>/system_state_machine' */
} controllerMain_n4wy4x2kbb_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  busControllerCA Delay_DSTATE;        /* '<Root>/Delay' */
  busControllerCA Delay1_DSTATE;       /* '<Root>/Delay1' */
  busControllerCA Delay3_DSTATE;       /* '<Root>/Delay3' */
  busControllerAltCtrl UnitDelay_DSTATE;/* '<Root>/Unit Delay' */
  real_T notch0_gyro_states[6];        /* '<S27>/notch0_gyro' */
  real_T notch0_acc_states[6];         /* '<S27>/notch0_acc' */
  real_T notch0_gyro1_states[6];       /* '<S27>/notch0_gyro1' */
  real_T notch0_acc1_states[6];        /* '<S27>/notch0_acc1' */
  real_T ud_1_DSTATE;                  /* '<S16>/ud' */
  real_T ud_2_DSTATE;                  /* '<S16>/ud' */
  real_T ud_3_DSTATE;                  /* '<S16>/ud' */
  real_T ud_4_DSTATE;                  /* '<S16>/ud' */
  real_T ud_5_DSTATE;                  /* '<S16>/ud' */
  real_T FixPtUnitDelay1_DSTATE;       /* '<S20>/FixPt Unit Delay1' */
  real_T ud1_1_DSTATE;                 /* '<S16>/ud1' */
  real_T ud1_2_DSTATE;                 /* '<S16>/ud1' */
  real_T ud1_3_DSTATE;                 /* '<S16>/ud1' */
  real_T ud1_4_DSTATE;                 /* '<S16>/ud1' */
  real_T ud1_5_DSTATE;                 /* '<S16>/ud1' */
  real_T UnitDelay3_5_DSTATE;          /* '<Root>/Unit Delay3' */
  real_T UnitDelay3_6_DSTATE;          /* '<Root>/Unit Delay3' */
  real_T UnitDelay3_7_DSTATE;          /* '<Root>/Unit Delay3' */
  real_T UnitDelay4_1_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T UnitDelay4_2_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T UnitDelay4_3_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T UnitDelay4_6_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T UnitDelay4_7_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T UnitDelay4_8_DSTATE;          /* '<Root>/Unit Delay4' */
  real_T Delay2_DSTATE;                /* '<Root>/Delay2' */
  real_T omega_DSTATE;                 /* '<S26>/omega' */
  real_T omega_DSTATE_l5hd10dn0e;      /* '<S25>/omega' */
  real_T Delay7_3_DSTATE;              /* '<Root>/Delay7' */
  real_T Delay4_1_DSTATE;              /* '<Root>/Delay4' */
  real_T Delay7_1_DSTATE;              /* '<Root>/Delay7' */
  real_T Delay7_4_DSTATE;              /* '<Root>/Delay7' */
  real_T Delay4_2_DSTATE;              /* '<Root>/Delay4' */
  real_T Delay7_2_DSTATE;              /* '<Root>/Delay7' */
  real_T Delay4_3_DSTATE;              /* '<Root>/Delay4' */
  real_T Delay4_4_DSTATE;              /* '<Root>/Delay4' */
  real_T Memory_PreviousInput;         /* '<S24>/Memory' */
  real_T PrevY;                        /* '<S4>/Rate Limiter' */
  real_T PrevY_fbnop0txyt;             /* '<S24>/Rate Limiter' */
  pic_t UnitDelay2_DSTATE;             /* '<Root>/Unit Delay2' */
  vom_t UnitDelay1_DSTATE;             /* '<Root>/Unit Delay1' */
  uint32_T m_bpIndex;                  /* '<S1>/1-D Lookup Table' */
  uint8_T FixPtUnitDelay2_DSTATE;      /* '<S20>/FixPt Unit Delay2' */
  boolean_T UnitDelay3_1_DSTATE;       /* '<Root>/Unit Delay3' */
  boolean_T UnitDelay3_2_DSTATE;       /* '<Root>/Unit Delay3' */
  boolean_T UnitDelay3_3_DSTATE;       /* '<Root>/Unit Delay3' */
  boolean_T UnitDelay3_4_DSTATE;       /* '<Root>/Unit Delay3' */
  boolean_T UnitDelay4_4_DSTATE;       /* '<Root>/Unit Delay4' */
  boolean_T UnitDelay4_5_DSTATE;       /* '<Root>/Unit Delay4' */
  boolean_T UnitDelay4_9_DSTATE;       /* '<Root>/Unit Delay4' */
  uint8_T omega_IC_LOADING;            /* '<S26>/omega' */
  uint8_T omega_IC_LOADING_bb503dutix; /* '<S25>/omega' */
} controllerMain_l5cfqqw4qz_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T Gain;                   /* '<S16>/Gain' */
  const real_T Sum1;                   /* '<S24>/Sum1' */
  const real_T Abs;                    /* '<S29>/Abs' */
  const real_T Switch1;                /* '<S29>/Switch1' */
  const real_T Product;                /* '<S29>/Product' */
  const real_T Switch3;                /* '<S29>/Switch3' */
} controllerMain_fauf41phob_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: transition_lambda_data
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  real_T uDLookupTable_tableData[2];

  /* Expression: transition_lambda_bp
   * Referenced by: '<S1>/1-D Lookup Table'
   */
  real_T uDLookupTable_bp01Data[2];
} controllerMain_dfsmisgjtw_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  pilot_ext_t pilot_ext;               /* '<Root>/pilot_ext' */
  pilot_int_t pilot_int;               /* '<Root>/pilot_int' */
  sensor_t sensor_in;                  /* '<Root>/sensor_in' */
  std_command_t std_command;           /* '<Root>/std_command' */
  failure_flag_t failure_flags;        /* '<Root>/failure_flags' */
  ext_command_t extd_cmd;              /* '<Root>/extd_cmd' */
  wp_data_t wp_data;                   /* '<Root>/wp_data' */
} controllerMain_csevfv4opo_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  std_ctrl_t std_ctrl;                 /* '<Root>/std_ctrl' */
  busController ctrl_log;              /* '<Root>/ctrl_log' */
  busStateMachine fcs_state;           /* '<Root>/fcs_state' */
  uint16_T wp_req_idx;                 /* '<Root>/wp_req_idx' */
  gcs_fb_t gcs_fb;                     /* '<Root>/gcs_fb' */
} controllerMain_jzldyrnxon_T;

/* Block signals (default storage) */
extern controllerMain_n4wy4x2kbb_T controllerMain_B;

/* Block states (default storage) */
extern controllerMain_l5cfqqw4qz_T controllerMain_DW;

/* External inputs (root inport signals with default storage) */
extern controllerMain_csevfv4opo_T controllerMain_U;

/* External outputs (root outports fed by signals with default storage) */
extern controllerMain_jzldyrnxon_T controllerMain_Y;

/* External data declarations for dependent source files */
extern const busMode_data controllerMain_rtZbusMode_data;/* busMode_data ground */
extern const controllerMain_fauf41phob_T controllerMain_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const controllerMain_dfsmisgjtw_T controllerMain_ConstP;

/* Model entry point functions */
extern void controllerMain_initialize(void);
extern void controllerMain_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S19>/Data Type Duplicate' : Unused code path elimination
 * Block '<S19>/Data Type Propagation' : Unused code path elimination
 * Block '<S20>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<Root>/Scope' : Unused code path elimination
 * Block '<S28>/Reshape' : Reshape block reduction
 * Block '<S28>/Reshape1' : Reshape block reduction
 * Block '<S16>/Constant3' : Unused code path elimination
 * Block '<S16>/Constant4' : Unused code path elimination
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
 * '<Root>' : 'controllerMain'
 * '<S1>'   : 'controllerMain/Integrator_crossfeed'
 * '<S2>'   : 'controllerMain/PilotCmd'
 * '<S3>'   : 'controllerMain/Safety'
 * '<S4>'   : 'controllerMain/SensorManagement'
 * '<S5>'   : 'controllerMain/gcs_fb_sel'
 * '<S6>'   : 'controllerMain/Integrator_crossfeed/saturation_handler'
 * '<S7>'   : 'controllerMain/Integrator_crossfeed/saturation_handler1'
 * '<S8>'   : 'controllerMain/Integrator_crossfeed/saturation_handler/Compare To Constant1'
 * '<S9>'   : 'controllerMain/Integrator_crossfeed/saturation_handler/Compare To Constant2'
 * '<S10>'  : 'controllerMain/Integrator_crossfeed/saturation_handler/Compare To Constant3'
 * '<S11>'  : 'controllerMain/Integrator_crossfeed/saturation_handler1/Compare To Constant1'
 * '<S12>'  : 'controllerMain/Integrator_crossfeed/saturation_handler1/Compare To Constant2'
 * '<S13>'  : 'controllerMain/Integrator_crossfeed/saturation_handler1/Compare To Constant3'
 * '<S14>'  : 'controllerMain/Integrator_crossfeed/saturation_handler1/Compare To Constant4'
 * '<S15>'  : 'controllerMain/PilotCmd/Compare To Constant'
 * '<S16>'  : 'controllerMain/PilotCmd/Smooth_Transition'
 * '<S17>'  : 'controllerMain/PilotCmd/Smooth_Transition/Rate_Limiter_Dynamic1'
 * '<S18>'  : 'controllerMain/PilotCmd/Smooth_Transition/Rate_Limiter_Dynamic1/Compare To Constant'
 * '<S19>'  : 'controllerMain/PilotCmd/Smooth_Transition/Rate_Limiter_Dynamic1/Saturation Dynamic'
 * '<S20>'  : 'controllerMain/PilotCmd/Smooth_Transition/Rate_Limiter_Dynamic1/Unit Delay External IC'
 * '<S21>'  : 'controllerMain/Safety/SAFETY'
 * '<S22>'  : 'controllerMain/Safety/SAFETY/Compare To Constant2'
 * '<S23>'  : 'controllerMain/SensorManagement/SpikeEnable'
 * '<S24>'  : 'controllerMain/SensorManagement/airspeedFilter'
 * '<S25>'  : 'controllerMain/SensorManagement/derivativeFilter'
 * '<S26>'  : 'controllerMain/SensorManagement/filter'
 * '<S27>'  : 'controllerMain/SensorManagement/sensor_filtering'
 * '<S28>'  : 'controllerMain/SensorManagement/static_voting_logic'
 * '<S29>'  : 'controllerMain/SensorManagement/zero_division_protection1'
 * '<S30>'  : 'controllerMain/SensorManagement/SpikeEnable/Compare To Constant'
 * '<S31>'  : 'controllerMain/SensorManagement/SpikeEnable/Compare To Constant1'
 * '<S32>'  : 'controllerMain/SensorManagement/SpikeEnable/Compare To Constant2'
 */

/*-
 * Requirements for '<Root>': controllerMain

 */
#endif                                 /* controllerMain_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
