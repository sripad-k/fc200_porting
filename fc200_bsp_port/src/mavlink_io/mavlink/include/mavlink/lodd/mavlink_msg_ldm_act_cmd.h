#pragma once
// MESSAGE LDM_ACT_CMD PACKING

#define MAVLINK_MSG_ID_LDM_ACT_CMD 52007


typedef struct __mavlink_ldm_act_cmd_t {
 uint32_t time_ms; /*< [ms] FCC internal time when the msg is sent*/
 uint16_t motor_cmd[8]; /*<  motor RPM cmd or cval (0 to 1000)*/
 int16_t servo_cmd[13]; /*< [1E2 deg] servo command in 1E2 deg*/
 uint16_t pusher_cmd; /*<  pusher motor/engine command*/
} mavlink_ldm_act_cmd_t;

#define MAVLINK_MSG_ID_LDM_ACT_CMD_LEN 48
#define MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN 48
#define MAVLINK_MSG_ID_52007_LEN 48
#define MAVLINK_MSG_ID_52007_MIN_LEN 48

#define MAVLINK_MSG_ID_LDM_ACT_CMD_CRC 142
#define MAVLINK_MSG_ID_52007_CRC 142

#define MAVLINK_MSG_LDM_ACT_CMD_FIELD_MOTOR_CMD_LEN 8
#define MAVLINK_MSG_LDM_ACT_CMD_FIELD_SERVO_CMD_LEN 13

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_ACT_CMD { \
    52007, \
    "LDM_ACT_CMD", \
    4, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_act_cmd_t, time_ms) }, \
         { "motor_cmd", NULL, MAVLINK_TYPE_UINT16_T, 8, 4, offsetof(mavlink_ldm_act_cmd_t, motor_cmd) }, \
         { "servo_cmd", NULL, MAVLINK_TYPE_INT16_T, 13, 20, offsetof(mavlink_ldm_act_cmd_t, servo_cmd) }, \
         { "pusher_cmd", NULL, MAVLINK_TYPE_UINT16_T, 0, 46, offsetof(mavlink_ldm_act_cmd_t, pusher_cmd) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_ACT_CMD { \
    "LDM_ACT_CMD", \
    4, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_act_cmd_t, time_ms) }, \
         { "motor_cmd", NULL, MAVLINK_TYPE_UINT16_T, 8, 4, offsetof(mavlink_ldm_act_cmd_t, motor_cmd) }, \
         { "servo_cmd", NULL, MAVLINK_TYPE_INT16_T, 13, 20, offsetof(mavlink_ldm_act_cmd_t, servo_cmd) }, \
         { "pusher_cmd", NULL, MAVLINK_TYPE_UINT16_T, 0, 46, offsetof(mavlink_ldm_act_cmd_t, pusher_cmd) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_act_cmd message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param motor_cmd  motor RPM cmd or cval (0 to 1000)
 * @param servo_cmd [1E2 deg] servo command in 1E2 deg
 * @param pusher_cmd  pusher motor/engine command
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const uint16_t *motor_cmd, const int16_t *servo_cmd, uint16_t pusher_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_CMD_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 46, pusher_cmd);
    _mav_put_uint16_t_array(buf, 4, motor_cmd, 8);
    _mav_put_int16_t_array(buf, 20, servo_cmd, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#else
    mavlink_ldm_act_cmd_t packet;
    packet.time_ms = time_ms;
    packet.pusher_cmd = pusher_cmd;
    mav_array_memcpy(packet.motor_cmd, motor_cmd, sizeof(uint16_t)*8);
    mav_array_memcpy(packet.servo_cmd, servo_cmd, sizeof(int16_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_CMD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
}

/**
 * @brief Pack a ldm_act_cmd message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param motor_cmd  motor RPM cmd or cval (0 to 1000)
 * @param servo_cmd [1E2 deg] servo command in 1E2 deg
 * @param pusher_cmd  pusher motor/engine command
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const uint16_t *motor_cmd, const int16_t *servo_cmd, uint16_t pusher_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_CMD_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 46, pusher_cmd);
    _mav_put_uint16_t_array(buf, 4, motor_cmd, 8);
    _mav_put_int16_t_array(buf, 20, servo_cmd, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#else
    mavlink_ldm_act_cmd_t packet;
    packet.time_ms = time_ms;
    packet.pusher_cmd = pusher_cmd;
    mav_array_memcpy(packet.motor_cmd, motor_cmd, sizeof(uint16_t)*8);
    mav_array_memcpy(packet.servo_cmd, servo_cmd, sizeof(int16_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_CMD;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#endif
}

/**
 * @brief Pack a ldm_act_cmd message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param motor_cmd  motor RPM cmd or cval (0 to 1000)
 * @param servo_cmd [1E2 deg] servo command in 1E2 deg
 * @param pusher_cmd  pusher motor/engine command
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const uint16_t *motor_cmd,const int16_t *servo_cmd,uint16_t pusher_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_CMD_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 46, pusher_cmd);
    _mav_put_uint16_t_array(buf, 4, motor_cmd, 8);
    _mav_put_int16_t_array(buf, 20, servo_cmd, 13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#else
    mavlink_ldm_act_cmd_t packet;
    packet.time_ms = time_ms;
    packet.pusher_cmd = pusher_cmd;
    mav_array_memcpy(packet.motor_cmd, motor_cmd, sizeof(uint16_t)*8);
    mav_array_memcpy(packet.servo_cmd, servo_cmd, sizeof(int16_t)*13);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_ACT_CMD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
}

/**
 * @brief Encode a ldm_act_cmd struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_act_cmd_t* ldm_act_cmd)
{
    return mavlink_msg_ldm_act_cmd_pack(system_id, component_id, msg, ldm_act_cmd->time_ms, ldm_act_cmd->motor_cmd, ldm_act_cmd->servo_cmd, ldm_act_cmd->pusher_cmd);
}

/**
 * @brief Encode a ldm_act_cmd struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_act_cmd_t* ldm_act_cmd)
{
    return mavlink_msg_ldm_act_cmd_pack_chan(system_id, component_id, chan, msg, ldm_act_cmd->time_ms, ldm_act_cmd->motor_cmd, ldm_act_cmd->servo_cmd, ldm_act_cmd->pusher_cmd);
}

/**
 * @brief Encode a ldm_act_cmd struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_act_cmd C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_act_cmd_t* ldm_act_cmd)
{
    return mavlink_msg_ldm_act_cmd_pack_status(system_id, component_id, _status, msg,  ldm_act_cmd->time_ms, ldm_act_cmd->motor_cmd, ldm_act_cmd->servo_cmd, ldm_act_cmd->pusher_cmd);
}

/**
 * @brief Send a ldm_act_cmd message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms [ms] FCC internal time when the msg is sent
 * @param motor_cmd  motor RPM cmd or cval (0 to 1000)
 * @param servo_cmd [1E2 deg] servo command in 1E2 deg
 * @param pusher_cmd  pusher motor/engine command
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_act_cmd_send(mavlink_channel_t chan, uint32_t time_ms, const uint16_t *motor_cmd, const int16_t *servo_cmd, uint16_t pusher_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_ACT_CMD_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 46, pusher_cmd);
    _mav_put_uint16_t_array(buf, 4, motor_cmd, 8);
    _mav_put_int16_t_array(buf, 20, servo_cmd, 13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_CMD, buf, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#else
    mavlink_ldm_act_cmd_t packet;
    packet.time_ms = time_ms;
    packet.pusher_cmd = pusher_cmd;
    mav_array_memcpy(packet.motor_cmd, motor_cmd, sizeof(uint16_t)*8);
    mav_array_memcpy(packet.servo_cmd, servo_cmd, sizeof(int16_t)*13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_CMD, (const char *)&packet, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#endif
}

/**
 * @brief Send a ldm_act_cmd message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_act_cmd_send_struct(mavlink_channel_t chan, const mavlink_ldm_act_cmd_t* ldm_act_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_act_cmd_send(chan, ldm_act_cmd->time_ms, ldm_act_cmd->motor_cmd, ldm_act_cmd->servo_cmd, ldm_act_cmd->pusher_cmd);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_CMD, (const char *)ldm_act_cmd, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_ACT_CMD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_act_cmd_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const uint16_t *motor_cmd, const int16_t *servo_cmd, uint16_t pusher_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 46, pusher_cmd);
    _mav_put_uint16_t_array(buf, 4, motor_cmd, 8);
    _mav_put_int16_t_array(buf, 20, servo_cmd, 13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_CMD, buf, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#else
    mavlink_ldm_act_cmd_t *packet = (mavlink_ldm_act_cmd_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->pusher_cmd = pusher_cmd;
    mav_array_memcpy(packet->motor_cmd, motor_cmd, sizeof(uint16_t)*8);
    mav_array_memcpy(packet->servo_cmd, servo_cmd, sizeof(int16_t)*13);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_ACT_CMD, (const char *)packet, MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN, MAVLINK_MSG_ID_LDM_ACT_CMD_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_ACT_CMD UNPACKING


/**
 * @brief Get field time_ms from ldm_act_cmd message
 *
 * @return [ms] FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_act_cmd_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field motor_cmd from ldm_act_cmd message
 *
 * @return  motor RPM cmd or cval (0 to 1000)
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_get_motor_cmd(const mavlink_message_t* msg, uint16_t *motor_cmd)
{
    return _MAV_RETURN_uint16_t_array(msg, motor_cmd, 8,  4);
}

/**
 * @brief Get field servo_cmd from ldm_act_cmd message
 *
 * @return [1E2 deg] servo command in 1E2 deg
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_get_servo_cmd(const mavlink_message_t* msg, int16_t *servo_cmd)
{
    return _MAV_RETURN_int16_t_array(msg, servo_cmd, 13,  20);
}

/**
 * @brief Get field pusher_cmd from ldm_act_cmd message
 *
 * @return  pusher motor/engine command
 */
static inline uint16_t mavlink_msg_ldm_act_cmd_get_pusher_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  46);
}

/**
 * @brief Decode a ldm_act_cmd message into a struct
 *
 * @param msg The message to decode
 * @param ldm_act_cmd C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_act_cmd_decode(const mavlink_message_t* msg, mavlink_ldm_act_cmd_t* ldm_act_cmd)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_act_cmd->time_ms = mavlink_msg_ldm_act_cmd_get_time_ms(msg);
    mavlink_msg_ldm_act_cmd_get_motor_cmd(msg, ldm_act_cmd->motor_cmd);
    mavlink_msg_ldm_act_cmd_get_servo_cmd(msg, ldm_act_cmd->servo_cmd);
    ldm_act_cmd->pusher_cmd = mavlink_msg_ldm_act_cmd_get_pusher_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_ACT_CMD_LEN? msg->len : MAVLINK_MSG_ID_LDM_ACT_CMD_LEN;
        memset(ldm_act_cmd, 0, MAVLINK_MSG_ID_LDM_ACT_CMD_LEN);
    memcpy(ldm_act_cmd, _MAV_PAYLOAD(msg), len);
#endif
}
