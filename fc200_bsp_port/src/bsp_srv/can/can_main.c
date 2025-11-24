/****************************************************
 *  can_main.c
 *  Created on: 25-Aug-2025 10:22:22 AM
 *  Implementation of the Class can_main
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "can_main.h"
#include "soc/can/d_can_cfg.h"
#include "soc/can/d_can.h"
#include "sru/can_holt/d_can_holt.h"     /* Discrete driver */
#include "sru/can_holt/d_can_holt_cfg.h" /* Discrete driver config */
#include "xcanps.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "sru/fcu/d_fcu.h"

// Masks for extracting ID components
#define CAN_BASE_ID_MASK (0x7FFU)       // 11 bits: 0b11111111111
#define CAN_EXTENDED_ID_MASK (0x3FFFFU) // 18 bits: 0b111111111111111111
#define CAN_EXTENDED_BIT_LEN (18)

static bool CanInitialized[CAN_CHANNEL_MAX];

/**
 * @brief Initialize a CAN channel
 *
 * This function initializes the specified CAN channel. It supports both MPSoC PS CAN
 * Controllers (channels 0-1) and CAN HOLT devices (channels 2+). For HOLT devices,
 * the channel number is remapped by subtracting CAN_CHANNEL_3 offset.
 *
 * @param can_ch The CAN channel to initialize (must be < CAN_CHANNEL_MAX)
 *
 * @return can_status_t Returns CAN_OK on success, CAN_ERROR on failure
 *
 * @retval CAN_OK     Channel initialization successful
 * @retval CAN_ERROR  Channel initialization failed (invalid channel or driver error)
 *
 * @note For channels < CAN_CHANNEL_2: Uses MPSoC PS CAN Controller
 * @note For channels >= CAN_CHANNEL_2: Uses CAN HOLT devices with remapped channel numbers
 *
 * @see d_CAN_Initialise
 * @see d_CAN_HOLT_Initialise
 */
can_status_t can_init(can_channel_t can_ch)
{
    can_status_t status = CAN_OK;
    d_Status_t drv_status = d_STATUS_SUCCESS;

    /* if channel is invalid */
    if (can_ch >= CAN_CHANNEL_MAX)
    {
        status = CAN_ERROR;
    }
    else
    {
        /* Initialise the CAN channel at MPSoC PS CAN Controller */
        if (can_ch <= CAN_CHANNEL_2)
        {
            /* Initialise the CAN channel at MPSoC PS CAN Controller */
            drv_status = d_CAN_Initialise(can_ch);

            /* Check if the driver initialization was successful */
            if (drv_status == d_STATUS_SUCCESS)
            {
                if (d_CAN_ModeSet(can_ch, d_CAN_MODE_NORMAL) == d_STATUS_SUCCESS)
                {
                    CanInitialized[can_ch] = true;
//                    d_CAN_InterruptEnable(can_ch, XCANPS_IXR_RXFWMFLL_MASK);
//                    d_INT_IrqEnable(d_CAN_Config[can_ch].interruptNumber);
                }
            }
            else
            {
                status = CAN_ERROR;
            }
        }
        else
        {
            /* Initialise the CAN channel at CAN HOLT Devices after remapping the channel number */
            drv_status = d_CAN_HOLT_Initialise(can_ch - CAN_CHANNEL_3);

            /* Set the mode to Initialise for the HOLT device */
            if (drv_status == d_STATUS_SUCCESS)
            {
                drv_status = d_CAN_HOLT_ModeSet((can_ch - CAN_CHANNEL_3), d_CAN_HOLT_MODE_INITIALISE);
            }

            /* Check if the mode initialization was successful */
            if (drv_status == d_STATUS_SUCCESS)
            {
                /* Disable filters for the HOLT device */
                drv_status = d_CAN_HOLT_FilterDisable(can_ch - CAN_CHANNEL_3);
            }

            /* Set the mode to Normal for the HOLT device */
            if (drv_status == d_STATUS_SUCCESS)
            {
                drv_status = d_CAN_HOLT_ModeSet((can_ch - CAN_CHANNEL_3), d_CAN_HOLT_MODE_NORMAL);
            }

            /* Check if the driver initialization was successful */
            if (drv_status == d_STATUS_SUCCESS)
            {
                CanInitialized[can_ch] = true;
                status = CAN_OK;
            }
            else
            {
                status = CAN_ERROR;
            }
        }
    }

    return status;
}

/**
 * @brief Transmits a CAN message on the specified CAN channel
 *
 * This function sends a CAN message through either the MPSoC PS CAN Controller
 * (for channels 0-1) or CAN HOLT Devices (for channels 2+). The function validates
 * the channel number and message length before transmission.
 *
 * @param can_ch    CAN channel identifier (must be < CAN_CHANNEL_MAX)
 * @param can_msg   CAN message structure containing:
 *                  - can_msg_id: CAN message identifier
 *                  - is_extended_id: Flag for extended ID format
 *                  - is_remote_req: Flag for remote transmission request
 *                  - dlc: Data length code (must be <= CAN_MAX_DLC)
 *                  - data: Message payload bytes
 *
 * @return can_status_t Status of the transmission operation:
 *         - CAN_OK: Message sent successfully
 *         - CAN_ERROR: Invalid channel or transmission failure
 *         - CAN_INVALID_MSG_LENGTH: DLC exceeds maximum allowed length
 *
 * @note For channels >= CAN_CHANNEL_2, the channel number is remapped by
 *       subtracting CAN_CHANNEL_3 before calling the HOLT device driver
 * @note Substitute remote transmission request is not supported and is set to FALSE
 */
