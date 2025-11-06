/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: transitionHandler.c
 *
 * Code generated for Simulink model 'transitionHandler'.
 *
 * Model version                  : 2.22
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * Git Hash                       : 109356e0
 * C/C++ source code generated on : Mon Sep 15 11:52:52 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "transitionHandler.h"
#include "transitionHandler_types.h"
#include <string.h>

/* Output and update for referenced model: 'transitionHandler' */
void transitionHandler(const busControllerCA *rtu_CA_MR, const busControllerCA
  *rtu_CA_FW, std_ctrl_t *rty_stdCtrl_IF, busControllerCA *rty_controllerCA_mix)
{
  /* BusAssignment: '<Root>/Bus Assignment1' */
  *rty_controllerCA_mix = *rtu_CA_MR;
  (void)memset(rty_stdCtrl_IF, 0, sizeof(std_ctrl_t));

  /* BusAssignment: '<Root>/Bus Assignment2' */
  (void)memcpy(&rty_stdCtrl_IF->lifter_cval_cmd[0],
               &rtu_CA_MR->stdCtrl_IF.lifter_cval_cmd[0], (sizeof(real_T)) << 3U);
  (void)memcpy(&rty_stdCtrl_IF->acs_servo_deg_cmd[0],
               &rtu_CA_FW->stdCtrl_IF.acs_servo_deg_cmd[0], 12U * (sizeof(real_T)));
  rty_stdCtrl_IF->pusher_pwm_cmd = rtu_CA_FW->stdCtrl_IF.pusher_pwm_cmd;

  /* BusAssignment: '<Root>/Bus Assignment1' */
  rty_controllerCA_mix->stdCtrl_IF = *rty_stdCtrl_IF;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
