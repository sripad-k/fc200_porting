#pragma once
// MESSAGE LDM_PX4_NAV PACKING

#define MAVLINK_MSG_ID_LDM_PX4_NAV 52005


typedef struct __mavlink_ldm_px4_nav_t {
 uint32_t time_ms; /*<  PX4 internal time when the msg is sent*/
 float eul_rpy[3]; /*<  321 Euler angle rad*/
 float omg[3]; /*<  angular rate rad/s*/
 float acc[3]; /*<  body frame accel measured by accelerometer m/s/s*/
 float vel_ned[3]; /*<  NED vel m/s*/
 int32_t lat; /*<  lat in 10^7 deg*/
 int32_t lon; /*<  lon in 10^7 deg*/
 float alt_amsl; /*<  EKF alt AMSL m*/
 float gnss_h_acc; /*<  GNSS horizontal uncertainty in m*/
 float gnss_v_acc; /*<  GNSS vertical uncertainty in m*/
 float alt_baro; /*<  baro alt m*/
 int16_t aspd1_cas; /*<  airspeed data from sensor 1 cm/s*/
 int16_t aspd2_cas; /*<  airspeed data from sensor 2 cm/s*/
 uint16_t batt_v; /*<  battery voltage in mV*/
 uint8_t gnss_n_sat; /*<  number of visible satellites*/
 uint8_t gnss_fix_type; /*<  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)*/
 uint8_t validity; /*<  bitwise validity for above data*/
} mavlink_ldm_px4_nav_t;

#define MAVLINK_MSG_ID_LDM_PX4_NAV_LEN 85
#define MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN 85
#define MAVLINK_MSG_ID_52005_LEN 85
#define MAVLINK_MSG_ID_52005_MIN_LEN 85

#define MAVLINK_MSG_ID_LDM_PX4_NAV_CRC 123
#define MAVLINK_MSG_ID_52005_CRC 123

