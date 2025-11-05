/****************************************************
 *  udp_main.c
 *  Created on: 03-Nov-2025 14:42:36
 *  Implementation of the Class udp_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "udp_main.h"
#include "uart_interface.h"
#include "sru/flash_mac/d_flash_mac.h"
#include "sru/qspiFlash/d_qspiFlash.h"
#include "sru/ethernet/d_eth_interface.h"
#include "xparameters.h"
#include "sru/fcu/d_fcu.h"

#define BUFF_SIZE (2048)
#define MAX_TX_BUFF_SIZE (300)

typedef struct 
{
    uint8_t octet1;
    uint8_t octet2;
    uint8_t octet3;
    uint8_t octet4;
    uint16_t rxport;
    uint16_t txport;
} udpportconfig_t;


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
} eth_if_port_def_t;

typedef enum
{
    // LP_ = Listen Port
    DST_PORT_IOCA_GCS = 0,     // External interface via switch on IOCA
    DST_PORT_IOCA_RPI,         // External interface via switch on IOCA
    DST_PORT_IOCA_PIL,         // External interface via switch on IOCA
    DST_PORT_IOCB_GCS,         // External interface via switch on IOCB
    DST_PORT_IOCB_RPI,         // External interface via switch on IOCB
    DST_PORT_IOCB_PIL,         // External interface via switch on IOCB
    DST_PORT_FCUPRIMARY,       // Primary interface FCU to FCU comms
    DST_PORT_FCUBACKUP,        // Backup interface FCU to FCU comms
    DST_PORT_FCU_VIA_IOCA,     // Internal FCU to FCU comms via the switch on the IOCA
    DST_PORT_FCU_VIA_IOCB,     // Internal FCU to FCU comms via the switch on the IOCB
    DST_PORT_SATACONTROL,      // External interface for SATA control application  (recording functionality)
    DST_PORT_COUNT
} listenportlist_t;

const udpportconfig_t UdpPortConfig[DST_PORT_COUNT] = 
{
  /* OCT1 OCT2 OCT3 OCT4  RXPORT TXPORT */
    {192, 168,  69,   5,  14501, 14501},    /* DST_PORT_IOCA_GCS     */
    {192, 168,  69, 120,   8000,  8000},    /* DST_PORT_IOCA_RPI     */
    {192, 168,  69, 101,  14503, 14503},    /* DST_PORT_IOCA_PIL     */

    {192, 168,  69,   6,  15501, 15501},    /* DST_PORT_IOCB_GCS     */
    {192, 168,  69, 121,   8000,  8000},    /* DST_PORT_IOCB_RPI     */
    {192, 168,  69, 101,  15503, 15503},    /* DST_PORT_IOCB_PIL     */

    {192, 168,  69,  51,   4000,  4000},    /* DST_PORT_FCUPRIMARY   */ 
    {192, 168,  69,  52,   4000,  4000},    /* DST_PORT_FCUBACKUP    */
 
    {192, 168,  69,  51,   1000,  1000},    /* DST_PORT_FCU_VIA_IOCA */  
    {192, 168,  69,  54,   3000,  3000},    /* DST_PORT_FCU_VIA_IOCB */ 
 
    {192, 168,  69,  51,   2000,  2000}     /* DST_PORT_SATACONTROL  */ 
};

eth_if_port_def_t ListenPortArray[DST_PORT_COUNT];

uint8_t GcsRxDataPool[BUFF_SIZE] = {0};
uint8_t RpiRxDataPool[BUFF_SIZE] = {0};
uint8_t PilRxDataPool[BUFF_SIZE] = {0};

static void display_mac(uint8_t mac_ethernet_address[6]);
static void convert_uint8_to_char(const uint8_t value, char *str);
static d_Status_t mem_initialise(void);
static d_Status_t eth_initialise(void);
static void receivecallback_via_ioca_gcs(const Ipv4Addr_t sourceAddress,
                                             const Uint16_t sourcePort,
                                             const Uint16_t destinationPort,
                                             const Uint8_t *const pbuffer,
                                             const Uint32_t length);

