/****************************************************
 *  mavlink_io.c
 *  Created on: 07-Jul-2025 12:41:23 PM
 *  Implementation of the Class mavlink_io
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "mavlink_io/mavlink/include/mavlink/lodd/mavlink.h"
#include "udp_interface.h"
#include "uart_interface.h"
#include "mavlink_io_types.h"
#include "mavlink_io.h"
#include "generic_util.h"
#include "math_util.h"
#include "timer_interface.h"
#include "fcs_mi_interface.h"
#include "da_interface.h"
#include "ach_interface.h"
#include "gpio_interface.h"
#include "mavlink_io/serdes/serdes_ss_log.h"

#ifndef OPS_GIT_HASH_BYTES
#define OPS_GIT_HASH_BYTES {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#ifndef MAVLINK_SUBMODULE_HASH_BYTES
#define MAVLINK_SUBMODULE_HASH_BYTES {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

#define LOOP_RATE 100U // 100 Hz
#define RATE_HEARTBEAT (LOOP_RATE / 1U)
#define RATE_ATTITUDE (LOOP_RATE / 5U)
#define RATE_GLOBAL_POS (LOOP_RATE / 4U)
#define RATE_AIR_DATA (LOOP_RATE / 5U)
#define RATE_PILOT_INPUT (LOOP_RATE / 2U)
#define RATE_INS_DATA (LOOP_RATE / 2U)

#define RATE_ADC_DATA (LOOP_RATE / 2U)
#define RATE_ACT_DATA (LOOP_RATE / 2U)
#define RATE_ACT_CMD_DATA (LOOP_RATE / 3U)
#define RATE_EPU_DATA (LOOP_RATE / 2U)
#define RATE_BATT_DATA (LOOP_RATE / 2U)
#define RATE_ECBU_DATA (LOOP_RATE / 2U)
#define RATE_PIL_CTRL (LOOP_RATE / 100U)
#define MSG_RX_BUF_LEN (10 * MAVLINK_MAX_PACKET_LEN)

#define SLOT1 (0U)
#define SLOT2 (1U)
#define SLOT3 (2U)
#define SLOT4 (3U)
#define SLOT5 (4U)
#define SLOT6 (5U)
#define SLOT7 (6U)
#define SLOT8 (7U)
#define SLOT9 (8U)
#define SLOT10 (9U)

#define MAX_GIT_SHORT_HASH_LEN (8)

#define POS_LAT_LONG_SCALING (1.0e7)

#define DEG2RAD_F 0.017453f
#define RAD2DEG_F 57.2957f


#define INTERNAL_PILOT_TIMEOUT (500) /* Half a second timeout for GCS internal pilot */
#define GCS_HEARTBEAT_TIMEOUT (4000) /* Four second timeout for GCS heartbeat */


static s_timer_data_t InternalPilotMonitor;
static s_timer_data_t GcsHeartbeatMonitor;

static mavlink_message_t send_msg;
static uint8_t msg_tx_buffer[MAVLINK_MAX_PACKET_LEN];
static uint8_t msg_rx_buffer[MSG_RX_BUF_LEN];

/* Get the Operational Software GIT Hash */
static const uint8_t OpsVer[MAX_GIT_SHORT_HASH_LEN] = OPS_GIT_HASH_BYTES;

/* Get the MAVLink Submodule GIT Hash */
static const uint8_t MavlinkVer[MAX_GIT_SHORT_HASH_LEN] = MAVLINK_SUBMODULE_HASH_BYTES;

static mavio_system_t MavioSystem =
    {
        .sys_id = 1,                          // System ID can be set as per requirement
        .comp_id = 1,                         // Component ID can be set as per requirement
        .sys_type = MAV_TYPE_VTOL_FIXEDROTOR, // Fixed wing type
        .autopilot_type = MAV_AUTOPILOT_PX4   // Generic autopilot type
};

// Log data
#define LOG_BUFFER_SIZE 1024
uint8_t LogBuffer[LOG_BUFFER_SIZE] = {0}; // Buffer to hold log data
serdes_ss_log_t SerdesLog = {0};
void send_log_data(void);

// GCS message handling
static void handle_gcs_message(const mavlink_message_t *msg, mavio_out_t *mavio_out);
static void handle_cmd_long(mavlink_command_long_t *cmd_long, mavio_out_t *mavio_out);
static void send_mavlink_msg_gcs(void);
static void send_gcs_hb(const mavio_in_t *mavio_in);
static void send_gcs_attitude(const mavio_in_t *mavio_in);
static void send_gcs_gps_pos(const mavio_in_t *mavio_in);
static void send_gcs_air_data(const mavio_in_t *mavio_in);
static void send_gcs_pilot_input(const mavio_in_t *mavio_in);
static void send_gcs_ins_il_data(const mavio_in_t *mavio_in);
static void send_gcs_act_cmd(const mavio_in_t *mavio_in);

// static void send_gcs_adc_9_data(const mavio_in_t *mavio_in);
static void send_gcs_act_kst_data(const mavio_in_t *mavio_in);
static void send_gcs_epu_tmotor_data(const mavio_in_t *mavio_in);
// static void send_gcs_batt_data(const mavio_in_t *mavio_in);
// static void send_gcs_ecbu_data(const mavio_in_t *mavio_in);
static void send_gcs_wp_info(mavio_in_t *mavio_in, mavio_out_t *mavio_out);
static void send_msg_over_gcs_link(const mavlink_message_t *msg);
static void check_gcs_comm_timer_expiry(void);

// PIL message handling
#ifdef PIL_BUILD_ENABLED
static void handle_pil_message(const mavlink_message_t *msg);
static void send_mavlink_msg_pil(void);
static void send_pil_ctrl_msg(void);
static void send_msg_over_pil_link(const mavlink_message_t *msg);
#endif // PIL_BUILD_ENABLED

// RPI logger message handling

// gather data from various sources and put them in MavioIn to be sent to GCS
static void mav_io_gather_data(mavio_in_t *mavio_in);

// Mission handling
void mavio_mission_init(mavio_wp_list_t *wp_list);
void handle_mission_items(mavlink_message_t *msg, mavio_wp_list_t *wp_list);

void mav_io_init(void)
{
    udp_setup_server();

    mavio_mission_init(&MavioOut.wp_list);

    util_memset(&MavioIn, 0, sizeof(MavioIn));
    util_memset(&MavioOut, 0, sizeof(MavioOut));

    timer_start(&InternalPilotMonitor, INTERNAL_PILOT_TIMEOUT);
    timer_start(&GcsHeartbeatMonitor, GCS_HEARTBEAT_TIMEOUT);
}

void mavlink_io_recv_periodic_uart(void)
{
    // mavlink_message_t msg = {0};
    mavlink_message_t gcs_msg = {0};
    mavlink_status_t status;

    int msg_len = uart_read(0, msg_rx_buffer, MSG_RX_BUF_LEN);
    // printf(" Message Length = %d : UDP Source: %d\r\n", msg_len, udp_src);
    if (msg_len > 0)
    {
        for (int i = 0; i < msg_len; i++)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0, msg_rx_buffer[i], &gcs_msg, &status))
            {
                handle_gcs_message(&gcs_msg, &MavioOut);
            }
        }
    }

    // handle mission items at fcs loop rate to account for timeout
    handle_mission_items(&gcs_msg, &MavioOut.wp_list);

    // check periodically expiry of timer
    check_gcs_comm_timer_expiry();
}

void mavlink_io_recv_periodic(void)
{
    mavlink_io_recv_periodic_uart();
    mavlink_message_t gcs_msg = {0};
    uint32_t msg_len = 0;
    mavlink_status_t status;
    static uint8_t previous_safety_status = 0;
    // receive upto 10 udp packets (mavlink msg) per tick
    for (int i = 0; i < 10; i++)
    {
        udp_receive(&msg_rx_buffer[0], &msg_len, UDP_SRC_GCS);
        // printf(" Message Length = %d : UDP Source: %d\r\n", msg_len, udp_src);
        if (msg_len > 0)
        {
            for (int gcd_idx = 0; gcd_idx < msg_len; gcd_idx++)
            {
                if (mavlink_parse_char(MAVLINK_COMM_0, msg_rx_buffer[gcd_idx], &gcs_msg, &status))
                {
                    handle_gcs_message(&gcs_msg, &MavioOut);
                }
            }
        }
    }

    // handle mission items at fcs loop rate to account for timeout
    // handle_mission_items(&gcs_msg, &MavioOut.wp_list);

    /* check periodically expiry of timer */
    // check_gcs_comm_timer_expiry();

    /* If Armed */
    if (MavioIn.safety_status == 1)
    {
        /* Latch the safety status to be used later */
        previous_safety_status = 1;
        /* Turn ON white strobe lights */
        gpio_white_strobe_lights_toggle(true);
        /* Turn ON red strobe lights */
        gpio_red_beacon_lights_toggle(true);
    }
    else
    {
        if (previous_safety_status == 1)
        {
            /* Turn OFF red beacon and white strobe lights */
            gpio_red_beacon_lights_toggle(false);
            /* Turn OFF the White Strobe Lights */
            gpio_white_strobe_lights_toggle(false);
            /* Reset the prior status */
            previous_safety_status = 0;
        }
    }

    /* Turn ON position lights */
    gpio_pos_lights_toggle(true);
}

