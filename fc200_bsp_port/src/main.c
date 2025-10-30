#include "type.h"
#include "uart_interface.h"
#include "soc/timer/d_timer.h"
#include "sys_srv_interface.h"

Int32_t main()
{

	/* FC-200 initialization */
    sys_boot();
    /* Init debug UART */
    uart_init(UART_DEBUG_CONSOLE);

    /* Message over the UART */
    const Char_t hello_message[80] = "\n\rHello, this is the FC-200.\n\r";
    const Char_t loop_msg[80] = "\n\r Main Loop \n\r";

    uart_write(UART_DEBUG_CONSOLE, (uint8_t*)hello_message, sizeof(hello_message));
    sys_set_tick_period(1000); // Set tick period to 1000 timer units (1 ms)
    while(1)
    {
        // Main loop
        uart_write(UART_DEBUG_CONSOLE, (uint8_t*)loop_msg, sizeof(loop_msg));
        sys_sleep();

    }

}
