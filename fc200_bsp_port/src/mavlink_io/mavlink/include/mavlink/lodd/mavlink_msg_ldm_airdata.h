#pragma once
// MESSAGE LDM_AIRDATA PACKING

#define MAVLINK_MSG_ID_LDM_AIRDATA 52004


typedef struct __mavlink_ldm_airdata_t {
 uint32_t time_ms; /*<  FCC internal time when the msg is sent*/
 int32_t ads9_alt_baro; /*< [mm] ADS9 barometric altitude*/
 int16_t ads9_cas; /*< [cm/s] airspeed data from ADS9*/
 int16_t px4_cas; /*< [cm/s] voted airspeed data from PX4*/
 int16_t px4_cas_1; /*< [cm/s] airspeed data from PX4 sensor 1*/
 int16_t px4_cas_2; /*< [cm/s] airspeed data from PX4 sensor 2*/
 uint16_t radalt; /*< [cm] radalt altitude*/
 uint8_t validity; /*<  validity flags*/
 int8_t ads9_oat; /*< [deg C] outside air temperature ADS9*/
 uint8_t radalt_snr; /*< [dB] radalt signal-to-noise ratio*/
 uint8_t radalt_validity; /*<  radalt timeout flag*/
} mavlink_ldm_airdata_t;

#define MAVLINK_MSG_ID_LDM_AIRDATA_LEN 22
#define MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN 22
#define MAVLINK_MSG_ID_52004_LEN 22
#define MAVLINK_MSG_ID_52004_MIN_LEN 22

#define MAVLINK_MSG_ID_LDM_AIRDATA_CRC 19
#define MAVLINK_MSG_ID_52004_CRC 19



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_AIRDATA { \
    52004, \
    "LDM_AIRDATA", \
    11, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_airdata_t, time_ms) }, \
         { "validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_ldm_airdata_t, validity) }, \
         { "ads9_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_airdata_t, ads9_cas) }, \
         { "ads9_alt_baro", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ldm_airdata_t, ads9_alt_baro) }, \
         { "ads9_oat", NULL, MAVLINK_TYPE_INT8_T, 0, 19, offsetof(mavlink_ldm_airdata_t, ads9_oat) }, \
         { "px4_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_airdata_t, px4_cas) }, \
         { "px4_cas_1", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_airdata_t, px4_cas_1) }, \
         { "px4_cas_2", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_ldm_airdata_t, px4_cas_2) }, \
         { "radalt", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_ldm_airdata_t, radalt) }, \
         { "radalt_snr", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_ldm_airdata_t, radalt_snr) }, \
         { "radalt_validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_ldm_airdata_t, radalt_validity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_AIRDATA { \
    "LDM_AIRDATA", \
    11, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_airdata_t, time_ms) }, \
         { "validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_ldm_airdata_t, validity) }, \
         { "ads9_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_airdata_t, ads9_cas) }, \
         { "ads9_alt_baro", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_ldm_airdata_t, ads9_alt_baro) }, \
         { "ads9_oat", NULL, MAVLINK_TYPE_INT8_T, 0, 19, offsetof(mavlink_ldm_airdata_t, ads9_oat) }, \
         { "px4_cas", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_airdata_t, px4_cas) }, \
         { "px4_cas_1", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_airdata_t, px4_cas_1) }, \
         { "px4_cas_2", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_ldm_airdata_t, px4_cas_2) }, \
         { "radalt", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_ldm_airdata_t, radalt) }, \
         { "radalt_snr", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_ldm_airdata_t, radalt_snr) }, \
         { "radalt_validity", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_ldm_airdata_t, radalt_validity) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_airdata message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param validity  validity flags
 * @param ads9_cas [cm/s] airspeed data from ADS9
 * @param ads9_alt_baro [mm] ADS9 barometric altitude
 * @param ads9_oat [deg C] outside air temperature ADS9
 * @param px4_cas [cm/s] voted airspeed data from PX4
 * @param px4_cas_1 [cm/s] airspeed data from PX4 sensor 1
 * @param px4_cas_2 [cm/s] airspeed data from PX4 sensor 2
 * @param radalt [cm] radalt altitude
 * @param radalt_snr [dB] radalt signal-to-noise ratio
 * @param radalt_validity  radalt timeout flag
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_airdata_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, uint8_t validity, int16_t ads9_cas, int32_t ads9_alt_baro, int8_t ads9_oat, int16_t px4_cas, int16_t px4_cas_1, int16_t px4_cas_2, uint16_t radalt, uint8_t radalt_snr, uint8_t radalt_validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_AIRDATA_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 4, ads9_alt_baro);
    _mav_put_int16_t(buf, 8, ads9_cas);
    _mav_put_int16_t(buf, 10, px4_cas);
    _mav_put_int16_t(buf, 12, px4_cas_1);
    _mav_put_int16_t(buf, 14, px4_cas_2);
    _mav_put_uint16_t(buf, 16, radalt);
    _mav_put_uint8_t(buf, 18, validity);
    _mav_put_int8_t(buf, 19, ads9_oat);
    _mav_put_uint8_t(buf, 20, radalt_snr);
    _mav_put_uint8_t(buf, 21, radalt_validity);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#else
    mavlink_ldm_airdata_t packet;
    packet.time_ms = time_ms;
    packet.ads9_alt_baro = ads9_alt_baro;
    packet.ads9_cas = ads9_cas;
    packet.px4_cas = px4_cas;
    packet.px4_cas_1 = px4_cas_1;
    packet.px4_cas_2 = px4_cas_2;
    packet.radalt = radalt;
    packet.validity = validity;
    packet.ads9_oat = ads9_oat;
    packet.radalt_snr = radalt_snr;
    packet.radalt_validity = radalt_validity;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_AIRDATA;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
}