void mavlink_io_send_periodic(void)
{
    mav_io_gather_data(&MavioIn);
    send_mavlink_msg_gcs();

#ifdef PIL_BUILD_ENABLED
    send_mavlink_msg_pil();
#endif // PIL_BUILD_ENABLED

    send_log_data();
}

void send_log_data(void)
{
    SerdesLog.logger_time_ms = timer_get_system_time_ms();

    // FCS data
    SerdesLog.fcs_vom_status = MavioIn.vom_status;
    SerdesLog.fcs_status_flags = 0;
    SerdesLog.fcs_status_flags |= (MavioIn.safety_status == 1) ? LDE_STATUS_ARMED : 0;
    SerdesLog.fcs_status_flags |= (MavioIn.pic_status == 1) ? LDE_STATUS_PIC_MODE : 0;
    SerdesLog.fcs_status_flags |= (MavioIn.in_air_status == 1) ? LDE_STATUS_IN_AIR : 0;
    SerdesLog.fcs_status_flags |= (MavioIn.ep_data_loss == 1) ? LDE_STATUS_RC_LINK_LOSS : 0;
    SerdesLog.fcs_status_flags |= (MavioIn.ip_data_loss == 1) ? LDE_STATUS_IP_LINK_LOSS : 0;
    SerdesLog.fcs_status_flags |= (MavioIn.gnss_loss == 1) ? LDE_STATUS_GPS_LOSS : 0;
    for (int i = 0; i < 3; i++)
    {
        SerdesLog.fcs_euler_rpy[i] = (int16_t)(wrap_pif(MavioIn.euler_rpy[i]) * RAD2DEG_F * 100.0f);
        SerdesLog.fcs_omg_xyz[i] = MavioIn.omg_xyz[i];
        SerdesLog.fcs_acc_xyz[i] = MavioIn.acc_xyz[i];
        SerdesLog.fcs_vel_ned[i] = (int16_t)(MavioIn.vel_ned[i] * 100.0f);
    }
    SerdesLog.fcs_lat = MavioIn.latitude;
    SerdesLog.fcs_lon = MavioIn.longitude;
    SerdesLog.fcs_alt_gps_amsl = MavioIn.alt_gps_amsl;
    SerdesLog.fcs_alt_radalt_filt = MavioIn.alt_radalt_filt;
    SerdesLog.fcs_alt_baro_amsl = MavioIn.alt_baro_amsl;
    SerdesLog.fcs_aspd_cas = MavioIn.aspd_cas;

    // FCS FB Controller Data
    fcs_mi_fbctrl_data_t fbctrl_data = {0};
    fcs_mi_get_fbctrl(&fbctrl_data);
    for (int i = 0; i < 3; i++)
    {
        SerdesLog.fcs_mr_eul_rpy_d[i] = (int16_t)(wrap_pif(fbctrl_data.fcs_mr_eul_rpy_d[i]) * RAD2DEG_F * 100.0f); // deg * 100, [-180.00, 180.00]
    }
    SerdesLog.fcs_mr_yawrate_d = fbctrl_data.fcs_mr_yawrate_d; // rad/s
    SerdesLog.fcs_mr_yaw_hold = fbctrl_data.fcs_mr_yaw_hold;
    for (int i = 0; i < 4; i++)
    {
        SerdesLog.fcs_ca_nu_des[i] = fbctrl_data.fcs_ca_nu_des[i];     // thrust, Mx, My, Mz
        SerdesLog.fcs_ca_nu_alloc[i] = fbctrl_data.fcs_ca_nu_alloc[i]; // thrust, Mx, My, Mz
    }
    for (int i = 0; i < 3; i++)
    {
        SerdesLog.fcs_ca_cerp[i] = (uint16_t)(fbctrl_data.fcs_ca_cerp[i] * 1000.0f); // Mx, My, Mz *1000
    }
    SerdesLog.fcs_mr_h_d = fbctrl_data.fcs_mr_h_d;       // m
    SerdesLog.fcs_mr_hdot_d = fbctrl_data.fcs_mr_hdot_d; // m/s
    SerdesLog.fcs_mr_h_hold = fbctrl_data.fcs_mr_h_hold;
    SerdesLog.fcs_fw_roll_d = (int16_t)(fbctrl_data.fcs_fw_roll_d * RAD2DEG_F * 100.0f);   // deg * 100, [-180.00, 180.00]
    SerdesLog.fcs_fw_pitch_d = (int16_t)(fbctrl_data.fcs_fw_pitch_d * RAD2DEG_F * 100.0f); // deg * 100, [-180.00, 180.00]
    SerdesLog.fcs_fw_h_d = fbctrl_data.fcs_fw_h_d;                                         // m
    SerdesLog.fcs_fw_cas_d = (uint16_t)(fbctrl_data.fcs_fw_cas_d * 100.0f);                // m/s * 100
    SerdesLog.fcs_mr_pit_intg = fbctrl_data.fcs_mr_pit_intg;                               // crossfeed
    SerdesLog.fcs_mr_pit_intg_sat = fbctrl_data.fcs_mr_pit_intg_sat;                       // crossfeed
    SerdesLog.fcs_fw_pit_intg = fbctrl_data.fcs_fw_pit_intg;                               // crossfeed
    SerdesLog.fcs_fw_pit_intg_sat = fbctrl_data.fcs_fw_pit_intg_sat;                       // crossfeed

    // Actuator Commands
    for (int i = 0; i < MAVIO_NUM_MOTORS; i++)
    {
        SerdesLog.cmd_rotor_cval[i] = (uint16_t)(MavioIn.motor_cmd[i] * 1000.0f); // Convert from CVal to (0-1000)
    }
    for (int i = 0; i < MAVIO_NUM_SERVOS - 1; i++)
    {
        SerdesLog.cmd_servo_deg[i] = (uint16_t)(MavioIn.servo_cmd[i] * 100.0f); // 100*degrees
    }
    SerdesLog.cmd_pusher_pwm = (uint16_t)(MavioIn.pusher_cmd); // PWM

    // INSD data
    SerdesLog.ins1_time_ms = timer_get_system_time_ms();
    for (int i = 0; i < 3; i++)
    {
        SerdesLog.ins1_euler_rpy[i] = (int16_t)(wrap_pif(MavioIn.ins_data[0].euler_rpy[i]) * RAD2DEG_F * 100.0f);
        SerdesLog.ins1_omg[i] = MavioIn.ins_data[0].omg[i];
        SerdesLog.ins1_acc[i] = MavioIn.ins_data[0].acc[i];
        SerdesLog.ins1_vel_ned[i] = (int16_t)(MavioIn.ins_data[0].vel_ned[i] * 100.0f);
    }
    SerdesLog.ins1_lat = MavioIn.ins_data[0].latitude;
    SerdesLog.ins1_lon = MavioIn.ins_data[0].longitude;
    SerdesLog.ins1_alt_amsl = MavioIn.ins_data[0].alt_amsl;
    SerdesLog.ins1_gnss_sat_used = MavioIn.ins_data[0].gnss_sat_used;
    SerdesLog.ins1_gnss_pdop = (uint16_t)(MavioIn.ins_data[0].gnss_hdop * 100.0f);
    SerdesLog.ins1_gnss_h_acc = (uint16_t)(MavioIn.ins_data[0].gnss_h_accuracy * 100.0f);
    SerdesLog.ins1_gnss_v_acc = (uint16_t)(MavioIn.ins_data[0].gnss_v_accuracy * 100.0f);
    SerdesLog.ins1_gnss_sol_type = MavioIn.ins_data[0].gnss_sol_type;
    uint8_t ins_sol_status;
    da_get_ins_il_ins_sol_status(&ins_sol_status);
    SerdesLog.ins1_ins_sol_status = ins_sol_status;
    uint8_t gnss_info1, gnss_info2;
    da_get_ins_il_gnss_info(&gnss_info1, &gnss_info2);
    SerdesLog.ins1_gnss_info1 = gnss_info1;
    SerdesLog.ins1_gnss_info2 = gnss_info2;
    da_get_ins_il_kf_cov(SerdesLog.ins1_kf_pos_cov_lla, SerdesLog.ins1_kf_vel_cov_ned);
    SerdesLog.ins1_flags = 0;
    SerdesLog.ins1_flags |= MavioIn.ins_data[0].data_timeout;
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].ins_healthy << 1);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].gyro_healthy << 2);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].acc_healthy << 3);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].gnss_healthy << 4);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].baro_healthy << 5);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].att_invalid << 6);
    SerdesLog.ins1_flags |= (MavioIn.ins_data[0].pos_invalid << 7);

    // ADS-9 data
    SerdesLog.ads_time_ms = timer_get_system_time_ms();
    SerdesLog.ads_aspd_cas = (int16_t)(MavioIn.adc_data[0].aspd_cas * 100.0f);
    SerdesLog.ads_aoa = (int16_t)(MavioIn.adc_data[0].aoa * 100.0f);
    SerdesLog.ads_aos = (int16_t)(MavioIn.adc_data[0].aos * 100.0f);
    SerdesLog.ads_aspd_tas = (int16_t)(MavioIn.adc_data[0].aspd_tas * 100.0f);
    SerdesLog.ads_alt_baro_amsl = MavioIn.adc_data[0].alt_baro_amsl;
    SerdesLog.ads_oat_celsius = (int16_t)(MavioIn.adc_data[0].oat_celsius * 100.0f);
    SerdesLog.ads_flags = 0;
    SerdesLog.ads_flags |= MavioIn.adc_data[0].data_timeout;
    SerdesLog.ads_flags |= (MavioIn.adc_data[0].aspd_cas_invalid << 1);
    SerdesLog.ads_flags |= (MavioIn.adc_data[0].aspd_tas_invalid << 2);
    SerdesLog.ads_flags |= (MavioIn.adc_data[0].aoa_invalid << 3);
    SerdesLog.ads_flags |= (MavioIn.adc_data[0].aos_invalid << 4);
    SerdesLog.ads_flags |= (MavioIn.adc_data[0].alt_baro_invalid << 5);

    // Radalt data
    SerdesLog.radalt_time_ms = timer_get_system_time_ms();
    SerdesLog.radalt_alt_raw = MavioIn.alt_radalt_raw;
    SerdesLog.radalt_snr = 0;   // TODO
    SerdesLog.radalt_flags = 0; // TODO

    // Pilot Input (EP:0 IP:1) data
    SerdesLog.pilot_time_ms = timer_get_system_time_ms();
    SerdesLog.pilot_roll[0] = (int16_t)(MavioIn.rc_input.axis_r * 1000.0f);
    SerdesLog.pilot_pitch[0] = (int16_t)(MavioIn.rc_input.axis_p * 1000.0f);
    SerdesLog.pilot_thrust[0] = (int16_t)(MavioIn.rc_input.axis_t * 1000.0f);
    SerdesLog.pilot_yaw[0] = (int16_t)(MavioIn.rc_input.axis_y * 1000.0f);
    SerdesLog.pilot_pusher[0] = (int16_t)(MavioIn.rc_input.slider_r * 1000.0f);
    SerdesLog.pilot_arm_ch[0] = (int16_t)(MavioIn.rc_input.knob_r * 1000.0f);
    SerdesLog.pilot_sw_a[0] = (int8_t)(MavioIn.rc_input.sw_a * 1000.0f);
    SerdesLog.pilot_sw_b[0] = (int8_t)(MavioIn.rc_input.sw_b * 1000.0f);
    SerdesLog.pilot_rssi = (int8_t)MavioIn.rc_input.rssi;
    SerdesLog.pilot_flags[0] = (MavioIn.rc_input.data_timeout | MavioIn.rc_input.link_lost << 1);
    SerdesLog.pilot_roll[1] = (int16_t)(MavioOut.ip_input.roll * 1000.0f);
    SerdesLog.pilot_pitch[1] = (int16_t)(MavioOut.ip_input.pitch * 1000.0f);
    SerdesLog.pilot_thrust[1] = (int16_t)(MavioOut.ip_input.thrust * 1000.0f);
    SerdesLog.pilot_yaw[1] = (int16_t)(MavioOut.ip_input.yaw * 1000.0f);
    SerdesLog.pilot_pusher[1] = (int16_t)(MavioOut.ip_input.pusher * 1000.0f);
    SerdesLog.pilot_flags[1] = MavioOut.ip_input.link_lost;

    // T-motor EPU data
    SerdesLog.epu_time_ms = timer_get_system_time_ms();
    for (int i = 0; i < 8; i++)
    {
        SerdesLog.epu_rpm[i] = (uint16_t)MavioIn.tmotor_data.rpm[i];
        SerdesLog.epu_voltage[i] = (uint8_t)MavioIn.tmotor_data.voltage[i];
        SerdesLog.epu_current[i] = (uint8_t)MavioIn.tmotor_data.current[i];
        SerdesLog.epu_temp[i] = (int8_t)MavioIn.tmotor_data.temp[i];
    }

    // KST servo data
    SerdesLog.servo_time_ms = timer_get_system_time_ms();
    for (int i = 0; i < 12; i++)
    {
        SerdesLog.servo_pos_deg[i] = (int16_t)(MavioIn.kst_data.pos_deg[i] * 100.0f); // 100*degrees
    }

    // Pack and Send Log Data
    int len = serdes_pack_ss_log(&SerdesLog, LogBuffer, LOG_BUFFER_SIZE);

    if (len > 0)
    {
        udp_send_rpi(&LogBuffer[0], len);
    }
}