static void receivecallback_via_ioca_rpi(const Ipv4Addr_t sourceAddress,
                                             const Uint16_t sourcePort,
                                             const Uint16_t destinationPort,
                                             const Uint8_t *const pbuffer,
                                             const Uint32_t length);

static void receivecallback_via_ioca_pil(const Ipv4Addr_t sourceAddress,
                                             const Uint16_t sourcePort,
                                             const Uint16_t destinationPort,
                                             const Uint8_t *const pbuffer,
                                             const Uint32_t length);

static void receivecallback_via_iocb(const Ipv4Addr_t sourceAddress,
                                         const Uint16_t sourcePort,
                                         const Uint16_t destinationPort,
                                         const Uint8_t *const pbuffer,
                                         const Uint32_t length);

static void fcu_to_fcu_receivecallback(const Ipv4Addr_t sourceAddress,
                                     const Uint16_t sourcePort,
                                     const Uint16_t destinationPort,
                                     const Uint8_t *const pbuffer,
                                     const Uint32_t length);

static bool setup_udp_listen_port(const udpportconfig_t *config, eth_if_port_def_t *portDef, d_ETH_UdpReceiveFunc_t callback);

/**
 * @brief Initializes the UDP server setup
 * 
 * This function performs the initial setup required for the UDP server by:
 * - Clearing the GCS (Ground Control Station) receive data buffer pool
 * - Clearing the PIL (Pilot) receive data buffer pool  
 * - Initializing the Ethernet interface for network communication
 * 
 * @param None
 * @return None
 * 
 * @note This function should be called once during system initialization
 *       before any UDP communication begins
 * @note Buffer size is defined by BUFF_SIZE macro
 */                                  
void udp_setup_server(void)
{
    /* Initialize the buffer data */
    for(uint32_t buf_idx = 0; buf_idx < BUFF_SIZE; buf_idx++)
    {
        GcsRxDataPool[buf_idx] = 0;
        PilRxDataPool[buf_idx] = 0;
    }

    /* Initialize the Ethernet interface */
    eth_initialise();
}

/**
 * @brief Performs periodic UDP synchronization operations
 * 
 * This function is called periodically to handle UDP synchronization tasks.
 * It triggers the fast Ethernet tick processing to maintain proper timing
 * and synchronization for UDP operations.
 * 
 * @param None
 * @return None
 * 
 * @note This function should be called at regular intervals to ensure
 *       proper UDP synchronization timing
 */
void udp_sync_periodic(void)
{
    d_ETH_TickFast();
}

/**
 * @brief Sends UDP data to Ground Control Station (GCS)
 * 
 * This function prepares and transmits UDP data to the GCS by copying the input
 * buffer to a local transmission buffer and sending it via the Ethernet UDP interface.
 * The function ensures that the data does not exceed the maximum transmission buffer size.
 * 
 * @param buffer Pointer to the data buffer to be transmitted
 * @param len Length of the data to be transmitted in bytes
 * 
 * @return None
 * 
 * @note The function will only transmit up to MAX_TX_BUFF_SIZE bytes, even if
 *       the input length is larger
 * @note Uses the GCS configuration from ListenPortArray[DST_PORT_IOCA_GCS] for
 *       remote IP and port settings
 * @note If input length exceeds buffer size, only the first MAX_TX_BUFF_SIZE 
 *       bytes are sent
 */
