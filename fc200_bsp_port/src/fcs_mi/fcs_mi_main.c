/****************************************************
 *  mi_main.c
 *  Created on: 03-Jul-2025 10:30:22 AM
 *  Implementation of the Class mi_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "fcs_mi_main.h"
#include "controllerMain.h"
#include "da_interface.h"
#include "ach_interface.h"
#include "mavlink_io_types.h"
#include "mavlink_io_interface.h"
#include "math.h"

#define DEG2RAD 0.0174532925
#define RAD2DEG 57.29577951

/**
 * @brief Initializes the Flight Control module.
 *
 * This function performs the necessary initialization for the FCS module
 * by calling the controllerMain_initialize() function. It should be called
 * before FCS Step functionality.
 *
 * @note This function does not take any parameters and does not return a value.
 */
void fcs_mi_init(void)
{
    controllerMain_initialize();
}

void fcs_mi_get_fcs_dscr(
    uint8_t *vom_status,
    uint8_t *safety_status,
    uint8_t *pic_status,
    uint8_t *in_air_status,
    uint8_t *ep_loss,
    uint8_t *ip_loss,
    uint8_t *gnss_loss,
    uint8_t *ins_selection,
    uint8_t *adc_selection,
    uint16_t *current_waypoint_idx,
    uint8_t *tecs_on,
    uint8_t *loiter_on,
    uint8_t *cog_track_on)
{
    *vom_status = controllerMain_Y.fcs_state.vom_status;
    *safety_status = controllerMain_Y.fcs_state.safety_state;
    *pic_status = controllerMain_Y.fcs_state.pic_status;
    *in_air_status = controllerMain_Y.fcs_state.inAir_flag;
    *ep_loss = controllerMain_U.failure_flags.ep_data_loss;
    *ip_loss = controllerMain_U.failure_flags.ip_data_loss;
    *gnss_loss = controllerMain_U.failure_flags.gps_loss; // TODO: get it from the voter of FCS module
    *ins_selection = 0;                                   // TODO
    *adc_selection = 0;                                   // TODO
    *current_waypoint_idx = controllerMain_Y.wp_req_idx;
    *tecs_on = controllerMain_Y.fcs_state.tecs_mode;
    *loiter_on = controllerMain_Y.fcs_state.loiter_mode;
    *cog_track_on = controllerMain_Y.fcs_state.CoG_tracking;
}

void fcs_mi_get_fcs_cont(
    float *euler_rpy,    // in rad
    float *omg_xyz,      // in rad/s
    float *acc_xyz,      // in m/s/s
    int32_t *latitude,   // in degrees * 1E7
    int32_t *longitude,  // in degrees * 1E7
    float *alt_gps_amsl, // in meters
    float *vel_ned,      // NED velocity in m/s
    float *aspd_cas,
    float *alt_radalt_agl)
{
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        euler_rpy[i] = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.eul_ang[i];
        omg_xyz[i] = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.omg[i];
        acc_xyz[i] = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.accel_b[i];
        vel_ned[i] = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.vel_ned[i];
    }
    *latitude = (int32_t)(controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[0] * RAD2DEG * 1E7);
    *longitude = (int32_t)(controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[1] * RAD2DEG * 1E7);
    *alt_gps_amsl = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.pos_lla[2];
    *aspd_cas = controllerMain_Y.ctrl_log.SensorMgmt.static_sensor_voting_out.aspd_cas;
    *alt_radalt_agl = controllerMain_Y.ctrl_log.SensorMgmt.rad_alt_out;
}