#define MAVLINK_MSG_LDM_PX4_NAV_FIELD_EUL_RPY_LEN 3
#define MAVLINK_MSG_LDM_PX4_NAV_FIELD_OMG_LEN 3
#define MAVLINK_MSG_LDM_PX4_NAV_FIELD_ACC_LEN 3
#define MAVLINK_MSG_LDM_PX4_NAV_FIELD_VEL_NED_LEN 3

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_PX4_NAV { \
    52005, \
    "LDM_PX4_NAV", \
    17, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_px4_nav_t, time_ms) }, \
         { "eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_px4_nav_t, eul_rpy) }, \
         { "omg", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_px4_nav_t, omg) }, \
         { "acc", NULL, MAVLINK_TYPE_FLOAT, 3, 28, offsetof(mavlink_ldm_px4_nav_t, acc) }, \
         { "vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 40, offsetof(mavlink_ldm_px4_nav_t, vel_ned) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 52, offsetof(mavlink_ldm_px4_nav_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 56, offsetof(mavlink_ldm_px4_nav_t, lon) }, \
         { "alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_ldm_px4_nav_t, alt_amsl) }, \
         { "aspd1_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_ldm_px4_nav_t, aspd1_cas) }, \
         { "aspd2_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 78, offsetof(mavlink_ldm_px4_nav_t, aspd2_cas) }, \
         { "gnss_n_sat", NULL, MAVLINK_TYPE_UINT8_T, 0, 82, offsetof(mavlink_ldm_px4_nav_t, gnss_n_sat) }, \
         { "gnss_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 83, offsetof(mavlink_ldm_px4_nav_t, gnss_fix_type) }, \
         { "gnss_h_acc", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_ldm_px4_nav_t, gnss_h_acc) }, \
         { "gnss_v_acc", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_ldm_px4_nav_t, gnss_v_acc) }, \
         { "alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_ldm_px4_nav_t, alt_baro) }, \
         { "batt_v", NULL, MAVLINK_TYPE_UINT16_T, 0, 80, offsetof(mavlink_ldm_px4_nav_t, batt_v) }, \
         { "validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 84, offsetof(mavlink_ldm_px4_nav_t, validity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_PX4_NAV { \
    "LDM_PX4_NAV", \
    17, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_px4_nav_t, time_ms) }, \
         { "eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_px4_nav_t, eul_rpy) }, \
         { "omg", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_px4_nav_t, omg) }, \
         { "acc", NULL, MAVLINK_TYPE_FLOAT, 3, 28, offsetof(mavlink_ldm_px4_nav_t, acc) }, \
         { "vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 40, offsetof(mavlink_ldm_px4_nav_t, vel_ned) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 52, offsetof(mavlink_ldm_px4_nav_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 56, offsetof(mavlink_ldm_px4_nav_t, lon) }, \
         { "alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 60, offsetof(mavlink_ldm_px4_nav_t, alt_amsl) }, \
         { "aspd1_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_ldm_px4_nav_t, aspd1_cas) }, \
         { "aspd2_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 78, offsetof(mavlink_ldm_px4_nav_t, aspd2_cas) }, \
         { "gnss_n_sat", NULL, MAVLINK_TYPE_UINT8_T, 0, 82, offsetof(mavlink_ldm_px4_nav_t, gnss_n_sat) }, \
         { "gnss_fix_type", NULL, MAVLINK_TYPE_UINT8_T, 0, 83, offsetof(mavlink_ldm_px4_nav_t, gnss_fix_type) }, \
         { "gnss_h_acc", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_ldm_px4_nav_t, gnss_h_acc) }, \
         { "gnss_v_acc", NULL, MAVLINK_TYPE_FLOAT, 0, 68, offsetof(mavlink_ldm_px4_nav_t, gnss_v_acc) }, \
         { "alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 72, offsetof(mavlink_ldm_px4_nav_t, alt_baro) }, \
         { "batt_v", NULL, MAVLINK_TYPE_UINT16_T, 0, 80, offsetof(mavlink_ldm_px4_nav_t, batt_v) }, \
         { "validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 84, offsetof(mavlink_ldm_px4_nav_t, validity) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_px4_nav message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  PX4 internal time when the msg is sent
 * @param eul_rpy  321 Euler angle rad
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel m/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param aspd1_cas  airspeed data from sensor 1 cm/s
 * @param aspd2_cas  airspeed data from sensor 2 cm/s
 * @param gnss_n_sat  number of visible satellites
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in m
 * @param gnss_v_acc  GNSS vertical uncertainty in m
 * @param alt_baro  baro alt m
 * @param batt_v  battery voltage in mV
 * @param validity  bitwise validity for above data
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const float *eul_rpy, const float *omg, const float *acc, const float *vel_ned, int32_t lat, int32_t lon, float alt_amsl, int16_t aspd1_cas, int16_t aspd2_cas, uint8_t gnss_n_sat, uint8_t gnss_fix_type, float gnss_h_acc, float gnss_v_acc, float alt_baro, uint16_t batt_v, uint8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PX4_NAV_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 52, lat);
    _mav_put_int32_t(buf, 56, lon);
    _mav_put_float(buf, 60, alt_amsl);
    _mav_put_float(buf, 64, gnss_h_acc);
    _mav_put_float(buf, 68, gnss_v_acc);
    _mav_put_float(buf, 72, alt_baro);
    _mav_put_int16_t(buf, 76, aspd1_cas);
    _mav_put_int16_t(buf, 78, aspd2_cas);
    _mav_put_uint16_t(buf, 80, batt_v);
    _mav_put_uint8_t(buf, 82, gnss_n_sat);
    _mav_put_uint8_t(buf, 83, gnss_fix_type);
    _mav_put_uint8_t(buf, 84, validity);
    _mav_put_float_array(buf, 4, eul_rpy, 3);
    _mav_put_float_array(buf, 16, omg, 3);
    _mav_put_float_array(buf, 28, acc, 3);
    _mav_put_float_array(buf, 40, vel_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#else
    mavlink_ldm_px4_nav_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.alt_baro = alt_baro;
    packet.aspd1_cas = aspd1_cas;
    packet.aspd2_cas = aspd2_cas;
    packet.batt_v = batt_v;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.validity = validity;
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PX4_NAV;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
}

/**
 * @brief Pack a ldm_px4_nav message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  PX4 internal time when the msg is sent
 * @param eul_rpy  321 Euler angle rad
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel m/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param aspd1_cas  airspeed data from sensor 1 cm/s
 * @param aspd2_cas  airspeed data from sensor 2 cm/s
 * @param gnss_n_sat  number of visible satellites
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in m
 * @param gnss_v_acc  GNSS vertical uncertainty in m
 * @param alt_baro  baro alt m
 * @param batt_v  battery voltage in mV
 * @param validity  bitwise validity for above data
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const float *eul_rpy, const float *omg, const float *acc, const float *vel_ned, int32_t lat, int32_t lon, float alt_amsl, int16_t aspd1_cas, int16_t aspd2_cas, uint8_t gnss_n_sat, uint8_t gnss_fix_type, float gnss_h_acc, float gnss_v_acc, float alt_baro, uint16_t batt_v, uint8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PX4_NAV_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 52, lat);
    _mav_put_int32_t(buf, 56, lon);
    _mav_put_float(buf, 60, alt_amsl);
    _mav_put_float(buf, 64, gnss_h_acc);
    _mav_put_float(buf, 68, gnss_v_acc);
    _mav_put_float(buf, 72, alt_baro);
    _mav_put_int16_t(buf, 76, aspd1_cas);
    _mav_put_int16_t(buf, 78, aspd2_cas);
    _mav_put_uint16_t(buf, 80, batt_v);
    _mav_put_uint8_t(buf, 82, gnss_n_sat);
    _mav_put_uint8_t(buf, 83, gnss_fix_type);
    _mav_put_uint8_t(buf, 84, validity);
    _mav_put_float_array(buf, 4, eul_rpy, 3);
    _mav_put_float_array(buf, 16, omg, 3);
    _mav_put_float_array(buf, 28, acc, 3);
    _mav_put_float_array(buf, 40, vel_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#else
    mavlink_ldm_px4_nav_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.alt_baro = alt_baro;
    packet.aspd1_cas = aspd1_cas;
    packet.aspd2_cas = aspd2_cas;
    packet.batt_v = batt_v;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.validity = validity;
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PX4_NAV;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#endif
}

/**
 * @brief Pack a ldm_px4_nav message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  PX4 internal time when the msg is sent
 * @param eul_rpy  321 Euler angle rad
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel m/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param aspd1_cas  airspeed data from sensor 1 cm/s
 * @param aspd2_cas  airspeed data from sensor 2 cm/s
 * @param gnss_n_sat  number of visible satellites
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in m
 * @param gnss_v_acc  GNSS vertical uncertainty in m
 * @param alt_baro  baro alt m
 * @param batt_v  battery voltage in mV
 * @param validity  bitwise validity for above data
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const float *eul_rpy,const float *omg,const float *acc,const float *vel_ned,int32_t lat,int32_t lon,float alt_amsl,int16_t aspd1_cas,int16_t aspd2_cas,uint8_t gnss_n_sat,uint8_t gnss_fix_type,float gnss_h_acc,float gnss_v_acc,float alt_baro,uint16_t batt_v,uint8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PX4_NAV_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 52, lat);
    _mav_put_int32_t(buf, 56, lon);
    _mav_put_float(buf, 60, alt_amsl);
    _mav_put_float(buf, 64, gnss_h_acc);
    _mav_put_float(buf, 68, gnss_v_acc);
    _mav_put_float(buf, 72, alt_baro);
    _mav_put_int16_t(buf, 76, aspd1_cas);
    _mav_put_int16_t(buf, 78, aspd2_cas);
    _mav_put_uint16_t(buf, 80, batt_v);
    _mav_put_uint8_t(buf, 82, gnss_n_sat);
    _mav_put_uint8_t(buf, 83, gnss_fix_type);
    _mav_put_uint8_t(buf, 84, validity);
    _mav_put_float_array(buf, 4, eul_rpy, 3);
    _mav_put_float_array(buf, 16, omg, 3);
    _mav_put_float_array(buf, 28, acc, 3);
    _mav_put_float_array(buf, 40, vel_ned, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#else
    mavlink_ldm_px4_nav_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.alt_baro = alt_baro;
    packet.aspd1_cas = aspd1_cas;
    packet.aspd2_cas = aspd2_cas;
    packet.batt_v = batt_v;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.validity = validity;
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PX4_NAV;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
}

/**
 * @brief Encode a ldm_px4_nav struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_px4_nav C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_px4_nav_t* ldm_px4_nav)
{
    return mavlink_msg_ldm_px4_nav_pack(system_id, component_id, msg, ldm_px4_nav->time_ms, ldm_px4_nav->eul_rpy, ldm_px4_nav->omg, ldm_px4_nav->acc, ldm_px4_nav->vel_ned, ldm_px4_nav->lat, ldm_px4_nav->lon, ldm_px4_nav->alt_amsl, ldm_px4_nav->aspd1_cas, ldm_px4_nav->aspd2_cas, ldm_px4_nav->gnss_n_sat, ldm_px4_nav->gnss_fix_type, ldm_px4_nav->gnss_h_acc, ldm_px4_nav->gnss_v_acc, ldm_px4_nav->alt_baro, ldm_px4_nav->batt_v, ldm_px4_nav->validity);
}

/**
 * @brief Encode a ldm_px4_nav struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_px4_nav C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_px4_nav_t* ldm_px4_nav)
{
    return mavlink_msg_ldm_px4_nav_pack_chan(system_id, component_id, chan, msg, ldm_px4_nav->time_ms, ldm_px4_nav->eul_rpy, ldm_px4_nav->omg, ldm_px4_nav->acc, ldm_px4_nav->vel_ned, ldm_px4_nav->lat, ldm_px4_nav->lon, ldm_px4_nav->alt_amsl, ldm_px4_nav->aspd1_cas, ldm_px4_nav->aspd2_cas, ldm_px4_nav->gnss_n_sat, ldm_px4_nav->gnss_fix_type, ldm_px4_nav->gnss_h_acc, ldm_px4_nav->gnss_v_acc, ldm_px4_nav->alt_baro, ldm_px4_nav->batt_v, ldm_px4_nav->validity);
}

/**
 * @brief Encode a ldm_px4_nav struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_px4_nav C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_px4_nav_t* ldm_px4_nav)
{
    return mavlink_msg_ldm_px4_nav_pack_status(system_id, component_id, _status, msg,  ldm_px4_nav->time_ms, ldm_px4_nav->eul_rpy, ldm_px4_nav->omg, ldm_px4_nav->acc, ldm_px4_nav->vel_ned, ldm_px4_nav->lat, ldm_px4_nav->lon, ldm_px4_nav->alt_amsl, ldm_px4_nav->aspd1_cas, ldm_px4_nav->aspd2_cas, ldm_px4_nav->gnss_n_sat, ldm_px4_nav->gnss_fix_type, ldm_px4_nav->gnss_h_acc, ldm_px4_nav->gnss_v_acc, ldm_px4_nav->alt_baro, ldm_px4_nav->batt_v, ldm_px4_nav->validity);
}

/**
 * @brief Send a ldm_px4_nav message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  PX4 internal time when the msg is sent
 * @param eul_rpy  321 Euler angle rad
 * @param omg  angular rate rad/s
 * @param acc  body frame accel measured by accelerometer m/s/s
 * @param vel_ned  NED vel m/s
 * @param lat  lat in 10^7 deg
 * @param lon  lon in 10^7 deg
 * @param alt_amsl  EKF alt AMSL m
 * @param aspd1_cas  airspeed data from sensor 1 cm/s
 * @param aspd2_cas  airspeed data from sensor 2 cm/s
 * @param gnss_n_sat  number of visible satellites
 * @param gnss_fix_type  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 * @param gnss_h_acc  GNSS horizontal uncertainty in m
 * @param gnss_v_acc  GNSS vertical uncertainty in m
 * @param alt_baro  baro alt m
 * @param batt_v  battery voltage in mV
 * @param validity  bitwise validity for above data
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_px4_nav_send(mavlink_channel_t chan, uint32_t time_ms, const float *eul_rpy, const float *omg, const float *acc, const float *vel_ned, int32_t lat, int32_t lon, float alt_amsl, int16_t aspd1_cas, int16_t aspd2_cas, uint8_t gnss_n_sat, uint8_t gnss_fix_type, float gnss_h_acc, float gnss_v_acc, float alt_baro, uint16_t batt_v, uint8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PX4_NAV_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 52, lat);
    _mav_put_int32_t(buf, 56, lon);
    _mav_put_float(buf, 60, alt_amsl);
    _mav_put_float(buf, 64, gnss_h_acc);
    _mav_put_float(buf, 68, gnss_v_acc);
    _mav_put_float(buf, 72, alt_baro);
    _mav_put_int16_t(buf, 76, aspd1_cas);
    _mav_put_int16_t(buf, 78, aspd2_cas);
    _mav_put_uint16_t(buf, 80, batt_v);
    _mav_put_uint8_t(buf, 82, gnss_n_sat);
    _mav_put_uint8_t(buf, 83, gnss_fix_type);
    _mav_put_uint8_t(buf, 84, validity);
    _mav_put_float_array(buf, 4, eul_rpy, 3);
    _mav_put_float_array(buf, 16, omg, 3);
    _mav_put_float_array(buf, 28, acc, 3);
    _mav_put_float_array(buf, 40, vel_ned, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PX4_NAV, buf, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#else
    mavlink_ldm_px4_nav_t packet;
    packet.time_ms = time_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_amsl = alt_amsl;
    packet.gnss_h_acc = gnss_h_acc;
    packet.gnss_v_acc = gnss_v_acc;
    packet.alt_baro = alt_baro;
    packet.aspd1_cas = aspd1_cas;
    packet.aspd2_cas = aspd2_cas;
    packet.batt_v = batt_v;
    packet.gnss_n_sat = gnss_n_sat;
    packet.gnss_fix_type = gnss_fix_type;
    packet.validity = validity;
    mav_array_memcpy(packet.eul_rpy, eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet.acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet.vel_ned, vel_ned, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PX4_NAV, (const char *)&packet, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#endif
}

/**
 * @brief Send a ldm_px4_nav message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_px4_nav_send_struct(mavlink_channel_t chan, const mavlink_ldm_px4_nav_t* ldm_px4_nav)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_px4_nav_send(chan, ldm_px4_nav->time_ms, ldm_px4_nav->eul_rpy, ldm_px4_nav->omg, ldm_px4_nav->acc, ldm_px4_nav->vel_ned, ldm_px4_nav->lat, ldm_px4_nav->lon, ldm_px4_nav->alt_amsl, ldm_px4_nav->aspd1_cas, ldm_px4_nav->aspd2_cas, ldm_px4_nav->gnss_n_sat, ldm_px4_nav->gnss_fix_type, ldm_px4_nav->gnss_h_acc, ldm_px4_nav->gnss_v_acc, ldm_px4_nav->alt_baro, ldm_px4_nav->batt_v, ldm_px4_nav->validity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PX4_NAV, (const char *)ldm_px4_nav, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_PX4_NAV_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_px4_nav_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const float *eul_rpy, const float *omg, const float *acc, const float *vel_ned, int32_t lat, int32_t lon, float alt_amsl, int16_t aspd1_cas, int16_t aspd2_cas, uint8_t gnss_n_sat, uint8_t gnss_fix_type, float gnss_h_acc, float gnss_v_acc, float alt_baro, uint16_t batt_v, uint8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 52, lat);
    _mav_put_int32_t(buf, 56, lon);
    _mav_put_float(buf, 60, alt_amsl);
    _mav_put_float(buf, 64, gnss_h_acc);
    _mav_put_float(buf, 68, gnss_v_acc);
    _mav_put_float(buf, 72, alt_baro);
    _mav_put_int16_t(buf, 76, aspd1_cas);
    _mav_put_int16_t(buf, 78, aspd2_cas);
    _mav_put_uint16_t(buf, 80, batt_v);
    _mav_put_uint8_t(buf, 82, gnss_n_sat);
    _mav_put_uint8_t(buf, 83, gnss_fix_type);
    _mav_put_uint8_t(buf, 84, validity);
    _mav_put_float_array(buf, 4, eul_rpy, 3);
    _mav_put_float_array(buf, 16, omg, 3);
    _mav_put_float_array(buf, 28, acc, 3);
    _mav_put_float_array(buf, 40, vel_ned, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PX4_NAV, buf, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#else
    mavlink_ldm_px4_nav_t *packet = (mavlink_ldm_px4_nav_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->lat = lat;
    packet->lon = lon;
    packet->alt_amsl = alt_amsl;
    packet->gnss_h_acc = gnss_h_acc;
    packet->gnss_v_acc = gnss_v_acc;
    packet->alt_baro = alt_baro;
    packet->aspd1_cas = aspd1_cas;
    packet->aspd2_cas = aspd2_cas;
    packet->batt_v = batt_v;
    packet->gnss_n_sat = gnss_n_sat;
    packet->gnss_fix_type = gnss_fix_type;
    packet->validity = validity;
    mav_array_memcpy(packet->eul_rpy, eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet->omg, omg, sizeof(float)*3);
    mav_array_memcpy(packet->acc, acc, sizeof(float)*3);
    mav_array_memcpy(packet->vel_ned, vel_ned, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PX4_NAV, (const char *)packet, MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN, MAVLINK_MSG_ID_LDM_PX4_NAV_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_PX4_NAV UNPACKING


/**
 * @brief Get field time_ms from ldm_px4_nav message
 *
 * @return  PX4 internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_px4_nav_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field eul_rpy from ldm_px4_nav message
 *
 * @return  321 Euler angle rad
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_get_eul_rpy(const mavlink_message_t* msg, float *eul_rpy)
{
    return _MAV_RETURN_float_array(msg, eul_rpy, 3,  4);
}

/**
 * @brief Get field omg from ldm_px4_nav message
 *
 * @return  angular rate rad/s
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_get_omg(const mavlink_message_t* msg, float *omg)
{
    return _MAV_RETURN_float_array(msg, omg, 3,  16);
}

/**
 * @brief Get field acc from ldm_px4_nav message
 *
 * @return  body frame accel measured by accelerometer m/s/s
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_get_acc(const mavlink_message_t* msg, float *acc)
{
    return _MAV_RETURN_float_array(msg, acc, 3,  28);
}

/**
 * @brief Get field vel_ned from ldm_px4_nav message
 *
 * @return  NED vel m/s
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_get_vel_ned(const mavlink_message_t* msg, float *vel_ned)
{
    return _MAV_RETURN_float_array(msg, vel_ned, 3,  40);
}

/**
 * @brief Get field lat from ldm_px4_nav message
 *
 * @return  lat in 10^7 deg
 */
static inline int32_t mavlink_msg_ldm_px4_nav_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  52);
}

