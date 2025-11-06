#pragma once
// MESSAGE LDM_PILOT_INPUT PACKING

#define MAVLINK_MSG_ID_LDM_PILOT_INPUT 52006


typedef struct __mavlink_ldm_pilot_input_t {
 uint32_t time_ms; /*<  FCC internal time when the msg is sent*/
 int8_t roll_ch[2]; /*<  Roll channel [-100,100]*/
 int8_t pitch_ch[2]; /*<  Pitch channel [-100,100]*/
 int8_t thrust_ch[2]; /*<  Thrust channel [-100,100]*/
 int8_t yaw_ch[2]; /*<  Yaw channel [-100,100]*/
 int8_t pusher_ch[2]; /*<  Pusher channel [-100,100]*/
 int8_t switch_1[2]; /*<  Switch 1 channel [-100,100]*/
 int8_t switch_2[2]; /*<  Switch 2 channel [-100,100]*/
 int8_t arm_ch; /*<  Arm channel only for EP [-100,100]*/
 uint8_t rssi[2]; /*<  RSSI in percent [0,100]*/
 int8_t validity; /*<  Data validity flags*/
} mavlink_ldm_pilot_input_t;

#define MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN 22
#define MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN 22
#define MAVLINK_MSG_ID_52006_LEN 22
#define MAVLINK_MSG_ID_52006_MIN_LEN 22

#define MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC 90
#define MAVLINK_MSG_ID_52006_CRC 90

