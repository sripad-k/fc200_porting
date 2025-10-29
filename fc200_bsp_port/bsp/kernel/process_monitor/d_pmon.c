/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Process Monitor

  Abstract : Diagnostic reading of any memory location.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-189
                       SDD References: 136T-2200-131100-001-D22 SWDES-2442
                                                                SWDES-2443
                                                                SWDES-2444
                                                                SWDES-2444
\note
  CSC ID             : SWDES-2421
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/general/d_gen_memory.h"
#include "soc/defines/d_common_types.h"
#include "soc/timer/d_timer.h"
#include "kernel/error_handler/d_error_handler.h"
#ifndef FREERTOS
#include "kernel/scheduler/d_sched_loading_cfg.h"
#include "kernel/scheduler/d_sched_loading.h"
#endif
#include "soc/interrupt_manager/d_int_critical.h"
#include "sru/ethernet/d_eth_interface.h"

#include "d_pmon.h"

/***** Constants ********************************************************/

/* Time, in milliseconds, after which data transmission stops unless new request received */
#define CONTINUE_TIME           2000u

/* Maximum number of items that may be requested in a single request message */
#define MAX_ITEMS               256u

/* Array of memory assigned for test variables with fixed locations */
#define TEST_ADDRESS  0x50000000u
#define TEST_SIZE     256u

/***** Type Definitions *************************************************/

typedef enum
{
  PMON_COMMAND_NONE = 0,
  PMON_COMMAND_DATA,
  PMON_COMMAND_LOADING,
  PMON_COMMAND_LOADING_RESET,
} PmonCommand_t;

typedef struct
{
  Uint8_t *address;
  Uint16_t count;
} DataItem_t;

/***** Variables ********************************************************/

static DataItem_t DataItems[MAX_ITEMS];
static Uint32_t dataItemCount;

static Uint32_t host_ip;
static Uint32_t host_port;
static Uint32_t requestTime;
// cppcheck-suppress misra-c2012-8.9; Defining this large variable at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Uint8_t transmitMessage[d_ETH_MAX_UDP_PACKET_DATA];

static Uint16_t responseNumber;
static PmonCommand_t requestCommand;

#ifndef FREERTOS
static Bool_t sendLoading;
#endif
static Bool_t sendData;
static Bool_t requestChanged;

/* Linker defined variables indicating valid memory regions */
// clang-tidy-jst bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp 4 Externally defined linker generated variables. Used to validate addressing and does not present a risk.
extern Uint32_t _vector_table;
extern Uint32_t __rodata1_end;
extern Uint32_t __sdata2_start;
extern Uint32_t _stack_end;

/***** Function Declarations ********************************************/

static void storeUint16(Uint8_t * const buffer, const Uint16_t value);

static void ReceiveCallback(const d_ETH_Ipv4Addr_t sourceAddress,
                            const Uint16_t sourcePort, 
                            const Uint16_t destinationPort, 
                            const Uint8_t * const buffer, 
                            const Uint32_t length);


/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_PMON_Initialise -->

  Configure the Process Monitor CSC
*************************************************************************/
d_Status_t                 /** \return Success or Failure */
d_PMON_Initialise
(
void
)
{
  d_Status_t status;

  requestTime = 0;
  requestCommand = PMON_COMMAND_NONE;
  dataItemCount = 0;
#ifndef FREERTOS
  sendLoading = d_FALSE;
#endif
  sendData = d_FALSE;
  requestChanged = d_FALSE;
  
  /* Setup a UDP listening port */
  status = d_ETH_UdpListen(d_ETH_PORT_PROCESS_MONITOR, ReceiveCallback);

  if (status != d_STATUS_SUCCESS)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_NOT_READY, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, status, 0, 0, 0);
  }
  else
  {
    DO_NOTHING();
  }
  
  return status;
}

