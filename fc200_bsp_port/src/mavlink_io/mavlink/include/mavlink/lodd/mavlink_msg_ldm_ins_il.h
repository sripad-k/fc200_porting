#pragma once
// MESSAGE LDM_INS_IL PACKING

#define MAVLINK_MSG_ID_LDM_INS_IL 52021


typedef struct __mavlink_ldm_ins_il_t {
 uint32_t time_ms; /*<  FCC internal time when the msg is sent*/
 float omg[3]; /*<  angular rate rad/s*/
 float acc[3]; /*<  body frame accel measured by accelerometer m/s/s*/
 int32_t lat; /*<  lat in 10^7 deg*/
 int32_t lon; /*<  lon in 10^7 deg*/
 float alt_amsl; /*<  EKF alt AMSL m*/
 float alt_baro; /*<  baro alt m*/
 int16_t eul_rpy[3]; /*<  Euler angle: RPY 10^2 deg in [-180,180] deg range*/
 int16_t vel_ned[3]; /*<  NED vel in cm/s*/
 uint16_t gnss_h_acc; /*<  GNSS horizontal uncertainty in cm*/
 uint16_t gnss_v_acc; /*<  GNSS vertical uncertainty in cm*/
 uint8_t kf_pos_cov_lla[3]; /*<  KF pos covariance LLA in cm*/
 uint8_t kf_vel_cov_ned[3]; /*<  KF vel covariance NED in mm/s*/
 uint8_t ins_sol_status; /*<  ins solution status, 0: INS sol is good. Ref Table 6.124*/
 uint8_t gnss_n_sat; /*<  number of satellites used in INS sol*/
 uint8_t gnss_fix_type; /*<  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)*/
 uint8_t gnss_pdop; /*<  GNSS pdop*10*/
 int8_t temp; /*< [deg C] ins temperature*/
 int8_t validity; /*<  Ind data validity flags*/
} mavlink_ldm_ins_il_t;

#define MAVLINK_MSG_ID_LDM_INS_IL_LEN 72
#define MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN 72
#define MAVLINK_MSG_ID_52021_LEN 72
#define MAVLINK_MSG_ID_52021_MIN_LEN 72

#define MAVLINK_MSG_ID_LDM_INS_IL_CRC 92
#define MAVLINK_MSG_ID_52021_CRC 92

