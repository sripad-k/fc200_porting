/****************************************************
 *  sys_srv_main.c
 *  Created on: 25-Aug-2025 10:25:57 AM
 *  Implementation of the Class sys_srv_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "sys_srv_main.h"
#include "uart_interface.h"
#include "sru/fcu/d_fcu.h"
#include "soc/timer/d_timer.h"
#include "soc/timer/d_timer_counter.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "xparameters.h"
#include "xscugic.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/date_time/d_date_time.h"
#include "uart_interface.h"

static bool TaskEventFlag = false;
static uint32_t ElapsedTicksinMillisec = 0;
static const d_Timer_t LOOP_TIMER = d_TIMER_TTC0_0;

/**
 * @brief System boot initialization function
 *
 * Initializes system timers required for system operation:
 * - Configures TTC0_0 timer as a software timer for main loop triggering
 * - Initializes free-running utility timer with 1.5625 MHz frequency
 *
 * Timer Configuration Details:
 * - Timer frequency: 100 MHz / 64 = 1.5625 MHz
 * - Tick duration: 640 ns (0.64 �s)
 * - 32-bit counter wraparound time: approximately 45 minutes
 *
 * @param None
 * @return None
 *
 * @note This function must be called during system initialization
 * @note The utility timer provides system-wide timing services
 */
void sys_boot(void)
{

	const Char_t error_msg[80] = "\n\r !!!! FC-200 Initialization Failed !!!! \n\r";
	const Char_t success_msg[80] = "\n\r **** FC-200 Initialization Successful ****\n\r";
	const Char_t init_start_msg[80] = "\n\r **** FC-200 Initialization Started **** \n\r";

	d_Status_t fcuInit;
	bool iocAOnline;
	bool iocBOnline;
	int32_t currSelectedMaster = 0;
	uint32_t slotNumber = 0;

	uart_write(UART_DEBUG_CONSOLE, (uint8_t *)init_start_msg, sizeof(init_start_msg));

	/* Free running Utility Timer */
	/* Timer frequency = 100 MHz / 64 = 1.5625 MHz */
	/* Tick duration = 1 / 1,562,500 Hz = 0.64 �s = 640 ns */
	/* Calculation of Wraparound Time for a 32-bit integer */
	/* 32-bit counter max = 2^32 = 4,294,967,296 ticks */
	/* Wrap time = 4,294,967,296 � 0.64 �s = 2,748,364.8 ms = 2,748.3648 s = approx 45 minutes */
	d_TIMER_Initialise();
	fcuInit = d_FCU_Initialise();

	iocAOnline = d_FCU_IocOnline(d_FCU_IOC_A);
	iocBOnline = d_FCU_IocOnline(d_FCU_IOC_B);

	/* Check if all components are online */
	if ((fcuInit != d_STATUS_SUCCESS) || (iocAOnline == false) || (iocBOnline == false))
	{
		/* send initialization error message */
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)error_msg, sizeof(error_msg));
	}
	else
	{
		/* Initialization successful message */
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)success_msg, sizeof(success_msg));
		d_INT_IrqDeviceInitialise();
	}

	if (d_FCU_IocOnline(d_FCU_IOC_A) == d_TRUE)
	{
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rIOC-A is Online.\n\r", 21);
	}
	else
	{
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rIOC-A is Offline.\n\r", 22);
	}

	if (d_FCU_IocOnline(d_FCU_IOC_B) == d_TRUE)
	{
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rIOC-B is Online.\n\r", 21);
	}
	else
	{
		uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rIOC-B is Offline.\n\r", 22);
	}

	/* Read Slot Number */
	slotNumber = d_FCU_SlotNumber();

	/* Read Master/Slave */
	currSelectedMaster = d_FCU_GetMaster();

	/* Set Master/Slave based on Slot Number and current selection */
	if (slotNumber == 0)
	{
		/* Set FCU1 as Master/Slave */
		if (currSelectedMaster == 0)
		{
			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rFCU1 Selected as Sync master.\n\r", 34);
			d_FCU_SetMaster();
		}
		else
		{
			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rFCU1 Selected as Sync Slave.\n\r", 34);
			d_FCU_SetSlave();
		}
	}
	else
	{
		/* Set FCU2 as Master/Slave */
		if (currSelectedMaster == 1)
		{
			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rFCU2 Selected as Sync master.\n\r", 34);
			d_FCU_SetMaster();
		}
		else
		{
			uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"\n\rFCU2 Selected as Sync Slave.\n\r", 34);
			d_FCU_SetSlave();
		}
	}

	return;
}

