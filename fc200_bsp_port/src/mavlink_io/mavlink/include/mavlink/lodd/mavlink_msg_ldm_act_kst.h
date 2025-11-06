#pragma once
// MESSAGE LDM_ACT_KST PACKING

#define MAVLINK_MSG_ID_LDM_ACT_KST 52031


typedef struct __mavlink_ldm_act_kst_t {
 uint32_t time_ms; /*<  FCC internal time when the msg is sent*/
 int16_t pos_deg[13]; /*< [degE2] servo deflection [-180,180] 10^2 deg*/
 int16_t lv_status; /*<  servo low voltage alarm bitfield*/
 uint8_t current[13]; /*< [A] servo current*/
 int8_t temp[13]; /*< [C] servo temperature*/
} mavlink_ldm_act_kst_t;

#define MAVLINK_MSG_ID_LDM_ACT_KST_LEN 58
#define MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN 58
#define MAVLINK_MSG_ID_52031_LEN 58
#define MAVLINK_MSG_ID_52031_MIN_LEN 58

#define MAVLINK_MSG_ID_LDM_ACT_KST_CRC 225
#define MAVLINK_MSG_ID_52031_CRC 225

#define MAVLINK_MSG_LDM_ACT_KST_FIELD_POS_DEG_LEN 13
#define MAVLINK_MSG_LDM_ACT_KST_FIELD_CURRENT_LEN 13
#define MAVLINK_MSG_LDM_ACT_KST_FIELD_TEMP_LEN 13

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_ACT_KST { \
    52031, \
    "LDM_ACT_KST", \
    5, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_act_kst_t, time_ms) }, \
         { "pos_deg", NULL, MAVLINK_TYPE_INT16_T, 13, 4, offsetof(mavlink_ldm_act_kst_t, pos_deg) }, \
         { "current", NULL, MAVLINK_TYPE_UINT8_T, 13, 32, offsetof(mavlink_ldm_act_kst_t, current) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 13, 45, offsetof(mavlink_ldm_act_kst_t, temp) }, \
         { "lv_status", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_ldm_act_kst_t, lv_status) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_ACT_KST { \
    "LDM_ACT_KST", \
    5, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_act_kst_t, time_ms) }, \
         { "pos_deg", NULL, MAVLINK_TYPE_INT16_T, 13, 4, offsetof(mavlink_ldm_act_kst_t, pos_deg) }, \
         { "current", NULL, MAVLINK_TYPE_UINT8_T, 13, 32, offsetof(mavlink_ldm_act_kst_t, current) }, \
         { "temp", NULL, MAVLINK_TYPE_INT8_T, 13, 45, offsetof(mavlink_ldm_act_kst_t, temp) }, \
         { "lv_status", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_ldm_act_kst_t, lv_status) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_act_kst message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param pos_deg [degE2] servo deflection [-180,180] 10^2 deg
 * @param current [A] servo current
 * @param temp [C] servo temperature
 * @param lv_status  servo low voltage alarm bitfield
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_kst_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *pos_deg, const uint8_t *current, const int8_t *temp, int16_t lv_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_KST_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 30, lv_status);
    _mav_put_int16_t_array(buf, 4, pos_deg, 13);
    _mav_put_uint8_t_array(buf, 32, current, 13);
    _mav_put_int8_t_array(buf, 45, temp, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#else
    mavlink_ldm_act_kst_t packet;
    packet.time_ms = time_ms;
    packet.lv_status = lv_status;
    mav_array_memcpy(packet.pos_deg, pos_deg, sizeof(int16_t)*13);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*13);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_KST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
}