void fcs_mi_get_fbctrl(
    fcs_mi_fbctrl_data_t *fbctrl_data // Feedback control data
)
{
    busController *pt_log = &controllerMain_Y.ctrl_log;

    fbctrl_data->fcs_mr_eul_rpy_d[0] = pt_log->controllerAttCtrl.rollCmd;
    fbctrl_data->fcs_mr_eul_rpy_d[1] = pt_log->controllerAttCtrl.pitchCmd;
    fbctrl_data->fcs_mr_eul_rpy_d[2] = pt_log->controllerIF_att.yawCmd;

    fbctrl_data->fcs_mr_yawrate_d = pt_log->controllerAttCtrl.yawRateCmd; // in rad/s
    fbctrl_data->fcs_mr_yaw_hold = pt_log->controllerIF_att.yawHold;
    for (int i = 0; i < 4; i++)
    {
        fbctrl_data->fcs_ca_nu_des[i] = pt_log->controllerCA.nu_des[i];         // thrust, Mx, My, Mz
        fbctrl_data->fcs_ca_nu_alloc[i] = pt_log->controllerCA.nu_allocated[i]; // thrust, Mx, My, Mz
    }

    fbctrl_data->fcs_ca_cerp[0] = pt_log->controllerCA.c_erp1;
    fbctrl_data->fcs_ca_cerp[1] = pt_log->controllerCA.c_erp2;
    fbctrl_data->fcs_ca_cerp[2] = pt_log->controllerCA.c_erp3;
    fbctrl_data->fcs_mr_h_d = pt_log->controllerAltCtrl.hCmd;
    fbctrl_data->fcs_mr_hdot_d = pt_log->controllerAltCtrl.hDotCmd;
    fbctrl_data->fcs_mr_h_hold = pt_log->controllerAltCtrl.hHold;
    fbctrl_data->fcs_mr_vel_ne_d[0] = pt_log->controllerVelCtrl.velCmd[0];
    fbctrl_data->fcs_mr_vel_ne_d[1] = pt_log->controllerVelCtrl.velCmd[1];

    fbctrl_data->fcs_fw_roll_d = pt_log->fwAttCmd.rollCmd;
    fbctrl_data->fcs_fw_pitch_d = pt_log->fwAttCmd.pitchCmd;
    fbctrl_data->fcs_fw_h_d = pt_log->controllerAltCtrl.hCmd;
    fbctrl_data->fcs_fw_cas_d = pt_log->controllerTECS.vel_cmd;
    fbctrl_data->fcs_mr_pit_intg = pt_log->IntegratorCF.MR_IntData.pitch_int;     // crossfeed
    fbctrl_data->fcs_mr_pit_intg_sat = pt_log->IntegratorCF.MR_IntData.pitch_sat; // crossfeed
    fbctrl_data->fcs_fw_pit_intg = pt_log->IntegratorCF.FW_IntData.pitch_int;     // crossfeed
    fbctrl_data->fcs_fw_pit_intg_sat = pt_log->IntegratorCF.FW_IntData.pitch_sat; // crossfeed
}

int fcs_mi_get_act_cmd(
    float *motor_cmd,  // Motor commands in RPM (PoC) or normalized [0,1] (SS)
    float *servo_cmd,  // Servo commands in degrees
    float *pusher_cmd, // Pusher command in RPM, PWM (SS)
    uint8_t num_motors,
    uint8_t num_servos)
{
    if ((num_motors < 8) || (num_servos < 12))
    {
        return -1; // Invalid number of motors or servos
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            motor_cmd[i] = controllerMain_Y.std_ctrl.lifter_cval_cmd[i];
        }

        for (int i = 0; i < 12; i++)
        {
            servo_cmd[i] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[i];
        }

        *pusher_cmd = controllerMain_Y.std_ctrl.pusher_pwm_cmd;
    }

    return 0; // Success
}