void udp_send_gcs(const uint8_t *buffer, uint32_t len)
{
    /* Prepare the transmission buffer */
    uint8_t txBuffer[MAX_TX_BUFF_SIZE];
    uint32_t char_idx = 0;

    /* Copy the data to the txBuffer */
    for (char_idx = 0; char_idx < len && char_idx < sizeof(txBuffer); char_idx++)
    {
        txBuffer[char_idx] = buffer[char_idx];
    }
    
    /* Send the UDP packet */
    d_Status_t txStatus = d_ETH_UdpSendIf(ListenPortArray[DST_PORT_IOCA_GCS].remoteIP,
                                          ListenPortArray[DST_PORT_IOCA_GCS].txPortNum,
                                          &txBuffer[0], char_idx, 0);

    if (txStatus != d_STATUS_SUCCESS)
    {
        /* Transmit error message on UART */
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"UDP GCS Send Error\n\r", 20);
    }
}
/**
 * @brief Sends UDP packet to Raspberry Pi (RPI) device
 * 
 * This function prepares and transmits a UDP packet to the configured RPI destination.
 * The input buffer is copied to a local transmission buffer with size validation
 * before being sent via the Ethernet UDP interface.
 * 
 * @param[in] buffer    Pointer to the data buffer to be transmitted
 * @param[in] len       Length of the data buffer in bytes
 * 
 * @return None
 * 
 * @note The function limits transmission to MAX_TX_BUFF_SIZE bytes maximum
 * @note Uses predefined RPI destination IP and port from ListenPortArray[DST_PORT_IOCA_RPI]
 * @note If input length exceeds buffer size, only the first MAX_TX_BUFF_SIZE bytes are sent
 * 
 */
void udp_send_rpi(const uint8_t *buffer, uint32_t len)
{
    /* Prepare the transmission buffer */
    uint8_t txBuffer[MAX_TX_BUFF_SIZE];
    uint32_t char_idx = 0;

    /* Copy the data to the txBuffer */
    for (char_idx = 0; char_idx < len && char_idx < sizeof(txBuffer); char_idx++)
    {
        txBuffer[char_idx] = buffer[char_idx];
    }
    
    /* Send the UDP packet */
    d_Status_t txStatus = d_ETH_UdpSendIf(ListenPortArray[DST_PORT_IOCA_RPI].remoteIP,
                                          ListenPortArray[DST_PORT_IOCA_RPI].txPortNum,
                                          &txBuffer[0], char_idx, 0);
    if (txStatus != d_STATUS_SUCCESS)
    {
        /* Transmit error message on UART */
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"UDP RPI Send Error\n\r", 20);
    }
}

/**
 * @brief Sends data to PIL (Processor-in-the-Loop) via UDP
 *
 * This function copies the provided buffer data to a local transmission buffer
 * and sends it via UDP to the PIL destination using the configured remote IP
 * and port number from the IOCA PIL port configuration.
 *
 * @param[in] buffer Pointer to the data buffer to be transmitted
 * @param[in] len    Length of the data to be transmitted in bytes
 *
 * @note The function limits transmission to a maximum of 52 bytes due to
 *       the fixed size of the internal txBuffer
 * @note If len exceeds the txBuffer size, only the first 52 bytes will be sent
 *
 * @return None
 *
 * @see d_ETH_UdpSendIf
 * @see ListenPortArray
 */
void udp_send_pil(const uint8_t *buffer, uint32_t len)
{
    Uint8_t txBuffer[52];
    uint32_t char_idx = 0;
    /* Copy the data to the txBuffer */
    for (char_idx = 0; char_idx < len && char_idx < sizeof(txBuffer); char_idx++)
    {
        txBuffer[char_idx] = buffer[char_idx];
    }
    d_Status_t txStatus = d_ETH_UdpSendIf(ListenPortArray[DST_PORT_IOCA_PIL].remoteIP, 
                                          ListenPortArray[DST_PORT_IOCA_PIL].txPortNum, 
                                          &txBuffer[0], char_idx, 0);

    if (txStatus != d_STATUS_SUCCESS)
    {
        /* Transmit error message on UART */
        uart_write(UART_DEBUG_CONSOLE, (uint8_t *)"UDP PIL Send Error\n\r", 20);
    }
}

/**
 * @brief Receives UDP data from specified source
 * 
 * This function is a placeholder for handling incoming UDP data from various sources.
 * It takes the received data buffer, its length, and the source identifier as parameters.
 * 
 * @param buffer Pointer to the received data buffer
 * @param len Length of the received data in bytes
 * @param udp_source Identifier of the UDP source (e.g., GCS, PIL, RPI)
 * 
 * @return None
 * 
 * @note The implementation of this function should include processing logic
 *       based on the source of the UDP data
 */
