/****************************************************
 *  gpio_main.c                                         
 *  Created on: 25-Aug-2025 10:22:22 AM                      
 *  Implementation of the Class gpio_main       
 *  Copyright: LODD (c) 2025                     
 ****************************************************/

#include "gpio_main.h"

bool gpio_init(void)
{
	return true;
}

bool gpio_configure(uint16_t port, gpio_direction_t direction)
{
	return true;
}

bool gpio_write(uint16_t port, bool state)
{
	return true;
}

bool gpio_read(uint16_t port, bool *ptr_state)
{
	return true;
}

void gpio_configure_all(void)
{
}

bool gpio_pos_lights_toggle(bool on)
{
	return true;
}

bool gpio_red_beacon_lights_toggle(bool on)
{
	return true;
}

bool gpio_white_strobe_lights_toggle(bool on)
{
	return true;
}
bool gpio_precharge_toggle(bool on)
{
	return true;
}