#define MAVLINK_MSG_LDM_INS_IL_FIELD_OMG_LEN 3
#define MAVLINK_MSG_LDM_INS_IL_FIELD_ACC_LEN 3
#define MAVLINK_MSG_LDM_INS_IL_FIELD_EUL_RPY_LEN 3
#define MAVLINK_MSG_LDM_INS_IL_FIELD_VEL_NED_LEN 3
#define MAVLINK_MSG_LDM_INS_IL_FIELD_KF_POS_COV_LLA_LEN 3
#define MAVLINK_MSG_LDM_INS_IL_FIELD_KF_VEL_COV_NED_LEN 3

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_INS_IL { \
    52021, \
    "LDM_INS_IL", \
    19, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_ins_il_t, time_ms) }, \
         { "eul_rpy", NULL, MAVLINK_TYPE_INT16_T, 3, 44, offsetof(mavlink_ldm_ins_il_t, eul_rpy) }, \
         { "omg", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_ins_il_t, omg) }, \
         { "acc", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_ins_il_t, acc) }, \
         { "vel_ned", NULL, MAVLINK_TYPE_INT16_T, 3, 50, offsetof(mavlink_ldm_ins_il_t, vel_ned) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ldm_ins_il_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_ldm_ins_il_t, lon) }, \
         { "alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_ldm_ins_il_t, alt_amsl) }, \
         { "kf_pos_cov_lla", NULL, MAVLINK_TYPE_UINT8_T, 3, 60, offsetof(mavlink_ldm_ins_il_t, kf_pos_cov_lla) }, \
         { "kf_vel_cov_ned", NULL, MAVLINK_TYPE_UINT8_T, 3, 63, offsetof(mavlink_ldm_ins_il_t, kf_vel_cov_ned) }, \
         { "ins_sol_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 66, offsetof(mavlink_ldm_ins_il_t, ins_sol_status) }, \
         { "gnss_n_sat", NULL, MAVLINK_TYPE_UINT8_T, 0, 67, offsetof(mavlink_ldm_ins_il_t, gnss_n_sat) }, \
         { "gnss_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 68, offsetof(mavlink_ldm_ins_il_t, gnss_fix_type) }, \
         { "gnss_h_acc", NULL, MAVLINK_TYPE_UINT16_T, 0, 56, offsetof(mavlink_ldm_ins_il_t, gnss_h_acc) }, \
         { "gnss_v_acc", NULL, MAVLINK_TYPE_UINT16_T, 0, 58, offsetof(mavlink_ldm_ins_il_t, gnss_v_acc) }, \
         { "gnss_pdop", NULL, MAVLINK_TYPE_UINT8_T, 0, 69, offsetof(mavlink_ldm_ins_il_t, gnss_pdop) }, \
         { "alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_ldm_ins_il_t, alt_baro) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 0, 70, offsetof(mavlink_ldm_ins_il_t, temp) }, \
         { "validity", NULL, MAVLINK_TYPE_INT8_T, 0, 71, offsetof(mavlink_ldm_ins_il_t, validity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_INS_IL { \
    "LDM_INS_IL", \
    19, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_ins_il_t, time_ms) }, \
         { "eul_rpy", NULL, MAVLINK_TYPE_INT16_T, 3, 44, offsetof(mavlink_ldm_ins_il_t, eul_rpy) }, \
         { "omg", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_ins_il_t, omg) }, \
         { "acc", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_ins_il_t, acc) }, \
         { "vel_ned", NULL, MAVLINK_TYPE_INT16_T, 3, 50, offsetof(mavlink_ldm_ins_il_t, vel_ned) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_ldm_ins_il_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_ldm_ins_il_t, lon) }, \
         { "alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_ldm_ins_il_t, alt_amsl) }, \
         { "kf_pos_cov_lla", NULL, MAVLINK_TYPE_UINT8_T, 3, 60, offsetof(mavlink_ldm_ins_il_t, kf_pos_cov_lla) }, \
         { "kf_vel_cov_ned", NULL, MAVLINK_TYPE_UINT8_T, 3, 63, offsetof(mavlink_ldm_ins_il_t, kf_vel_cov_ned) }, \
         { "ins_sol_status", NULL, MAVLINK_TYPE_UINT8_T, 0, 66, offsetof(mavlink_ldm_ins_il_t, ins_sol_status) }, \
         { "gnss_n_sat", NULL, MAVLINK_TYPE_UINT8_T, 0, 67, offsetof(mavlink_ldm_ins_il_t, gnss_n_sat) }, \
         { "gnss_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 68, offsetof(mavlink_ldm_ins_il_t, gnss_fix_type) }, \
         { "gnss_h_acc", NULL, MAVLINK_TYPE_UINT16_T, 0, 56, offsetof(mavlink_ldm_ins_il_t, gnss_h_acc) }, \
         { "gnss_v_acc", NULL, MAVLINK_TYPE_UINT16_T, 0, 58, offsetof(mavlink_ldm_ins_il_t, gnss_v_acc) }, \
         { "gnss_pdop", NULL, MAVLINK_TYPE_UINT8_T, 0, 69, offsetof(mavlink_ldm_ins_il_t, gnss_pdop) }, \
         { "alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 40, offsetof(mavlink_ldm_ins_il_t, alt_baro) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 0, 70, offsetof(mavlink_ldm_ins_il_t, temp) }, \
         { "validity", NULL, MAVLINK_TYPE_INT8_T, 0, 71, offsetof(mavlink_ldm_ins_il_t, validity) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_ins_il message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param eul_rpy  Euler angle: RPY 10^2 deg in [-180,180] deg range
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel in cm/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param kf_pos_cov_lla  KF pos covariance LLA in cm
 * @param kf_vel_cov_ned  KF vel covariance NED in mm/s
 * @param ins_sol_status  ins solution status, 0: INS sol is good. Ref Table 6.124
 * @param gnss_n_sat  number of satellites used in INS sol
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in cm
 * @param gnss_v_acc  GNSS vertical uncertainty in cm
 * @param gnss_pdop  GNSS pdop*10
 * @param alt_baro  baro alt m
 * @param temp [deg C] ins temperature
 * @param validity  Ind data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ins_il_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *eul_rpy, const float *omg, const float *acc, const int16_t *vel_ned, int32_t lat, int32_t lon, float alt_amsl, const uint8_t *kf_pos_cov_lla, const uint8_t *kf_vel_cov_ned, uint8_t ins_sol_status, uint8_t gnss_n_sat, uint8_t gnss_fix_type, uint16_t gnss_h_acc, uint16_t gnss_v_acc, uint8_t gnss_pdop, float alt_baro, int8_t temp, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_INS_IL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 28, lat);
    _mav_put_int32_t(buf, 32, lon);
    _mav_put_float(buf, 36, alt_amsl);
    _mav_put_float(buf, 40, alt_baro);
    _mav_put_uint16_t(buf, 56, gnss_h_acc);
    _mav_put_uint16_t(buf, 58, gnss_v_acc);
    _mav_put_uint8_t(buf, 66, ins_sol_status);
    _mav_put_uint8_t(buf, 67, gnss_n_sat);
    _mav_put_uint8_t(buf, 68, gnss_fix_type);
    _mav_put_uint8_t(buf, 69, gnss_pdop);
    _mav_put_int8_t(buf, 70, temp);
    _mav_put_int8_t(buf, 71, validity);
    _mav_put_float_array(buf, 4, omg, 3);
    _mav_put_float_array(buf, 16, acc, 3);
    _mav_put_int16_t_array(buf, 44, eul_rpy, 3);
    _mav_put_int16_t_array(buf, 50, vel_ned, 3);
    _mav_put_uint8_t_array(buf, 60, kf_pos_cov_lla, 3);
    _mav_put_uint8_t_array(buf, 63, kf_vel_cov_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#else
    mavlink_ldm_ins_il_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.alt_baro = alt_baro;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.ins_sol_status = ins_sol_status;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.gnss_pdop = gnss_pdop;
    packet.temp = temp;
    packet.validity = validity;
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(int16_t)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(int16_t)*3);
    mav_array_memcpy(packet.kf_pos_cov_lla, kf_pos_cov_lla, sizeof(uint8_t)*3);
    mav_array_memcpy(packet.kf_vel_cov_ned, kf_vel_cov_ned, sizeof(uint8_t)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_INS_IL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
}

