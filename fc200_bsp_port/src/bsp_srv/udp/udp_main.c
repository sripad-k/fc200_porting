/****************************************************
 *  udp_main.c
 *  Created on: 03-Nov-2025 14:42:36
 *  Implementation of the Class udp_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "udp_main.h"
#include "sru/flash_mac/d_flash_mac.h"
#include "sru/qspiFlash/d_qspiFlash.h"
#include "sru/ethernet/d_eth_interface.h"
#include "xparameters.h"
#include "uart_interface.h"

/* IPV4 address type */
typedef uint32_t Ipv4Addr_t;

typedef struct
{
    Ipv4Addr_t remoteIP; // Ip address of the device we want to talk too. Hard coded in most cases.
    uint32_t rxPortNum;  // Port to listen to
    uint32_t txPortNum;  // Port used for response

    // Message debug information
    uint32_t msgInCount;           // Keeps track of messages received (ReceiveCallBack triggers)
    uint32_t msgOutCount;          // Keeps track of messages transmitted
    uint32_t msgNotProcessedCount; // Keeps track of error messages.  (Incorrect IP, port, size, etc)
} EthInterfaceListenPortDef_t;

typedef enum
{
    // LP_ = Listen Port
    LP_ITB_IOCA = 0, // External interface via switch on IOCA
    LP_ITB_IOCB,     // External interface via switch on IOCB
    LP_FCUPRIMARY,   // Primary interface FCU to FCU comms
    LP_FCUBACKUP,    // Backup interface FCU to FCU comms
    LP_FCU_VIA_IOCA, // Internal FCU to FCU comms via the switch on the IOCA
    LP_FCU_VIA_IOCB, // Internal FCU to FCU comms via the switch on the IOCB
    LP_SATACONTROL,  // External interface for SATA control application  (recording functionality)
    LP_COUNT
} ListenPortList_t;

EthInterfaceListenPortDef_t ListenPortArray[LP_COUNT];

static void DisplayMAC(uint8_t mac_ethernet_address[6]);
static void ConvertUint8ToChar(const uint8_t value, char *str);
static d_Status_t mem_initialise(void);
static d_Status_t eth_initialise(void);
static void ITB_ReceiveCallback_via_IOCA(const Ipv4Addr_t sourceAddress,
                                         const Uint16_t sourcePort,
                                         const Uint16_t destinationPort,
                                         const Uint8_t *const pbuffer,
                                         const Uint32_t length);

static void ITB_ReceiveCallback_via_IOCB(const Ipv4Addr_t sourceAddress,
                                         const Uint16_t sourcePort,
                                         const Uint16_t destinationPort,
                                         const Uint8_t *const pbuffer,
                                         const Uint32_t length);

static void FcuToFcu_ReceiveCallback(const Ipv4Addr_t sourceAddress,
                                     const Uint16_t sourcePort,
                                     const Uint16_t destinationPort,
                                     const Uint8_t *const pbuffer,
                                     const Uint32_t length);

void udp_setup_server(void)
{
    eth_initialise();
}

void udp_send_gcs(const uint8_t *buffer, uint32_t len)
{
}

void udp_send_rpi(const uint8_t *buffer, uint32_t len)
{
}

void udp_send_pil(const uint8_t *buffer, uint32_t len)
{
}

void udp_receive(uint8_t *buffer, uint32_t len, udp_source_t udp_source)
{
}

static d_Status_t mem_initialise(void)
{
    d_Status_t returnValue = d_STATUS_SUCCESS;

    // MAC FLASH
    returnValue = d_FLASH_MAC_Initialise();

    if (returnValue == d_STATUS_SUCCESS)
    {
        d_FLASH_MAC_Unlock(0); // NOTE this opens up the flash for writing??, could possibly over write the HW config information
        d_FLASH_MAC_Unlock(1); // NOTE this opens up the flash for writing, could possibly over write the SW config information
    }

    // QSPI FLASH
    if (returnValue == d_STATUS_SUCCESS)
    {
        returnValue = d_QSPI_Initialise();
    }

    return returnValue;
}

