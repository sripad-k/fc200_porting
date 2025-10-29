/*********************************************************************//**
\file
\brief
  Module Title       : Ethernet Interface Module

  Abstract           : Wrapper for the LWIP network stack library.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-109
                                                                SWREQ-110
                       SDD References: 136T-2200-131000-001-D22 SWDES-551
                                                                SWDES-675
                                                                SWDES-676
                                                                SWDES-677
                                                                SWDES-678
                                                                SWDES-679
                                                                SWDES-680
                                                                SWDES-681

\note
  CSC ID             : SWDES-67
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_memory.h"
#include "xparameters.h"
#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_memory.h"

#include "xemacps.h"
#include "netif/xadapter.h"
#include "lwip/init.h"
#include "lwip/udp.h"

#include "d_eth_interface.h"

/***** Constants ********************************************************/

/* Convert LWIP error code to driver function status value */
#define ERROR_OFFSET  100u

#define MAX_PORT_NUMBER    65535u

#define MAX_INTERFACES     4u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static Uint32_t InterfaceCount = 0;

/* Flag to indicate when the ethernet interface has been successfully initialised */
static Bool_t Initialised;

/* Network interface */
static struct netif server_netif[MAX_INTERFACES];

/* Protocol control block for sending UDP packets */
static struct udp_pcb *send_pcb[MAX_INTERFACES];

static Uint32_t ReceiveCount[MAX_INTERFACES];

/***** Function Declarations ********************************************/

/* Receive callback function prototype as defined by the LWIP library */
static void recv_callback
(
void *arg,                                   /**< Application callback function pointer */
struct udp_pcb  *protocolControlBlock,       /**< Protocol control block */
struct pbuf     *packetBuffer,               /**< Message buffer */
const ip_addr_t *sourceIpAddr,               /**< Source IP address */
u16_t           port                         /**< Source port */
);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_ETH_Ipv4Addr -->

  Generate IPV4 address in format required by driver.
*************************************************************************/
Uint32_t                 /** \return IP address */
d_ETH_Ipv4Addr
(
Uint8_t byte0,           /**< MS byte of IP address */
Uint8_t byte1,           /**< Next byte of IP address */
Uint8_t byte2,           /**< Next byte of IP address */
Uint8_t byte3            /**< LS byte of IP address */
)
{
  Uint32_t addr;

  addr = (((Uint32_t)byte3) << 24u) + (((Uint32_t)byte2) << 16u) + (((Uint32_t)byte1) << 8u) + ((Uint32_t)byte0);

  return addr;
}

