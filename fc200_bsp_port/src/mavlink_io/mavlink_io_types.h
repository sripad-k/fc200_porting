
/****************************************************
 *  mavlink_io_types.h
 *  Created on: 07-Jul-2025 12:44:27 PM
 *  Implementation of the Interface mavlink_io_types
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_MAVLINK_IO_TYPES
#define H_MAVLINK_IO_TYPES

#include "type.h"
#include "types_sbus.h"
#include "types_fcs.h"

#define MAX_MISSION_ITEMS 100

#define MAVIO_NUM_MOTORS 8
#define MAVIO_NUM_SERVOS 13

typedef enum
{
    WP_FLYBY = 1,
    WP_FLYOVER = 2,
    WP_LAND = 3
} mavio_wp_type_t;

typedef struct
{
    bool wp_list_valid;
    uint16_t wp_count;
    uint16_t active_wp_idx;
    bool last_wp_land;
    int32_t lat[MAX_MISSION_ITEMS];             // Latitude in 1E7 degrees
    int32_t lon[MAX_MISSION_ITEMS];             // Longitude in 1E7 degrees
    float alt[MAX_MISSION_ITEMS];               // Altitude in meters
    mavio_wp_type_t wp_type[MAX_MISSION_ITEMS]; // Waypoint type (flyby, flyover, land)
} mavio_wp_list_t;

typedef struct
{
    /* ins solution*/
    float euler_rpy[3];
    float omg[3];
    float acc[3];      // in m/s^2
    int32_t latitude;  // in degrees * 1E7
    int32_t longitude; // in degrees * 1E7
    float alt_amsl;    // in meters
    float vel_ned[3];  // NED velocity in m/s

    uint8_t kf_pos_cov_lla[3]; // KF position covariance in cm
    uint8_t kf_vel_cov_ned[3]; // KF velocity covariance in mm/s

    /* GNSS */
    uint8_t gnss_sat_used; // number of GNSS satellites visible
    float gnss_hdop;       // horizontal dilution of precision
    float gnss_vdop;       // vertical dilution of precision
    float gnss_h_accuracy; // horizontal accuracy in meters
    float gnss_v_accuracy; // vertical accuracy in meters
    uint8_t gnss_sol_type; // GNSS solution type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)

    /* health status*/
    uint8_t ins_healthy;
    uint8_t gyro_healthy;
    uint8_t acc_healthy;
    uint8_t baro_healthy;
    uint8_t gnss_healthy;
    uint8_t data_timeout;
    uint8_t att_invalid;
    uint8_t pos_invalid;

    float temp; // temperature

} mavio_ins_data_t;

typedef struct
{
    /* ADC data */
    float aspd_cas;      // in m/s
    float aoa;           // angle of attack in rad
    float aos;           // angle of sideslip in rad
    float aspd_tas;      // in m/s
    float alt_baro_amsl; // in meters
    float oat_celsius;   // in degrees Celsius

    /* health status */
    uint8_t adc_healthy; // ADC health status
    uint8_t data_timeout;
    uint8_t aspd_cas_invalid;
    uint8_t aoa_invalid;
    uint8_t aos_invalid;
    uint8_t aspd_tas_invalid;
    uint8_t alt_baro_invalid;

} mavio_adc_data_t;

typedef struct
{
    /* mavlink system ID */
    uint8_t sys_id;

    /* mavlink component ID */
    uint8_t comp_id;

    /* mavlink system type */
    uint8_t sys_type;

    /* mavlink autopilot type */
    uint8_t autopilot_type;

} mavio_system_t;

