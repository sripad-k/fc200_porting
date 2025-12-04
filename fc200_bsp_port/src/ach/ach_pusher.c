/****************************************************
 *  ach_pusher.c
 *  Created on: 10-Jul-2025 10:22:28 AM
 *  Implementation of the Class ach_pusher
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "ach_pusher.h"
#include "pwm_interface.h"
#include "gpio_interface.h"

#define PWM_MIN (1000.0f) /*Minimum PWM in usec value for the pusher*/
#define PWM_MAX (2000.0f) /*Maximum PWM in usec value for the pusher*/

static double DutyCycle_uSec = 0.0f; /*Variable to hold the PWM duty cycle for the pusher*/

/**
 * @brief Initializes the pusher mechanism by configuring and initializing its PWM.
 *
 * This function performs the following steps:
 * 1. Configures the PWM for the pusher using the specified GPIO and frequency.
 * 2. Initializes the pusher with a PWM signal that ensures it remains inactive (prevents beeping).
 * 3. Checks if both configuration and initialization are successful.
 *    - On success, prints a confirmation message.
 *    - On failure, prints an error message.
 *
 * @note This function assumes that the GPIO and PWM constants (GPIO_PUSHER_PWM, PWM_AF_FREQ, PWM_MIN)
 *       are defined elsewhere in the codebase.
 */
bool ach_pusher_init(void)
{
	bool config_status = false;
	bool init_pwm_status = false;
	bool is_init_successful = false;

	/* Configure the PWM For Pusher*/
	config_status = pwm_configure(GPIO_PUSHER_PWM, PWM_AF_FREQ);

	/* check if configuration and initialization is successful */
	if ((config_status) && (init_pwm_status))
	{
		/* Pusher is configured and initialized successfully */
		is_init_successful = true;
		printf("Pusher GPIO configured successfully\r\n");
	}
	else
	{
		/* Handle error */
		printf("Pusher PWM configuration failed\r\n");
	}

	return is_init_successful;
}

/**
 * @brief Sets the PWM duty cycle for the pusher actuator.
 *
 * This function clamps the input duty cycle to the allowed PWM range defined by
 * PWM_MIN and PWM_MAX, then sets the PWM output on the GPIO_PUSHER_PWM pin.
 *
 * @param duty_cycle_us The desired PWM duty cycle in microseconds as a double.
 * @return true if the PWM was set successfully, false otherwise.
 */
void ach_set_pusher_pwm(double duty_cycle_us)
{
	/* Clamp the PWM Output */
	if (duty_cycle_us < PWM_MIN)
	{
		DutyCycle_uSec = PWM_MIN; /* Clamp to minimum PWM value */
	}
	else if (duty_cycle_us > PWM_MAX)
	{
		DutyCycle_uSec = PWM_MAX; /* Clamp to maximum PWM value */
	}
	else
	{
		DutyCycle_uSec = duty_cycle_us; /* Use the provided duty cycle */
	}
}

/**
 * @brief Sets the PWM duty cycle for the pusher in periodic mode.
 *
 * This function configures the PWM output on the pusher's GPIO pin
 * using the specified duty cycle. It is intended to be called periodically
 * to update or maintain the pusher's state.
 *
 * @return true if the PWM was set successfully, false otherwise.
 */
bool ach_cmd_pusher_periodic(void)
{
	bool status = false;

	/* Set the PWM for the pusher */
	status = pwm_set(GPIO_PUSHER_PWM, DutyCycle_uSec);

	return status;
}