#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_ROLL_CH_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_PITCH_CH_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_THRUST_CH_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_YAW_CH_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_PUSHER_CH_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_SWITCH_1_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_SWITCH_2_LEN 2
#define MAVLINK_MSG_LDM_PILOT_INPUT_FIELD_RSSI_LEN 2

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_PILOT_INPUT { \
    52006, \
    "LDM_PILOT_INPUT", \
    11, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pilot_input_t, time_ms) }, \
         { "roll_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 4, offsetof(mavlink_ldm_pilot_input_t, roll_ch) }, \
         { "pitch_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 6, offsetof(mavlink_ldm_pilot_input_t, pitch_ch) }, \
         { "thrust_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 8, offsetof(mavlink_ldm_pilot_input_t, thrust_ch) }, \
         { "yaw_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 10, offsetof(mavlink_ldm_pilot_input_t, yaw_ch) }, \
         { "pusher_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 12, offsetof(mavlink_ldm_pilot_input_t, pusher_ch) }, \
         { "switch_1", NULL, MAVLINK_TYPE_INT8_T, 2, 14, offsetof(mavlink_ldm_pilot_input_t, switch_1) }, \
         { "switch_2", NULL, MAVLINK_TYPE_INT8_T, 2, 16, offsetof(mavlink_ldm_pilot_input_t, switch_2) }, \
         { "arm_ch", NULL, MAVLINK_TYPE_INT8_T, 0, 18, offsetof(mavlink_ldm_pilot_input_t, arm_ch) }, \
         { "rssi", NULL, MAVLINK_TYPE_UINT8_T, 2, 19, offsetof(mavlink_ldm_pilot_input_t, rssi) }, \
         { "validity", NULL, MAVLINK_TYPE_INT8_T, 0, 21, offsetof(mavlink_ldm_pilot_input_t, validity) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_PILOT_INPUT { \
    "LDM_PILOT_INPUT", \
    11, \
    {  { "time_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_ldm_pilot_input_t, time_ms) }, \
         { "roll_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 4, offsetof(mavlink_ldm_pilot_input_t, roll_ch) }, \
         { "pitch_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 6, offsetof(mavlink_ldm_pilot_input_t, pitch_ch) }, \
         { "thrust_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 8, offsetof(mavlink_ldm_pilot_input_t, thrust_ch) }, \
         { "yaw_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 10, offsetof(mavlink_ldm_pilot_input_t, yaw_ch) }, \
         { "pusher_ch", NULL, MAVLINK_TYPE_INT8_T, 2, 12, offsetof(mavlink_ldm_pilot_input_t, pusher_ch) }, \
         { "switch_1", NULL, MAVLINK_TYPE_INT8_T, 2, 14, offsetof(mavlink_ldm_pilot_input_t, switch_1) }, \
         { "switch_2", NULL, MAVLINK_TYPE_INT8_T, 2, 16, offsetof(mavlink_ldm_pilot_input_t, switch_2) }, \
         { "arm_ch", NULL, MAVLINK_TYPE_INT8_T, 0, 18, offsetof(mavlink_ldm_pilot_input_t, arm_ch) }, \
         { "rssi", NULL, MAVLINK_TYPE_UINT8_T, 2, 19, offsetof(mavlink_ldm_pilot_input_t, rssi) }, \
         { "validity", NULL, MAVLINK_TYPE_INT8_T, 0, 21, offsetof(mavlink_ldm_pilot_input_t, validity) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_pilot_input message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param roll_ch  Roll channel [-100,100]
 * @param pitch_ch  Pitch channel [-100,100]
 * @param thrust_ch  Thrust channel [-100,100]
 * @param yaw_ch  Yaw channel [-100,100]
 * @param pusher_ch  Pusher channel [-100,100]
 * @param switch_1  Switch 1 channel [-100,100]
 * @param switch_2  Switch 2 channel [-100,100]
 * @param arm_ch  Arm channel only for EP [-100,100]
 * @param rssi  RSSI in percent [0,100]
 * @param validity  Data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_ms, const int8_t *roll_ch, const int8_t *pitch_ch, const int8_t *thrust_ch, const int8_t *yaw_ch, const int8_t *pusher_ch, const int8_t *switch_1, const int8_t *switch_2, int8_t arm_ch, const uint8_t *rssi, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int8_t(buf, 18, arm_ch);
    _mav_put_int8_t(buf, 21, validity);
    _mav_put_int8_t_array(buf, 4, roll_ch, 2);
    _mav_put_int8_t_array(buf, 6, pitch_ch, 2);
    _mav_put_int8_t_array(buf, 8, thrust_ch, 2);
    _mav_put_int8_t_array(buf, 10, yaw_ch, 2);
    _mav_put_int8_t_array(buf, 12, pusher_ch, 2);
    _mav_put_int8_t_array(buf, 14, switch_1, 2);
    _mav_put_int8_t_array(buf, 16, switch_2, 2);
    _mav_put_uint8_t_array(buf, 19, rssi, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#else
    mavlink_ldm_pilot_input_t packet;
    packet.time_ms = time_ms;
    packet.arm_ch = arm_ch;
    packet.validity = validity;
    mav_array_memcpy(packet.roll_ch, roll_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pitch_ch, pitch_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.thrust_ch, thrust_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.yaw_ch, yaw_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pusher_ch, pusher_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_1, switch_1, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_2, switch_2, sizeof(int8_t)*2);
    mav_array_memcpy(packet.rssi, rssi, sizeof(uint8_t)*2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PILOT_INPUT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
}

/**
 * @brief Pack a ldm_pilot_input message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param roll_ch  Roll channel [-100,100]
 * @param pitch_ch  Pitch channel [-100,100]
 * @param thrust_ch  Thrust channel [-100,100]
 * @param yaw_ch  Yaw channel [-100,100]
 * @param pusher_ch  Pusher channel [-100,100]
 * @param switch_1  Switch 1 channel [-100,100]
 * @param switch_2  Switch 2 channel [-100,100]
 * @param arm_ch  Arm channel only for EP [-100,100]
 * @param rssi  RSSI in percent [0,100]
 * @param validity  Data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_ms, const int8_t *roll_ch, const int8_t *pitch_ch, const int8_t *thrust_ch, const int8_t *yaw_ch, const int8_t *pusher_ch, const int8_t *switch_1, const int8_t *switch_2, int8_t arm_ch, const uint8_t *rssi, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int8_t(buf, 18, arm_ch);
    _mav_put_int8_t(buf, 21, validity);
    _mav_put_int8_t_array(buf, 4, roll_ch, 2);
    _mav_put_int8_t_array(buf, 6, pitch_ch, 2);
    _mav_put_int8_t_array(buf, 8, thrust_ch, 2);
    _mav_put_int8_t_array(buf, 10, yaw_ch, 2);
    _mav_put_int8_t_array(buf, 12, pusher_ch, 2);
    _mav_put_int8_t_array(buf, 14, switch_1, 2);
    _mav_put_int8_t_array(buf, 16, switch_2, 2);
    _mav_put_uint8_t_array(buf, 19, rssi, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#else
    mavlink_ldm_pilot_input_t packet;
    packet.time_ms = time_ms;
    packet.arm_ch = arm_ch;
    packet.validity = validity;
    mav_array_memcpy(packet.roll_ch, roll_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pitch_ch, pitch_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.thrust_ch, thrust_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.yaw_ch, yaw_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pusher_ch, pusher_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_1, switch_1, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_2, switch_2, sizeof(int8_t)*2);
    mav_array_memcpy(packet.rssi, rssi, sizeof(uint8_t)*2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PILOT_INPUT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#endif
}

/**
 * @brief Pack a ldm_pilot_input message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_ms  FCC internal time when the msg is sent
 * @param roll_ch  Roll channel [-100,100]
 * @param pitch_ch  Pitch channel [-100,100]
 * @param thrust_ch  Thrust channel [-100,100]
 * @param yaw_ch  Yaw channel [-100,100]
 * @param pusher_ch  Pusher channel [-100,100]
 * @param switch_1  Switch 1 channel [-100,100]
 * @param switch_2  Switch 2 channel [-100,100]
 * @param arm_ch  Arm channel only for EP [-100,100]
 * @param rssi  RSSI in percent [0,100]
 * @param validity  Data validity flags
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_ms,const int8_t *roll_ch,const int8_t *pitch_ch,const int8_t *thrust_ch,const int8_t *yaw_ch,const int8_t *pusher_ch,const int8_t *switch_1,const int8_t *switch_2,int8_t arm_ch,const uint8_t *rssi,int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int8_t(buf, 18, arm_ch);
    _mav_put_int8_t(buf, 21, validity);
    _mav_put_int8_t_array(buf, 4, roll_ch, 2);
    _mav_put_int8_t_array(buf, 6, pitch_ch, 2);
    _mav_put_int8_t_array(buf, 8, thrust_ch, 2);
    _mav_put_int8_t_array(buf, 10, yaw_ch, 2);
    _mav_put_int8_t_array(buf, 12, pusher_ch, 2);
    _mav_put_int8_t_array(buf, 14, switch_1, 2);
    _mav_put_int8_t_array(buf, 16, switch_2, 2);
    _mav_put_uint8_t_array(buf, 19, rssi, 2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#else
    mavlink_ldm_pilot_input_t packet;
    packet.time_ms = time_ms;
    packet.arm_ch = arm_ch;
    packet.validity = validity;
    mav_array_memcpy(packet.roll_ch, roll_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pitch_ch, pitch_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.thrust_ch, thrust_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.yaw_ch, yaw_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pusher_ch, pusher_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_1, switch_1, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_2, switch_2, sizeof(int8_t)*2);
    mav_array_memcpy(packet.rssi, rssi, sizeof(uint8_t)*2);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_PILOT_INPUT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
}

/**
 * @brief Encode a ldm_pilot_input struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pilot_input C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_pilot_input_t* ldm_pilot_input)
{
    return mavlink_msg_ldm_pilot_input_pack(system_id, component_id, msg, ldm_pilot_input->time_ms, ldm_pilot_input->roll_ch, ldm_pilot_input->pitch_ch, ldm_pilot_input->thrust_ch, ldm_pilot_input->yaw_ch, ldm_pilot_input->pusher_ch, ldm_pilot_input->switch_1, ldm_pilot_input->switch_2, ldm_pilot_input->arm_ch, ldm_pilot_input->rssi, ldm_pilot_input->validity);
}

/**
 * @brief Encode a ldm_pilot_input struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pilot_input C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_pilot_input_t* ldm_pilot_input)
{
    return mavlink_msg_ldm_pilot_input_pack_chan(system_id, component_id, chan, msg, ldm_pilot_input->time_ms, ldm_pilot_input->roll_ch, ldm_pilot_input->pitch_ch, ldm_pilot_input->thrust_ch, ldm_pilot_input->yaw_ch, ldm_pilot_input->pusher_ch, ldm_pilot_input->switch_1, ldm_pilot_input->switch_2, ldm_pilot_input->arm_ch, ldm_pilot_input->rssi, ldm_pilot_input->validity);
}

/**
 * @brief Encode a ldm_pilot_input struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_pilot_input C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_pilot_input_t* ldm_pilot_input)
{
    return mavlink_msg_ldm_pilot_input_pack_status(system_id, component_id, _status, msg,  ldm_pilot_input->time_ms, ldm_pilot_input->roll_ch, ldm_pilot_input->pitch_ch, ldm_pilot_input->thrust_ch, ldm_pilot_input->yaw_ch, ldm_pilot_input->pusher_ch, ldm_pilot_input->switch_1, ldm_pilot_input->switch_2, ldm_pilot_input->arm_ch, ldm_pilot_input->rssi, ldm_pilot_input->validity);
}

/**
 * @brief Send a ldm_pilot_input message
 * @param chan MAVLink channel to send the message
 *
 * @param time_ms  FCC internal time when the msg is sent
 * @param roll_ch  Roll channel [-100,100]
 * @param pitch_ch  Pitch channel [-100,100]
 * @param thrust_ch  Thrust channel [-100,100]
 * @param yaw_ch  Yaw channel [-100,100]
 * @param pusher_ch  Pusher channel [-100,100]
 * @param switch_1  Switch 1 channel [-100,100]
 * @param switch_2  Switch 2 channel [-100,100]
 * @param arm_ch  Arm channel only for EP [-100,100]
 * @param rssi  RSSI in percent [0,100]
 * @param validity  Data validity flags
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_pilot_input_send(mavlink_channel_t chan, uint32_t time_ms, const int8_t *roll_ch, const int8_t *pitch_ch, const int8_t *thrust_ch, const int8_t *yaw_ch, const int8_t *pusher_ch, const int8_t *switch_1, const int8_t *switch_2, int8_t arm_ch, const uint8_t *rssi, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN];
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int8_t(buf, 18, arm_ch);
    _mav_put_int8_t(buf, 21, validity);
    _mav_put_int8_t_array(buf, 4, roll_ch, 2);
    _mav_put_int8_t_array(buf, 6, pitch_ch, 2);
    _mav_put_int8_t_array(buf, 8, thrust_ch, 2);
    _mav_put_int8_t_array(buf, 10, yaw_ch, 2);
    _mav_put_int8_t_array(buf, 12, pusher_ch, 2);
    _mav_put_int8_t_array(buf, 14, switch_1, 2);
    _mav_put_int8_t_array(buf, 16, switch_2, 2);
    _mav_put_uint8_t_array(buf, 19, rssi, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT, buf, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#else
    mavlink_ldm_pilot_input_t packet;
    packet.time_ms = time_ms;
    packet.arm_ch = arm_ch;
    packet.validity = validity;
    mav_array_memcpy(packet.roll_ch, roll_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pitch_ch, pitch_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.thrust_ch, thrust_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.yaw_ch, yaw_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.pusher_ch, pusher_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_1, switch_1, sizeof(int8_t)*2);
    mav_array_memcpy(packet.switch_2, switch_2, sizeof(int8_t)*2);
    mav_array_memcpy(packet.rssi, rssi, sizeof(uint8_t)*2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT, (const char *)&packet, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#endif
}

/**
 * @brief Send a ldm_pilot_input message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_pilot_input_send_struct(mavlink_channel_t chan, const mavlink_ldm_pilot_input_t* ldm_pilot_input)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_pilot_input_send(chan, ldm_pilot_input->time_ms, ldm_pilot_input->roll_ch, ldm_pilot_input->pitch_ch, ldm_pilot_input->thrust_ch, ldm_pilot_input->yaw_ch, ldm_pilot_input->pusher_ch, ldm_pilot_input->switch_1, ldm_pilot_input->switch_2, ldm_pilot_input->arm_ch, ldm_pilot_input->rssi, ldm_pilot_input->validity);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT, (const char *)ldm_pilot_input, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_pilot_input_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_ms, const int8_t *roll_ch, const int8_t *pitch_ch, const int8_t *thrust_ch, const int8_t *yaw_ch, const int8_t *pusher_ch, const int8_t *switch_1, const int8_t *switch_2, int8_t arm_ch, const uint8_t *rssi, int8_t validity)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_ms);
    _mav_put_int8_t(buf, 18, arm_ch);
    _mav_put_int8_t(buf, 21, validity);
    _mav_put_int8_t_array(buf, 4, roll_ch, 2);
    _mav_put_int8_t_array(buf, 6, pitch_ch, 2);
    _mav_put_int8_t_array(buf, 8, thrust_ch, 2);
    _mav_put_int8_t_array(buf, 10, yaw_ch, 2);
    _mav_put_int8_t_array(buf, 12, pusher_ch, 2);
    _mav_put_int8_t_array(buf, 14, switch_1, 2);
    _mav_put_int8_t_array(buf, 16, switch_2, 2);
    _mav_put_uint8_t_array(buf, 19, rssi, 2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT, buf, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#else
    mavlink_ldm_pilot_input_t *packet = (mavlink_ldm_pilot_input_t *)msgbuf;
    packet->time_ms = time_ms;
    packet->arm_ch = arm_ch;
    packet->validity = validity;
    mav_array_memcpy(packet->roll_ch, roll_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet->pitch_ch, pitch_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet->thrust_ch, thrust_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet->yaw_ch, yaw_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet->pusher_ch, pusher_ch, sizeof(int8_t)*2);
    mav_array_memcpy(packet->switch_1, switch_1, sizeof(int8_t)*2);
    mav_array_memcpy(packet->switch_2, switch_2, sizeof(int8_t)*2);
    mav_array_memcpy(packet->rssi, rssi, sizeof(uint8_t)*2);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_PILOT_INPUT, (const char *)packet, MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN, MAVLINK_MSG_ID_LDM_PILOT_INPUT_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_PILOT_INPUT UNPACKING


/**
 * @brief Get field time_ms from ldm_pilot_input message
 *
 * @return  FCC internal time when the msg is sent
 */
static inline uint32_t mavlink_msg_ldm_pilot_input_get_time_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field roll_ch from ldm_pilot_input message
 *
 * @return  Roll channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_roll_ch(const mavlink_message_t* msg, int8_t *roll_ch)
{
    return _MAV_RETURN_int8_t_array(msg, roll_ch, 2,  4);
}

/**
 * @brief Get field pitch_ch from ldm_pilot_input message
 *
 * @return  Pitch channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_pitch_ch(const mavlink_message_t* msg, int8_t *pitch_ch)
{
    return _MAV_RETURN_int8_t_array(msg, pitch_ch, 2,  6);
}

/**
 * @brief Get field thrust_ch from ldm_pilot_input message
 *
 * @return  Thrust channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_thrust_ch(const mavlink_message_t* msg, int8_t *thrust_ch)
{
    return _MAV_RETURN_int8_t_array(msg, thrust_ch, 2,  8);
}

/**
 * @brief Get field yaw_ch from ldm_pilot_input message
 *
 * @return  Yaw channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_yaw_ch(const mavlink_message_t* msg, int8_t *yaw_ch)
{
    return _MAV_RETURN_int8_t_array(msg, yaw_ch, 2,  10);
}

/**
 * @brief Get field pusher_ch from ldm_pilot_input message
 *
 * @return  Pusher channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_pusher_ch(const mavlink_message_t* msg, int8_t *pusher_ch)
{
    return _MAV_RETURN_int8_t_array(msg, pusher_ch, 2,  12);
}

/**
 * @brief Get field switch_1 from ldm_pilot_input message
 *
 * @return  Switch 1 channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_switch_1(const mavlink_message_t* msg, int8_t *switch_1)
{
    return _MAV_RETURN_int8_t_array(msg, switch_1, 2,  14);
}

/**
 * @brief Get field switch_2 from ldm_pilot_input message
 *
 * @return  Switch 2 channel [-100,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_switch_2(const mavlink_message_t* msg, int8_t *switch_2)
{
    return _MAV_RETURN_int8_t_array(msg, switch_2, 2,  16);
}

/**
 * @brief Get field arm_ch from ldm_pilot_input message
 *
 * @return  Arm channel only for EP [-100,100]
 */
static inline int8_t mavlink_msg_ldm_pilot_input_get_arm_ch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  18);
}

