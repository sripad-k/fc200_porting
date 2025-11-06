#pragma once
// MESSAGE LDM_PIL_SENSOR PACKING

#define MAVLINK_MSG_ID_LDM_PIL_SENSOR 52010


typedef struct __mavlink_ldm_pil_sensor_t {
 uint32_t time_ms; /*< [ms] Simulator timestamp when the msg was sent.*/
 float ins1_eul_rpy[3]; /*< [rad] attitude expressed 321 Euler angle*/
 float ins1_omg[3]; /*< [rad/s] Body frame angular speed*/
 float ins1_acc[3]; /*< [rad/s] Body frame acceleration*/
 int32_t ins1_lat; /*< [degE7] Latitude*/
 int32_t ins1_lon; /*< [degE7] Longitude*/
 float ins1_alt_amsl; /*< [m] Altitude gps AMSL*/
 float ins1_vel_ned[3]; /*< [m/s] vel NED*/
 float ins2_eul_rpy[3]; /*< [rad] attitude expressed 321 Euler angle*/
 float ins2_omg[3]; /*< [rad/s] Body frame angular speed*/
 float ins2_acc[3]; /*< [rad/s] Body frame acceleration*/
 int32_t ins2_lat; /*< [degE7] Latitude*/
 int32_t ins2_lon; /*< [degE7] Longitude*/
 float ins2_alt_amsl; /*< [m] Altitude gps AMSL*/
 float ins2_vel_ned[3]; /*< [m/s] vel NED*/
 float adc1_cas; /*< [m/s] Indicated airspeed*/
 float adc1_alt_baro; /*< [m] Barometric altitude*/
 float adc1_aoa; /*< [rad] Angle of attack*/
 float adc1_aos; /*< [rad] Angle of sideslip*/
 float adc2_cas; /*< [m/s] Indicated airspeed*/
 float adc2_alt_baro; /*< [m] Barometric altitude*/
 float adc2_aoa; /*< [rad] Angle of attack*/
 float adc2_aos; /*< [rad] Angle of sideslip*/
 float radalt; /*< [m] Radalt altitde*/
 int16_t rotor_rpm[8]; /*< [RPM] rotor rpm as reported by EPU*/
 int16_t pusher_rpm; /*< [RPM] pusher rpm as reported by engine*/
 int16_t servo_def[12]; /*< [degE2] actuator deflection reported by servo +/- 180 deg range*/
 int8_t ins1_validity; /*<  bitfield with INS1 validity flags*/
 int8_t ins2_validity; /*<  bitfield with INS2 validity flags*/
 int8_t adc1_validity; /*<  bitfield with ADC1 validity flags*/
 int8_t adc2_validity; /*<  bitfield with ADC2 validity flags*/
 int8_t wow; /*< [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3*/
} mavlink_ldm_pil_sensor_t;

#define MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN 207
#define MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN 207
#define MAVLINK_MSG_ID_52010_LEN 207
#define MAVLINK_MSG_ID_52010_MIN_LEN 207

#define MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC 253
#define MAVLINK_MSG_ID_52010_CRC 253

