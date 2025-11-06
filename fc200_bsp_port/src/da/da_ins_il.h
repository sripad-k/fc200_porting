/****************************************************
 *  da_ins_il.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_ins_il
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_DA_INS_IL
#define H_DA_INS_IL

#include "uart_interface.h" /* Adjusted path for 'UART_interface' */
#include "types_ins.h"      /* Adjusted path for 'TYPES_ins_d' */
#include "da_interface.h"

// Standard deviation thresholds
#define THR_SIG_LAT 10.0f  // [m]
#define THR_SIG_LON 10.0f  // [m]
#define THR_SIG_H 10.0f    // [m]
#define THR_SIG_VN 1.0f    // [m/s]
#define THR_SIG_VE 1.0f    // [m/s]
#define THR_SIG_VD 1.0f    // [m/s]
#define THR_SIG_ROLL 0.5f  // [deg]
#define THR_SIG_PITCH 0.5f // [deg]
#define THR_SIG_YAW 1.0f   // [deg]

// Header Characters
#define IL_SYNC_1 0xFF // Message Sync Char #1
#define IL_SYNC_2 0x5A // Message Sync Char #2
#define IL_ETX 0x33    // End of Frame Char
#define IL_OH 9        // IL Message Overheaod

// Physical Conversion Constants
#define KG 50.0f      // Gyro Range max = 450 deg/s (Table 6.6 [1 pp. 83]) in [deg/s *KG]
#define KA 2000.0f    // Accelerometer range, g = 8 (Table 6.6 [1 pp. 83]) in [g*KA]
#define nT 0.1f       // Magnetic Field (Table 6.5)
#define g_ref 9.8106f // Gravity

/**
 * @brief Message length: Payload + Overhead + CRC
 * @note
 */
#define INS_MAX_MSG_LEN 200

/**
 * @brief Length of the INS Overhead message
 *
 */
#define INS_OVERHEAD_LEN 6
/**
 * @brief Length of the payload message (max_msg_len_byts - overhead - checksum)
 * @note  OPVT: 92, OPVT2AW: 103, UDD: TBD
 *
 */
#define INS_PAYLOAD_MSG_LEN 190

/**
 * @brief  Inertial Labs decode data formats
 */
enum
{
    IL_DCODE_OPVT = 0,
    IL_DCODE_QPVT = 1,
    IL_DCODE_OPVT2A = 2,
    IL_DCODE_OPVT2AW = 3,
    IL_DCODE_OPVT2HR = 4,
    IL_DCODE_OPVTAD = 5,
    IL_DCODE_UDD = 6,
};

/**
 * @brief   IL Fault status
 */
enum
{
    IL_DCODE_PENDING = 10,       // So far, all good; waiting for the things to come
    IL_DCODE_FAILED_START1 = 11, // Initalization fail
    IL_DCODE_FAILED_START2 = 12, // Fail during header scan
    IL_DCODE_FAILED_LENGTH = 13, //
    IL_DCODE_FAILED_CRC = 14,    // fail during payload scan
    IL_DCODE_FAILED_ERROR = 15,  // general fail
    IL_DCODE_CRC_OK = 16,        // success
};

/**
 * @brief GNSS_INFO1 Position Types
 *
 */
typedef enum gnss_info1_position_type
{
    SINGLE_POINT_POS = 0,
    PSEUDORANGE_DIFF_SOL = 1,
    SOL_WITH_SBAS_CORR = 2,
    PPP_SOL = 3,
    RTK_OTHER_SOL = 4,
    RTK_NARROW_INT_SOL = 5,
    OTHER = 6,
    CONVERGING_TERRASTAR = 7,
    CONVERGED_TERRASTAR = 8,
    CONVERGING_TERRASTARL = 9,
    CONVERGED_TERRASTARL = 10,
} gnss_info1_position_type;

/**
 * @brief GNSS_INFO1 Pseudorange iono correction
 *
 */
typedef enum gnss_info1_pseudorange_iono_corr
{
    UNKOWN_DEFAULT_KLOBUCHAR_MDL = 0,
    KLOBUCHAR_BROADCAST = 1,
    SBAS_BROADCAST = 2,
    MULTI_FREQ_COMP = 3,
    PSEUDORANGE_DIFF_CORR = 4,
    NOVATEL_BLENDED_IONO_VAL = 5,
} gnss_info1_pseudorange_iono_corr;

