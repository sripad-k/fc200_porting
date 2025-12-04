/** @file
 *    @brief MAVLink comm protocol testsuite generated from lodd.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef LODD_TESTSUITE_H
#define LODD_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_lodd_px4_nav(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_lodd(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_lodd_px4_nav(system_id, component_id, last_msg);
    mavlink_test_lodd(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"
#include "../lodd_px4_nav/testsuite.h"


static void mavlink_test_ldm_vom_cmd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_VOM_CMD >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_vom_cmd_t packet_in = {
        5,72,{ 139, 140, 141 }
    };
    mavlink_ldm_vom_cmd_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        mav_array_memcpy(packet1.vom_cmd, packet_in.vom_cmd, sizeof(uint8_t)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_VOM_CMD_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_VOM_CMD_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_vom_cmd_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_vom_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_vom_cmd_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.vom_cmd );
    mavlink_msg_ldm_vom_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_vom_cmd_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.vom_cmd );
    mavlink_msg_ldm_vom_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_vom_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_vom_cmd_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.vom_cmd );
    mavlink_msg_ldm_vom_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_VOM_CMD") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_VOM_CMD) != NULL);
#endif
}

static void mavlink_test_ldm_ip_control(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_IP_CONTROL >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_ip_control_t packet_in = {
        963497464,17443,17547,17651,17755,17859,17963
    };
    mavlink_ldm_ip_control_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.roll = packet_in.roll;
        packet1.pitch = packet_in.pitch;
        packet1.thrust = packet_in.thrust;
        packet1.yaw = packet_in.yaw;
        packet1.pusher = packet_in.pusher;
        packet1.buttons = packet_in.buttons;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_IP_CONTROL_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ip_control_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_ip_control_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ip_control_pack(system_id, component_id, &msg , packet1.time_ms , packet1.roll , packet1.pitch , packet1.thrust , packet1.yaw , packet1.pusher , packet1.buttons );
    mavlink_msg_ldm_ip_control_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ip_control_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.roll , packet1.pitch , packet1.thrust , packet1.yaw , packet1.pusher , packet1.buttons );
    mavlink_msg_ldm_ip_control_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_ip_control_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ip_control_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.roll , packet1.pitch , packet1.thrust , packet1.yaw , packet1.pusher , packet1.buttons );
    mavlink_msg_ldm_ip_control_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_IP_CONTROL") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_IP_CONTROL) != NULL);
#endif
}

static void mavlink_test_ldm_tr(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_TR >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_tr_t packet_in = {
        17235,17339,17443,17547,17651,17755,17859,175,242
    };
    mavlink_ldm_tr_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.thr_alloc = packet_in.thr_alloc;
        packet1.ele = packet_in.ele;
        packet1.ail = packet_in.ail;
        packet1.rud = packet_in.rud;
        packet1.pusher = packet_in.pusher;
        packet1.roll_err = packet_in.roll_err;
        packet1.pitch_err = packet_in.pitch_err;
        packet1.msg_seq = packet_in.msg_seq;
        packet1.ctrl_flags = packet_in.ctrl_flags;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_TR_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_TR_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_tr_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_tr_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_tr_pack(system_id, component_id, &msg , packet1.msg_seq , packet1.thr_alloc , packet1.ele , packet1.ail , packet1.rud , packet1.pusher , packet1.roll_err , packet1.pitch_err , packet1.ctrl_flags );
    mavlink_msg_ldm_tr_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_tr_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.msg_seq , packet1.thr_alloc , packet1.ele , packet1.ail , packet1.rud , packet1.pusher , packet1.roll_err , packet1.pitch_err , packet1.ctrl_flags );
    mavlink_msg_ldm_tr_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_tr_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_tr_send(MAVLINK_COMM_1 , packet1.msg_seq , packet1.thr_alloc , packet1.ele , packet1.ail , packet1.rud , packet1.pusher , packet1.roll_err , packet1.pitch_err , packet1.ctrl_flags );
    mavlink_msg_ldm_tr_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_TR") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_TR) != NULL);
#endif
}

static void mavlink_test_ldm_airdata(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_AIRDATA >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_airdata_t packet_in = {
        963497464,963497672,17651,17755,17859,17963,18067,187,254,65,132
    };
    mavlink_ldm_airdata_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.ads9_alt_baro = packet_in.ads9_alt_baro;
        packet1.ads9_cas = packet_in.ads9_cas;
        packet1.px4_cas = packet_in.px4_cas;
        packet1.px4_cas_1 = packet_in.px4_cas_1;
        packet1.px4_cas_2 = packet_in.px4_cas_2;
        packet1.radalt = packet_in.radalt;
        packet1.validity = packet_in.validity;
        packet1.ads9_oat = packet_in.ads9_oat;
        packet1.radalt_snr = packet_in.radalt_snr;
        packet1.radalt_validity = packet_in.radalt_validity;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_AIRDATA_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_airdata_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_airdata_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_airdata_pack(system_id, component_id, &msg , packet1.time_ms , packet1.validity , packet1.ads9_cas , packet1.ads9_alt_baro , packet1.ads9_oat , packet1.px4_cas , packet1.px4_cas_1 , packet1.px4_cas_2 , packet1.radalt , packet1.radalt_snr , packet1.radalt_validity );
    mavlink_msg_ldm_airdata_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_airdata_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.validity , packet1.ads9_cas , packet1.ads9_alt_baro , packet1.ads9_oat , packet1.px4_cas , packet1.px4_cas_1 , packet1.px4_cas_2 , packet1.radalt , packet1.radalt_snr , packet1.radalt_validity );
    mavlink_msg_ldm_airdata_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_airdata_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_airdata_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.validity , packet1.ads9_cas , packet1.ads9_alt_baro , packet1.ads9_oat , packet1.px4_cas , packet1.px4_cas_1 , packet1.px4_cas_2 , packet1.radalt , packet1.radalt_snr , packet1.radalt_validity );
    mavlink_msg_ldm_airdata_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_AIRDATA") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_AIRDATA) != NULL);
#endif
}

static void mavlink_test_ldm_pilot_input(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_PILOT_INPUT >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_pilot_input_t packet_in = {
        963497464,{ 17, 18 },{ 151, 152 },{ 29, 30 },{ 163, 164 },{ 41, 42 },{ 175, 176 },{ 53, 54 },187,{ 254, 255 },132
    };
    mavlink_ldm_pilot_input_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.arm_ch = packet_in.arm_ch;
        packet1.validity = packet_in.validity;
        
        mav_array_memcpy(packet1.roll_ch, packet_in.roll_ch, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.pitch_ch, packet_in.pitch_ch, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.thrust_ch, packet_in.thrust_ch, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.yaw_ch, packet_in.yaw_ch, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.pusher_ch, packet_in.pusher_ch, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.switch_1, packet_in.switch_1, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.switch_2, packet_in.switch_2, sizeof(int8_t)*2);
        mav_array_memcpy(packet1.rssi, packet_in.rssi, sizeof(uint8_t)*2);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_PILOT_INPUT_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_pilot_input_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_pilot_input_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_pilot_input_pack(system_id, component_id, &msg , packet1.time_ms , packet1.roll_ch , packet1.pitch_ch , packet1.thrust_ch , packet1.yaw_ch , packet1.pusher_ch , packet1.switch_1 , packet1.switch_2 , packet1.arm_ch , packet1.rssi , packet1.validity );
    mavlink_msg_ldm_pilot_input_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_pilot_input_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.roll_ch , packet1.pitch_ch , packet1.thrust_ch , packet1.yaw_ch , packet1.pusher_ch , packet1.switch_1 , packet1.switch_2 , packet1.arm_ch , packet1.rssi , packet1.validity );
    mavlink_msg_ldm_pilot_input_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_pilot_input_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_pilot_input_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.roll_ch , packet1.pitch_ch , packet1.thrust_ch , packet1.yaw_ch , packet1.pusher_ch , packet1.switch_1 , packet1.switch_2 , packet1.arm_ch , packet1.rssi , packet1.validity );
    mavlink_msg_ldm_pilot_input_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_PILOT_INPUT") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_PILOT_INPUT) != NULL);
#endif
}

static void mavlink_test_ldm_act_cmd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_ACT_CMD >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_act_cmd_t packet_in = {
        963497464,{ 17443, 17444, 17445, 17446, 17447, 17448, 17449, 17450 },{ 18275, 18276, 18277, 18278, 18279, 18280, 18281, 18282, 18283, 18284, 18285, 18286, 18287 },19627
    };
    mavlink_ldm_act_cmd_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.pusher_cmd = packet_in.pusher_cmd;
        
        mav_array_memcpy(packet1.motor_cmd, packet_in.motor_cmd, sizeof(uint16_t)*8);
        mav_array_memcpy(packet1.servo_cmd, packet_in.servo_cmd, sizeof(int16_t)*13);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_ACT_CMD_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_cmd_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_act_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_cmd_pack(system_id, component_id, &msg , packet1.time_ms , packet1.motor_cmd , packet1.servo_cmd , packet1.pusher_cmd );
    mavlink_msg_ldm_act_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_cmd_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.motor_cmd , packet1.servo_cmd , packet1.pusher_cmd );
    mavlink_msg_ldm_act_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_act_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_cmd_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.motor_cmd , packet1.servo_cmd , packet1.pusher_cmd );
    mavlink_msg_ldm_act_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_ACT_CMD") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_ACT_CMD) != NULL);
#endif
}

static void mavlink_test_ldm_ins_il(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_INS_IL >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_ins_il_t packet_in = {
        963497464,{ 45.0, 46.0, 47.0 },{ 129.0, 130.0, 131.0 },963498920,963499128,269.0,297.0,{ 19523, 19524, 19525 },{ 19835, 19836, 19837 },20147,20251,{ 185, 186, 187 },{ 130, 131, 132 },75,142,209,20,87,154
    };
    mavlink_ldm_ins_il_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.lat = packet_in.lat;
        packet1.lon = packet_in.lon;
        packet1.alt_amsl = packet_in.alt_amsl;
        packet1.alt_baro = packet_in.alt_baro;
        packet1.gnss_h_acc = packet_in.gnss_h_acc;
        packet1.gnss_v_acc = packet_in.gnss_v_acc;
        packet1.ins_sol_status = packet_in.ins_sol_status;
        packet1.gnss_n_sat = packet_in.gnss_n_sat;
        packet1.gnss_fix_type = packet_in.gnss_fix_type;
        packet1.gnss_pdop = packet_in.gnss_pdop;
        packet1.temp = packet_in.temp;
        packet1.validity = packet_in.validity;
        
        mav_array_memcpy(packet1.omg, packet_in.omg, sizeof(float)*3);
        mav_array_memcpy(packet1.acc, packet_in.acc, sizeof(float)*3);
        mav_array_memcpy(packet1.eul_rpy, packet_in.eul_rpy, sizeof(int16_t)*3);
        mav_array_memcpy(packet1.vel_ned, packet_in.vel_ned, sizeof(int16_t)*3);
        mav_array_memcpy(packet1.kf_pos_cov_lla, packet_in.kf_pos_cov_lla, sizeof(uint8_t)*3);
        mav_array_memcpy(packet1.kf_vel_cov_ned, packet_in.kf_vel_cov_ned, sizeof(uint8_t)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_INS_IL_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ins_il_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_ins_il_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ins_il_pack(system_id, component_id, &msg , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.kf_pos_cov_lla , packet1.kf_vel_cov_ned , packet1.ins_sol_status , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.gnss_pdop , packet1.alt_baro , packet1.temp , packet1.validity );
    mavlink_msg_ldm_ins_il_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ins_il_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.kf_pos_cov_lla , packet1.kf_vel_cov_ned , packet1.ins_sol_status , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.gnss_pdop , packet1.alt_baro , packet1.temp , packet1.validity );
    mavlink_msg_ldm_ins_il_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_ins_il_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_ins_il_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.kf_pos_cov_lla , packet1.kf_vel_cov_ned , packet1.ins_sol_status , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.gnss_pdop , packet1.alt_baro , packet1.temp , packet1.validity );
    mavlink_msg_ldm_ins_il_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_INS_IL") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_INS_IL) != NULL);
#endif
}

static void mavlink_test_ldm_act_kst(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_ACT_KST >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_act_kst_t packet_in = {
        963497464,{ 17443, 17444, 17445, 17446, 17447, 17448, 17449, 17450, 17451, 17452, 17453, 17454, 17455 },18795,{ 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113 },{ 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216 }
    };
    mavlink_ldm_act_kst_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.lv_status = packet_in.lv_status;
        
        mav_array_memcpy(packet1.pos_deg, packet_in.pos_deg, sizeof(int16_t)*13);
        mav_array_memcpy(packet1.current, packet_in.current, sizeof(uint8_t)*13);
        mav_array_memcpy(packet1.temp, packet_in.temp, sizeof(int8_t)*13);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_ACT_KST_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_kst_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_act_kst_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_kst_pack(system_id, component_id, &msg , packet1.time_ms , packet1.pos_deg , packet1.current , packet1.temp , packet1.lv_status );
    mavlink_msg_ldm_act_kst_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_kst_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.pos_deg , packet1.current , packet1.temp , packet1.lv_status );
    mavlink_msg_ldm_act_kst_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_act_kst_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_act_kst_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.pos_deg , packet1.current , packet1.temp , packet1.lv_status );
    mavlink_msg_ldm_act_kst_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_ACT_KST") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_ACT_KST) != NULL);
#endif
}

static void mavlink_test_ldm_epu_tmotor(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_EPU_TMOTOR >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_epu_tmotor_t packet_in = {
        963497464,{ 17443, 17444, 17445, 17446, 17447, 17448, 17449, 17450 },{ 65, 66, 67, 68, 69, 70, 71, 72 },{ 89, 90, 91, 92, 93, 94, 95, 96 },{ 113, 114, 115, 116, 117, 118, 119, 120 }
    };
    mavlink_ldm_epu_tmotor_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        
        mav_array_memcpy(packet1.rpm, packet_in.rpm, sizeof(int16_t)*8);
        mav_array_memcpy(packet1.voltage, packet_in.voltage, sizeof(uint8_t)*8);
        mav_array_memcpy(packet1.current, packet_in.current, sizeof(uint8_t)*8);
        mav_array_memcpy(packet1.temp, packet_in.temp, sizeof(int8_t)*8);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_EPU_TMOTOR_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_epu_tmotor_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_epu_tmotor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_epu_tmotor_pack(system_id, component_id, &msg , packet1.time_ms , packet1.rpm , packet1.voltage , packet1.current , packet1.temp );
    mavlink_msg_ldm_epu_tmotor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_epu_tmotor_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.rpm , packet1.voltage , packet1.current , packet1.temp );
    mavlink_msg_ldm_epu_tmotor_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_epu_tmotor_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_epu_tmotor_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.rpm , packet1.voltage , packet1.current , packet1.temp );
    mavlink_msg_ldm_epu_tmotor_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_EPU_TMOTOR") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_EPU_TMOTOR) != NULL);
#endif
}

static void mavlink_test_lodd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_ldm_vom_cmd(system_id, component_id, last_msg);
    mavlink_test_ldm_ip_control(system_id, component_id, last_msg);
    mavlink_test_ldm_tr(system_id, component_id, last_msg);
    mavlink_test_ldm_airdata(system_id, component_id, last_msg);
    mavlink_test_ldm_pilot_input(system_id, component_id, last_msg);
    mavlink_test_ldm_act_cmd(system_id, component_id, last_msg);
    mavlink_test_ldm_ins_il(system_id, component_id, last_msg);
    mavlink_test_ldm_act_kst(system_id, component_id, last_msg);
    mavlink_test_ldm_epu_tmotor(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // LODD_TESTSUITE_H