#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS1_EUL_RPY_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS1_OMG_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS1_ACC_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS1_VEL_NED_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS2_EUL_RPY_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS2_OMG_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS2_ACC_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_INS2_VEL_NED_LEN 3
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_ROTOR_RPM_LEN 8
#define MAVLINK_MSG_LDM_PIL_SENSOR_FIELD_SERVO_DEF_LEN 12

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_PIL_SENSOR { \
    52010, \
    "LDM_PIL_SENSOR", \
    32, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pil_sensor_t, time_ms) }, \
         { "ins1_eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_pil_sensor_t, ins1_eul_rpy) }, \
         { "ins1_omg", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_pil_sensor_t, ins1_omg) }, \
         { "ins1_acc", NULL, MAVLINK_TYPE_FLOAT, 3, 28, offsetof(mavlink_ldm_pil_sensor_t, ins1_acc) }, \
         { "ins1_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_ldm_pil_sensor_t, ins1_lat) }, \
         { "ins1_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_ldm_pil_sensor_t, ins1_lon) }, \
         { "ins1_alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_ldm_pil_sensor_t, ins1_alt_amsl) }, \
         { "ins1_vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 52, offsetof(mavlink_ldm_pil_sensor_t, ins1_vel_ned) }, \
         { "ins1_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 202, offsetof(mavlink_ldm_pil_sensor_t, ins1_validity) }, \
         { "ins2_eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 64, offsetof(mavlink_ldm_pil_sensor_t, ins2_eul_rpy) }, \
         { "ins2_omg", NULL, MAVLINK_TYPE_FLOAT, 3, 76, offsetof(mavlink_ldm_pil_sensor_t, ins2_omg) }, \
         { "ins2_acc", NULL, MAVLINK_TYPE_FLOAT, 3, 88, offsetof(mavlink_ldm_pil_sensor_t, ins2_acc) }, \
         { "ins2_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 100, offsetof(mavlink_ldm_pil_sensor_t, ins2_lat) }, \
         { "ins2_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 104, offsetof(mavlink_ldm_pil_sensor_t, ins2_lon) }, \
         { "ins2_alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 108, offsetof(mavlink_ldm_pil_sensor_t, ins2_alt_amsl) }, \
         { "ins2_vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 112, offsetof(mavlink_ldm_pil_sensor_t, ins2_vel_ned) }, \
         { "ins2_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 203, offsetof(mavlink_ldm_pil_sensor_t, ins2_validity) }, \
         { "adc1_cas", NULL, MAVLINK_TYPE_FLOAT, 0, 124, offsetof(mavlink_ldm_pil_sensor_t, adc1_cas) }, \
         { "adc1_alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 128, offsetof(mavlink_ldm_pil_sensor_t, adc1_alt_baro) }, \
         { "adc1_aoa", NULL, MAVLINK_TYPE_FLOAT, 0, 132, offsetof(mavlink_ldm_pil_sensor_t, adc1_aoa) }, \
         { "adc1_aos", NULL, MAVLINK_TYPE_FLOAT, 0, 136, offsetof(mavlink_ldm_pil_sensor_t, adc1_aos) }, \
         { "adc1_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 204, offsetof(mavlink_ldm_pil_sensor_t, adc1_validity) }, \
         { "adc2_cas", NULL, MAVLINK_TYPE_FLOAT, 0, 140, offsetof(mavlink_ldm_pil_sensor_t, adc2_cas) }, \
         { "adc2_alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 144, offsetof(mavlink_ldm_pil_sensor_t, adc2_alt_baro) }, \
         { "adc2_aoa", NULL, MAVLINK_TYPE_FLOAT, 0, 148, offsetof(mavlink_ldm_pil_sensor_t, adc2_aoa) }, \
         { "adc2_aos", NULL, MAVLINK_TYPE_FLOAT, 0, 152, offsetof(mavlink_ldm_pil_sensor_t, adc2_aos) }, \
         { "adc2_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 205, offsetof(mavlink_ldm_pil_sensor_t, adc2_validity) }, \
         { "radalt", NULL, MAVLINK_TYPE_FLOAT, 0, 156, offsetof(mavlink_ldm_pil_sensor_t, radalt) }, \
         { "wow", NULL, MAVLINK_TYPE_INT8_T, 0, 206, offsetof(mavlink_ldm_pil_sensor_t, wow) }, \
         { "rotor_rpm", NULL, MAVLINK_TYPE_INT16_T, 8, 160, offsetof(mavlink_ldm_pil_sensor_t, rotor_rpm) }, \
         { "pusher_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 176, offsetof(mavlink_ldm_pil_sensor_t, pusher_rpm) }, \
         { "servo_def", NULL, MAVLINK_TYPE_INT16_T, 12, 178, offsetof(mavlink_ldm_pil_sensor_t, servo_def) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_PIL_SENSOR { \
    "LDM_PIL_SENSOR", \
    32, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pil_sensor_t, time_ms) }, \
         { "ins1_eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 4, offsetof(mavlink_ldm_pil_sensor_t, ins1_eul_rpy) }, \
         { "ins1_omg", NULL, MAVLINK_TYPE_FLOAT, 3, 16, offsetof(mavlink_ldm_pil_sensor_t, ins1_omg) }, \
         { "ins1_acc", NULL, MAVLINK_TYPE_FLOAT, 3, 28, offsetof(mavlink_ldm_pil_sensor_t, ins1_acc) }, \
         { "ins1_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_ldm_pil_sensor_t, ins1_lat) }, \
         { "ins1_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_ldm_pil_sensor_t, ins1_lon) }, \
         { "ins1_alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 48, offsetof(mavlink_ldm_pil_sensor_t, ins1_alt_amsl) }, \
         { "ins1_vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 52, offsetof(mavlink_ldm_pil_sensor_t, ins1_vel_ned) }, \
         { "ins1_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 202, offsetof(mavlink_ldm_pil_sensor_t, ins1_validity) }, \
         { "ins2_eul_rpy", NULL, MAVLINK_TYPE_FLOAT, 3, 64, offsetof(mavlink_ldm_pil_sensor_t, ins2_eul_rpy) }, \
         { "ins2_omg", NULL, MAVLINK_TYPE_FLOAT, 3, 76, offsetof(mavlink_ldm_pil_sensor_t, ins2_omg) }, \
         { "ins2_acc", NULL, MAVLINK_TYPE_FLOAT, 3, 88, offsetof(mavlink_ldm_pil_sensor_t, ins2_acc) }, \
         { "ins2_lat", NULL, MAVLINK_TYPE_INT32_T, 0, 100, offsetof(mavlink_ldm_pil_sensor_t, ins2_lat) }, \
         { "ins2_lon", NULL, MAVLINK_TYPE_INT32_T, 0, 104, offsetof(mavlink_ldm_pil_sensor_t, ins2_lon) }, \
         { "ins2_alt_amsl", NULL, MAVLINK_TYPE_FLOAT, 0, 108, offsetof(mavlink_ldm_pil_sensor_t, ins2_alt_amsl) }, \
         { "ins2_vel_ned", NULL, MAVLINK_TYPE_FLOAT, 3, 112, offsetof(mavlink_ldm_pil_sensor_t, ins2_vel_ned) }, \
         { "ins2_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 203, offsetof(mavlink_ldm_pil_sensor_t, ins2_validity) }, \
         { "adc1_cas", NULL, MAVLINK_TYPE_FLOAT, 0, 124, offsetof(mavlink_ldm_pil_sensor_t, adc1_cas) }, \
         { "adc1_alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 128, offsetof(mavlink_ldm_pil_sensor_t, adc1_alt_baro) }, \
         { "adc1_aoa", NULL, MAVLINK_TYPE_FLOAT, 0, 132, offsetof(mavlink_ldm_pil_sensor_t, adc1_aoa) }, \
         { "adc1_aos", NULL, MAVLINK_TYPE_FLOAT, 0, 136, offsetof(mavlink_ldm_pil_sensor_t, adc1_aos) }, \
         { "adc1_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 204, offsetof(mavlink_ldm_pil_sensor_t, adc1_validity) }, \
         { "adc2_cas", NULL, MAVLINK_TYPE_FLOAT, 0, 140, offsetof(mavlink_ldm_pil_sensor_t, adc2_cas) }, \
         { "adc2_alt_baro", NULL, MAVLINK_TYPE_FLOAT, 0, 144, offsetof(mavlink_ldm_pil_sensor_t, adc2_alt_baro) }, \
         { "adc2_aoa", NULL, MAVLINK_TYPE_FLOAT, 0, 148, offsetof(mavlink_ldm_pil_sensor_t, adc2_aoa) }, \
         { "adc2_aos", NULL, MAVLINK_TYPE_FLOAT, 0, 152, offsetof(mavlink_ldm_pil_sensor_t, adc2_aos) }, \
         { "adc2_validity", NULL, MAVLINK_TYPE_INT8_T, 0, 205, offsetof(mavlink_ldm_pil_sensor_t, adc2_validity) }, \
         { "radalt", NULL, MAVLINK_TYPE_FLOAT, 0, 156, offsetof(mavlink_ldm_pil_sensor_t, radalt) }, \
         { "wow", NULL, MAVLINK_TYPE_INT8_T, 0, 206, offsetof(mavlink_ldm_pil_sensor_t, wow) }, \
         { "rotor_rpm", NULL, MAVLINK_TYPE_INT16_T, 8, 160, offsetof(mavlink_ldm_pil_sensor_t, rotor_rpm) }, \
         { "pusher_rpm", NULL, MAVLINK_TYPE_INT16_T, 0, 176, offsetof(mavlink_ldm_pil_sensor_t, pusher_rpm) }, \
         { "servo_def", NULL, MAVLINK_TYPE_INT16_T, 12, 178, offsetof(mavlink_ldm_pil_sensor_t, servo_def) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_pil_sensor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] Simulator timestamp when the msg was sent.
 * @param ins1_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins1_omg [rad/s] Body frame angular speed
 * @param ins1_acc [rad/s] Body frame acceleration
 * @param ins1_lat [degE7] Latitude
 * @param ins1_lon [degE7] Longitude
 * @param ins1_alt_amsl [m] Altitude gps AMSL
 * @param ins1_vel_ned [m/s] vel NED
 * @param ins1_validity  bitfield with INS1 validity flags
 * @param ins2_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins2_omg [rad/s] Body frame angular speed
 * @param ins2_acc [rad/s] Body frame acceleration
 * @param ins2_lat [degE7] Latitude
 * @param ins2_lon [degE7] Longitude
 * @param ins2_alt_amsl [m] Altitude gps AMSL
 * @param ins2_vel_ned [m/s] vel NED
 * @param ins2_validity  bitfield with INS2 validity flags
 * @param adc1_cas [m/s] Indicated airspeed
 * @param adc1_alt_baro [m] Barometric altitude
 * @param adc1_aoa [rad] Angle of attack
 * @param adc1_aos [rad] Angle of sideslip
 * @param adc1_validity  bitfield with ADC1 validity flags
 * @param adc2_cas [m/s] Indicated airspeed
 * @param adc2_alt_baro [m] Barometric altitude
 * @param adc2_aoa [rad] Angle of attack
 * @param adc2_aos [rad] Angle of sideslip
 * @param adc2_validity  bitfield with ADC2 validity flags
 * @param radalt [m] Radalt altitde
 * @param wow [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3
 * @param rotor_rpm [RPM] rotor rpm as reported by EPU
 * @param pusher_rpm [RPM] pusher rpm as reported by engine
 * @param servo_def [degE2] actuator deflection reported by servo +/- 180 deg range
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const float *ins1_eul_rpy, const float *ins1_omg, const float *ins1_acc, int32_t ins1_lat, int32_t ins1_lon, float ins1_alt_amsl, const float *ins1_vel_ned, int8_t ins1_validity, const float *ins2_eul_rpy, const float *ins2_omg, const float *ins2_acc, int32_t ins2_lat, int32_t ins2_lon, float ins2_alt_amsl, const float *ins2_vel_ned, int8_t ins2_validity, float adc1_cas, float adc1_alt_baro, float adc1_aoa, float adc1_aos, int8_t adc1_validity, float adc2_cas, float adc2_alt_baro, float adc2_aoa, float adc2_aos, int8_t adc2_validity, float radalt, int8_t wow, const int16_t *rotor_rpm, int16_t pusher_rpm, const int16_t *servo_def)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 40, ins1_lat);
    _mav_put_int32_t(buf, 44, ins1_lon);
    _mav_put_float(buf, 48, ins1_alt_amsl);
    _mav_put_int32_t(buf, 100, ins2_lat);
    _mav_put_int32_t(buf, 104, ins2_lon);
    _mav_put_float(buf, 108, ins2_alt_amsl);
    _mav_put_float(buf, 124, adc1_cas);
    _mav_put_float(buf, 128, adc1_alt_baro);
    _mav_put_float(buf, 132, adc1_aoa);
    _mav_put_float(buf, 136, adc1_aos);
    _mav_put_float(buf, 140, adc2_cas);
    _mav_put_float(buf, 144, adc2_alt_baro);
    _mav_put_float(buf, 148, adc2_aoa);
    _mav_put_float(buf, 152, adc2_aos);
    _mav_put_float(buf, 156, radalt);
    _mav_put_int16_t(buf, 176, pusher_rpm);
    _mav_put_int8_t(buf, 202, ins1_validity);
    _mav_put_int8_t(buf, 203, ins2_validity);
    _mav_put_int8_t(buf, 204, adc1_validity);
    _mav_put_int8_t(buf, 205, adc2_validity);
    _mav_put_int8_t(buf, 206, wow);
    _mav_put_float_array(buf, 4, ins1_eul_rpy, 3);
    _mav_put_float_array(buf, 16, ins1_omg, 3);
    _mav_put_float_array(buf, 28, ins1_acc, 3);
    _mav_put_float_array(buf, 52, ins1_vel_ned, 3);
    _mav_put_float_array(buf, 64, ins2_eul_rpy, 3);
    _mav_put_float_array(buf, 76, ins2_omg, 3);
    _mav_put_float_array(buf, 88, ins2_acc, 3);
    _mav_put_float_array(buf, 112, ins2_vel_ned, 3);
    _mav_put_int16_t_array(buf, 160, rotor_rpm, 8);
    _mav_put_int16_t_array(buf, 178, servo_def, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#else
    mavlink_ldm_pil_sensor_t packet;
    packet.time_ms = time_ms;
    packet.ins1_lat = ins1_lat;
    packet.ins1_lon = ins1_lon;
    packet.ins1_alt_amsl = ins1_alt_amsl;
    packet.ins2_lat = ins2_lat;
    packet.ins2_lon = ins2_lon;
    packet.ins2_alt_amsl = ins2_alt_amsl;
    packet.adc1_cas = adc1_cas;
    packet.adc1_alt_baro = adc1_alt_baro;
    packet.adc1_aoa = adc1_aoa;
    packet.adc1_aos = adc1_aos;
    packet.adc2_cas = adc2_cas;
    packet.adc2_alt_baro = adc2_alt_baro;
    packet.adc2_aoa = adc2_aoa;
    packet.adc2_aos = adc2_aos;
    packet.radalt = radalt;
    packet.pusher_rpm = pusher_rpm;
    packet.ins1_validity = ins1_validity;
    packet.ins2_validity = ins2_validity;
    packet.adc1_validity = adc1_validity;
    packet.adc2_validity = adc2_validity;
    packet.wow = wow;
    mav_array_memcpy(packet.ins1_eul_rpy, ins1_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_omg, ins1_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_acc, ins1_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_vel_ned, ins1_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_eul_rpy, ins2_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_omg, ins2_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_acc, ins2_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_vel_ned, ins2_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.rotor_rpm, rotor_rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.servo_def, servo_def, sizeof(int16_t)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_SENSOR;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
}

/**
 * @brief Pack a ldm_pil_sensor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] Simulator timestamp when the msg was sent.
 * @param ins1_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins1_omg [rad/s] Body frame angular speed
 * @param ins1_acc [rad/s] Body frame acceleration
 * @param ins1_lat [degE7] Latitude
 * @param ins1_lon [degE7] Longitude
 * @param ins1_alt_amsl [m] Altitude gps AMSL
 * @param ins1_vel_ned [m/s] vel NED
 * @param ins1_validity  bitfield with INS1 validity flags
 * @param ins2_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins2_omg [rad/s] Body frame angular speed
 * @param ins2_acc [rad/s] Body frame acceleration
 * @param ins2_lat [degE7] Latitude
 * @param ins2_lon [degE7] Longitude
 * @param ins2_alt_amsl [m] Altitude gps AMSL
 * @param ins2_vel_ned [m/s] vel NED
 * @param ins2_validity  bitfield with INS2 validity flags
 * @param adc1_cas [m/s] Indicated airspeed
 * @param adc1_alt_baro [m] Barometric altitude
 * @param adc1_aoa [rad] Angle of attack
 * @param adc1_aos [rad] Angle of sideslip
 * @param adc1_validity  bitfield with ADC1 validity flags
 * @param adc2_cas [m/s] Indicated airspeed
 * @param adc2_alt_baro [m] Barometric altitude
 * @param adc2_aoa [rad] Angle of attack
 * @param adc2_aos [rad] Angle of sideslip
 * @param adc2_validity  bitfield with ADC2 validity flags
 * @param radalt [m] Radalt altitde
 * @param wow [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3
 * @param rotor_rpm [RPM] rotor rpm as reported by EPU
 * @param pusher_rpm [RPM] pusher rpm as reported by engine
 * @param servo_def [degE2] actuator deflection reported by servo +/- 180 deg range
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const float *ins1_eul_rpy, const float *ins1_omg, const float *ins1_acc, int32_t ins1_lat, int32_t ins1_lon, float ins1_alt_amsl, const float *ins1_vel_ned, int8_t ins1_validity, const float *ins2_eul_rpy, const float *ins2_omg, const float *ins2_acc, int32_t ins2_lat, int32_t ins2_lon, float ins2_alt_amsl, const float *ins2_vel_ned, int8_t ins2_validity, float adc1_cas, float adc1_alt_baro, float adc1_aoa, float adc1_aos, int8_t adc1_validity, float adc2_cas, float adc2_alt_baro, float adc2_aoa, float adc2_aos, int8_t adc2_validity, float radalt, int8_t wow, const int16_t *rotor_rpm, int16_t pusher_rpm, const int16_t *servo_def)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 40, ins1_lat);
    _mav_put_int32_t(buf, 44, ins1_lon);
    _mav_put_float(buf, 48, ins1_alt_amsl);
    _mav_put_int32_t(buf, 100, ins2_lat);
    _mav_put_int32_t(buf, 104, ins2_lon);
    _mav_put_float(buf, 108, ins2_alt_amsl);
    _mav_put_float(buf, 124, adc1_cas);
    _mav_put_float(buf, 128, adc1_alt_baro);
    _mav_put_float(buf, 132, adc1_aoa);
    _mav_put_float(buf, 136, adc1_aos);
    _mav_put_float(buf, 140, adc2_cas);
    _mav_put_float(buf, 144, adc2_alt_baro);
    _mav_put_float(buf, 148, adc2_aoa);
    _mav_put_float(buf, 152, adc2_aos);
    _mav_put_float(buf, 156, radalt);
    _mav_put_int16_t(buf, 176, pusher_rpm);
    _mav_put_int8_t(buf, 202, ins1_validity);
    _mav_put_int8_t(buf, 203, ins2_validity);
    _mav_put_int8_t(buf, 204, adc1_validity);
    _mav_put_int8_t(buf, 205, adc2_validity);
    _mav_put_int8_t(buf, 206, wow);
    _mav_put_float_array(buf, 4, ins1_eul_rpy, 3);
    _mav_put_float_array(buf, 16, ins1_omg, 3);
    _mav_put_float_array(buf, 28, ins1_acc, 3);
    _mav_put_float_array(buf, 52, ins1_vel_ned, 3);
    _mav_put_float_array(buf, 64, ins2_eul_rpy, 3);
    _mav_put_float_array(buf, 76, ins2_omg, 3);
    _mav_put_float_array(buf, 88, ins2_acc, 3);
    _mav_put_float_array(buf, 112, ins2_vel_ned, 3);
    _mav_put_int16_t_array(buf, 160, rotor_rpm, 8);
    _mav_put_int16_t_array(buf, 178, servo_def, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#else
    mavlink_ldm_pil_sensor_t packet;
    packet.time_ms = time_ms;
    packet.ins1_lat = ins1_lat;
    packet.ins1_lon = ins1_lon;
    packet.ins1_alt_amsl = ins1_alt_amsl;
    packet.ins2_lat = ins2_lat;
    packet.ins2_lon = ins2_lon;
    packet.ins2_alt_amsl = ins2_alt_amsl;
    packet.adc1_cas = adc1_cas;
    packet.adc1_alt_baro = adc1_alt_baro;
    packet.adc1_aoa = adc1_aoa;
    packet.adc1_aos = adc1_aos;
    packet.adc2_cas = adc2_cas;
    packet.adc2_alt_baro = adc2_alt_baro;
    packet.adc2_aoa = adc2_aoa;
    packet.adc2_aos = adc2_aos;
    packet.radalt = radalt;
    packet.pusher_rpm = pusher_rpm;
    packet.ins1_validity = ins1_validity;
    packet.ins2_validity = ins2_validity;
    packet.adc1_validity = adc1_validity;
    packet.adc2_validity = adc2_validity;
    packet.wow = wow;
    mav_array_memcpy(packet.ins1_eul_rpy, ins1_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_omg, ins1_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_acc, ins1_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_vel_ned, ins1_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_eul_rpy, ins2_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_omg, ins2_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_acc, ins2_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_vel_ned, ins2_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.rotor_rpm, rotor_rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.servo_def, servo_def, sizeof(int16_t)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_SENSOR;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#endif
}

/**
 * @brief Pack a ldm_pil_sensor message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms [ms] Simulator timestamp when the msg was sent.
 * @param ins1_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins1_omg [rad/s] Body frame angular speed
 * @param ins1_acc [rad/s] Body frame acceleration
 * @param ins1_lat [degE7] Latitude
 * @param ins1_lon [degE7] Longitude
 * @param ins1_alt_amsl [m] Altitude gps AMSL
 * @param ins1_vel_ned [m/s] vel NED
 * @param ins1_validity  bitfield with INS1 validity flags
 * @param ins2_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins2_omg [rad/s] Body frame angular speed
 * @param ins2_acc [rad/s] Body frame acceleration
 * @param ins2_lat [degE7] Latitude
 * @param ins2_lon [degE7] Longitude
 * @param ins2_alt_amsl [m] Altitude gps AMSL
 * @param ins2_vel_ned [m/s] vel NED
 * @param ins2_validity  bitfield with INS2 validity flags
 * @param adc1_cas [m/s] Indicated airspeed
 * @param adc1_alt_baro [m] Barometric altitude
 * @param adc1_aoa [rad] Angle of attack
 * @param adc1_aos [rad] Angle of sideslip
 * @param adc1_validity  bitfield with ADC1 validity flags
 * @param adc2_cas [m/s] Indicated airspeed
 * @param adc2_alt_baro [m] Barometric altitude
 * @param adc2_aoa [rad] Angle of attack
 * @param adc2_aos [rad] Angle of sideslip
 * @param adc2_validity  bitfield with ADC2 validity flags
 * @param radalt [m] Radalt altitde
 * @param wow [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3
 * @param rotor_rpm [RPM] rotor rpm as reported by EPU
 * @param pusher_rpm [RPM] pusher rpm as reported by engine
 * @param servo_def [degE2] actuator deflection reported by servo +/- 180 deg range
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const float *ins1_eul_rpy,const float *ins1_omg,const float *ins1_acc,int32_t ins1_lat,int32_t ins1_lon,float ins1_alt_amsl,const float *ins1_vel_ned,int8_t ins1_validity,const float *ins2_eul_rpy,const float *ins2_omg,const float *ins2_acc,int32_t ins2_lat,int32_t ins2_lon,float ins2_alt_amsl,const float *ins2_vel_ned,int8_t ins2_validity,float adc1_cas,float adc1_alt_baro,float adc1_aoa,float adc1_aos,int8_t adc1_validity,float adc2_cas,float adc2_alt_baro,float adc2_aoa,float adc2_aos,int8_t adc2_validity,float radalt,int8_t wow,const int16_t *rotor_rpm,int16_t pusher_rpm,const int16_t *servo_def)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 40, ins1_lat);
    _mav_put_int32_t(buf, 44, ins1_lon);
    _mav_put_float(buf, 48, ins1_alt_amsl);
    _mav_put_int32_t(buf, 100, ins2_lat);
    _mav_put_int32_t(buf, 104, ins2_lon);
    _mav_put_float(buf, 108, ins2_alt_amsl);
    _mav_put_float(buf, 124, adc1_cas);
    _mav_put_float(buf, 128, adc1_alt_baro);
    _mav_put_float(buf, 132, adc1_aoa);
    _mav_put_float(buf, 136, adc1_aos);
    _mav_put_float(buf, 140, adc2_cas);
    _mav_put_float(buf, 144, adc2_alt_baro);
    _mav_put_float(buf, 148, adc2_aoa);
    _mav_put_float(buf, 152, adc2_aos);
    _mav_put_float(buf, 156, radalt);
    _mav_put_int16_t(buf, 176, pusher_rpm);
    _mav_put_int8_t(buf, 202, ins1_validity);
    _mav_put_int8_t(buf, 203, ins2_validity);
    _mav_put_int8_t(buf, 204, adc1_validity);
    _mav_put_int8_t(buf, 205, adc2_validity);
    _mav_put_int8_t(buf, 206, wow);
    _mav_put_float_array(buf, 4, ins1_eul_rpy, 3);
    _mav_put_float_array(buf, 16, ins1_omg, 3);
    _mav_put_float_array(buf, 28, ins1_acc, 3);
    _mav_put_float_array(buf, 52, ins1_vel_ned, 3);
    _mav_put_float_array(buf, 64, ins2_eul_rpy, 3);
    _mav_put_float_array(buf, 76, ins2_omg, 3);
    _mav_put_float_array(buf, 88, ins2_acc, 3);
    _mav_put_float_array(buf, 112, ins2_vel_ned, 3);
    _mav_put_int16_t_array(buf, 160, rotor_rpm, 8);
    _mav_put_int16_t_array(buf, 178, servo_def, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#else
    mavlink_ldm_pil_sensor_t packet;
    packet.time_ms = time_ms;
    packet.ins1_lat = ins1_lat;
    packet.ins1_lon = ins1_lon;
    packet.ins1_alt_amsl = ins1_alt_amsl;
    packet.ins2_lat = ins2_lat;
    packet.ins2_lon = ins2_lon;
    packet.ins2_alt_amsl = ins2_alt_amsl;
    packet.adc1_cas = adc1_cas;
    packet.adc1_alt_baro = adc1_alt_baro;
    packet.adc1_aoa = adc1_aoa;
    packet.adc1_aos = adc1_aos;
    packet.adc2_cas = adc2_cas;
    packet.adc2_alt_baro = adc2_alt_baro;
    packet.adc2_aoa = adc2_aoa;
    packet.adc2_aos = adc2_aos;
    packet.radalt = radalt;
    packet.pusher_rpm = pusher_rpm;
    packet.ins1_validity = ins1_validity;
    packet.ins2_validity = ins2_validity;
    packet.adc1_validity = adc1_validity;
    packet.adc2_validity = adc2_validity;
    packet.wow = wow;
    mav_array_memcpy(packet.ins1_eul_rpy, ins1_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_omg, ins1_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_acc, ins1_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_vel_ned, ins1_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_eul_rpy, ins2_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_omg, ins2_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_acc, ins2_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_vel_ned, ins2_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.rotor_rpm, rotor_rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.servo_def, servo_def, sizeof(int16_t)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_SENSOR;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
}

/**
 * @brief Encode a ldm_pil_sensor struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_sensor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_pil_sensor_t* ldm_pil_sensor)
{
    return mavlink_msg_ldm_pil_sensor_pack(system_id, component_id, msg, ldm_pil_sensor->time_ms, ldm_pil_sensor->ins1_eul_rpy, ldm_pil_sensor->ins1_omg, ldm_pil_sensor->ins1_acc, ldm_pil_sensor->ins1_lat, ldm_pil_sensor->ins1_lon, ldm_pil_sensor->ins1_alt_amsl, ldm_pil_sensor->ins1_vel_ned, ldm_pil_sensor->ins1_validity, ldm_pil_sensor->ins2_eul_rpy, ldm_pil_sensor->ins2_omg, ldm_pil_sensor->ins2_acc, ldm_pil_sensor->ins2_lat, ldm_pil_sensor->ins2_lon, ldm_pil_sensor->ins2_alt_amsl, ldm_pil_sensor->ins2_vel_ned, ldm_pil_sensor->ins2_validity, ldm_pil_sensor->adc1_cas, ldm_pil_sensor->adc1_alt_baro, ldm_pil_sensor->adc1_aoa, ldm_pil_sensor->adc1_aos, ldm_pil_sensor->adc1_validity, ldm_pil_sensor->adc2_cas, ldm_pil_sensor->adc2_alt_baro, ldm_pil_sensor->adc2_aoa, ldm_pil_sensor->adc2_aos, ldm_pil_sensor->adc2_validity, ldm_pil_sensor->radalt, ldm_pil_sensor->wow, ldm_pil_sensor->rotor_rpm, ldm_pil_sensor->pusher_rpm, ldm_pil_sensor->servo_def);
}

/**
 * @brief Encode a ldm_pil_sensor struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_sensor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_pil_sensor_t* ldm_pil_sensor)
{
    return mavlink_msg_ldm_pil_sensor_pack_chan(system_id, component_id, chan, msg, ldm_pil_sensor->time_ms, ldm_pil_sensor->ins1_eul_rpy, ldm_pil_sensor->ins1_omg, ldm_pil_sensor->ins1_acc, ldm_pil_sensor->ins1_lat, ldm_pil_sensor->ins1_lon, ldm_pil_sensor->ins1_alt_amsl, ldm_pil_sensor->ins1_vel_ned, ldm_pil_sensor->ins1_validity, ldm_pil_sensor->ins2_eul_rpy, ldm_pil_sensor->ins2_omg, ldm_pil_sensor->ins2_acc, ldm_pil_sensor->ins2_lat, ldm_pil_sensor->ins2_lon, ldm_pil_sensor->ins2_alt_amsl, ldm_pil_sensor->ins2_vel_ned, ldm_pil_sensor->ins2_validity, ldm_pil_sensor->adc1_cas, ldm_pil_sensor->adc1_alt_baro, ldm_pil_sensor->adc1_aoa, ldm_pil_sensor->adc1_aos, ldm_pil_sensor->adc1_validity, ldm_pil_sensor->adc2_cas, ldm_pil_sensor->adc2_alt_baro, ldm_pil_sensor->adc2_aoa, ldm_pil_sensor->adc2_aos, ldm_pil_sensor->adc2_validity, ldm_pil_sensor->radalt, ldm_pil_sensor->wow, ldm_pil_sensor->rotor_rpm, ldm_pil_sensor->pusher_rpm, ldm_pil_sensor->servo_def);
}

/**
 * @brief Encode a ldm_pil_sensor struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_sensor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_pil_sensor_t* ldm_pil_sensor)
{
    return mavlink_msg_ldm_pil_sensor_pack_status(system_id, component_id, _status, msg,  ldm_pil_sensor->time_ms, ldm_pil_sensor->ins1_eul_rpy, ldm_pil_sensor->ins1_omg, ldm_pil_sensor->ins1_acc, ldm_pil_sensor->ins1_lat, ldm_pil_sensor->ins1_lon, ldm_pil_sensor->ins1_alt_amsl, ldm_pil_sensor->ins1_vel_ned, ldm_pil_sensor->ins1_validity, ldm_pil_sensor->ins2_eul_rpy, ldm_pil_sensor->ins2_omg, ldm_pil_sensor->ins2_acc, ldm_pil_sensor->ins2_lat, ldm_pil_sensor->ins2_lon, ldm_pil_sensor->ins2_alt_amsl, ldm_pil_sensor->ins2_vel_ned, ldm_pil_sensor->ins2_validity, ldm_pil_sensor->adc1_cas, ldm_pil_sensor->adc1_alt_baro, ldm_pil_sensor->adc1_aoa, ldm_pil_sensor->adc1_aos, ldm_pil_sensor->adc1_validity, ldm_pil_sensor->adc2_cas, ldm_pil_sensor->adc2_alt_baro, ldm_pil_sensor->adc2_aoa, ldm_pil_sensor->adc2_aos, ldm_pil_sensor->adc2_validity, ldm_pil_sensor->radalt, ldm_pil_sensor->wow, ldm_pil_sensor->rotor_rpm, ldm_pil_sensor->pusher_rpm, ldm_pil_sensor->servo_def);
}

/**
 * @brief Send a ldm_pil_sensor message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms [ms] Simulator timestamp when the msg was sent.
 * @param ins1_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins1_omg [rad/s] Body frame angular speed
 * @param ins1_acc [rad/s] Body frame acceleration
 * @param ins1_lat [degE7] Latitude
 * @param ins1_lon [degE7] Longitude
 * @param ins1_alt_amsl [m] Altitude gps AMSL
 * @param ins1_vel_ned [m/s] vel NED
 * @param ins1_validity  bitfield with INS1 validity flags
 * @param ins2_eul_rpy [rad] attitude expressed 321 Euler angle
 * @param ins2_omg [rad/s] Body frame angular speed
 * @param ins2_acc [rad/s] Body frame acceleration
 * @param ins2_lat [degE7] Latitude
 * @param ins2_lon [degE7] Longitude
 * @param ins2_alt_amsl [m] Altitude gps AMSL
 * @param ins2_vel_ned [m/s] vel NED
 * @param ins2_validity  bitfield with INS2 validity flags
 * @param adc1_cas [m/s] Indicated airspeed
 * @param adc1_alt_baro [m] Barometric altitude
 * @param adc1_aoa [rad] Angle of attack
 * @param adc1_aos [rad] Angle of sideslip
 * @param adc1_validity  bitfield with ADC1 validity flags
 * @param adc2_cas [m/s] Indicated airspeed
 * @param adc2_alt_baro [m] Barometric altitude
 * @param adc2_aoa [rad] Angle of attack
 * @param adc2_aos [rad] Angle of sideslip
 * @param adc2_validity  bitfield with ADC2 validity flags
 * @param radalt [m] Radalt altitde
 * @param wow [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3
 * @param rotor_rpm [RPM] rotor rpm as reported by EPU
 * @param pusher_rpm [RPM] pusher rpm as reported by engine
 * @param servo_def [degE2] actuator deflection reported by servo +/- 180 deg range
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_pil_sensor_send(mavlink_channel_t chan, uint32_t time_ms, const float *ins1_eul_rpy, const float *ins1_omg, const float *ins1_acc, int32_t ins1_lat, int32_t ins1_lon, float ins1_alt_amsl, const float *ins1_vel_ned, int8_t ins1_validity, const float *ins2_eul_rpy, const float *ins2_omg, const float *ins2_acc, int32_t ins2_lat, int32_t ins2_lon, float ins2_alt_amsl, const float *ins2_vel_ned, int8_t ins2_validity, float adc1_cas, float adc1_alt_baro, float adc1_aoa, float adc1_aos, int8_t adc1_validity, float adc2_cas, float adc2_alt_baro, float adc2_aoa, float adc2_aos, int8_t adc2_validity, float radalt, int8_t wow, const int16_t *rotor_rpm, int16_t pusher_rpm, const int16_t *servo_def)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 40, ins1_lat);
    _mav_put_int32_t(buf, 44, ins1_lon);
    _mav_put_float(buf, 48, ins1_alt_amsl);
    _mav_put_int32_t(buf, 100, ins2_lat);
    _mav_put_int32_t(buf, 104, ins2_lon);
    _mav_put_float(buf, 108, ins2_alt_amsl);
    _mav_put_float(buf, 124, adc1_cas);
    _mav_put_float(buf, 128, adc1_alt_baro);
    _mav_put_float(buf, 132, adc1_aoa);
    _mav_put_float(buf, 136, adc1_aos);
    _mav_put_float(buf, 140, adc2_cas);
    _mav_put_float(buf, 144, adc2_alt_baro);
    _mav_put_float(buf, 148, adc2_aoa);
    _mav_put_float(buf, 152, adc2_aos);
    _mav_put_float(buf, 156, radalt);
    _mav_put_int16_t(buf, 176, pusher_rpm);
    _mav_put_int8_t(buf, 202, ins1_validity);
    _mav_put_int8_t(buf, 203, ins2_validity);
    _mav_put_int8_t(buf, 204, adc1_validity);
    _mav_put_int8_t(buf, 205, adc2_validity);
    _mav_put_int8_t(buf, 206, wow);
    _mav_put_float_array(buf, 4, ins1_eul_rpy, 3);
    _mav_put_float_array(buf, 16, ins1_omg, 3);
    _mav_put_float_array(buf, 28, ins1_acc, 3);
    _mav_put_float_array(buf, 52, ins1_vel_ned, 3);
    _mav_put_float_array(buf, 64, ins2_eul_rpy, 3);
    _mav_put_float_array(buf, 76, ins2_omg, 3);
    _mav_put_float_array(buf, 88, ins2_acc, 3);
    _mav_put_float_array(buf, 112, ins2_vel_ned, 3);
    _mav_put_int16_t_array(buf, 160, rotor_rpm, 8);
    _mav_put_int16_t_array(buf, 178, servo_def, 12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR, buf, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#else
    mavlink_ldm_pil_sensor_t packet;
    packet.time_ms = time_ms;
    packet.ins1_lat = ins1_lat;
    packet.ins1_lon = ins1_lon;
    packet.ins1_alt_amsl = ins1_alt_amsl;
    packet.ins2_lat = ins2_lat;
    packet.ins2_lon = ins2_lon;
    packet.ins2_alt_amsl = ins2_alt_amsl;
    packet.adc1_cas = adc1_cas;
    packet.adc1_alt_baro = adc1_alt_baro;
    packet.adc1_aoa = adc1_aoa;
    packet.adc1_aos = adc1_aos;
    packet.adc2_cas = adc2_cas;
    packet.adc2_alt_baro = adc2_alt_baro;
    packet.adc2_aoa = adc2_aoa;
    packet.adc2_aos = adc2_aos;
    packet.radalt = radalt;
    packet.pusher_rpm = pusher_rpm;
    packet.ins1_validity = ins1_validity;
    packet.ins2_validity = ins2_validity;
    packet.adc1_validity = adc1_validity;
    packet.adc2_validity = adc2_validity;
    packet.wow = wow;
    mav_array_memcpy(packet.ins1_eul_rpy, ins1_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_omg, ins1_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_acc, ins1_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins1_vel_ned, ins1_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_eul_rpy, ins2_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_omg, ins2_omg, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_acc, ins2_acc, sizeof(float)*3);
    mav_array_memcpy(packet.ins2_vel_ned, ins2_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet.rotor_rpm, rotor_rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.servo_def, servo_def, sizeof(int16_t)*12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR, (const char *)&packet, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#endif
}

/**
 * @brief Send a ldm_pil_sensor message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_pil_sensor_send_struct(mavlink_channel_t chan, const mavlink_ldm_pil_sensor_t* ldm_pil_sensor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_pil_sensor_send(chan, ldm_pil_sensor->time_ms, ldm_pil_sensor->ins1_eul_rpy, ldm_pil_sensor->ins1_omg, ldm_pil_sensor->ins1_acc, ldm_pil_sensor->ins1_lat, ldm_pil_sensor->ins1_lon, ldm_pil_sensor->ins1_alt_amsl, ldm_pil_sensor->ins1_vel_ned, ldm_pil_sensor->ins1_validity, ldm_pil_sensor->ins2_eul_rpy, ldm_pil_sensor->ins2_omg, ldm_pil_sensor->ins2_acc, ldm_pil_sensor->ins2_lat, ldm_pil_sensor->ins2_lon, ldm_pil_sensor->ins2_alt_amsl, ldm_pil_sensor->ins2_vel_ned, ldm_pil_sensor->ins2_validity, ldm_pil_sensor->adc1_cas, ldm_pil_sensor->adc1_alt_baro, ldm_pil_sensor->adc1_aoa, ldm_pil_sensor->adc1_aos, ldm_pil_sensor->adc1_validity, ldm_pil_sensor->adc2_cas, ldm_pil_sensor->adc2_alt_baro, ldm_pil_sensor->adc2_aoa, ldm_pil_sensor->adc2_aos, ldm_pil_sensor->adc2_validity, ldm_pil_sensor->radalt, ldm_pil_sensor->wow, ldm_pil_sensor->rotor_rpm, ldm_pil_sensor->pusher_rpm, ldm_pil_sensor->servo_def);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR, (const char *)ldm_pil_sensor, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_pil_sensor_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const float *ins1_eul_rpy, const float *ins1_omg, const float *ins1_acc, int32_t ins1_lat, int32_t ins1_lon, float ins1_alt_amsl, const float *ins1_vel_ned, int8_t ins1_validity, const float *ins2_eul_rpy, const float *ins2_omg, const float *ins2_acc, int32_t ins2_lat, int32_t ins2_lon, float ins2_alt_amsl, const float *ins2_vel_ned, int8_t ins2_validity, float adc1_cas, float adc1_alt_baro, float adc1_aoa, float adc1_aos, int8_t adc1_validity, float adc2_cas, float adc2_alt_baro, float adc2_aoa, float adc2_aos, int8_t adc2_validity, float radalt, int8_t wow, const int16_t *rotor_rpm, int16_t pusher_rpm, const int16_t *servo_def)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 40, ins1_lat);
    _mav_put_int32_t(buf, 44, ins1_lon);
    _mav_put_float(buf, 48, ins1_alt_amsl);
    _mav_put_int32_t(buf, 100, ins2_lat);
    _mav_put_int32_t(buf, 104, ins2_lon);
    _mav_put_float(buf, 108, ins2_alt_amsl);
    _mav_put_float(buf, 124, adc1_cas);
    _mav_put_float(buf, 128, adc1_alt_baro);
    _mav_put_float(buf, 132, adc1_aoa);
    _mav_put_float(buf, 136, adc1_aos);
    _mav_put_float(buf, 140, adc2_cas);
    _mav_put_float(buf, 144, adc2_alt_baro);
    _mav_put_float(buf, 148, adc2_aoa);
    _mav_put_float(buf, 152, adc2_aos);
    _mav_put_float(buf, 156, radalt);
    _mav_put_int16_t(buf, 176, pusher_rpm);
    _mav_put_int8_t(buf, 202, ins1_validity);
    _mav_put_int8_t(buf, 203, ins2_validity);
    _mav_put_int8_t(buf, 204, adc1_validity);
    _mav_put_int8_t(buf, 205, adc2_validity);
    _mav_put_int8_t(buf, 206, wow);
    _mav_put_float_array(buf, 4, ins1_eul_rpy, 3);
    _mav_put_float_array(buf, 16, ins1_omg, 3);
    _mav_put_float_array(buf, 28, ins1_acc, 3);
    _mav_put_float_array(buf, 52, ins1_vel_ned, 3);
    _mav_put_float_array(buf, 64, ins2_eul_rpy, 3);
    _mav_put_float_array(buf, 76, ins2_omg, 3);
    _mav_put_float_array(buf, 88, ins2_acc, 3);
    _mav_put_float_array(buf, 112, ins2_vel_ned, 3);
    _mav_put_int16_t_array(buf, 160, rotor_rpm, 8);
    _mav_put_int16_t_array(buf, 178, servo_def, 12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR, buf, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#else
    mavlink_ldm_pil_sensor_t *packet = (mavlink_ldm_pil_sensor_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->ins1_lat = ins1_lat;
    packet->ins1_lon = ins1_lon;
    packet->ins1_alt_amsl = ins1_alt_amsl;
    packet->ins2_lat = ins2_lat;
    packet->ins2_lon = ins2_lon;
    packet->ins2_alt_amsl = ins2_alt_amsl;
    packet->adc1_cas = adc1_cas;
    packet->adc1_alt_baro = adc1_alt_baro;
    packet->adc1_aoa = adc1_aoa;
    packet->adc1_aos = adc1_aos;
    packet->adc2_cas = adc2_cas;
    packet->adc2_alt_baro = adc2_alt_baro;
    packet->adc2_aoa = adc2_aoa;
    packet->adc2_aos = adc2_aos;
    packet->radalt = radalt;
    packet->pusher_rpm = pusher_rpm;
    packet->ins1_validity = ins1_validity;
    packet->ins2_validity = ins2_validity;
    packet->adc1_validity = adc1_validity;
    packet->adc2_validity = adc2_validity;
    packet->wow = wow;
    mav_array_memcpy(packet->ins1_eul_rpy, ins1_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet->ins1_omg, ins1_omg, sizeof(float)*3);
    mav_array_memcpy(packet->ins1_acc, ins1_acc, sizeof(float)*3);
    mav_array_memcpy(packet->ins1_vel_ned, ins1_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet->ins2_eul_rpy, ins2_eul_rpy, sizeof(float)*3);
    mav_array_memcpy(packet->ins2_omg, ins2_omg, sizeof(float)*3);
    mav_array_memcpy(packet->ins2_acc, ins2_acc, sizeof(float)*3);
    mav_array_memcpy(packet->ins2_vel_ned, ins2_vel_ned, sizeof(float)*3);
    mav_array_memcpy(packet->rotor_rpm, rotor_rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet->servo_def, servo_def, sizeof(int16_t)*12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_SENSOR, (const char *)packet, MAVLINK_MSG_ID_LDM_PIL_SENSOR_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN, MAVLINK_MSG_ID_LDM_PIL_SENSOR_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_PIL_SENSOR UNPACKING


/**
 * @brief Get field time_ms from ldm_pil_sensor message
 *
 * @return [ms] Simulator timestamp when the msg was sent.
 */
static inline uint32_t mavlink_msg_ldm_pil_sensor_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field ins1_eul_rpy from ldm_pil_sensor message
 *
 * @return [rad] attitude expressed 321 Euler angle
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins1_eul_rpy(const mavlink_message_t* msg, float *ins1_eul_rpy)
{
    return _MAV_RETURN_float_array(msg, ins1_eul_rpy, 3,  4);
}

/**
 * @brief Get field ins1_omg from ldm_pil_sensor message
 *
 * @return [rad/s] Body frame angular speed
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins1_omg(const mavlink_message_t* msg, float *ins1_omg)
{
    return _MAV_RETURN_float_array(msg, ins1_omg, 3,  16);
}

/**
 * @brief Get field ins1_acc from ldm_pil_sensor message
 *
 * @return [rad/s] Body frame acceleration
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins1_acc(const mavlink_message_t* msg, float *ins1_acc)
{
    return _MAV_RETURN_float_array(msg, ins1_acc, 3,  28);
}

/**
 * @brief Get field ins1_lat from ldm_pil_sensor message
 *
 * @return [degE7] Latitude
 */
static inline int32_t mavlink_msg_ldm_pil_sensor_get_ins1_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  40);
}