/**
 * @brief Get field lon from ldm_px4_nav message
 *
 * @return  lon in 10^7 deg
 */
static inline int32_t mavlink_msg_ldm_px4_nav_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  56);
}

/**
 * @brief Get field alt_amsl from ldm_px4_nav message
 *
 * @return  EKF alt AMSL m
 */
static inline float mavlink_msg_ldm_px4_nav_get_alt_amsl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  60);
}

/**
 * @brief Get field aspd1_cas from ldm_px4_nav message
 *
 * @return  airspeed data from sensor 1 cm/s
 */
static inline int16_t mavlink_msg_ldm_px4_nav_get_aspd1_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  76);
}

/**
 * @brief Get field aspd2_cas from ldm_px4_nav message
 *
 * @return  airspeed data from sensor 2 cm/s
 */
static inline int16_t mavlink_msg_ldm_px4_nav_get_aspd2_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  78);
}

/**
 * @brief Get field gnss_n_sat from ldm_px4_nav message
 *
 * @return  number of visible satellites
 */
static inline uint8_t mavlink_msg_ldm_px4_nav_get_gnss_n_sat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  82);
}

/**
 * @brief Get field gnss_fix_type from ldm_px4_nav message
 *
 * @return  GNSS fix type (0: no fix, 1: 2D fix, 2: 3D fix, 3: RTK float, 4: RTK fixed)
 */
