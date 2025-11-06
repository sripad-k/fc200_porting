/****************************************************
 *  pwm_main.h                                         
 *  Created on: 25-Aug-2025 10:22:23 AM                      
 *  Implementation of the Class pwm_main       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_PWM_MAIN
#define H_PWM_MAIN


#include "pwm_interface.h"

void pwm_init();
bool pwm_configure(uint16_t channel, uint16_t frequency);
bool pwm_set(uint16_t channel, double duty_cycle);


#endif /*!defined(H_PWM_MAIN)*/
 