#include "type.h"
#include "bsp_srv/interface/uart_interface.h"
#include "soc/timer/d_timer.h"

Int32_t main()
{

    d_TIMER_Initialise();
    uart_init(UART_DEBUG_CONSOLE);
    const Char_t hello_message[80] = "\n\rHello, this is the FC-200.\n\r";

    while(1)
    {
        // Main loop
        d_TIMER_DelayMilliseconds(100);
        uart_write(UART_DEBUG_CONSOLE, (uint8_t*)hello_message, sizeof(hello_message));
    }

}
