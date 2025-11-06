/****************************************************
 *  types_ins.h
 *  Created on: 07-Jul-2025 3:53:27 PM
 *  Implementation of the Interface types_ins
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_TYPES_INS
#define H_TYPES_INS

#include "type.h"

/*-------------------------------------- SCALED & PROCESSED DATA STRUCTURE ------------------------------------------*/

typedef struct
{
    float roll;  /* Euler Angle: roll  [rad] */
    float pitch; /* Euler Angle: pitch [rad] */
    float yaw;   /* Euler Angle: yaw   [rad] */
} s_scaled_euler_angle_t;

typedef struct
{
    float vel_n; /* Inertial Speeds (NED Frame) [m/s]*/
    float vel_e;
    float vel_d;
} s_scaled_velocities_t;

typedef struct
{
    float vel_x; /* Body frame speeds (FRD) [m/s] */
    float vel_y;
    float vel_z;
} s_scaled_body_frame_vel_t;

typedef struct
{
    float mag_x; /* Magnetometer X, nT/10 */
    float mag_y; /* Magnetometer Y, nT/10 */
    float mag_z; /* Magnetometer Z, nT/10 */
} s_scaled_mag_data_t;

typedef struct
{
    double lat; /* Latitude      [deg] */
    double lon; /* Longitude     [deg] */
    float alt;  /* Altitude      [m]   */
} s_scaled_pos_data_t;

typedef struct
{
    float baro_p; /*  Raw absolute pressure measured by the barometer sensor  [Pa]*/
    float baro_h; /* Barometric Altitude  [m] */
} s_scaled_baro_data_t;

typedef struct
{
    float accel_e; /* East Inertial Frame accelerations     [m/s^2] */
    float accel_n; /* North Inertial Frame accelerations    [m/s^2] */
    float accel_d; /* Down Inertial Frame accelerations     [m/s^2] */
} s_scaled_enu_lin_accl_t;

typedef struct
{
    float gyro_x;  /* x-axis Body Filtered Gyro   [rad/s] */
    float gyro_y;  /* y-axis Body Filtered Gyro   [rad/s] */
    float gyro_z;  /* z-axis Body Filtered Gyro   [rad/s] */
    float accel_x; /* x-axis Body Filtered Acc    [m/s^2] */
    float accel_y; /* y-axis Body Filtered Acc    [m/s^2] */
    float accel_z; /* z-axis Body Filtered Acc    [m/s^2] */
} s_scaled_imu_data_t;

typedef struct
{
    float pitch_std;       /* roll STD, deg*1000 */
    float roll_std;        /* Pitch STD, deg*1000*/
} s_scaled_roll_pit_cov_t; /* KF pitch, roll covariance */

typedef struct
{
    float heading_std; /* Heading STD, deg*100 */
} s_scaled_heading_cov_t;

typedef struct
{
    float gns_lat_std; /* GNSS Latitude STD, m*1000 */
    float gns_lon_std; /* GNSS Longitude STD, m*1000 */
    float gns_alt_std; /* GNSS Altitude STD, m*1000 */
} s_scaled_gnss_pos_std_t;

typedef struct
{
    float gdop; /* GDOP, *10^3 */
    float pdop; /* PDOP, *10^3 */
} s_scaled_gdop_pdop_t;

typedef struct
{
    float gns_heading_std; /* GNSS Heading STD, deg*100 */
    float gns_pitch_std;   /* GNSS Pitch STD, deg*100 */
} s_scaled_gnss_head_pitch_std_t;

typedef struct
{
    float inp_voltage; /* VDC*100 */
} s_scaled_insd_supply_voltage_t;

typedef struct
{
    float insd_temperature; /* Temperature, C*10 */
} s_scaled_insd_temperature_t;

typedef struct
{
    float eph;
    float epv;
} s_scaled_eph_epv_t;

typedef struct
{
    double gnss_lat; /* Latitude      [deg] */
    double gnss_lon; /* Longitude     [deg] */
    double gnss_alt; /* Altitude      [m]   */
} s_scaled_gnss_position_t;

typedef struct
{
    float horizontal_speed_gnss;
    float track_over_ground_gnss;
    float vertical_speed_gnss;
} s_scaled_gnss_velocity_t;

typedef struct
{
    float eul_rpy[3];    /* Euler angles [rad] */
    float omg_xyz[3];    /* Angular rates in body frame [rad/s] */
    float acc_xyz[3];    /* Accelerations in body frame [m/s^2] */
    float vel_ned[3];    /* NED velocities [m/s] */
    double lat;          /* Latitude [deg] */
    double lon;          /* Longitude [deg] */
    float alt_gnss_amsl; /* Altitude AMSL [m] */
    float eph;           /* Horizontal Position Error [m] */
    float epv;           /* Vertical Position Error [m] */
    bool att_invalid;    /* Flag to indicate if attitude data is invalid */
    bool acc_invalid;    /* Flag to indicate if accelerometer data is valid */
    bool omg_invalid;    /* Flag to indicate if gyro data is valid */
    bool pos_invalid;    /* Flag to indicate if position data is invalid */
    bool data_timeout;   /* Flag to indicate if the INS communication timed out */
} std_ins_data_t;

#endif /* H_TYPES_INS */