void udp_receive(uint8_t *buffer, uint32_t len, udp_source_t udp_source)
{

}

/**
 * @brief Initializes memory subsystems including MAC FLASH and QSPI FLASH
 * 
 * This function performs initialization of the memory subsystems required for
 * proper system operation. It initializes the MAC FLASH to ensure FCC Ethernet
 * interfaces have a proper MAC address, and also initializes the QSPI FLASH.
 * 
 * The function unlocks both MAC FLASH sectors (0 and 1) which enables writing
 * capabilities. Note that this could potentially allow overwriting of hardware
 * and software configuration information stored in these sectors.
 * 
 * @return d_Status_t Status of the initialization operation
 *         - d_STATUS_SUCCESS: All memory subsystems initialized successfully
 *         - Other values: Initialization failed at some stage
 * 
 * @warning Unlocking MAC FLASH sectors opens them for writing operations,
 *          which may risk overwriting critical configuration data
 */
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

/**
 * @brief Initializes the Ethernet interface for UDP communication
 * 
 * This function performs the necessary initialization steps to set up the
 * Ethernet hardware and software components required for UDP networking
 * functionality. It configures the network interface and prepares it for
 * data transmission and reception.
 * 
 * @return d_Status_t Returns the status of the initialization operation
 *                   - Success status if initialization completed successfully
 *                   - Error status if initialization failed
 * 
 * @note This function should be called before any UDP communication operations
 * @note Function is static and only accessible within this compilation unit
 * 
 * @see udp_main.c
 */