/**
 * @brief Pack a ldm_airdata message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param validity  validity flags
 * @param ads9_cas [cm/s] airspeed data from ADS9
 * @param ads9_alt_baro [mm] ADS9 barometric altitude
 * @param ads9_oat [deg C] outside air temperature ADS9
 * @param px4_cas [cm/s] voted airspeed data from PX4
 * @param px4_cas_1 [cm/s] airspeed data from PX4 sensor 1
 * @param px4_cas_2 [cm/s] airspeed data from PX4 sensor 2
 * @param radalt [cm] radalt altitude
 * @param radalt_snr [dB] radalt signal-to-noise ratio
 * @param radalt_validity  radalt timeout flag
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_airdata_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, uint8_t validity, int16_t ads9_cas, int32_t ads9_alt_baro, int8_t ads9_oat, int16_t px4_cas, int16_t px4_cas_1, int16_t px4_cas_2, uint16_t radalt, uint8_t radalt_snr, uint8_t radalt_validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_AIRDATA_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 4, ads9_alt_baro);
    _mav_put_int16_t(buf, 8, ads9_cas);
    _mav_put_int16_t(buf, 10, px4_cas);
    _mav_put_int16_t(buf, 12, px4_cas_1);
    _mav_put_int16_t(buf, 14, px4_cas_2);
    _mav_put_uint16_t(buf, 16, radalt);
    _mav_put_uint8_t(buf, 18, validity);
    _mav_put_int8_t(buf, 19, ads9_oat);
    _mav_put_uint8_t(buf, 20, radalt_snr);
    _mav_put_uint8_t(buf, 21, radalt_validity);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#else
    mavlink_ldm_airdata_t packet;
    packet.time_ms = time_ms;
    packet.ads9_alt_baro = ads9_alt_baro;
    packet.ads9_cas = ads9_cas;
    packet.px4_cas = px4_cas;
    packet.px4_cas_1 = px4_cas_1;
    packet.px4_cas_2 = px4_cas_2;
    packet.radalt = radalt;
    packet.validity = validity;
    packet.ads9_oat = ads9_oat;
    packet.radalt_snr = radalt_snr;
    packet.radalt_validity = radalt_validity;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_AIRDATA;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#endif
}

/**
 * @brief Pack a ldm_airdata message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  FCC internal time when the msg is sent
 * @param validity  validity flags
 * @param ads9_cas [cm/s] airspeed data from ADS9
 * @param ads9_alt_baro [mm] ADS9 barometric altitude
 * @param ads9_oat [deg C] outside air temperature ADS9
 * @param px4_cas [cm/s] voted airspeed data from PX4
 * @param px4_cas_1 [cm/s] airspeed data from PX4 sensor 1
 * @param px4_cas_2 [cm/s] airspeed data from PX4 sensor 2
 * @param radalt [cm] radalt altitude
 * @param radalt_snr [dB] radalt signal-to-noise ratio
 * @param radalt_validity  radalt timeout flag
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_airdata_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,uint8_t validity,int16_t ads9_cas,int32_t ads9_alt_baro,int8_t ads9_oat,int16_t px4_cas,int16_t px4_cas_1,int16_t px4_cas_2,uint16_t radalt,uint8_t radalt_snr,uint8_t radalt_validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_AIRDATA_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 4, ads9_alt_baro);
    _mav_put_int16_t(buf, 8, ads9_cas);
    _mav_put_int16_t(buf, 10, px4_cas);
    _mav_put_int16_t(buf, 12, px4_cas_1);
    _mav_put_int16_t(buf, 14, px4_cas_2);
    _mav_put_uint16_t(buf, 16, radalt);
    _mav_put_uint8_t(buf, 18, validity);
    _mav_put_int8_t(buf, 19, ads9_oat);
    _mav_put_uint8_t(buf, 20, radalt_snr);
    _mav_put_uint8_t(buf, 21, radalt_validity);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#else
    mavlink_ldm_airdata_t packet;
    packet.time_ms = time_ms;
    packet.ads9_alt_baro = ads9_alt_baro;
    packet.ads9_cas = ads9_cas;
    packet.px4_cas = px4_cas;
    packet.px4_cas_1 = px4_cas_1;
    packet.px4_cas_2 = px4_cas_2;
    packet.radalt = radalt;
    packet.validity = validity;
    packet.ads9_oat = ads9_oat;
    packet.radalt_snr = radalt_snr;
    packet.radalt_validity = radalt_validity;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_AIRDATA;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
}

/**
 * @brief Encode a ldm_airdata struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_airdata C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_airdata_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_airdata_t* ldm_airdata)
{
    return mavlink_msg_ldm_airdata_pack(system_id, component_id, msg, ldm_airdata->time_ms, ldm_airdata->validity, ldm_airdata->ads9_cas, ldm_airdata->ads9_alt_baro, ldm_airdata->ads9_oat, ldm_airdata->px4_cas, ldm_airdata->px4_cas_1, ldm_airdata->px4_cas_2, ldm_airdata->radalt, ldm_airdata->radalt_snr, ldm_airdata->radalt_validity);
}

/**
 * @brief Encode a ldm_airdata struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_airdata C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_airdata_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_airdata_t* ldm_airdata)
{
    return mavlink_msg_ldm_airdata_pack_chan(system_id, component_id, chan, msg, ldm_airdata->time_ms, ldm_airdata->validity, ldm_airdata->ads9_cas, ldm_airdata->ads9_alt_baro, ldm_airdata->ads9_oat, ldm_airdata->px4_cas, ldm_airdata->px4_cas_1, ldm_airdata->px4_cas_2, ldm_airdata->radalt, ldm_airdata->radalt_snr, ldm_airdata->radalt_validity);
}

/**
 * @brief Encode a ldm_airdata struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_airdata C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_airdata_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_airdata_t* ldm_airdata)
{
    return mavlink_msg_ldm_airdata_pack_status(system_id, component_id, _status, msg,  ldm_airdata->time_ms, ldm_airdata->validity, ldm_airdata->ads9_cas, ldm_airdata->ads9_alt_baro, ldm_airdata->ads9_oat, ldm_airdata->px4_cas, ldm_airdata->px4_cas_1, ldm_airdata->px4_cas_2, ldm_airdata->radalt, ldm_airdata->radalt_snr, ldm_airdata->radalt_validity);
}

/**
 * @brief Send a ldm_airdata message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param validity  validity flags
 * @param ads9_cas [cm/s] airspeed data from ADS9
 * @param ads9_alt_baro [mm] ADS9 barometric altitude
 * @param ads9_oat [deg C] outside air temperature ADS9
 * @param px4_cas [cm/s] voted airspeed data from PX4
 * @param px4_cas_1 [cm/s] airspeed data from PX4 sensor 1
 * @param px4_cas_2 [cm/s] airspeed data from PX4 sensor 2
 * @param radalt [cm] radalt altitude
 * @param radalt_snr [dB] radalt signal-to-noise ratio
 * @param radalt_validity  radalt timeout flag
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_airdata_send(mavlink_channel_t chan, uint32_t time_ms, uint8_t validity, int16_t ads9_cas, int32_t ads9_alt_baro, int8_t ads9_oat, int16_t px4_cas, int16_t px4_cas_1, int16_t px4_cas_2, uint16_t radalt, uint8_t radalt_snr, uint8_t radalt_validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_AIRDATA_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 4, ads9_alt_baro);
    _mav_put_int16_t(buf, 8, ads9_cas);
    _mav_put_int16_t(buf, 10, px4_cas);
    _mav_put_int16_t(buf, 12, px4_cas_1);
    _mav_put_int16_t(buf, 14, px4_cas_2);
    _mav_put_uint16_t(buf, 16, radalt);
    _mav_put_uint8_t(buf, 18, validity);
    _mav_put_int8_t(buf, 19, ads9_oat);
    _mav_put_uint8_t(buf, 20, radalt_snr);
    _mav_put_uint8_t(buf, 21, radalt_validity);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_AIRDATA, buf, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#else
    mavlink_ldm_airdata_t packet;
    packet.time_ms = time_ms;
    packet.ads9_alt_baro = ads9_alt_baro;
    packet.ads9_cas = ads9_cas;
    packet.px4_cas = px4_cas;
    packet.px4_cas_1 = px4_cas_1;
    packet.px4_cas_2 = px4_cas_2;
    packet.radalt = radalt;
    packet.validity = validity;
    packet.ads9_oat = ads9_oat;
    packet.radalt_snr = radalt_snr;
    packet.radalt_validity = radalt_validity;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_AIRDATA, (const char *)&packet, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#endif
}

/**
 * @brief Send a ldm_airdata message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_airdata_send_struct(mavlink_channel_t chan, const mavlink_ldm_airdata_t* ldm_airdata)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_airdata_send(chan, ldm_airdata->time_ms, ldm_airdata->validity, ldm_airdata->ads9_cas, ldm_airdata->ads9_alt_baro, ldm_airdata->ads9_oat, ldm_airdata->px4_cas, ldm_airdata->px4_cas_1, ldm_airdata->px4_cas_2, ldm_airdata->radalt, ldm_airdata->radalt_snr, ldm_airdata->radalt_validity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_AIRDATA, (const char *)ldm_airdata, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_AIRDATA_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_airdata_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, uint8_t validity, int16_t ads9_cas, int32_t ads9_alt_baro, int8_t ads9_oat, int16_t px4_cas, int16_t px4_cas_1, int16_t px4_cas_2, uint16_t radalt, uint8_t radalt_snr, uint8_t radalt_validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int32_t(buf, 4, ads9_alt_baro);
    _mav_put_int16_t(buf, 8, ads9_cas);
    _mav_put_int16_t(buf, 10, px4_cas);
    _mav_put_int16_t(buf, 12, px4_cas_1);
    _mav_put_int16_t(buf, 14, px4_cas_2);
    _mav_put_uint16_t(buf, 16, radalt);
    _mav_put_uint8_t(buf, 18, validity);
    _mav_put_int8_t(buf, 19, ads9_oat);
    _mav_put_uint8_t(buf, 20, radalt_snr);
    _mav_put_uint8_t(buf, 21, radalt_validity);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_AIRDATA, buf, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#else
    mavlink_ldm_airdata_t *packet = (mavlink_ldm_airdata_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->ads9_alt_baro = ads9_alt_baro;
    packet->ads9_cas = ads9_cas;
    packet->px4_cas = px4_cas;
    packet->px4_cas_1 = px4_cas_1;
    packet->px4_cas_2 = px4_cas_2;
    packet->radalt = radalt;
    packet->validity = validity;
    packet->ads9_oat = ads9_oat;
    packet->radalt_snr = radalt_snr;
    packet->radalt_validity = radalt_validity;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_AIRDATA, (const char *)packet, MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_LEN, MAVLINK_MSG_ID_LDM_AIRDATA_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_AIRDATA UNPACKING


/**
 * @brief Get field time_ms from ldm_airdata message
 *
 * @return  FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_airdata_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field validity from ldm_airdata message
 *
 * @return  validity flags
 */