/**
 * @brief Pack a ldm_ins_il message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param eul_rpy  Euler angle: RPY 10^2 deg in [-180,180] deg range
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel in cm/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param kf_pos_cov_lla  KF pos covariance LLA in cm
 * @param kf_vel_cov_ned  KF vel covariance NED in mm/s
 * @param ins_sol_status  ins solution status, 0: INS sol is good. Ref Table 6.124
 * @param gnss_n_sat  number of satellites used in INS sol
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in cm
 * @param gnss_v_acc  GNSS vertical uncertainty in cm
 * @param gnss_pdop  GNSS pdop*10
 * @param alt_baro  baro alt m
 * @param temp [deg C] ins temperature
 * @param validity  Ind data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ins_il_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *eul_rpy, const float *omg, const float *acc, const int16_t *vel_ned, int32_t lat, int32_t lon, float alt_amsl, const uint8_t *kf_pos_cov_lla, const uint8_t *kf_vel_cov_ned, uint8_t ins_sol_status, uint8_t gnss_n_sat, uint8_t gnss_fix_type, uint16_t gnss_h_acc, uint16_t gnss_v_acc, uint8_t gnss_pdop, float alt_baro, int8_t temp, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_INS_IL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 28, lat);
    _mav_put_int32_t(buf, 32, lon);
    _mav_put_float(buf, 36, alt_amsl);
    _mav_put_float(buf, 40, alt_baro);
    _mav_put_uint16_t(buf, 56, gnss_h_acc);
    _mav_put_uint16_t(buf, 58, gnss_v_acc);
    _mav_put_uint8_t(buf, 66, ins_sol_status);
    _mav_put_uint8_t(buf, 67, gnss_n_sat);
    _mav_put_uint8_t(buf, 68, gnss_fix_type);
    _mav_put_uint8_t(buf, 69, gnss_pdop);
    _mav_put_int8_t(buf, 70, temp);
    _mav_put_int8_t(buf, 71, validity);
    _mav_put_float_array(buf, 4, omg, 3);
    _mav_put_float_array(buf, 16, acc, 3);
    _mav_put_int16_t_array(buf, 44, eul_rpy, 3);
    _mav_put_int16_t_array(buf, 50, vel_ned, 3);
    _mav_put_uint8_t_array(buf, 60, kf_pos_cov_lla, 3);
    _mav_put_uint8_t_array(buf, 63, kf_vel_cov_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#else
    mavlink_ldm_ins_il_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.alt_baro = alt_baro;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.ins_sol_status = ins_sol_status;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.gnss_pdop = gnss_pdop;
    packet.temp = temp;
    packet.validity = validity;
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(int16_t)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(int16_t)*3);
    mav_array_memcpy(packet.kf_pos_cov_lla, kf_pos_cov_lla, sizeof(uint8_t)*3);
    mav_array_memcpy(packet.kf_vel_cov_ned, kf_vel_cov_ned, sizeof(uint8_t)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_INS_IL;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#endif
}

/**
 * @brief Pack a ldm_ins_il message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  FCC internal time when the msg is sent
 * @param eul_rpy  Euler angle: RPY 10^2 deg in [-180,180] deg range
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel in cm/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param kf_pos_cov_lla  KF pos covariance LLA in cm
 * @param kf_vel_cov_ned  KF vel covariance NED in mm/s
 * @param ins_sol_status  ins solution status, 0: INS sol is good. Ref Table 6.124
 * @param gnss_n_sat  number of satellites used in INS sol
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in cm
 * @param gnss_v_acc  GNSS vertical uncertainty in cm
 * @param gnss_pdop  GNSS pdop*10
 * @param alt_baro  baro alt m
 * @param temp [deg C] ins temperature
 * @param validity  Ind data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ins_il_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const int16_t *eul_rpy,const float *omg,const float *acc,const int16_t *vel_ned,int32_t lat,int32_t lon,float alt_amsl,const uint8_t *kf_pos_cov_lla,const uint8_t *kf_vel_cov_ned,uint8_t ins_sol_status,uint8_t gnss_n_sat,uint8_t gnss_fix_type,uint16_t gnss_h_acc,uint16_t gnss_v_acc,uint8_t gnss_pdop,float alt_baro,int8_t temp,int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_INS_IL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 28, lat);
    _mav_put_int32_t(buf, 32, lon);
    _mav_put_float(buf, 36, alt_amsl);
    _mav_put_float(buf, 40, alt_baro);
    _mav_put_uint16_t(buf, 56, gnss_h_acc);
    _mav_put_uint16_t(buf, 58, gnss_v_acc);
    _mav_put_uint8_t(buf, 66, ins_sol_status);
    _mav_put_uint8_t(buf, 67, gnss_n_sat);
    _mav_put_uint8_t(buf, 68, gnss_fix_type);
    _mav_put_uint8_t(buf, 69, gnss_pdop);
    _mav_put_int8_t(buf, 70, temp);
    _mav_put_int8_t(buf, 71, validity);
    _mav_put_float_array(buf, 4, omg, 3);
    _mav_put_float_array(buf, 16, acc, 3);
    _mav_put_int16_t_array(buf, 44, eul_rpy, 3);
    _mav_put_int16_t_array(buf, 50, vel_ned, 3);
    _mav_put_uint8_t_array(buf, 60, kf_pos_cov_lla, 3);
    _mav_put_uint8_t_array(buf, 63, kf_vel_cov_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#else
    mavlink_ldm_ins_il_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.alt_baro = alt_baro;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.ins_sol_status = ins_sol_status;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.gnss_pdop = gnss_pdop;
    packet.temp = temp;
    packet.validity = validity;
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(int16_t)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(int16_t)*3);
    mav_array_memcpy(packet.kf_pos_cov_lla, kf_pos_cov_lla, sizeof(uint8_t)*3);
    mav_array_memcpy(packet.kf_vel_cov_ned, kf_vel_cov_ned, sizeof(uint8_t)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_INS_IL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
}

/**
 * @brief Encode a ldm_ins_il struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ins_il C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ins_il_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_ins_il_t* ldm_ins_il)
{
    return mavlink_msg_ldm_ins_il_pack(system_id, component_id, msg, ldm_ins_il->time_ms, ldm_ins_il->eul_rpy, ldm_ins_il->omg, ldm_ins_il->acc, ldm_ins_il->vel_ned, ldm_ins_il->lat, ldm_ins_il->lon, ldm_ins_il->alt_amsl, ldm_ins_il->kf_pos_cov_lla, ldm_ins_il->kf_vel_cov_ned, ldm_ins_il->ins_sol_status, ldm_ins_il->gnss_n_sat, ldm_ins_il->gnss_fix_type, ldm_ins_il->gnss_h_acc, ldm_ins_il->gnss_v_acc, ldm_ins_il->gnss_pdop, ldm_ins_il->alt_baro, ldm_ins_il->temp, ldm_ins_il->validity);
}

/**
 * @brief Encode a ldm_ins_il struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ins_il C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ins_il_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_ins_il_t* ldm_ins_il)
{
    return mavlink_msg_ldm_ins_il_pack_chan(system_id, component_id, chan, msg, ldm_ins_il->time_ms, ldm_ins_il->eul_rpy, ldm_ins_il->omg, ldm_ins_il->acc, ldm_ins_il->vel_ned, ldm_ins_il->lat, ldm_ins_il->lon, ldm_ins_il->alt_amsl, ldm_ins_il->kf_pos_cov_lla, ldm_ins_il->kf_vel_cov_ned, ldm_ins_il->ins_sol_status, ldm_ins_il->gnss_n_sat, ldm_ins_il->gnss_fix_type, ldm_ins_il->gnss_h_acc, ldm_ins_il->gnss_v_acc, ldm_ins_il->gnss_pdop, ldm_ins_il->alt_baro, ldm_ins_il->temp, ldm_ins_il->validity);
}

/**
 * @brief Encode a ldm_ins_il struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ins_il C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ins_il_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_ins_il_t* ldm_ins_il)
{
    return mavlink_msg_ldm_ins_il_pack_status(system_id, component_id, _status, msg,  ldm_ins_il->time_ms, ldm_ins_il->eul_rpy, ldm_ins_il->omg, ldm_ins_il->acc, ldm_ins_il->vel_ned, ldm_ins_il->lat, ldm_ins_il->lon, ldm_ins_il->alt_amsl, ldm_ins_il->kf_pos_cov_lla, ldm_ins_il->kf_vel_cov_ned, ldm_ins_il->ins_sol_status, ldm_ins_il->gnss_n_sat, ldm_ins_il->gnss_fix_type, ldm_ins_il->gnss_h_acc, ldm_ins_il->gnss_v_acc, ldm_ins_il->gnss_pdop, ldm_ins_il->alt_baro, ldm_ins_il->temp, ldm_ins_il->validity);
}

/**
 * @brief Send a ldm_ins_il message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param eul_rpy  Euler angle: RPY 10^2 deg in [-180,180] deg range
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel in cm/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param kf_pos_cov_lla  KF pos covariance LLA in cm
 * @param kf_vel_cov_ned  KF vel covariance NED in mm/s
 * @param ins_sol_status  ins solution status, 0: INS sol is good. Ref Table 6.124
 * @param gnss_n_sat  number of satellites used in INS sol
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in cm
 * @param gnss_v_acc  GNSS vertical uncertainty in cm
 * @param gnss_pdop  GNSS pdop*10
 * @param alt_baro  baro alt m
 * @param temp [deg C] ins temperature
 * @param validity  Ind data validity flags
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_ins_il_send(mavlink_channel_t chan, uint32_t time_ms, const int16_t *eul_rpy, const float *omg, const float *acc, const int16_t *vel_ned, int32_t lat, int32_t lon, float alt_amsl, const uint8_t *kf_pos_cov_lla, const uint8_t *kf_vel_cov_ned, uint8_t ins_sol_status, uint8_t gnss_n_sat, uint8_t gnss_fix_type, uint16_t gnss_h_acc, uint16_t gnss_v_acc, uint8_t gnss_pdop, float alt_baro, int8_t temp, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_INS_IL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 28, lat);
    _mav_put_int32_t(buf, 32, lon);
    _mav_put_float(buf, 36, alt_amsl);
    _mav_put_float(buf, 40, alt_baro);
    _mav_put_uint16_t(buf, 56, gnss_h_acc);
    _mav_put_uint16_t(buf, 58, gnss_v_acc);
    _mav_put_uint8_t(buf, 66, ins_sol_status);
    _mav_put_uint8_t(buf, 67, gnss_n_sat);
    _mav_put_uint8_t(buf, 68, gnss_fix_type);
    _mav_put_uint8_t(buf, 69, gnss_pdop);
    _mav_put_int8_t(buf, 70, temp);
    _mav_put_int8_t(buf, 71, validity);
    _mav_put_float_array(buf, 4, omg, 3);
    _mav_put_float_array(buf, 16, acc, 3);
    _mav_put_int16_t_array(buf, 44, eul_rpy, 3);
    _mav_put_int16_t_array(buf, 50, vel_ned, 3);
    _mav_put_uint8_t_array(buf, 60, kf_pos_cov_lla, 3);
    _mav_put_uint8_t_array(buf, 63, kf_vel_cov_ned, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_INS_IL, buf, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#else
    mavlink_ldm_ins_il_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.alt_baro = alt_baro;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.ins_sol_status = ins_sol_status;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.gnss_pdop = gnss_pdop;
    packet.temp = temp;
    packet.validity = validity;
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(int16_t)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(int16_t)*3);
    mav_array_memcpy(packet.kf_pos_cov_lla, kf_pos_cov_lla, sizeof(uint8_t)*3);
    mav_array_memcpy(packet.kf_vel_cov_ned, kf_vel_cov_ned, sizeof(uint8_t)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_INS_IL, (const char *)&packet, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#endif
}

/**
 * @brief Send a ldm_ins_il message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_ins_il_send_struct(mavlink_channel_t chan, const mavlink_ldm_ins_il_t* ldm_ins_il)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_ins_il_send(chan, ldm_ins_il->time_ms, ldm_ins_il->eul_rpy, ldm_ins_il->omg, ldm_ins_il->acc, ldm_ins_il->vel_ned, ldm_ins_il->lat, ldm_ins_il->lon, ldm_ins_il->alt_amsl, ldm_ins_il->kf_pos_cov_lla, ldm_ins_il->kf_vel_cov_ned, ldm_ins_il->ins_sol_status, ldm_ins_il->gnss_n_sat, ldm_ins_il->gnss_fix_type, ldm_ins_il->gnss_h_acc, ldm_ins_il->gnss_v_acc, ldm_ins_il->gnss_pdop, ldm_ins_il->alt_baro, ldm_ins_il->temp, ldm_ins_il->validity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_INS_IL, (const char *)ldm_ins_il, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_INS_IL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_ins_il_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const int16_t *eul_rpy, const float *omg, const float *acc, const int16_t *vel_ned, int32_t lat, int32_t lon, float alt_amsl, const uint8_t *kf_pos_cov_lla, const uint8_t *kf_vel_cov_ned, uint8_t ins_sol_status, uint8_t gnss_n_sat, uint8_t gnss_fix_type, uint16_t gnss_h_acc, uint16_t gnss_v_acc, uint8_t gnss_pdop, float alt_baro, int8_t temp, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 28, lat);
    _mav_put_int32_t(buf, 32, lon);
    _mav_put_float(buf, 36, alt_amsl);
    _mav_put_float(buf, 40, alt_baro);
    _mav_put_uint16_t(buf, 56, gnss_h_acc);
    _mav_put_uint16_t(buf, 58, gnss_v_acc);
    _mav_put_uint8_t(buf, 66, ins_sol_status);
    _mav_put_uint8_t(buf, 67, gnss_n_sat);
    _mav_put_uint8_t(buf, 68, gnss_fix_type);
    _mav_put_uint8_t(buf, 69, gnss_pdop);
    _mav_put_int8_t(buf, 70, temp);
    _mav_put_int8_t(buf, 71, validity);
    _mav_put_float_array(buf, 4, omg, 3);
    _mav_put_float_array(buf, 16, acc, 3);
    _mav_put_int16_t_array(buf, 44, eul_rpy, 3);
    _mav_put_int16_t_array(buf, 50, vel_ned, 3);
    _mav_put_uint8_t_array(buf, 60, kf_pos_cov_lla, 3);
    _mav_put_uint8_t_array(buf, 63, kf_vel_cov_ned, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_INS_IL, buf, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#else
    mavlink_ldm_ins_il_t *packet = (mavlink_ldm_ins_il_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->lat = lat;
    packet->lon = lon;
    packet->alt_amsl = alt_amsl;
    packet->alt_baro = alt_baro;
    packet->gnss_h_acc = gnss_h_acc;
    packet->gnss_v_acc = gnss_v_acc;
    packet->ins_sol_status = ins_sol_status;
    packet->gnss_n_sat = gnss_n_sat;
    packet->gnss_fix_type = gnss_fix_type;
    packet->gnss_pdop = gnss_pdop;
    packet->temp = temp;
    packet->validity = validity;
    mav_array_memcpy(packet->omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet->acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet->eul_rpy, eul_rpy, sizeof(int16_t)*3);
    mav_array_memcpy(packet->vel_ned, vel_ned, sizeof(int16_t)*3);
    mav_array_memcpy(packet->kf_pos_cov_lla, kf_pos_cov_lla, sizeof(uint8_t)*3);
    mav_array_memcpy(packet->kf_vel_cov_ned, kf_vel_cov_ned, sizeof(uint8_t)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_INS_IL, (const char *)packet, MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN, MAVLINK_MSG_ID_LDM_INS_IL_LEN, MAVLINK_MSG_ID_LDM_INS_IL_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_INS_IL UNPACKING


/**
 * @brief Get field time_ms from ldm_ins_il message
 *
 * @return  FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_ins_il_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field eul_rpy from ldm_ins_il message
 *
 * @return  Euler angle: RPY 10^2 deg in [-180,180] deg range
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_eul_rpy(const mavlink_message_t* msg, int16_t *eul_rpy)
{
    return _MAV_RETURN_int16_t_array(msg, eul_rpy, 3,  44);
}

/**
 * @brief Get field omg from ldm_ins_il message
 *
 * @return  angular rate rad/s
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_omg(const mavlink_message_t* msg, float *omg)
{
    return _MAV_RETURN_float_array(msg, omg, 3,  4);
}

/**
 * @brief Get field acc from ldm_ins_il message
 *
 * @return  body frame accel measured by accelerometer m/s/s
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_acc(const mavlink_message_t* msg, float *acc)
{
    return _MAV_RETURN_float_array(msg, acc, 3,  16);
}

/**
 * @brief Get field vel_ned from ldm_ins_il message
 *
 * @return  NED vel in cm/s
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_vel_ned(const mavlink_message_t* msg, int16_t *vel_ned)
{
    return _MAV_RETURN_int16_t_array(msg, vel_ned, 3,  50);
}

/**
 * @brief Get field lat from ldm_ins_il message
 *
 * @return  lat in 10^7 deg
 */