static d_Status_t eth_initialise(void)
{
    // Startup info
    uint8_t numIF = 0;
    uint8_t numPort = 0;
    uint8_t mac_ethernet_address[6];
    uint32_t mySlotNum = d_FCU_SlotNumber();
    const char ioc_a_success[80] = "\n\rITB Interface setup (via IOC A) done.\n\r";
    const char ioc_b_success[80] = "\n\rITB Interface setup (via IOC B) done.\n\r";
    const char fcu_internal_success[80] = "\n\rFCU to FCU Internal Interface setup done.\n\r";
    const char primary_internal_success[80] = "\n\rFCU to FCU Primary Internal Interface setup done.\n\r";
    const char backup_internal_success[80] = "\n\rFCU to FCU Backup Internal Interface setup done.\n\r";

    // Initialise the driver modules used.
    d_Status_t returnValue = d_STATUS_SUCCESS;

    returnValue = mem_initialise();

    if (returnValue == d_STATUS_SUCCESS)
    {
        returnValue = d_ETH_Initialise();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setup the Ethernet end point for the external interface via IOCA      192.168.<86>.x
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (returnValue == d_STATUS_SUCCESS)
    {
        // After initialisation the MAC and IP addresses of the local end points are not used further.
        // Thus there is no need to make an array of interface information.
        returnValue = d_FLASH_MAC_MacAddress(0, &mac_ethernet_address[0]);

        // My Static IP for talking to the ITB
        d_ETH_EndPoint_t fcuEndpoint;
        fcuEndpoint.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 86u, 50u + mySlotNum);
        fcuEndpoint.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuEndpoint.gateway = d_ETH_Ipv4Addr(192u, 168u, 86u, 1u);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuEndpoint, XPAR_PSU_ETHERNET_2_BASEADDR, NULL); // //XPAR_PSU_ETHERNET_3_BASEADDR
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    // Setup a UDP listening ports to receive data from the end points
    // Note. Unlike the MC code the FC uses different callback functions for the different ports.
    // This ensures that the ITB IpAddress can be dynamically assigned instead of hardcoded.
    if (returnValue == d_STATUS_SUCCESS)
    {
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_a_success, sizeof(ioc_a_success));
        DisplayMAC(&mac_ethernet_address[0]);

        ListenPortArray[LP_ITB_IOCA].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 86u, 203u); // Default ITB address             = 0   // NOTE. This interface cannot transmit until it received a message from the ITB.
        ListenPortArray[LP_ITB_IOCA].rxPortNum = 19540 + mySlotNum;                    // Port to listen to
        ListenPortArray[LP_ITB_IOCA].txPortNum = 19540 + mySlotNum;                    // Port used for response
        // Message debug information
        ListenPortArray[LP_ITB_IOCA].msgInCount = 0;
        ListenPortArray[LP_ITB_IOCA].msgOutCount = 0;
        ListenPortArray[LP_ITB_IOCA].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_ITB_IOCA].rxPortNum, ITB_ReceiveCallback_via_IOCA);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    // Setup a UDP listening port for FCU to FCU comms via the IOC
    if (returnValue == d_STATUS_SUCCESS)
    {
        ListenPortArray[LP_FCU_VIA_IOCA].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 86u, 51u - mySlotNum);
        ListenPortArray[LP_FCU_VIA_IOCA].rxPortNum = 1000; // Port to listen to
        ListenPortArray[LP_FCU_VIA_IOCA].txPortNum = 1000; // Port used for response
        // Message debug information
        ListenPortArray[LP_FCU_VIA_IOCA].msgInCount = 0;
        ListenPortArray[LP_FCU_VIA_IOCA].msgOutCount = 0;
        ListenPortArray[LP_FCU_VIA_IOCA].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_FCU_VIA_IOCA].rxPortNum, FcuToFcu_ReceiveCallback);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setup the Ethernet end point for the external interface via IOCB           192.168.<87>.x
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (returnValue == d_STATUS_SUCCESS)
    {
        returnValue = d_FLASH_MAC_MacAddress(1, &mac_ethernet_address[0]);

        // After initialisation the MAC and IP addresses of the local end points are not used further.
        // Thus there is no need to make an array of interface information.

        // My Static IP for talking to the ITB
        d_ETH_EndPoint_t fcuEndpoint;
        fcuEndpoint.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 87u, 50u + mySlotNum);
        fcuEndpoint.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuEndpoint.gateway = d_ETH_Ipv4Addr(192u, 168u, 87u, 1u);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuEndpoint, XPAR_PSU_ETHERNET_1_BASEADDR, NULL);
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    // Setup a UDP listening ports to receive data from the end points
    // Note. Unlike the MC code the FC uses different callback functions for the different ports.
    // This ensures that the ITB IpAddress can be dynamically assigned instead of hardcoded.
    if (returnValue == d_STATUS_SUCCESS)
    {
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_b_success, sizeof(ioc_b_success));
        DisplayMAC(&mac_ethernet_address[0]);

        ListenPortArray[LP_ITB_IOCB].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 87u, 203u); // Default ITB address             = 0   // NOTE. This interface cannot transmit until it received a message from the ITB.
        ListenPortArray[LP_ITB_IOCB].rxPortNum = 19550 + mySlotNum;                    // Port to listen to
        ListenPortArray[LP_ITB_IOCB].txPortNum = 19550 + mySlotNum;                    // Port used for response
        // Message debug information
        ListenPortArray[LP_ITB_IOCB].msgInCount = 0;
        ListenPortArray[LP_ITB_IOCB].msgOutCount = 0;
        ListenPortArray[LP_ITB_IOCB].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_ITB_IOCB].rxPortNum, ITB_ReceiveCallback_via_IOCB);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    if (returnValue == d_STATUS_SUCCESS)
    {
        ListenPortArray[LP_FCU_VIA_IOCB].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 87u, 51u - mySlotNum);
        ListenPortArray[LP_FCU_VIA_IOCB].rxPortNum = 3000; // Port to listen to
        ListenPortArray[LP_FCU_VIA_IOCB].txPortNum = 3000; // Port used for response
        // Message debug information
        ListenPortArray[LP_FCU_VIA_IOCB].msgInCount = 0;
        ListenPortArray[LP_FCU_VIA_IOCB].msgOutCount = 0;
        ListenPortArray[LP_FCU_VIA_IOCB].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_FCU_VIA_IOCB].rxPortNum, FcuToFcu_ReceiveCallback);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    // In the FCRP setup (and in design) Internal FCU ethernet should have been connected 0-1 and 1-0.
    // But due to FCU wiring they were actually connected 0-0 and 1-1.
    // This leads to a much simpler configuration.

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setup the Ethernet end point for the internal FCU to FCU interface     192.168.<89>.x
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (returnValue == d_STATUS_SUCCESS)
    {
        returnValue = d_FLASH_MAC_MacAddress(0, &mac_ethernet_address[0]); // Re-use MAC[0], on the internal interfaces, no external visibility.

        // After initialisation the MAC and IP addresses of the local end points are not used further.
        // Thus there is no need to make an array of interface information.

        d_ETH_EndPoint_t fcuToFcuInternal;
        fcuToFcuInternal.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 89, 50u + mySlotNum);
        fcuToFcuInternal.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuToFcuInternal.gateway = d_ETH_Ipv4Addr(192u, 168u, 89u, 51u - mySlotNum);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuToFcuInternal, XPAR_PSU_ETHERNET_0_BASEADDR, NULL);
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    if (returnValue == d_STATUS_SUCCESS)
    {
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)primary_internal_success, sizeof(primary_internal_success));
        DisplayMAC(&mac_ethernet_address[0]);

        ListenPortArray[LP_FCUPRIMARY].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 89u, 51u - mySlotNum);
        ListenPortArray[LP_FCUPRIMARY].rxPortNum = 4000; // Port to listen to
        ListenPortArray[LP_FCUPRIMARY].txPortNum = 4000; // Port used for response
        // Message debug information
        ListenPortArray[LP_FCUPRIMARY].msgInCount = 0;
        ListenPortArray[LP_FCUPRIMARY].msgOutCount = 0;
        ListenPortArray[LP_FCUPRIMARY].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_FCUPRIMARY].rxPortNum, FcuToFcu_ReceiveCallback);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Setup the Ethernet end point for the internal FCU to FCU interface     192.168.<90>.x
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (returnValue == d_STATUS_SUCCESS)
    {
        returnValue = d_FLASH_MAC_MacAddress(1, &mac_ethernet_address[0]); // Re-use MAC[1], on the internal interfaces, no external visibility.

        // After initialisation the MAC and IP addresses of the local end points are not used further.
        // Thus there is no need to make an array of interface information.

        d_ETH_EndPoint_t fcuToFcuInternal;
        fcuToFcuInternal.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 90, 50u + mySlotNum);
        fcuToFcuInternal.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuToFcuInternal.gateway = d_ETH_Ipv4Addr(192u, 168u, 90u, 51u - mySlotNum);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuToFcuInternal, XPAR_PSU_ETHERNET_3_BASEADDR, NULL); // //XPAR_PSU_ETHERNET_2_BASEADDR
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    if (returnValue == d_STATUS_SUCCESS)
    {
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)backup_internal_success, sizeof(backup_internal_success));
        DisplayMAC(&mac_ethernet_address[0]);

        ListenPortArray[LP_FCUBACKUP].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 90u, 51u - mySlotNum);
        ListenPortArray[LP_FCUBACKUP].rxPortNum = 5000; // Port to listen to
        ListenPortArray[LP_FCUBACKUP].txPortNum = 5000; // Port used for response
        // Message debug information
        ListenPortArray[LP_FCUBACKUP].msgInCount = 0;
        ListenPortArray[LP_FCUBACKUP].msgOutCount = 0;
        ListenPortArray[LP_FCUBACKUP].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[LP_FCUBACKUP].rxPortNum, FcuToFcu_ReceiveCallback);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    char uart_msg[100];
    char numIF_str[4];
    char numPort_str[4];
    int index = 0;

    // Convert numbers to strings
    ConvertUint8ToChar(numIF, numIF_str);
    ConvertUint8ToChar(numPort, numPort_str);

    // First line: Number of ethernet interfaces
    const char *line1_prefix = "Number of ethernet interfaces = ";
    for (int i = 0; line1_prefix[i] != '\0'; i++)
    {
        uart_msg[index++] = line1_prefix[i];
    }
    for (int i = 0; i < 3; i++)
    {
        uart_msg[index++] = numIF_str[i];
    }
    uart_msg[index++] = '\n';
    uart_msg[index++] = '\r';

    // Second line: Number of Ports Opened
    const char *line2_prefix = "Number of Ports Opened = ";
    for (int i = 0; line2_prefix[i] != '\0'; i++)
    {
        uart_msg[index++] = line2_prefix[i];
    }
    for (int i = 0; i < 3; i++)
    {
        uart_msg[index++] = numPort_str[i];
    }
    uart_msg[index++] = '\n';
    uart_msg[index++] = '\r';

    uart_msg[index] = '\0';
    uart_write(UART_DEBUG_CONSOLE, (uint8_t *)uart_msg, index);

    return returnValue;
}

