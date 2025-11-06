/****************************************************
 *  da_ins_il.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_ins_il
 *  Copyright: LODD (c) 2025
 ****************************************************/
#include "da_ins_il.h"
#include "generic_util.h"
#include "timer_interface.h"
#include <math.h>

#define D2R (0.0174532925199433)

#define HEADER_BYTE_1 (0xAA)
#define HEADER_BYTE_2 (0x55)
#define IO_DATA_INS_D (0x01)
#define INS_OPVT (0x52)
#define INS_UDD (0x95)

#define IMU_BUFFER_SIZE (200)

#define ATTITUDE_SCALING (0.01f)
#define ANGULAR_VEL_SCALING (0.02f)       /* 1.0/KG(= 50.0) */
#define ACCELERATION_SCALING (0.0049053f) /* => g_ref(=9.8106f) / KA(=2000.0f) */
#define MAGNETIC_FIELD_SCALING (10.0f)    /* Magnetic field resolution is 10 nT */
#define POSITION_SCALING (0.01f)          /* For Position - Altitude */
#define VELOCITY_SCALING (0.01f)          /* For Velocity - Altitude */
#define BARO_PRESSURE_SCALING (2.0f)      /* For Barometric - Pressure */
#define BARO_ALTITUDE_SCALING (0.01f)     /* For Barometric - Altitude */
#define SUPPLY_VOLTAGE_SCALING (0.01f)    /* For Supply Voltage - VDC */
#define TEMPERATURE_SCALING (0.1f)        /* For Temperature - DegC */
#define METER_SCALING (0.001f)            /* Conversion to meters */
#define GDOP_SCALING (0.001f)             /* GDOP scaling factor */
#define PDOP_SCALING (0.001f)             /* PDOP scaling factor */
#define POS_LAT_LONG_SCALING (1.0e7)      /* Scaling for Latitude and Longitude */
#define ALT_SCALING (0.01)                /* Altitude scaling */

#define INS_PERIODIC_TIMEOUT (60)

#define UDD_DATA_OFFSET (26) /* Start of relevant data payload in UDD mode */

/* Enumerated type to run the deframing state machine */
typedef enum
{
    STAGE1_HEADER0 = 0,
    STAGE2_HEADER1,
    STAGE3_MSG_TYPE,
    STAGE4_MSG_ID,
    STAGE5_MSG_LENGTH_LSB,
    STAGE6_MSG_LENGTH_MSB,
    STAGE7_MSG_PAYLOAD,
    STAGE8_MSG_CHECKSUM_VER

} ins_deframe_states_t;

/**
 * @brief Imu read buffer
 *
 */
static uint8_t INS_RxPacket[IMU_BUFFER_SIZE] = {0};

/**
 * @brief IMU message structure
 *
 */
static il_msg_s imu_msg;

static s_da_ins_opvt_t OpvtRawData;
static s_da_ins_udd_t UddRawData;

/**
 * @brief INS sensor abstraction structure
 *
 */
static s_ins_d_scaled_data_t InsProcessedData;

static s_timer_data_t InsMonitorTimer;
static bool InsCommTimeout;

/* private function prototypes */
static void da_ins_il_parse_data(const uint8_t *ptr_byte);
static void da_ins_il_process_data(void);
static void da_ins_il_decode(void);

