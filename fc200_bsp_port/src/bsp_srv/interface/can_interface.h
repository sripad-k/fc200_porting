/****************************************************
 *  can_interface.h                                         
 *  Created on: 25-Aug-2025 10:22:22 AM                      
 *  Implementation of the Interface can_interface       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_CAN_INTERFACE
#define H_CAN_INTERFACE

#include "type.h"

#define CAN_MAX_DLC (8)
#define CAN_MAX_FILTERS (4)

typedef struct 
{
	uint32_t can_msg_id;
	uint8_t  data[CAN_MAX_DLC];
	uint8_t  dlc;
	bool     extended_id_flag;
	bool     is_remote_req;
} can_msg_t;

typedef enum 
{
	CAN_OK = 0,
	CAN_ERROR = -1,
	CAN_BUSY = -2,
	CAN_NOT_INITIALIZED = -3,
	CAN_INVALID_MSG_LENGTH = -4,
	CAN_NO_NEW_DATA = -5,
	CAN_FILTER_EXHAUSTED = -6
}can_status_t;

typedef enum 
{
	CAN_CHANNEL_1 = 0,
	CAN_CHANNEL_2 = 1,
	CAN_CHANNEL_3 = 2,
	CAN_CHANNEL_4 = 3,
	CAN_CHANNEL_5 = 4,
	CAN_CHANNEL_MAX
}can_channel_t;


typedef struct 
{
	uint32_t can_start_id;
	uint32_t can_end_id;
	bool   is_single_id_filter;
} can_filter_cfg_t;


can_status_t can_init(can_channel_t can_ch);
can_status_t can_write(can_channel_t can_ch, const can_msg_t *ptr_can_msg);
can_status_t can_read(can_channel_t can_ch, can_msg_t *ptr_can_msg);

// can_status_t can_set_filter(can_channel_t can_ch, bool is_single_id_filter, 
// 	                        const uint32_t *const start_id, const uint32_t *const end_id);

#endif /*!defined(H_CAN_INTERFACE)*/
 