/**
 * @brief Pack a ldm_act_kst message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param pos_deg [degE2] servo deflection [-180,180] 10^2 deg
 * @param current [A] servo current
 * @param temp [C] servo temperature
 * @param lv_status  servo low voltage alarm bitfield
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_kst_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const int16_t *pos_deg, const uint8_t *current, const int8_t *temp, int16_t lv_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_KST_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 30, lv_status);
    _mav_put_int16_t_array(buf, 4, pos_deg, 13);
    _mav_put_uint8_t_array(buf, 32, current, 13);
    _mav_put_int8_t_array(buf, 45, temp, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#else
    mavlink_ldm_act_kst_t packet;
    packet.time_ms = time_ms;
    packet.lv_status = lv_status;
    mav_array_memcpy(packet.pos_deg, pos_deg, sizeof(int16_t)*13);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*13);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_KST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#endif
}

/**
 * @brief Pack a ldm_act_kst message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  FCC internal time when the msg is sent
 * @param pos_deg [degE2] servo deflection [-180,180] 10^2 deg
 * @param current [A] servo current
 * @param temp [C] servo temperature
 * @param lv_status  servo low voltage alarm bitfield
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_kst_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const int16_t *pos_deg,const uint8_t *current,const int8_t *temp,int16_t lv_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_KST_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 30, lv_status);
    _mav_put_int16_t_array(buf, 4, pos_deg, 13);
    _mav_put_uint8_t_array(buf, 32, current, 13);
    _mav_put_int8_t_array(buf, 45, temp, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#else
    mavlink_ldm_act_kst_t packet;
    packet.time_ms = time_ms;
    packet.lv_status = lv_status;
    mav_array_memcpy(packet.pos_deg, pos_deg, sizeof(int16_t)*13);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*13);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_KST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
}

/**
 * @brief Encode a ldm_act_kst struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_kst C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_kst_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_act_kst_t* ldm_act_kst)
{
    return mavlink_msg_ldm_act_kst_pack(system_id, component_id, msg, ldm_act_kst->time_ms, ldm_act_kst->pos_deg, ldm_act_kst->current, ldm_act_kst->temp, ldm_act_kst->lv_status);
}

/**
 * @brief Encode a ldm_act_kst struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_kst C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_kst_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_act_kst_t* ldm_act_kst)
{
    return mavlink_msg_ldm_act_kst_pack_chan(system_id, component_id, chan, msg, ldm_act_kst->time_ms, ldm_act_kst->pos_deg, ldm_act_kst->current, ldm_act_kst->temp, ldm_act_kst->lv_status);
}

/**
 * @brief Encode a ldm_act_kst struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_kst C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_kst_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_act_kst_t* ldm_act_kst)
{
    return mavlink_msg_ldm_act_kst_pack_status(system_id, component_id, _status, msg,  ldm_act_kst->time_ms, ldm_act_kst->pos_deg, ldm_act_kst->current, ldm_act_kst->temp, ldm_act_kst->lv_status);
}

/**
 * @brief Send a ldm_act_kst message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param pos_deg [degE2] servo deflection [-180,180] 10^2 deg
 * @param current [A] servo current
 * @param temp [C] servo temperature
 * @param lv_status  servo low voltage alarm bitfield
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_act_kst_send(mavlink_channel_t chan, uint32_t time_ms, const int16_t *pos_deg, const uint8_t *current, const int8_t *temp, int16_t lv_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_KST_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 30, lv_status);
    _mav_put_int16_t_array(buf, 4, pos_deg, 13);
    _mav_put_uint8_t_array(buf, 32, current, 13);
    _mav_put_int8_t_array(buf, 45, temp, 13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_KST, buf, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#else
    mavlink_ldm_act_kst_t packet;
    packet.time_ms = time_ms;
    packet.lv_status = lv_status;
    mav_array_memcpy(packet.pos_deg, pos_deg, sizeof(int16_t)*13);
    mav_array_memcpy(packet.current, current, sizeof(uint8_t)*13);
    mav_array_memcpy(packet.temp, temp, sizeof(int8_t)*13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_KST, (const char *)&packet, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#endif
}

/**
 * @brief Send a ldm_act_kst message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_act_kst_send_struct(mavlink_channel_t chan, const mavlink_ldm_act_kst_t* ldm_act_kst)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_act_kst_send(chan, ldm_act_kst->time_ms, ldm_act_kst->pos_deg, ldm_act_kst->current, ldm_act_kst->temp, ldm_act_kst->lv_status);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_KST, (const char *)ldm_act_kst, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_ACT_KST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_act_kst_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const int16_t *pos_deg, const uint8_t *current, const int8_t *temp, int16_t lv_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 30, lv_status);
    _mav_put_int16_t_array(buf, 4, pos_deg, 13);
    _mav_put_uint8_t_array(buf, 32, current, 13);
    _mav_put_int8_t_array(buf, 45, temp, 13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_KST, buf, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#else
    mavlink_ldm_act_kst_t *packet = (mavlink_ldm_act_kst_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->lv_status = lv_status;
    mav_array_memcpy(packet->pos_deg, pos_deg, sizeof(int16_t)*13);
    mav_array_memcpy(packet->current, current, sizeof(uint8_t)*13);
    mav_array_memcpy(packet->temp, temp, sizeof(int8_t)*13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_KST, (const char *)packet, MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_LEN, MAVLINK_MSG_ID_LDM_ACT_KST_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_ACT_KST UNPACKING


/**
 * @brief Get field time_ms from ldm_act_kst message
 *
 * @return  FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_act_kst_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field pos_deg from ldm_act_kst message
 *
 * @return [degE2] servo deflection [-180,180] 10^2 deg
 */
static inline uint16_t mavlink_msg_ldm_act_kst_get_pos_deg(const mavlink_message_t* msg, int16_t *pos_deg)
{
    return _MAV_RETURN_int16_t_array(msg, pos_deg, 13,  4);
}

/**
 * @brief Get field current from ldm_act_kst message
 *
 * @return [A] servo current
 */
static inline uint16_t mavlink_msg_ldm_act_kst_get_current(const mavlink_message_t* msg, uint8_t *current)
{
    return _MAV_RETURN_uint8_t_array(msg, current, 13,  32);
}

/**
 * @brief Get field temp from ldm_act_kst message
 *
 * @return [C] servo temperature
 */
static inline uint16_t mavlink_msg_ldm_act_kst_get_temp(const mavlink_message_t* msg, int8_t *temp)
{
    return _MAV_RETURN_int8_t_array(msg, temp, 13,  45);
}

/**
 * @brief Get field lv_status from ldm_act_kst message
 *
 * @return  servo low voltage alarm bitfield
 */
static inline int16_t mavlink_msg_ldm_act_kst_get_lv_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  30);
}

/**
 * @brief Decode a ldm_act_kst message into a struct
 *
 * @param msg The message to decode
 * @param ldm_act_kst C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_act_kst_decode(const mavlink_message_t* msg, mavlink_ldm_act_kst_t* ldm_act_kst)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_act_kst->time_ms = mavlink_msg_ldm_act_kst_get_time_ms(msg);
    mavlink_msg_ldm_act_kst_get_pos_deg(msg, ldm_act_kst->pos_deg);
    ldm_act_kst->lv_status = mavlink_msg_ldm_act_kst_get_lv_status(msg);
    mavlink_msg_ldm_act_kst_get_current(msg, ldm_act_kst->current);
    mavlink_msg_ldm_act_kst_get_temp(msg, ldm_act_kst->temp);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_ACT_KST_LEN? msg->len : MAVLINK_MSG_ID_LDM_ACT_KST_LEN;
        memset(ldm_act_kst, 0, MAVLINK_MSG_ID_LDM_ACT_KST_LEN);
    memcpy(ldm_act_kst, _MAV_PAYLOAD(msg), len);
#endif
}