static d_Status_t eth_initialise(void)
{
    // Startup info
    uint8_t numIF = 0;
    uint8_t numPort = 0;
    uint8_t mac_ethernet_address[6];
    uint32_t mySlotNum = d_FCU_SlotNumber();
    bool is_lp_setup_success = false;
    const char ioc_a_gcs_success[80] = "\n\rITB Interface setup for GCS (via IOC A) done.\n\r";
    const char ioc_b_gcs_success[80] = "\n\rITB Interface setup for GCS (via IOC B) done.\n\r";
    
    const char ioc_a_pil_success[80] = "\n\rITB Interface setup for PIL (via IOC A) done.\n\r";
//    const char ioc_b_pil_success[80] = "\n\rITB Interface setup for PIL (via IOC B) done.\n\r";

    const char ioc_a_rpi_success[80] = "\n\rITB Interface setup for RPI (via IOC A) done.\n\r";
//    const char ioc_b_rpi_success[80] = "\n\rITB Interface setup for RPI (via IOC B) done.\n\r";

    const char fcu_to_fcu_ioca_success[80] = "\n\rITB Interface setup for FCU to FCU (via IOC A) done.\n\r";

    //    const char fcu_internal_success[80] = "\n\rFCU to FCU Internal Interface setup done.\n\r";
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
    // Setup the Ethernet end point for the external interface via IOCA      192.168.<69>.x
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (returnValue == d_STATUS_SUCCESS)
    {
        // After initialisation the MAC and IP addresses of the local end points are not used further.
        // Thus there is no need to make an array of interface information.
        returnValue = d_FLASH_MAC_MacAddress(0, &mac_ethernet_address[0]);

        // My Static IP for talking to the ITB
        d_ETH_EndPoint_t fcuEndpoint;
        fcuEndpoint.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 69u, 50u + mySlotNum);
        fcuEndpoint.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuEndpoint.gateway = d_ETH_Ipv4Addr(192u, 168u, 69u, 1u);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuEndpoint, XPAR_PSU_ETHERNET_2_BASEADDR, NULL); // //XPAR_PSU_ETHERNET_3_BASEADDR
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    /* Setup a UDP listening ports to receive data from the end points */
    /* Note. Unlike the MC code the FC uses different callback functions for the different ports. */
    /* This ensures that the ITB IpAddress can be dynamically assigned instead of hardcoded. */
    if (returnValue == d_STATUS_SUCCESS)
    {

        display_mac(&mac_ethernet_address[0]);

        /* Configure the GCS Listening Port */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_IOCA_GCS], 
                                                    &ListenPortArray[DST_PORT_IOCA_GCS], 
                                                    receivecallback_via_ioca_gcs);
        /* Check if UDP listening port for GCS setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_a_gcs_success, sizeof(ioc_a_gcs_success));
            /* increment the port count */
            numPort++;
        }
        
        /* Configure the RPI Listening Port */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_IOCA_RPI], 
                                                    &ListenPortArray[DST_PORT_IOCA_RPI], 
                                                    receivecallback_via_ioca_rpi);

        /* Check if UDP listening port for RPI setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_a_rpi_success, sizeof(ioc_a_rpi_success));
            /* increment the port count */
            numPort++;
        }

        /* Configure the PIL Listening Port */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_IOCA_PIL], 
                                                    &ListenPortArray[DST_PORT_IOCA_PIL], 
                                                    receivecallback_via_ioca_pil);

        /* Check if UDP listening port for PIL setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_a_pil_success, sizeof(ioc_a_pil_success));
            /* increment the port count */
            numPort++;
        }

        /* UDP listening port for FCU to FCU comms via the IOC-A */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_FCU_VIA_IOCA], 
                                                    &ListenPortArray[DST_PORT_FCU_VIA_IOCA], 
                                                    fcu_to_fcu_receivecallback);

        /* Check if UDP listening port for FCU to FCU via IOC-A setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)fcu_to_fcu_ioca_success, sizeof(fcu_to_fcu_ioca_success));
            /* increment the port count */
            numPort++;
        }

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
        fcuEndpoint.ipaddr = d_ETH_Ipv4Addr(192u, 168u, 69u, 51u + mySlotNum);
        fcuEndpoint.netmask = d_ETH_Ipv4Addr(255u, 255u, 255u, 0u);
        fcuEndpoint.gateway = d_ETH_Ipv4Addr(192u, 168u, 69u, 1u);

        returnValue = d_ETH_InterfaceAdd(mac_ethernet_address, &fcuEndpoint, XPAR_PSU_ETHERNET_1_BASEADDR, NULL);
        if (returnValue == d_STATUS_SUCCESS)
            numIF++;
    }

    /* Setup a UDP listening ports to receive data from the end points */
    if (returnValue == d_STATUS_SUCCESS)
    {
        
        display_mac(&mac_ethernet_address[0]);

        /* UDP listening port for FCU to FCU comms via the IOC-B */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_IOCB_GCS], 
                                                    &ListenPortArray[DST_PORT_IOCB_GCS], 
                                                    receivecallback_via_iocb);
        
        /* Check if UDP listening port for GCS setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)ioc_b_gcs_success, sizeof(ioc_b_gcs_success));
            /* increment the port count */
            numPort++;
        }

        /* UDP listening port for FCU to FCU comms via the IOC-B */
        is_lp_setup_success = setup_udp_listen_port(&UdpPortConfig[DST_PORT_FCU_VIA_IOCB], 
                                                    &ListenPortArray[DST_PORT_FCU_VIA_IOCB], 
                                                    fcu_to_fcu_receivecallback);

        /* Check if UDP listening port for FCU to FCU via IOC-B setup successfully */
        if (is_lp_setup_success == true)
        {
            uart_write(UART_DEBUG_CONSOLE, (uint8_t *)fcu_to_fcu_ioca_success, sizeof(fcu_to_fcu_ioca_success));
            /* increment the port count */
            numPort++;
        }
    }

    /* TBD : This part needs to be tested - Action post subscale flight */

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
        display_mac(&mac_ethernet_address[0]);

        ListenPortArray[DST_PORT_FCUPRIMARY].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 89u, 51u - mySlotNum);
        ListenPortArray[DST_PORT_FCUPRIMARY].rxPortNum = 4000; // Port to listen to
        ListenPortArray[DST_PORT_FCUPRIMARY].txPortNum = 4000; // Port used for response
        // Message debug information
        ListenPortArray[DST_PORT_FCUPRIMARY].msgInCount = 0;
        ListenPortArray[DST_PORT_FCUPRIMARY].msgOutCount = 0;
        ListenPortArray[DST_PORT_FCUPRIMARY].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[DST_PORT_FCUPRIMARY].rxPortNum, fcu_to_fcu_receivecallback);
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
        display_mac(&mac_ethernet_address[0]);

        ListenPortArray[DST_PORT_FCUBACKUP].remoteIP = d_ETH_Ipv4Addr(192u, 168u, 90u, 51u - mySlotNum);
        ListenPortArray[DST_PORT_FCUBACKUP].rxPortNum = 5000; // Port to listen to
        ListenPortArray[DST_PORT_FCUBACKUP].txPortNum = 5000; // Port used for response
        // Message debug information
        ListenPortArray[DST_PORT_FCUBACKUP].msgInCount = 0;
        ListenPortArray[DST_PORT_FCUBACKUP].msgOutCount = 0;
        ListenPortArray[DST_PORT_FCUBACKUP].msgNotProcessedCount = 0;

        returnValue = d_ETH_UdpListen(ListenPortArray[DST_PORT_FCUBACKUP].rxPortNum, fcu_to_fcu_receivecallback);
        if (returnValue == d_STATUS_SUCCESS)
            numPort++;
    }

    char uart_msg[100];
    char numIF_str[4];
    char numPort_str[4];
    int index = 0;

    // Convert numbers to strings
    convert_uint8_to_char(numIF, numIF_str);
    convert_uint8_to_char(numPort, numPort_str);

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