/************************************************************
*************************************************************
GCS
*************************************************************
*************************************************************/

static void mav_io_gather_data(mavio_in_t *mavio_in)
{
    /* Update INS data */
    float eul_ang[3];
    float omg[3];
    float acc[3];
    double lat, lon;
    float alt;
    float v_ned[3];
    float eph, epv;
    uint8_t gnss_sat_used = 0;

    fcs_mi_get_fcs_dscr(
        &mavio_in->vom_status,
        &mavio_in->safety_status,
        &mavio_in->pic_status,
        &mavio_in->in_air_status,
        &mavio_in->ep_data_loss,
        &mavio_in->ip_data_loss,
        &mavio_in->gnss_loss,
        &mavio_in->ins_selection,
        &mavio_in->adc_selection,
        &mavio_in->current_waypoint_idx);

    fcs_mi_get_fcs_cont(
        mavio_in->euler_rpy,
        mavio_in->omg_xyz,
        mavio_in->acc_xyz,
        &mavio_in->latitude,
        &mavio_in->longitude,
        &mavio_in->alt_gps_amsl,
        mavio_in->vel_ned,
        &mavio_in->aspd_cas,
        &mavio_in->alt_radalt_filt);

    fcs_mi_get_act_cmd(
        mavio_in->motor_cmd,   // Motor commands in RPM (PoC) or normalized [0,1] (SS)
        mavio_in->servo_cmd,   // Servo commands in degrees
        &mavio_in->pusher_cmd, // Pusher command in RPM, PWM (SS)
        MAVIO_NUM_MOTORS,
        MAVIO_NUM_SERVOS);

    da_get_ep_data(&mavio_in->rc_input);

    da_get_radalt_data(&mavio_in->alt_radalt_raw);

    /* Update INSD data */
    da_get_ins_il_euler_angles(&eul_ang[0], &eul_ang[1], &eul_ang[2]);
    da_get_ins_il_angular_velocity(&omg[0], &omg[1], &omg[2]);
    da_get_ins_il_accelerometer_data(&acc[0], &acc[1], &acc[2]);
    da_get_ins_il_inertial_velocity(&v_ned[0], &v_ned[1], &v_ned[2]);
    da_get_ins_il_position(&lat, &lon, &alt);
    da_get_ins_il_eph_epv_data(&eph, &epv);

    da_get_ins_il_kf_cov(mavio_in->ins_data[0].kf_pos_cov_lla, mavio_in->ins_data[0].kf_vel_cov_ned);

    for (int i = 0; i < 3; i++)
    {
        mavio_in->ins_data[0].euler_rpy[i] = eul_ang[i];
        mavio_in->ins_data[0].omg[i] = omg[i];
        mavio_in->ins_data[0].acc[i] = acc[i];
        mavio_in->ins_data[0].vel_ned[i] = v_ned[i];
    }

    mavio_in->ins_data[0].latitude = (int32_t)(lat * POS_LAT_LONG_SCALING);
    mavio_in->ins_data[0].longitude = (int32_t)(lon * POS_LAT_LONG_SCALING);
    mavio_in->ins_data[0].alt_amsl = alt;

    mavio_in->ins_data[0].att_invalid = (da_get_ins_il_att_invalid() ? 1 : 0);
    mavio_in->ins_data[0].pos_invalid = (da_get_ins_il_pos_invalid() ? 1 : 0);
    mavio_in->ins_data[0].data_timeout = (da_get_ins_il_timeout() ? 1 : 0);

    /* Get the GNSS Sat Used only if the timeout is not set on INS */
    if (da_get_ins_il_timeout() != true)
    {
        da_get_ins_il_gnss_sat_used(&gnss_sat_used);
    }

    mavio_in->ins_data[0].gnss_sat_used = gnss_sat_used;

    uint8_t pos_type, sol_status;
    da_get_ins_il_gnss_sol_status(&pos_type, &sol_status);
    if ((sol_status > 0) || (gnss_sat_used < 4))
    {
        mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_NO_FIX;
    }
    else
    {
        switch (pos_type)
        {
        case 0: // Single point solution
            mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_3D_FIX;
            break;
        case 1: // DGPS
        case 2: // SBAS
            mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_DGPS;
            break;
        case 3: // PPP
            mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_DGPS;
            break;
        case 4: // RTK float
            mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_RTK_FLOAT;
            break;
        case 5: // RTK fixed
            mavio_in->ins_data[0].gnss_sol_type = GPS_FIX_TYPE_RTK_FIXED;
            break;
        default: // Unknown
            break;
        }
    }

    da_get_ins_il_gnss_dop(&mavio_in->ins_data[0].gnss_hdop);

    mavio_in->ins_data[0].gnss_h_accuracy = eph;
    mavio_in->ins_data[0].gnss_v_accuracy = epv;
    da_get_ins_il_temperature(&mavio_in->ins_data[0].temp);

    for (int i = 0; i < 3; i++)
    {
        mavio_in->ins_data[1].euler_rpy[i] = eul_ang[i];
        mavio_in->ins_data[1].omg[i] = omg[i];
        mavio_in->ins_data[1].acc[i] = acc[i];
        mavio_in->ins_data[1].vel_ned[i] = v_ned[i];
    }
    mavio_in->ins_data[1].latitude = (int32_t)(lat * POS_LAT_LONG_SCALING);
    mavio_in->ins_data[1].longitude = (int32_t)(lon * POS_LAT_LONG_SCALING);
    mavio_in->ins_data[1].alt_amsl = alt;

    /* Update ADS-9 data*/
    float tmp;
    bool valid = da_get_adc_9_cas(&tmp);
    mavio_in->adc_data[0].aspd_cas = tmp;
    mavio_in->adc_data[0].aspd_cas_invalid = !valid;
    mavio_in->adc_data[0].data_timeout = (da_get_adc_9_timeout() ? 1 : 0);

    da_get_adc_9_oat(&tmp);
    mavio_in->adc_data[0].oat_celsius = tmp;

    da_get_adc_9_alt(&tmp);
    mavio_in->adc_data[0].alt_baro_amsl = tmp;

    /* Update KST servo data*/
    uint16_t lv_alarm = 0;

    for (int i = 0; i < MAVIO_NUM_SERVOS; i++)
    {

        mavio_in->kst_data.pos_deg[i] = ach_get_servo_pos_deg(i + 1); /* Servo starts from servo_id = 1 (AILERON_L) to 13 (STEERING) */
        float current, temperature;
        ach_get_servo_device_status((i + 1), &current, &temperature);
        mavio_in->kst_data.current[i] = current;
        mavio_in->kst_data.temp[i] = temperature;
        if (ach_get_servo_lv_alarm(i + 1))
        {
            lv_alarm |= 1 << i;
        }
    }

    mavio_in->kst_data.lv_status = lv_alarm;

    /* Update Tmotor data*/
    for (int i = 0; i < MAVIO_NUM_MOTORS; i++)
    {
        s_esc_status_frame_t tmotor_status;
        ach_get_epu_status((i + 1), &tmotor_status); /* Motor ID starts from 1 (ESC_1) to 8 (ESC_8)*/

        mavio_in->tmotor_data.rpm[i] = (float)tmotor_status.rpm;
        mavio_in->tmotor_data.current[i] = tmotor_status.current_f16;
        mavio_in->tmotor_data.voltage[i] = tmotor_status.voltage_f16;
        mavio_in->tmotor_data.temp[i] = tmotor_status.temperature_f16;
    }
}