/*********************************************************************//**
  <!-- d_PMON_Cyclic -->

  Cyclic process monitor function.
*************************************************************************/
void                 /** \return None */
d_PMON_Cyclic
(
void
)
{
  Uint32_t elapsed;
  const Uint8_t REQUEST_FLAG = 0x80u;

  elapsed = d_TIMER_ElapsedMilliseconds(requestTime, NULL);

#ifndef FREERTOS
  if (sendLoading == d_TRUE)
  {
    sendLoading = d_FALSE;
    storeUint16(&transmitMessage[0], responseNumber);
    transmitMessage[2] = (Uint8_t)requestCommand | REQUEST_FLAG;
    transmitMessage[3] = LOADING_TASK_COUNT;
    Uint32_t length = d_SCHED_LoadingGetMetrics(&transmitMessage[4], d_ETH_MAX_UDP_PACKET_DATA);
    (void)d_ETH_UdpSend(host_ip, host_port, transmitMessage, length + 4u);
    responseNumber++;
  }
  else
#endif
  if (elapsed < CONTINUE_TIME)
  {
    if (sendData == d_TRUE)
    {
      Uint32_t index;
      Uint32_t txindex;
    
      requestChanged = d_FALSE;
      storeUint16(&transmitMessage[0], responseNumber);
  
      transmitMessage[2] = (Uint8_t)requestCommand | REQUEST_FLAG;
      transmitMessage[3] = dataItemCount;
      txindex = 4;
      for (index = 0; index < dataItemCount; index++)
      {
        /* Make sure space in buffer for next entry */
        if ((txindex + (Uint32_t)DataItems[index].count) <= d_ETH_MAX_UDP_PACKET_DATA)
        {
          /* Respond with zeros if invalid address */  
          if (DataItems[index].address == NULL)
          {
            d_GEN_MemorySet(&transmitMessage[txindex], 0, DataItems[index].count);
          }      
          else
          {
            d_GEN_MemoryCopy(&transmitMessage[txindex], DataItems[index].address, DataItems[index].count);
          }
          txindex += DataItems[index].count;
        }
        else
        {
          // gcov-jst 1 It is not practical to generate this error during bench testing.
          DO_NOTHING();
        }
      }
      if (requestChanged == d_FALSE)
      {
        /* Do not send the message if the request changed while processing */
        (void)d_ETH_UdpSend(host_ip, host_port, transmitMessage, txindex);
        responseNumber++;
      }
      else
      {
  	    // gcov-jst 1 It is not practical to generate this coverage during bench testing.
        DO_NOTHING();
      }
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this coverage during bench testing.
      DO_NOTHING();
    }
  }
  else
  {
    sendData = d_FALSE;
  }
  
  return;
}

/*********************************************************************//**
  <!-- ReceiveCallback -->

  Callback function when UDP packet received.
*************************************************************************/
static void                            /** \return None */
ReceiveCallback
(
const d_ETH_Ipv4Addr_t sourceAddress,  /**< [in] source IP address */
const Uint16_t sourcePort,             /**< [in] source port number */
const Uint16_t destinationPort,        /**< [in] destination port number */
const Uint8_t * const buffer,          /**< [in] packet buffer */
const Uint32_t length                  /**< [in] data length */
)
{
  Uint32_t address;
  Uint8_t *dataPtr;

  UNUSED_PARAMETER(destinationPort);

  host_ip = sourceAddress;
  host_port = sourcePort;
  d_GEN_MemoryCopy((Uint8_t *)&responseNumber, &buffer[0], sizeof(responseNumber));
  requestCommand = buffer[2];
  if (requestCommand == PMON_COMMAND_DATA)
  {
    /* We do not use the item count value, rather just step through data to end of packet */
    dataItemCount = 0;
    Uint32_t index = 4;
    while ((index < length) && (dataItemCount < MAX_ITEMS))
    {
      d_GEN_MemoryCopy((Uint8_t *)&address, &buffer[index], sizeof(address));
      // cppcheck-suppress misra-c2012-11.4; Conversion required to convert protocol 4 byte address to pointer. Violation of 'Advisory' rule does not present a risk.
      dataPtr = (Uint8_t *)address;
      if (((dataPtr < (Uint8_t *)&_vector_table) || (dataPtr >= (Uint8_t *)&__rodata1_end)) &&
          ((dataPtr < (Uint8_t *)&__sdata2_start) || (dataPtr >= (Uint8_t *)&_stack_end)) &&
	        // cppcheck-suppress misra-c2012-11.4; Conversion required to convert protocol 4 byte address to pointer. Violation of 'Advisory' rule does not present a risk.
          ((dataPtr < (Uint8_t *)TEST_ADDRESS) || (dataPtr >= (Uint8_t *)(TEST_ADDRESS + TEST_SIZE))))
      {
        dataPtr = NULL;
      }
      else
      {
        DO_NOTHING();
      }
      DataItems[dataItemCount].address = dataPtr;
      d_GEN_MemoryCopy((Uint8_t *)&DataItems[dataItemCount].count, &buffer[index + 4u], sizeof(DataItems[dataItemCount].count));
      dataItemCount++;
      index = index + 6u;
    }
    /* Save the time at which the request was received */
    requestTime = d_TIMER_ReadValueInTicks();
    
    /* indicate that a new request has been received */
    requestChanged = d_TRUE;
  
    /* Set flag to send data to host */
    sendData = d_TRUE;
  }
  
#ifndef FREERTOS
  else if (requestCommand == PMON_COMMAND_LOADING_RESET)
  {
    /* Clear the loading data */
    d_SCHED_LoadingReset();
    
    /* Set flag to send loading to host */
    sendLoading = d_TRUE;
  }
  
  else if (requestCommand == PMON_COMMAND_LOADING)
  {
    /* Set flag to send loading to host */
    sendLoading = d_TRUE;
  }
#endif
  else
  {
    /* ignore unrecognised command */
    DO_NOTHING();
  }
  
  return;
}

/*********************************************************************//**
  <!-- storeUint16 -->

  Store a 16 bit value in a buffer.
*************************************************************************/
static void                 /** \return None */
storeUint16
(
Uint8_t * const buffer,     /**< [in] Address where value is to be stored */
const Uint16_t value        /**< [in] Value to be stored */
)
{
  d_GEN_MemoryCopy(buffer, (const Uint8_t *)&value, sizeof(value));
  
  return;
}