static bool setup_udp_listen_port(const udpportconfig_t *config, eth_if_port_def_t *portDef, d_ETH_UdpReceiveFunc_t callback)
{
    portDef->remoteIP = d_ETH_Ipv4Addr(config->octet1, config->octet2, config->octet3, config->octet4);
    portDef->rxPortNum = config->rxport;
    portDef->txPortNum = config->txport;
    portDef->msgInCount = 0;
    portDef->msgOutCount = 0;
    portDef->msgNotProcessedCount = 0;
    d_Status_t status = d_ETH_UdpListen(portDef->rxPortNum, callback);

    return (status == d_STATUS_SUCCESS);
}

/**
 * @brief Callback function for receiving UDP packets via IOCA GCS interface
 * 
 * This function is called when a UDP packet is received on the IOCA GCS port.
 * It validates the incoming message by checking the destination port against
 * the configured listening port, stores the remote IP address for response
 * purposes, and updates message counters for monitoring.
 * 
 * @param sourceAddress     Source IP address of the incoming packet
 * @param sourcePort        Source UDP port number of the incoming packet
 * @param destinationPort   Destination UDP port number of the incoming packet
 * @param pbuffer          Pointer to the packet data buffer
 * @param length           Length of the packet data in bytes
 * 
 * @return void
 * 
 * @note This function updates the following counters:
 *       - msgInCount: Incremented for every received message
 *       - msgNotProcessedCount: Incremented for invalid messages
 * @note The remote IP address is stored for response transmission
 */
static void                         
receivecallback_via_ioca_gcs(
    const Ipv4Addr_t sourceAddress,
    const Uint16_t sourcePort,     
    const Uint16_t destinationPort,
    const Uint8_t *const pbuffer,  
    const Uint32_t length          
)
{
    Bool_t msgProcessed = d_FALSE;

    /* Increment incoming message count */
    ListenPortArray[DST_PORT_IOCA_GCS].msgInCount++;

    /* Check for valid message coming in from the Correct IP source Address and UDP port. */
    if ((destinationPort == ListenPortArray[DST_PORT_IOCA_GCS].rxPortNum) && 
        (ListenPortArray[DST_PORT_IOCA_GCS].remoteIP == sourceAddress))
    {
        for(uint32_t i = 0; i < length && i < BUFF_SIZE; i++)
        {
            GcsRxDataPool[i] = pbuffer[i];
        }

        /* Mark message as processed */
        msgProcessed = d_TRUE;
    }

    /* If message was not processed, increment the not processed count */
    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[DST_PORT_IOCA_GCS].msgNotProcessedCount++;
    }

    return;
}