static void update_fcs_input_ins_1(void)
{
    /* get Euler Angles */
    float eul_ang[3];
    float omg[3];
    float acc[3];
    double lat, lon;
    float alt;
    float v_ned[3];
    float eph, epv;

    /* get Euler Angle */
    da_get_ins_il_euler_angles(&eul_ang[0], &eul_ang[1], &eul_ang[2]);
    for (int i = 0; i < 3; ++i)
    {
        if (!isnan(eul_ang[i]) && !isinf(eul_ang[i]))
        {
            controllerMain_U.sensor_in.ins_1.eul_ang[i] = eul_ang[i];
        }
    }

    /* get Angular Velocity */
    da_get_ins_il_angular_velocity(&omg[0], &omg[1], &omg[2]);
    for (int i = 0; i < 3; ++i)
    {
        if (!isnan(omg[i]) && !isinf(omg[i]))
        {
            controllerMain_U.sensor_in.ins_1.omg[i] = omg[i];
        }
    }

    /* get Accelerometer Data */
    da_get_ins_il_accelerometer_data(&acc[0], &acc[1], &acc[2]);
    for (int i = 0; i < 3; ++i)
    {
        if (!isnan(acc[i]) && !isinf(acc[i]))
        {
            controllerMain_U.sensor_in.ins_1.acc[i] = acc[i];
        }
    }

    /* get position data */
    da_get_ins_il_position(&lat, &lon, &alt);

    if (!isnan(lat) && !isinf(lat))
    {
        controllerMain_U.sensor_in.ins_1.lat = lat * DEG2RAD;
    }

    if (!isnan(lon) && !isinf(lon))
    {
        controllerMain_U.sensor_in.ins_1.lon = lon * DEG2RAD;
    }

    if (!isnan(alt) && !isinf(alt))
    {
        controllerMain_U.sensor_in.ins_1.alt_gps_amsl = alt;
    }

    /* get Inertial Velocity */
    da_get_ins_il_inertial_velocity(&v_ned[0], &v_ned[1], &v_ned[2]);
    for (int i = 0; i < 3; ++i)
    {
        if (!isnan(v_ned[i]) && !isinf(v_ned[i]))
        {
            controllerMain_U.sensor_in.ins_1.v_ned[i] = v_ned[i];
        }
    }

    /* get INS-D timeout data */
    controllerMain_U.sensor_in.ins_1.data_timeout = da_get_ins_il_timeout();

    /* get INS-D attitude validity */
    controllerMain_U.sensor_in.ins_1.att_invalid = da_get_ins_il_att_invalid();

    /* get INS-D gyro sensor validity */
    controllerMain_U.sensor_in.ins_1.omg_invalid = da_get_ins_il_omg_invalid();

    /* get INS-D accelerometer sensor validity */
    controllerMain_U.sensor_in.ins_1.acc_invalid = da_get_ins_il_accel_invalid();

    /* get estimated horizontal and veritical position error */
    da_get_ins_il_eph_epv_data(&eph, &epv);

    if (!isnan(eph) && !isinf(eph))
    {
        controllerMain_U.sensor_in.ins_1.eph = eph;
    }
    if (!isnan(epv) && !isinf(epv))
    {
        controllerMain_U.sensor_in.ins_1.epv = epv;
    }

    /* get INS-D GNSS validity */
    controllerMain_U.sensor_in.ins_1.pos_invalid = da_get_ins_il_pos_invalid() || da_get_ins_il_timeout();

    /* TODO: temporary solution until enabling the voter */
    controllerMain_U.failure_flags.gps_loss = controllerMain_U.sensor_in.ins_1.pos_invalid;
}

static void update_fcs_input_ins_2(void)
{
    // TODO: need to fill in
}

// static void update_fcs_input_ads_2(void);
static void update_fcs_input_ads_1(void)
{
    float cas = 0.0;
    bool valid = da_get_adc_9_cas(&cas);

    if (!isnan(cas) && !isinf(cas))
    {
        controllerMain_U.sensor_in.ads_1.aspd_cas = cas;
    }

    controllerMain_U.sensor_in.ads_1.aspd_cas_invalid = valid;
}

static void update_fcs_input_ads_2(void)
{
    // TODO: need to fill in
}

static void update_fcs_input_radalt(void)
{
    float alt_radalt;
    float radalt_snr;
    if (da_get_radalt_data(&alt_radalt, &radalt_snr))
    {
        controllerMain_U.sensor_in.h_radar_agl = alt_radalt;
    }
}