/**
 * @brief Get field ins1_lon from ldm_pil_sensor message
 *
 * @return [degE7] Longitude
 */
static inline int32_t mavlink_msg_ldm_pil_sensor_get_ins1_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  44);
}

/**
 * @brief Get field ins1_alt_amsl from ldm_pil_sensor message
 *
 * @return [m] Altitude gps AMSL
 */
static inline float mavlink_msg_ldm_pil_sensor_get_ins1_alt_amsl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  48);
}

/**
 * @brief Get field ins1_vel_ned from ldm_pil_sensor message
 *
 * @return [m/s] vel NED
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins1_vel_ned(const mavlink_message_t* msg, float *ins1_vel_ned)
{
    return _MAV_RETURN_float_array(msg, ins1_vel_ned, 3,  52);
}

/**
 * @brief Get field ins1_validity from ldm_pil_sensor message
 *
 * @return  bitfield with INS1 validity flags
 */
static inline int8_t mavlink_msg_ldm_pil_sensor_get_ins1_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  202);
}

/**
 * @brief Get field ins2_eul_rpy from ldm_pil_sensor message
 *
 * @return [rad] attitude expressed 321 Euler angle
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins2_eul_rpy(const mavlink_message_t* msg, float *ins2_eul_rpy)
{
    return _MAV_RETURN_float_array(msg, ins2_eul_rpy, 3,  64);
}

/**
 * @brief Get field ins2_omg from ldm_pil_sensor message
 *
 * @return [rad/s] Body frame angular speed
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins2_omg(const mavlink_message_t* msg, float *ins2_omg)
{
    return _MAV_RETURN_float_array(msg, ins2_omg, 3,  76);
}

/**
 * @brief Get field ins2_acc from ldm_pil_sensor message
 *
 * @return [rad/s] Body frame acceleration
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins2_acc(const mavlink_message_t* msg, float *ins2_acc)
{
    return _MAV_RETURN_float_array(msg, ins2_acc, 3,  88);
}

/**
 * @brief Get field ins2_lat from ldm_pil_sensor message
 *
 * @return [degE7] Latitude
 */
