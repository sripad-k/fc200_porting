#pragma once
// MESSAGE LDM_TR PACKING

#define MAVLINK_MSG_ID_LDM_TR 52003


typedef struct __mavlink_ldm_tr_t {
 int16_t thr_alloc; /*<  Allocated thrust in 1e-2 Kg*/
 int16_t ele; /*<  elevator in 1e-2 deg*/
 int16_t ail; /*<  aileron in 1e-2 deg*/
 int16_t rud; /*<  rudder in 1e-2 deg*/
 int16_t pusher; /*<  pusher command PWM 1000 to 2000*/
 int16_t roll_err; /*<  roll err in 1e-2 deg*/
 int16_t pitch_err; /*<  pitch err in 1e-2 deg*/
 uint8_t msg_seq; /*<  sequence of the message*/
 uint8_t ctrl_flags; /*<  A bitfield of various controller flags.*/
} mavlink_ldm_tr_t;

#define MAVLINK_MSG_ID_LDM_TR_LEN 16
#define MAVLINK_MSG_ID_LDM_TR_MIN_LEN 16
#define MAVLINK_MSG_ID_52003_LEN 16
#define MAVLINK_MSG_ID_52003_MIN_LEN 16

#define MAVLINK_MSG_ID_LDM_TR_CRC 154
#define MAVLINK_MSG_ID_52003_CRC 154



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_LDM_TR { \
    52003, \
    "LDM_TR", \
    9, \
    {  { "msg_seq", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_ldm_tr_t, msg_seq) }, \
         { "thr_alloc", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_ldm_tr_t, thr_alloc) }, \
         { "ele", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_ldm_tr_t, ele) }, \
         { "ail", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ldm_tr_t, ail) }, \
         { "rud", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_ldm_tr_t, rud) }, \
         { "pusher", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_tr_t, pusher) }, \
         { "roll_err", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_tr_t, roll_err) }, \
         { "pitch_err", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_tr_t, pitch_err) }, \
         { "ctrl_flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_ldm_tr_t, ctrl_flags) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_LDM_TR { \
    "LDM_TR", \
    9, \
    {  { "msg_seq", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_ldm_tr_t, msg_seq) }, \
         { "thr_alloc", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_ldm_tr_t, thr_alloc) }, \
         { "ele", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_ldm_tr_t, ele) }, \
         { "ail", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_ldm_tr_t, ail) }, \
         { "rud", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_ldm_tr_t, rud) }, \
         { "pusher", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_ldm_tr_t, pusher) }, \
         { "roll_err", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_ldm_tr_t, roll_err) }, \
         { "pitch_err", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_ldm_tr_t, pitch_err) }, \
         { "ctrl_flags", NULL, MAVLINK_TYPE_UINT8_T, 0, 15, offsetof(mavlink_ldm_tr_t, ctrl_flags) }, \
         } \
}
#endif

/**
 * @brief Pack a ldm_tr message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param msg_seq  sequence of the message
 * @param thr_alloc  Allocated thrust in 1e-2 Kg
 * @param ele  elevator in 1e-2 deg
 * @param ail  aileron in 1e-2 deg
 * @param rud  rudder in 1e-2 deg
 * @param pusher  pusher command PWM 1000 to 2000
 * @param roll_err  roll err in 1e-2 deg
 * @param pitch_err  pitch err in 1e-2 deg
 * @param ctrl_flags  A bitfield of various controller flags.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_tr_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t msg_seq, int16_t thr_alloc, int16_t ele, int16_t ail, int16_t rud, int16_t pusher, int16_t roll_err, int16_t pitch_err, uint8_t ctrl_flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_TR_LEN];
    _mav_put_int16_t(buf, 0, thr_alloc);
    _mav_put_int16_t(buf, 2, ele);
    _mav_put_int16_t(buf, 4, ail);
    _mav_put_int16_t(buf, 6, rud);
    _mav_put_int16_t(buf, 8, pusher);
    _mav_put_int16_t(buf, 10, roll_err);
    _mav_put_int16_t(buf, 12, pitch_err);
    _mav_put_uint8_t(buf, 14, msg_seq);
    _mav_put_uint8_t(buf, 15, ctrl_flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_TR_LEN);
#else
    mavlink_ldm_tr_t packet;
    packet.thr_alloc = thr_alloc;
    packet.ele = ele;
    packet.ail = ail;
    packet.rud = rud;
    packet.pusher = pusher;
    packet.roll_err = roll_err;
    packet.pitch_err = pitch_err;
    packet.msg_seq = msg_seq;
    packet.ctrl_flags = ctrl_flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_TR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_TR;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
}

/**
 * @brief Pack a ldm_tr message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param msg_seq  sequence of the message
 * @param thr_alloc  Allocated thrust in 1e-2 Kg
 * @param ele  elevator in 1e-2 deg
 * @param ail  aileron in 1e-2 deg
 * @param rud  rudder in 1e-2 deg
 * @param pusher  pusher command PWM 1000 to 2000
 * @param roll_err  roll err in 1e-2 deg
 * @param pitch_err  pitch err in 1e-2 deg
 * @param ctrl_flags  A bitfield of various controller flags.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_tr_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t msg_seq, int16_t thr_alloc, int16_t ele, int16_t ail, int16_t rud, int16_t pusher, int16_t roll_err, int16_t pitch_err, uint8_t ctrl_flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_TR_LEN];
    _mav_put_int16_t(buf, 0, thr_alloc);
    _mav_put_int16_t(buf, 2, ele);
    _mav_put_int16_t(buf, 4, ail);
    _mav_put_int16_t(buf, 6, rud);
    _mav_put_int16_t(buf, 8, pusher);
    _mav_put_int16_t(buf, 10, roll_err);
    _mav_put_int16_t(buf, 12, pitch_err);
    _mav_put_uint8_t(buf, 14, msg_seq);
    _mav_put_uint8_t(buf, 15, ctrl_flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_TR_LEN);
#else
    mavlink_ldm_tr_t packet;
    packet.thr_alloc = thr_alloc;
    packet.ele = ele;
    packet.ail = ail;
    packet.rud = rud;
    packet.pusher = pusher;
    packet.roll_err = roll_err;
    packet.pitch_err = pitch_err;
    packet.msg_seq = msg_seq;
    packet.ctrl_flags = ctrl_flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_TR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_TR;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN);
#endif
}

/**
 * @brief Pack a ldm_tr message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param msg_seq  sequence of the message
 * @param thr_alloc  Allocated thrust in 1e-2 Kg
 * @param ele  elevator in 1e-2 deg
 * @param ail  aileron in 1e-2 deg
 * @param rud  rudder in 1e-2 deg
 * @param pusher  pusher command PWM 1000 to 2000
 * @param roll_err  roll err in 1e-2 deg
 * @param pitch_err  pitch err in 1e-2 deg
 * @param ctrl_flags  A bitfield of various controller flags.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ldm_tr_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t msg_seq,int16_t thr_alloc,int16_t ele,int16_t ail,int16_t rud,int16_t pusher,int16_t roll_err,int16_t pitch_err,uint8_t ctrl_flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_TR_LEN];
    _mav_put_int16_t(buf, 0, thr_alloc);
    _mav_put_int16_t(buf, 2, ele);
    _mav_put_int16_t(buf, 4, ail);
    _mav_put_int16_t(buf, 6, rud);
    _mav_put_int16_t(buf, 8, pusher);
    _mav_put_int16_t(buf, 10, roll_err);
    _mav_put_int16_t(buf, 12, pitch_err);
    _mav_put_uint8_t(buf, 14, msg_seq);
    _mav_put_uint8_t(buf, 15, ctrl_flags);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_LDM_TR_LEN);
#else
    mavlink_ldm_tr_t packet;
    packet.thr_alloc = thr_alloc;
    packet.ele = ele;
    packet.ail = ail;
    packet.rud = rud;
    packet.pusher = pusher;
    packet.roll_err = roll_err;
    packet.pitch_err = pitch_err;
    packet.msg_seq = msg_seq;
    packet.ctrl_flags = ctrl_flags;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_LDM_TR_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_LDM_TR;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
}

/**
 * @brief Encode a ldm_tr struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ldm_tr C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_tr_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ldm_tr_t* ldm_tr)
{
    return mavlink_msg_ldm_tr_pack(system_id, component_id, msg, ldm_tr->msg_seq, ldm_tr->thr_alloc, ldm_tr->ele, ldm_tr->ail, ldm_tr->rud, ldm_tr->pusher, ldm_tr->roll_err, ldm_tr->pitch_err, ldm_tr->ctrl_flags);
}

/**
 * @brief Encode a ldm_tr struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ldm_tr C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_tr_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ldm_tr_t* ldm_tr)
{
    return mavlink_msg_ldm_tr_pack_chan(system_id, component_id, chan, msg, ldm_tr->msg_seq, ldm_tr->thr_alloc, ldm_tr->ele, ldm_tr->ail, ldm_tr->rud, ldm_tr->pusher, ldm_tr->roll_err, ldm_tr->pitch_err, ldm_tr->ctrl_flags);
}

/**
 * @brief Encode a ldm_tr struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param ldm_tr C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ldm_tr_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_ldm_tr_t* ldm_tr)
{
    return mavlink_msg_ldm_tr_pack_status(system_id, component_id, _status, msg,  ldm_tr->msg_seq, ldm_tr->thr_alloc, ldm_tr->ele, ldm_tr->ail, ldm_tr->rud, ldm_tr->pusher, ldm_tr->roll_err, ldm_tr->pitch_err, ldm_tr->ctrl_flags);
}

/**
 * @brief Send a ldm_tr message
 * @param chan MAVLink channel to send the message
 *
 * @param msg_seq  sequence of the message
 * @param thr_alloc  Allocated thrust in 1e-2 Kg
 * @param ele  elevator in 1e-2 deg
 * @param ail  aileron in 1e-2 deg
 * @param rud  rudder in 1e-2 deg
 * @param pusher  pusher command PWM 1000 to 2000
 * @param roll_err  roll err in 1e-2 deg
 * @param pitch_err  pitch err in 1e-2 deg
 * @param ctrl_flags  A bitfield of various controller flags.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ldm_tr_send(mavlink_channel_t chan, uint8_t msg_seq, int16_t thr_alloc, int16_t ele, int16_t ail, int16_t rud, int16_t pusher, int16_t roll_err, int16_t pitch_err, uint8_t ctrl_flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_LDM_TR_LEN];
    _mav_put_int16_t(buf, 0, thr_alloc);
    _mav_put_int16_t(buf, 2, ele);
    _mav_put_int16_t(buf, 4, ail);
    _mav_put_int16_t(buf, 6, rud);
    _mav_put_int16_t(buf, 8, pusher);
    _mav_put_int16_t(buf, 10, roll_err);
    _mav_put_int16_t(buf, 12, pitch_err);
    _mav_put_uint8_t(buf, 14, msg_seq);
    _mav_put_uint8_t(buf, 15, ctrl_flags);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_TR, buf, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#else
    mavlink_ldm_tr_t packet;
    packet.thr_alloc = thr_alloc;
    packet.ele = ele;
    packet.ail = ail;
    packet.rud = rud;
    packet.pusher = pusher;
    packet.roll_err = roll_err;
    packet.pitch_err = pitch_err;
    packet.msg_seq = msg_seq;
    packet.ctrl_flags = ctrl_flags;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_TR, (const char *)&packet, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#endif
}

/**
 * @brief Send a ldm_tr message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_ldm_tr_send_struct(mavlink_channel_t chan, const mavlink_ldm_tr_t* ldm_tr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_ldm_tr_send(chan, ldm_tr->msg_seq, ldm_tr->thr_alloc, ldm_tr->ele, ldm_tr->ail, ldm_tr->rud, ldm_tr->pusher, ldm_tr->roll_err, ldm_tr->pitch_err, ldm_tr->ctrl_flags);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_TR, (const char *)ldm_tr, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#endif
}

#if MAVLINK_MSG_ID_LDM_TR_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_ldm_tr_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t msg_seq, int16_t thr_alloc, int16_t ele, int16_t ail, int16_t rud, int16_t pusher, int16_t roll_err, int16_t pitch_err, uint8_t ctrl_flags)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int16_t(buf, 0, thr_alloc);
    _mav_put_int16_t(buf, 2, ele);
    _mav_put_int16_t(buf, 4, ail);
    _mav_put_int16_t(buf, 6, rud);
    _mav_put_int16_t(buf, 8, pusher);
    _mav_put_int16_t(buf, 10, roll_err);
    _mav_put_int16_t(buf, 12, pitch_err);
    _mav_put_uint8_t(buf, 14, msg_seq);
    _mav_put_uint8_t(buf, 15, ctrl_flags);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_TR, buf, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#else
    mavlink_ldm_tr_t *packet = (mavlink_ldm_tr_t *)msgbuf;
    packet->thr_alloc = thr_alloc;
    packet->ele = ele;
    packet->ail = ail;
    packet->rud = rud;
    packet->pusher = pusher;
    packet->roll_err = roll_err;
    packet->pitch_err = pitch_err;
    packet->msg_seq = msg_seq;
    packet->ctrl_flags = ctrl_flags;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_LDM_TR, (const char *)packet, MAVLINK_MSG_ID_LDM_TR_MIN_LEN, MAVLINK_MSG_ID_LDM_TR_LEN, MAVLINK_MSG_ID_LDM_TR_CRC);
#endif
}
#endif

#endif

// MESSAGE LDM_TR UNPACKING


/**
 * @brief Get field msg_seq from ldm_tr message
 *
 * @return  sequence of the message
 */
static inline uint8_t mavlink_msg_ldm_tr_get_msg_seq(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field thr_alloc from ldm_tr message
 *
 * @return  Allocated thrust in 1e-2 Kg
 */
static inline int16_t mavlink_msg_ldm_tr_get_thr_alloc(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  0);
}

/**
 * @brief Get field ele from ldm_tr message
 *
 * @return  elevator in 1e-2 deg
 */
static inline int16_t mavlink_msg_ldm_tr_get_ele(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  2);
}