static inline int32_t mavlink_msg_ldm_ins_il_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  28);
}

/**
 * @brief Get field lon from ldm_ins_il message
 *
 * @return  lon in 10^7 deg
 */
static inline int32_t mavlink_msg_ldm_ins_il_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  32);
}

/**
 * @brief Get field alt_amsl from ldm_ins_il message
 *
 * @return  EKF alt AMSL m
 */
static inline float mavlink_msg_ldm_ins_il_get_alt_amsl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Get field kf_pos_cov_lla from ldm_ins_il message
 *
 * @return  KF pos covariance LLA in cm
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_kf_pos_cov_lla(const mavlink_message_t* msg, uint8_t *kf_pos_cov_lla)
{
    return _MAV_RETURN_uint8_t_array(msg, kf_pos_cov_lla, 3,  60);
}

/**
 * @brief Get field kf_vel_cov_ned from ldm_ins_il message
 *
 * @return  KF vel covariance NED in mm/s
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_kf_vel_cov_ned(const mavlink_message_t* msg, uint8_t *kf_vel_cov_ned)
{
    return _MAV_RETURN_uint8_t_array(msg, kf_vel_cov_ned, 3,  63);
}

/**
 * @brief Get field ins_sol_status from ldm_ins_il message
 *
 * @return  ins solution status, 0: INS sol is good. Ref Table 6.124
 */