static inline int32_t mavlink_msg_ldm_pil_sensor_get_ins2_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  100);
}

/**
 * @brief Get field ins2_lon from ldm_pil_sensor message
 *
 * @return [degE7] Longitude
 */
static inline int32_t mavlink_msg_ldm_pil_sensor_get_ins2_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  104);
}

/**
 * @brief Get field ins2_alt_amsl from ldm_pil_sensor message
 *
 * @return [m] Altitude gps AMSL
 */
static inline float mavlink_msg_ldm_pil_sensor_get_ins2_alt_amsl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  108);
}

/**
 * @brief Get field ins2_vel_ned from ldm_pil_sensor message
 *
 * @return [m/s] vel NED
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_ins2_vel_ned(const mavlink_message_t* msg, float *ins2_vel_ned)
{
    return _MAV_RETURN_float_array(msg, ins2_vel_ned, 3,  112);
}

/**
 * @brief Get field ins2_validity from ldm_pil_sensor message
 *
 * @return  bitfield with INS2 validity flags
 */
static inline int8_t mavlink_msg_ldm_pil_sensor_get_ins2_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  203);
}

/**
 * @brief Get field adc1_cas from ldm_pil_sensor message
 *
 * @return [m/s] Indicated airspeed
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc1_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  124);
}

/**
 * @brief Get field adc1_alt_baro from ldm_pil_sensor message
 *
 * @return [m] Barometric altitude
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc1_alt_baro(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  128);
}

/**
 * @brief Get field adc1_aoa from ldm_pil_sensor message
 *
 * @return [rad] Angle of attack
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc1_aoa(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  132);
}

/**
 * @brief Get field adc1_aos from ldm_pil_sensor message
 *
 * @return [rad] Angle of sideslip
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc1_aos(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  136);
}

/**
 * @brief Get field adc1_validity from ldm_pil_sensor message
 *
 * @return  bitfield with ADC1 validity flags
 */
