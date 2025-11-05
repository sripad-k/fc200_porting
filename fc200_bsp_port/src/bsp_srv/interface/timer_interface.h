/****************************************************
 *  timer_interface.h                                         
 *  Created on: 25-Aug-2025 10:22:24 AM                      
 *  Implementation of the Interface timer_interface       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_TIMER_INTERFACE
#define H_TIMER_INTERFACE
#include "type.h"

typedef enum
{
	TIMER_STOPPED,
	TIMER_COUNTING,
	TIMER_EXPIRED

} e_timer_state_t;

typedef struct timer_data_s
{
	/**
	 * Timer state: stopped, counting, expired
	 */
	e_timer_state_t state;
	/**
	 * Start time in milliseconds
	 */
	uint64_t start_time;
	/**
	 * Timer period in milliseconds
	 */
	uint64_t period;

}  s_timer_data_t;

void timer_init(void);
uint64_t timer_get_system_time_ms(void);
void timer_start(s_timer_data_t* ptr_timer_instance, uint64_t period);
bool timer_check_expiry(s_timer_data_t* ptr_timer_instance);
void timer_reset(s_timer_data_t* ptr_timer_instance);
void timer_reload(s_timer_data_t* ptr_timer_instance);
void timer_delay(uint64_t delay_ms);


#endif /*!defined(H_TIMER_INTERFACE)*/
 