/**
 * @brief Callback function to handle incoming UDP packets for IOCA PIL communication
 * 
 * This function processes incoming UDP packets received via the IOCA PIL interface.
 * It validates the destination port against the configured listening port and stores
 * the remote IP address for response purposes. Message statistics are updated to
 * track processed and unprocessed messages.
 * 
 * @param[in] sourceAddress     Source IP address of the incoming packet
 * @param[in] sourcePort        Source port number of the incoming packet  
 * @param[in] destinationPort   Destination port number of the incoming packet
 * @param[in] pbuffer          Pointer to the packet buffer containing received data
 * @param[in] length           Length of the received data in bytes
 * 
 * @return None
 * 
 * @note This function updates the ListenPortArray[DST_PORT_IOCA_PIL] entry with:
 *       - Message count statistics (msgInCount, msgNotProcessedCount)
 *       - Remote IP address for response routing
 * 
 * @warning The function only processes messages destined for the configured
 *          DST_PORT_IOCA_PIL port number
 */
static void                         
receivecallback_via_ioca_pil(
    const Ipv4Addr_t sourceAddress, 
    const Uint16_t sourcePort,      
    const Uint16_t destinationPort, 
    const Uint8_t *const pbuffer,   
    const Uint32_t length           
)
{
    Bool_t msgProcessed = d_FALSE;

    ListenPortArray[DST_PORT_IOCA_PIL].msgInCount++;

    // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
    if (destinationPort == ListenPortArray[DST_PORT_IOCA_PIL].rxPortNum)
    {
        ListenPortArray[DST_PORT_IOCA_PIL].remoteIP = sourceAddress; // We store the itb's source address, and then respond to the same address.

        msgProcessed = d_TRUE;
    }

    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[DST_PORT_IOCA_PIL].msgNotProcessedCount++;
    }

    return;
}

static void                         
receivecallback_via_ioca_rpi(
    const Ipv4Addr_t sourceAddress,
    const Uint16_t sourcePort,
    const Uint16_t destinationPort,
    const Uint8_t *const pbuffer,
    const Uint32_t length
)
{
    Bool_t msgProcessed = d_FALSE;

    ListenPortArray[DST_PORT_IOCA_RPI].msgInCount++;

    // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
    if (destinationPort == ListenPortArray[DST_PORT_IOCA_RPI].rxPortNum)
    {
        ListenPortArray[DST_PORT_IOCA_RPI].remoteIP = sourceAddress; // We store the itb's source address, and then respond to the same address.

        msgProcessed = d_TRUE;
    }

    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[DST_PORT_IOCA_RPI].msgNotProcessedCount++;
    }

    return;
}

/**
 * @brief ITB receive callback function via IOCB for handling incoming UDP packets
 * 
 * This function processes incoming UDP packets from the ITB (presumably Integrated Test Bench)
 * via IOCB (Input/Output Control Block). It validates the destination port against the 
 * configured listening port and updates message statistics. When a valid message is received,
 * it stores the source IP address for response purposes.
 * 
 * @param sourceAddress     [in] Source IP address of the incoming packet
 * @param sourcePort        [in] Source UDP port number of the incoming packet
 * @param destinationPort   [in] Destination UDP port number of the incoming packet
 * @param pbuffer          [in] Pointer to the packet buffer containing the received data
 * @param length           [in] Length of the received data in bytes
 * 
 * @return None
 * 
 * @note This function increments message counters for both processed and unprocessed messages
 * @note The remote IP address is stored only for valid messages matching the expected port
 * @note Function uses global ListenPortArray with LP_ITB_IOCB index
 */
static void                        
receivecallback_via_iocb(
    const Ipv4Addr_t sourceAddress,
    const Uint16_t sourcePort,     
    const Uint16_t destinationPort,
    const Uint8_t *const pbuffer,  
    const Uint32_t length          
)
{
    Bool_t msgProcessed = d_FALSE;

    ListenPortArray[DST_PORT_IOCB_GCS].msgInCount++;

    // Check for valid message coming in from the ITB  (Correct IP source Address and UDP port.)
    if (destinationPort == ListenPortArray[DST_PORT_IOCB_GCS].rxPortNum)
    {
        ListenPortArray[DST_PORT_IOCB_GCS].remoteIP = sourceAddress; // We store the itb's source address, and then respond to the same address.

        msgProcessed = d_TRUE;
    }

    if (msgProcessed == d_FALSE)
    {
        ListenPortArray[DST_PORT_IOCB_GCS].msgNotProcessedCount++;
    }

    return;
}

