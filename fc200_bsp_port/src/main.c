#include "type.h"
#include "soc/timer/d_timer.h"
#include "soc/can/d_can_cfg.h"
#include "soc/can/d_can.h"

#include "sru/can_holt/d_can_holt.h"	 /* Discrete driver */
#include "sru/can_holt/d_can_holt_cfg.h" /* Discrete driver config */
#include "sru/ethernet/d_eth_interface.h"

#include "sys_srv_interface.h"
#include "uart_interface.h"
#include "can_interface.h"
#include "udp_interface.h"

can_msg_t tx1Message = {
	.can_msg_id = 0xA400081,
	.data = {0xFC, 0xC1, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
	.dlc = 8,
	.is_extended_id = true,
	.is_remote_frame = false};

can_msg_t rxMessage[10];

Int32_t main()
{

	static uint64_t count = 0;

	/* FC-200 initialization */
	sys_boot();
	can_init(CAN_CHANNEL_1);
	/* Init debug UART */
	uart_init(UART_DEBUG_CONSOLE);

	/* Message over the UART */
	const Char_t hello_message[80] = "\n\rHello, this is the FC-200.\n\r";
	//    const Char_t loop_msg[80] = "\n\r Main Loop \n\r";

	uart_write(UART_DEBUG_CONSOLE, (uint8_t *)hello_message, sizeof(hello_message));
	sys_set_tick_period(1000000); // Set tick period to 1000 timer units (1 ms)

	udp_setup_server();
	while (1)
	{
		/* Call periodically to check if UDP messages are received */
		udp_sync_periodic();

		// Main loop
		if (count == 0)
		{
			udp_send_gcs((const uint8_t *)"FC-200 UDP GCS Test Message", 27);
		}

		if (count == 100)
		{
			udp_send_pil((const uint8_t *)"FC-200 UDP PIL Test Message", 27);
		}

		can_write(CAN_CHANNEL_1, &tx1Message);

		/* Reset all RX messages */
		for (uint8_t i = 0; i < 10; i++)
		{
			rxMessage[i] = (can_msg_t){0};
		}

		//      if(count == 5)
		//      {
		for (uint8_t i = 0; i < 10; i++)
		{
			can_read(CAN_CHANNEL_1, &rxMessage[i]);
		}
		//      }

		count = (count + 1) % 200;
		//        uart_write(UART_DEBUG_CONSOLE, (uint8_t*)loop_msg, sizeof(loop_msg));
		sys_sleep();
		//    	d_TIMER_DelayMilliseconds(10);
	}
}
