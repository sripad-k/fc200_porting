/****************************************************
 *  pwm_interface.h                                         
 *  Created on: 25-Aug-2025 10:22:23 AM                      
 *  Implementation of the Interface pwm_interface       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_PWM_INTERFACE
#define H_PWM_INTERFACE

#include "type.h"

void pwm_init(void);
bool pwm_configure(uint16_t channel, uint16_t frequency);
bool pwm_set(uint16_t channel, double duty_cycle_us);

#endif /*!defined(H_PWM_INTERFACE)*/
