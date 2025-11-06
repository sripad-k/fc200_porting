/****************************************************
 *  mavlink_io_interface.h
 *  Created on: 07-Jul-2025 12:44:23 PM
 *  Implementation of the Interface mavlink_io_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_MAVLINK_IO_INTERFACE
#define H_MAVLINK_IO_INTERFACE

#include "type.h"
#include "types_sbus.h"
#include "mavlink_io_types.h"

void mav_io_init(void);
void mavlink_io_send_periodic(void);
void mavlink_io_recv_periodic(void);

/*------------------------------------Getters---------------------------------------------*/

bool mav_io_get_ip_data(rc_input_t *rc_input);

bool mav_io_get_vom_cmd(uint8_t *vom_cmd, uint16_t *vom_cmd_cnt);

bool mav_io_get_pic_cmd(uint8_t *pic_cmd, uint16_t *pic_cmd_cnt);

bool mav_io_get_umm_cmd(uint8_t *loiter_on, uint8_t *tecs_on);

bool mav_io_get_wp_list(mavio_wp_list_t *wp_list);

bool mav_io_get_pil_input(pil_in_t *pil_input);

void mav_io_set_pil_out(const pil_out_t *pil_out);

bool mav_io_get_gcs_link_lost(void);

#endif /*!defined(H_MAVLINK_IO_INTERFACE)*/