static void send_mavlink_msg_gcs(void)
{
    // This function is called periodically to send a MAVLink message.
    static uint32_t counter = 0;

    // Send heartbeat @1 Hz
    if ((counter + SLOT1) % (RATE_HEARTBEAT) == 0)
    {
        send_gcs_hb(&MavioIn);
    }

    if ((counter + SLOT2) % (RATE_ATTITUDE) == 0)
    {
        send_gcs_attitude(&MavioIn);
    }

    if ((counter + SLOT3) % (RATE_GLOBAL_POS) == 0)
    {
        send_gcs_gps_pos(&MavioIn);
    }

    if ((counter + SLOT4) % (RATE_AIR_DATA) == 0)
    {
        send_gcs_air_data(&MavioIn);
    }

    if ((counter + SLOT5) % (RATE_PILOT_INPUT) == 0)
    {
        send_gcs_pilot_input(&MavioIn);
    }

    if ((counter + SLOT10) % (RATE_ACT_CMD_DATA) == 0)
    {
        send_gcs_act_cmd(&MavioIn);
    }

    if ((counter + SLOT6) % (RATE_INS_DATA) == 0)
    {
        send_gcs_ins_il_data(&MavioIn);
    }

    if ((counter + SLOT8) % (RATE_ACT_DATA) == 0)
    {
        send_gcs_act_kst_data(&MavioIn);
    }

    if ((counter + SLOT9) % (RATE_EPU_DATA) == 0)
    {
        send_gcs_epu_tmotor_data(&MavioIn);
    }

    //    }
    //    if (counter % (RATE_BATT_DATA) == 0)
    //    {
    //        send_gcs_batt_data(&MavioIn);
    //    }
    //
    //    if (counter % (RATE_ECBU_DATA) == 0)
    //    {
    //        send_gcs_ecbu_data(&MavioIn);

    send_gcs_wp_info(&MavioIn, &MavioOut);

    // Increment the counter
    counter++;
}