static void update_fcs_input_ep(void)
{
    rc_input_t rc_input;
    if (da_get_ep_data(&rc_input))
    {
        controllerMain_U.pilot_ext.roll_ch = rc_input.axis_r;
        controllerMain_U.pilot_ext.pitch_ch = rc_input.axis_p;
        controllerMain_U.pilot_ext.throttle_ch = rc_input.axis_t;
        controllerMain_U.pilot_ext.yaw_ch = rc_input.axis_y;
        controllerMain_U.pilot_ext.engine_ch = (rc_input.slider_r + 1.0) * 0.5; // input range is [0,1]
        controllerMain_U.pilot_ext.arm_ch = rc_input.knob_r;
        controllerMain_U.pilot_ext.switch_1 = rc_input.sw_b;
    }

    if (rc_input.data_timeout || rc_input.link_lost)
    {
        controllerMain_U.failure_flags.ep_data_loss = true;
    }
    else
    {
        controllerMain_U.failure_flags.ep_data_loss = false;
    }
}

static void update_fcs_input_ip(void)
{
    rc_input_t rc_input;
    if (mav_io_get_ip_data(&rc_input))
    {
        controllerMain_U.pilot_int.roll_ch = rc_input.axis_r;
        controllerMain_U.pilot_int.pitch_ch = rc_input.axis_p;
        controllerMain_U.pilot_int.throttle_ch = rc_input.axis_t;
        controllerMain_U.pilot_int.yaw_ch = rc_input.axis_y;
        controllerMain_U.pilot_int.engine_ch = (rc_input.slider_r + 1.0) * 0.5; // input range is [0,1]
    }

    if (rc_input.data_timeout || rc_input.link_lost)
    {
        controllerMain_U.failure_flags.ip_data_loss = true;
    }
    else
    {
        controllerMain_U.failure_flags.ip_data_loss = false;
    }
}

static void update_fcs_input_gcs_cmd(void)
{
    uint8_t vom_cmd = 0;
    uint16_t vom_cmd_cnt = 0;
    uint8_t pic_cmd = 0;
    uint16_t pic_cmd_cnt = 0;

    mav_io_get_vom_cmd(&vom_cmd, &vom_cmd_cnt);
    controllerMain_U.std_command.vom_cmd = vom_cmd;
    controllerMain_U.std_command.vom_cmd_cnt = vom_cmd_cnt;

    mav_io_get_pic_cmd(&pic_cmd, &pic_cmd_cnt);
    controllerMain_U.std_command.pic_cmd = pic_cmd;
    controllerMain_U.std_command.pic_cmd_cnt = pic_cmd_cnt;

    // commnads used in UMM mode
    uint8_t loiter_on, loiter_on_cnt, tecs_on, tecs_on_cnt;
    mav_io_get_umm_cmd(&loiter_on, &loiter_on_cnt, &tecs_on, &tecs_on_cnt);
    controllerMain_U.extd_cmd.loiter_cmd = loiter_on;
    controllerMain_U.extd_cmd.loiter_cmd_cnt = loiter_on_cnt;
    controllerMain_U.extd_cmd.tecs_cmd = tecs_on;
    controllerMain_U.extd_cmd.tecs_cmd_cnt = tecs_on_cnt;

    controllerMain_U.std_command.airspeed_cas_cmd = 25.0;
    controllerMain_U.std_command.fwrth_apr_deg = 135.0;

    controllerMain_U.failure_flags.gcs_link_loss = mav_io_get_gcs_link_lost();
}

static void update_fcs_input_wp_list(void)
{
    mavio_wp_list_t wp_list;
    mav_io_get_wp_list(&wp_list);

    uint16_t wp_req_idx = controllerMain_Y.wp_req_idx;

    if (wp_req_idx < wp_list.wp_count)
    {
        controllerMain_U.wp_data.wp_list_valid = wp_list.wp_list_valid;
        controllerMain_U.wp_data.wp_list_count = wp_list.wp_count;
        controllerMain_U.wp_data.cur_wp_lat = ((double)wp_list.lat[wp_req_idx]) * 1e-7 * DEG2RAD;
        controllerMain_U.wp_data.cur_wp_lon = ((double)wp_list.lon[wp_req_idx]) * 1e-7 * DEG2RAD;
        controllerMain_U.wp_data.cur_wp_alt = wp_list.alt[wp_req_idx];
        controllerMain_U.wp_data.cur_wp_idx = wp_req_idx;
        controllerMain_U.wp_data.cmd_wp_idx = wp_list.active_wp_idx;
        controllerMain_U.wp_data.last_wp_land = wp_list.last_wp_land;
    }
}

