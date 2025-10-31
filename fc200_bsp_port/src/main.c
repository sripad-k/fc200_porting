#include "type.h"
#include "uart_interface.h"
#include "can_interface.h"
#include "soc/timer/d_timer.h"
#include "sys_srv_interface.h"

#include "soc/can/d_can_cfg.h"
#include "soc/can/d_can.h"
#include "sru/can_holt/d_can_holt.h"	 /* Discrete driver */
#include "sru/can_holt/d_can_holt_cfg.h" /* Discrete driver config */

can_msg_t tx1Message = {
  .can_msg_id = 0xA400081,
  .data = {0xFC, 0xC1, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
  .dlc = 8,
  .is_extended_id = true,
  .is_remote_frame = false
};

can_msg_t tx2Message = {
  .can_msg_id = 0xA400082,
  .data = {0xFC, 0xC2, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
  .dlc = 8,
  .is_extended_id = true,
  .is_remote_frame = false
};


can_msg_t tx3Message = {
  .can_msg_id = 0xA400083,
  .data = {0xFC, 0xC3, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
  .dlc = 8,
  .is_extended_id = true,
  .is_remote_frame = false
};

can_msg_t tx4Message = {
  .can_msg_id = 0xA400084,
  .data = {0xFC, 0xC4, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
  .dlc = 8,
  .is_extended_id = true,
  .is_remote_frame = false
};

can_msg_t tx5Message = {
  .can_msg_id = 0xA400085,
  .data = {0xFC, 0xC5, 0xDF, 0xCC, 0xCA, 0xFE, 0xBA, 0xBE},
  .dlc = 8,
  .is_extended_id = true,
  .is_remote_frame = false
};

Int32_t main()
{

	static uint8_t count = 0;

	/* FC-200 initialization */
    sys_boot();
    can_init(CAN_CHANNEL_1);
    can_init(CAN_CHANNEL_2);
    can_init(CAN_CHANNEL_3);
    can_init(CAN_CHANNEL_4);
    can_init(CAN_CHANNEL_5);
    /* Init debug UART */
    uart_init(UART_DEBUG_CONSOLE);


    /* Message over the UART */
    const Char_t hello_message[80] = "\n\rHello, this is the FC-200.\n\r";
    const Char_t loop_msg[80] = "\n\r Main Loop \n\r";

    uart_write(UART_DEBUG_CONSOLE, (uint8_t*)hello_message, sizeof(hello_message));
    sys_set_tick_period(1000000); // Set tick period to 1000 timer units (1 ms)
    while(1)
    {
        // Main loop
    	if(count == 0)
    	{
    		can_write(CAN_CHANNEL_1, &tx1Message);
    		can_write(CAN_CHANNEL_2, &tx2Message);
    		can_write(CAN_CHANNEL_3, &tx3Message);
    		can_write(CAN_CHANNEL_4, &tx4Message);
    		can_write(CAN_CHANNEL_5, &tx5Message);
    	}

    	count = (count + 1) % 10;
//        uart_write(UART_DEBUG_CONSOLE, (uint8_t*)loop_msg, sizeof(loop_msg));
        sys_sleep();
//    	d_TIMER_DelayMilliseconds(10);


    }

}