static void /** \return None */
ITB_ReceiveCallback_via_IOCA(
    const Ipv4Addr_t sourceAddress, /**< [in] source IP address */
    const Uint16_t sourcePort,      /**< [in] source port number */
    const Uint16_t destinationPort, /**< [in] destination port number */
    const Uint8_t *const pbuffer,   /**< [in] packet buffer */
    const Uint32_t length           /**< [in] data length */
)
{
    Bool_t msgProcessed = d_FALSE;

    ListenPortArray[LP_ITB_IOCA].msgInCount++;

    // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
    if (destinationPort == ListenPortArray[LP_ITB_IOCA].rxPortNum)
    {
        ListenPortArray[LP_ITB_IOCA].remoteIP = sourceAddress; // We store the itb's source address, and then respond to the same address.

        msgProcessed = d_TRUE;
    }

    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[LP_ITB_IOCA].msgNotProcessedCount++;
    }

    return;
}

static void /** \return None */
ITB_ReceiveCallback_via_IOCB(
    const Ipv4Addr_t sourceAddress, /**< [in] source IP address */
    const Uint16_t sourcePort,      /**< [in] source port number */
    const Uint16_t destinationPort, /**< [in] destination port number */
    const Uint8_t *const pbuffer,   /**< [in] packet buffer */
    const Uint32_t length           /**< [in] data length */
)
{
    Bool_t msgProcessed = d_FALSE;

    ListenPortArray[LP_ITB_IOCB].msgInCount++;

    // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
    if (destinationPort == ListenPortArray[LP_ITB_IOCB].rxPortNum)
    {
        ListenPortArray[LP_ITB_IOCB].remoteIP = sourceAddress; // We store the itb's source address, and then respond to the same address.

        msgProcessed = d_TRUE;
    }

    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[LP_ITB_IOCB].msgNotProcessedCount++;
    }

    return;
}

