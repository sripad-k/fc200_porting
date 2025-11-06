#pragma once
// MESSAGE LDM_IP_CONTROL PACKING

#define MAVLINK_MSG_ID_LDM_IP_CONTROL 52002


typedef struct __mavlink_ldm_ip_control_t {
 uint32_t time_ms; /*<  time when the msg is sent*/
 int16_t roll; /*<  roll command -1000 to 1000*/
 int16_t pitch; /*<  pitch command -1000 to 1000*/
 int16_t thrust; /*<  thrust command -1000 to 1000*/
 int16_t yaw; /*<  yaw command -1000 to 1000*/
 int16_t pusher; /*<  pusher command -1000 to 1000*/
 int16_t buttons; /*<  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.*/
} mavlink_ldm_ip_control_t;

#define MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN 16
#define MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN 16
#define MAVLINK_MSG_ID_52002_LEN 16
#define MAVLINK_MSG_ID_52002_MIN_LEN 16

#define MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC 113
#define MAVLINK_MSG_ID_52002_CRC 113



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_IP_CONTROL { \
    52002, \
    "LDM_IP_CONTROL", \
    7, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_ip_control_t, time_ms) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ldm_ip_control_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_ldm_ip_control_t, pitch) }, \
         { "thrust", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_ip_control_t, thrust) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_ip_control_t, yaw) }, \
         { "pusher", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_ip_control_t, pusher) }, \
         { "buttons", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_ldm_ip_control_t, buttons) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_IP_CONTROL { \
    "LDM_IP_CONTROL", \
    7, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_ip_control_t, time_ms) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ldm_ip_control_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_ldm_ip_control_t, pitch) }, \
         { "thrust", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_ip_control_t, thrust) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_ip_control_t, yaw) }, \
         { "pusher", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_ip_control_t, pusher) }, \
         { "buttons", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_ldm_ip_control_t, buttons) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_ip_control message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  time when the msg is sent
 * @param roll  roll command -1000 to 1000
 * @param pitch  pitch command -1000 to 1000
 * @param thrust  thrust command -1000 to 1000
 * @param yaw  yaw command -1000 to 1000
 * @param pusher  pusher command -1000 to 1000
 * @param buttons  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ip_control_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, int16_t roll, int16_t pitch, int16_t thrust, int16_t yaw, int16_t pusher, int16_t buttons)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 4, roll);
    _mav_put_int16_t(buf, 6, pitch);
    _mav_put_int16_t(buf, 8, thrust);
    _mav_put_int16_t(buf, 10, yaw);
    _mav_put_int16_t(buf, 12, pusher);
    _mav_put_int16_t(buf, 14, buttons);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#else
    mavlink_ldm_ip_control_t packet;
    packet.time_ms = time_ms;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.thrust = thrust;
    packet.yaw = yaw;
    packet.pusher = pusher;
    packet.buttons = buttons;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_IP_CONTROL;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
}