/**
 * @brief Sets the system tick period and configures the timer
 *
 * This function configures the TTC0_0 timer with the specified tick period,
 * starts the timer, enables interval interrupts, and sets up the interrupt
 * priority and trigger type.
 *
 * @param timer_tick_period The desired timer tick period in timer units.
 *                         The actual interval programmed is (timer_tick_period - 1)
 *
 * @return None
 *
 * @note The function uses TTC0_0 timer and configures it with:
 *       - Interval: timer_tick_period - 1
 *       - Interrupt priority: 224
 *       - Interrupt trigger: Rising edge
 *       - Interrupt ID: XPS_TTC0_0_INT_ID
 */
void sys_set_tick_period(uint64_t timer_tick_period)
{

	/* SW Timer used to trigger main loop. */
	(void)d_TIMER_Configure(LOOP_TIMER, d_FALSE, 0);
	(void)d_TIMER_Options(LOOP_TIMER, d_TRUE);
	(void)d_TIMER_Interval(LOOP_TIMER, timer_tick_period - 1);
	(void)d_TIMER_Start(LOOP_TIMER);
	(void)d_TIMER_InterruptEnable(LOOP_TIMER, d_TIMER_INTERRUPT_INTERVAL);

	/* Initialize IRQ */
	d_INT_IrqDeviceInitialise();

	d_INT_IrqSetPriorityTriggerType(XPS_TTC0_0_INT_ID, 224, d_INT_RISING_EDGE);
	d_INT_IrqSetPriorityTriggerType(XPAR_FABRIC_SYNCHRONISER_IRQ_INTR, 232, d_INT_RISING_EDGE);

	/* Enable all interrupt once timer initialization is done*/
	d_INT_Enable();

	/* Enable interrupt */
	d_INT_IrqEnable(XPS_TTC0_0_INT_ID);

	return;
}

/**
 * @brief Suspends execution until a tick handler is registered
 *
 * This function implements a blocking sleep mechanism that waits for a tick
 * handler to be registered. It measures the elapsed time during the sleep
 * period and resets the tick handler registration flag upon completion.
 *
 * @details The function:
 *          - Records the start time using timer ticks
 *          - Blocks execution in a busy-wait loop until TickHandlerRegistered becomes true
 *          - Calculates and stores the elapsed time in milliseconds
 *          - Resets the TickHandlerRegistered flag for subsequent sleep operations
 *
 * @note This function uses busy-waiting which consumes CPU cycles during sleep.
 *       The global variable ElapsedTicksinMillisec is updated with the sleep duration.
 *
 * @warning This function will block indefinitely if TickHandlerRegistered is never set to true.
 *
 * @param None
 * @return None
 *
 * @see d_TIMER_ReadValueInTicks()
 * @see d_TIMER_ElapsedMilliseconds()
 */
void sys_sleep(void)
{

	/* Record start time */
	uint32_t startTime = d_TIMER_ReadValueInTicks();
	/* Wait until TaskEventFlag is set to true */
	while (!TaskEventFlag)
	{
		/* Do nothing for this time */
		;
	}
	/* Calculate elapsed time in milliseconds */
	ElapsedTicksinMillisec = d_TIMER_ElapsedMilliseconds(startTime, NULL);
	/* Reset for next sleep */
	TaskEventFlag = false;

	return;
}

/**
 * @brief Tick interrupt handler for system timer
 *
 * This function is called on each system timer tick interrupt. It performs
 * essential system timing operations including timestamp updates and timer
 * acknowledgment. The handler also manages interrupt nesting by sending
 * an End of Interrupt (EOI) signal.
 *
 * @param unusedParam Unused parameter (required for interrupt handler signature)
 *
 * @return void
 *
 * @note This function sends EOI manually to enable interrupt nesting
 * @note Sets TickHandlerRegistered flag to indicate handler execution
 *
 * @see d_DATE_TIME_TimestampUpdate()
 * @see SW_TimerAck()
 */
void sys_tickHandler(const Uint32_t parameter)
{

	uint32_t interruptStatus = 0;

	/* Update date time in timestamp */
	d_DATE_TIME_TimestampUpdate();

	/* Acknowledge the interrupt. */
	(void)d_TIMER_InterruptStatus(d_TIMER_TTC0_0, &interruptStatus);

	/* Reset the flag to resume the task*/
	TaskEventFlag = true;

	/* Send EOI to allow nesting of same interrupt. No EOI sent by interrupt handler for this interrupt */
	d_GEN_RegisterWrite(XPAR_SCUGIC_0_CPU_BASEADDR + XSCUGIC_EOI_OFFSET, XPAR_XTTCPS_0_INTR);

	return;
}
