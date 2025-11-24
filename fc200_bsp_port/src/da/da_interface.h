/****************************************************
 *  da_interface.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Interface da_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_DA_INTERFACE
#define H_DA_INTERFACE

#include "type.h" /* Include for 'relation to classifier' 'types' */
#include "types_ins.h"
#include "types_ads.h"
#include "types_sbus.h"

/* Implementation of operation 'da_init' from interface 'DA_interface' */
void da_init(void);

/* Implementation of operation 'da_periodic' from interface 'DA_interface' */
void da_periodic(void);

/* getter functions for all the data input equipment interfacing the FCC */
/*--------------------------------- INS IL GETTERS ----------------------------------*/
bool da_get_ins_il_euler_angles(float *roll, float *pitch, float *yaw);

bool da_get_ins_il_angular_velocity(float *omg_x, float *omg_y, float *omg_z);

bool da_get_ins_il_accelerometer_data(float *accl_x, float *accl_y, float *accl_z);

bool da_get_ins_il_position(double *latitude, double *longitude, float *gps_altitude);

bool da_get_ins_il_inertial_velocity(float *vel_n, float *vel_e, float *vel_d);

bool da_get_ins_il_body_velocity(float *vel_x, float *vel_y, float *vel_z);

bool da_get_ins_il_supply_voltage(float *supply_voltage);

bool da_get_ins_il_temperature(float *temperature);

bool da_get_ins_il_gnss_sat_used(uint8_t *gnss_sat_used);

bool da_get_ins_il_alt_baro(float *alt_baro);

bool da_get_ins_il_eph_epv_data(float *ptr_eph, float *ptr_epv);

bool da_get_ins_il_gnss_dop(float *pdop);

bool da_get_ins_il_gnss_sol_status(uint8_t *pos_type, uint8_t *sol_status);

bool da_get_ins_il_gnss_info(uint8_t *gnss_info1, uint8_t *gnss_info2);

bool da_get_ins_il_kf_cov(uint8_t *pos_cov_lla, uint8_t *vel_cov_ned);

bool da_get_ins_il_ins_sol_status(uint8_t *ins_sol_status);

bool da_get_ins_il_timeout(void);

bool da_get_ins_il_att_invalid(void);

bool da_get_ins_il_omg_invalid(void);

bool da_get_ins_il_accel_invalid(void);

bool da_get_ins_il_pos_invalid(void);

float da_get_ins_msg_rate_hz(void);

/*--------------------------------ADC 9 GETTERS -------------------------------------*/
bool da_get_adc_9_data(adc_data_type_t signal_name, float *data);

uint16_t da_get_adc_9_status(void);

bool da_get_adc_9_cas(float *cas); // return value shows validity

bool da_get_adc_9_aoa(float *tas); // return value shows validity

bool da_get_adc_9_aos(float *aos); // return value shows validity

bool da_get_adc_9_oat(float *oat); // return value shows validity

bool da_get_adc_9_alt(float *alt); // return value shows validity

bool da_get_adc_9_timeout(void); // return value shows validity

float da_get_adc_9_msg_rate_hz(void);

/*--------------------------------- RADALT GETTERS ----------------------------------*/
bool da_get_radalt_timeout(void);
bool da_get_radalt_data(float *agl);
float da_get_radalt_msg_rate_hz(void);

/*--------------------------------- SBUS GETTERS ------------------------------------*/
bool da_get_sbus_timeout(void);
bool da_get_ep_data(rc_input_t *rc_input);

#endif /* H_DA_INTERFACE */
