/*
 * File: system_state_machine_private.h
 *
 * Code generated for Simulink model 'system_state_machine'.
 *
 * Model version                  : 3.635
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 4a0df9c8
 * C/C++ source code generated on : Thu Nov 27 13:07:05 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef system_state_machine_private_h_
#define system_state_machine_private_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "system_state_machine_types.h"

/* Block signals for model 'system_state_machine' */
typedef struct {
  busWP_SMdata_in
    BusConversion_InsertedFor_fcs_state_machine_stateflow_at_inport_;
  busFWRTH_data_in
    BusConversion_InsertedFor_fcs_state_machine_stateflow_eygehnvcfe;
  failure_flag_t
    BusConversion_InsertedFor_fcs_state_machine_stateflow_nqsisctbxb;
  real_T Max;                          /* '<S9>/Max' */
  real_T sLLA[3];                      /* '<S1>/Reshape1' */
  real_T TrigonometricFunction6;       /* '<S10>/Trigonometric Function6' */
  real_T Add4;                         /* '<S15>/Add4' */
  vom_t Switch_d533txexjk;             /* '<S14>/Switch' */
  pic_t Switch_po1f1vqhhu;             /* '<S13>/Switch' */
  boolean_T bInAirFlag;                /* '<S1>/Unit Delay' */
  boolean_T Switch1;                   /* '<S5>/Switch1' */
} system_state_machine_TB;

/* Block states (default storage) for model 'system_state_machine' */
typedef struct {
  real_T UnitDelay1_DSTATE;            /* '<S2>/Unit Delay1' */
  real_T DelayOneStep1_DSTATE;         /* '<S6>/Delay One Step1' */
  real_T UnitDelay2_DSTATE;            /* '<S7>/Unit Delay2' */
  real_T timer_Tele;                   /* '<S1>/fcs_state_machine_stateflow' */
  real_T timer_RC;                     /* '<S1>/fcs_state_machine_stateflow' */
  real_T TimerHover;                   /* '<S1>/fcs_state_machine_stateflow' */
  real_T timer_GPSLoss;                /* '<S1>/fcs_state_machine_stateflow' */
  real_T safety_state_time;            /* '<S1>/fcs_state_machine_stateflow' */
  real_T rampup_timer;                 /* '<S1>/fcs_state_machine_stateflow' */
  real_T bRTH2LandingFlag;             /* '<S1>/fcs_state_machine_stateflow' */
  real_T bRecoverGPS;                  /* '<S1>/fcs_state_machine_stateflow' */
  real_T currentLLA[3];                /* '<S1>/fcs_state_machine_stateflow' */
  real_T bL2RTH_LinkLossFlag;          /* '<S1>/fcs_state_machine_stateflow' */
  real_T TimerEPIPLoss;                /* '<S1>/fcs_state_machine_stateflow' */
  vom_t UnitDelay1_DSTATE_jsa3d2gsdq;  /* '<S1>/Unit Delay1' */
  uint32_T exit_port_index_FW_Modes;   /* '<S1>/fcs_state_machine_stateflow' */
  uint32_T exit_port_index_BackTransition;/* '<S1>/fcs_state_machine_stateflow' */
  uint32_T exit_port_index_FTransition;/* '<S1>/fcs_state_machine_stateflow' */
  uint32_T exit_port_index_FW_RTH;     /* '<S1>/fcs_state_machine_stateflow' */
  uint16_T DelayInput1_DSTATE;         /* '<S70>/Delay Input1' */
  uint16_T DelayInput1_DSTATE_citpu4sg5i;/* '<S71>/Delay Input1' */
  uint16_T temporalCounter_i1;         /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T DelayInput1_DSTATE_lufocpaa5h;/* '<S68>/Delay Input1' */
  uint8_T DelayInput1_DSTATE_brq5z3xipc;/* '<S69>/Delay Input1' */
  boolean_T UnitDelay_DSTATE;          /* '<S1>/Unit Delay' */
  boolean_T DelayInput1_DSTATE_es42msgisa;/* '<S35>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_bgpgka0occ;/* '<S4>/Delay Input1' */
  boolean_T DelayInput1_DSTATE_hdp33oolxt;/* '<S38>/Delay Input1' */
  uint8_T is_active_c4_system_state_machine;/* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_FMM;                      /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_FW_Modes;                 /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_BackTransition;           /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_FTransition;              /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_FW_RTH;                   /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_HOVER;                    /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_RTH;                      /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_STARTUP;                  /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_InAirOnGroundLogic;       /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_Telemtry_LinkLoss;        /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_ep_loss;                  /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_HoverTimer;               /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_GPS_Loss;                 /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_ip_loss;                  /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_EP_IP_Loss_5sec;          /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_Safety_SM;                /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_Loiter_SM;                /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_TECS_SM;                  /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_PIC_SM;                   /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T is_CoG_SM;                   /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T temporalCounter_i2;          /* '<S1>/fcs_state_machine_stateflow' */
  uint8_T temporalCounter_i3;          /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T icLoad;                    /* '<S6>/Delay One Step1' */
  boolean_T bTelemtryLinkLoss5sec;     /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bHover5sec;                /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bTakeoff2Hover;            /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bGPSLoss2sec;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bBTransition2Hover;        /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bBTrans_hover_sp_set;      /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T goto_back_trans;           /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bHover2WPN_yaw_aligned;    /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T ip_loss5sec;               /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T ip_loss250ms;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T ep_loss250ms;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T ep_loss5sec;               /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T ip_loss1sec;               /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T hover2FLTDIR;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T hover2WAYPNT;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T startup2manual;            /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T startup2takeoff;           /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T startup2uman;              /* '<S1>/fcs_state_machine_stateflow' */
  boolean_T bEPIPLossFlag;             /* '<S1>/fcs_state_machine_stateflow' */
} system_state_machine_TDW;

