/****************************************************
 *  types_gpio.h
 *  Created on: 10-Jul-2025 10:45:53 AM
 *  Implementation of the Interface types_gpio
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_TYPES_GPIO
#define H_TYPES_GPIO

typedef enum
{
    OUTPUT,
    INPUT
} gpio_direction_t;

#define GPIO_PUSHER_PWM (0)
#define GPIO_POSITION_LIGHT (1)
#define GPIO_STROBE_LIGHT (2)
#define GPIO_BEACON_LIGHT (3)
#define GPIO_FTS_IN_SIGNAL (4)
#define GPIO_PRECHARGE (5)

#define PWM_AF_FREQ 200

#endif /*!defined(H_TYPES_GPIO)*/
