#include "type.h"
#include "sys_srv_interface.h"
#include "gpio_interface.h"
#include "pwm_interface.h"
#include "da_interface.h"
#include "ach_interface.h"
#include "udp_interface.h"
#include "mavlink_io.h"
#include "fcs_mi_interface.h"
#define ONE_MSEC (1000000UL)


Int32_t main()
{

	sys_boot();

    /* Initialize default GPIO configuration */
    gpio_init();

    gpio_configure_all();

    /* Initialize default PWM Configuration */
    pwm_init();

    /* Initialise all Sensors to Acquire data */
    da_init();

    /* Initialise Actuator Control Hub */
    ach_init();

    /* Initialise Flight Control System */
    fcs_mi_init();

    /* Set tick period to 1000 timer units (1 ms)*/
	sys_set_tick_period(ONE_MSEC);

    /* Initialize default MAVLINK IO */
    mav_io_init();

	while (1)
	{
		/* Call periodically to check if UDP messages are received */
		udp_sync_periodic();

        /* Run Periodic Data Acquisition of all Sensors */
        da_periodic();

        /* read the inbound mavlink message */
        mavlink_io_recv_periodic();

        /* periodic flight control task processing */
        fcs_mi_periodic();

        /* Issue commands to the actuators periodically */
        ach_cmd_periodic();

        /* Run Periodic Actuator Control Hub read */
        ach_read_periodic();

        /* tx the outbound mavlink message */
        mavlink_io_send_periodic();

		sys_sleep();
	}
}
