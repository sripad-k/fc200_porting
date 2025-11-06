#pragma once
// MESSAGE LDM_PIL_CTRL PACKING

#define MAVLINK_MSG_ID_LDM_PIL_CTRL 52011


typedef struct __mavlink_ldm_pil_ctrl_t {
 uint32_t time_ms; /*< [ms] FCC timestamp when the msg was sent.*/
 float rotor_cmd_cval[8]; /*< [na] Lifter motor command 0 to 1*/
 float servo_cmd_deg[12]; /*< [deg] Servo cmd deg*/
 uint16_t pusher_pwm; /*< [usec] Pusher pwm*/
} mavlink_ldm_pil_ctrl_t;

#define MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN 86
#define MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN 86
#define MAVLINK_MSG_ID_52011_LEN 86
#define MAVLINK_MSG_ID_52011_MIN_LEN 86

#define MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC 237
#define MAVLINK_MSG_ID_52011_CRC 237

#define MAVLINK_MSG_LDM_PIL_CTRL_FIELD_ROTOR_CMD_CVAL_LEN 8
#define MAVLINK_MSG_LDM_PIL_CTRL_FIELD_SERVO_CMD_DEG_LEN 12

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_PIL_CTRL { \
    52011, \
    "LDM_PIL_CTRL", \
    4, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pil_ctrl_t, time_ms) }, \
         { "rotor_cmd_cval", NULL, MAVLINK_TYPE_FLOAT, 8, 4, offsetof(mavlink_ldm_pil_ctrl_t, rotor_cmd_cval) }, \
         { "servo_cmd_deg", NULL, MAVLINK_TYPE_FLOAT, 12, 36, offsetof(mavlink_ldm_pil_ctrl_t, servo_cmd_deg) }, \
         { "pusher_pwm", NULL, MAVLINK_TYPE_UINT16_T, 0, 84, offsetof(mavlink_ldm_pil_ctrl_t, pusher_pwm) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_PIL_CTRL { \
    "LDM_PIL_CTRL", \
    4, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pil_ctrl_t, time_ms) }, \
         { "rotor_cmd_cval", NULL, MAVLINK_TYPE_FLOAT, 8, 4, offsetof(mavlink_ldm_pil_ctrl_t, rotor_cmd_cval) }, \
         { "servo_cmd_deg", NULL, MAVLINK_TYPE_FLOAT, 12, 36, offsetof(mavlink_ldm_pil_ctrl_t, servo_cmd_deg) }, \
         { "pusher_pwm", NULL, MAVLINK_TYPE_UINT16_T, 0, 84, offsetof(mavlink_ldm_pil_ctrl_t, pusher_pwm) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_pil_ctrl message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC timestamp when the msg was sent.
 * @param rotor_cmd_cval [na] Lifter motor command 0 to 1
 * @param servo_cmd_deg [deg] Servo cmd deg
 * @param pusher_pwm [usec] Pusher pwm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const float *rotor_cmd_cval, const float *servo_cmd_deg, uint16_t pusher_pwm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 84, pusher_pwm);
    _mav_put_float_array(buf, 4, rotor_cmd_cval, 8);
    _mav_put_float_array(buf, 36, servo_cmd_deg, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#else
    mavlink_ldm_pil_ctrl_t packet;
    packet.time_ms = time_ms;
    packet.pusher_pwm = pusher_pwm;
    mav_array_memcpy(packet.rotor_cmd_cval, rotor_cmd_cval, sizeof(float)*8);
    mav_array_memcpy(packet.servo_cmd_deg, servo_cmd_deg, sizeof(float)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_CTRL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
}

/**
 * @brief Pack a ldm_pil_ctrl message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms [ms] FCC timestamp when the msg was sent.
 * @param rotor_cmd_cval [na] Lifter motor command 0 to 1
 * @param servo_cmd_deg [deg] Servo cmd deg
 * @param pusher_pwm [usec] Pusher pwm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const float *rotor_cmd_cval, const float *servo_cmd_deg, uint16_t pusher_pwm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 84, pusher_pwm);
    _mav_put_float_array(buf, 4, rotor_cmd_cval, 8);
    _mav_put_float_array(buf, 36, servo_cmd_deg, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#else
    mavlink_ldm_pil_ctrl_t packet;
    packet.time_ms = time_ms;
    packet.pusher_pwm = pusher_pwm;
    mav_array_memcpy(packet.rotor_cmd_cval, rotor_cmd_cval, sizeof(float)*8);
    mav_array_memcpy(packet.servo_cmd_deg, servo_cmd_deg, sizeof(float)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_CTRL;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#endif
}

/**
 * @brief Pack a ldm_pil_ctrl message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms [ms] FCC timestamp when the msg was sent.
 * @param rotor_cmd_cval [na] Lifter motor command 0 to 1
 * @param servo_cmd_deg [deg] Servo cmd deg
 * @param pusher_pwm [usec] Pusher pwm
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const float *rotor_cmd_cval,const float *servo_cmd_deg,uint16_t pusher_pwm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 84, pusher_pwm);
    _mav_put_float_array(buf, 4, rotor_cmd_cval, 8);
    _mav_put_float_array(buf, 36, servo_cmd_deg, 12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#else
    mavlink_ldm_pil_ctrl_t packet;
    packet.time_ms = time_ms;
    packet.pusher_pwm = pusher_pwm;
    mav_array_memcpy(packet.rotor_cmd_cval, rotor_cmd_cval, sizeof(float)*8);
    mav_array_memcpy(packet.servo_cmd_deg, servo_cmd_deg, sizeof(float)*12);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PIL_CTRL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
}

/**
 * @brief Encode a ldm_pil_ctrl struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_ctrl C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_pil_ctrl_t* ldm_pil_ctrl)
{
    return mavlink_msg_ldm_pil_ctrl_pack(system_id, component_id, msg, ldm_pil_ctrl->time_ms, ldm_pil_ctrl->rotor_cmd_cval, ldm_pil_ctrl->servo_cmd_deg, ldm_pil_ctrl->pusher_pwm);
}

/**
 * @brief Encode a ldm_pil_ctrl struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_ctrl C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_pil_ctrl_t* ldm_pil_ctrl)
{
    return mavlink_msg_ldm_pil_ctrl_pack_chan(system_id, component_id, chan, msg, ldm_pil_ctrl->time_ms, ldm_pil_ctrl->rotor_cmd_cval, ldm_pil_ctrl->servo_cmd_deg, ldm_pil_ctrl->pusher_pwm);
}

/**
 * @brief Encode a ldm_pil_ctrl struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pil_ctrl C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_pil_ctrl_t* ldm_pil_ctrl)
{
    return mavlink_msg_ldm_pil_ctrl_pack_status(system_id, component_id, _status, msg,  ldm_pil_ctrl->time_ms, ldm_pil_ctrl->rotor_cmd_cval, ldm_pil_ctrl->servo_cmd_deg, ldm_pil_ctrl->pusher_pwm);
}

/**
 * @brief Send a ldm_pil_ctrl message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms [ms] FCC timestamp when the msg was sent.
 * @param rotor_cmd_cval [na] Lifter motor command 0 to 1
 * @param servo_cmd_deg [deg] Servo cmd deg
 * @param pusher_pwm [usec] Pusher pwm
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_pil_ctrl_send(mavlink_channel_t chan, uint32_t time_ms, const float *rotor_cmd_cval, const float *servo_cmd_deg, uint16_t pusher_pwm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 84, pusher_pwm);
    _mav_put_float_array(buf, 4, rotor_cmd_cval, 8);
    _mav_put_float_array(buf, 36, servo_cmd_deg, 12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_CTRL, buf, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#else
    mavlink_ldm_pil_ctrl_t packet;
    packet.time_ms = time_ms;
    packet.pusher_pwm = pusher_pwm;
    mav_array_memcpy(packet.rotor_cmd_cval, rotor_cmd_cval, sizeof(float)*8);
    mav_array_memcpy(packet.servo_cmd_deg, servo_cmd_deg, sizeof(float)*12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_CTRL, (const char *)&packet, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#endif
}

/**
 * @brief Send a ldm_pil_ctrl message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_pil_ctrl_send_struct(mavlink_channel_t chan, const mavlink_ldm_pil_ctrl_t* ldm_pil_ctrl)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_pil_ctrl_send(chan, ldm_pil_ctrl->time_ms, ldm_pil_ctrl->rotor_cmd_cval, ldm_pil_ctrl->servo_cmd_deg, ldm_pil_ctrl->pusher_pwm);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_CTRL, (const char *)ldm_pil_ctrl, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_pil_ctrl_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const float *rotor_cmd_cval, const float *servo_cmd_deg, uint16_t pusher_pwm)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_uint16_t(buf, 84, pusher_pwm);
    _mav_put_float_array(buf, 4, rotor_cmd_cval, 8);
    _mav_put_float_array(buf, 36, servo_cmd_deg, 12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_CTRL, buf, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#else
    mavlink_ldm_pil_ctrl_t *packet = (mavlink_ldm_pil_ctrl_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->pusher_pwm = pusher_pwm;
    mav_array_memcpy(packet->rotor_cmd_cval, rotor_cmd_cval, sizeof(float)*8);
    mav_array_memcpy(packet->servo_cmd_deg, servo_cmd_deg, sizeof(float)*12);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PIL_CTRL, (const char *)packet, MAVLINK_MSG_ID_LDM_PIL_CTRL_MIN_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN, MAVLINK_MSG_ID_LDM_PIL_CTRL_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_PIL_CTRL UNPACKING


/**
 * @brief Get field time_ms from ldm_pil_ctrl message
 *
 * @return [ms] FCC timestamp when the msg was sent.
 */
static inline uint32_t mavlink_msg_ldm_pil_ctrl_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field rotor_cmd_cval from ldm_pil_ctrl message
 *
 * @return [na] Lifter motor command 0 to 1
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_get_rotor_cmd_cval(const mavlink_message_t* msg, float *rotor_cmd_cval)
{
    return _MAV_RETURN_float_array(msg, rotor_cmd_cval, 8,  4);
}

/**
 * @brief Get field servo_cmd_deg from ldm_pil_ctrl message
 *
 * @return [deg] Servo cmd deg
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_get_servo_cmd_deg(const mavlink_message_t* msg, float *servo_cmd_deg)
{
    return _MAV_RETURN_float_array(msg, servo_cmd_deg, 12,  36);
}

/**
 * @brief Get field pusher_pwm from ldm_pil_ctrl message
 *
 * @return [usec] Pusher pwm
 */
static inline uint16_t mavlink_msg_ldm_pil_ctrl_get_pusher_pwm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  84);
}

/**
 * @brief Decode a ldm_pil_ctrl message into a struct
 *
 * @param msg The message to decode
 * @param ldm_pil_ctrl C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_pil_ctrl_decode(const mavlink_message_t* msg, mavlink_ldm_pil_ctrl_t* ldm_pil_ctrl)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_pil_ctrl->time_ms = mavlink_msg_ldm_pil_ctrl_get_time_ms(msg);
    mavlink_msg_ldm_pil_ctrl_get_rotor_cmd_cval(msg, ldm_pil_ctrl->rotor_cmd_cval);
    mavlink_msg_ldm_pil_ctrl_get_servo_cmd_deg(msg, ldm_pil_ctrl->servo_cmd_deg);
    ldm_pil_ctrl->pusher_pwm = mavlink_msg_ldm_pil_ctrl_get_pusher_pwm(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN? msg->len : MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN;
        memset(ldm_pil_ctrl, 0, MAVLINK_MSG_ID_LDM_PIL_CTRL_LEN);
    memcpy(ldm_pil_ctrl, _MAV_PAYLOAD(msg), len);
#endif
}