/**
 * @brief GNSS_INFO2 Solution status (EKF convergence)
 *
 */
typedef enum gnss_info2_solution_status
{
    GNSS_SOL_COMPUTED = 0,
    INSUFF_OBSV = 1,
    NOT_CONVERGED = 2,
    NO_SOL = 3,
} gnss_info2_solution_status;

/**
 * @brief GNSS_INFO2 Reference Time Status
 *
 */
typedef enum gnss_info2_ref_time_status
{
    TIME_VALIDTY_UNKNOWN = 0,
    TIME_COARSE = 1,
    POSITION_LOST = 2,
    TIME_FINE = 3,
} gnss_info2_ref_time_status;

/**
 * @brief   Message structure
 */
typedef struct il_msg_s
{
    uint8_t state;                     //!< State of the decoding engine (faster version of
                                       //!< sbg_msg_deframe)
    uint8_t flag;                      //!< New Message flag
    uint8_t id;                        //!< Message id (0-255)
    uint8_t cls;                       //!< Message class (0-255)
    uint16_t len;                      //!< Size in bytes of the data payload (less than 4086).
    uint8_t data[INS_PAYLOAD_MSG_LEN]; //!< Payload message
} il_msg_s;

/**
 * @brief Unit status word fields
 */
typedef union
{
    struct
    {
        bool init_align : 1;
        bool sw_status : 1;
        bool gyro_status : 1;
        bool acc_status : 1;
        bool mag_status : 1;
        bool elec_status : 1;
        bool gnss_rcvr : 1;
        bool otf_calib_calcs : 1;
        bool vcc_too_low : 1;
        bool vcc_too_high : 1;
        bool ang_vel_x_exc : 1;
        bool ang_vel_y_exc : 1;
        bool ang_vel_z_exc : 1;
        bool mag_field_exc : 1;
        bool oat_exc : 1; // oat: outside air temperature
        bool otf_calib_run : 1;
    };

    uint16_t value;
} ins_usw_status_t;

/**
 * @brief Structure to assign GNSS_Info1 fields
 *
 */
typedef union
{
    struct
    {
        uint8_t position_type : 4;
        uint8_t pseudorange_iono_corr : 4;
    };

    uint16_t value;
} gnss_info1_t;

/**
 * @brief Structure to assign GNSS_Info2 fields
 *
 */
typedef union
{
    struct
    {
        uint8_t solution_status : 2;
        uint8_t gps_ref_time_status : 2;
        bool gps_used : 1;
        bool glonass_used : 1;
        bool galileo_used : 1;
        bool beidou_used : 1;
    };

    uint16_t value;
} gnss_info2_t;

/**
 * @brief Structure to assign 'New_GPS' fields
 * @note The INS onboard GNSS receiver generates 1PPS signal at all times, but the
New_GPS indicator sets bit #3 to 1 only if the GPS reference time status is fine.
 */
typedef union
{
    struct
    {
        bool gnss_pos_update : 1;
        bool gnss_vel_update : 1;
        bool gnss_hdg_update : 1;
        bool recv_valid_1pps_sig : 1;
        bool gnss_bestxyz_log_update : 1;
        bool gnss_psrdop_log_update : 1;
        bool rcv_1pps_sig : 1;
        bool gnss_range_log_update : 1;
    };

    uint8_t value;
} new_gnss_data_t;

/**
 * @brief UTC Time Data Abstraction Structure
 *
 */
typedef struct utc_s
{
    bool flag;     //!< Set to true when new data available   [ ]
    uint32_t time; //!< Time from system power up             [us]
    uint16_t stat; //!< UTC status
    uint16_t year; //!< Year                                  []
    uint8_t month; //!< Month in Year [1 ... 12]              []
    uint8_t day;   //!< Day in Month [1 ... 31]               []
    uint8_t hour;  //!< Hour in day [0 ... 23]                []
    uint8_t min;   //!< Minute in hour [0 ... 59]             [s]
    uint8_t sec;   //!< Second in minute [0 ... 60]           []
    uint32_t nsec; //!< Nanosecond of second                  []
    uint32_t gtow; //!< GPS Time of week                      [ms]
    uint16_t week; //!< GPS week                              []
} utc_s;

