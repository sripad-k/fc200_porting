#pragma once
// MESSAGE LDM_EPU_TMOTOR PACKING

#define MAVLINK_MSG_ID_LDM_EPU_TMOTOR 52041


typedef struct __mavlink_ldm_epu_tmotor_t {
 uint32_t time_ms; /*< [ms] FCC internal time when the msg is sent*/
 int16_t rpm[8]; /*<  motor rpm*/
 uint8_t voltage[8]; /*< [V] motor input voltage*/
 uint8_t current[8]; /*< [A] motor input current*/
 int8_t temp[8]; /*< [C] motor temperature*/
} mavlink_ldm_epu_tmotor_t;

#define MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN 44
#define MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN 44
#define MAVLINK_MSG_ID_52041_LEN 44
#define MAVLINK_MSG_ID_52041_MIN_LEN 44

#define MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC 107
#define MAVLINK_MSG_ID_52041_CRC 107

#define MAVLINK_MSG_LDM_EPU_TMOTOR_FIELD_RPM_LEN 8
#define MAVLINK_MSG_LDM_EPU_TMOTOR_FIELD_VOLTAGE_LEN 8
#define MAVLINK_MSG_LDM_EPU_TMOTOR_FIELD_CURRENT_LEN 8
#define MAVLINK_MSG_LDM_EPU_TMOTOR_FIELD_TEMP_LEN 8

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_EPU_TMOTOR { \
    52041, \
    "LDM_EPU_TMOTOR", \
    5, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_epu_tmotor_t, time_ms) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT16_T, 8, 4, offsetof(mavlink_ldm_epu_tmotor_t, rpm) }, \
         { "voltage", NULL, MAVLINK_TYPE_UINT8_T, 8, 20, offsetof(mavlink_ldm_epu_tmotor_t, voltage) }, \
         { "current", NULL, MAVLINK_TYPE_UINT8_T, 8, 28, offsetof(mavlink_ldm_epu_tmotor_t, current) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 8, 36, offsetof(mavlink_ldm_epu_tmotor_t, temp) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_EPU_TMOTOR { \
    "LDM_EPU_TMOTOR", \
    5, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_epu_tmotor_t, time_ms) }, \
         { "rpm", NULL, MAVLINK_TYPE_INT16_T, 8, 4, offsetof(mavlink_ldm_epu_tmotor_t, rpm) }, \
         { "voltage", NULL, MAVLINK_TYPE_UINT8_T, 8, 20, offsetof(mavlink_ldm_epu_tmotor_t, voltage) }, \
         { "current", NULL, MAVLINK_TYPE_UINT8_T, 8, 28, offsetof(mavlink_ldm_epu_tmotor_t, current) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 8, 36, offsetof(mavlink_ldm_epu_tmotor_t, temp) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_epu_tmotor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param rpm  motor rpm
 * @param voltage [V] motor input voltage
 * @param current [A] motor input current
 * @param temp [C] motor temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *rpm, const uint8_t *voltage, const uint8_t *current, const int8_t *temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t_array(buf, 4, rpm, 8);
    _mav_put_uint8_t_array(buf, 20, voltage, 8);
    _mav_put_uint8_t_array(buf, 28, current, 8);
    _mav_put_int8_t_array(buf, 36, temp, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#else
    mavlink_ldm_epu_tmotor_t packet;
    packet.time_ms = time_ms;
    mav_array_memcpy(packet.rpm, rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.voltage, voltage, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_EPU_TMOTOR;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
}

/**
 * @brief Pack a ldm_epu_tmotor message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param rpm  motor rpm
 * @param voltage [V] motor input voltage
 * @param current [A] motor input current
 * @param temp [C] motor temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *rpm, const uint8_t *voltage, const uint8_t *current, const int8_t *temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t_array(buf, 4, rpm, 8);
    _mav_put_uint8_t_array(buf, 20, voltage, 8);
    _mav_put_uint8_t_array(buf, 28, current, 8);
    _mav_put_int8_t_array(buf, 36, temp, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#else
    mavlink_ldm_epu_tmotor_t packet;
    packet.time_ms = time_ms;
    mav_array_memcpy(packet.rpm, rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.voltage, voltage, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_EPU_TMOTOR;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#endif
}

/**
 * @brief Pack a ldm_epu_tmotor message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param rpm  motor rpm
 * @param voltage [V] motor input voltage
 * @param current [A] motor input current
 * @param temp [C] motor temperature
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const int16_t *rpm,const uint8_t *voltage,const uint8_t *current,const int8_t *temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t_array(buf, 4, rpm, 8);
    _mav_put_uint8_t_array(buf, 20, voltage, 8);
    _mav_put_uint8_t_array(buf, 28, current, 8);
    _mav_put_int8_t_array(buf, 36, temp, 8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#else
    mavlink_ldm_epu_tmotor_t packet;
    packet.time_ms = time_ms;
    mav_array_memcpy(packet.rpm, rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.voltage, voltage, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*8);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_EPU_TMOTOR;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
}

/**
 * @brief Encode a ldm_epu_tmotor struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_epu_tmotor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_epu_tmotor_t* ldm_epu_tmotor)
{
    return mavlink_msg_ldm_epu_tmotor_pack(system_id, component_id, msg, ldm_epu_tmotor->time_ms, ldm_epu_tmotor->rpm, ldm_epu_tmotor->voltage, ldm_epu_tmotor->current, ldm_epu_tmotor->temp);
}

/**
 * @brief Encode a ldm_epu_tmotor struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_epu_tmotor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_epu_tmotor_t* ldm_epu_tmotor)
{
    return mavlink_msg_ldm_epu_tmotor_pack_chan(system_id, component_id, chan, msg, ldm_epu_tmotor->time_ms, ldm_epu_tmotor->rpm, ldm_epu_tmotor->voltage, ldm_epu_tmotor->current, ldm_epu_tmotor->temp);
}

/**
 * @brief Encode a ldm_epu_tmotor struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_epu_tmotor C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_epu_tmotor_t* ldm_epu_tmotor)
{
    return mavlink_msg_ldm_epu_tmotor_pack_status(system_id, component_id, _status, msg,  ldm_epu_tmotor->time_ms, ldm_epu_tmotor->rpm, ldm_epu_tmotor->voltage, ldm_epu_tmotor->current, ldm_epu_tmotor->temp);
}

/**
 * @brief Send a ldm_epu_tmotor message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param rpm  motor rpm
 * @param voltage [V] motor input voltage
 * @param current [A] motor input current
 * @param temp [C] motor temperature
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_epu_tmotor_send(mavlink_channel_t chan, uint32_t time_ms, const int16_t *rpm, const uint8_t *voltage, const uint8_t *current, const int8_t *temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t_array(buf, 4, rpm, 8);
    _mav_put_uint8_t_array(buf, 20, voltage, 8);
    _mav_put_uint8_t_array(buf, 28, current, 8);
    _mav_put_int8_t_array(buf, 36, temp, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR, buf, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#else
    mavlink_ldm_epu_tmotor_t packet;
    packet.time_ms = time_ms;
    mav_array_memcpy(packet.rpm, rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet.voltage, voltage, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*8);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR, (const char *)&packet, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#endif
}

/**
 * @brief Send a ldm_epu_tmotor message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_epu_tmotor_send_struct(mavlink_channel_t chan, const mavlink_ldm_epu_tmotor_t* ldm_epu_tmotor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_epu_tmotor_send(chan, ldm_epu_tmotor->time_ms, ldm_epu_tmotor->rpm, ldm_epu_tmotor->voltage, ldm_epu_tmotor->current, ldm_epu_tmotor->temp);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR, (const char *)ldm_epu_tmotor, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_epu_tmotor_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const int16_t *rpm, const uint8_t *voltage, const uint8_t *current, const int8_t *temp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t_array(buf, 4, rpm, 8);
    _mav_put_uint8_t_array(buf, 20, voltage, 8);
    _mav_put_uint8_t_array(buf, 28, current, 8);
    _mav_put_int8_t_array(buf, 36, temp, 8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR, buf, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#else
    mavlink_ldm_epu_tmotor_t *packet = (mavlink_ldm_epu_tmotor_t *)msgbuf;
    packet->time_ms = time_ms;
    mav_array_memcpy(packet->rpm, rpm, sizeof(int16_t)*8);
    mav_array_memcpy(packet->voltage, voltage, sizeof(uint8_t)*8);
    mav_array_memcpy(packet->current, current, sizeof(uint8_t)*8);
    mav_array_memcpy(packet->temp, temp, sizeof(int8_t)*8);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_EPU_TMOTOR, (const char *)packet, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_EPU_TMOTOR UNPACKING


/**
 * @brief Get field time_ms from ldm_epu_tmotor message
 *
 * @return [ms] FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_epu_tmotor_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field rpm from ldm_epu_tmotor message
 *
 * @return  motor rpm
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_get_rpm(const mavlink_message_t* msg, int16_t *rpm)
{
    return _MAV_RETURN_int16_t_array(msg, rpm, 8,  4);
}

/**
 * @brief Get field voltage from ldm_epu_tmotor message
 *
 * @return [V] motor input voltage
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_get_voltage(const mavlink_message_t* msg, uint8_t *voltage)
{
    return _MAV_RETURN_uint8_t_array(msg, voltage, 8,  20);
}

/**
 * @brief Get field current from ldm_epu_tmotor message
 *
 * @return [A] motor input current
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_get_current(const mavlink_message_t* msg, uint8_t *current)
{
    return _MAV_RETURN_uint8_t_array(msg, current, 8,  28);
}

/**
 * @brief Get field temp from ldm_epu_tmotor message
 *
 * @return [C] motor temperature
 */
static inline uint16_t mavlink_msg_ldm_epu_tmotor_get_temp(const mavlink_message_t* msg, int8_t *temp)
{
    return _MAV_RETURN_int8_t_array(msg, temp, 8,  36);
}

/**
 * @brief Decode a ldm_epu_tmotor message into a struct
 *
 * @param msg The message to decode
 * @param ldm_epu_tmotor C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_epu_tmotor_decode(const mavlink_message_t* msg, mavlink_ldm_epu_tmotor_t* ldm_epu_tmotor)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_epu_tmotor->time_ms = mavlink_msg_ldm_epu_tmotor_get_time_ms(msg);
    mavlink_msg_ldm_epu_tmotor_get_rpm(msg, ldm_epu_tmotor->rpm);
    mavlink_msg_ldm_epu_tmotor_get_voltage(msg, ldm_epu_tmotor->voltage);
    mavlink_msg_ldm_epu_tmotor_get_current(msg, ldm_epu_tmotor->current);
    mavlink_msg_ldm_epu_tmotor_get_temp(msg, ldm_epu_tmotor->temp);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN? msg->len : MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN;
        memset(ldm_epu_tmotor, 0, MAVLINK_MSG_ID_LDM_EPU_TMOTOR_LEN);
    memcpy(ldm_epu_tmotor, _MAV_PAYLOAD(msg), len);
#endif
}
