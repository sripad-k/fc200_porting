/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Shared Memory Interface Ethernet

  Abstract           : Ethernet interface

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-
                       SDD References: 136T-2200-131100-001-D22 SWDES-

\note
  CSC ID             : SWDES-80
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/timer/d_timer.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/general/d_gen_memory.h"
#include "kernel/error_handler/d_error_handler.h"
#include "soc/interrupt_manager/d_int_critical.h"
#include "sru/ethernet/d_eth_interface.h"

#include "d_smi.h"
#include "d_smi_eth.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* Structure of packet in shared memory message */
typedef struct
{
  d_ETH_Ipv4Addr_t ipAddress;
  Uint32_t sourcePort;
  Uint32_t destinationPort;
  Uint32_t payloadLength;
  Uint8_t payload[d_ETH_MAX_UDP_PACKET_DATA];
} d_ETH_UdpSmiPacket_t;

/***** Variables ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining this large variable at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static Uint8_t txBuffer[sizeof(d_ETH_UdpSmiPacket_t)];

/***** Function Declarations ********************************************/

static void ReceiveCallback(const d_ETH_Ipv4Addr_t sourceAddress,
                            const Uint16_t sourcePort, 
                            const Uint16_t destinationPort, 
                            const Uint8_t * const buffer, 
                            const Uint32_t length);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_SMI_EthInitialise -->

  Initialise the variable message length buffers.
*************************************************************************/
void                      /** \return None */
d_SMI_EthInitialise
(
void
)
{

  return;
}

/*********************************************************************//**
  <!-- d_SMI_EthPortAdd -->

  Initialise the variable message length buffers.
*************************************************************************/
d_Status_t                      /** \return None */
d_SMI_EthPortAdd
(
const Uint32_t port
)
{
  /* Setup a UDP listening port */
  d_Status_t status = d_ETH_UdpListen(port, ReceiveCallback);

  return status;
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
  d_ETH_UdpSmiPacket_t * pPacket;

  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
  *(Uint32_t *)&txBuffer[0] = 0; /* Specifiy ethernet type message */

  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
  pPacket = (d_ETH_UdpSmiPacket_t *)&txBuffer[4];
  pPacket->ipAddress = sourceAddress;
  pPacket->sourcePort = sourcePort;
  pPacket->destinationPort = destinationPort;
  pPacket->payloadLength = length;
  d_GEN_MemoryCopy(pPacket->payload, &buffer[0], length);
  
  /* Send the message to the APU */
  (void)d_SMI_Transmit(txBuffer, 20u + length);
  
  return;
}

/*********************************************************************//**
  <!-- d_SMI_EthTransmit -->

  Send received ethernet packet to ethernet stack.
*************************************************************************/
d_Status_t
d_SMI_EthTransmit
(
const Uint8_t * const buffer,
const Uint32_t length
)
{
  d_Status_t status = d_STATUS_FAILURE;

  /* Extract UDP packet components from message */
  const d_ETH_UdpSmiPacket_t * pPacket;

  // cppcheck-suppress misra-c2012-11.3; The ARM processor does not have any alignment restrictions for integers.
  pPacket = (const d_ETH_UdpSmiPacket_t *)&buffer[0];

  Uint32_t destinationAddress = pPacket->ipAddress;
  Uint32_t destinationPort = pPacket->destinationPort;
  Uint32_t payloadLength = pPacket->payloadLength;

  /* Check message is the correct length */
  if (length == (payloadLength + 16u))
  {
    status = d_ETH_UdpSend(destinationAddress, destinationPort, pPacket->payload, payloadLength);
  }
  
  return status;
}

