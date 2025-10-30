/****************************************************
 *  sys_srv_interface.h                                         
 *  Created on: 25-Aug-2025 10:25:57 AM                      
 *  Implementation of the Interface sys_srv_interface       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_SYS_SRV_INTERFACE
#define H_SYS_SRV_INTERFACE


#include "type.h"

void sys_boot();
void sys_set_tick_period(uint64_t timer_tick_period);
void sys_sleep();

void sys_tickHandler(const Uint32_t parameter);


#endif /*!defined(H_SYS_SRV_INTERFACE)*/
 