static inline uint8_t mavlink_msg_ldm_airdata_get_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field ads9_cas from ldm_airdata message
 *
 * @return [cm/s] airspeed data from ADS9
 */
static inline int16_t mavlink_msg_ldm_airdata_get_ads9_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field ads9_alt_baro from ldm_airdata message
 *
 * @return [mm] ADS9 barometric altitude
 */
static inline int32_t mavlink_msg_ldm_airdata_get_ads9_alt_baro(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field ads9_oat from ldm_airdata message
 *
 * @return [deg C] outside air temperature ADS9
 */
static inline int8_t mavlink_msg_ldm_airdata_get_ads9_oat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  19);
}

/**
 * @brief Get field px4_cas from ldm_airdata message
 *
 * @return [cm/s] voted airspeed data from PX4
 */
static inline int16_t mavlink_msg_ldm_airdata_get_px4_cas(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field px4_cas_1 from ldm_airdata message
 *
 * @return [cm/s] airspeed data from PX4 sensor 1
 */
static inline int16_t mavlink_msg_ldm_airdata_get_px4_cas_1(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field px4_cas_2 from ldm_airdata message
 *
 * @return [cm/s] airspeed data from PX4 sensor 2
 */
static inline int16_t mavlink_msg_ldm_airdata_get_px4_cas_2(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field radalt from ldm_airdata message
 *
 * @return [cm] radalt altitude
 */
static inline uint16_t mavlink_msg_ldm_airdata_get_radalt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field radalt_snr from ldm_airdata message
 *
 * @return [dB] radalt signal-to-noise ratio
 */
static inline uint8_t mavlink_msg_ldm_airdata_get_radalt_snr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field radalt_validity from ldm_airdata message
 *
 * @return  radalt timeout flag
 */
static inline uint8_t mavlink_msg_ldm_airdata_get_radalt_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Decode a ldm_airdata message into a struct
 *
 * @param msg The message to decode
 * @param ldm_airdata C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_airdata_decode(const mavlink_message_t* msg, mavlink_ldm_airdata_t* ldm_airdata)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_airdata->time_ms = mavlink_msg_ldm_airdata_get_time_ms(msg);
    ldm_airdata->ads9_alt_baro = mavlink_msg_ldm_airdata_get_ads9_alt_baro(msg);
    ldm_airdata->ads9_cas = mavlink_msg_ldm_airdata_get_ads9_cas(msg);
    ldm_airdata->px4_cas = mavlink_msg_ldm_airdata_get_px4_cas(msg);
    ldm_airdata->px4_cas_1 = mavlink_msg_ldm_airdata_get_px4_cas_1(msg);
    ldm_airdata->px4_cas_2 = mavlink_msg_ldm_airdata_get_px4_cas_2(msg);
    ldm_airdata->radalt = mavlink_msg_ldm_airdata_get_radalt(msg);
    ldm_airdata->validity = mavlink_msg_ldm_airdata_get_validity(msg);
    ldm_airdata->ads9_oat = mavlink_msg_ldm_airdata_get_ads9_oat(msg);
    ldm_airdata->radalt_snr = mavlink_msg_ldm_airdata_get_radalt_snr(msg);
    ldm_airdata->radalt_validity = mavlink_msg_ldm_airdata_get_radalt_validity(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_AIRDATA_LEN? msg->len : MAVLINK_MSG_ID_LDM_AIRDATA_LEN;
        memset(ldm_airdata, 0, MAVLINK_MSG_ID_LDM_AIRDATA_LEN);
    memcpy(ldm_airdata, _MAV_PAYLOAD(msg), len);
#endif
}