/* Zero-crossing (trigger) state for model 'system_state_machine' */
typedef struct {
  ZCSigState DelayOneStep1_Reset_ZCE;  /* '<S6>/Delay One Step1' */
} system_state_machine_TZCE;

/* Invariant block signals for system '<S8>/FlightManagement.FMM.HOVER.wraptopi' */
typedef struct {
  const real_T Sum1;                   /* '<S66>/Sum1' */
} system_state_machine_mxmu1gefao_TConstB;

/* Invariant block signals for model 'system_state_machine' */
typedef struct {
  const real_T Tan;                    /* '<S5>/Tan' */
  const real_T Sum1;                   /* '<S15>/Sum1' */
  system_state_machine_mxmu1gefao_TConstB FlightManagementFMMRTHwraptopi;
                                  /* '<S8>/FlightManagement.FMM.RTH.wraptopi' */
  system_state_machine_mxmu1gefao_TConstB FlightManagementFMMHOVERwraptopi;
                                /* '<S8>/FlightManagement.FMM.HOVER.wraptopi' */
} system_state_machine_TConstB;

extern void system_state_machine_lkpmp3sm0z(real_T rtu_u, real_T *rty_y, const
  system_state_machine_mxmu1gefao_TConstB *localC);
extern void system_state_machine_mkamotaax2(real_T rtu_Lat, real_T rtu_Lon,
  real_T rtu_dist, real_T rtu_heading, real_T *rty_latOut, real_T *rty_lonOut);
extern void system_state_machine_k5j3b5svff(const real_T rtu_LLA1[3], const
  real_T rtu_LLA2[3], real_T *rty_distance);

/* Invariant block signals (default storage) */
extern const system_state_machine_TConstB system_state_machine_ConstB;

/* Block signals (default storage) */
extern system_state_machine_TB system_state_machine_B;

/* Block states (default storage) */
extern system_state_machine_TDW system_state_machine_DW;

/* Previous zero-crossings (trigger) states */
extern system_state_machine_TZCE system_state_machine_PrevZCX;

#endif                                 /* system_state_machine_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