static inline uint8_t mavlink_msg_ldm_ins_il_get_ins_sol_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  66);
}

/**
 * @brief Get field gnss_n_sat from ldm_ins_il message
 *
 * @return  number of satellites used in INS sol
 */
static inline uint8_t mavlink_msg_ldm_ins_il_get_gnss_n_sat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  67);
}

/**
 * @brief Get field gnss_fix_type from ldm_ins_il message
 *
 * @return  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 */
static inline uint8_t mavlink_msg_ldm_ins_il_get_gnss_fix_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  68);
}

/**
 * @brief Get field gnss_h_acc from ldm_ins_il message
 *
 * @return  GNSS horizontal uncertainty in cm
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_gnss_h_acc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  56);
}

/**
 * @brief Get field gnss_v_acc from ldm_ins_il message
 *
 * @return  GNSS vertical uncertainty in cm
 */
static inline uint16_t mavlink_msg_ldm_ins_il_get_gnss_v_acc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  58);
}

/**
 * @brief Get field gnss_pdop from ldm_ins_il message
 *
 * @return  GNSS pdop*10
 */
static inline uint8_t mavlink_msg_ldm_ins_il_get_gnss_pdop(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  69);
}

/**
 * @brief Get field alt_baro from ldm_ins_il message
 *
 * @return  baro alt m
 */
