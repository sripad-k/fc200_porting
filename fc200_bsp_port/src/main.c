#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"
#include "bsp_srv/interface/uart_interface.h"

Int32_t main()
{

    uart_init(UART_DEBUG_CONSOLE);

}