static void update_fcs_output_servos(void)
{
    // TODO: servo direction sign is specific to the aircraft, need to move to aircraft-specific code
    std_servo_cmd_t servo_cmd = {0};
    servo_cmd.servo_cmd_deg[AILERON_L] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[0];
    servo_cmd.servo_cmd_deg[FLAPERON_OUTER_LH] = -1 * controllerMain_Y.std_ctrl.acs_servo_deg_cmd[1];
    servo_cmd.servo_cmd_deg[FLAPERON_INNER_LH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[2];
    servo_cmd.servo_cmd_deg[FLAP_LH] = -1 * controllerMain_Y.std_ctrl.acs_servo_deg_cmd[3];
    servo_cmd.servo_cmd_deg[FLAP_RH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[4];
    servo_cmd.servo_cmd_deg[FLAPERON_INNER_RH] = -1 * controllerMain_Y.std_ctrl.acs_servo_deg_cmd[5];
    servo_cmd.servo_cmd_deg[FLAPERON_OUTER_RH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[6];
    servo_cmd.servo_cmd_deg[AILERON_R] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[7];
    servo_cmd.servo_cmd_deg[RUDDER_LH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[8];
    servo_cmd.servo_cmd_deg[RUDDER_RH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[9];
    servo_cmd.servo_cmd_deg[ELEVATOR_LH] = -1 * controllerMain_Y.std_ctrl.acs_servo_deg_cmd[10];
    servo_cmd.servo_cmd_deg[ELEVATOR_RH] = controllerMain_Y.std_ctrl.acs_servo_deg_cmd[11];

    ach_set_servo_cmd_deg(&servo_cmd);
}

static void update_fcs_output_epu(void)
{
    std_epu_cmd_t epu_cmd = {0};
    for (int i = 0; i < 8; i++)
    {
        epu_cmd.motor_cmd_cval[i] = controllerMain_Y.std_ctrl.lifter_cval_cmd[i];
    }
    ach_set_epu_cmd(&epu_cmd);
}

static void update_fcs_output_pusher(void)
{
    double duty_cycle = controllerMain_Y.std_ctrl.pusher_pwm_cmd;

    ach_set_pusher_pwm(duty_cycle);
}

/*
 set gcs terminal data
*/
void fcs_mi_set_gcs_data(mavio_in_t *mavio_in)
{
    mavio_in->vom_status = controllerMain_Y.fcs_state.vom_status;
    mavio_in->safety_status = controllerMain_Y.fcs_state.safety_state;
    mavio_in->pic_status = controllerMain_Y.fcs_state.pic_status;
}

/**
 * @brief Periodic task function to invoke the Flight Control Law.
 *
 * This function is intended to be called at regular intervals (periodically)
 * to execute the main control step for the MI FCS (Flight Control System).
 * It delegates the periodic processing to the controllerMain_step() function.
 *
 * @note Ensure that this function is invoked with the required timing constraints
 *       for correct controller operation.
 */
void fcs_mi_periodic(void)
{
    update_fcs_input_ins_1();

    update_fcs_input_ins_2();

    update_fcs_input_ads_1();

    update_fcs_input_ads_2();

    update_fcs_input_radalt();

    update_fcs_input_ep();

    update_fcs_input_ip();

    update_fcs_input_gcs_cmd();

    update_fcs_input_wp_list();

    controllerMain_step();

    update_fcs_output_servos();

    update_fcs_output_epu();

    update_fcs_output_pusher();
}