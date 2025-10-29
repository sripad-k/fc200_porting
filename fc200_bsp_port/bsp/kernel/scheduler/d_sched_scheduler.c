/*********************************************************************//**
\file
\brief
  Module Title       : Scheduler

  Abstract           : Rate monotonic scheduler.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-74
                                                                SWREQ-75
                       SDD References: 136T-2200-131000-001-D22 SWDES-557
                                                                SWDES-664
                                                                SWDES-665
                                                                SWDES-666
                                                                SWDES-667
                                                                SWDES-668
                                                                SWDES-669
                                                                SWDES-670
                                                                SWDES-671
                                                                SWDES-672
                                                                SWDES-673
                                                                SWDES-674
                                                                SWDES-835
\note
  CSC ID             : SWDES-79
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/scheduler/d_sched_data_manage.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/scheduler/d_sched_scheduler_cfg.h"
#include "kernel/scheduler/d_sched_scheduler.h"

/***** Constants ********************************************************/

#define MAX_SCHEDULER_TASKS    (Uint32_t)10

#define TASK_ID_BACKGROUND     0u;

/***** Type Definitions *************************************************/

typedef enum
{
  TASK_STATE_IDLE = 0,
  TASK_STATE_PENDING,
  TASK_STATE_ACTIVE,
  TASK_STATE_SUSPENDED
} ScheduleTaskState_t;

/***** Variables ********************************************************/

/* 0 is background, scheduled tasks start from 1 */
static Uint32_t taskCurrent;

static Uint32_t systemCounter;

static Uint32_t timeSlot;

static ScheduleTaskState_t taskState[MAX_SCHEDULER_TASKS];

static Uint32_t TaskStack[MAX_SCHEDULER_TASKS];

static Uint32_t TaskStackPointer;

/***** Function Declarations ********************************************/

static void TaskPush(const Uint32_t task);

static Uint32_t TaskPop(void);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SCHED_SchedulerInitialise -->

  Check the configuration data and initialise the local data.
*************************************************************************/
d_Status_t                    /** \return SUCCESS or FAILURE */
d_SCHED_SchedulerInitialise
(
void
)
{
  d_Status_t retval;
  Uint32_t index;

  retval = d_STATUS_SUCCESS;

  if (SCHEDULER_TASK_COUNT > MAX_SCHEDULER_TASKS)
  {
    /* Call error handler */
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    retval = d_STATUS_FAILURE;
  }

  timeSlot = 0u;
  systemCounter = 0;

  index = 0u;
  while ((retval == d_STATUS_SUCCESS) && (index < SCHEDULER_TASK_COUNT))
  {
    /* Verify the task function is not NULL and the multiplier is an integer multiple of the previous task multiplier */
    if ((SchedulerTasks[index].task == NULL) ||
        ((index > 0u) && ((SchedulerTasks[index].multiplier % SchedulerTasks[index - 1u].multiplier) != 0u)))
    {
      /* Call error handler */
      // gcov-jst 2 It is not practical to generate this error during bench testing.
      d_ERROR_Logger(d_STATUS_BAD_DATA, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, index, 0, 0, 0);
      retval = d_STATUS_BAD_DATA;
    }
    taskState[index] = TASK_STATE_IDLE;

    index++;
  }

  taskCurrent = TASK_ID_BACKGROUND;
  TaskStackPointer = 0u;
  
  return retval;
}

/*********************************************************************//**
  <!-- d_SCHED_SchedulerTick -->

  Perform the scheduling algorithm.
*************************************************************************/
void         /** \return None */
d_SCHED_SchedulerTick
(
void
)
{
  Uint32_t index;

  systemCounter++;

  timeSlot = (timeSlot + 1u) % SchedulerTasks[SCHEDULER_TASK_COUNT - 1u].multiplier;

  /* set tasks as pending if required */
  for (index = 0; index < SCHEDULER_TASK_COUNT; index++)
  {
    if ((timeSlot % SchedulerTasks[index].multiplier) == 0u)
    {
      if (taskState[index] == TASK_STATE_ACTIVE)
      {
        /* Task overrun */
        /* We have to enable interrupts else UART tx function will block while sending entire message */
        d_INT_Enable();
        d_ERROR_Logger(d_STATUS_OVERRUN, d_ERROR_CRITICALITY_UNKNOWN, index, systemCounter, timeSlot, 0);
        d_INT_Disable();
      }
      else if (taskState[index] == TASK_STATE_IDLE)
      {
        /* Copy higher rate data to this rate variables */
        d_SCHED_DataMngTaskStart(TICK_PERIOD * SchedulerTasks[index].multiplier);
        taskState[index] = TASK_STATE_PENDING;
      }
      else
      {
        /* was already pending, should not occur, or suspended */
      }
    }
  }

  /* check for any task to execute and continue until all done */
  index = 0;
  while (index < SCHEDULER_TASK_COUNT)
  {
    if (taskState[index] == TASK_STATE_ACTIVE)
    {
      /* highest priority task is still busy, resume */
      index = SCHEDULER_TASK_COUNT;
    }
    else if (taskState[index] == TASK_STATE_PENDING)
    {
      /* Higher priority task ready */
      taskState[index] = TASK_STATE_ACTIVE;
      TaskPush(taskCurrent);
      taskCurrent = index + 1u;
      /* Execute the task */
      d_INT_Enable();
      SchedulerTasks[index].task();
      d_INT_Disable();

      /* Copy this rate data to higher rate variables */
      d_SCHED_DataMngTaskEnd(TICK_PERIOD * SchedulerTasks[index].multiplier);
      taskCurrent = TaskPop();

      /* Do not set to Idle if was suspended during operation */
      if (taskState[index] != TASK_STATE_SUSPENDED)
      {
        taskState[index] = TASK_STATE_IDLE;
      }
    }
    else
    {
      /* Task not active, pending or suspended, go on to next lower priority */
    }
    index++;
  }

  return;
}

/*********************************************************************//**
  <!-- d_SCHED_TaskSuspend -->

  Suspend a task.
*************************************************************************/
d_Status_t             /** \return Success or Failure */
d_SCHED_TaskSuspend
(
const Uint32_t task    /**< [in] Task number to suspend, numbered from 1 (0 is background which cannot be suspended) */
)
{
  d_Status_t retval;

  retval = d_STATUS_SUCCESS;
  
  if ((task == 0u) || (task > SCHEDULER_TASK_COUNT))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, task, 0, 0);
    retval = d_STATUS_INVALID_PARAMETER;
  }
  else
  {
    taskState[task - 1u] = TASK_STATE_SUSPENDED;
  }
  
  return retval;
}

/*********************************************************************//**
  <!-- TaskPush -->

  Save the task number about to be swapped out.
*************************************************************************/
static void           /** \return None */
TaskPush
(
const Uint32_t task   /**< Task number about to swapped out */
)
{
  TaskStack[TaskStackPointer] = task;
  TaskStackPointer++;

  return;
}

/*********************************************************************//**
  <!-- TaskPop -->

  Retrieve the previously executing task number.
*************************************************************************/
static Uint32_t   /** \return Previous executing task number */
TaskPop
(
void
)
{
  TaskStackPointer--;

  return TaskStack[TaskStackPointer];
}