static inline float mavlink_msg_ldm_ins_il_get_alt_baro(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  40);
}

/**
 * @brief Get field temp from ldm_ins_il message
 *
 * @return [deg C] ins temperature
 */
static inline int8_t mavlink_msg_ldm_ins_il_get_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  70);
}

/**
 * @brief Get field validity from ldm_ins_il message
 *
 * @return  Ind data validity flags
 */
static inline int8_t mavlink_msg_ldm_ins_il_get_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  71);
}

/**
 * @brief Decode a ldm_ins_il message into a struct
 *
 * @param msg The message to decode
 * @param ldm_ins_il C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_ins_il_decode(const mavlink_message_t* msg, mavlink_ldm_ins_il_t* ldm_ins_il)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_ins_il->time_ms = mavlink_msg_ldm_ins_il_get_time_ms(msg);
    mavlink_msg_ldm_ins_il_get_omg(msg, ldm_ins_il->omg);
    mavlink_msg_ldm_ins_il_get_acc(msg, ldm_ins_il->acc);
    ldm_ins_il->lat = mavlink_msg_ldm_ins_il_get_lat(msg);
    ldm_ins_il->lon = mavlink_msg_ldm_ins_il_get_lon(msg);
    ldm_ins_il->alt_amsl = mavlink_msg_ldm_ins_il_get_alt_amsl(msg);
    ldm_ins_il->alt_baro = mavlink_msg_ldm_ins_il_get_alt_baro(msg);
    mavlink_msg_ldm_ins_il_get_eul_rpy(msg, ldm_ins_il->eul_rpy);
    mavlink_msg_ldm_ins_il_get_vel_ned(msg, ldm_ins_il->vel_ned);
    ldm_ins_il->gnss_h_acc = mavlink_msg_ldm_ins_il_get_gnss_h_acc(msg);
    ldm_ins_il->gnss_v_acc = mavlink_msg_ldm_ins_il_get_gnss_v_acc(msg);
    mavlink_msg_ldm_ins_il_get_kf_pos_cov_lla(msg, ldm_ins_il->kf_pos_cov_lla);
    mavlink_msg_ldm_ins_il_get_kf_vel_cov_ned(msg, ldm_ins_il->kf_vel_cov_ned);
    ldm_ins_il->ins_sol_status = mavlink_msg_ldm_ins_il_get_ins_sol_status(msg);
    ldm_ins_il->gnss_n_sat = mavlink_msg_ldm_ins_il_get_gnss_n_sat(msg);
    ldm_ins_il->gnss_fix_type = mavlink_msg_ldm_ins_il_get_gnss_fix_type(msg);
    ldm_ins_il->gnss_pdop = mavlink_msg_ldm_ins_il_get_gnss_pdop(msg);
    ldm_ins_il->temp = mavlink_msg_ldm_ins_il_get_temp(msg);
    ldm_ins_il->validity = mavlink_msg_ldm_ins_il_get_validity(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_INS_IL_LEN? msg->len : MAVLINK_MSG_ID_LDM_INS_IL_LEN;
        memset(ldm_ins_il, 0, MAVLINK_MSG_ID_LDM_INS_IL_LEN);
    memcpy(ldm_ins_il, _MAV_PAYLOAD(msg), len);
#endif
}
