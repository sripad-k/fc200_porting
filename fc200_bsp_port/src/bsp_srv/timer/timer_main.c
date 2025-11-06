/****************************************************
 *  timer_main.c                                         
 *  Created on: 25-Aug-2025 10:22:24 AM                      
 *  Implementation of the Class timer_main       
 *  Copyright: LODD (c) 2025                     
 ****************************************************/

#include "timer_main.h"
#include "soc/timer/d_timer_counter.h"
#include "soc/timer/d_timer.h"

static const d_Timer_t TIMER = d_TIMER_TTC1_0;

void timer_init(void)
{

  	/* SW Timer used to trigger main loop. */
	(void)d_TIMER_Configure(TIMER, d_FALSE, 0);
	(void)d_TIMER_Options(TIMER, d_TRUE);
	(void)d_TIMER_Interval(TIMER, 100000 - 1);
	(void)d_TIMER_Start(TIMER);
	// (void)d_TIMER_InterruptEnable(TIMER, d_TIMER_INTERRUPT_INTERVAL);

	// /* Initialize IRQ */
	// d_INT_IrqDeviceInitialise();

	// d_INT_IrqSetPriorityTriggerType(XPS_TTC1_0_INT_ID, 224, d_INT_RISING_EDGE);
	// d_INT_IrqSetPriorityTriggerType(XPAR_FABRIC_SYNCHRONISER_IRQ_INTR, 232, d_INT_RISING_EDGE);

	// /* Enable all interrupt once timer initialization is done*/
	// d_INT_Enable();

	// /* Enable interrupt */
	// d_INT_IrqEnable(XPS_TTC0_0_INT_ID);
}

/**
 * @brief Get the timer value which is free running counter with resolution in ms.
 *
 * @return uint64_t
 */
uint64_t timer_get_system_time_ms(void)
{
	uint64_t timer_value = 0;
//	(void)d_TIMER_Read(TIMER, &timer_value);
    timer_value = d_TIMER_ReadValueInTicks();
    timer_value = (timer_value*640) / 1000000;
    return timer_value;
}

/**
 * @brief Starts the timer instance with a specified period.
 *
 * This function sets the timer state to TIMER_COUNTING, records the current system time as
 * the start time, and sets the timer period.
 *
 * @param ptr_timer_instance Pointer to the timer instance to start.
 * @param period The period (in system time units) for the timer.
 */
void timer_start(s_timer_data_t *ptr_timer_instance, uint64_t period)
{
    if (ptr_timer_instance != NULL)
    {
        ptr_timer_instance->state = TIMER_COUNTING;
        ptr_timer_instance->start_time = timer_get_system_time_ms();
        ptr_timer_instance->period = period;
    }
}

/**
 * @brief Checks if the specified timer instance has expired.
 *
 * This function evaluates whether the timer represented by the given
 * s_timer_data_t pointer has reached its expiration period. If the timer
 * is in the TIMER_COUNTING state and the elapsed time since its start
 * exceeds or equals its configured period, the timer state is updated
 * to TIMER_EXPIRED.
 *
 * @param ptr_timer_instance Pointer to the timer instance to check.
 *                          Must not be NULL.
 *
 * @return boolean true if timer is expired.
 */
bool timer_check_expiry(s_timer_data_t *ptr_timer_instance)
{
    bool is_expired = false;
    if (ptr_timer_instance != NULL)
    {
        uint64_t current_time = timer_get_system_time_ms();

        if (ptr_timer_instance->state == TIMER_COUNTING)
        {
            if ((current_time - ptr_timer_instance->start_time) >= ptr_timer_instance->period)
            {
                ptr_timer_instance->state = TIMER_EXPIRED;
                ptr_timer_instance->start_time = 0;
                is_expired = true;
            }
        }
        else if (ptr_timer_instance->state == TIMER_EXPIRED)
        {
            is_expired = true;
        }
        else
        {
            is_expired = false;
        }
    }

    return is_expired;
}

/**
 * @brief Resets the timer instance.
 *
 * This function sets the timer state to TIMER_STOPPED and clears the start time and period.
 *
 * @param ptr_timer_instance Pointer to the timer instance to reset.
 */
void timer_reset(s_timer_data_t *ptr_timer_instance)
{
    if (ptr_timer_instance != NULL)
    {
        ptr_timer_instance->state = TIMER_STOPPED;
        ptr_timer_instance->start_time = 0;
        ptr_timer_instance->period = 0;
    }
}

/**
 * @brief Reloads the specified timer instance.
 *
 * This function resets the start time of the given timer instance to the current system time
 * and sets its state to TIMER_COUNTING, effectively restarting the timer.
 *
 * @param ptr_timer_instance Pointer to the timer instance to be reloaded. If NULL, the function does nothing.
 */
void timer_reload(s_timer_data_t *ptr_timer_instance)
{
    if (ptr_timer_instance != NULL)
    {
        ptr_timer_instance->start_time = timer_get_system_time_ms();
        ptr_timer_instance->state = TIMER_COUNTING;
    }
}

/**
 * @brief Delays execution for a specified number of milliseconds.
 *
 * This function implements a busy-wait loop that blocks execution until the
 * specified delay in milliseconds has elapsed. It uses the system timer to
 * measure elapsed time.
 *
 * @param delay_ms The delay duration in milliseconds.
 *
 * @note This function will block the calling thread and may affect system
 *       responsiveness. Use with caution in time-critical or multitasking
 *       environments.
 */
void timer_delay(uint64_t delay_ms)
{
    uint64_t start_time = timer_get_system_time_ms();
    while ((timer_get_system_time_ms() - start_time) < delay_ms)
    {
        // Busy wait until the specified delay has passed
    }
}


//
///**
// * @brief Tick interrupt handler for system timer
// *
// * This function is called on each system timer tick interrupt. It performs
// * essential system timing operations including timestamp updates and timer
// * acknowledgment. The handler also manages interrupt nesting by sending
// * an End of Interrupt (EOI) signal.
// *
// * @param unusedParam Unused parameter (required for interrupt handler signature)
// *
// * @return void
// *
// * @note This function sends EOI manually to enable interrupt nesting
// * @note Sets TickHandlerRegistered flag to indicate handler execution
// *
// * @see d_DATE_TIME_TimestampUpdate()
// * @see SW_TimerAck()
// */
//void timer_tickHandler(const Uint32_t parameter)
//{
//
//	uint32_t interruptStatus = 0;
//
//	/* Update date time in timestamp */
//	d_DATE_TIME_TimestampUpdate();
//
//	/* Acknowledge the interrupt. */
//	(void)d_TIMER_InterruptStatus(d_TIMER_TTC0_0, &interruptStatus);
//
//	/* Reset the flag to resume the task*/
//	TaskEventFlag = true;
//
//	/* Send EOI to allow nesting of same interrupt. No EOI sent by interrupt handler for this interrupt */
//	d_GEN_RegisterWrite(XPAR_SCUGIC_0_CPU_BASEADDR + XSCUGIC_EOI_OFFSET, XPAR_XTTCPS_0_INTR);
//
//	return;
//}
