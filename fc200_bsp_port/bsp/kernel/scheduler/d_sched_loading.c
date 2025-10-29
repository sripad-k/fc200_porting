/*********************************************************************//**
\file
\brief
  Module Title       : Processor loading measurement

  Abstract           : Provides functions to measure the processor
                       loading for tasks and interrupts.

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

#include "kernel/general/d_gen_memory.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "xparameters.h"

#include "soc/timer/d_timer.h"

#include "soc/interrupt_manager/d_int_critical.h"
#include "kernel/scheduler/d_sched_loading_cfg.h"
#include "kernel/scheduler/d_sched_loading.h"

/***** Constants ********************************************************/

#define TASK_ID_BACKGROUND 0u

#define TASK_STACK_SIZE 20u

#define LOG_SIZE 1000u

/* Clock frequency in Hz */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Float32_t CLOCK_FREQUENCY = (Float32_t)(100000000.0f / 64.0f);

/* Clock resolution in milliseconds */
// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Float32_t CLOCK_RESOLUTION = 1000.0f / CLOCK_FREQUENCY;

enum
{
  TASK_START = 0,
  TASK_END
};

/***** Type Definitions *************************************************/

/* Structure of events in the event buffer */
typedef struct
{
  Uint32_t logTime;
  Uint32_t logEvent;
  Uint32_t logTaskId;
} LogEntry_t;

/* Structure of task execution times as determined from the events */
typedef struct
{
  Uint32_t currentElapsed;
  Uint32_t maximumElapsed;
  Uint32_t elapsedCount;
  Uint32_t accumulatedElapsed;
} TaskStats_t;

/***** Variables ********************************************************/

static LogEntry_t    logEntries[LOG_SIZE];
static Uint32_t      logEntryIn;
static Uint32_t      logEntryOut;
static Uint32_t      logEntrySize;

static Uint32_t      taskStack[TASK_STACK_SIZE];
static Uint32_t      taskStackIndex;

static TaskStats_t   taskStats[MAX_LOADING_TASKS];
static Uint32_t      currentStart;
static Uint32_t      currentTask;
static Uint32_t      captureStartTime;
static Uint32_t      captureEndTime;

static Uint32_t      maximumLogSize;
static Uint32_t      maximumStackSize;

static d_SCHED_TaskMetrics_t taskMetrics[MAX_LOADING_TASKS];

/***** Function Declarations ********************************************/

static void LogEntryAdd(const Uint32_t task, const Uint32_t event);

static void ResetStats(void);

static void TaskStartProcess(void);
static void TaskEndProcess(void);

