/****************************************************
 *  mi_interface.h
 *  Created on: 03-Jul-2025 10:30:22 AM
 *  Implementation of the Interface mi_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_MI_INTERFACE
#define H_MI_INTERFACE

#include "type.h"
#include "types_fcs.h"

void fcs_mi_init(void);
void fcs_mi_periodic(void);

// get fcs continious state
void fcs_mi_get_fcs_cont(
    float *euler_rpy,    // in rad
    float *omg_xyz,      // in rad/s
    float *acc_xyz,      // in m/s/s
    int32_t *latitude,   // in degrees * 1E7
    int32_t *longitude,  // in degrees * 1E7
    float *alt_gps_amsl, // in meters
    float *vel_ned,      // NED velocity in m/s
    float *aspd_cas,
    float *alt_radalt_agl);

// get fcs discrete state
void fcs_mi_get_fcs_dscr(
    uint8_t *vom_status,
    uint8_t *safety_status,
    uint8_t *pic_status,
    uint8_t *in_air_status,
    uint8_t *ep_loss,
    uint8_t *ip_loss,
    uint8_t *gnss_loss,
    uint8_t *ins_selection,
    uint8_t *adc_selection,
    uint16_t *current_waypoint_idx);

void fcs_mi_get_fbctrl(
    fcs_mi_fbctrl_data_t *fbctrl_data // Feedback control data
);

int fcs_mi_get_act_cmd(
    float *motor_cmd,  // Motor commands in RPM (PoC) or normalized [0,1] (SS)
    float *servo_cmd,  // Servo commands in degrees
    float *pusher_cmd, // Pusher command in RPM, PWM (SS)
    uint8_t num_motors,
    uint8_t num_servos);

#endif /*!defined(H_MI_INTERFACE)*/
