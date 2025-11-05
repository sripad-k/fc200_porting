/*
 * ***************************************************
 * Model File: Model::UART_Interface_Domain::UART_peripherals_t
 * Model Path: C:\Users\SripadKulkarni\Code_Vault\UML Modelling Test\Data_Acquisition_Interface.qea
 *
 * 2025-04-11  - 23:04
 * ***************************************************
 */
#ifndef H_UART_PERIPHERALS_T
#define H_UART_PERIPHERALS_T

typedef enum
{
    UART_DEBUG_CONSOLE = 0, /* Explicit value specification added by code generator */
    UART_RADALT,
    UART_SBUS,
    UART_INS,
    UART_SERVO,
    UART_ADS,
	UART_LOS,
    UART_MAX_PERIPHERAL
} UART_peripherals_t;

#endif /* H_UART_PERIPHERALS_T */