/**
 * @brief Get field rssi from ldm_pilot_input message
 *
 * @return  RSSI in percent [0,100]
 */
static inline uint16_t mavlink_msg_ldm_pilot_input_get_rssi(const mavlink_message_t* msg, uint8_t *rssi)
{
    return _MAV_RETURN_uint8_t_array(msg, rssi, 2,  19);
}

/**
 * @brief Get field validity from ldm_pilot_input message
 *
 * @return  Data validity flags
 */
static inline int8_t mavlink_msg_ldm_pilot_input_get_validity(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  21);
}

/**
 * @brief Decode a ldm_pilot_input message into a struct
 *
 * @param msg The message to decode
 * @param ldm_pilot_input C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_pilot_input_decode(const mavlink_message_t* msg, mavlink_ldm_pilot_input_t* ldm_pilot_input)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_pilot_input->time_ms = mavlink_msg_ldm_pilot_input_get_time_ms(msg);
    mavlink_msg_ldm_pilot_input_get_roll_ch(msg, ldm_pilot_input->roll_ch);
    mavlink_msg_ldm_pilot_input_get_pitch_ch(msg, ldm_pilot_input->pitch_ch);
    mavlink_msg_ldm_pilot_input_get_thrust_ch(msg, ldm_pilot_input->thrust_ch);
    mavlink_msg_ldm_pilot_input_get_yaw_ch(msg, ldm_pilot_input->yaw_ch);
    mavlink_msg_ldm_pilot_input_get_pusher_ch(msg, ldm_pilot_input->pusher_ch);
    mavlink_msg_ldm_pilot_input_get_switch_1(msg, ldm_pilot_input->switch_1);
    mavlink_msg_ldm_pilot_input_get_switch_2(msg, ldm_pilot_input->switch_2);
    ldm_pilot_input->arm_ch = mavlink_msg_ldm_pilot_input_get_arm_ch(msg);
    mavlink_msg_ldm_pilot_input_get_rssi(msg, ldm_pilot_input->rssi);
    ldm_pilot_input->validity = mavlink_msg_ldm_pilot_input_get_validity(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN? msg->len : MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN;
        memset(ldm_pilot_input, 0, MAVLINK_MSG_ID_LDM_PILOT_INPUT_LEN);
    memcpy(ldm_pilot_input, _MAV_PAYLOAD(msg), len);
#endif
}