/* mavlink_io module input data to be set before sending mavlink messages*/
typedef struct
{
    /* voted data used by FCS */
    float euler_rpy[3]; // in rad
    float omg_xyz[3];   // in rad/s
    float acc_xyz[3];   // in m/s/s
    int32_t latitude;   // in degrees * 1E7
    int32_t longitude;  // in degrees * 1E7
    float alt_gps_amsl; // in meters
    float vel_ned[3];   // NED velocity in m/s

    float aspd_cas;      // in m/s
    float aspd_tas;      // in m/s
    float alt_baro_amsl; // in meters
    float oat_celsius;   // in degrees Celsius

    float alt_radalt_filt; // in meters from FCS

    fcs_mi_fbctrl_data_t fbctrl_data;

    /* FCS discrete state*/
    uint8_t vom_status;
    uint8_t safety_status;
    uint8_t pic_status;
    uint8_t in_air_status;
    uint8_t ep_data_loss; // external pilot data loss
    uint8_t ip_data_loss; // internal pilot data loss
    uint8_t gnss_loss;    // GNSS data loss
    uint8_t tecs_on;
    uint8_t loiter_on;
    uint8_t cog_track_on;

    /* WPN VOM info*/
    uint16_t current_waypoint_idx; // currently executing waypoint index
    uint16_t prev_waypoint_idx;    // previous value of current_waypoint_idx

    /* dual INS data */
    mavio_ins_data_t ins_data[2];
    uint8_t ins_selection; // 0: INS1, 1: INS2

    /* dual ADC data */
    mavio_adc_data_t adc_data[2];
    uint8_t adc_selection; // 0: ADC1, 1: ADC2

    /* radalt data */
    float radalt_agl; // in meters
    float radalt_snr; // in dB
    bool radalt_timeout;

    /* RC input (external pilot)*/
    rc_input_t rc_input;

    /* Actuator Commands*/
    float motor_cmd[MAVIO_NUM_MOTORS]; // Motor commands in RPM (PoC) or normalized [0,1] (SS)
    float servo_cmd[MAVIO_NUM_SERVOS]; // Servo commands in degrees
    float pusher_cmd;                  // Pusher command in RPM, PWM (SS)

    /* T-Motor feedback data*/
    struct epu_tmotor_data_t
    {
        float rpm[MAVIO_NUM_MOTORS];
        float voltage[MAVIO_NUM_MOTORS];
        float current[MAVIO_NUM_MOTORS];
        float temp[MAVIO_NUM_MOTORS];
    } tmotor_data;

    /* KST Servo feedback data*/
    struct act_kst_data_t
    {
        float pos_deg[MAVIO_NUM_SERVOS];
        float current[MAVIO_NUM_SERVOS];
        float temp[MAVIO_NUM_SERVOS];
        uint16_t lv_status; // low voltage status bitfield
    } kst_data;

} mavio_in_t;

/* mavlink_io module output data set by received mavlink messages*/
typedef struct
{
    // vom command from gcs
    uint8_t vom_cmd;
    uint16_t vom_cmd_cnt;

    uint8_t pic_cmd;
    uint16_t pic_cmd_cnt;

    uint8_t loiter_on;
    uint8_t loiter_on_cnt;

    uint8_t tecs_on;
    uint8_t tecs_on_cnt;

    uint8_t precharge_enable;

    uint8_t lights_enable;

    bool gcs_link_lost; // true if the GCS link is lost

    // internal pilot input
    struct ip_input_t
    {
        uint32_t counter;
        float roll;   // -1.0 to 1.0
        float pitch;  // -1.0 to 1.0
        float yaw;    // -1.0 to 1.0
        float thrust; // -1.0 to 1.0
        float pusher; // -1.0 to 1.0

        bool link_lost; // true if the link is lost
    } ip_input;

    mavio_wp_list_t wp_list;

} mavio_out_t;

typedef struct
{
    mavio_ins_data_t ins_data[2];
    mavio_adc_data_t adc_data[2];

    float radalt_agl; // in meters

    float rotor_rpm[8];  /*< [RPM] rotor rpm as reported by EPU*/
    float pusher_rpm;    /*< [RPM] pusher rpm as reported by engine*/
    float servo_def[12]; /*< [degE2] actuator deflection reported by servo +/- 180 deg range*/

    int8_t wow[3];
} pil_in_t;

typedef struct
{
    float rotor_cmd_cval[8]; /*< [RPM] rotor rpm as reported by EPU*/
    float pusher_cmd_pwm;    /*< [RPM] pusher rpm as reported by engine*/
    float servo_cmd_deg[12]; /*< [degE2] actuator deflection reported by servo +/- 180 deg range*/

} pil_out_t;

#endif /*!defined(H_MAVLINK_IO_TYPES)*/