static inline int8_t mavlink_msg_ldm_pil_sensor_get_adc1_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  204);
}

/**
 * @brief Get field adc2_cas from ldm_pil_sensor message
 *
 * @return [m/s] Indicated airspeed
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc2_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  140);
}

/**
 * @brief Get field adc2_alt_baro from ldm_pil_sensor message
 *
 * @return [m] Barometric altitude
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc2_alt_baro(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  144);
}

/**
 * @brief Get field adc2_aoa from ldm_pil_sensor message
 *
 * @return [rad] Angle of attack
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc2_aoa(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  148);
}

/**
 * @brief Get field adc2_aos from ldm_pil_sensor message
 *
 * @return [rad] Angle of sideslip
 */
static inline float mavlink_msg_ldm_pil_sensor_get_adc2_aos(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  152);
}

/**
 * @brief Get field adc2_validity from ldm_pil_sensor message
 *
 * @return  bitfield with ADC2 validity flags
 */
static inline int8_t mavlink_msg_ldm_pil_sensor_get_adc2_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  205);
}

/**
 * @brief Get field radalt from ldm_pil_sensor message
 *
 * @return [m] Radalt altitde
 */
static inline float mavlink_msg_ldm_pil_sensor_get_radalt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  156);
}

/**
 * @brief Get field wow from ldm_pil_sensor message
 *
 * @return [null] LSB bits 1,2,3 represent the WoW for wheel 1,2,3
 */