/*--------------------------------------------- RAW DATA STRUCTURE ----------------------------------------*/

/* 0x07 - Orientation angles */
typedef struct
{
    uint16_t yaw;  /* Heading, deg*100  */
    int16_t pitch; /* Pitch, deg*100    */
    int16_t roll;  /* Roll, deg*100     */
} s_orientation_angles_t;

/* 0x97 -  IMU data filtered */
typedef struct
{
    int16_t gyro_x;  /* Gyro X filtered, deg/s*KG       */
    int16_t gyro_y;  /* Gyro Y filtered, deg/s*KG       */
    int16_t gyro_z;  /* Gyro Z filtered, deg/s*KG       */
    int16_t accel_x; /* Accelerometer X filtered, g*KA  */
    int16_t accel_y; /* Accelerometer Y filtered, g*KA  */
    int16_t accel_z; /* Accelerometer Z filtered, g*KA  */
} s_imu_data_filtered_t;

/* 0x24 -  Magnetometer data */
typedef struct
{
    int16_t mag_x; /* Magnetometer X, nT/10 */
    int16_t mag_y; /* Magnetometer Y, nT/10 */
    int16_t mag_z; /* Magnetometer Z, nT/10 */
} s_magnetometer_data_t;

/* 0x53 - Unit Status Word  */
typedef union
{
    uint16_t s_raw_usw; /* Raw 16-bit value (2 bytes) */
    struct
    {
        uint16_t s_init_align : 1;    /* Bit 0: Initial alignment */
        uint16_t s_sw_status : 1;     /* Bit 1: Software status */
        uint16_t s_gyro_status : 1;   /* Bit 2: Gyroscope unit */
        uint16_t s_acc_status : 1;    /* Bit 3: Accelerometer unit */
        uint16_t s_mag_status : 1;    /* Bit 4: Magnetometer unit */
        uint16_t s_elec_status : 1;   /* Bit 5: Electronics */
        uint16_t s_gnss_rcvr : 1;     /* Bit 6: GNSS receiver */
        uint16_t s_otf_calib : 1;     /* Bit 7: On-the-fly calibration */
        uint16_t s_power_supply : 2;  /* Bits 8-9: Incorrect power supply */
        uint16_t s_ang_rate_exc : 3;  /* Bits 10-12: Angular rate exceeding detect */
        uint16_t s_mag_field_exc : 1; /* Bit 13: Large magnetic field detect */
        uint16_t s_env_temp : 1;      /* Bit 14: Environmental temperature */
        uint16_t s_otf_calib_run : 1; /* Bit 15: On-the-fly calibration */
    } s_status_bits;
} u_unit_status_word_t;

/* 0x10 - Position */
typedef struct
{
    int32_t latitude;  /* Latitude, deg*1.0e7 */
    int32_t longitude; /* Longitude, deg*1.0e7 */
    int32_t altitude;  /* Altitude, m*100 */
} s_position_t;

/* 0x12 - Velocities */
typedef struct
{
    int32_t east_speed;     /* East speed, m/s*100 */
    int32_t north_speed;    /* North speed, m/s*100 */
    int32_t vertical_speed; /* Vertical speed, m/s*100 */
} s_velocities_t;

/*0x02 - GPS Time */
typedef struct
{
    uint64_t gps_ins_time;
} s_gps_ins_time_t;

/* 0x36 - GNSS info1 (first byte) */
typedef union
{
    uint8_t raw_gnss_info1; /* Raw 8-bit value (1 byte) */

    struct
    {
        /* Bits 0-3: Position type */
        uint8_t position_type : 4; /* 0 - Single point position, 1 - DGPS, 2 - Solution using SBAS, 3 - PPP, 4 - RTK (other), 5 - RTK (narrow-int), 6 - Other, 7 - Converging TerraStar-C, 8 - Converged TerraStar-C, 9 - Converging TerraStar-L, 10 - Converging TerraStar-L solution */

        /* Bits 4-7: Pseudorange iono correction */
        uint8_t pseudorange_iono_corr : 4; /* 0 - Unknown or default Klobuchar model, 1 - Klobuchar broadcast, 2 - SBAS broadcast, 3 - Multi-frequency computed, 4 - DGPS, 5 - NovAtel blended iono value */
    } gnss_info1_bits;
} s_gnss_info1_t;