can_status_t can_write(can_channel_t can_ch, const can_msg_t *ptr_can_msg)
{
    can_status_t status = CAN_OK;
    d_Status_t drv_status = d_STATUS_SUCCESS;
    d_CAN_Message_t txMessage;
    uint32_t selectedMaster = d_FCU_GetMaster();
	uint32_t slot = d_FCU_SlotNumber();

    /* if channel is invalid */
    if ((can_ch >= CAN_CHANNEL_MAX) || (ptr_can_msg == NULL))
    {
        status = CAN_ERROR;
    }
    /* if message length is invalid */
    else if (ptr_can_msg->dlc > CAN_MAX_DLC)
    {
        status = CAN_INVALID_MSG_LENGTH;
    }
    else
    {
        /* Prepare the CAN message for transmission */
        txMessage.extended = ptr_can_msg->extended_id_flag ? d_TRUE : d_FALSE;
        txMessage.id = (Uint32_t)((ptr_can_msg->can_msg_id >> CAN_EXTENDED_BIT_LEN) & CAN_BASE_ID_MASK);
        txMessage.exId = (Uint32_t)((ptr_can_msg->can_msg_id) & CAN_EXTENDED_ID_MASK);
        txMessage.substituteRemoteTxRequest = d_FALSE; /* Not supported in this interface */
        txMessage.remoteTxRequest = ptr_can_msg->is_remote_req ? d_TRUE : d_FALSE;
        txMessage.dataLength = ptr_can_msg->dlc;

        /* Copy the data bytes */
        for (uint8_t byte_id = 0; byte_id < ptr_can_msg->dlc; byte_id++)
        {
            txMessage.data[byte_id] = ptr_can_msg->data[byte_id];
        }

        /* IMPORTANT: Currently Transmit CAN Message only from FCU-1 - MUST CHANGE to either of the master FCU-1 or FCU-2*/
        if ((selectedMaster == 0) && (slot == 0))
        {
            if (can_ch <= CAN_CHANNEL_2)
            {
                /* Send the CAN message from MPSoC PS CAN Controller */
                drv_status = d_CAN_SendMessage(can_ch, &txMessage);
            }
            else
            {
                /* Send the CAN message from CAN HOLT Devices after remapping the channel number */
                drv_status = d_CAN_HOLT_SendMessage(can_ch - CAN_CHANNEL_3, (d_CAN_HOLT_Message_t *)&txMessage);
            }

            if (drv_status != d_STATUS_SUCCESS)
            {
                status = CAN_ERROR;
            }
        }
        else
        {
            status = CAN_ERROR;
        }
    }

    return status;
}

can_status_t can_read(can_channel_t can_ch, can_msg_t *ptr_can_msg)
{
    can_status_t status = CAN_OK;
    d_Status_t drv_status = d_STATUS_SUCCESS;
    d_CAN_Message_t rxMessage;

    /* if channel is invalid */
    if (can_ch >= CAN_CHANNEL_MAX || ptr_can_msg == NULL)
    {
        status = CAN_ERROR;
    }
    else
    {
        if (can_ch <= CAN_CHANNEL_2)
        {
            /* Receive the CAN message from MPSoC PS CAN Controller */
            drv_status = d_CAN_ReceiveMessage(can_ch, &rxMessage);
        }
        else
        {
            /* Receive the CAN message from CAN HOLT Devices after remapping the channel number */
            drv_status = d_CAN_HOLT_ReceiveMessage(can_ch - CAN_CHANNEL_3, (d_CAN_HOLT_Message_t *)&rxMessage);
        }

        if (drv_status == d_STATUS_SUCCESS)
        {
            /* Prepare the received CAN message */
            ptr_can_msg->extended_id_flag = (rxMessage.extended == d_TRUE) ? true : false;
            ptr_can_msg->can_msg_id = ((rxMessage.id & CAN_BASE_ID_MASK) << CAN_EXTENDED_BIT_LEN) |
                                      (rxMessage.exId & CAN_EXTENDED_ID_MASK);
            ptr_can_msg->is_remote_req = (rxMessage.remoteTxRequest == d_TRUE) ? true : false;
            ptr_can_msg->dlc = rxMessage.dataLength;

            /* Copy the data bytes */
            for (uint8_t byte_id = 0; byte_id < rxMessage.dataLength; byte_id++)
            {
                ptr_can_msg->data[byte_id] = rxMessage.data[byte_id];
            }
        }
        else
        {
            status = CAN_ERROR;
        }
    }

    return status;
}

// /* IMPORTANT: CAN Filter Cannot be set once initialized to normal mode - 
//    Filter settings must be performed in the init and before mode is set to normal*/
// can_status_t can_set_filter(can_channel_t can_ch, bool is_single_id_filter, 
// 	                        const uint32_t *const start_id, const uint32_t *const end_id)
// {
//     return CAN_OK;
// }
