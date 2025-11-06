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
#include "timer_interface.h"

can_msg_t tx1Message = {
	.can_msg_id = 0xA400081,
	.data = {0xFC, 0xC1, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
	.dlc = 8,
	.is_extended_id = true,
	.is_remote_frame = false};

can_msg_t rxMessage[10];

uint16_t u64_to_ascii(uint64_t value, char *buf);


Int32_t main()
{

	static uint64_t count = 0;
	uint8_t buffer[2048] = {0};
	uint16_t length = 0;
	uint16_t index = 0;

	/* FC-200 initialization */
	sys_boot();
	can_init(CAN_CHANNEL_1);
	/* Init debug UART */
	uart_init(UART_DEBUG_CONSOLE);
	uart_init(UART_INS);

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

//		sw_timer_ticks = timer_get_system_time_ms();

		// Main loop
		if (count == 0)
		{
//			ascii_len = u64_to_ascii(sw_timer_ticks, ascii);
//			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ascii, ascii_len);
//			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\r\n", 3);

			uart_write(UART_INS, (uint8_t *)"INS", 4);
			uart_write(UART_INS, (uint8_t *)"\r\n", 3);
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

		length = uart_read(UART_INS, &buffer[index], 500);

		if(length > 0)
		{
			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"Shabash\r\n", 10);
			length = 0;
			index = (index + length) % 2048;
		}


		// udp_receive(&buffer[0], &len, UDP_SRC_GCS);
		//      }

		count = (count + 1) % 100;
		//        uart_write(UART_DEBUG_CONSOLE, (uint8_t*)loop_msg, sizeof(loop_msg));
		sys_sleep();
		//    	d_TIMER_DelayMilliseconds(10);
	}
}


uint16_t u64_to_ascii(uint64_t value, char *buf)
{
    char temp[21];   // temporary digits (max uint64 is 20 digits)
    int i = 0;

    // Special case for zero
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return 2;
    }

    // Extract digits in reverse
    while (value > 0) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Reverse into output buffer
    int j = 0;
    while (i > 0) {
        buf[j++] = temp[--i];
    }

    buf[j] = '\0';
    return j;
}