static void send_gcs_hb(const mavio_in_t *mavio_in)
{
    uint8_t base_mode = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    if (mavio_in->safety_status)
    {
        base_mode |= MAV_MODE_FLAG_SAFETY_ARMED; // Armed state
    }

    union lodd_custom_mode_t custom_mode;
    custom_mode.data = 0;
    custom_mode.main_mode = mavio_in->vom_status;
    custom_mode.flags |= (mavio_in->safety_status == 1) ? LDE_STATUS_ARMED : 0;
    custom_mode.flags |= (mavio_in->pic_status == 1) ? LDE_STATUS_PIC_MODE : 0;
    custom_mode.flags |= (mavio_in->in_air_status == 1) ? LDE_STATUS_IN_AIR : 0;
    custom_mode.flags |= (mavio_in->ep_data_loss == 1) ? LDE_STATUS_RC_LINK_LOSS : 0;
    custom_mode.flags |= (mavio_in->ip_data_loss == 1) ? LDE_STATUS_IP_LINK_LOSS : 0;
    custom_mode.flags |= (mavio_in->gnss_loss == 1) ? LDE_STATUS_GPS_LOSS : 0;

    uint8_t system_status = MAV_STATE_ACTIVE; // System status can be set as per requirement
    mavlink_msg_heartbeat_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                               MavioSystem.sys_type, MavioSystem.autopilot_type,
                               base_mode, custom_mode.data, system_status);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_attitude(const mavio_in_t *mavio_in)
{
    mavlink_msg_attitude_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                              (uint32_t)timer_get_system_time_ms(),
                              mavio_in->euler_rpy[0],
                              mavio_in->euler_rpy[1],
                              mavio_in->euler_rpy[2],
                              mavio_in->omg_xyz[0],
                              mavio_in->omg_xyz[1],
                              mavio_in->omg_xyz[2]);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_gps_pos(const mavio_in_t *mavio_in)
{
    mavlink_gps_raw_int_t gps = {0};
    gps.time_usec = (uint64_t)timer_get_system_time_ms() * 1000;          // Convert ms to us
    gps.fix_type = mavio_in->ins_data[0].gnss_sol_type;                   // GPS fix type
    gps.lat = mavio_in->latitude;                                         // Latitude in degE7
    gps.lon = mavio_in->longitude;                                        // Longitude in degE7
    gps.alt = (int32_t)(mavio_in->alt_gps_amsl * 1000);                   // Altitude in mm
    gps.eph = (uint16_t)mavio_in->ins_data[0].gnss_hdop * 100;            // hdop
    gps.epv = 0;                                                          // vdop
    gps.h_acc = (uint32_t)(mavio_in->ins_data[0].gnss_h_accuracy * 1000); // Horizontal accuracy in mm
    gps.v_acc = (uint32_t)(mavio_in->ins_data[0].gnss_v_accuracy * 1000); // Vertical accuracy in mm
    const float *tmp = mavio_in->vel_ned;
    gps.vel = (int32_t)sqrtf(tmp[0] * tmp[0] + tmp[1] * tmp[1]) * 100; // Velocity in cm/s
    gps.cog = atan2f(tmp[1], tmp[0]) * RAD2DEG_F * 100;                // Course over ground in centi-degrees
    gps.satellites_visible = mavio_in->ins_data[0].gnss_sat_used;      // Number

    mavlink_msg_gps_raw_int_encode(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg, &gps);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_air_data(const mavio_in_t *mavio_in)
{
    uint8_t validity = 0;
    uint8_t tmp_valid = mavio_in->adc_data[0].aspd_cas_invalid ? 0 : LDE_AIRDATA_ADS9_VALID;
    validity |= tmp_valid;

    tmp_valid = mavio_in->adc_data[0].data_timeout ? LDE_AIRDATA_ADS9_TIMEOUT : 0;
    validity |= tmp_valid;

    mavlink_msg_ldm_airdata_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                 (uint32_t)timer_get_system_time_ms(),       // time_boot_ms
                                 validity,                                   // validity flag
                                 mavio_in->adc_data[0].aspd_cas * 100,       // airspeed in cm/s
                                 mavio_in->adc_data[0].alt_baro_amsl * 1000, // altitude in mm
                                 mavio_in->adc_data[0].oat_celsius,
                                 mavio_in->adc_data[1].aspd_cas * 100,
                                 0,
                                 0,
                                 mavio_in->alt_radalt_raw * 100); // radalt altitude in cm

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_pilot_input(const mavio_in_t *mavio_in)
{
    mavlink_ldm_pilot_input_t pilot_input;

    uint8_t validity = 0;

    /* Copy EP data */
    pilot_input.roll_ch[0] = (int8_t)(mavio_in->rc_input.axis_r * 100);
    pilot_input.pitch_ch[0] = (int8_t)(mavio_in->rc_input.axis_p * 100);
    pilot_input.thrust_ch[0] = (int8_t)(mavio_in->rc_input.axis_t * 100);
    pilot_input.yaw_ch[0] = (int8_t)(mavio_in->rc_input.axis_y * 100);
    pilot_input.pusher_ch[0] = (int8_t)(mavio_in->rc_input.slider_r * 100);
    pilot_input.switch_1[0] = (int8_t)(mavio_in->rc_input.sw_b * 100);
    pilot_input.switch_2[0] = (int8_t)(mavio_in->rc_input.sw_c * 100);
    pilot_input.arm_ch = (int8_t)(mavio_in->rc_input.knob_r * 100);
    pilot_input.rssi[0] = (uint8_t)mavio_in->rc_input.rssi;
    validity |= (mavio_in->rc_input.link_lost | mavio_in->rc_input.data_timeout)
                    ? 0
                    : LDE_PILOT_INPUT_EP_VALID;

    /* Copy IP data */
    rc_input_t rc_input;
    mav_io_get_ip_data(&rc_input);
    pilot_input.roll_ch[1] = (int8_t)(rc_input.axis_r * 100);
    pilot_input.pitch_ch[1] = (int8_t)(rc_input.axis_p * 100);
    pilot_input.thrust_ch[1] = (int8_t)(rc_input.axis_t * 100);
    pilot_input.yaw_ch[1] = (int8_t)(rc_input.axis_y * 100);
    pilot_input.pusher_ch[1] = (int8_t)(rc_input.slider_r * 100);
    validity |= (rc_input.link_lost | rc_input.data_timeout)
                    ? 0
                    : LDE_PILOT_INPUT_IP_VALID;

    pilot_input.validity = validity;

    mavlink_msg_ldm_pilot_input_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                       &send_msg, &pilot_input);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_act_cmd(const mavio_in_t *mavio_in)
{
    mavlink_ldm_act_cmd_t act_cmd;

    act_cmd.time_ms = (uint32_t)timer_get_system_time_ms();

    for (int i = 0; i < MAVIO_NUM_MOTORS; i++)
    {
        act_cmd.motor_cmd[i] = (int16_t)(mavio_in->motor_cmd[i] * 1000.0f);
    }

    for (int i = 0; i < MAVIO_NUM_SERVOS; i++)
    {
        act_cmd.servo_cmd[i] = (int16_t)(mavio_in->servo_cmd[i] * 100.0f);
    }

    act_cmd.pusher_cmd = (int16_t)(mavio_in->pusher_cmd);

    mavlink_msg_ldm_act_cmd_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                   &send_msg, &act_cmd);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_ins_il_data(const mavio_in_t *mavio_in)
{
    mavlink_ldm_ins_il_t ins_il;

    ins_il.time_ms = (uint32_t)timer_get_system_time_ms();

    for (int i = 0; i < 3; i++)
    {
        ins_il.eul_rpy[i] = (int16_t)(mavio_in->ins_data[0].euler_rpy[i] * RAD2DEG_F * 100.0f);
        ins_il.omg[i] = mavio_in->ins_data[0].omg[i];
        ins_il.acc[i] = mavio_in->ins_data[0].acc[i];
        ins_il.vel_ned[i] = (int16_t)(mavio_in->ins_data[0].vel_ned[i] * 100.0f);
    }
    float yaw_deg = mavio_in->ins_data[0].euler_rpy[2] * RAD2DEG_F;
    if (yaw_deg > 180.0f)
    {
        yaw_deg = yaw_deg - 360.0f;
    }
    ins_il.eul_rpy[2] = (int16_t)(yaw_deg * 100.0f);
    ins_il.lat = mavio_in->ins_data[0].latitude;
    ins_il.lon = mavio_in->ins_data[0].longitude;
    ins_il.alt_amsl = mavio_in->ins_data[0].alt_amsl;
    for (int i = 0; i < 3; i++)
    {
        ins_il.kf_pos_cov_lla[i] = mavio_in->ins_data[0].kf_pos_cov_lla[i];
        ins_il.kf_vel_cov_ned[i] = mavio_in->ins_data[0].kf_vel_cov_ned[i];
    }

    ins_il.gnss_h_acc = (uint16_t)(mavio_in->ins_data[0].gnss_h_accuracy * 100.0f);
    ins_il.gnss_v_acc = (uint16_t)(mavio_in->ins_data[0].gnss_v_accuracy * 100.0f);
    ins_il.gnss_n_sat = mavio_in->ins_data[0].gnss_sat_used;
    ins_il.gnss_fix_type = mavio_in->ins_data[0].gnss_sol_type;
    ins_il.temp = (int8_t)(mavio_in->ins_data[0].temp);
    da_get_ins_il_alt_baro(&ins_il.alt_baro);
    ins_il.gnss_pdop = (uint8_t)(mavio_in->ins_data[0].gnss_hdop * 10.0f);
    uint8_t ins_sol_status;
    da_get_ins_il_ins_sol_status(&ins_sol_status);
    ins_il.ins_sol_status = ins_sol_status;

    mavlink_msg_ldm_ins_il_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                  &send_msg, &ins_il);

    send_msg_over_gcs_link(&send_msg);
}

// static void send_gcs_adc_9_data(const mavio_in_t *mavio_in)
//{
//     /* TODO: need to define new mavlink message*/
// }

static void send_gcs_act_kst_data(const mavio_in_t *mavio_in)
{
    mavlink_ldm_act_kst_t kst;

    kst.time_ms = (uint32_t)timer_get_system_time_ms();
    int i;
    for (i = 0; i < MAVIO_NUM_SERVOS; i++)
    {
        kst.pos_deg[i] = (int16_t)(mavio_in->kst_data.pos_deg[i]);
        kst.current[i] = (uint8_t)(mavio_in->kst_data.current[i]);
        kst.temp[i] = (int8_t)(mavio_in->kst_data.temp[i]);
    }
    kst.lv_status = mavio_in->kst_data.lv_status;

    mavlink_msg_ldm_act_kst_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                   &send_msg, &kst);

    send_msg_over_gcs_link(&send_msg);
}

static void send_gcs_epu_tmotor_data(const mavio_in_t *mavio_in)
{
    mavlink_ldm_epu_tmotor_t tmotor;

    tmotor.time_ms = (uint32_t)timer_get_system_time_ms();
    int i;
    for (i = 0; i < MAVIO_NUM_MOTORS; i++)
    {
        tmotor.rpm[i] = (int16_t)(mavio_in->tmotor_data.rpm[i]);
        tmotor.current[i] = (uint8_t)(mavio_in->tmotor_data.current[i]);
        tmotor.voltage[i] = (uint8_t)(mavio_in->tmotor_data.voltage[i]);
        tmotor.temp[i] = (int8_t)(mavio_in->tmotor_data.temp[i]);
    }

    mavlink_msg_ldm_epu_tmotor_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                      &send_msg, &tmotor);

    send_msg_over_gcs_link(&send_msg);
}

// static void send_gcs_batt_data(const mavio_in_t *mavio_in)
// {
//     /* TODO: need to define new mavlink message*/
// }

// static void send_gcs_ecbu_data(const mavio_in_t *mavio_in)
// {
//     /* TODO: need to define new mavlink message*/
// }

static void send_gcs_wp_info(mavio_in_t *mavio_in, mavio_out_t *mavio_out)
{
    if (mavio_in->current_waypoint_idx != mavio_in->prev_waypoint_idx)
    {
        mavio_in->prev_waypoint_idx = mavio_in->current_waypoint_idx;

        mavlink_mission_current_t mission_cur = {0};
        mission_cur.seq = mavio_in->current_waypoint_idx;
        mission_cur.total = mavio_out->wp_list.wp_count;

        mavlink_msg_mission_current_encode(MavioSystem.sys_id, MavioSystem.comp_id,
                                           &send_msg, &mission_cur);

        send_msg_over_gcs_link(&send_msg);
    }
}

static void send_msg_over_gcs_link(const mavlink_message_t *msg)
{
    const int len = mavlink_msg_to_send_buffer(msg_tx_buffer, msg);

    udp_send_gcs(&msg_tx_buffer[0], len);
    // uart_write(0, &msg_tx_buffer[0], len);
}

