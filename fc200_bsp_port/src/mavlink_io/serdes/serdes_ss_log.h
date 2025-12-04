#ifndef SERDES__SS_LOG_H
#define SERDES__SS_LOG_H

#define SERDES_SS_LOG_ID 10

#include <stdint.h>
#include <string.h>
#include "serdes_crc.h"

// Message structure for serialization/deserialization
typedef struct {
    uint32_t logger_time_ms;
    int16_t fcs_mr_eul_rpy_d[3];
    float fcs_mr_yawrate_d;
    int8_t fcs_mr_yaw_hold;
    float fcs_ca_nu_des[4];
    float fcs_ca_nu_alloc[4];
    uint16_t fcs_ca_cerp[3];
    float fcs_mr_h_d;
    float fcs_mr_hdot_d;
    float fcs_mr_h_hold;
    int16_t fcs_mr_vel_ne_d[2];
    int16_t fcs_fw_roll_d;
    int16_t fcs_fw_pitch_d;
    float fcs_fw_h_d;
    uint16_t fcs_fw_cas_d;
    float fcs_mr_pit_intg;
    int8_t fcs_mr_pit_intg_sat;
    float fcs_fw_pit_intg;
    int8_t fcs_fw_pit_intg_sat;
    uint8_t fcs_vom_status;
    uint16_t fcs_status_flags;
    int16_t fcs_euler_rpy[3];
    float fcs_omg_xyz[3];
    float fcs_acc_xyz[3];
    int32_t fcs_lat;
    int32_t fcs_lon;
    float fcs_alt_gps_amsl;
    int16_t fcs_vel_ned[3];
    float fcs_aspd_cas;
    float fcs_alt_baro_amsl;
    float fcs_alt_radalt_filt;
    uint16_t cmd_rotor_cval[8];
    uint16_t cmd_pusher_pwm;
    uint16_t cmd_servo_deg[12];
    uint32_t ins1_time_ms;
    int16_t ins1_euler_rpy[3];
    float ins1_omg[3];
    float ins1_acc[3];
    int32_t ins1_lat;
    int32_t ins1_lon;
    float ins1_alt_amsl;
    int16_t ins1_vel_ned[3];
    uint8_t ins1_kf_pos_cov_lla[3];
    uint8_t ins1_kf_vel_cov_ned[3];
    uint8_t ins1_gnss_sat_used;
    uint16_t ins1_gnss_pdop;
    uint16_t ins1_gnss_h_acc;
    uint16_t ins1_gnss_v_acc;
    uint8_t ins1_gnss_sol_type;
    uint8_t ins1_gnss_info1;
    uint8_t ins1_gnss_info2;
    uint8_t ins1_ins_sol_status;
    uint16_t ins1_flags;
    uint32_t ads_time_ms;
    int16_t ads_aspd_cas;
    int16_t ads_aoa;
    int16_t ads_aos;
    int16_t ads_aspd_tas;
    float ads_alt_baro_amsl;
    int16_t ads_oat_celsius;
    uint16_t ads_flags;
    int32_t radalt_time_ms;
    float radalt_alt_raw;
    float radalt_snr;
    uint16_t radalt_flags;
    int32_t pilot_time_ms;
    int16_t pilot_roll[2];
    int16_t pilot_pitch[2];
    int16_t pilot_thrust[2];
    int16_t pilot_yaw[2];
    int16_t pilot_pusher[2];
    int16_t pilot_arm_ch[2];
    int8_t pilot_sw_a[2];
    int8_t pilot_sw_b[2];
    int8_t pilot_flags[2];
    int8_t pilot_rssi;
    uint32_t epu_time_ms;
    uint16_t epu_rpm[8];
    uint8_t epu_current[8];
    uint8_t epu_voltage[8];
    int8_t epu_temp[8];
    uint32_t servo_time_ms;
    int16_t servo_pos_deg[13];
} serdes_ss_log_t;

// Function declarations
// Returns: 0 on success, negative on error (e.g., -1: null ptr, -2: buffer too small)
int serdes_pack_ss_log(const serdes_ss_log_t *packet, uint8_t *buffer, uint16_t max_size);
// Returns: 0 on success, negative on error (e.g., -1: invalid format, -2: CRC mismatch)
int serdes_unpack_ss_log(const uint8_t *buffer, uint16_t buffer_size, serdes_ss_log_t *packet);

#endif // SERDES__SS_LOG_H