/*********************************************************************//**
  <!-- d_SCHED_LoadingInitialise -->

  Initialise loading measurement.
*************************************************************************/
d_Status_t                       /** \return SUCCESS or FAILURE */
d_SCHED_LoadingInitialise
(
void
)
{
  d_Status_t retval;
 
  retval = d_STATUS_SUCCESS;
  if (LOADING_TASK_COUNT > MAX_LOADING_TASKS)
  {
    /* Call error handler */
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    retval = d_STATUS_FAILURE;
  }
  else
  {
    ResetStats();
  }

  return retval;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingTaskStart -->

  Indicate the start of a task.
*************************************************************************/
void                         /** \return None */
d_SCHED_LoadingTaskStart
(
const Uint32_t taskId        /**< [in] The task number */
)
{
  /* The task ID is validated in the logEntryAdd function */
  LogEntryAdd(taskId, TASK_START);

  return;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingTaskEnd -->

  Indicate the end of a task.
*************************************************************************/
void                         /** \return None */
d_SCHED_LoadingTaskEnd
(
const Uint32_t taskId        /**< [in] The task number */
)
{
  /* The task ID is validated in the logEntryAdd function */
  LogEntryAdd(taskId, TASK_END);

  return;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingReset -->

  Reset loading times to restart averaging.
*************************************************************************/
void                         /** \return None */
d_SCHED_LoadingReset
(
void
)
{
  ResetStats();
  
  return;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingProcessBuffer -->

  Process the event buffer to determine the time spent in each task.
*************************************************************************/
void                         /** \return None */
d_SCHED_LoadingProcessBuffer
(
void
)
{
  Uint32_t elapsed;

  while (logEntrySize > 0u)
  {
    if (logEntries[logEntryOut].logTime < currentStart)
    {
      /* Timer has wrapped */
      // gcov-jst 1 It is not practical to execute this path during bench testing.
      ResetStats();
    }
    else
    {
      if (captureStartTime != 0u)
      {
        elapsed = logEntries[logEntryOut].logTime - currentStart;
      }
      else
      {
        captureStartTime = logEntries[logEntryOut].logTime;
        elapsed = 0;
      }

      /* Add time since last marker to current task stats */
      taskStats[currentTask].currentElapsed += elapsed;
      if (currentTask == TASK_ID_BACKGROUND)
      {
        taskStats[currentTask].accumulatedElapsed = taskStats[currentTask].currentElapsed;
      }

      if (logEntries[logEntryOut].logEvent == TASK_START)
      {
        TaskStartProcess();
      }

      if (logEntries[logEntryOut].logEvent == TASK_END)
      {
        TaskEndProcess();
      }

      currentStart = logEntries[logEntryOut].logTime;
      captureEndTime = currentStart;

      logEntryOut = (logEntryOut + 1u) % LOG_SIZE;
      Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
      logEntrySize--;
      d_INT_CriticalSectionLeave(interruptFlags);
    } 
  } /* end "while (logEntrySize > 0u)" */
  
  return;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingMetrics -->

  Calculate the task loading metrics.
*************************************************************************/
Float32_t                  /** \return The total processor loading */
d_SCHED_LoadingMetrics
(
void
)
{
  Uint32_t index;
  Float32_t totalTime;
  Float32_t loading;

  // cppcheck-suppress misra-c2012-10.8; The end time will always be greater than the start time and thus the result is positive and can be expressed in a single precision float
  totalTime = (Float32_t)(captureEndTime - captureStartTime);

  Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
  for (index = 0; index < LOADING_TASK_COUNT; index++)
  {
    if (taskStats[index].elapsedCount > 0u)
    {
      taskMetrics[index].averageTime = CLOCK_RESOLUTION * (Float32_t)taskStats[index].accumulatedElapsed / (Float32_t)taskStats[index].elapsedCount;
    }
    else
    {
      taskMetrics[index].averageTime = 0.0f;
    }
    taskMetrics[index].maximumTime = CLOCK_RESOLUTION * (Float32_t)taskStats[index].maximumElapsed;
    if (totalTime > 0.0f)
    {
      taskMetrics[index].frequency = CLOCK_FREQUENCY * (Float32_t)taskStats[index].elapsedCount / totalTime;
      taskMetrics[index].loading = 100.0f * (Float32_t)taskStats[index].accumulatedElapsed / totalTime;
    }
    else
    {
      // gcov-jst 2 It is not practical to execute this path during bench testing.
      taskMetrics[index].frequency = 0.0f;
      taskMetrics[index].loading = 0.0f;
    }
  }

  loading = 0.0f;
  for (index = (Uint32_t)TASK_ID_BACKGROUND + 1u; index < LOADING_TASK_COUNT; index++)
  {
    loading = loading + taskMetrics[index].loading;
  }

  /* Fix background task, only loading is of interest */
  taskMetrics[TASK_ID_BACKGROUND].averageTime = 0.0f;
  taskMetrics[TASK_ID_BACKGROUND].maximumTime = 0.0f;
  taskMetrics[TASK_ID_BACKGROUND].frequency = 0.0f;
  taskMetrics[TASK_ID_BACKGROUND].loading = 100.0f - loading;

  d_INT_CriticalSectionLeave(interruptFlags);

  return loading;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingGetMetrics -->

  Get the loading data for sending to a host PC.
*************************************************************************/
Uint32_t                            /** \return Number of bytes in metrics data */
d_SCHED_LoadingGetMetrics
(
Uint8_t * const destination,        /**< [out] Location to copy the data to */
const Uint32_t maxCount             /**< [in] The maximum number of bytes that may be copied */
)
{
  Uint32_t length;

  if (destination == NULL)
  {
    /* On invalid pointer return zero for the number of bytes copied */
    length = 0;
  }
  else
  {
    length = LOADING_TASK_COUNT * sizeof(d_SCHED_TaskMetrics_t);
    /* Limit the about of data to be copied */
    if (length > maxCount)
    {
      length = maxCount;
    }
    Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
    d_GEN_MemoryCopy(destination, (Uint8_t *)&taskMetrics, length);
    d_INT_CriticalSectionLeave(interruptFlags);
  }

  return length;
}

/*********************************************************************//**
  <!-- d_SCHED_LoadingGetMetric -->

  Get the loading data for specific task.
*************************************************************************/
d_Status_t                              /** \return SUCCESS or FAILURE */
d_SCHED_LoadingGetMetric
(
const Uint32_t task,                    /**< [in]  Task number */
d_SCHED_TaskMetrics_t * const pMetrics  /**< [out] Pointer to storage for the metric data */
)
{
  if (task >= MAX_LOADING_TASKS)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, task, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (pMetrics == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
  pMetrics->averageTime = taskMetrics[task].averageTime;
  pMetrics->maximumTime = taskMetrics[task].maximumTime;
  pMetrics->frequency = taskMetrics[task].frequency;
  pMetrics->loading = taskMetrics[task].loading;
  d_INT_CriticalSectionLeave(interruptFlags);
  
  return d_STATUS_SUCCESS;
}


/*********************************************************************//**
  <!-- LogEntryAdd -->

  Add a log event to the buffer.
*************************************************************************/
static void             /** \return None */
LogEntryAdd
(
const Uint32_t task,    /**< [in] Task number */
const Uint32_t event    /**< [in] Event */
)
{
  /* Ensure the buffer is not full */
  if ((logEntrySize < LOG_SIZE) && (task < LOADING_TASK_COUNT))
  {
    Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
    logEntries[logEntryIn].logTime = d_TIMER_ReadValueInTicks();
    logEntries[logEntryIn].logEvent = event;
    logEntries[logEntryIn].logTaskId = task;
    logEntryIn = (logEntryIn + 1u) % LOG_SIZE;
    logEntrySize++;
    if (logEntrySize > maximumLogSize)
    {
      /* Monitor the maximum buffer size for diagnostic purposes */
      maximumLogSize = logEntrySize;
    }
    d_INT_CriticalSectionLeave(interruptFlags);
  }

  return;
}

/*********************************************************************//**
  <!-- ResetStats -->

  Reset the cummulative data.
*************************************************************************/
static void      /** \return None */
ResetStats
(
void
)
{
  Uint32_t interruptFlags = d_INT_CriticalSectionEnter();
  logEntryIn = 0;
  logEntryOut = 0;
  logEntrySize = 0;
  
  taskStackIndex = 0;
  
  currentTask = TASK_ID_BACKGROUND;
  currentStart = 0;

  captureStartTime = 0;
  captureEndTime = 0;

  maximumLogSize = 0;
  maximumStackSize = 0;
  
  Uint32_t index;
  for (index = 0; index < MAX_LOADING_TASKS; index++)
  {
    taskStats[index].currentElapsed = 0;
    taskStats[index].maximumElapsed = 0;
    taskStats[index].accumulatedElapsed = 0;
    taskStats[index].elapsedCount = 0;
  }
  d_INT_CriticalSectionLeave(interruptFlags);

  return;
}

/*********************************************************************//**
  <!-- TaskStartProcess -->

  Process task start log entry.
*************************************************************************/
static void      /** \return None */
TaskStartProcess
(
void
)
{
  if (currentTask != TASK_ID_BACKGROUND)
  {
    taskStack[taskStackIndex] = currentTask;
    taskStackIndex++;
    if (taskStackIndex > maximumStackSize)
    {
      maximumStackSize = taskStackIndex;
    }
  }
  currentTask = logEntries[logEntryOut].logTaskId;
  taskStats[currentTask].currentElapsed = 0;
  
  return;
}

/*********************************************************************//**
  <!-- TaskEndProcess -->

  Process task end log entry.
*************************************************************************/
static void      /** \return None */
TaskEndProcess
(
void
)
{
  if (taskStats[currentTask].currentElapsed > taskStats[currentTask].maximumElapsed)
  {
    taskStats[currentTask].maximumElapsed = taskStats[currentTask].currentElapsed;
  }
  taskStats[currentTask].accumulatedElapsed += taskStats[currentTask].currentElapsed;
  taskStats[currentTask].elapsedCount++;

  if (taskStackIndex > 0u)
  {
    taskStackIndex--;
    currentTask = taskStack[taskStackIndex];
  }
  
  return;
}