static void handle_gcs_message(const mavlink_message_t *msg, mavio_out_t *mavio_out)
{
    // Handle incoming MAVLink messages here
    // This function can be expanded to process different message types
    switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        /* Reset the timer for GCS heartbeat */
        timer_reload(&GcsHeartbeatMonitor);
        // Process heartbeat message
        printf("Received Heartbeat: System ID %d, Component ID %d\r\n",
               msg->sysid, msg->compid);
        break;
    }
    case MAVLINK_MSG_ID_COMMAND_INT:
    {
        // Process command int
        mavlink_command_int_t cmd_int;
        mavlink_msg_command_int_decode(msg, &cmd_int);

        // printf("Received Command Int: cmd %u, param1 %u\r\n",
        //        cmd_int.command, (uint32_t)cmd_int.param1);
        break;
    }
    case MAVLINK_MSG_ID_LDM_IP_CONTROL:
    {
        /* Reset the timer for internal pilot */
        timer_reload(&InternalPilotMonitor);
        // Process IP control message
        mavlink_ldm_ip_control_t ip_control;
        mavlink_msg_ldm_ip_control_decode(msg, &ip_control);

        // printf("Received IP Control: roll %d, pitch %d, yaw %d, throttle %d, pusher %d\r\n",
        //        ip_control.roll, ip_control.pitch, ip_control.yaw,
        //        ip_control.thrust, ip_control.pusher);

        mavio_out->ip_input.counter += 1;
        mavio_out->ip_input.roll = ip_control.roll / 1000.0f; // Convert to -1.0 to 1.0 range
        mavio_out->ip_input.pitch = ip_control.pitch / 1000.0f;
        mavio_out->ip_input.yaw = ip_control.yaw / 1000.0f;
        mavio_out->ip_input.thrust = (ip_control.thrust / 1000.0f) * 2.0f - 1.0f; // thrust channel from GCS comes in [0,1000] range
        mavio_out->ip_input.pusher = ip_control.pusher / 1000.0f;

        break;
    }
    case MAVLINK_MSG_ID_COMMAND_LONG:
    {
        // Process command long
        mavlink_command_long_t cmd_long;
        mavlink_msg_command_long_decode(msg, &cmd_long);

        handle_cmd_long(&cmd_long, mavio_out);

        // printf("Received Command Long: cmd %u, param1 %u\r\n",
        //        cmd_long.command, (uint32_t)cmd_long.param1);
        break;
    }
    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
    {
        // printf("Received Param Request List\r\n");
        mavlink_msg_param_value_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                     "NULL", 0, MAV_PARAM_TYPE_INT32, 0, 0);
        send_msg_over_gcs_link(&send_msg);
        break;
    }

    // Add more cases for other messages as needed
    default:
        break;
    }
}

static void handle_cmd_long(mavlink_command_long_t *cmd_long, mavio_out_t *mavio_out)
{
    bool send_ack = false; // Default not to sending an ACK
    switch (cmd_long->command)
    {
    case MAV_CMD_REQUEST_MESSAGE:
    {
        // Handle request message command
        if (cmd_long->param1 == MAVLINK_MSG_ID_AUTOPILOT_VERSION)
        {
            printf("Received Request for Autopilot Version\r\n");
            uint64_t capabilities = MAV_PROTOCOL_CAPABILITY_MISSION_INT |
                                    MAV_PROTOCOL_CAPABILITY_COMMAND_INT |
                                    MAV_PROTOCOL_CAPABILITY_MAVLINK2; // Set capabilities as needed
            uint8_t fcs_ver[8] = {0};
            uint8_t mdl_ver[8] = {0};
            uint8_t os_ver[8] = {0};
            uint8_t uid2[18] = {0};
            mavlink_msg_autopilot_version_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                               capabilities,
                                               0, // flight_sw_version
                                               0, // middleware_sw_version
                                               0, // os_sw_version
                                               0, // board_version
                                               fcs_ver,
                                               mdl_ver,
                                               os_ver,
                                               0, 0, 0, uid2);

            send_msg_over_gcs_link(&send_msg);
        }
        else if (cmd_long->param1 == MAVLINK_MSG_ID_PROTOCOL_VERSION)
        {
            printf("Received Request for Protocol Version\r\n");

            /* Pack the Protocol Version Data including Main & Submodule GIT first 8-bytes hash */
            mavlink_msg_protocol_version_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                              200,
                                              100,
                                              200, // flight_sw_version
                                              &OpsVer[0], &MavlinkVer[0]);

            /* Send the Protocol Version Data over GCS Link */
            send_msg_over_gcs_link(&send_msg);
        }
        break;
    }
    case LDE_CMD_VOM:
    {
        printf("VOM cmd received: %d\r\n", (int)cmd_long->param1);
        mavio_out->vom_cmd = (uint8_t)cmd_long->param1;
        mavio_out->vom_cmd_cnt += 1;
        send_ack = true;
        break;
    }
    case LDE_CMD_PIC:
    {
        printf("PIC selection cmd received: %d\r\n", (int)cmd_long->param1);
        mavio_out->pic_cmd = (uint8_t)cmd_long->param1;
        mavio_out->pic_cmd_cnt += 1;
        send_ack = true;
        break;
    }
    case LDE_CMD_GPIO:
    {
        printf("GPIO cmd received: %d\r\n", (int)cmd_long->param1);
        uint8_t gpio_cmd = (uint8_t)cmd_long->param1;

        send_ack = true;

        if (gpio_cmd == 0)
        {
            gpio_precharge_toggle(true);
            gpio_red_beacon_lights_toggle(true);
            //    printf("precharge ON\r\n");
        }
        else if (gpio_cmd == 1)
        {
            gpio_precharge_toggle(false);
            //    printf("precharge OFF\r\n");
        }
        else if (gpio_cmd == 2)
        {
            gpio_pos_lights_toggle(true);
            //    printf("lights ON\r\n");
        }
        else if (gpio_cmd == 3)
        {
            gpio_pos_lights_toggle(false);
            //    printf("lights OFF\r\n");
        }

        break;
    }
    case LDE_CMD_UMM:
    {
        printf("UMM cmd received: %d\r\n", (int)cmd_long->param1);
        uint8_t umm_cmd = (uint8_t)cmd_long->param1;
        if (umm_cmd == 1)
        {
            // Disable Loiter
            mavio_out->loiter_on = 0;
        }
        else if (umm_cmd == 2)
        {
            // Enable Loiter
            mavio_out->loiter_on = 1;
        }
        else if (umm_cmd == 3)
        {
            // Disable TECS
            mavio_out->tecs_on = 0;
        }
        else if (umm_cmd == 4)
        {
            // Enable TECS
            mavio_out->tecs_on = 1;
        }
        break;
    }
    // Handle other commands as needed
    default:
        printf("Unknown command: %u\r\n", cmd_long->command);
        break;
    }

    if (send_ack)
    {
        // Send command ack
        mavlink_msg_command_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                     cmd_long->command, MAV_RESULT_ACCEPTED, 0, 0, 0, 0);
        send_msg_over_gcs_link(&send_msg);
    }
}

/**
 * @brief Checks for expiry of GCS communication timers.
 *
 * This function checks whether the internal pilot communication timer
 * and the GCS heartbeat timer have expired. If the internal pilot timer
 * has expired, it sets the internal pilot link as lost and prints a
 * timeout message. If the GCS heartbeat timer has expired, it prints
 * a GCS communication timeout message.
 *
 * Timers checked:
 *  - InternalPilotMonitor: Monitors internal pilot communication.
 *  - GcsHeartbeatMonitor: Monitors GCS heartbeat.
 */
static void check_gcs_comm_timer_expiry(void)
{
    /* Check for Internal pilot timer expiry */
    if (timer_check_expiry(&InternalPilotMonitor))
    {
        /* Set GCS internal pilot link lost */
        MavioOut.ip_input.link_lost = true;
        // printf("Internal Pilot communication timeout\r\n");
    }
    else
    {
        /* Reset the link lost flag if the timer is still valid */
        MavioOut.ip_input.link_lost = false;
    }

    /* Check for GCS heartbeat timer expiry */
    if (timer_check_expiry(&GcsHeartbeatMonitor))
    {
        /* Set GCS heartbeat link lost */
        MavioOut.gcs_link_lost = true;
        // printf("GCS communication timeout\r\n");
    }
    else
    {
        /* Reset the GCS timeout flag if the timer is still valid */
        MavioOut.gcs_link_lost = false;
    }
}