static inline int8_t mavlink_msg_ldm_pil_sensor_get_wow(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  206);
}

/**
 * @brief Get field rotor_rpm from ldm_pil_sensor message
 *
 * @return [RPM] rotor rpm as reported by EPU
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_rotor_rpm(const mavlink_message_t* msg, int16_t *rotor_rpm)
{
    return _MAV_RETURN_int16_t_array(msg, rotor_rpm, 8,  160);
}

/**
 * @brief Get field pusher_rpm from ldm_pil_sensor message
 *
 * @return [RPM] pusher rpm as reported by engine
 */
static inline int16_t mavlink_msg_ldm_pil_sensor_get_pusher_rpm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  176);
}

/**
 * @brief Get field servo_def from ldm_pil_sensor message
 *
 * @return [degE2] actuator deflection reported by servo +/- 180 deg range
 */
static inline uint16_t mavlink_msg_ldm_pil_sensor_get_servo_def(const mavlink_message_t* msg, int16_t *servo_def)
{
    return _MAV_RETURN_int16_t_array(msg, servo_def, 12,  178);
}

/**
 * @brief Decode a ldm_pil_sensor message into a struct
 *
 * @param msg The message to decode
 * @param ldm_pil_sensor C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_pil_sensor_decode(const mavlink_message_t* msg, mavlink_ldm_pil_sensor_t* ldm_pil_sensor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_pil_sensor->time_ms = mavlink_msg_ldm_pil_sensor_get_time_ms(msg);
    mavlink_msg_ldm_pil_sensor_get_ins1_eul_rpy(msg, ldm_pil_sensor->ins1_eul_rpy);
    mavlink_msg_ldm_pil_sensor_get_ins1_omg(msg, ldm_pil_sensor->ins1_omg);
    mavlink_msg_ldm_pil_sensor_get_ins1_acc(msg, ldm_pil_sensor->ins1_acc);
    ldm_pil_sensor->ins1_lat = mavlink_msg_ldm_pil_sensor_get_ins1_lat(msg);
    ldm_pil_sensor->ins1_lon = mavlink_msg_ldm_pil_sensor_get_ins1_lon(msg);
    ldm_pil_sensor->ins1_alt_amsl = mavlink_msg_ldm_pil_sensor_get_ins1_alt_amsl(msg);
    mavlink_msg_ldm_pil_sensor_get_ins1_vel_ned(msg, ldm_pil_sensor->ins1_vel_ned);
    mavlink_msg_ldm_pil_sensor_get_ins2_eul_rpy(msg, ldm_pil_sensor->ins2_eul_rpy);
    mavlink_msg_ldm_pil_sensor_get_ins2_omg(msg, ldm_pil_sensor->ins2_omg);
    mavlink_msg_ldm_pil_sensor_get_ins2_acc(msg, ldm_pil_sensor->ins2_acc);
    ldm_pil_sensor->ins2_lat = mavlink_msg_ldm_pil_sensor_get_ins2_lat(msg);
    ldm_pil_sensor->ins2_lon = mavlink_msg_ldm_pil_sensor_get_ins2_lon(msg);
    ldm_pil_sensor->ins2_alt_amsl = mavlink_msg_ldm_pil_sensor_get_ins2_alt_amsl(msg);
    mavlink_msg_ldm_pil_sensor_get_ins2_vel_ned(msg, ldm_pil_sensor->ins2_vel_ned);
    ldm_pil_sensor->adc1_cas = mavlink_msg_ldm_pil_sensor_get_adc1_cas(msg);
    ldm_pil_sensor->adc1_alt_baro = mavlink_msg_ldm_pil_sensor_get_adc1_alt_baro(msg);
    ldm_pil_sensor->adc1_aoa = mavlink_msg_ldm_pil_sensor_get_adc1_aoa(msg);
    ldm_pil_sensor->adc1_aos = mavlink_msg_ldm_pil_sensor_get_adc1_aos(msg);
    ldm_pil_sensor->adc2_cas = mavlink_msg_ldm_pil_sensor_get_adc2_cas(msg);
    ldm_pil_sensor->adc2_alt_baro = mavlink_msg_ldm_pil_sensor_get_adc2_alt_baro(msg);
    ldm_pil_sensor->adc2_aoa = mavlink_msg_ldm_pil_sensor_get_adc2_aoa(msg);
    ldm_pil_sensor->adc2_aos = mavlink_msg_ldm_pil_sensor_get_adc2_aos(msg);
    ldm_pil_sensor->radalt = mavlink_msg_ldm_pil_sensor_get_radalt(msg);
    mavlink_msg_ldm_pil_sensor_get_rotor_rpm(msg, ldm_pil_sensor->rotor_rpm);
    ldm_pil_sensor->pusher_rpm = mavlink_msg_ldm_pil_sensor_get_pusher_rpm(msg);
    mavlink_msg_ldm_pil_sensor_get_servo_def(msg, ldm_pil_sensor->servo_def);
    ldm_pil_sensor->ins1_validity = mavlink_msg_ldm_pil_sensor_get_ins1_validity(msg);
    ldm_pil_sensor->ins2_validity = mavlink_msg_ldm_pil_sensor_get_ins2_validity(msg);
    ldm_pil_sensor->adc1_validity = mavlink_msg_ldm_pil_sensor_get_adc1_validity(msg);
    ldm_pil_sensor->adc2_validity = mavlink_msg_ldm_pil_sensor_get_adc2_validity(msg);
    ldm_pil_sensor->wow = mavlink_msg_ldm_pil_sensor_get_wow(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN? msg->len : MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN;
        memset(ldm_pil_sensor, 0, MAVLINK_MSG_ID_LDM_PIL_SENSOR_LEN);
    memcpy(ldm_pil_sensor, _MAV_PAYLOAD(msg), len);
#endif
}
