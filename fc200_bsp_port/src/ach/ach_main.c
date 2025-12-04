/****************************************************
 *  ach_main.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class ach_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "ach_main.h"

/**
 * @brief Initializes the ACH subsystem by initializing the servo and EPU modules.
 *
 * This function attempts to initialize both the servo and EPU components required for the ACH subsystem.
 * It prints status messages indicating the success or failure of each initialization step.
 *
 * @return true if both servo and EPU initialization succeed, false otherwise.
 */
void ach_init(void)
{
    bool is_init_successful = false;

    is_init_successful = ach_servo_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("Servo init failed\r\n");
    }
    else
    {
        printf("Servo init successful\r\n");
    }

    is_init_successful = ach_epu_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("EPU init failed\r\n");
    }
    else
    {
        printf("EPU init successful\r\n");
    }

    is_init_successful = ach_pusher_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("Pusher init failed\r\n");
    }
    else
    {
        printf("Pusher init successful\r\n");
    }
}

/**
 * @brief Periodically reads data from ACH subsystems.
 *
 * This function calls the periodic read routines for both the servo and EPU
 * (Electronic Processing Unit) subsystems. It is intended to be invoked at
 * regular intervals to ensure timely data acquisition from these components.
 *
 * @note Ensure that ach_servo_read_periodic() and ach_epu_read_periodic()
 *       are properly implemented and initialized before calling this function.
 */
void ach_read_periodic()
{
    ach_servo_read_periodic();
    ach_epu_read_periodic();
    /* Pusher does not provide a feedback */
}

/**
 * @brief Executes periodic command routines for the ACH module.
 *
 * This function calls the periodic routines for both the servo and EPU subsystems.
 * It should be invoked at regular intervals to ensure timely execution of subsystem tasks.
 *
 * Calls:
 *  - ach_cmd_servo_periodic(): Handles periodic tasks for the servo subsystem.
 *  - ach_cmd_epu_periodic(): Handles periodic tasks for the EPU subsystem.
 */
void ach_cmd_periodic(void)
{
    ach_cmd_servo_periodic();
    ach_cmd_epu_periodic();
    (void)ach_cmd_pusher_periodic();
}
