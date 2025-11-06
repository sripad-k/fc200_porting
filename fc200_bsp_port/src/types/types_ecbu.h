/****************************************************
 *  types_ecbu.h
 *  Created on: 07-Oct-2025 11:42:15
 *  Implementation of the Interface types_ecbu
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_TYPES_ECBU
#define H_TYPES_ECBU

typedef enum
{
	ECB_SW_NO_ACTION,
	ECB_SW_CLOSE_CMD,
	ECB_SW_OPEN_CMD,
	ECB_SW_RESERVED
} ecb_switch_command_t;

typedef enum
{
	ECB_CMD_DONT_CHANGE,
	ECB_CMD_OFF,
	ECB_CMD_ON,
	ECB_CMD_RESET,
	ECB_CMD_PULL,
	ECB_CMD_COLLAR
} ecb_ind_breaker_cmd_t;

typedef enum
{
	ECBU_1_LRM_1 = 0x20,
	ECBU_1_LRM_2 = 0x21,
	ECBU_2_LRM_1 = 0x23,
	ECBU_2_LRM_2 = 0x27
} ecb_address_t;

typedef enum
{
	ECB_INVALID = 0,
	ECB_1 = 1,
	ECB_2 = 2,
	ECB_3 = 3,
	ECB_4 = 4,
	ECB_5 = 5,
	ECB_6 = 6,
	ECB_7 = 7,
	ECB_8 = 8,
	ECB_9 = 9,
	ECB_10 = 10,
	ECB_11 = 11,
	ECB_12 = 12,
	ECB_13 = 13,
	ECB_14 = 14,
	ECB_15 = 15,
	ECB_16 = 16,
	ECB_17 = 17,
	ECB_18 = 18,
	ECB_19 = 19,
	ECB_20 = 20,
	ECB_21 = 21,
	ECB_22 = 22,
	ECB_23 = 23,
	ECB_24 = 24,
	MAX_ECB
} ecb_id_t;

typedef enum
{
	LRM_1,
	LRM_2,
	LRM_4,
	LRM_8,
	MAX_LRM
} lrm_id_t;

#endif /*!defined(H_TYPES_ECBU)*/