/**
 * @brief Pack a ldm_ip_control message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  time when the msg is sent
 * @param roll  roll command -1000 to 1000
 * @param pitch  pitch command -1000 to 1000
 * @param thrust  thrust command -1000 to 1000
 * @param yaw  yaw command -1000 to 1000
 * @param pusher  pusher command -1000 to 1000
 * @param buttons  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ip_control_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, int16_t roll, int16_t pitch, int16_t thrust, int16_t yaw, int16_t pusher, int16_t buttons)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 4, roll);
    _mav_put_int16_t(buf, 6, pitch);
    _mav_put_int16_t(buf, 8, thrust);
    _mav_put_int16_t(buf, 10, yaw);
    _mav_put_int16_t(buf, 12, pusher);
    _mav_put_int16_t(buf, 14, buttons);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#else
    mavlink_ldm_ip_control_t packet;
    packet.time_ms = time_ms;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.thrust = thrust;
    packet.yaw = yaw;
    packet.pusher = pusher;
    packet.buttons = buttons;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_IP_CONTROL;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#endif
}

/**
 * @brief Pack a ldm_ip_control message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  time when the msg is sent
 * @param roll  roll command -1000 to 1000
 * @param pitch  pitch command -1000 to 1000
 * @param thrust  thrust command -1000 to 1000
 * @param yaw  yaw command -1000 to 1000
 * @param pusher  pusher command -1000 to 1000
 * @param buttons  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_ip_control_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,int16_t roll,int16_t pitch,int16_t thrust,int16_t yaw,int16_t pusher,int16_t buttons)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 4, roll);
    _mav_put_int16_t(buf, 6, pitch);
    _mav_put_int16_t(buf, 8, thrust);
    _mav_put_int16_t(buf, 10, yaw);
    _mav_put_int16_t(buf, 12, pusher);
    _mav_put_int16_t(buf, 14, buttons);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#else
    mavlink_ldm_ip_control_t packet;
    packet.time_ms = time_ms;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.thrust = thrust;
    packet.yaw = yaw;
    packet.pusher = pusher;
    packet.buttons = buttons;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_IP_CONTROL;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
}

/**
 * @brief Encode a ldm_ip_control struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ip_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ip_control_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_ip_control_t* ldm_ip_control)
{
    return mavlink_msg_ldm_ip_control_pack(system_id, component_id, msg, ldm_ip_control->time_ms, ldm_ip_control->roll, ldm_ip_control->pitch, ldm_ip_control->thrust, ldm_ip_control->yaw, ldm_ip_control->pusher, ldm_ip_control->buttons);
}

/**
 * @brief Encode a ldm_ip_control struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ip_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ip_control_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_ip_control_t* ldm_ip_control)
{
    return mavlink_msg_ldm_ip_control_pack_chan(system_id, component_id, chan, msg, ldm_ip_control->time_ms, ldm_ip_control->roll, ldm_ip_control->pitch, ldm_ip_control->thrust, ldm_ip_control->yaw, ldm_ip_control->pusher, ldm_ip_control->buttons);
}

/**
 * @brief Encode a ldm_ip_control struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_ip_control C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_ip_control_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_ip_control_t* ldm_ip_control)
{
    return mavlink_msg_ldm_ip_control_pack_status(system_id, component_id, _status, msg,  ldm_ip_control->time_ms, ldm_ip_control->roll, ldm_ip_control->pitch, ldm_ip_control->thrust, ldm_ip_control->yaw, ldm_ip_control->pusher, ldm_ip_control->buttons);
}

/**
 * @brief Send a ldm_ip_control message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  time when the msg is sent
 * @param roll  roll command -1000 to 1000
 * @param pitch  pitch command -1000 to 1000
 * @param thrust  thrust command -1000 to 1000
 * @param yaw  yaw command -1000 to 1000
 * @param pusher  pusher command -1000 to 1000
 * @param buttons  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_ip_control_send(mavlink_channel_t chan, uint32_t time_ms, int16_t roll, int16_t pitch, int16_t thrust, int16_t yaw, int16_t pusher, int16_t buttons)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 4, roll);
    _mav_put_int16_t(buf, 6, pitch);
    _mav_put_int16_t(buf, 8, thrust);
    _mav_put_int16_t(buf, 10, yaw);
    _mav_put_int16_t(buf, 12, pusher);
    _mav_put_int16_t(buf, 14, buttons);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_IP_CONTROL, buf, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#else
    mavlink_ldm_ip_control_t packet;
    packet.time_ms = time_ms;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.thrust = thrust;
    packet.yaw = yaw;
    packet.pusher = pusher;
    packet.buttons = buttons;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_IP_CONTROL, (const char *)&packet, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#endif
}

/**
 * @brief Send a ldm_ip_control message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_ip_control_send_struct(mavlink_channel_t chan, const mavlink_ldm_ip_control_t* ldm_ip_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_ip_control_send(chan, ldm_ip_control->time_ms, ldm_ip_control->roll, ldm_ip_control->pitch, ldm_ip_control->thrust, ldm_ip_control->yaw, ldm_ip_control->pusher, ldm_ip_control->buttons);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_IP_CONTROL, (const char *)ldm_ip_control, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_ip_control_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, int16_t roll, int16_t pitch, int16_t thrust, int16_t yaw, int16_t pusher, int16_t buttons)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int16_t(buf, 4, roll);
    _mav_put_int16_t(buf, 6, pitch);
    _mav_put_int16_t(buf, 8, thrust);
    _mav_put_int16_t(buf, 10, yaw);
    _mav_put_int16_t(buf, 12, pusher);
    _mav_put_int16_t(buf, 14, buttons);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_IP_CONTROL, buf, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#else
    mavlink_ldm_ip_control_t *packet = (mavlink_ldm_ip_control_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->roll = roll;
    packet->pitch = pitch;
    packet->thrust = thrust;
    packet->yaw = yaw;
    packet->pusher = pusher;
    packet->buttons = buttons;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_IP_CONTROL, (const char *)packet, MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN, MAVLINK_MSG_ID_LDM_IP_CONTROL_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_IP_CONTROL UNPACKING


/**
 * @brief Get field time_ms from ldm_ip_control message
 *
 * @return  time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_ip_control_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field roll from ldm_ip_control message
 *
 * @return  roll command -1000 to 1000
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field pitch from ldm_ip_control message
 *
 * @return  pitch command -1000 to 1000
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field thrust from ldm_ip_control message
 *
 * @return  thrust command -1000 to 1000
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_thrust(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field yaw from ldm_ip_control message
 *
 * @return  yaw command -1000 to 1000
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field pusher from ldm_ip_control message
 *
 * @return  pusher command -1000 to 1000
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_pusher(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field buttons from ldm_ip_control message
 *
 * @return  A bitfield corresponding to the joystick buttons'
      0-15 current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
 */
static inline int16_t mavlink_msg_ldm_ip_control_get_buttons(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Decode a ldm_ip_control message into a struct
 *
 * @param msg The message to decode
 * @param ldm_ip_control C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_ip_control_decode(const mavlink_message_t* msg, mavlink_ldm_ip_control_t* ldm_ip_control)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_ip_control->time_ms = mavlink_msg_ldm_ip_control_get_time_ms(msg);
    ldm_ip_control->roll = mavlink_msg_ldm_ip_control_get_roll(msg);
    ldm_ip_control->pitch = mavlink_msg_ldm_ip_control_get_pitch(msg);
    ldm_ip_control->thrust = mavlink_msg_ldm_ip_control_get_thrust(msg);
    ldm_ip_control->yaw = mavlink_msg_ldm_ip_control_get_yaw(msg);
    ldm_ip_control->pusher = mavlink_msg_ldm_ip_control_get_pusher(msg);
    ldm_ip_control->buttons = mavlink_msg_ldm_ip_control_get_buttons(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN? msg->len : MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN;
        memset(ldm_ip_control, 0, MAVLINK_MSG_ID_LDM_IP_CONTROL_LEN);
    memcpy(ldm_ip_control, _MAV_PAYLOAD(msg), len);
#endif
}
