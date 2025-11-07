/****************************************************
 *  da_main.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "da_main.h"

/* Operation 'da_init' of Class 'DA_main' */
void da_init(void)
{
    /* SyncableUserCode{6E3C160E-B9FB-499d-9EF6-E5B64A2F1187}:Nbrlk8aPUZ */

    bool is_init_successful = false;

    is_init_successful = da_ins_il_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("INS init failed\r\n");
    }
    else
    {
        printf("INS init successful\r\n");
    }

    is_init_successful = da_adc_9_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("ADC init failed\r\n");
    }
    else
    {
        printf("ADC init successful\r\n");
    }

    is_init_successful = da_radalt_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("RADALT init failed\r\n");
    }
    else
    {
        printf("RADALT init successful\r\n");
    }
    is_init_successful = da_sbus_init();
    if (is_init_successful == false)
    {
        /* Handle error */
        printf("SBUS init failed\r\n");
    }
    else
    {
        printf("SBUS init successful\r\n");
    }

    /* SyncableUserCode{6E3C160E-B9FB-499d-9EF6-E5B64A2F1187} */
}

/* Operation 'da_periodic' of Class 'DA_main' */
void da_periodic(void)
{
	uint32_t count = 0; //MUST BE DELETED AFTER FPGA FIX
    /* SyncableUserCode{DC606085-4129-4df6-B1F4-8768A171A8B6}:Nbrlk8aPUZ */
    /* Read INS_D Periodically */
    da_ins_il_read_periodic();

    /* Read Air Data Computer Periodically */
    da_adc_9_read_periodic();

    /* Read Radar Altimeter Periodically */
    da_radalt_read_periodic();

    /* Read SBUS Periodically */
    da_sbus_read_periodic();

    /* WARNING: THIS IS A TEMPORARY FIX. THIS MUST BE FIXED IN FPGA H/W */
    if( count == 0 )
    {
		uart_write(UART_INS, (uint8_t *)"1", 2);

		uart_write(UART_RADALT, (uint8_t *)"2", 2);

		uart_write(UART_ADS, (uint8_t *)"3", 2);

		uart_write(UART_SBUS, (uint8_t *)"4", 2);
    }

    count = (count + 1)%100;

    /* SyncableUserCode{DC606085-4129-4df6-B1F4-8768A171A8B6} */
}