static inline uint8_t mavlink_msg_ldm_px4_nav_get_gnss_fix_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  83);
}

/**
 * @brief Get field gnss_h_acc from ldm_px4_nav message
 *
 * @return  GNSS horizontal uncertainty in m
 */
static inline float mavlink_msg_ldm_px4_nav_get_gnss_h_acc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  64);
}

/**
 * @brief Get field gnss_v_acc from ldm_px4_nav message
 *
 * @return  GNSS vertical uncertainty in m
 */
static inline float mavlink_msg_ldm_px4_nav_get_gnss_v_acc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  68);
}

/**
 * @brief Get field alt_baro from ldm_px4_nav message
 *
 * @return  baro alt m
 */
static inline float mavlink_msg_ldm_px4_nav_get_alt_baro(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  72);
}

/**
 * @brief Get field batt_v from ldm_px4_nav message
 *
 * @return  battery voltage in mV
 */
static inline uint16_t mavlink_msg_ldm_px4_nav_get_batt_v(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  80);
}

/**
 * @brief Get field validity from ldm_px4_nav message
 *
 * @return  bitwise validity for above data
 */
static inline uint8_t mavlink_msg_ldm_px4_nav_get_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  84);
}

/**
 * @brief Decode a ldm_px4_nav message into a struct
 *
 * @param msg The message to decode
 * @param ldm_px4_nav C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_px4_nav_decode(const mavlink_message_t* msg, mavlink_ldm_px4_nav_t* ldm_px4_nav)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_px4_nav->time_ms = mavlink_msg_ldm_px4_nav_get_time_ms(msg);
    mavlink_msg_ldm_px4_nav_get_eul_rpy(msg, ldm_px4_nav->eul_rpy);
    mavlink_msg_ldm_px4_nav_get_omg(msg, ldm_px4_nav->omg);
    mavlink_msg_ldm_px4_nav_get_acc(msg, ldm_px4_nav->acc);
    mavlink_msg_ldm_px4_nav_get_vel_ned(msg, ldm_px4_nav->vel_ned);
    ldm_px4_nav->lat = mavlink_msg_ldm_px4_nav_get_lat(msg);
    ldm_px4_nav->lon = mavlink_msg_ldm_px4_nav_get_lon(msg);
    ldm_px4_nav->alt_amsl = mavlink_msg_ldm_px4_nav_get_alt_amsl(msg);
    ldm_px4_nav->gnss_h_acc = mavlink_msg_ldm_px4_nav_get_gnss_h_acc(msg);
    ldm_px4_nav->gnss_v_acc = mavlink_msg_ldm_px4_nav_get_gnss_v_acc(msg);
    ldm_px4_nav->alt_baro = mavlink_msg_ldm_px4_nav_get_alt_baro(msg);
    ldm_px4_nav->aspd1_cas = mavlink_msg_ldm_px4_nav_get_aspd1_cas(msg);
    ldm_px4_nav->aspd2_cas = mavlink_msg_ldm_px4_nav_get_aspd2_cas(msg);
    ldm_px4_nav->batt_v = mavlink_msg_ldm_px4_nav_get_batt_v(msg);
    ldm_px4_nav->gnss_n_sat = mavlink_msg_ldm_px4_nav_get_gnss_n_sat(msg);
    ldm_px4_nav->gnss_fix_type = mavlink_msg_ldm_px4_nav_get_gnss_fix_type(msg);
    ldm_px4_nav->validity = mavlink_msg_ldm_px4_nav_get_validity(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_PX4_NAV_LEN? msg->len : MAVLINK_MSG_ID_LDM_PX4_NAV_LEN;
        memset(ldm_px4_nav, 0, MAVLINK_MSG_ID_LDM_PX4_NAV_LEN);
    memcpy(ldm_px4_nav, _MAV_PAYLOAD(msg), len);
#endif
}