/*********************************************************************//**
  <!-- d_ETH_Initialise -->

  Initialise the ethernet interface.
*************************************************************************/
d_Status_t                                 /** \return Initialisation success / failure */
d_ETH_Initialise
(
void       /**< [in] None */
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  Initialised = d_FALSE;
  lwip_init();
  InterfaceCount = 0;

  Initialised = d_TRUE;

  netif_set_default(&server_netif[0]);

  for (Uint32_t index = 0; index < MAX_INTERFACES; index++)
  {
    ReceiveCount[index] = 0;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_ETH_InterfaceAdd -->

  Add an ethernet interface.
*************************************************************************/
d_Status_t                                 /** \return Initialisation success / failure */
d_ETH_InterfaceAdd
(
const d_MacAddress_t macAddress,       /**< [in] MAC address */
const d_ETH_EndPoint_t * const endpoint,   /**< [in] IP address, netmask and gateway */
const Uint32_t baseAddress,                /**< [in] GEM base address */
Uint32_t * pInterfaceID)                   /**< [out] pointer to return the interface ID for the interface, can be NULL */  
{
  d_Status_t returnValue;
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gateway;

  returnValue = d_STATUS_SUCCESS;

  if (macAddress == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (endpoint == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (baseAddress == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Network interface check pointer */
  struct netif * server_netif_ret;

  ipaddr.addr = endpoint->ipaddr;
  netmask.addr = endpoint->netmask;
  gateway.addr = endpoint->gateway;

#pragma GCC diagnostic push
/* The following warning is ignored. It is due to the LWIP library not const specifying some pointer members of the 'netif' structure. */
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
  // cppcheck-suppress misra-c2012-11.8; The LWIP library is not const specifying some pointer members of the 'netif' structure.
  server_netif_ret = xemac_add((struct netif * const)&server_netif[InterfaceCount], &ipaddr, &netmask, &gateway, macAddress, baseAddress);
#pragma GCC diagnostic pop
  if (server_netif_ret != &server_netif[InterfaceCount])
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    returnValue = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Create socket for UDP sending */
    send_pcb[InterfaceCount] = udp_new();
    if (send_pcb[InterfaceCount] == NULL)
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      returnValue = d_STATUS_FAILURE;
    }
    else
    {
      /* specify that the network if is up */
      netif_set_up(&server_netif[InterfaceCount]);

      if (pInterfaceID != NULL)
      {
        *pInterfaceID = InterfaceCount;
      }
      ELSE_DO_NOTHING

      InterfaceCount++;
    }

  }
  else
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_ETH_GetIpMac -->

  Get the MAC address and IP address of an interface.
*************************************************************************/
d_Status_t                                 /** \return Success / failure */
d_ETH_GetIpMac
(
const Uint32_t interface,                  /**< [in] Network to interrogate */
Uint8_t * const pMac,                      /**< [out] Pointer to storage for MAC address */
d_ETH_Ipv4Addr_t * const pIp,              /**< [out] Pointer to storage for IP address */
d_ETH_Ipv4Addr_t * const pMask,            /**< [out] Pointer to storage for IP address */
d_ETH_Ipv4Addr_t * const pGateway          /**< [out] Pointer to storage for IP address */
)
{
  if (interface >= InterfaceCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, interface, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_GEN_MemoryCopy(pMac, &server_netif[interface].hwaddr[0], d_MAC_ADDRESS_LENGTH);
  *pIp = (Uint32_t)server_netif[interface].ip_addr.addr;
  *pMask = server_netif[interface].netmask.addr;
  *pGateway = server_netif[interface].gw.addr;

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_ETH_UdpSend -->

  Send a UDP packet to a specific port at a specific IP address.
*************************************************************************/
d_Status_t                                 /** \return Success / failure */
d_ETH_UdpSend
(
const Uint32_t destinationAddress,         /**< [in] Destination IP address */
const Uint32_t destinationPort,            /**< [in] Destination port */
const Uint8_t * const message,             /**< [in] Message to send */
const Uint32_t length)                     /**< [in] Message length in bytes */
{
  d_Status_t returnValue;
  ip_addr_t addr;

  returnValue = d_STATUS_SUCCESS;

  if (destinationAddress == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (destinationPort > MAX_PORT_NUMBER)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, destinationPort, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (message == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (length > d_ETH_MAX_UDP_PACKET_DATA)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, length, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  struct pbuf * p = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_RAM);
  if (p != NULL)
  {
    d_GEN_MemoryCopy(p->payload, message, length);
    p->len = length;
    p->tot_len = length;
    addr.addr = destinationAddress;
    struct udp_pcb * pcb = udp_new();
    err_enum_t lwip_return = (err_enum_t)udp_sendto(pcb, p, &addr, destinationPort);
    udp_remove(pcb); // clean up the pcb after it was used
    if (lwip_return != ERR_OK)
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      returnValue = (Uint32_t)lwip_return - ERROR_OFFSET;
    }
    else
    {
      DO_NOTHING();
    }
    (void)pbuf_free(p);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    returnValue = d_STATUS_INSUFFICIENT_MEMORY;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_ETH_UdpSendIf -->

  Send a UDP packet to a specific port at a specific IP address on a
  specific interface.
*************************************************************************/
d_Status_t                                 /** \return Success / failure */
d_ETH_UdpSendIf
(
const Uint32_t destinationAddress,         /**< [in] Destination IP address */
const Uint32_t destinationPort,            /**< [in] Destination port */
const Uint8_t * const message,             /**< [in] Message to send */
const Uint32_t length,                     /**< [in] Message length in bytes */
const Uint32_t interface)                  /**< [in] Network to send on */
{
  d_Status_t returnValue;
  ip_addr_t addr;

  returnValue = d_STATUS_SUCCESS;

  if (destinationAddress == 0u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (destinationPort > MAX_PORT_NUMBER)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, destinationPort, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (message == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (length > d_ETH_MAX_UDP_PACKET_DATA)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 4, length, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (interface >= InterfaceCount)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 5, interface, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  struct pbuf * p = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_RAM);
  if (p != NULL)
  {
    d_GEN_MemoryCopy(p->payload, message, length);
    p->len = length;
    p->tot_len = length;
    addr.addr = destinationAddress;
    err_enum_t lwip_return = (err_enum_t)udp_sendto_if(send_pcb[interface], p, &addr, destinationPort, &server_netif[interface]);
    if (lwip_return != ERR_OK)
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      returnValue = (Uint32_t)lwip_return - ERROR_OFFSET;
    }
    else
    {
      DO_NOTHING();
    }
    (void)pbuf_free(p);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    returnValue = d_STATUS_INSUFFICIENT_MEMORY;
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_ETH_UdpListen -->

  Create a UDP listening socket on a specific port.
*************************************************************************/
d_Status_t                               /** \return Success / failure */
d_ETH_UdpListen
(
const Uint32_t localPort,                /**< [in] Listening port */
d_ETH_UdpReceiveFunc_t receiveCallback   /**< [in] Callback function pointer */
)
{
  d_Status_t returnValue;
  struct udp_pcb *pcb;
  err_enum_t err;

  returnValue = d_STATUS_SUCCESS;

  if (localPort > MAX_PORT_NUMBER)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, localPort, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (receiveCallback == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* create new UDP PCB structure */
  pcb = udp_new();
  if (pcb == NULL)
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    returnValue = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* bind to specified port */
    err = (err_enum_t)udp_bind(pcb, IP_ANY_TYPE, localPort);
    if (err != ERR_OK)
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      returnValue = (Uint32_t)err - ERROR_OFFSET;
    }
    else
    {
      DO_NOTHING();
    }
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    /* Store the application callback function pointer in the 'arg' parameter */
    udp_recv(pcb, recv_callback, receiveCallback);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    DO_NOTHING();
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- d_ETH_TickFast -->

  Tick function to process received packets. Execute at approx 10ms intervals.
*************************************************************************/
void              /** \return None */
d_ETH_TickFast
(
void
)
{
  /* receive and process packets */
  if (Initialised == d_TRUE)
  {
    Uint32_t index;
    for (index = 0; index < InterfaceCount; index++)
    {
      /* Process up to 10 packets on each interface */
      Uint32_t count = 0;
      Uint32_t processed;
      do
      {
        /* The function xemacif_input returns the number of packets processed, but only ever attempts to process one */
        processed = xemacif_input(&server_netif[index]);
        ReceiveCount[index] += processed;
        count++;
      } while ((processed > 0u) && (count < 10u));
    }
  }

  return;
}

/*********************************************************************//**
  <!-- recv_callback -->

  Callback function executed on receipt of UDP packet directed to selected port.
*************************************************************************/
static void                              /** \return None */
recv_callback
(
void *arg,                               /**< [in] Application callback function pointer */
struct udp_pcb  *protocolControlBlock,   /**< [in] Protocol control block */
struct pbuf     *packetBuffer,           /**< [in] Message buffer */
const ip_addr_t *sourceIpAddr,           /**< [in] Source IP address */
u16_t           port                     /**< [in] Source port */
)
{
  /* do not read the packet if we are not in ESTABLISHED state */
  if (packetBuffer != NULL)
  {
    /* Application callback function pointer stored in 'arg' */
    if (arg != NULL)
    {
      // cppcheck-suppress misra-c2012-11.5; The pointer to void definition is from a Xilinx generated header file. It is intended to be cast to something else.
      ((d_ETH_UdpReceiveFunc_t)arg)(sourceIpAddr->addr, port, protocolControlBlock->local_port, (Uint8_t *)packetBuffer->payload, packetBuffer->len);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this error during bench testing.
      DO_NOTHING();
    }

    /* free the received pbuf */
    (void)pbuf_free(packetBuffer);
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this error during bench testing.
    DO_NOTHING();
  }

  return;
}