/**
 * @brief Callback function for handling FCU to FCU UDP message reception
 * 
 * This function processes incoming UDP packets between FCU (Flight Control Unit) 
 * instances. It validates the destination port against configured listen ports,
 * increments message counters, and verifies the source IP address for message
 * authentication. The function supports multiple communication channels including
 * internal primary/secondary links and external IOCA/IOCB connections.
 * 
 * @param[in] sourceAddress     Source IP address of the incoming packet
 * @param[in] sourcePort        Source UDP port number of the incoming packet
 * @param[in] destinationPort   Destination UDP port number of the incoming packet
 * @param[in] pbuffer          Pointer to the packet data buffer
 * @param[in] length           Length of the packet data in bytes
 * 
 * @return None
 * 
 * @note This function iterates through all configured listen ports and processes
 *       messages only for matching destination ports with valid source addresses.
 *       Unprocessed messages are counted for diagnostic purposes.
 * 
 * @warning The function currently contains a TODO comment indicating it should
 *          be refactored to use a sub-function with index parameter.
 */
static void                         
fcu_to_fcu_receivecallback(
    const Ipv4Addr_t sourceAddress, 
    const Uint16_t sourcePort,      
    const Uint16_t destinationPort, 
    const Uint8_t *const pbuffer,   
    const Uint32_t length           
)
{

    // TODO.  Rewrite this as a sub function taking in the index

    for (Uint8_t idx = 0; idx < DST_PORT_COUNT; idx++)
    {

        if (destinationPort == ListenPortArray[idx].rxPortNum)
        {
            ListenPortArray[idx].msgInCount++;

            switch (idx)
            {
            case DST_PORT_FCUPRIMARY:
                // GLOBAL_SerialPrint("<<    FCU to FCU - Internal Primary   - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case DST_PORT_FCUBACKUP:
                // GLOBAL_SerialPrint("<<    FCU to FCU - Internal Secondary - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case DST_PORT_FCU_VIA_IOCA:
                // GLOBAL_SerialPrint("<<    FCU to FCU - External IOCA      - [Rx Count: %d]\n\r", ListenPortArray[idx].msgInCount, 0.0f);
                break;
            case DST_PORT_FCU_VIA_IOCB:
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

/**
 * @brief Displays a MAC address in formatted output via UART debug console
 * 
 * This function formats a 6-byte MAC address into a human-readable string format
 * with the pattern "    MAC: XX:XX:XX:XX:XX:XX\n\r" and transmits it through
 * the UART debug console.
 * 
 * @param mac_ethernet_address Array of 6 bytes representing the MAC address
 *                            Each byte represents one octet of the MAC address
 * 
 * @note The output format includes:
 *       - 4 leading spaces for indentation
 *       - "MAC: " prefix
 *       - Hexadecimal representation of each byte separated by colons
 *       - Newline and carriage return terminators
 * 
 * @warning The function assumes convert_uint8_to_char() produces exactly 3 characters
 *          and that uart_write() and UART_DEBUG_CONSOLE are properly configured
 * 
 * @dependencies 
 *       - convert_uint8_to_char(): Converts uint8 to 3-character string
 *       - uart_write(): Transmits data via UART
 *       - UART_DEBUG_CONSOLE: Debug console UART identifier
 */
static void display_mac(uint8_t mac_ethernet_address[6])
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
        convert_uint8_to_char(mac_ethernet_address[i], str);
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

static void convert_uint8_to_char(
    const uint8_t value,
    char *str)
{
    str[0] = '0' + (value / 100);       // Extract hundreds digit
    str[1] = '0' + ((value / 10) % 10); // Extract tens digit
    str[2] = '0' + (value % 10);        // Extract units digit
}