/* 0x36 - GNSS info2 (second byte) */
typedef union
{
    uint8_t raw_gnss_info2; /* Raw 8-bit value (1 byte) */
    struct
    {
        /* Bits 0-1: Solution status */
        /* 0 - GNSS solution is computed,
           1 - Insufficient observations,
           2 - Not yet converged from cold start,
           3 - Other reason of absent solution
        */
        uint8_t solution_status : 2;

        /* Bits 2-3: GPS reference time status */
        /* 0 - Time validity is unknown,
         * 1 - Time is coarse set and is being steered,
         * 2 - Position is lost and the range bias cannot be calculated,
         * 3 - Time is fine set and is being steered
         */
        uint8_t gps_time_status : 2;

        /* Bits 4-7: GNSS signals used */
        uint8_t gps_gnss_signal_used : 1;     /* Bit 4 - GPS GNSS signal is used     */
        uint8_t glonass_gnss_signal_used : 1; /* Bit 5 - GLONASS GNSS signal is used */
        uint8_t galileo_gnss_signal_used : 1; /* Bit 6 - Galileo GNSS signal is used */
        uint8_t beidou_gnss_signal_used : 1;  /* Bit 7 - BeiDou GNSS signal is used  */

    } gnss_info2_bits;
} s_gnss_info2_t;

/* 0x36 - GNSS info short */
typedef struct
{
    s_gnss_info1_t gnss_info1; /* Position type, pseudorange iono correction (see Table 6.7) */
    s_gnss_info2_t gnss_info2; /* Solution status, time status, GNSS constellations (see Table 6.8) */
} s_gnss_info_short_t;

/* 0x3B - Number of satellites used in solution */
typedef struct
{
    uint8_t num_satellites; /* Number of satellites used in solution */
} s_number_of_satellites_t;

/* 0x25 - Barometer data */
typedef struct
{
    uint16_t pressure;     /* Pressure, Pa/2 */
    int32_t baro_altitude; /* Baro altitude, m*100 */
} s_barometer_data_t;

/* 0x54 - INS solution status */
typedef struct
{
    uint8_t status; /* See section 6.16 */
} s_navigation_solution_status_t;

/* 0x42 - Dilution of precision */
typedef struct
{
    uint16_t gdop; /* GDOP, *10^3 */
    uint16_t pdop; /* PDOP, *10^3 */
} s_gdop_pdop_t;

/* 0x1D - Body frame velocity */
typedef struct
{
    int32_t velocity_y; /* Lateral velocity, m/s*100 */
    int32_t velocity_x; /* Longitudinal velocity, m/s*100 */
    int32_t velocity_z; /* Vertical velocity, m/s*100 */

} s_body_frame_velocity_t;

/* 0x1E - ENU linear acceleration */
typedef struct
{
    int32_t east_accel;     /* East acceleration, m/s^2 *1.0e6 */
    int32_t north_accel;    /* North acceleration, m/s^2 *1.0e6 */
    int32_t vertical_accel; /* Vertical acceleration, m/s^2 *1.0e6 */
} s_enu_linear_acceleration_t;

/* 0x55 - KF position covariance */
typedef struct
{
    uint8_t lat_std; /* Latitude STD, cm */
    uint8_t lon_std; /* Longitude STD, cm */
    uint8_t alt_std; /* Altitude STD, cm */
} s_kf_position_covariance_t;

/* 0x58 - KF velocity covariance */
typedef struct
{
    uint8_t vel_n_std; /* Vel N STD, mm */
    uint8_t vel_e_std; /* Vel E STD, mm */
    uint8_t vel_d_std; /* Vel D STD, mm */
} s_kf_velocity_covariance_t;

