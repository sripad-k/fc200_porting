/****************************************************
 *  timer_main.h                                         
 *  Created on: 25-Aug-2025 10:22:24 AM                      
 *  Implementation of the Class timer_main       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_TIMER_MAIN
#define H_TIMER_MAIN


#include "timer_interface.h"

uint64_t timer_get_system_time_ms();
void timer_start(s_timer_data_t* ptr_timer_instance, uint64_t period);
bool timer_check_expiry(s_timer_data_t* ptr_timer_instance);
void timer_reset(s_timer_data_t* ptr_timer_instance);
void timer_reload(s_timer_data_t* ptr_timer_instance);
void timer_delay(uint64_t delay_ms);


#endif /*!defined(H_TIMER_MAIN)*/
 