static void /** \return None */
FcuToFcu_ReceiveCallback(
    const Ipv4Addr_t sourceAddress, /**< [in] source IP address */
    const Uint16_t sourcePort,      /**< [in] source port number */
    const Uint16_t destinationPort, /**< [in] destination port number */
    const Uint8_t *const pbuffer,   /**< [in] packet buffer */
    const Uint32_t length           /**< [in] data length */
)
{

    // TODO.  Rewrite this as a sub function taking in the index

    for (Uint8_t idx = 0; idx < LP_COUNT; idx++)
    {

        if (destinationPort == ListenPortArray[idx].rxPortNum)
        {
            ListenPortArray[idx].msgInCount++;

            switch (idx)
            {
            case LP_FCUPRIMARY:
                // GLOBAL_SerialPrint("<<    FCU to FCU - Internal Primary   - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case LP_FCUBACKUP:
                // GLOBAL_SerialPrint("<<    FCU to FCU - Internal Secondary - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case LP_FCU_VIA_IOCA:
                // GLOBAL_SerialPrint("<<    FCU to FCU - External IOCA      - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case LP_FCU_VIA_IOCB:
                // GLOBAL_SerialPrint("<<    FCU to FCU - External IOCB      - [Tx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            default:
                break;
            }

            Bool_t msgProcessed = d_FALSE;

            // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
            if (sourceAddress == ListenPortArray[idx].remoteIP)
            {
                msgProcessed = d_TRUE;
            }

            if (msgProcessed == d_FALSE)
            {
                ListenPortArray[idx].msgNotProcessedCount++;
            }
        }
    }
    return;
}