/**
 * @brief Get field ail from ldm_tr message
 *
 * @return  aileron in 1e-2 deg
 */
static inline int16_t mavlink_msg_ldm_tr_get_ail(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field rud from ldm_tr message
 *
 * @return  rudder in 1e-2 deg
 */
static inline int16_t mavlink_msg_ldm_tr_get_rud(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field pusher from ldm_tr message
 *
 * @return  pusher command PWM 1000 to 2000
 */
static inline int16_t mavlink_msg_ldm_tr_get_pusher(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field roll_err from ldm_tr message
 *
 * @return  roll err in 1e-2 deg
 */
static inline int16_t mavlink_msg_ldm_tr_get_roll_err(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field pitch_err from ldm_tr message
 *
 * @return  pitch err in 1e-2 deg
 */
static inline int16_t mavlink_msg_ldm_tr_get_pitch_err(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field ctrl_flags from ldm_tr message
 *
 * @return  A bitfield of various controller flags.
 */
static inline uint8_t mavlink_msg_ldm_tr_get_ctrl_flags(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  15);
}

/**
 * @brief Decode a ldm_tr message into a struct
 *
 * @param msg The message to decode
 * @param ldm_tr C-struct to decode the message contents into
 */
static inline void mavlink_msg_ldm_tr_decode(const mavlink_message_t* msg, mavlink_ldm_tr_t* ldm_tr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    ldm_tr->thr_alloc = mavlink_msg_ldm_tr_get_thr_alloc(msg);
    ldm_tr->ele = mavlink_msg_ldm_tr_get_ele(msg);
    ldm_tr->ail = mavlink_msg_ldm_tr_get_ail(msg);
    ldm_tr->rud = mavlink_msg_ldm_tr_get_rud(msg);
    ldm_tr->pusher = mavlink_msg_ldm_tr_get_pusher(msg);
    ldm_tr->roll_err = mavlink_msg_ldm_tr_get_roll_err(msg);
    ldm_tr->pitch_err = mavlink_msg_ldm_tr_get_pitch_err(msg);
    ldm_tr->msg_seq = mavlink_msg_ldm_tr_get_msg_seq(msg);
    ldm_tr->ctrl_flags = mavlink_msg_ldm_tr_get_ctrl_flags(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_LDM_TR_LEN? msg->len : MAVLINK_MSG_ID_LDM_TR_LEN;
        memset(ldm_tr, 0, MAVLINK_MSG_ID_LDM_TR_LEN);
    memcpy(ldm_tr, _MAV_PAYLOAD(msg), len);
#endif
}
