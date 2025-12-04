/****************************************************
 *  ach_interface.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Interface ach_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_ACH_INTERFACE
#define H_ACH_INTERFACE

#include "type.h"
#include "types_servo.h"
#include "types_epu.h"

void ach_init(void);
void ach_read_periodic(void);
void ach_cmd_periodic(void);

/*--------------------------Servo Interface Functions---------------------------------*/
/* Servo Position Command Interface Functions */
void ach_set_servo_cmd_deg(const std_servo_cmd_t *servo_cmd);

/* Servo Feedback Interface Functions */
float ach_get_servo_pos_deg(e_servo_positions_t servo_id);
bool ach_get_servo_lv_alarm(e_servo_positions_t servo_id);
void ach_get_servo_device_status(e_servo_positions_t servo_id, float *current, float *temperature);

/*--------------------------EPU Interface Functions-----------------------------------*/
/* EPU Command Interface Function */
void ach_set_epu_cmd(const std_epu_cmd_t *epu_cmd);

/* EPU Status Interface Function */
bool ach_get_epu_status(uint8_t esc_id, s_esc_status_frame_t *out);

/*-------------------------Pusher Interface Functions---------------------------------*/
void ach_set_pusher_pwm(double duty_cycle);

#endif /*!defined(EA_4959470C_1200_470f_9174_EF9362C24B05__INCLUDED_)*/
