/****************************************************
 *  gpio_interface.h
 *  Created on: 10-Jul-2025 10:22:29 AM
 *  Implementation of the Interface gpio_interface
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_GPIO_INTERFACE
#define H_GPIO_INTERFACE

#include "type.h"
#include "types_gpio.h"

bool gpio_init(void);
bool gpio_configure(uint16_t port, gpio_direction_t direction);
bool gpio_write(uint16_t port, bool state);
bool gpio_read(uint16_t port, bool *ptr_state);

void gpio_configure_all(void);

bool gpio_pos_lights_toggle(bool on);
bool gpio_red_beacon_lights_toggle(bool on);
bool gpio_white_strobe_lights_toggle(bool on);
bool gpio_precharge_toggle(bool on);

#endif /*!defined(H_GPIO_INTERFACE)*/