/************************************************************
*************************************************************
PIL
*************************************************************
*************************************************************/
#ifdef PIL_BUILD_ENABLED
static void handle_pil_message(const mavlink_message_t *msg)
{
    switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        // Process heartbeat message
        printf("Received Heartbeat: System ID %d, Component ID %d\r\n",
               msg->sysid, msg->compid);
        break;
    }
    case MAVLINK_MSG_ID_LDM_PIL_SENSOR:
    {
        // Process command int
        mavlink_ldm_pil_sensor_t pil_sensor;
        mavlink_msg_ldm_pil_sensor_decode(msg, &pil_sensor);

        // printf("PIL Sensor: RPY: [%.2f, %.2f, %.2f] omg: [%.2f, %.2f, %.2f], acc: [%.2f, %.2f, %.2f]\r\n",
        //        pil_sensor.ins1_eul_rpy[0], pil_sensor.ins1_eul_rpy[1], pil_sensor.ins1_eul_rpy[2],
        //        pil_sensor.ins1_omg[0], pil_sensor.ins1_omg[1], pil_sensor.ins1_omg[2],
        //        pil_sensor.ins1_acc[0], pil_sensor.ins1_acc[1], pil_sensor.ins1_acc[2]);

        /*INS data*/
        int i;
        for (i = 0; i < 3; i++)
        {
            PilIn.ins_data[0].euler_rpy[i] = pil_sensor.ins1_eul_rpy[i];
            PilIn.ins_data[0].omg[i] = pil_sensor.ins1_omg[i];
            PilIn.ins_data[0].acc[i] = pil_sensor.ins1_acc[i];
            PilIn.ins_data[0].vel_ned[i] = pil_sensor.ins1_vel_ned[i];

            PilIn.ins_data[1].euler_rpy[i] = pil_sensor.ins2_eul_rpy[i];
            PilIn.ins_data[1].omg[i] = pil_sensor.ins2_omg[i];
            PilIn.ins_data[1].acc[i] = pil_sensor.ins2_acc[i];
            PilIn.ins_data[1].vel_ned[i] = pil_sensor.ins2_vel_ned[i];
        }
        PilIn.ins_data[0].latitude = pil_sensor.ins1_lat;
        PilIn.ins_data[0].longitude = pil_sensor.ins1_lon;
        PilIn.ins_data[0].alt_amsl = pil_sensor.ins1_alt_amsl;

        PilIn.ins_data[1].latitude = pil_sensor.ins2_lat;
        PilIn.ins_data[1].longitude = pil_sensor.ins2_lon;
        PilIn.ins_data[1].alt_amsl = pil_sensor.ins2_alt_amsl;

        /*ADC data*/
        PilIn.adc_data[0].aspd_cas = pil_sensor.adc1_cas;
        PilIn.adc_data[0].aoa = pil_sensor.adc1_aoa;
        PilIn.adc_data[0].aos = pil_sensor.adc1_aos;
        PilIn.adc_data[0].alt_baro_amsl = pil_sensor.adc1_alt_baro;

        PilIn.adc_data[1].aspd_cas = pil_sensor.adc2_cas;
        PilIn.adc_data[1].aoa = pil_sensor.adc2_aoa;
        PilIn.adc_data[1].aos = pil_sensor.adc2_aos;
        PilIn.adc_data[1].alt_baro_amsl = pil_sensor.adc2_alt_baro;

        PilIn.radalt_agl = pil_sensor.radalt;

        PilIn.wow[0] = pil_sensor.wow;
        PilIn.wow[1] = pil_sensor.wow;
        PilIn.wow[2] = pil_sensor.wow;

        PilIn.pusher_rpm = pil_sensor.pusher_rpm;
        for (i = 0; i < 8; i++)
        {
            PilIn.rotor_rpm[i] = pil_sensor.rotor_rpm[i];
        }

        for (i = 0; i < 12; i++)
        {
            PilIn.servo_def[i] = pil_sensor.servo_def[i];
        }

        PilIn.ins_data[0].gyro_healthy = (pil_sensor.ins1_validity & LDE_PIL_INS_OMG_VALID) > 0;
        PilIn.ins_data[0].acc_healthy = (pil_sensor.ins1_validity & LDE_PIL_INS_ACC_VALID) > 0;
        PilIn.ins_data[0].pos_invalid = (pil_sensor.ins1_validity & LDE_PIL_INS_POS_VALID) == 0;

        PilIn.ins_data[1].gyro_healthy = (pil_sensor.ins2_validity & LDE_PIL_INS_OMG_VALID) > 0;
        PilIn.ins_data[1].acc_healthy = (pil_sensor.ins2_validity & LDE_PIL_INS_ACC_VALID) > 0;
        PilIn.ins_data[1].pos_invalid = (pil_sensor.ins2_validity & LDE_PIL_INS_POS_VALID) == 0;

        PilIn.adc_data[0].aspd_cas_invalid = (pil_sensor.adc1_validity & LDE_PIL_ADC_CAS_VALID) == 0;
        PilIn.adc_data[0].aoa_invalid = (pil_sensor.adc1_validity & LDE_PIL_ADC_AOA_VALID) == 0;
        PilIn.adc_data[0].aos_invalid = (pil_sensor.adc1_validity & LDE_PIL_ADC_AOS_VALID) == 0;
        PilIn.adc_data[0].alt_baro_invalid = (pil_sensor.adc1_validity & LDE_PIL_ADC_ALT_VALID) == 0;

        PilIn.adc_data[1].aspd_cas_invalid = (pil_sensor.adc2_validity & LDE_PIL_ADC_CAS_VALID) == 0;
        PilIn.adc_data[1].aoa_invalid = (pil_sensor.adc2_validity & LDE_PIL_ADC_AOA_VALID) == 0;
        PilIn.adc_data[1].aos_invalid = (pil_sensor.adc2_validity & LDE_PIL_ADC_AOS_VALID) == 0;
        PilIn.adc_data[1].alt_baro_invalid = (pil_sensor.adc2_validity & LDE_PIL_ADC_ALT_VALID) == 0;
        break;
    }
    // Add more cases for other messages as needed
    default:
        break;
    }
}

static void send_pil_ctrl_msg(void)
{
    mavlink_ldm_pil_ctrl_t pil_msg;

    int i = 0;
    for (i = 0; i < 8; i++)
    {
        pil_msg.rotor_cmd_cval[i] = PilOut.rotor_cmd_cval[i];
    }

    for (i = 0; i < 12; i++)
    {
        pil_msg.servo_cmd_deg[i] = PilOut.servo_cmd_deg[i];
    }

    pil_msg.pusher_pwm = PilOut.pusher_cmd_pwm;

    mavlink_message_t pil_send_msg;
    mavlink_msg_ldm_pil_ctrl_encode(MavioSystem.sys_id, MavioSystem.comp_id, &pil_send_msg, &pil_msg);
    send_msg_over_pil_link(&pil_send_msg);
}

static void send_mavlink_msg_pil(void)
{
    // This function is called periodically to send a MAVLink message.
    static uint32_t counter = 0;
    // Send PIL CONTROLLER OUTPUT @100 Hz
    if (counter % RATE_PIL_CTRL == 0)
    {
        send_pil_ctrl_msg();
    }
    counter++;
}

static void send_msg_over_pil_link(const mavlink_message_t *msg)
{
    const int len = mavlink_msg_to_send_buffer(msg_tx_buffer, msg);

    udp_send_pil(&msg_tx_buffer[0], len);
}

#endif // PIL_BUILD_ENABLED

/************************************************************
*************************************************************
Mission Management State Machine
*************************************************************
*************************************************************/

typedef enum
{
    MISSION_RX_SM_IDLE = 0,
    MISSION_RX_SM_REQ_ITEM,
    MISSION_RX_SM_RECEIVE_ITEM,
    MISSION_RX_SM_SEND_ACK,
} mission_rx_sm_state_t;

typedef struct
{
    mission_rx_sm_state_t state;
    uint16_t mission_count;
    uint16_t sequence;
    uint8_t target_sysid;
    uint8_t target_compid;
} mission_rx_sm_t;

static mission_rx_sm_t mission_rx_sm;

void mavio_mission_receive(mission_rx_sm_t *sm, mavlink_message_t *msg, mavio_wp_list_t *wp_list);
void mavio_mission_send(mavlink_message_t *msg, mavio_wp_list_t *wp_list);

void handle_mission_items(mavlink_message_t *msg, mavio_wp_list_t *wp_list)
{
    // Handle mission items based on the current state of the state machine
    mavio_mission_receive(&mission_rx_sm, msg, wp_list);

    mavio_mission_send(msg, wp_list);
}

void mavio_mission_init(mavio_wp_list_t *wp_list)
{
    mission_rx_sm.state = MISSION_RX_SM_IDLE;
    mission_rx_sm.mission_count = 0;
    mission_rx_sm.sequence = 0;
    mission_rx_sm.target_sysid = 0;
    mission_rx_sm.target_compid = 0;

    wp_list->wp_count = 0;
    wp_list->last_wp_land = false;
    wp_list->wp_list_valid = false;

    for (int i = 0; i < MAX_MISSION_ITEMS; i++)
    {
        wp_list->lat[i] = 0.0;
        wp_list->lon[i] = 0.0;
        wp_list->alt[i] = 0.0;
    }
}