static void DisplayMAC(uint8_t mac_ethernet_address[6])
{
    char result[35]; // 4 spaces + "MAC: " + 6 values * 3 characters per value + 5 colons + \n +\r + 1 for null terminator
    char str[4];     // 3 characters + 1 for null terminator
    int index = 0;

    result[index++] = ' '; // Leading Space
    result[index++] = ' '; // Leading Space
    result[index++] = ' '; // Leading Space
    result[index++] = ' '; // Leading Space

    result[index++] = 'M'; // Leading Space
    result[index++] = 'A'; // Leading Space
    result[index++] = 'C'; // Leading Space
    result[index++] = ':'; // Leading Space
    result[index++] = ' '; // Leading Space

    for (int i = 0; i < 6; i++)
    {
        ConvertUint8ToChar(mac_ethernet_address[i], str);
        for (int j = 0; j < 3; j++)
        {
            result[index++] = str[j];
        }
        if (i < 5)
        {
            result[index++] = ':';
        }
    }

    result[index++] = '\n'; // Newline
    result[index++] = '\r'; // Return
    result[index++] = '\0'; // Null-terminate the string

    uart_write(UART_DEBUG_CONSOLE, (uint8_t *)result, sizeof(result));
}

static void ConvertUint8ToChar(
    const uint8_t value,
    char *str)
{
    str[0] = '0' + (value / 100);       // Extract hundreds digit
    str[1] = '0' + ((value / 10) % 10); // Extract tens digit
    str[2] = '0' + (value % 10);        // Extract units digit
}