/* 0x5C - KF pitch, roll covariance */
typedef struct
{
    uint16_t pitch_std; /* Pitch STD, deg*1000 */
    uint16_t roll_std;  /* Roll STD, deg*1000 */
} s_kf_pitch_roll_covariance_t;

/* 0x56 - KF heading covariance */
typedef struct
{
    uint8_t heading_std; /* Heading STD, deg*100 */
} s_kf_heading_covariance_t;

/* 0x41 - New GPS */
typedef union
{
    struct
    {
        uint8_t gnss_position_update : 1; /* bit 0: gnss position data update */
        uint8_t gnss_velocity_update : 1; /* bit 1: gnss velocity data update */
        uint8_t gnss_heading_update : 1;  /* bit 2: gnss heading data update */
        uint8_t valid_1pps_signal : 1;    /* bit 3: valid 1pps signal received */
        uint8_t gnss_bestxyz_update : 1;  /* bit 4: gnss bestxyz log update */
        uint8_t gnss_psrdop_update : 1;   /* bit 5: gnss psrdop log update */
        uint8_t pps_signal_received : 1;  /* bit 6: 1pps signal received regardless of its validity */
        uint8_t gnss_range_update : 1;    /* bit 7: gnss range log update */
    };
    uint8_t new_gps; /* full 8-bit value */
} u_new_gps_t;

/* 0x5E - Heading correction source */
typedef struct
{
    uint8_t source; /* 0 – no correction, 1 – magnetometers, 2 – external, 3 – inertial, 4 – dual GNSS */
} s_heading_correction_source_t;

/* 0x34 - GNSS position STD  */
typedef struct
{
    uint16_t gnss_lat_std;  /* GNSS Latitude STD, m*1000 */
    uint16_t gnss_long_std; /* GNSS Longitude STD, m*1000 */
    uint16_t gnss_alt_std;  /* GNSS Altitude STD, m*1000 */
} s_gnss_data_std_t;

typedef struct
{
    uint16_t gnss_heading_std; /* GNSS Heading STD, deg*100 */
    uint16_t gnss_pitch_std;   /* GNSS Pitch STD, deg*100 */
} s_gnss_heading_pitch_std_t;

typedef struct
{
    uint8_t gnss_solution_status; /* GNSS Solution status (See Table 6.17 Datasheet ) */
} s_gnss_solution_status_t;

typedef struct
{
    uint16_t supply_voltage; /* VDC*100 */
} s_insd_supply_voltage_t;

typedef struct
{
    int16_t temperature; /* Temperature, C*10 */
} s_insd_temperature_t;

/* 0x30 - Position */
typedef struct
{
    int32_t gnss_latitude;  /* Latitude, deg*1.0e7 */
    int32_t gnss_longitude; /* Longitude, deg*1.0e7 */
    int32_t gnss_altitude;  /* Altitude, m*100 */
} s_gnss_position_t;

/* 0x32 - GNSS Velocity, Track over ground */
typedef struct
{
    int32_t horizontal_speed_gnss;
    uint16_t track_over_ground_gnss;
    int32_t vertical_speed_gnss;
} s_gnss_velocity_t;

typedef struct
{
    int32_t unscaled_lat;
    int32_t unscaled_lon;
    float unscaled_alt;

} s_unscaled_pos_data_t;

typedef struct
{
    /* data */
    s_orientation_angles_t orientation_angles;
    s_imu_data_filtered_t imu_data_filtered;
    s_magnetometer_data_t magnetometer_data;
    u_unit_status_word_t unit_status_word;
    s_insd_supply_voltage_t v_inp;
    s_insd_temperature_t temp;
    s_position_t position;
    s_velocities_t velocities;
    s_gnss_position_t gnss_position; /* GNSS Position */
    s_gnss_velocity_t gnss_velocity; /* GNSS Velocity */
    uint32_t GPS_ms;                 /* milliseconds from the beginning of the GPS reference week */
    s_gnss_info_short_t gnss_info_short;
    s_number_of_satellites_t no_of_sat_used; /* Number of satellites used in the navigation solution */
    uint8_t Latency_ms_pos;                  /* latencies of time stamps in the GNSS receiver’s position */
    uint8_t Latency_ms_vel;                  /* latencies of time stamps in the GNSS receiver’s velocity */
    s_barometer_data_t barometer_data;
    u_new_gps_t new_gps_data;

} s_da_ins_opvt_t;

