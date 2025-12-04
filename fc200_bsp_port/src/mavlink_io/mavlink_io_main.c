/****************************************************
 *  mavlink_io_main.c
 *  Created on: 07-Jul-2025 12:44:23 PM
 *  Implementation of the Class mavlink_io_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "mavlink_io_main.h"
#include "generic_util.h"

bool mav_io_get_ip_data(rc_input_t *rc_input)
{
    bool valid = false;
    if (rc_input != NULL)
    {
        rc_input->axis_r = MavioOut.ip_input.roll;
        rc_input->axis_p = MavioOut.ip_input.pitch;
        rc_input->axis_y = MavioOut.ip_input.yaw;
        rc_input->axis_t = MavioOut.ip_input.thrust;
        rc_input->slider_r = MavioOut.ip_input.pusher;

        rc_input->link_lost = MavioOut.ip_input.link_lost;
        rc_input->data_timeout = MavioOut.ip_input.link_lost;

        valid = true;
    }

    return valid;
}

bool mav_io_get_vom_cmd(uint8_t *vom_cmd, uint16_t *vom_cmd_cnt)
{
    if (vom_cmd != NULL && vom_cmd_cnt != NULL)
    {
        *vom_cmd = MavioOut.vom_cmd;
        *vom_cmd_cnt = MavioOut.vom_cmd_cnt;
        return true;
    }
    return false;
}

bool mav_io_get_pic_cmd(uint8_t *pic_cmd, uint16_t *pic_cmd_cnt)
{
    if (pic_cmd != NULL && pic_cmd_cnt != NULL)
    {
        *pic_cmd = MavioOut.pic_cmd;
        *pic_cmd_cnt = MavioOut.pic_cmd_cnt;
        return true;
    }
    return false;
}

bool mav_io_get_umm_cmd(uint8_t *loiter_on, uint8_t *loiter_on_cnt, uint8_t *tecs_on, uint8_t *tecs_on_cnt)
{
    if (loiter_on != NULL && loiter_on_cnt != NULL && tecs_on != NULL && tecs_on_cnt != NULL)
    {
        *loiter_on = MavioOut.loiter_on;
        *loiter_on_cnt = MavioOut.loiter_on_cnt;
        *tecs_on = MavioOut.tecs_on;
        *tecs_on_cnt = MavioOut.tecs_on_cnt;
        return true;
    }
    return false;
}

bool mav_io_get_wp_list(mavio_wp_list_t *wp_list)
{
    if (wp_list != NULL)
    {
        *wp_list = MavioOut.wp_list;
        return true;
    }
    return false;
}

bool mav_io_get_pil_input(pil_in_t *pil_input)
{
    bool success = false;

    if (pil_input != NULL)
    {
        util_memcpy(pil_input, &PilIn, sizeof(pil_in_t));
        success = true;
    }

    return success;
}

void mav_io_set_pil_out(const pil_out_t *pil_out)
{
    util_memcpy(&PilOut, pil_out, sizeof(pil_out_t));
}

bool mav_io_get_gcs_link_lost(void)
{
    return MavioOut.gcs_link_lost;
}