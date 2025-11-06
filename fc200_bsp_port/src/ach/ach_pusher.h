/****************************************************
 *  ach_pusher.h
 *  Created on: 10-Jul-2025 10:22:28 AM
 *  Implementation of the Class ach_pusher
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_ACH_PUSHER
#define H_ACH_PUSHER

#include "ach_interface.h"

bool ach_pusher_init(void);
bool ach_cmd_pusher_periodic(void);

#endif /*!defined(H_ACH_PUSHER)*/