/* Main struct to encapsulate all selected user defined data types */
typedef struct
{
    s_orientation_angles_t orientation_angles;
    s_imu_data_filtered_t imu_data_filtered;
    s_magnetometer_data_t magnetometer_data;
    u_unit_status_word_t unit_status_word;
    s_position_t position;
    s_velocities_t velocities;
    s_gps_ins_time_t gps_ins_time_val;
    s_gnss_info_short_t gnss_info_short;
    s_number_of_satellites_t no_of_sat_used;
    s_barometer_data_t barometer_data;
    s_navigation_solution_status_t navigation_sol_status;
    s_gdop_pdop_t gdop_pdop;
    s_body_frame_velocity_t body_frame_velocity;
    s_enu_linear_acceleration_t enu_linear_acceleration;
    s_kf_position_covariance_t kf_position_covariance;
    s_kf_velocity_covariance_t kf_velocity_covariance;
    s_kf_pitch_roll_covariance_t kf_pitch_roll_covariance;
    s_kf_heading_covariance_t kf_heading_covariance;
    u_new_gps_t new_gps_data;
    s_heading_correction_source_t heading_correction_source;
    s_gnss_data_std_t gnss_data_std;
    s_gnss_heading_pitch_std_t gnss_heading_pitch_std;
    s_gnss_solution_status_t gnss_sol_status;
    s_insd_supply_voltage_t v_inp;
    s_insd_temperature_t temp;

} s_da_ins_udd_t;

typedef struct
{
    s_scaled_euler_angle_t scaled_euler_angle;
    s_scaled_imu_data_t scaled_imu_data;
    s_scaled_mag_data_t scaled_mag_data;
    bool attitude_invalid;
    bool position_invalid;
    u_unit_status_word_t unit_status_word;
    s_scaled_pos_data_t scaled_position;
    s_scaled_velocities_t scaled_inertial_vel;
    s_gps_ins_time_t gps_ins_time_val; /* nanoseconds from the beginning of the GPS reference week*/
    s_gnss_info_short_t gnss_info_short;
    s_number_of_satellites_t no_of_sat_used;
    s_scaled_baro_data_t scaled_baro_data;
    s_navigation_solution_status_t nav_soln_status;
    s_scaled_gdop_pdop_t scaled_gp_dop;
    s_scaled_body_frame_vel_t scaled_body_frm_vel;
    s_scaled_enu_lin_accl_t scaled_enu_lin_accl;
    s_kf_position_covariance_t scaled_pos_cov;
    s_kf_velocity_covariance_t scaled_vel_cov;
    s_scaled_roll_pit_cov_t scaled_roll_pit_cov;
    s_scaled_heading_cov_t scaled_heading_cov;
    u_new_gps_t new_gps_data;
    s_heading_correction_source_t corr_source;
    s_scaled_gnss_pos_std_t scaled_gnss_data;
    s_scaled_gnss_head_pitch_std_t gns_hd_pit_std;
    s_gnss_solution_status_t gnss_sol_status;
    s_scaled_insd_supply_voltage_t scaled_v_inp;
    s_scaled_insd_temperature_t scaled_temp;
    s_scaled_eph_epv_t scaled_h_v_pos_err;
    s_scaled_gnss_position_t scaled_gnss_pos;
    s_scaled_gnss_velocity_t scaled_gnss_vel;
    uint32_t GPS_ms;        /* milliseconds from the beginning of the GPS reference week */
    uint8_t Latency_ms_pos; /* latencies of time stamps in the GNSS receiver’s position */
    uint8_t Latency_ms_vel; /* latencies of time stamps in the GNSS receiver’s velocity */
    s_unscaled_pos_data_t unscaled_pos;

} s_ins_d_scaled_data_t;

/* Operation 'da_ins_init' of Class 'DA_ins_d' */
bool da_ins_il_init(void);

/* Operation 'da_ins_d_read_periodic' of Class 'DA_ins_d' */
void da_ins_il_read_periodic(void);

#endif /* H_DA_INS_IL */
