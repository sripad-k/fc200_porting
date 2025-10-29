/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Ethernet Interface Header File

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

#ifndef D_ETH_INTERFACE_H
#define D_ETH_INTERFACE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals *********************************************************/

/* Maximum data in UDP packet, based on XEMACPS_MTU(1500) - ETH_HLEN(14) - PBUF_IP_HLEN(20) - UDP_HLEN(8) */
#define d_ETH_MAX_UDP_PACKET_DATA  1458u

/* Error return codes */
enum
{
  d_ETH_ERR_MEM        = -101,    /* Out of memory error.     */
  d_ETH_ERR_BUF        = -102,    /* Buffer error.            */
  d_ETH_ERR_TIMEOUT    = -103,    /* Timeout.                 */
  d_ETH_ERR_RTE        = -104,    /* Routing problem.         */
  d_ETH_ERR_INPROGRESS = -105,    /* Operation in progress    */
  d_ETH_ERR_VAL        = -106,    /* Illegal value.           */
  d_ETH_ERR_WOULDBLOCK = -107,    /* Operation would block.   */
  d_ETH_ERR_USE        = -108,    /* Address in use.          */
  d_ETH_ERR_ALREADY    = -109,    /* Already connecting.      */
  d_ETH_ERR_ISCONN     = -110,    /* Conn already established.*/
  d_ETH_ERR_CONN       = -111,    /* Not connected.           */
  d_ETH_ERR_IF         = -112,    /* Low-level netif error    */
  d_ETH_ERR_ABRT       = -113,    /* Connection aborted.      */
  d_ETH_ERR_RST        = -114,    /* Connection reset.        */
  d_ETH_ERR_CLSD       = -115,    /* Connection closed.       */
  d_ETH_ERR_ARG        = -116     /* Illegal argument.        */
};

/* Standard ports to be used */
enum
{
  d_ETH_PORT_HOST = 19540,
  d_ETH_PORT_PROCESS_MONITOR,
  d_ETH_PORT_COVERAGE
};

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* IPV4 address type */
typedef Uint32_t d_ETH_Ipv4Addr_t;

/* UDP packet receive callback function prototype definition */
typedef void (*d_ETH_UdpReceiveFunc_t)(const d_ETH_Ipv4Addr_t souceAddress,  /** sourceAddress */
                                       const Uint16_t sourcePort,            /** source port */
                                       const Uint16_t destinationPort,       /** destination port */
                                       const Uint8_t * const buffer,         /** message buffer */
                                       const Uint32_t length);               /** message length */

/* Endpoint definition for initialisation of the interface */
typedef struct
{
  d_ETH_Ipv4Addr_t ipaddr;    /** Local IP address */
  d_ETH_Ipv4Addr_t netmask;   /** Net mask */
  d_ETH_Ipv4Addr_t gateway;   /** Gateway */
} d_ETH_EndPoint_t;

/* Packet structure used to store outgoing packets in a queue */
typedef struct
{
	Uint32_t ipaddress;
	Uint32_t port;
	Uint8_t payload[d_ETH_MAX_UDP_PACKET_DATA];
	Uint32_t length;
	Uint32_t interface;
} d_ETH_UdpPacket_t;


/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Define an IP address using the conventional format of 192.168.0.1 etc. */
Uint32_t d_ETH_Ipv4Addr(Uint8_t byte0,                                  /**< [in] MS byte of IP address */
                        Uint8_t byte1,                                  /**< [in] Next byte of IP address */
                        Uint8_t byte2,                                  /**< [in] Next byte of IP address */
                        Uint8_t byte3);                                 /**< [in] LS byte of IP address */

/* Initialise the ethernet interface. */
d_Status_t d_ETH_Initialise(void);                                      /**< [in] None */

/* Add an ethernet interface */
d_Status_t d_ETH_InterfaceAdd(const d_MacAddress_t macAddress,          /**< [in] MAC address */
                              const d_ETH_EndPoint_t * const endpoint,  /**< [in] IP address, netmask and gateway */
                              const Uint32_t baseAddress,               /**< [in] GEM base address */
                              Uint32_t * pInterfaceID);                 /**< [out] pointer to return the interface ID for the interface, can be NULL */  

d_Status_t d_ETH_GetIpMac(const Uint32_t interface,
                          Uint8_t * const pMac,
                          d_ETH_Ipv4Addr_t * const pIp,
                          d_ETH_Ipv4Addr_t * const pMask,
                          d_ETH_Ipv4Addr_t * const pGateway);

/* Send a UDP packet to a specific port at a specific IP address. */
d_Status_t d_ETH_UdpSend(const Uint32_t destinationAddress,             /**< [in] Destination IP address */
                         const Uint32_t destinationPort,                /**< [in] Destination port */
                         const Uint8_t * const message,                 /**< [in] Message to send */
                         const Uint32_t length);                        /**< [in] Message length in bytes */

/* Send a UDP packet to a specific port at a specific IP address on a specific interface */
d_Status_t d_ETH_UdpSendIf(const Uint32_t destinationAddress,          /**< [in] Destination IP address */
                            const Uint32_t destinationPort,             /**< [in] Destination port */
                            const Uint8_t * const message,              /**< [in] Message to send */
                            const Uint32_t length,                      /**< [in] Message length in bytes */
                            const Uint32_t interface);                  /**< [in] Network to send on */

/* Create a UDP listening socket on a specific port */
d_Status_t d_ETH_UdpListen(const Uint32_t localPort,                    /**< [in] Listening port */
                           d_ETH_UdpReceiveFunc_t receiveCallback);     /**< [in] Callback function pointer */

/* Tick function to process received packets */
void d_ETH_TickFast(void);

#endif /* D_ETH_INTERFACE_H */
