/** @file
 *    @brief MAVLink comm protocol testsuite generated from lodd_px4_nav.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef LODD_PX4_NAV_TESTSUITE_H
#define LODD_PX4_NAV_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_lodd_px4_nav(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_lodd_px4_nav(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_ldm_px4_nav(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_LDM_PX4_NAV >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_ldm_px4_nav_t packet_in = {
        963497464,{ 45.0, 46.0, 47.0 },{ 129.0, 130.0, 131.0 },{ 213.0, 214.0, 215.0 },{ 297.0, 298.0, 299.0 },963500168,963500376,437.0,465.0,493.0,521.0,21187,21291,21395,123,190,1
    };
    mavlink_ldm_px4_nav_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_ms = packet_in.time_ms;
        packet1.lat = packet_in.lat;
        packet1.lon = packet_in.lon;
        packet1.alt_amsl = packet_in.alt_amsl;
        packet1.gnss_h_acc = packet_in.gnss_h_acc;
        packet1.gnss_v_acc = packet_in.gnss_v_acc;
        packet1.alt_baro = packet_in.alt_baro;
        packet1.aspd1_cas = packet_in.aspd1_cas;
        packet1.aspd2_cas = packet_in.aspd2_cas;
        packet1.batt_v = packet_in.batt_v;
        packet1.gnss_n_sat = packet_in.gnss_n_sat;
        packet1.gnss_fix_type = packet_in.gnss_fix_type;
        packet1.validity = packet_in.validity;
        
        mav_array_memcpy(packet1.eul_rpy, packet_in.eul_rpy, sizeof(float)*3);
        mav_array_memcpy(packet1.omg, packet_in.omg, sizeof(float)*3);
        mav_array_memcpy(packet1.acc, packet_in.acc, sizeof(float)*3);
        mav_array_memcpy(packet1.vel_ned, packet_in.vel_ned, sizeof(float)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_LDM_PX4_NAV_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_px4_nav_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_ldm_px4_nav_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_px4_nav_pack(system_id, component_id, &msg , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.aspd1_cas , packet1.aspd2_cas , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.alt_baro , packet1.batt_v , packet1.validity );
    mavlink_msg_ldm_px4_nav_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_px4_nav_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.aspd1_cas , packet1.aspd2_cas , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.alt_baro , packet1.batt_v , packet1.validity );
    mavlink_msg_ldm_px4_nav_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_ldm_px4_nav_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_ldm_px4_nav_send(MAVLINK_COMM_1 , packet1.time_ms , packet1.eul_rpy , packet1.omg , packet1.acc , packet1.vel_ned , packet1.lat , packet1.lon , packet1.alt_amsl , packet1.aspd1_cas , packet1.aspd2_cas , packet1.gnss_n_sat , packet1.gnss_fix_type , packet1.gnss_h_acc , packet1.gnss_v_acc , packet1.alt_baro , packet1.batt_v , packet1.validity );
    mavlink_msg_ldm_px4_nav_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("LDM_PX4_NAV") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_LDM_PX4_NAV) != NULL);
#endif
}

static void mavlink_test_lodd_px4_nav(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_ldm_px4_nav(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // LODD_PX4_NAV_TESTSUITE_H