// TODO: add timeout handling for mission protocol
void mavio_mission_receive(mission_rx_sm_t *sm, mavlink_message_t *msg, mavio_wp_list_t *wp_list)
{
    static uint16_t timeout = 0; // timeout of 250 ms, loop rate is 100 Hz or 10 ms cycle
    static uint8_t retries = 0;  // 5 retries according to MAVLink spec

    switch (sm->state)
    {
    case MISSION_RX_SM_IDLE:
    {
        if (msg->msgid == MAVLINK_MSG_ID_MISSION_COUNT)
        {
            mavlink_mission_count_t mission_count;
            mavlink_msg_mission_count_decode(msg, &mission_count);
            if (mission_count.count > MAX_MISSION_ITEMS)
            {
                // Send mission ack
                mavlink_msg_mission_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                             sm->target_sysid, sm->target_compid,
                                             MAV_MISSION_NO_SPACE, 0, 0);
                send_msg_over_gcs_link(&send_msg);

                printf("Mission count exceeds maximum allowed items (%d)\r\n", MAX_MISSION_ITEMS);
                break;
            }
            else if (mission_count.mission_type != MAV_MISSION_TYPE_MISSION)
            {
                printf("Only MISSION items allowed\r\n");
                break;
            }
            else
            {
                // start accepting mission items
                wp_list->wp_count = 0;          // Reset waypoint count
                wp_list->wp_list_valid = false; // Reset waypoint list validity
                printf("Received mission count: %d\r\n", mission_count.count);
            }
            sm->mission_count = mission_count.count;
            sm->state = MISSION_RX_SM_REQ_ITEM;
            sm->sequence = 0;
            sm->target_sysid = mission_count.target_system;
            sm->target_compid = mission_count.target_component;
            printf("Mission count: %d\r\n", sm->mission_count);
        }

        // Reset timeout and retries
        timeout = 0;
        retries = 0;

        break;
    }
    case MISSION_RX_SM_REQ_ITEM:
    {
        // Send request mission item
        mavlink_msg_mission_request_int_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                             sm->target_sysid, sm->target_compid,
                                             sm->sequence, 0);
        send_msg_over_gcs_link(&send_msg);
        sm->state = MISSION_RX_SM_RECEIVE_ITEM;

        // Reset timeout
        timeout = 0;

        break;
    }
    case MISSION_RX_SM_RECEIVE_ITEM:
    {
        // Receive mission item
        if (msg->msgid == MAVLINK_MSG_ID_MISSION_ITEM_INT)
        {
            mavlink_mission_item_int_t mission_item;
            mavlink_msg_mission_item_int_decode(msg, &mission_item);

            if (mission_item.seq == sm->sequence)
            {
                // accept only waypiint, land, and takeoff mission items
                if (mission_item.command == MAV_CMD_NAV_WAYPOINT || mission_item.command == MAV_CMD_NAV_LAND ||
                    mission_item.command == MAV_CMD_NAV_TAKEOFF || mission_item.command == MAV_CMD_NAV_VTOL_TAKEOFF)
                {
                    // Store waypoint in list
                    wp_list->lat[wp_list->wp_count] = mission_item.x;
                    wp_list->lon[wp_list->wp_count] = mission_item.y;
                    wp_list->alt[wp_list->wp_count] = mission_item.z;
                    wp_list->wp_type[wp_list->wp_count] = mission_item.command;
                    wp_list->wp_count++;
                    if (mission_item.current)
                    {
                        wp_list->active_wp_idx = sm->sequence; // Set active waypoint
                    }
                    printf("Received mission item %d: lat=%.7f, lon=%.7f, alt=%.2f, type: %d\r\n",
                           mission_item.seq, (double)mission_item.x, (double)mission_item.y, mission_item.z, mission_item.command);
                }
                else
                {
                    printf("Ignoring non waypoint item %d\r\n", mission_item.command);
                }

                sm->sequence++;
                if (sm->sequence >= sm->mission_count)
                {
                    sm->state = MISSION_RX_SM_SEND_ACK;
                    wp_list->wp_list_valid = true;
                }
                else
                {
                    sm->state = MISSION_RX_SM_REQ_ITEM;
                }
            }
            else
            {
                printf("Received mission item %d but expected %d, ignoring.\r\n",
                       mission_item.seq, sm->sequence);
                // Send mission ack
                mavlink_msg_mission_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                             sm->target_sysid, sm->target_compid,
                                             MAV_MISSION_INVALID_SEQUENCE, 0, 0);
                send_msg_over_gcs_link(&send_msg);
                sm->state = MISSION_RX_SM_IDLE; // Reset state machine
            }
        }
        else
        {
            timeout++;         // increment timeout until 250 ms
            if (timeout >= 25) // 250 ms timeout
            {
                if (retries < 5) // 5 retries
                {
                    retries++;
                    printf("Timeout waiting for mission item %d, retrying...\r\n", sm->sequence);
                    sm->state = MISSION_RX_SM_REQ_ITEM; // Retry requesting the same item
                }
                else
                {
                    printf("Failed to receive mission item %d after 5 retries, aborting mission.\r\n", sm->sequence);
                    sm->state = MISSION_RX_SM_IDLE; // Reset state machine
                }
            }
        }
        break;
    }
    case MISSION_RX_SM_SEND_ACK:
    {
        // Send mission ack
        mavlink_msg_mission_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                     sm->target_sysid, sm->target_compid,
                                     MAV_MISSION_ACCEPTED, 0, 0);
        send_msg_over_gcs_link(&send_msg);

        printf("Mission received: %d waypoints\r\n", wp_list->wp_count);
        for (int i = 0; i < wp_list->wp_count; i++)
        {
            printf("Waypoint %d: lat=%d, lon=%d, alt=%.2f\r\n",
                   i, wp_list->lat[i], wp_list->lon[i], wp_list->alt[i]);
        }
        // Reset state machine
        sm->state = MISSION_RX_SM_IDLE;
        break;
    }
    default:
        break;
    }
}

void mavio_mission_send(mavlink_message_t *msg, mavio_wp_list_t *wp_list)
{
    switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
    {
        printf("Mission request list received\r\n");
        mavlink_mission_request_list_t mission_request_list;
        mavlink_msg_mission_request_list_decode(msg, &mission_request_list);
        // Send mission count
        mavlink_msg_mission_count_pack(MavioSystem.sys_id, MavioSystem.comp_id, msg,
                                       mission_request_list.target_system, mission_request_list.target_component,
                                       wp_list->wp_count, 0, 0);
        send_msg_over_gcs_link(msg);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_REQUEST_INT:
    {
        mavlink_mission_request_int_t mission_request;
        mavlink_msg_mission_request_int_decode(msg, &mission_request);
        printf("Mission item %d requested\r\n", mission_request.seq);
        if (mission_request.seq < wp_list->wp_count)
        {
            bool current = (mission_request.seq == wp_list->active_wp_idx) ? 1 : 0;
            // Send mission item
            mavlink_msg_mission_item_int_pack(MavioSystem.sys_id, MavioSystem.comp_id, msg,
                                              mission_request.target_system, mission_request.target_component,
                                              mission_request.seq, 0, MAV_CMD_NAV_WAYPOINT, current, 1, 0, 0, 0, 0,
                                              wp_list->lat[mission_request.seq],
                                              wp_list->lon[mission_request.seq],
                                              wp_list->alt[mission_request.seq],
                                              0);
            send_msg_over_gcs_link(msg);
        }
        else
        {
            // Send mission ack for invalid sequence
            mavlink_msg_mission_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, msg,
                                         mission_request.target_system, mission_request.target_component,
                                         MAV_MISSION_ERROR, 0, 0);
            send_msg_over_gcs_link(msg);
        }
        break;
    }
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
    {
        // Clear all missions
        printf("Clearing all missions.\r\n");
        wp_list->wp_count = 0;
        wp_list->wp_list_valid = false;
        // Send mission ack
        mavlink_mission_clear_all_t mission_clear_all;
        mavlink_msg_mission_clear_all_decode(msg, &mission_clear_all);
        mavlink_msg_mission_ack_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                     mission_clear_all.target_system, mission_clear_all.target_component,
                                     MAV_MISSION_ACCEPTED, 0, 0);
        send_msg_over_gcs_link(&send_msg);
        break;
    }
    case MAVLINK_MSG_ID_MISSION_SET_CURRENT:
    {
        // Set current mission item
        mavlink_mission_set_current_t mission_set_current;
        mavlink_msg_mission_set_current_decode(msg, &mission_set_current);

        if ((mission_set_current.seq < wp_list->wp_count) &&
            wp_list->wp_list_valid)
        {
            printf("Set current mission item.\r\n");

            wp_list->active_wp_idx = mission_set_current.seq; // Set active waypoint index
            // Send mission ack
            mavlink_msg_mission_current_pack(MavioSystem.sys_id, MavioSystem.comp_id, &send_msg,
                                             wp_list->active_wp_idx, wp_list->wp_count,
                                             MISSION_STATE_ACTIVE, 1, 0, 0, 0);
            send_msg_over_gcs_link(&send_msg);
        }
        else
        {
            printf("No valid mission items to set current.\r\n");
        }
        break;
    }
    default:
        // Handle other mission messages as needed
        // printf("Unhandled mission message ID: %d\r\n", msg->msgid);
        break;
    }
}