static void da_ins_il_copy_orientation_data(s_orientation_angles_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_imu_data_filtered(s_imu_data_filtered_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_magnetometer_data(s_magnetometer_data_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_usw(u_unit_status_word_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_position_data(s_position_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_velocities_data(s_velocities_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gps_time(s_gps_ins_time_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_info(s_gnss_info_short_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_no_of_sat_used(s_number_of_satellites_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_baro_data(s_barometer_data_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_nav_solution_status(s_navigation_solution_status_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gdop_pdop_data(s_gdop_pdop_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_body_frame_velocity(s_body_frame_velocity_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_enu_lin_accel(s_enu_linear_acceleration_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_kf_pos_cov(s_kf_position_covariance_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_kf_vel_cov(s_kf_velocity_covariance_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_pitch_roll_cov(s_kf_pitch_roll_covariance_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_yaw_cov(s_kf_heading_covariance_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_new_gps_state(u_new_gps_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_yaw_cor_src(s_heading_correction_source_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_data_std(s_gnss_data_std_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_yaw_pitch_std(s_gnss_heading_pitch_std_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_sol_status(s_gnss_solution_status_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_supply_voltage(s_insd_supply_voltage_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_temperature(s_insd_temperature_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_position(s_gnss_position_t *ptr_data, const uint8_t *ptr_raw_data);
static void da_ins_il_copy_gnss_velocity(s_gnss_velocity_t *ptr_data, const uint8_t *ptr_raw_data);

static void da_ins_il_scale_attitude(s_orientation_angles_t *ptr_orientation);
static void da_ins_il_scale_imu_data(s_imu_data_filtered_t *ptr_imu_data);
static void da_ins_il_scale_magnetometer_data(s_magnetometer_data_t *ptr_mag_data);
static void da_ins_il_unscaled_position(s_position_t *ptr_position);
static void da_ins_il_scale_pos_data(s_position_t *ptr_pos_data);
static void da_ins_il_scale_inertial_vel(s_velocities_t *ptr_velocities);
static void da_ins_il_scale_bodyfr_vel(s_body_frame_velocity_t *ptr_velocities);
static void da_ins_il_scale_baro_data(s_barometer_data_t *ptr_baro_data);
static void da_ins_il_scale_gnss_pos_std(s_gnss_data_std_t *ptr_gns_pos_std);
static void da_ins_il_scale_gdop_pdop(s_gdop_pdop_t *ptr_gdop_pdop);
static void da_ins_il_inspect_att_invalid(void);
static void da_ins_il_inspect_position_invalid(void);

/**
 * @brief Retrieves the Euler angles (roll, pitch, yaw) from the INS system.
 *
 * This function extracts the roll, pitch, and yaw values from the INS system
 * if the IMU message checksum is verified as OK. The retrieved values are
 * stored in the provided pointers.
 *
 * @param[out] roll  Pointer to a float where the roll angle will be stored.
 * @param[out] pitch Pointer to a float where the pitch angle will be stored.
 * @param[out] yaw   Pointer to a float where the yaw angle will be stored.
 *
 * @return true if the Euler angles were successfully retrieved; false otherwise.
 */
bool da_get_ins_il_euler_angles(float *roll, float *pitch, float *yaw)
{

    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((roll != NULL) && (pitch != NULL) && (yaw != NULL))
    {
        *roll = InsProcessedData.scaled_euler_angle.roll;
        *pitch = InsProcessedData.scaled_euler_angle.pitch;
        *yaw = InsProcessedData.scaled_euler_angle.yaw;
        status = (false == InsProcessedData.attitude_invalid);
    }

    return status;
}

/**
 * @brief Retrieves the current angular velocity values from the INS processed data.
 *
 * This function copies the latest scaled gyroscope data (angular velocity) from the
 * INS processed data structure into the provided output pointers. It ensures that
 * the provided pointers are not NULL before attempting to write the values.
 *
 * @param[out] omg_x Pointer to a float where the X-axis angular velocity will be stored.
 * @param[out] omg_y Pointer to a float where the Y-axis angular velocity will be stored.
 * @param[out] omg_z Pointer to a float where the Z-axis angular velocity will be stored.
 *
 * @return true if the angular velocity values were successfully retrieved and stored;
 *         false if any of the provided pointers are NULL.
 */
bool da_get_ins_il_angular_velocity(float *omg_x, float *omg_y, float *omg_z)
{
    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((omg_x != NULL) && (omg_y != NULL) && (omg_z != NULL))
    {
        /* Updated values are copied */
        *omg_x = InsProcessedData.scaled_imu_data.gyro_x;
        *omg_y = InsProcessedData.scaled_imu_data.gyro_y;
        *omg_z = InsProcessedData.scaled_imu_data.gyro_z;
        status = ((0 == InsProcessedData.unit_status_word.s_status_bits.s_gyro_status) && (false == InsCommTimeout));
    }

    return status;
}

/**
 * @brief Retrieves the latest accelerometer data if the IMU message checksum is valid.
 *
 * This function checks the validity of the IMU message by verifying its checksum.
 * If the checksum is valid, it updates the provided pointers with the latest
 * accelerometer readings along the X, Y, and Z axes. If the checksum is invalid,
 * the function does not update the values and returns false.
 *
 * @param[out] accl_x Pointer to store the accelerometer X-axis value.
 * @param[out] accl_y Pointer to store the accelerometer Y-axis value.
 * @param[out] accl_z Pointer to store the accelerometer Z-axis value.
 *
 * @return true if the accelerometer data was updated successfully, false otherwise.
 */
bool da_get_ins_il_accelerometer_data(float *accl_x, float *accl_y, float *accl_z)
{
    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((accl_x != NULL) && (accl_y != NULL) && (accl_z != NULL))
    {
        /* Updated values are copied */
        *accl_x = InsProcessedData.scaled_imu_data.accel_x;
        *accl_y = InsProcessedData.scaled_imu_data.accel_y;
        *accl_z = InsProcessedData.scaled_imu_data.accel_z;
        status = ((0 == InsProcessedData.unit_status_word.s_status_bits.s_acc_status) && (false == InsCommTimeout));
    }
    return status;
}

/**
 * @brief Retrieves the latest latitude, longitude, and GPS altitude values.
 *
 * This function checks the validity of the IMU message using its checksum flag.
 * If the checksum is verified as OK, it updates the provided latitude, longitude,
 * and GPS altitude pointers with the latest values. Otherwise, it retains the
 * last known good values.
 *
 * @param[out] latitude Pointer to an integer where the latitude value will be stored.
 * @param[out] longitude Pointer to an integer where the longitude value will be stored.
 * @param[out] gps_altitude Pointer to a float where the GPS altitude value will be stored.
 *
 * @return true if the IMU message checksum is verified as OK and values are updated,
 *         false otherwise.
 */
bool da_get_ins_il_position(double *latitude, double *longitude, float *gps_altitude)
{
    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((latitude != NULL) && (longitude != NULL) && (gps_altitude != NULL))
    {
        /* Updated values are copied */
        *latitude = InsProcessedData.scaled_position.lat;
        *longitude = InsProcessedData.scaled_position.lon;
        *gps_altitude = InsProcessedData.scaled_position.alt;
        status = ((false == InsProcessedData.position_invalid) && (false == InsCommTimeout));
    }
    return status;
}

/**
 * @brief Retrieves the inertial velocity components (north, east, down).
 *
 * This function fetches the inertial velocity components (vN, vE, vD)
 * from the INS (Inertial Navigation System) if the IMU (Inertial Measurement Unit)
 * message has a valid checksum. If the checksum is invalid, the function
 * retains the last good values and does not update the velocity components.
 *
 * @param[out] vel_n Pointer to store the northward velocity component.
 * @param[out] vel_e Pointer to store the eastward velocity component.
 * @param[out] vel_d Pointer to store the downward velocity component.
 *
 * @return true if the velocity components were successfully updated,
 *         false otherwise.
 */
bool da_get_ins_il_inertial_velocity(float *vel_n, float *vel_e, float *vel_d)
{
    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((vel_n != NULL) && (vel_e != NULL) && (vel_d != NULL))
    {
        /* Updated values are copied */
        *vel_n = InsProcessedData.scaled_inertial_vel.vel_n;
        *vel_e = InsProcessedData.scaled_inertial_vel.vel_e;
        *vel_d = InsProcessedData.scaled_inertial_vel.vel_d;
        status = (false == InsCommTimeout);
    }

    return status;
}

/**
 * @brief Retrieves the body velocity components (x, y, z) from the INS system.
 *
 * This function fetches the velocity components of the body in the x, y, and z
 * directions. The values are updated only if the IMU message checksum is verified
 * as OK. If the checksum is not valid, the function retains the last known good
 * values and does not update the output parameters.
 *
 * @param[out] vel_x Pointer to store the velocity in the x direction.
 * @param[out] vel_y Pointer to store the velocity in the y direction.
 * @param[out] vel_z Pointer to store the velocity in the z direction.
 *
 * @return true if the velocity values were successfully updated, false otherwise.
 */
bool da_get_ins_il_body_velocity(float *vel_x, float *vel_y, float *vel_z)
{
    bool status = false;
    /* Check if the pointer passed is not NULL */
    if ((vel_x != NULL) && (vel_y != NULL) && (vel_z != NULL))
    {
        /* Updated values are copied */
        *vel_x = InsProcessedData.scaled_body_frm_vel.vel_x;
        *vel_y = InsProcessedData.scaled_body_frm_vel.vel_y;
        *vel_z = InsProcessedData.scaled_body_frm_vel.vel_z;
        status = (false == InsCommTimeout);
    }
    return status;
}

/**
 * @brief Retrieves the current supply voltage from the INS processed data.
 *
 * This function checks if the IMU message has a valid checksum. If the checksum
 * is valid, it updates the provided pointer with the latest supply voltage value.
 * If the checksum verification fails, the function does not update the value and
 * returns false, retaining the last known good value.
 *
 * @param[out] supply_voltage Pointer to a float variable where the supply voltage will be stored.
 *
 * @return true if the supply voltage was successfully updated; false otherwise.
 */
bool da_get_ins_il_supply_voltage(float *supply_voltage)
{
    bool status = false;
    /* Validate the message if Checksum is verified OK,
       otherwise retain the last good value */
    if (supply_voltage != NULL)
    {
        /* Updated values are copied */
        *supply_voltage = InsProcessedData.scaled_v_inp.inp_voltage;
        status = (false == InsCommTimeout);
    }
    return status;
}

/**
 * @brief Retrieves the latest valid INS temperature value.
 *
 * This function checks if the latest IMU message has a valid checksum.
 * If the checksum is valid, it updates the provided temperature pointer
 * with the most recent INS temperature value. If the checksum is invalid,
 * the function does not update the temperature value and returns false.
 *
 * @param[out] temperature Pointer to a float where the INS temperature will be stored if valid.
 * @return true if a valid temperature value was retrieved and stored, false otherwise.
 */
bool da_get_ins_il_temperature(float *temperature)
{
    bool status = false;
    /* Validate the message if Checksum is verified OK,
       otherwise retain the last good value */
    if (temperature != NULL)
    {
        /* Updated values are copied */
        *temperature = InsProcessedData.scaled_temp.insd_temperature;
        status = (false == InsCommTimeout);
    }
    return status;
}

/**
 * @brief Retrieves the current values of the estimated horizontal and vertical position errors.
 *
 * This function assigns the values of the estimated horizontal position error (eph)
 * and estimated vertical position error (epv) from the global InsProcessedData structure
 * to the memory locations pointed to by ptr_eph and ptr_epv, respectively.
 *
 * @param[out] ptr_eph Pointer to a double where the estimated horizontal position error will be stored.
 * @param[out] ptr_epv Pointer to a double where the estimated vertical position error will be stored.
 *
 * @return true Always returns true.
 *
 * @note If either pointer is NULL, the function does not write to that location.
 */
bool da_get_ins_il_eph_epv_data(float *ptr_eph, float *ptr_epv)
{
    bool status = false;
    if ((ptr_eph != NULL) && (ptr_epv != NULL))
    {
        *ptr_eph = InsProcessedData.scaled_h_v_pos_err.eph;
        *ptr_epv = InsProcessedData.scaled_h_v_pos_err.epv;
        status = (false == InsCommTimeout);
    }

    return status;
}

/**
 * @brief Retrieves the current GNSS Position Dilution of Precision (PDOP) value.
 *
 * This function assigns the current PDOP value from the INS processed data to the
 * memory location pointed to by the input parameter. It also checks if the INS
 * communication has not timed out before returning a success status.
 *
 * @param[out] pdop Pointer to a float variable where the PDOP value will be stored.
 *                  Must not be NULL.
 *
 * @return true if the PDOP value was successfully retrieved and the INS communication
 *         has not timed out; false otherwise.
 */
bool da_get_ins_il_gnss_dop(float *pdop)
{
    bool status = false;
    if (pdop != NULL)
    {
        *pdop = InsProcessedData.scaled_gp_dop.pdop;

        status = (false == InsCommTimeout);
    }

    return status;
}

bool da_get_ins_il_gnss_sat_used(uint8_t *gnss_sat_used)
{
    bool status = false;
    if (gnss_sat_used != NULL)
    {
        *gnss_sat_used = InsProcessedData.no_of_sat_used.num_satellites;

        status = InsCommTimeout;
    }

    return status;
}

bool da_get_ins_il_alt_baro(float *alt_baro)
{
    bool status = false;
    if (alt_baro != NULL)
    {
        *alt_baro = InsProcessedData.scaled_baro_data.baro_h;

        status = InsCommTimeout;
    }

    return status;
}

bool da_get_ins_il_ins_sol_status(uint8_t *ins_sol_status)
{
    bool status = false;
    if (ins_sol_status != NULL)
    {
        *ins_sol_status = InsProcessedData.nav_soln_status.status;

        status = InsCommTimeout;
    }

    return status;
}

/**
 * @brief Checks if the INS (Inertial Navigation System) attitude data is valid.
 *
 * This function returns the validity status of the attitude data processed by the INS.
 *
 * @return true if the INS attitude data is valid, false otherwise.
 */
bool da_get_ins_il_att_invalid(void)
{
    return InsProcessedData.attitude_invalid;
}

bool da_get_ins_il_gnss_sol_status(uint8_t *pos_type, uint8_t *sol_status)
{
    bool status = false;
    if (pos_type != NULL && sol_status != NULL)
    {
        *pos_type = InsProcessedData.gnss_info_short.gnss_info1.gnss_info1_bits.position_type;

        *sol_status = InsProcessedData.gnss_info_short.gnss_info2.gnss_info2_bits.solution_status;

        status = (false == InsCommTimeout);
    }

    return status;
}

bool da_get_ins_il_gnss_info(uint8_t *gnss_info1, uint8_t *gnss_info2)
{
    bool status = false;
    if (gnss_info1 != NULL && gnss_info2 != NULL)
    {
        *gnss_info1 = InsProcessedData.gnss_info_short.gnss_info1.raw_gnss_info1;
        *gnss_info2 = InsProcessedData.gnss_info_short.gnss_info2.raw_gnss_info2;

        status = (false == InsCommTimeout);
    }

    return status;
}

bool da_get_ins_il_kf_cov(uint8_t *pos_cov_lla, uint8_t *vel_cov_ned)
{
    bool status = false;
    if (pos_cov_lla != NULL && vel_cov_ned != NULL)
    {
        pos_cov_lla[0] = InsProcessedData.scaled_pos_cov.lat_std;
        pos_cov_lla[1] = InsProcessedData.scaled_pos_cov.lon_std;
        pos_cov_lla[2] = InsProcessedData.scaled_pos_cov.alt_std;
        vel_cov_ned[0] = InsProcessedData.scaled_vel_cov.vel_n_std;
        vel_cov_ned[1] = InsProcessedData.scaled_vel_cov.vel_e_std;
        vel_cov_ned[2] = InsProcessedData.scaled_vel_cov.vel_d_std;

        status = (false == InsCommTimeout);
    }

    return status;
}

/**
 * @brief Retrieves the current status of the InsCommTimeout flag.
 *
 * This function returns the value of the InsCommTimeout variable,
 * indicating whether a communication timeout has occurred for the
 * instrument.
 *
 * @return true if a communication timeout has occurred, false otherwise.
 */
bool da_get_ins_il_timeout(void)
{
    return InsCommTimeout;
}

/**
 * @brief Checks if the INS (Inertial Navigation System) OMG (Omega) status is invalid.
 *
 * This function returns the status of the INS gyro by accessing the
 * s_gyro_status bit within the s_status_bits structure of the unit_status_word
 * in the InsProcessedData global variable. It is used to determine if the
 * INS OMG is in an invalid state.
 *
 * @return true if the INS OMG status is invalid, false otherwise.
 */
bool da_get_ins_il_omg_invalid(void)
{
    return (0 != InsProcessedData.unit_status_word.s_status_bits.s_gyro_status);
}

/**
 * @brief Checks if the INS (Inertial Navigation System) acceleration data is invalid.
 *
 * This function returns the status of the INS acceleration validity by evaluating
 * the corresponding status bit in the InsProcessedData structure.
 *
 * @return true  If the INS acceleration data is invalid.
 * @return false If the INS acceleration data is valid.
 */
bool da_get_ins_il_accel_invalid(void)
{
    return (0 != InsProcessedData.unit_status_word.s_status_bits.s_acc_status);
}

/**
 * @brief Checks if the GNSS receiver status bit is set in the unit status word.
 *
 * This function returns the value of the GNSS receiver status bit from the
 * InsProcessedData structure. It is used to determine if the current
 * INS position is invalid due to GNSS receiver status.
 *
 * @return true if the GNSS receiver status bit is set (indicating an invalid INS position),
 *         false otherwise.
 */
bool da_get_ins_il_pos_invalid(void)
{
    return (InsProcessedData.position_invalid);
}

/**
 * @brief Retrieves the latest valid UDD (User Defined Data) from the IMU message.
 *
 * This function checks if the IMU message has a valid checksum. If the checksum is valid,
 * it copies the updated UDD raw data to the provided pointer and returns true. If the checksum
 * verification fails, the function does not update the data and returns false, retaining the last
 * known good value.
 *
 * @param[out] ptr_data Pointer to a s_da_ins_udd_t structure where the latest valid UDD data will be stored.
 *
 * @return true  If the UDD data was successfully updated with valid data.
 * @return false If the checksum verification failed and data was not updated.
 */
bool da_ins_get_udd_data(s_da_ins_udd_t *ptr_data)
{
    bool status = false;

    /* Validate the message if Checksum is verified OK,
       otherwise retain the last good value */
    if (imu_msg.flag != IL_DCODE_FAILED_CRC)
    {
        /* Updated values are copied */
        *ptr_data = UddRawData;
        status = true;
    }

    return status;
}

/**
 * @brief Initializes the INS (Inertial Navigation System) UART port.
 *
 * This function sets up the UART port for the INS module by calling the
 * `uart_init` function with the appropriate UART identifier. It returns
 * the initialization status indicating whether the operation was successful.
 *
 * @return true if the INS UART port was successfully initialized, false otherwise.
 */
bool da_ins_il_init(void)
{
    /* SyncableUserCode{2BE41689-8893-484a-90EB-C9334493186E}:Nbrlk8aPUZ */
    bool init_status = false;

    /* Initialize the INS UART Port */
    init_status = uart_init(UART_INS);
    util_memset(&OpvtRawData, 0, sizeof(s_da_ins_opvt_t));
    util_memset(&UddRawData, 0, sizeof(s_da_ins_udd_t));
    timer_start(&InsMonitorTimer, INS_PERIODIC_TIMEOUT);

    return init_status;
    /* SyncableUserCode{2BE41689-8893-484a-90EB-C9334493186E} */
}

/**
 * @brief Reads periodic data and processes it.
 *
 * This function is responsible for reading periodic data and invoking
 * the processing function `da_get_ins_il_process_data` to handle the data.
 *
 * @note This function contains user-defined syncable code blocks
 *       identified by unique tags for customization or extension.
 */
void da_ins_il_read_periodic(void)
{
    /* SyncableUserCode{E91E2C29-1A09-4965-94D4-139E4F7266D0}:Nbrlk8aPUZ */

    da_ins_il_process_data();

    /* SyncableUserCode{E91E2C29-1A09-4965-94D4-139E4F7266D0} */
}

/**
 * @brief Processes incoming data from the INS via UART.
 *
 * This function reads data from the INS through the UART interface and processes
 * it byte by byte. It uses a deframing logic to parse the data and checks for
 * valid CRC. If the CRC is valid, the data is decoded and processed further.
 *
 *
 * @details
 * - Reads data from the UART interface into a buffer.
 * - Iterates through the received data and parses it using `da_get_ins_il_parse_data`.
 * - If a valid message with correct CRC is detected, it decodes the message.
 *
 * @internal - Private function for the module.
 *
 */
static void da_ins_il_process_data(void)
{
    /* SyncableUserCode{F102D825-B00A-4f55-9B8C-46FC67A9FBFC}:1Li0sadfbu */
    uint16_t bytes_read;
    uint16_t byte_id;

    /* UART Read IMU Buffer */
    bytes_read = uart_read((uint8_t)UART_INS, &INS_RxPacket[0], IMU_BUFFER_SIZE);

    /* if bytes are read in the current cycle */
    if (bytes_read > 0)
    {
        /* Iterate through all the bytes read */
        for (byte_id = 0; byte_id < bytes_read; byte_id++)
        {
            /*Apply Deframing Logic */
            da_ins_il_parse_data(&INS_RxPacket[byte_id]);

            /* if checksum is verified successfully */
            if (imu_msg.flag == IL_DCODE_CRC_OK)
            {
                /* Decode the received data */
                da_ins_il_decode();
            }
        }
    }

    /* Check for timer expiry */
    InsCommTimeout = timer_check_expiry(&InsMonitorTimer);
    /* SyncableUserCode{F102D825-B00A-4f55-9B8C-46FC67A9FBFC} */
}

/**
 * @brief State machine chart for da_get_ins_il_parse_data
 *
 * The state machine transitions through the following states:
 *
 * STAGE1_HEADER0:
 *   - Waits for the first header byte (0xAA).
 *   - If matched, transitions to STAGE2_HEADER1.
 *   - Otherwise, remains in STAGE1_HEADER0.
 *
 * STAGE2_HEADER1:
 *   - Waits for the second header byte (0x55).
 *   - If matched, transitions to STAGE3_MSG_TYPE.
 *   - Otherwise, resets to STAGE1_HEADER0.
 *
 * STAGE3_MSG_TYPE:
 *   - Checks if the message type matches IO_DATA_INS_D (0x01).
 *   - If matched, transitions to STAGE4_MSG_ID.
 *   - Otherwise, resets to STAGE1_HEADER0.
 *
 * STAGE4_MSG_ID:
 *   - Validates the message ID (INS_OPVT, INS_OPVT2AW, INS_UDD).
 *   - If valid, transitions to STAGE5_MSG_LENGTH_LSB.
 *   - Otherwise, resets to STAGE1_HEADER0.
 *
 * STAGE5_MSG_LENGTH_LSB:
 *   - Reads the LSB of the message length.
 *   - Transitions to STAGE6_MSG_LENGTH_MSB.
 *
 * STAGE6_MSG_LENGTH_MSB:
 *   - Reads the MSB of the message length.
 *   - If the length is valid, transitions to STAGE7_MSG_PAYLOAD.
 *   - Otherwise, resets to STAGE1_HEADER0.
 *
 * STAGE7_MSG_PAYLOAD:
 *   - Reads the payload bytes.
 *   - If all payload bytes are read, transitions to STAGE8_MSG_CHECKSUM_LSB.
 *
 * STAGE8_MSG_CHECKSUM_LSB:
 *   - Reads the LSB of the checksum.
 *   - Transitions to STAGE9_MSG_CHECKSUM_MSB.
 *
 * STAGE9_MSG_CHECKSUM_MSB:
 *   - Reads the MSB of the checksum.
 *   - Validates the checksum.
 *   - Resets to STAGE1_HEADER0.
 *
 * @param ptr_byte pointer to single byte of received msg element
 *
 * @internal - Private function for the module.
 */
/* Operation 'da_get_ins_il_parse_data' of Class 'DA_ins_d' */
static void da_ins_il_parse_data(const uint8_t *ptr_byte)
{
    /* SyncableUserCode{A4BAA65C-736D-4612-886A-8E2B615C0D8B}:Nbrlk8aPUZ */
    static ins_deframe_states_t state = STAGE1_HEADER0;
    static uint16_t msg_len = 0;
    static uint16_t checksum = 0;
    static uint16_t inbound_chksum = 0;
    static uint8_t payload_len = 0;
    static uint8_t payload_index = 0;
    bool valid_msg_id = false;

    // printf(" Byte = 0x%02X\r\n", *ptr_byte);

    if (ptr_byte != NULL)
    {

        switch (state)
        {
        case STAGE1_HEADER0:
            if (HEADER_BYTE_1 == *ptr_byte)
            {
                /* Proceed to the verify the next header in the frame */
                state = STAGE2_HEADER1;
                /* Mark the beginning of decoding new packet */
                imu_msg.flag = IL_DCODE_PENDING;
                /* Reset all the intermediate state machine variables */
                msg_len = 0;
                payload_len = 0;
                payload_index = 0;
                checksum = 0;
                inbound_chksum = 0;
            }
            break;

        case STAGE2_HEADER1:
            if (HEADER_BYTE_2 == *ptr_byte)
            {
                /* Proceed to the verify the message type */
                state = STAGE3_MSG_TYPE;
            }
            else
            {
                /* If header 2 is not verified, packet failed return to verify new packet */
                state = STAGE1_HEADER0;
            }
            break;

        case STAGE3_MSG_TYPE:
            if (IO_DATA_INS_D == *ptr_byte)
            {
                /* Proceed to the verify the message ID */
                state = STAGE4_MSG_ID;
                /* Accumulate the checksum value */
                checksum = checksum + (uint16_t)*ptr_byte;
            }
            else
            {
                /* If message type is command, not intended to decode, return to verify new packet */
                state = STAGE1_HEADER0;
            }
            break;

        case STAGE4_MSG_ID:
            /* Check the message structure type */
            if (INS_OPVT == *ptr_byte)
            {
                /* Indicate a valid message ID */
                valid_msg_id = true;
                /* set decoding logic to decode OPVT stream */
                imu_msg.id = IL_DCODE_OPVT;
            }
            else if (INS_UDD == *ptr_byte)
            {
                /* Indicate a valid message ID */
                valid_msg_id = true;
                /* set decoding logic to decode OPVT stream */
                imu_msg.id = IL_DCODE_UDD;
            }
            else
            {
                /* return to decoding next stream as
                    no matching messsage structure found */
                state = STAGE1_HEADER0;
            }

            /* If the message ID is valid */
            if (valid_msg_id == true)
            {
                /* move to the next state */
                state = STAGE5_MSG_LENGTH_LSB;
                /* Accumulate the checksum value */
                checksum = checksum + (uint16_t)*ptr_byte;
            }
            break;

        case STAGE5_MSG_LENGTH_LSB:
            /* copy the incoming data byte to message length */
            msg_len = *ptr_byte;
            /* Accumulate the checksum */
            checksum = checksum + (uint16_t)*ptr_byte;
            /* Proceed to the next state */
            state = STAGE6_MSG_LENGTH_MSB;
            break;

        case STAGE6_MSG_LENGTH_MSB:
            /* copy the incoming data byte to message length */
            msg_len = (msg_len | (uint16_t)(*ptr_byte << 8));
            /* Check if the message length is valid */
            if (msg_len > INS_OVERHEAD_LEN)
            {
                /* If the message length is greater than 0 */
                state = STAGE7_MSG_PAYLOAD;
                /* Calculate the payload length */
                payload_len = msg_len - INS_OVERHEAD_LEN;
                /* Accumulate the checksum */
                checksum = checksum + (uint16_t)*ptr_byte;
            }
            else
            {
                /* Proceed to the next state */
                state = STAGE1_HEADER0;
            }
            break;

        case STAGE7_MSG_PAYLOAD:
            /* Check if the payload index is less than payload length */
            if (payload_index < payload_len)
            {
                /* copy the incoming data byte to message length */
                imu_msg.data[payload_index] = *ptr_byte;
                /* Accumulate the checksum */
                checksum = checksum + (uint16_t)*ptr_byte;
                /* Increment the payload index */
                payload_index++;
            }
            else
            {
                /* copy the incoming LSB checksum */
                inbound_chksum = (uint16_t)*ptr_byte;
                /* Proceed to the next state */
                state = STAGE8_MSG_CHECKSUM_VER;
            }
            break;

        case STAGE8_MSG_CHECKSUM_VER:
            /* copy the incoming MSB checksum */
            inbound_chksum = (inbound_chksum | (uint16_t)(*ptr_byte << 8));

            /* Check if the computed checksum is equal to the received checksum */
            if (checksum == inbound_chksum)
            {
                /* Set the message flag to indicate success */
                imu_msg.flag = IL_DCODE_CRC_OK;
                /* Reload timer upon successful deframing */
                timer_reload(&InsMonitorTimer);
            }
            else
            {
                /* Set the message flag to indicate failure */
                imu_msg.flag = IL_DCODE_FAILED_CRC;
            }
            /* Proceed to the first state in any case */
            state = STAGE1_HEADER0;
            break;

        default:
            /* If the state is not valid, check to see if it is equal to HEADER_BYTE-1 (0xAA) */
            if (HEADER_BYTE_1 == *ptr_byte)
            {
                /* Proceed to the verify the next header in the frame */
                state = STAGE2_HEADER1;
                /* Mark the beginning of decoding new packet */
                imu_msg.flag = IL_DCODE_PENDING;
                /* Reset all the intermediate state machine variables */
                msg_len = 0;
                payload_len = 0;
                payload_index = 0;
                checksum = 0;
                inbound_chksum = 0;
            }
            else
            {
                /* If the header 1 is not verified, packet failed return to verify new packet */
                state = STAGE1_HEADER0;
            }
            break;
        }
    }

    /* SyncableUserCode{A4BAA65C-736D-4612-886A-8E2B615C0D8B} */
}

/**
 * @brief Decodes the IMU message and updates the INS data structure.
 *
 * This function processes the IMU message based on its type and extracts
 * relevant data such as attitude, angular velocity, acceleration, magnetometer
 * readings, position, velocity, barometric pressure, and altitude. The extracted
 * data is scaled appropriately and stored in the INS data structure.
 *
 * @details
 * - Supports decoding of OPVT message type.
 * - Applies scaling factors to raw data for meaningful interpretation.
 * - Updates the INS data structure with decoded values.
 *
 * @internal - Private function for the module.
 */
static void da_ins_il_decode(void)
{
    if (imu_msg.id == IL_DCODE_OPVT)
    {
        /* ----------------------------------- Extraction ------------------------------ */

        /* NOTE: To ensure proper alignment and avoid issues
         * with struct padding, copy each field individually */

        /* Copy raw Euler Angles data from buffer */
        da_ins_il_copy_orientation_data(&OpvtRawData.orientation_angles, &imu_msg.data[0]);
        /* Copy raw IMU filtered data from buffer */
        da_ins_il_copy_imu_data_filtered(&OpvtRawData.imu_data_filtered, &imu_msg.data[6]);
        /* Copy Magnetometer Data from buffer */
        da_ins_il_copy_magnetometer_data(&OpvtRawData.magnetometer_data, &imu_msg.data[18]);
        /* Copy Unit Status Word from buffer */
        da_ins_il_copy_usw(&OpvtRawData.unit_status_word, &imu_msg.data[24]);
        /* Copy input reference voltage from buffer */
        da_ins_il_copy_supply_voltage(&OpvtRawData.v_inp, &imu_msg.data[26]);
        /* Copy temperature from buffer */
        da_ins_il_copy_temperature(&OpvtRawData.temp, &imu_msg.data[28]);
        /* Copy Position Data from buffer */
        da_ins_il_copy_position_data(&OpvtRawData.position, &imu_msg.data[30]);
        /* Copy Velocity data from buffer */
        da_ins_il_copy_velocities_data(&OpvtRawData.velocities, &imu_msg.data[42]);
        /* Copy GNSS Position data from buffer */
        da_ins_il_copy_gnss_position(&OpvtRawData.gnss_position, &imu_msg.data[54]);
        /* Copy GNSS Velocity data from buffer */
        da_ins_il_copy_gnss_velocity(&OpvtRawData.gnss_velocity, &imu_msg.data[66]);
        /* Copy raw GPS_ms data from buffer */
        OpvtRawData.GPS_ms = *((uint32_t *)&imu_msg.data[76]);
        /* Copy raw GNSS_info data from buffer */
        da_ins_il_copy_gnss_info(&OpvtRawData.gnss_info_short, &imu_msg.data[80]);
        /*  Copy number of satellites used data from buffer */
        da_ins_il_copy_no_of_sat_used(&OpvtRawData.no_of_sat_used, &imu_msg.data[82]);
        /* Copy Latency_ms_pos information from buffer */
        OpvtRawData.Latency_ms_pos = *((uint8_t *)&imu_msg.data[83]);
        /* Copy Latency_ms_vel information from buffer */
        OpvtRawData.Latency_ms_vel = *((uint8_t *)&imu_msg.data[84]);
        /* Copy Barometer Data from buffer */
        da_ins_il_copy_baro_data(&OpvtRawData.barometer_data, &imu_msg.data[85]);
        /* Copy NEW GPS Data from buffer */
        da_ins_il_copy_new_gps_state(&OpvtRawData.new_gps_data, &imu_msg.data[91]);

        /* ----------------------------------- Scaling --------------------------------- */

        /* Attitude Scaling */
        da_ins_il_scale_attitude(&OpvtRawData.orientation_angles);
        /* IMU Filtered Data Scaling */
        da_ins_il_scale_imu_data(&OpvtRawData.imu_data_filtered);
        /* Magnetometer Scaling */
        da_ins_il_scale_magnetometer_data(&OpvtRawData.magnetometer_data);
        /* Position Scaling */
        da_ins_il_unscaled_position(&OpvtRawData.position);
        /* scale position data */
        da_ins_il_scale_pos_data(&OpvtRawData.position);
        /* Velocity Scaling */
        da_ins_il_scale_inertial_vel(&OpvtRawData.velocities);
        /* Barometric Data */
        da_ins_il_scale_baro_data(&OpvtRawData.barometer_data);
        /* Input Reference Voltage */
        InsProcessedData.scaled_v_inp.inp_voltage = (float)(OpvtRawData.v_inp.supply_voltage * SUPPLY_VOLTAGE_SCALING);
        /* Temperature */
        InsProcessedData.scaled_temp.insd_temperature = (float)(OpvtRawData.temp.temperature * TEMPERATURE_SCALING);
        /* INS GPS milliseconds from the beginning of the GPS reference week */
        InsProcessedData.GPS_ms = (uint32_t)OpvtRawData.GPS_ms;
        /* Copy number of satellites used */
        InsProcessedData.no_of_sat_used = UddRawData.no_of_sat_used;
        /* Update the Attitude Validity */
        da_ins_il_inspect_att_invalid();
        /* Update the Position Validity */
        da_ins_il_inspect_position_invalid();
    }
    else if (imu_msg.id == IL_DCODE_OPVT2AW)
    {
        /* Decode OPVT2AW message type  - To be implemented when it's necessary */
    }
    else if (imu_msg.id == IL_DCODE_UDD)
    {

        /* ----------------------------------- Extraction ------------------------------ */

        /* Table 6.29. Payload of the â€œUser Defined Dataâ€� */
        /*
        Field           Offset in payload,      Size,            Value
                             bytes               bytes

        Pckg Number            0                    1         Number of data packages
                                                             present in the payload (N)

        Data List              1                    N       List of data types,
                                                            one per byte (refer Table 6.30)
        Data #1
        ..
        ..
        Data #N
        */

        /* !!! NOTE: As per the table above the Offset of Data #1 starts at 26
        (We have chosen 25 User Defined Data ). Any Change in the UDD must strictly be
        configuration controlled and communicated through PRs */

        /* Copy raw Euler Angles data from buffer */
        da_ins_il_copy_orientation_data(&UddRawData.orientation_angles, &imu_msg.data[UDD_DATA_OFFSET + 0]);
        /* Copy raw IMU filtered data from buffer */
        da_ins_il_copy_imu_data_filtered(&UddRawData.imu_data_filtered, &imu_msg.data[UDD_DATA_OFFSET + 6]);
        /* Copy Magnetometer Data from buffer */
        da_ins_il_copy_magnetometer_data(&UddRawData.magnetometer_data, &imu_msg.data[UDD_DATA_OFFSET + 18]);
        /* Copy Unit Status Word from buffer */
        da_ins_il_copy_usw(&UddRawData.unit_status_word, &imu_msg.data[UDD_DATA_OFFSET + 24]);
        /* Copy Position Data from buffer */
        da_ins_il_copy_position_data(&UddRawData.position, &imu_msg.data[UDD_DATA_OFFSET + 26]);
        /* Copy Velocity data from buffer */
        da_ins_il_copy_velocities_data(&UddRawData.velocities, &imu_msg.data[UDD_DATA_OFFSET + 38]);
        /* Copy raw GP INS Time data from buffer */
        da_ins_il_copy_gps_time(&UddRawData.gps_ins_time_val, &imu_msg.data[UDD_DATA_OFFSET + 50]);
        /* Copy raw GNSS_info data from buffer */
        da_ins_il_copy_gnss_info(&UddRawData.gnss_info_short, &imu_msg.data[UDD_DATA_OFFSET + 58]);
        /*  Copy number of satellites used data from buffer */
        da_ins_il_copy_no_of_sat_used(&UddRawData.no_of_sat_used, &imu_msg.data[UDD_DATA_OFFSET + 60]);
        /* Copy Barometer Data from buffer */
        da_ins_il_copy_baro_data(&UddRawData.barometer_data, &imu_msg.data[UDD_DATA_OFFSET + 61]);
        /* Copy navigation solution status from buffer */
        da_ins_il_copy_nav_solution_status(&UddRawData.navigation_sol_status, &imu_msg.data[UDD_DATA_OFFSET + 67]);
        /* Copy Dilution of Precsion Data from buffer */
        da_ins_il_copy_gdop_pdop_data(&UddRawData.gdop_pdop, &imu_msg.data[UDD_DATA_OFFSET + 68]);
        /* Copy Body Frame Velocity Data from buffer */
        da_ins_il_copy_body_frame_velocity(&UddRawData.body_frame_velocity, &imu_msg.data[UDD_DATA_OFFSET + 72]);
        /* Copy ENU linear acceleration Data from buffer */
        da_ins_il_copy_enu_lin_accel(&UddRawData.enu_linear_acceleration, &imu_msg.data[UDD_DATA_OFFSET + 84]);
        /* Copy KF position covariance Data from buffer */
        da_ins_il_copy_kf_pos_cov(&UddRawData.kf_position_covariance, &imu_msg.data[UDD_DATA_OFFSET + 96]);
        /* Copy KF velocity covariance Data from buffer */
        da_ins_il_copy_kf_vel_cov(&UddRawData.kf_velocity_covariance, &imu_msg.data[UDD_DATA_OFFSET + 99]);
        /* Copy kf_pitch_roll_covariance Data from buffer */
        da_ins_il_copy_pitch_roll_cov(&UddRawData.kf_pitch_roll_covariance, &imu_msg.data[UDD_DATA_OFFSET + 102]);
        /* Copy kf_heading_covariance Data from buffer */
        da_ins_il_copy_yaw_cov(&UddRawData.kf_heading_covariance, &imu_msg.data[UDD_DATA_OFFSET + 106]);
        /* Copy new_gps_data Data from buffer */
        da_ins_il_copy_new_gps_state(&UddRawData.new_gps_data, &imu_msg.data[UDD_DATA_OFFSET + 107]);
        /* Copy heading_correction_source Data from buffer */
        da_ins_il_copy_yaw_cor_src(&UddRawData.heading_correction_source, &imu_msg.data[UDD_DATA_OFFSET + 108]);
        /* Copy gnss_data_std Data from buffer */
        da_ins_il_copy_gnss_data_std(&UddRawData.gnss_data_std, &imu_msg.data[UDD_DATA_OFFSET + 109]);
        /* Copy gnss_heading_pitch_std Data from buffer */
        da_ins_il_copy_gnss_yaw_pitch_std(&UddRawData.gnss_heading_pitch_std, &imu_msg.data[UDD_DATA_OFFSET + 115]);
        /* Copy gnss_sol_status Data from buffer */
        da_ins_il_copy_gnss_sol_status(&UddRawData.gnss_sol_status, &imu_msg.data[UDD_DATA_OFFSET + 119]);
        /* Copy input reference voltage from buffer */
        da_ins_il_copy_supply_voltage(&UddRawData.v_inp, &imu_msg.data[UDD_DATA_OFFSET + 120]);
        /* Copy temperature from buffer */
        da_ins_il_copy_temperature(&UddRawData.temp, &imu_msg.data[UDD_DATA_OFFSET + 122]);

        /* ----------------------------------- Scaling --------------------------------- */
        /* Attitude Scaling */
        da_ins_il_scale_attitude(&UddRawData.orientation_angles);
        /* IMU Filtered Data Scaling */
        da_ins_il_scale_imu_data(&UddRawData.imu_data_filtered);
        /* Magnetometer Scaling */
        da_ins_il_scale_magnetometer_data(&UddRawData.magnetometer_data);
        /* Position Scaling */
        da_ins_il_unscaled_position(&UddRawData.position);
        /* scale position data */
        da_ins_il_scale_pos_data(&UddRawData.position);
        /* Inertial Velocity Scaling */
        da_ins_il_scale_inertial_vel(&UddRawData.velocities);
        /* Body Frame Velocity Scaling */
        da_ins_il_scale_bodyfr_vel(&UddRawData.body_frame_velocity);
        /* Barometric Pressure */
        da_ins_il_scale_baro_data(&UddRawData.barometer_data);
        /* Position Standard Deviation */
        da_ins_il_scale_gnss_pos_std(&UddRawData.gnss_data_std);
        /* Scale Geometric and Position Dilution of precision */
        da_ins_il_scale_gdop_pdop(&UddRawData.gdop_pdop);
        /* Copy Attitude Invalid */
        da_ins_il_inspect_att_invalid();
        /* Copy Position Invalid */
        da_ins_il_inspect_position_invalid();
        /* Copy KF Position Covariance */
        InsProcessedData.scaled_pos_cov = UddRawData.kf_position_covariance;
        /* Copy KF Velocity Covariance */
        InsProcessedData.scaled_vel_cov = UddRawData.kf_velocity_covariance;
        /* Input Reference Voltage */
        InsProcessedData.scaled_v_inp.inp_voltage = (float)(UddRawData.v_inp.supply_voltage * SUPPLY_VOLTAGE_SCALING);
        /* Temperature */
        InsProcessedData.scaled_temp.insd_temperature = (float)(UddRawData.temp.temperature * TEMPERATURE_SCALING);
        /* Copy USW */
        InsProcessedData.unit_status_word.s_raw_usw = UddRawData.unit_status_word.s_raw_usw;
        /* Copy GPS Time */
        InsProcessedData.gps_ins_time_val.gps_ins_time = UddRawData.gps_ins_time_val.gps_ins_time;
        /* Copy number of satellites used */
        InsProcessedData.no_of_sat_used = UddRawData.no_of_sat_used;
        /* Copy INS solution status data */
        InsProcessedData.nav_soln_status.status = UddRawData.navigation_sol_status.status;
        /* Copy all GNSS related status info */
        InsProcessedData.gnss_info_short.gnss_info1.raw_gnss_info1 = UddRawData.gnss_info_short.gnss_info1.raw_gnss_info1;
        InsProcessedData.gnss_info_short.gnss_info2.raw_gnss_info2 = UddRawData.gnss_info_short.gnss_info2.raw_gnss_info2;
        InsProcessedData.gnss_sol_status.gnss_solution_status = UddRawData.gnss_sol_status.gnss_solution_status;
    }
    else
    {
        /* Invalid message type, do nothing */
    }
}

/**
 * @brief Copies raw orientation data into a structured orientation angles object.
 *
 * This function extracts yaw, pitch, and roll values from a raw data buffer
 * and assigns them to the corresponding fields in the provided orientation
 * angles structure. The yaw value is interpreted as an unsigned 16-bit integer,
 * while pitch and roll are interpreted as signed 16-bit integers.
 *
 * @param[out] ptr_data      Pointer to the s_orientation_angles_t structure where the orientation data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing orientation values.
 *                           The buffer must be at least 6 bytes long:
 *                             - Bytes 0-1: yaw (uint16_t)
 *                             - Bytes 2-3: pitch (int16_t)
 *                             - Bytes 4-5: roll (int16_t)
 *
 * @note The function performs pointer validity checks before copying data.
 */
static void da_ins_il_copy_orientation_data(s_orientation_angles_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw orientation data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->yaw = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->pitch = *((int16_t *)&ptr_raw_data[2]);
        ptr_data->roll = *((int16_t *)&ptr_raw_data[4]);
    }
}

/**
 * @brief Copies filtered IMU data from a raw data buffer to a structured data type.
 *
 * This function extracts gyroscope and accelerometer values from a raw byte buffer
 * and assigns them to the corresponding fields in the provided s_imu_data_filtered_t structure.
 * The raw data is expected to be in the following order (each as int16_t, little-endian):
 *   - Gyroscope X, Y, Z (6 bytes)
 *   - Accelerometer X, Y, Z (6 bytes)
 *
 * @param[out] ptr_data      Pointer to the s_imu_data_filtered_t structure to populate.
 * @param[in]  ptr_raw_data  Pointer to the raw IMU data buffer (must be at least 12 bytes).
 */
static void da_ins_il_copy_imu_data_filtered(s_imu_data_filtered_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw IMU data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gyro_x = *((int16_t *)&ptr_raw_data[0]);
        ptr_data->gyro_y = *((int16_t *)&ptr_raw_data[2]);
        ptr_data->gyro_z = *((int16_t *)&ptr_raw_data[4]);
        ptr_data->accel_x = *((int16_t *)&ptr_raw_data[6]);
        ptr_data->accel_y = *((int16_t *)&ptr_raw_data[8]);
        ptr_data->accel_z = *((int16_t *)&ptr_raw_data[10]);
    }
}

/**
 * @brief Copies raw magnetometer data into a structured data type.
 *
 * This function extracts 16-bit signed integer values representing the X, Y, and Z
 * axes of magnetometer data from a raw byte array and stores them in the provided
 * s_magnetometer_data_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the magnetometer data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data array containing magnetometer readings.
 *                           The array must contain at least 6 bytes, with each axis represented
 *                           as a 16-bit signed integer in little-endian format.
 *
 * @note The function performs null pointer checks on both input pointers.
 */
static void da_ins_il_copy_magnetometer_data(s_magnetometer_data_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw magnetometer data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->mag_x = *((int16_t *)&ptr_raw_data[0]);
        ptr_data->mag_y = *((int16_t *)&ptr_raw_data[2]);
        ptr_data->mag_z = *((int16_t *)&ptr_raw_data[4]);
    }
}

/**
 * @brief Copies raw unit status word data into a u_unit_status_word_t structure.
 *
 * This function copies the raw unit status word data from the provided pointer
 * to the specified u_unit_status_word_t structure. It performs null pointer checks
 * before copying the data.
 *
 * @param[out] ptr_data      Pointer to the destination u_unit_status_word_t structure.
 * @param[in]  ptr_raw_data  Pointer to the source raw unit status word data (uint8_t array).
 */
static void da_ins_il_copy_usw(u_unit_status_word_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw unit status word data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->s_raw_usw = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw position data into a position structure.
 *
 * This function extracts latitude, longitude, and altitude values from a raw data buffer
 * and assigns them to the corresponding fields in the provided position structure.
 *
 * @param[out] ptr_data      Pointer to the position structure to be filled.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing position information.
 *                           The buffer is expected to contain three consecutive int32_t values:
 *                           latitude (bytes 0-3), longitude (bytes 4-7), and altitude (bytes 8-11).
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_position_data(s_position_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw position data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->latitude = *((int32_t *)&ptr_raw_data[0]);
        ptr_data->longitude = *((int32_t *)&ptr_raw_data[4]);
        ptr_data->altitude = *((int32_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies raw velocity data into a s_velocities_t structure.
 *
 * This function extracts velocity components (east, north, vertical) from a raw data buffer
 * and assigns them to the corresponding fields in the provided s_velocities_t structure.
 * The raw data buffer is expected to contain three consecutive 32-bit signed integers,
 * representing east, north, and vertical speeds, respectively.
 *
 * @param[out] ptr_data      Pointer to the s_velocities_t structure to receive the velocity data.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing velocity values.
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_velocities_data(s_velocities_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw velocity data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->east_speed = *((int32_t *)&ptr_raw_data[0]);
        ptr_data->north_speed = *((int32_t *)&ptr_raw_data[4]);
        ptr_data->vertical_speed = *((int32_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies raw GPS time data into a s_gps_ins_time_t structure.
 *
 * This function takes a pointer to a s_gps_ins_time_t structure and a pointer to raw GPS time data,
 * and copies the 64-bit GPS time value from the raw data into the structure.
 *
 * @param[out] ptr_data      Pointer to the s_gps_ins_time_t structure to receive the GPS time.
 * @param[in]  ptr_raw_data  Pointer to the raw GPS time data (must be at least 8 bytes).
 *
 * @note Both pointers must be non-NULL. The function does not perform bounds checking on ptr_raw_data.
 */
static void da_ins_il_copy_gps_time(s_gps_ins_time_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GPS time data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gps_ins_time = *((uint64_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw GNSS information from a byte array to a GNSS info structure.
 *
 * This function takes a pointer to a destination structure of type s_gnss_info_short_t
 * and a pointer to a source array of raw GNSS data. It copies the first two bytes
 * from the raw data array into the corresponding fields of the destination structure.
 *
 * @param[out] ptr_data      Pointer to the destination s_gnss_info_short_t structure.
 * @param[in]  ptr_raw_data  Pointer to the source array containing raw GNSS data (at least 2 bytes).
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_gnss_info(s_gnss_info_short_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GNSS info data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gnss_info1.raw_gnss_info1 = *((uint8_t *)&ptr_raw_data[0]);
        ptr_data->gnss_info2.raw_gnss_info2 = *((uint8_t *)&ptr_raw_data[1]);
    }
}

/**
 * @brief Copies the number of satellites used from raw data to the provided structure.
 *
 * This function extracts the number of satellites used from the given raw data buffer
 * and assigns it to the num_satellites field of the s_number_of_satellites_t structure.
 *
 * @param[out] ptr_data      Pointer to the s_number_of_satellites_t structure where the number of satellites will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the number of satellites used (expected at index 0).
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_no_of_sat_used(s_number_of_satellites_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw number of satellites used data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->num_satellites = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw barometric sensor data into a structured data type.
 *
 * This function extracts barometric pressure and altitude values from a raw data buffer
 * and stores them in the provided s_barometer_data_t structure. The raw data buffer is
 * expected to contain the pressure as a 16-bit unsigned integer at offset 0, and the
 * barometric altitude as a 32-bit signed integer at offset 2.
 *
 * @param[out] ptr_data      Pointer to the s_barometer_data_t structure to store the parsed data.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing barometric sensor data.
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_baro_data(s_barometer_data_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw barometric data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->pressure = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->baro_altitude = *((int32_t *)&ptr_raw_data[2]);
    }
}

/**
 * @brief Copies the raw navigation solution status data to the provided structure.
 *
 * This function extracts the navigation solution status from the raw data buffer
 * and assigns it to the status field of the s_navigation_solution_status_t structure.
 *
 * @param[out] ptr_data      Pointer to the navigation solution status structure to be updated.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the status value.
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_nav_solution_status(s_navigation_solution_status_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw navigation solution status data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->status = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies GDOP and PDOP data from a raw byte array to a structured data pointer.
 *
 * This function extracts GDOP and PDOP values from the provided raw data buffer
 * and assigns them to the corresponding fields in the s_gdop_pdop_t structure.
 * The raw data is expected to contain the GDOP value at offset 0 and the PDOP value at offset 2,
 * both stored as 16-bit unsigned integers.
 *
 * @param[out] ptr_data      Pointer to the s_gdop_pdop_t structure where the extracted data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing GDOP and PDOP values.
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_gdop_pdop_data(s_gdop_pdop_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw navigation solution status data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gdop = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->pdop = *((uint16_t *)&ptr_raw_data[2]);
    }
}

/**
 * @brief Copies raw body frame velocity data into a structured format.
 *
 * This function extracts velocity components (Y, X, Z) from a raw data buffer
 * and assigns them to the corresponding fields in the provided
 * s_body_frame_velocity_t structure. The raw data is expected to contain
 * three consecutive 32-bit signed integers representing the velocities.
 *
 * @param[out] ptr_data      Pointer to the s_body_frame_velocity_t structure where the velocity data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing velocity values (at least 12 bytes).
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_body_frame_velocity(s_body_frame_velocity_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw body frame velocity data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->velocity_y = *((int32_t *)&ptr_raw_data[0]);
        ptr_data->velocity_x = *((int32_t *)&ptr_raw_data[4]);
        ptr_data->velocity_z = *((int32_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies raw ENU (East-North-Up) linear acceleration data into a structured format.
 *
 * This function extracts three 32-bit signed integer values representing the east, north,
 * and vertical components of linear acceleration from a raw byte array and stores them
 * in the provided s_enu_linear_acceleration_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the parsed acceleration data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw byte array containing ENU linear acceleration data.
 *                           The array must contain at least 12 bytes (3 x int32_t).
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_enu_lin_accel(s_enu_linear_acceleration_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw ENU linear acceleration data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->east_accel = *((int32_t *)&ptr_raw_data[0]);
        ptr_data->north_accel = *((int32_t *)&ptr_raw_data[4]);
        ptr_data->vertical_accel = *((int32_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies Kalman filter position covariance data from raw input to a structured format.
 *
 * This function extracts the latitude, longitude, and altitude standard deviation values
 * from the provided raw data buffer and assigns them to the corresponding fields in the
 * s_kf_position_covariance_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the extracted covariance data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing Kalman filter position covariance values.
 *
 * @note The function expects the raw data buffer to be at least 12 bytes long, with each
 *       standard deviation value located at offsets 0, 4, and 8 respectively.
 * @note No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_kf_pos_cov(s_kf_position_covariance_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw Kalman filter position covariance data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->lat_std = *((uint8_t *)&ptr_raw_data[0]);
        ptr_data->lon_std = *((uint8_t *)&ptr_raw_data[4]);
        ptr_data->alt_std = *((uint8_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies Kalman filter velocity covariance data from a raw data buffer to a structured data pointer.
 *
 * This function extracts the North, East, and Down velocity standard deviations from the provided
 * raw data buffer and assigns them to the corresponding fields in the s_kf_velocity_covariance_t structure.
 *
 * @param[out] ptr_data      Pointer to the s_kf_velocity_covariance_t structure to be populated.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing velocity covariance data.
 *
 * @note The function assumes that the raw data buffer contains the required data at offsets 0, 4, and 8.
 *       No bounds checking is performed on the buffer.
 */
static void da_ins_il_copy_kf_vel_cov(s_kf_velocity_covariance_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw Kalman filter velocity covariance data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->vel_n_std = *((uint8_t *)&ptr_raw_data[0]);
        ptr_data->vel_e_std = *((uint8_t *)&ptr_raw_data[4]);
        ptr_data->vel_d_std = *((uint8_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies Kalman filter pitch and roll covariance data from raw input to the provided structure.
 *
 * This function extracts the pitch and roll standard deviation values from the given raw data buffer
 * and assigns them to the corresponding fields in the s_kf_pitch_roll_covariance_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the extracted pitch and roll covariance data will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing Kalman filter pitch and roll covariance values.
 *
 * @note The function expects that ptr_raw_data points to at least 6 bytes of valid data.
 *       The pitch_std is read from bytes 0-1, and roll_std is read from bytes 4-5.
 *       Both pointers must be non-NULL for the operation to proceed.
 */
static void da_ins_il_copy_pitch_roll_cov(s_kf_pitch_roll_covariance_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw Kalman filter pitch and roll covariance data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->pitch_std = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->roll_std = *((uint16_t *)&ptr_raw_data[4]);
    }
}

/**
 * @brief Copies the raw Kalman filter heading covariance data into the provided data structure.
 *
 * This function extracts the heading standard deviation value from the raw data buffer
 * and assigns it to the corresponding field in the s_kf_heading_covariance_t structure.
 *
 * @param[out] ptr_data      Pointer to the destination s_kf_heading_covariance_t structure.
 * @param[in]  ptr_raw_data  Pointer to the source buffer containing raw heading covariance data.
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_yaw_cov(s_kf_heading_covariance_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw Kalman filter heading covariance data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->heading_std = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies the raw new GPS state data into the provided data structure.
 *
 * This function takes a pointer to a u_new_gps_t structure and a pointer to raw GPS state data.
 * If both pointers are valid (not NULL), it copies the first byte of the raw data into the
 * 'new_gps' member of the u_new_gps_t structure.
 *
 * @param[out] ptr_data      Pointer to the u_new_gps_t structure where the GPS state will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw GPS state data (at least 1 byte).
 */
static void da_ins_il_copy_new_gps_state(u_new_gps_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw new GPS state data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->new_gps = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw yaw correction source data into a heading correction source structure.
 *
 * This function takes a pointer to a heading correction source structure and a pointer to raw data,
 * and copies the yaw correction source byte from the raw data into the structure's source field.
 *
 * @param[out] ptr_data      Pointer to the s_heading_correction_source_t structure to be updated.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the yaw correction source.
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_yaw_cor_src(s_heading_correction_source_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw yaw correction source data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->source = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies GNSS standard deviation data from a raw byte array to a GNSS data structure.
 *
 * This function extracts the latitude, longitude, and altitude standard deviation values
 * from the provided raw data array and assigns them to the corresponding fields in the
 * s_gnss_data_std_t structure. The raw data is expected to contain the standard deviation
 * values as consecutive 16-bit unsigned integers in the following order:
 *   - Bytes 0-1: Latitude standard deviation
 *   - Bytes 2-3: Longitude standard deviation
 *   - Bytes 4-5: Altitude standard deviation
 *
 * @param[out] ptr_data      Pointer to the s_gnss_data_std_t structure to be populated.
 * @param[in]  ptr_raw_data  Pointer to the raw byte array containing GNSS standard deviation data.
 *
 * @note Both pointers must be non-NULL for the copy operation to proceed.
 */
static void da_ins_il_copy_gnss_data_std(s_gnss_data_std_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GNSS data standard deviation to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gnss_lat_std = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->gnss_long_std = *((uint16_t *)&ptr_raw_data[2]);
        ptr_data->gnss_alt_std = *((uint16_t *)&ptr_raw_data[4]);
    }
}

/**
 * @brief Copies GNSS heading and pitch standard deviation values from raw data to the provided structure.
 *
 * This function extracts the GNSS heading and pitch standard deviation values from the given raw data buffer
 * and assigns them to the corresponding fields in the s_gnss_heading_pitch_std_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the extracted values will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the GNSS heading and pitch standard deviation values.
 *                           The buffer is expected to contain at least 4 bytes:
 *                             - Bytes 0-1: GNSS heading standard deviation (uint16_t)
 *                             - Bytes 2-3: GNSS pitch standard deviation (uint16_t)
 *
 * @note The function performs NULL pointer checks on both input pointers.
 */
static void da_ins_il_copy_gnss_yaw_pitch_std(s_gnss_heading_pitch_std_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GNSS heading and pitch standard deviation to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gnss_heading_std = *((uint16_t *)&ptr_raw_data[0]);
        ptr_data->gnss_pitch_std = *((uint16_t *)&ptr_raw_data[2]);
    }
}

/**
 * @brief Copies the raw GNSS solution status from a byte array to a GNSS solution status structure.
 *
 * This function extracts the GNSS solution status from the provided raw data buffer
 * and assigns it to the specified GNSS solution status structure.
 *
 * @param[out] ptr_data      Pointer to the GNSS solution status structure to be updated.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the GNSS solution status.
 *
 * @note Both pointers must be non-NULL for the copy to occur.
 */
static void da_ins_il_copy_gnss_sol_status(s_gnss_solution_status_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GNSS solution status to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gnss_solution_status = *((uint8_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw supply voltage data into the provided structure.
 *
 * This function extracts the supply voltage value from the raw data buffer
 * and assigns it to the corresponding field in the s_insd_supply_voltage_t structure.
 *
 * @param[out] ptr_data      Pointer to the structure where the supply voltage will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the supply voltage value.
 *
 * @note Both pointers must be non-NULL. The function does not perform endian conversion.
 */
static void da_ins_il_copy_supply_voltage(s_insd_supply_voltage_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw supply voltage data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->supply_voltage = *((uint16_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies raw temperature data into the provided temperature structure.
 *
 * This function extracts a 16-bit signed integer value from the raw data buffer
 * and assigns it to the temperature field of the given s_insd_temperature_t structure.
 *
 * @param[out] ptr_data      Pointer to the s_insd_temperature_t structure where the temperature will be stored.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing the temperature value (at least 2 bytes).
 *
 * @note The function performs NULL pointer checks before accessing the data.
 */
static void da_ins_il_copy_temperature(s_insd_temperature_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw temperature data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->temperature = *((int16_t *)&ptr_raw_data[0]);
    }
}

/**
 * @brief Copies GNSS position data from a raw byte array to a GNSS position structure.
 *
 * This function extracts latitude, longitude, and altitude values from the provided
 * raw data buffer and assigns them to the corresponding fields in the s_gnss_position_t
 * structure. The raw data is expected to be in the following order:
 * - Bytes 0-3: Latitude (int32_t)
 * - Bytes 4-7: Longitude (int32_t)
 * - Bytes 8-11: Altitude (int32_t)
 *
 * @param[out] ptr_data      Pointer to the s_gnss_position_t structure to be populated.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing GNSS position data.
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_gnss_position(s_gnss_position_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw position data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        ptr_data->gnss_latitude = *((int32_t *)&ptr_raw_data[0]);
        ptr_data->gnss_longitude = *((int32_t *)&ptr_raw_data[4]);
        ptr_data->gnss_altitude = *((int32_t *)&ptr_raw_data[8]);
    }
}

/**
 * @brief Copies raw GNSS velocity data from a byte buffer to a GNSS velocity structure.
 *
 * This function extracts GNSS velocity parameters (horizontal speed, track over ground,
 * and vertical speed) from a raw data buffer and stores them in the provided
 * s_gnss_velocity_t structure. The function assumes the data is packed in the buffer
 * as follows:
 *   - horizontal_speed_gnss: int32_t at offset 0
 *   - track_over_ground_gnss: uint16_t at offset 4
 *   - vertical_speed_gnss: int32_t at offset 6
 *
 * @param[out] ptr_data      Pointer to the s_gnss_velocity_t structure to populate.
 * @param[in]  ptr_raw_data  Pointer to the raw data buffer containing GNSS velocity data.
 *
 * @note Both pointers must be non-NULL. No action is taken if either pointer is NULL.
 */
static void da_ins_il_copy_gnss_velocity(s_gnss_velocity_t *ptr_data, const uint8_t *ptr_raw_data)
{
    /* Copy raw GNSS velocity data to the provided pointer */
    if ((ptr_raw_data != NULL) && (ptr_data != NULL))
    {
        /* Copy raw Horizontal Speed GNSS data from buffer */
        ptr_data->horizontal_speed_gnss = *((int32_t *)&ptr_raw_data[0]);
        /* Copy raw Track Over Ground GNSS data from buffer */
        ptr_data->track_over_ground_gnss = *((uint16_t *)&ptr_raw_data[4]);
        /* Copy raw Vertical Speed GNSS data from buffer */
        ptr_data->vertical_speed_gnss = *((int32_t *)&ptr_raw_data[6]);
    }
}

/**
 * @brief Scales the attitude angles (yaw, pitch, roll) from the given orientation structure
 *        and updates the global 'ins' structure with the scaled values in radians.
 *
 * This function multiplies the yaw, pitch, and roll values from the input pointer
 * by ATTITUDE_SCALING and D2R (degrees to radians conversion factor), and assigns
 * the results to the corresponding fields in the global 'ins' structure.
 *
 * @param[in] ptr_orientation Pointer to a s_orientation_angles_t structure containing
 *            the raw attitude angles (yaw, pitch, roll). If NULL, the function does nothing.
 */
static void da_ins_il_scale_attitude(s_orientation_angles_t *ptr_orientation)
{
    /* Scale attitude values */
    if (ptr_orientation != NULL)
    {
        InsProcessedData.scaled_euler_angle.yaw = (float)(ptr_orientation->yaw * ATTITUDE_SCALING * D2R);
        InsProcessedData.scaled_euler_angle.pitch = (float)(ptr_orientation->pitch * ATTITUDE_SCALING * D2R);
        InsProcessedData.scaled_euler_angle.roll = (float)(ptr_orientation->roll * ATTITUDE_SCALING * D2R);
    }
}

/**
 * @brief Scales raw IMU data and stores the results in the global INS structure.
 *
 * This function takes a pointer to a filtered IMU data structure, applies scaling factors
 * to the gyroscope and accelerometer readings, and stores the scaled values in the global
 * INS IMU structure. The gyroscope values are converted from degrees to radians and scaled,
 * while the accelerometer values are scaled according to predefined constants.
 *
 * @param[in] ptr_imu_data Pointer to the filtered IMU data structure (s_imu_data_filtered_t).
 *                         If NULL, the function does nothing.
 */
static void da_ins_il_scale_imu_data(s_imu_data_filtered_t *ptr_imu_data)
{
    /* Scale IMU data values */
    if (ptr_imu_data != NULL)
    {
        /* Angular Velocity Scaling */
        InsProcessedData.scaled_imu_data.gyro_y = (float)(ptr_imu_data->gyro_x * ANGULAR_VEL_SCALING * D2R); /* X <-> Y swapped due to the orientation of INS on the aircraft */
        InsProcessedData.scaled_imu_data.gyro_x = (float)(ptr_imu_data->gyro_y * ANGULAR_VEL_SCALING * D2R); /* X <-> Y swapped due to the orientation of INS on the aircraft */
        InsProcessedData.scaled_imu_data.gyro_z = (float)((-1 * ptr_imu_data->gyro_z) * ANGULAR_VEL_SCALING * D2R);

        /* Acceleration Scaling */
        InsProcessedData.scaled_imu_data.accel_y = (float)(ptr_imu_data->accel_x * ACCELERATION_SCALING); /* X <-> Y swapped due to the orientation of INS on the aircraft */
        InsProcessedData.scaled_imu_data.accel_x = (float)(ptr_imu_data->accel_y * ACCELERATION_SCALING); /* X <-> Y swapped due to the orientation of INS on the aircraft */
        InsProcessedData.scaled_imu_data.accel_z = (float)((-1 * ptr_imu_data->accel_z) * ACCELERATION_SCALING);
    }
}

/**
 * @brief Scales raw magnetometer data and updates the global INS structure.
 *
 * This function takes a pointer to a structure containing raw magnetometer data,
 * applies a scaling factor to each axis (X, Y, Z), and stores the scaled values
 * in the corresponding fields of the global INS structure.
 *
 * @param ptr_mag_data Pointer to the structure containing raw magnetometer data.
 *                     If NULL, the function does nothing.
 */
static void da_ins_il_scale_magnetometer_data(s_magnetometer_data_t *ptr_mag_data)
{
    /* Scale magnetometer data values */
    if (ptr_mag_data != NULL)
    {
        InsProcessedData.scaled_mag_data.mag_x = (float)(ptr_mag_data->mag_x * MAGNETIC_FIELD_SCALING);
        InsProcessedData.scaled_mag_data.mag_y = (float)(ptr_mag_data->mag_y * MAGNETIC_FIELD_SCALING);
        InsProcessedData.scaled_mag_data.mag_z = (float)(ptr_mag_data->mag_z * MAGNETIC_FIELD_SCALING);
    }
}

/**
 * @brief Scales and assigns position values from the given position structure.
 *
 * This function takes a pointer to an s_position_t structure, checks if it is not NULL,
 * and then scales and assigns its latitude, longitude, and altitude values to the global
 * or external 'ins' structure. The latitude and longitude are cast to int32_t, while the
 * altitude is scaled by POSITION_SCALING and assigned as a float.
 *
 * @param ptr_position Pointer to the s_position_t structure containing position data.
 */
static void da_ins_il_unscaled_position(s_position_t *ptr_position)
{
    /* Scale position values */
    if (ptr_position != NULL)
    {
        InsProcessedData.unscaled_pos.unscaled_lat = (int32_t)ptr_position->latitude;
        InsProcessedData.unscaled_pos.unscaled_lon = (int32_t)ptr_position->longitude;
        InsProcessedData.unscaled_pos.unscaled_alt = (float)(ptr_position->altitude * POSITION_SCALING);
    }
}

/**
 * @brief Scales the velocity values from the input structure and updates the global INS velocities.
 *
 * This function takes a pointer to a structure containing raw velocity values (east, north, and vertical speeds),
 * scales them using the VELOCITY_SCALING factor, and assigns the results to the corresponding fields in the global
 * INS structure. If the input pointer is NULL, the function does nothing.
 *
 * @param ptr_velocities Pointer to a s_velocities_t structure containing raw velocity values.
 */
static void da_ins_il_scale_inertial_vel(s_velocities_t *ptr_velocities)
{
    /* Scale velocity values */
    if (ptr_velocities != NULL)
    {
        InsProcessedData.scaled_inertial_vel.vel_e = (float)(ptr_velocities->east_speed * VELOCITY_SCALING);
        InsProcessedData.scaled_inertial_vel.vel_n = (float)(ptr_velocities->north_speed * VELOCITY_SCALING);
        InsProcessedData.scaled_inertial_vel.vel_d = (float)((-1 * ptr_velocities->vertical_speed) * VELOCITY_SCALING); /* Vertical up velocity is converted to down velocity */
    }
}

/**
 * @brief Scales the velocity values in the provided body frame velocity structure.
 *
 * This function takes a pointer to a structure containing body frame velocities,
 * scales each velocity component (x, y, z) by the predefined VELOCITY_SCALING factor,
 * and stores the results in the InsProcessedData.scaled_body_frm_vel structure.
 *
 * @param ptr_velocities Pointer to a s_body_frame_velocity_t structure containing
 *        the velocity components to be scaled. If NULL, the function does nothing.
 */
static void da_ins_il_scale_bodyfr_vel(s_body_frame_velocity_t *ptr_velocities)
{
    /* Scale velocity values */
    if (ptr_velocities != NULL)
    {
        InsProcessedData.scaled_body_frm_vel.vel_x = (float)(ptr_velocities->velocity_x * VELOCITY_SCALING);
        InsProcessedData.scaled_body_frm_vel.vel_y = (float)(ptr_velocities->velocity_y * VELOCITY_SCALING);
        InsProcessedData.scaled_body_frm_vel.vel_z = (float)(ptr_velocities->velocity_z * VELOCITY_SCALING);
    }
}

/**
 * @brief Scales raw barometric sensor data and updates INS structure.
 *
 * This function takes a pointer to a barometric data structure, scales the
 * pressure and altitude values using predefined scaling factors, and assigns
 * the results to the corresponding fields in the INS (Inertial Navigation System)
 * structure. If the input pointer is NULL, the function does nothing.
 *
 * @param ptr_baro_data Pointer to the structure containing raw barometric data.
 */
static void da_ins_il_scale_baro_data(s_barometer_data_t *ptr_baro_data)
{
    /* Scale barometric data values */
    if (ptr_baro_data != NULL)
    {
        InsProcessedData.scaled_baro_data.baro_p = (float)(ptr_baro_data->pressure * BARO_PRESSURE_SCALING);
        InsProcessedData.scaled_baro_data.baro_h = (float)(ptr_baro_data->baro_altitude * BARO_ALTITUDE_SCALING);
    }
}

/**
 * @brief Scales and stores position data from the provided position structure.
 *
 * This function takes a pointer to a position data structure (s_position_t),
 * scales its latitude and longitude fields by dividing them with the constant
 * POS_LAT_LONG_SCALING, and scales the altitude field by multiplying it with
 * ALT_SCALING. The scaled values are then stored in the global 'ins' structure.
 *
 * @param ptr_pos_data Pointer to the position data structure to be scaled.
 *                     If NULL, the function does nothing.
 */
static void da_ins_il_scale_pos_data(s_position_t *ptr_pos_data)
{
    if (ptr_pos_data != NULL)
    {
        InsProcessedData.scaled_position.lat = (double)(ptr_pos_data->latitude / POS_LAT_LONG_SCALING);
        InsProcessedData.scaled_position.lon = (double)(ptr_pos_data->longitude / POS_LAT_LONG_SCALING);
        InsProcessedData.scaled_position.alt = (float)(ptr_pos_data->altitude * ALT_SCALING);
    }
}

/**
 * @brief Scales GNSS position standard deviation values and updates INS error estimates.
 *
 * This function takes a pointer to a structure containing GNSS position standard deviation values,
 * scales the latitude, longitude, and altitude standard deviations by a predefined meter scaling factor,
 * and updates the global INS structure's estimated position error horizontal (eph) and vertical (epv) values.
 *
 * @param[in] ptr_gns_pos_std Pointer to the GNSS position standard deviation structure (s_gnss_data_std_t).
 *                            If NULL, the function will not update the error estimates.
 *
 * @note The function assumes that the global 'ins' structure and 'METER_SCALING' constant are defined elsewhere.
 */
static void da_ins_il_scale_gnss_pos_std(s_gnss_data_std_t *ptr_gns_pos_std)
{
    float gnsslat_std = 0.0f;
    float gnsslon_std = 0.0f;

    /* Scale GNSS position STD values */
    if (ptr_gns_pos_std != NULL)
    {
        gnsslat_std = (float)ptr_gns_pos_std->gnss_lat_std * METER_SCALING;
        gnsslon_std = (float)ptr_gns_pos_std->gnss_long_std * METER_SCALING;
    }

    /* Estimated Position Error Horizontal */
    InsProcessedData.scaled_h_v_pos_err.eph = (float)(sqrt((gnsslat_std * gnsslat_std) + (gnsslon_std * gnsslon_std)));
    /* Estimated Position Error Vertical */
    InsProcessedData.scaled_h_v_pos_err.epv = (float)(ptr_gns_pos_std->gnss_alt_std * METER_SCALING);
}

/**
 * @brief Scales GDOP and PDOP values from the provided structure and stores them in InsProcessedData.
 *
 * This function takes a pointer to an s_gdop_pdop_t structure, scales its GDOP and PDOP values
 * using the predefined GDOP_SCALING and PDOP_SCALING factors, and stores the results in the
 * InsProcessedData.scaled_gp_dop structure. If the input pointer is NULL, the function does nothing.
 *
 * @param[in] ptr_gdop_pdop Pointer to the s_gdop_pdop_t structure containing raw GDOP and PDOP values.
 */
static void da_ins_il_scale_gdop_pdop(s_gdop_pdop_t *ptr_gdop_pdop)
{
    /* Scale GDOP and PDOP values */
    if (ptr_gdop_pdop != NULL)
    {
        InsProcessedData.scaled_gp_dop.gdop = (float)ptr_gdop_pdop->gdop * GDOP_SCALING;
        InsProcessedData.scaled_gp_dop.pdop = (float)ptr_gdop_pdop->pdop * PDOP_SCALING;
    }
}

/**
 * @brief Computes and updates the attitude validity flag for the INS.
 *
 * This function checks the status bits in the UddRawData structure to determine
 * if the initial alignment, software status, gyro status, accelerometer status,
 * and electrical status are all in a non-error (zero) state. If all these
 * conditions are met, the function sets the INS attitude validity flag
 * to false, indicating that the attitude is not valid.
 *
 * @note This function does not return a value; it updates the global or static
 *       variable based on the status bits.
 */
static void da_ins_il_inspect_att_invalid(void)
{
    if ((UddRawData.unit_status_word.s_status_bits.s_init_align == 0) &&
        (UddRawData.unit_status_word.s_status_bits.s_sw_status == 0) &&
        (UddRawData.unit_status_word.s_status_bits.s_gyro_status == 0) &&
        (UddRawData.unit_status_word.s_status_bits.s_acc_status == 0) &&
        (UddRawData.unit_status_word.s_status_bits.s_elec_status == 0) && (InsCommTimeout != true))
    {
        InsProcessedData.attitude_invalid = false;
    }
    else
    {
        InsProcessedData.attitude_invalid = true;
    }
}

/**
 * @brief Inspects and updates the INS position validity flag.
 *
 * This function checks the GNSS receiver status, the number of satellites used, position
 * standard deiation, and insd darta timeout.
 * If the GNSS receiver is operational (s_gnss_rcvr == 0), at least 4 satellites are used,
 * position standard deviation is less than 20 m, and there is no communication timeout,
 * it sets the InsProcessedData.position_invalid flag to false, indicating a valid position.
 * Otherwise, it sets the flag to true, indicating an invalid position.
 *
 * @note Relies on the global structures UddRawData and InsProcessedData.
 */
static void da_ins_il_inspect_position_invalid(void)
{
    if ((UddRawData.unit_status_word.s_status_bits.s_gnss_rcvr == 0) &&
        (UddRawData.no_of_sat_used.num_satellites >= 4) &&
        (InsProcessedData.scaled_h_v_pos_err.eph < 20.0f) &&
        (InsCommTimeout != true))
    {
        InsProcessedData.position_invalid = false;
    }
    else
    {
        InsProcessedData.position_invalid = true;
    }
}
