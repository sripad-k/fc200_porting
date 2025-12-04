/****************************************************
 *  ach_epu.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class ach_epu
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "ach_epu.h"
#include "can_interface.h"
#include "generic_util.h"
#include <math.h>
#include "timer_interface.h"

// Signature constant (provide as needed)
#define ESC_STATUS_SIGN (0xA9AF28AEA2FBB254U)
#define ESC_RAW_MAX (8191u)                                       // maximum raw command send to an ESC
#define ESC_RAW_CMD_SIGN ((unsigned long long)0x217F5C87D7EC951D) // Raw Command Signature
#define ESC_RAW_ID (1030)                                         // RAW command ID
#define SOLONE_CAN_ID (25)                                        // can bus master device ID
#define ESC_1_STATUS_CAN_ID (0x18040A01U)
#define ESC_8_STATUS_CAN_ID (0x18040A08U)
#define NODE_ID_MASK (0x0FU)
#define TRANSFER_ID_MASK (0x1FU)
#define TM_UAVCAN_TAIL_TOGGLE (1U << 5)         /* Bit 5 for Toggle */
#define TM_UAVCAN_TAIL_START_TRANSFER (1U << 7) /* Bit 7 for Start of Transfer */
#define TM_UAVCAN_TAIL_END_TRANSFER (1U << 6)   /* Bit 6 for End of Transfer */
#define MAX_FRAMES_TO_READ (48U)                /* Max frames to read */
#define MAX_SEQ_BUF_LEN (6U)                    /* Max sequence buffer length */
#define ESC_STATUS_TIMEOUT_MS (100U)            /* ESC status timeout in ms */
#define CAN_EPU (CAN_CHANNEL_1)                 /* CAN channel for EPU */

/* Float16 conversion */
typedef union
{
    uint32_t u;
    float f;
} FP32_t;

/**
 * @def min(a, b)
 * @brief A macro that returns the minimum of \a a and \a a.
 */
#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

/**
 * @def max(a, b)
 * @brief A macro that returns the maximum of \a a and \a b.
 */
#ifndef max
#define max(x, y) (((x) > (y)) ? (x) : (y))
#endif

/**
 * @def sat(x, lb, ub)
 * @brief A macro that saturates \a x between the lower bound \a lb and the
 *        upper bound \a ub.
 */
#ifndef sat
#define sat(x, lb, ub) max(min((x), (ub)), (lb))
#endif

typedef enum
{
    // HIGHEST_PRIORITY = 0x00,
    HIGH_PRIORITY = 0x08,
    // MEDIUM_PRIORITY = 0x10,
    // LOW_PRIORITY    = 0x18,
    // LOWEST_PRIORITY = 0x1F,
} can_priority_t;

/* State machine states */
typedef enum
{

    FIRST_FRAME = 0,
    SECOND_FRAME = 1,
    THIRD_FRAME = 2,
    DECODE_STATUS = 3

} e_esc_parse_state_t;

/**
 * @brief State machine context
 */
typedef struct
{
    e_esc_parse_state_t state; /* Current state of the deframing process */
    bool is_success;           /* At least one valid set decoded */
    uint8_t node_id;
    uint32_t first_frame_can_id;              /* Message ID of first frame */
    uint32_t transfer_id;                     /* Transfer ID for the current message */
    can_msg_t frames[3];                      /* Store three frames */
    uint16_t crc_esc_status;                  /* CRC value for ESC status */
    uint8_t payload[ESC_STATUS_PAYLOAD_SIZE]; /* Extracted status payload */

} s_esc_deframe_context_t;

static s_esc_deframe_context_t EscStatusContext[MAX_ESCS] = {0}; /* Parsing State Machine Context */
static can_msg_t CanRxBuffer[MAX_ESCS][MAX_SEQ_BUF_LEN] = {0};   /* CAN RX buffer for each ESC */
s_esc_status_frame_t EscStatus[MAX_ESCS] = {0};                  /* Array to hold ESC status frames */
std_epu_cmd_t EscRawCmd;                                         /* Raw command structure for ESC */
s_timer_data_t EscStatusMon[MAX_ESCS] = {0};                     /* Timer to monitor ESC status reception */

static uint16_t crcAddByte(uint16_t crc_val, uint8_t byte);
static uint16_t crcAdd(uint16_t crc_val, const uint8_t *bytes, size_t len);
static uint16_t crcAddSignature(uint16_t crc_val, uint64_t data_type_signature);
static void convertToRawThrottleData(const uint16_t *input, int count, uint8_t *output);
static uint32_t createID_field(can_priority_t prio, uint16_t msg_id, uint8_t source_id);
static uint8_t createTailbyte(bool tx_start, bool tx_end, bool toggle, uint8_t tf_id);
static float convert_float16_to_float(uint16_t value);
static void ach_epu_read_rx_can_frame(e_esc_id_t esc_idx);
static bool ach_epu_verify_can_frame_sanity(uint32_t can_msg_id, uint8_t tail, e_esc_parse_state_t state);
static void ach_epu_raw_ctrl_cmd(const std_epu_cmd_t *motor);
static void ach_epu_decode_esc_status(const uint8_t *payload, s_esc_status_frame_t *esc);
static void ach_epu_can_buffer_update(void);

/**
 * @brief Retrieves the latest valid ESC status for a specified ESC ID.
 *
 * Copies the most recently received and decoded EscStatusFrame for the given ESC ID
 * into the output structure, if available.
 *
 * @param[in]  esc_id The ESC identifier (range: 0..NUM_ESCS-1).
 * @param[out] out    Pointer to EscStatusFrame structure to receive the data.
 */
bool ach_get_epu_status(uint8_t esc_id, s_esc_status_frame_t *out)
{
    bool is_valid = false;

    /* Validate pointer and ESC ID range */
    if ((out != NULL) && (esc_id < NUM_ESCS))
    {
        /* Return the ESC status valid */
        if (EscStatus[esc_id].valid == true)
        {
            is_valid = true;
            /* Copy the latest ESC status anyway, upto
            the consumer to use it or reject based on validity */
            *out = EscStatus[esc_id];
        }
        else
        {
            /* ESC Status is not valid, return the default ESC status */
            *out = EscStatus[0];
        }
    }

    return is_valid;
}

/**
 * @brief Sets the EPU (Electronic Power Unit) motor command values with bounds checking.
 *
 * This function copies the motor command values from the provided EPU command structure
 * to the global EscRawCmd structure, ensuring that each command value is within the valid
 * range [0.0, 1.0]. If a command value is less than 0.0, it is set to 0.0. If it is greater
 * than 1.0, it is set to 1.0. Otherwise, the value is copied as is.
 *
 * @param[in] epu_cmd Pointer to the input EPU command structure containing motor command values.
 *                    If NULL, the function does nothing.
 */
void ach_set_epu_cmd(const std_epu_cmd_t *epu_cmd)
{
    if (epu_cmd != NULL)
    {
        for (uint8_t i = 0; i < NUM_ESCS; i++)
        {
            /* If command is negative */
            if (epu_cmd->motor_cmd_cval[i] < 0.0f)
            {
                /* Set to minimum */
                EscRawCmd.motor_cmd_cval[i] = 0.0f;
            }
            /* Else If command is greater than max command value */
            else if (epu_cmd->motor_cmd_cval[i] > 1.0f)
            {
                /* Set to maximum */
                EscRawCmd.motor_cmd_cval[i] = 1.0f;
            }
            else
            {
                /*Else, retain the command value as is */
                EscRawCmd.motor_cmd_cval[i] = epu_cmd->motor_cmd_cval[i];
            }
        }
    }
}

/**
 * @brief Initializes the EPU (Electronic Power Unit) CAN interface and related contexts.
 *
 * This function performs the following steps:
 *   - Initializes the CAN interface for the EPU.
 *   - Sets all motor command values to zero.
 *   - Configures the CAN RX filter to accept messages from ID 0.
 *   - Resets the ESC status context and status structures.
 *
 * @return true if initialization is successful, false otherwise.
 */
bool ach_epu_init(void)
{
    int res = (int)can_init(CAN_EPU);
    if (res != 0)
    {
        printf("Failed to open CAN interface '%d'\n", CAN_EPU);
        return false;
    }

    /* Initialize all motor commands to zero */
    for (e_esc_id_t esc_idx = ESC_ID_1; esc_idx < MAX_ESCS; esc_idx++)
    {
        EscRawCmd.motor_cmd_cval[esc_idx] = 0.0f;
        timer_start(&EscStatusMon[esc_idx], ESC_STATUS_TIMEOUT_MS);
    }

    util_memset(&EscStatusContext, 0, sizeof(EscStatusContext)); // Reset deframer context
    util_memset(&EscStatus, 0, sizeof(EscStatus));               // Reset status content
    util_memset(&CanRxBuffer, 0, sizeof(CanRxBuffer));           // Reset CAN RX buffer

    printf("Successfully opened CAN interface '%d'\n", CAN_EPU);
    return true; // All went well
}

/**
 * @brief Sends the periodic EPU command to the ESCs.
 *
 * This function sends the raw command values stored in EscRawCmd to the ESCs.
 * It is typically called periodically to update the motor commands.
 */
void ach_cmd_epu_periodic(void)
{
    /* Send the raw command to ESCs */
    ach_epu_raw_ctrl_cmd(&EscRawCmd);
}

/**
 * @brief Reads and processes periodic ESC (Electronic Speed Controller) status data from CAN bus
 *
 * This function performs a periodic read operation to update ESC status information by:
 * 1. Reading CAN frames and updating the CAN message buffer for all EPUs
 * 2. Processing CAN messages for each ESC in the system
 * 3. Validating the received ESC status data based on timeout monitoring
 * 4. Setting the validity flag for each ESC based on successful data reception
 *
 * The function iterates through all ESCs (ESC_ID_1 to MAX_ESCS) and marks each
 * ESC status as valid only if fresh data is received within the expected timeframe.
 * ESCs that timeout are marked as invalid to ensure system safety.
 *
 * @param None
 * @return None
 *
 * @note This function should be called periodically to maintain up-to-date ESC status
 * @note ESC status validity is determined by the EscStatusMon timer for each ESC
 * @see ach_epu_can_buffer_update()
 * @see ach_epu_read_rx_can_frame()
 * @see timer_check_expiry()
 */
void ach_epu_read_periodic(void)
{

    /* Read the CAN frames and populate the CAN messages per EPU */
    ach_epu_can_buffer_update();

    for (e_esc_id_t esc_idx = ESC_ID_1; esc_idx < MAX_ESCS; esc_idx++)
    {
        /* Set the ESC status valid flag to false initially */
        EscStatus[esc_idx].valid = false;

        /* Process the CAN messages for each ESC */
        ach_epu_read_rx_can_frame(esc_idx);

        /* Check if a valid ESC status was decoded */
        if (timer_check_expiry(&EscStatusMon[esc_idx]) == false)
        {
            EscStatus[esc_idx].valid = true;
        }
        else
        {
            /* Timed out before a valid ESC status was received, mark ESC status as invalid */
            EscStatus[esc_idx].valid = false;
        }
    }
}

/**
 * @brief Updates the CAN buffer by reading incoming CAN messages and distributing them to ESC buffers
 *
 * This function reads CAN frames from the EPU CAN interface up to a maximum limit per cycle
 * and distributes valid ESC status messages into their respective node buffers based on the
 * CAN message ID. The function continues reading until either the maximum frame limit is
 * reached or no new messages are available.
 *
 * @details The function performs the following operations:
 *          - Reads CAN frames from CAN_EPU interface (up to MAX_FRAMES_TO_READ per cycle)
 *          - Validates received messages against ESC status CAN ID range
 *          - Extracts node ID from CAN message ID using NODE_ID_MASK
 *          - Stores valid messages in CanRxBuffer based on node ID
 *          - Uses first-available slot strategy for buffer storage
 *
 * @note Only CAN messages with IDs in the range [ESC_1_STATUS_CAN_ID, ESC_8_STATUS_CAN_ID]
 *       and node IDs in the range [ESC_ID_1, ESC_ID_8] are processed and stored
 *
 * @note The function uses DLC field of 0 to identify empty buffer slots
 *
 * @param None
 * @return None
 */
static void ach_epu_can_buffer_update(void)
{
    can_status_t can_read_status = CAN_OK;
    uint16_t frame_count = 0U;
    can_msg_t can_rx_msg[MAX_FRAMES_TO_READ] = {0};

    /* Read Frames until MAX Frame limit to be read per cycle is met or it returns no new message */
    while ((frame_count < MAX_FRAMES_TO_READ) && (can_read_status == CAN_OK))
    {
        /* Read the CAN Frame */
        can_read_status = can_read(CAN_EPU, &can_rx_msg[frame_count]);
        /* Check if read was successful */
        if (can_read_status == CAN_OK)
        {
            /* Increment frame count */
            frame_count++;
            /* Determine the Node ID from the CAN message ID */
        }
    }

    /* Distribute the read CAN messages into the respective ESC buffers based on Node ID */
    for (uint16_t i = 0U; i < frame_count; i++)
    {
        /* Check if the received message is valid */
        if ((can_rx_msg[i].can_msg_id >= ESC_1_STATUS_CAN_ID) &&
            (can_rx_msg[i].can_msg_id <= ESC_8_STATUS_CAN_ID))
        {
            /* Extract the node id */
            uint8_t node_id = (uint8_t)(can_rx_msg[i].can_msg_id & NODE_ID_MASK);

            /* if node_id is within valid range */
            if (node_id >= ESC_ID_1 && node_id <= ESC_ID_8)
            {
                /* Find the next available slot in the ESC's CAN buffer */
                for (uint8_t j = 0U; j < MAX_SEQ_BUF_LEN; j++)
                {
                    if (CanRxBuffer[node_id][j].dlc == 0U)
                    {
                        /* Store the CAN message in the buffer */
                        util_memcpy(&CanRxBuffer[node_id][j], &can_rx_msg[i], sizeof(can_msg_t));
                        j = MAX_SEQ_BUF_LEN - 1; /* Exit the loop after storing */
                    }
                }
            }
        }
    }
}

/**
 * @brief Reads and processes CAN frames from the receive buffer for ESC status messages
 *
 * This function implements a state machine to deframe multi-frame CAN messages containing
 * ESC (Electronic Speed Controller) status data. It processes up to MAX_SEQ_BUF_LEN frames
 * from the CAN receive buffer, validates frame integrity, reconstructs the payload across
 * multiple frames, and decodes the final ESC status.
 *
 * State Machine Flow:
 * - FIRST_FRAME: Initializes context, extracts CRC and first payload bytes
 * - SECOND_FRAME: Validates continuity and extracts additional payload bytes
 * - THIRD_FRAME: Completes payload reconstruction and validates CRC
 * - DECODE_STATUS: Decodes final ESC status and resets state machine
 *
 * Frame Validation:
 * - Verifies CAN message ID consistency across frames
 * - Validates transfer ID matches across all frames
 * - Performs CRC validation on complete payload
 * - Checks frame sanity using dedicated validation function
 *
 * @param esc_idx ESC index identifying which ESC controller's CAN buffer to process
 *
 * @note This function modifies global state including:
 *       - EscStatusContext[esc_idx]: State machine context and frame data
 *       - EscStatus[esc_idx]: Decoded ESC status data
 *       - EscStatusMon[esc_idx]: Timer for status monitoring
 *       - CanRxBuffer[esc_idx]: CAN receive buffer (cleared after processing)
 *
 * @warning Function assumes esc_idx is valid and within array bounds
 * @warning Function processes frames sequentially and clears buffer slots after use
 */
static void ach_epu_read_rx_can_frame(e_esc_id_t esc_idx)
{
    bool is_valid = true;
    uint16_t frame_count = 0U;
    uint8_t tail = 0;

    while (frame_count < MAX_SEQ_BUF_LEN)
    {
        if (CanRxBuffer[esc_idx][frame_count].dlc != 0U)
        {
            /* Run the state machine to deframe CAN messages */
            switch (EscStatusContext[esc_idx].state)
            {
            case FIRST_FRAME:
            {
                /* Initialize the validity flag */
                is_valid = true;
                /* Reset the first frame MSG ID */
                EscStatusContext[esc_idx].first_frame_can_id = 0U;
                /* Initialize the esc status payload to 0 without using memset */
                util_memset(EscStatusContext[esc_idx].payload, 0, ESC_STATUS_PAYLOAD_SIZE);
                /* Reset the CRC for ESC status */
                EscStatusContext[esc_idx].crc_esc_status = 0U;
                EscStatusContext[esc_idx].is_success = false;
                EscStatusContext[esc_idx].first_frame_can_id = 0U;

                /* Read the CAN RX Buffer */
                EscStatusContext[esc_idx].frames[FIRST_FRAME] = CanRxBuffer[esc_idx][frame_count];

                /* Check if the DLC is non-zero */
                if ((EscStatusContext[esc_idx].frames[FIRST_FRAME].dlc >= 1U))
                {
                    /* extract the tail byte */
                    tail = EscStatusContext[esc_idx].frames[FIRST_FRAME].data[EscStatusContext[esc_idx].frames[FIRST_FRAME].dlc - 1U];
                    /* Verify CAN frame is correct */
                    is_valid = ach_epu_verify_can_frame_sanity(EscStatusContext[esc_idx].frames[FIRST_FRAME].can_msg_id, tail, EscStatusContext[esc_idx].state);
                    /* Check if the message ID is valid */
                    if (is_valid)
                    {
                        /* Get the Node ID */
                        EscStatusContext[esc_idx].node_id = (uint8_t)(EscStatusContext[esc_idx].frames[FIRST_FRAME].can_msg_id & NODE_ID_MASK);
                        /* Reset the ESC Status for the particular Node ID to FALSE */
                        // EscStatus[EscStatusContext.node_id].valid = false;
                        /* Store the first frame Message ID */
                        EscStatusContext[esc_idx].first_frame_can_id = EscStatusContext[esc_idx].frames[FIRST_FRAME].can_msg_id;
                        /* Get the transfer ID */
                        EscStatusContext[esc_idx].transfer_id = tail & TRANSFER_ID_MASK;
                        /* Get the CRC */
                        EscStatusContext[esc_idx].crc_esc_status = (uint16_t)((uint16_t)(EscStatusContext[esc_idx].frames[FIRST_FRAME].data[0]) |
                                                                              (uint16_t)(EscStatusContext[esc_idx].frames[FIRST_FRAME].data[1] << 8));
                        /* Store the first byte of the payload */
                        util_memcpy(&EscStatusContext[esc_idx].payload[0], &EscStatusContext[esc_idx].frames[FIRST_FRAME].data[2], 5U * (sizeof(uint8_t)));
                        /* Advance to the next state */
                        EscStatusContext[esc_idx].state = SECOND_FRAME;
                    }
                    else
                    {
                        EscStatusContext[esc_idx].state = FIRST_FRAME;
                    }
                }
                break;
            }

            case SECOND_FRAME:
            {
                /* Sequential checks for second frame */
                is_valid = true;
                /* Read the second consecutive frame from the CAN RX Buffer*/
                EscStatusContext[esc_idx].frames[SECOND_FRAME] = CanRxBuffer[esc_idx][frame_count];

                /* Check if the DLC is non-zero */
                if ((EscStatusContext[esc_idx].frames[SECOND_FRAME].dlc >= 1U))
                {
                    /* extract the tail byte */
                    tail = EscStatusContext[esc_idx].frames[SECOND_FRAME].data[EscStatusContext[esc_idx].frames[SECOND_FRAME].dlc - 1U];
                    /* Verify CAN frame is correct */
                    is_valid = ach_epu_verify_can_frame_sanity(EscStatusContext[esc_idx].frames[SECOND_FRAME].can_msg_id, tail, EscStatusContext[esc_idx].state);
                    /* Check if the message ID is equal to previous message ID */
                    is_valid &= (EscStatusContext[esc_idx].frames[SECOND_FRAME].can_msg_id == EscStatusContext[esc_idx].first_frame_can_id);
                    /* Check if the transfer ID matches */
                    is_valid &= (EscStatusContext[esc_idx].transfer_id == (tail & 0x1FU));
                    /* if all the checks above are valid */
                    if (is_valid)
                    {
                        /* Advance to the next state */
                        EscStatusContext[esc_idx].state = THIRD_FRAME;
                        /* Store the fifth byte of the payload */
                        util_memcpy(&EscStatusContext[esc_idx].payload[5], &EscStatusContext[esc_idx].frames[SECOND_FRAME].data[0], 7U * (sizeof(uint8_t)));
                    }
                    else
                    {
                        /* Reset state to first frame */
                        EscStatusContext[esc_idx].state = FIRST_FRAME;
                    }
                }
                break;
            }

            case THIRD_FRAME:
            {
                /* Sequential checks for third frame */
                is_valid = true;
                /* Read the third consecutive frame */
                EscStatusContext[esc_idx].frames[THIRD_FRAME] = CanRxBuffer[esc_idx][frame_count];

                /* Check if the DLC is non-zero */
                if ((EscStatusContext[esc_idx].frames[THIRD_FRAME].dlc >= 1U))
                {
                    /* extract the tail byte */
                    tail = EscStatusContext[esc_idx].frames[THIRD_FRAME].data[EscStatusContext[esc_idx].frames[THIRD_FRAME].dlc - 1U];
                    /* Verify CAN frame is correct */
                    is_valid = ach_epu_verify_can_frame_sanity(EscStatusContext[esc_idx].frames[THIRD_FRAME].can_msg_id, tail, EscStatusContext[esc_idx].state);
                    /* Check if the message ID is equal to previous message ID */
                    is_valid &= (EscStatusContext[esc_idx].frames[THIRD_FRAME].can_msg_id == EscStatusContext[esc_idx].first_frame_can_id);
                    /* Check if the transfer ID matches */
                    is_valid &= (EscStatusContext[esc_idx].transfer_id == (tail & 0x1FU));
                    /* if all the checks above are valid */
                    if (is_valid)
                    {
                        /* Store number of relevant bytes received in the third frame */
                        util_memcpy(&EscStatusContext[esc_idx].payload[12], &EscStatusContext[esc_idx].frames[THIRD_FRAME].data[0], 2U * (sizeof(uint8_t)));
                        /* Calculate the CRC for the ESC status */
                        uint16_t SignCrc = crcAddSignature(0xFFFFU, ESC_STATUS_SIGN);
                        uint16_t CrC_encoded = crcAdd(SignCrc, &EscStatusContext[esc_idx].payload[0], 14);

                        /* Check if the CRC matches */
                        if (CrC_encoded == EscStatusContext[esc_idx].crc_esc_status)
                        {
                            EscStatusContext[esc_idx].is_success = true;
                        }
                    }
                }
                /* If the frame is valid, advance to decode state */
                if (EscStatusContext[esc_idx].is_success == true)
                {
                    /* Advance to decode state */
                    EscStatusContext[esc_idx].state = DECODE_STATUS;
                }
                else
                {
                    /* Reset state to first frame  */
                    EscStatusContext[esc_idx].state = FIRST_FRAME;
                    EscStatusContext[esc_idx].first_frame_can_id = 0U;
                }
            }
            break;

            case DECODE_STATUS:
            {
                /* Decode the ESC status payload */
                ach_epu_decode_esc_status(EscStatusContext[esc_idx].payload, &EscStatus[esc_idx]);
                /* Reload the timer upon successful parsing */
                timer_reload(&EscStatusMon[esc_idx]);

                /* Reset the state to FIRST_FRAME */
                EscStatusContext[esc_idx].state = FIRST_FRAME;
                EscStatusContext[esc_idx].first_frame_can_id = 0U;
                EscStatusContext[esc_idx].is_success = false;

                break;
            }

            default:
            {
                EscStatusContext[esc_idx].state = FIRST_FRAME;
                EscStatusContext[esc_idx].first_frame_can_id = 0U;
                break;
            }
            }

            /* Clear the CAN message slot */
            util_memset(&CanRxBuffer[esc_idx][frame_count], 0, sizeof(can_msg_t));
        }

        frame_count++;
    }
}

/**
 * @brief Verifies the sanity of a CAN frame based on its message ID, tail byte, and parsing state.
 *
 * This function checks if the provided CAN message ID falls within the expected range for ESC status messages.
 * It also validates the tail byte according to the current frame's parsing state (first, second, or third frame)
 * by inspecting the start of transfer, end of transfer, and toggle bits as per the multi-frame transfer protocol.
 *
 * @param can_msg_id  The CAN message ID to verify.
 * @param tail        The tail byte containing transfer control bits.
 * @param state       The current parsing state (e.g., FIRST_FRAME, SECOND_FRAME, THIRD_FRAME).
 *
 * @return true if the CAN frame is valid and meets all protocol requirements for the given state, false otherwise.
 */
static bool ach_epu_verify_can_frame_sanity(uint32_t can_msg_id, uint8_t tail, e_esc_parse_state_t state)
{
    /* Check validity */
    bool is_valid = true;

    /* Check if the CAN message ID is in the expected range */
    is_valid &= ((can_msg_id >= ESC_1_STATUS_CAN_ID) &&
                 (can_msg_id <= ESC_8_STATUS_CAN_ID));

    /*
      Check for the following tail byte characteristics:

       1. For Multi frame transfer, the start of tx bit is 1 if the current frame
          is the first frame of the packet, and 0 otherwise.

       2. For Multi frame transfer, the end of tx bit is 1 if the current frame is the
          last frame of the packet, and 0 otherwise.

       3. For Multi frame transfer, the Toggle bit is 0 for the first frame
          of the packet and is flipped once for each subsequent frame
    */

    if (is_valid == true)
    {
        /* Check if the CAN message ID is valid for the first frame */
        if (state == FIRST_FRAME)
        {
            /* inspect first frame */
            is_valid &= ((tail & TM_UAVCAN_TAIL_START_TRANSFER) != 0U) &&
                        ((tail & TM_UAVCAN_TAIL_END_TRANSFER) == 0U) &&
                        ((tail & TM_UAVCAN_TAIL_TOGGLE) == 0U);
        }
        else if (state == SECOND_FRAME)
        {
            /* inspect second frame */
            is_valid &= ((tail & TM_UAVCAN_TAIL_START_TRANSFER) == 0U) &&
                        ((tail & TM_UAVCAN_TAIL_END_TRANSFER) == 0U) &&
                        ((tail & TM_UAVCAN_TAIL_TOGGLE) != 0U);
        }
        else if (state == THIRD_FRAME)
        {
            /* inspect second frame */
            is_valid &= ((tail & TM_UAVCAN_TAIL_START_TRANSFER) == 0U) &&
                        ((tail & TM_UAVCAN_TAIL_END_TRANSFER) != 0U) &&
                        ((tail & TM_UAVCAN_TAIL_TOGGLE) == 0U);
        }
    }

    return is_valid;
}

/**
 * @brief Decodes ESC (Electronic Speed Controller) status frame from payload.
 *
 * This function parses a raw payload buffer containing ESC status information and populates
 * the provided s_esc_status_frame_t structure with decoded values. The payload is expected
 * to contain status bits, voltage, current, temperature, RPM, throttle, and index fields
 * in a specific byte layout.
 *
 * @param[in]  payload  Pointer to the raw payload buffer (must not be NULL).
 * @param[out] esc      Pointer to the ESC status frame structure to populate (must not be NULL).
 *
 * @note The function assumes the payload buffer is at least 14 bytes long and fields are
 *       packed in little-endian format. Conversion from float16 to float is performed
 *       using the convert_float16_to_float() helper function.
 */
static void ach_epu_decode_esc_status(const uint8_t *payload, s_esc_status_frame_t *esc)
{
    if ((payload != NULL) && (esc != NULL))
    {
        /* Gather the status byte */
        esc->status_bits.raw_status = (uint32_t)((uint32_t)payload[0] |
                                                 ((uint32_t)payload[1] << 8) |
                                                 ((uint32_t)payload[2] << 16) |
                                                 ((uint32_t)payload[3] << 24));

        /* Extract and convert voltage, current, temperature */
        esc->voltage_f16 = convert_float16_to_float((uint16_t)payload[4] | ((uint16_t)payload[5] << 8));
        esc->current_f16 = convert_float16_to_float((uint16_t)payload[6] | ((uint16_t)payload[7] << 8));

        /* Convert Kelvin to Celsius */
        esc->temperature_f16 = (float)(convert_float16_to_float((uint16_t)payload[8] | ((uint16_t)payload[9] << 8)) -
                                       273.15f);

        /* Extract RPM */
        esc->rpm = (int32_t)((int32_t)payload[10] |
                             ((int32_t)payload[11] << 8) | ((int32_t)((payload[12] >> 6) & 0x03) << 16));

        /* Extract throttle */
        esc->throttle = (uint8_t)(((payload[12] & 0x3FU) << 1) | ((payload[13] & 0x80U) >> 7));

        /* Extract index */
        esc->index = (uint8_t)((payload[13] & 0x7CU) >> 2);
    }
}

/**
 * @brief Sends raw control commands to ESCs (Electronic Speed Controllers) via CAN bus.
 *
 * This function prepares and transmits a full raw command to up to 8 motors by:
 *  - Converting normalized motor command values to 16-bit crude commands.
 *  - Converting 16-bit commands to 14-bit raw throttle data.
 *  - Calculating CRC for message integrity.
 *  - Packing the data into three CAN frames, each with a tail byte for message sequencing.
 *  - Sending the frames sequentially over the CAN bus.
 *
 * The function ensures compliance with MISRA C:2012 Rule 18.8 by avoiding variable-length arrays.
 * The transfer ID (tf_id) is incremented with each call and wraps around after 31.
 *
 * @param[in] motor Pointer to a structure containing normalized motor command values.
 */
static void ach_epu_raw_ctrl_cmd(const std_epu_cmd_t *motor)
{
    // fixme: replace next line because of MISRA C:2012 Rule 18.8(required): Variable-length array
    // types shall not be used.
    uint16_t crude_cmd[8]; // 16 bit (unconverted) command values (max 8 motors)
    // fixme: replace next line because of MISRA C:2012 Rule 18.8(required): Variable-length array
    // types shall not be used.
    uint8_t raw_cmd[14U];     // converted command values 16 bit to 14 bit conversion
    static uint8_t tf_id = 0; // Transfer ID (tail byte) [0...31] remains same for a full raw command

    // init with zeros
    (void)util_memset(raw_cmd, 0, sizeof(raw_cmd));

    // set all motor rpm
    for (uint8_t i = 0; i < NUM_ESCS; i++)
    {
        crude_cmd[i] = (uint16_t)truncf((float)ESC_RAW_MAX * (sat(motor->motor_cmd_cval[i], 0.0f, 1.0f)));
    }

    // 16 bit to 14 bit conversion:
    convertToRawThrottleData(&crude_cmd[0], (int)NUM_ESCS, &raw_cmd[0]); // first 4 motors

    // Signature to CrC
    uint16_t SignCrc = crcAddSignature(0xFFFF, ESC_RAW_CMD_SIGN);
    uint16_t CrC = crcAdd(SignCrc, &raw_cmd[0], (size_t)14U);

    // prepare CAN message/frame:
    can_msg_t frame_1 = {0}; // 1. frame of full raw cmd
    can_msg_t frame_2 = {0}; // 2. frame of full raw cmd
    can_msg_t frame_3 = {0}; // 3. end of full msg
    // pre-set knwon values:

    uint32_t ID = createID_field(HIGH_PRIORITY, ESC_RAW_ID, SOLONE_CAN_ID);
    frame_1.can_msg_id = ID;
    frame_2.can_msg_id = ID;
    frame_3.can_msg_id = ID;

    frame_1.extended_id_flag = true;
    frame_2.extended_id_flag = true;
    frame_3.extended_id_flag = true;

    frame_1.is_remote_req = false;
    frame_2.is_remote_req = false;
    frame_3.is_remote_req = false;

    frame_1.dlc = 8; // by default a full message
    frame_2.dlc = 8; // by default a full message
    frame_3.dlc = 3; // last 2 bytes + Tail byte message

    // create taylbyte     bits: start |  end  | toggle
    uint8_t tb_1 = createTailbyte(true, false, false, tf_id);
    uint8_t tb_2 = createTailbyte(false, false, true, tf_id);
    uint8_t tb_3 = createTailbyte(false, true, false, tf_id);

    int pl_cnt = 0; // payload counter

    // prepare 1. msg:
    // (void) memcpy(&frame_1.data[0], &CrC, sizeof(uint16_t));  // 2 byte
    (void)util_memcpy((void *)&frame_1.data[0], (const void *)&CrC, sizeof(uint16_t));
    (void)util_memcpy(&frame_1.data[2], &raw_cmd[pl_cnt], 5U * sizeof(uint8_t)); // 5 byte
    (void)util_memcpy(&frame_1.data[7], &tb_1, sizeof(uint8_t));                 // 1 byte (Tail byte)

    pl_cnt += 5; // == 5

    // prepare msg 2:
    (void)util_memcpy(&frame_2.data[0], &raw_cmd[pl_cnt], 7U * sizeof(uint8_t)); // 7 byte
    (void)util_memcpy(&frame_2.data[7], &tb_2, sizeof(uint8_t));                 // 1 byte (Tail byte)

    pl_cnt += 7; // == 12

    // prepare msg 3:
    (void)util_memcpy(&frame_3.data[0], &raw_cmd[pl_cnt], 2U * sizeof(uint8_t)); // 2 byte
    (void)util_memcpy(&frame_3.data[2], &tb_3, sizeof(uint8_t));                 // 1 byte (Tail byte)

    // Send Each frame:
    int res_1 = (int)can_write(CAN_EPU, &frame_1);
    int res_2 = (int)can_write(CAN_EPU, &frame_2);
    int res_3 = (int)can_write(CAN_EPU, &frame_3);

    if ((res_1 != 0) || (res_2 != 0) || (res_3 != 0))
    {
        // printf("CAN TX Error: \r\n");
    }

    tf_id++;
    // reset tf_id
    if (tf_id > 31U)
    {
        tf_id = 0;
    }
}

/**
 * @brief Converts a 16-bit half-precision floating point value to a 32-bit single-precision float
 *
 * This function performs IEEE 754 half-precision (binary16) to single-precision (binary32)
 * floating point conversion. It handles all special cases including:
 * - Subnormal numbers (denormalized values)
 * - Infinity and NaN values
 * - Normal numbers
 *
 * @param value 16-bit half-precision floating point value in IEEE 754 binary16 format
 *              Bit layout: [15] sign, [14:10] exponent, [9:0] mantissa
 *
 * @return 32-bit single-precision floating point value equivalent to the input
 *
 * @note The function assumes FP32_t is a union type that allows access to both
 *       float (.f) and uint32_t (.u) representations of the same memory location
 *
 * @details Conversion process:
 *          - Sign bit: Extended from bit 15 to bit 31
 *          - Exponent: Adjusted from 5-bit biased by 15 to 8-bit biased by 127
 *          - Mantissa: Extended from 10 bits to 23 bits with zero padding
 *          - Special handling for subnormal numbers using normalization
 */
static float convert_float16_to_float(uint16_t value)
{
    FP32_t out;
    uint32_t sign = (uint32_t)(value & 0x8000U) << 16;
    uint32_t exponent = (uint32_t)(value & 0x7C00U) >> 10;
    uint32_t mantissa = (uint32_t)(value & 0x03FFU);
    out.u = sign;
    if (exponent == 0U)
    {
        if (mantissa != 0U)
        {
            uint32_t shift = 0U;
            while ((mantissa & 0x0400U) == 0U)
            {
                mantissa <<= 1;
                shift++;
            }
            mantissa &= 0x03FFU;
            exponent = 113U - shift;
        }
    }
    else if (exponent == 31U)
    {
        out.u |= (255UL << 23) | (mantissa << 13);
        return out.f;
    }
    else
    {
        exponent += 112U;
    }
    out.u |= (exponent << 23) | (mantissa << 13);
    return out.f;
}

/**
 * @brief Adds a single byte to a CRC-16 calculation using CCITT polynomial
 *
 * This function implements the CRC-16-CCITT algorithm to update the current
 * CRC value with a new byte. The function uses the polynomial 0x1021 and
 * processes the byte bit by bit using shift operations.
 *
 * @param crc_val Current CRC value to be updated
 * @param byte    Single byte to be added to the CRC calculation
 *
 * @return Updated CRC-16 value after processing the input byte
 *
 * @note This function implements the standard CRC-16-CCITT algorithm
 * @note The polynomial used is 0x1021 (x^16 + x^12 + x^5 + 1)
 */
static uint16_t crcAddByte(uint16_t crc_val, uint8_t byte)
{
    crc_val ^= (uint16_t)((uint16_t)(byte) << 8);
    for (int j = 0; j < 8; j++)
    {
        if ((crc_val & 0x8000U) != 0U)
        {
            crc_val = (uint16_t)((uint16_t)(crc_val << 1) ^ 0x1021U);
        }
        else
        {
            crc_val = (uint16_t)(crc_val << 1);
        }
    }
    return crc_val;
}

/**
 * @brief Calculates CRC-16 checksum over a byte array
 *
 * This function computes a CRC-16 checksum by iterating through each byte
 * in the provided array and updating the CRC value using the crcAddByte helper.
 *
 * @param crc_val Initial CRC value or previously computed CRC to continue calculation
 * @param bytes   Pointer to the byte array to process
 * @param len     Number of bytes in the array to process
 *
 * @return Updated CRC-16 value after processing all bytes
 *
 * @note The function safely handles zero-length arrays
 * @note Depends on crcAddByte() function for single-byte CRC calculation
 */
static uint16_t crcAdd(uint16_t crc_val, const uint8_t *bytes, size_t len)
{
    while ((len--) != (size_t)0U)
    {
        crc_val = crcAddByte(crc_val, *bytes++);
    }
    return crc_val;
}

/**
 * @brief Adds a 64-bit data type signature to CRC calculation by processing it byte by byte
 *
 * This function takes a 64-bit signature and processes it in 8-byte chunks (LSB first),
 * adding each byte to the CRC calculation using the crcAddByte function.
 *
 * @param crc_val Current CRC value to be updated
 * @param data_type_signature 64-bit signature to be added to the CRC calculation
 *
 * @return Updated CRC value after processing the signature
 *
 * @note The signature is processed starting from the least significant byte (LSB)
 * @note This function assumes crcAddByte() is available and properly implemented
 */
static uint16_t crcAddSignature(uint16_t crc_val, uint64_t data_type_signature)
{
    for (uint8_t shift_val = 0; shift_val < 64U; shift_val = (uint8_t)(shift_val + 8U))
    {
        crc_val = crcAddByte(crc_val, (uint8_t)(data_type_signature >> shift_val));
    }
    return crc_val;
}

/**
 * @brief Converts an array of 16-bit throttle values to a packed raw data format
 *
 * This function takes an array of 14-bit throttle values (stored in 16-bit containers)
 * and packs them into a continuous bit stream within the output byte array. Each 14-bit
 * value is split into its lower 8 bits and upper 6 bits, then packed sequentially
 * bit-by-bit into the output buffer.
 *
 * The packing format stores bits in MSB-first order within each byte, with the lower
 * 8 bits of each input value packed first, followed by the upper 6 bits.
 *
 * @param input   Pointer to array of 16-bit values containing 14-bit throttle data
 * @param count   Number of 14-bit values to convert from the input array
 * @param output  Pointer to output byte array where packed raw data will be stored.
 *                Must be pre-allocated with sufficient size to hold (count * 14) bits
 *
 * @note The output array must be pre-zeroed or properly initialized before calling
 *       this function, as the function performs bit-level operations.
 * @note Only the lower 14 bits of each input value are used; upper 2 bits are ignored.
 *
 * @warning No bounds checking is performed. Caller must ensure output buffer is
 *          large enough to accommodate the packed data.
 */
static void convertToRawThrottleData(const uint16_t *input, int count, uint8_t *output)
{
    uint8_t bitIndex = 0; // Start at the first bit

    // Iterate over each 14-bit value
    for (int i = 0; i < count; i++)
    {
        uint16_t value = input[i];

        // Split the 14-bit value into two parts: res1 and res2
        uint8_t res1 = (uint8_t)(((uint8_t)(value >> 8)) & ((uint8_t)0x3F)); // Top 6 bits (since it's a 14-bit value)
        uint8_t res2 = (uint8_t)(value & ((uint8_t)0xFF));                   // Lower 8 bits

        // Pack res1 and res2 into the data array as 14 bits
        for (uint8_t j = 0; j < 14U; j++)
        {
            uint8_t bitPos = (uint8_t)(bitIndex + j);
            uint8_t byteIndex = bitPos / 8U;
            uint8_t bitInByte = (uint8_t)(7U - (bitPos % 8U));

            uint8_t bit;
            if (j < 8U)
            {
                bit = (res2 >> (7U - j)) & (uint8_t)1U; // Extract bit from res2
            }
            else
            {
                bit = (res1 >> (13U - j)) & (uint8_t)1U; // Extract bit from res1 (only 6 bits are valid)
            }

            // Set the bit in the data array
            output[byteIndex] &= (uint8_t)(~(1U << bitInByte)); // Clear the bit first
            output[byteIndex] |= (uint8_t)(bit << bitInByte);   // Set the new bit
        }

        // Move to the next set of 14 bits
        bitIndex = (uint8_t)(bitIndex + 14U);
    }
}

/**
 * @brief Creates a CAN 2.0B extended format identifier field
 *
 * This function constructs a 32-bit CAN identifier by combining priority,
 * message ID, and source ID fields according to CAN 2.0B extended format.
 * The function automatically sets the start of frame indicator (bit 31) and
 * ensures the source ID's highest bit is cleared for compliance.
 *
 * @param prio      CAN message priority (8-bit value, positioned in bits 24-31)
 * @param msg_id    Message identifier (16-bit value, positioned in bits 8-23)
 * @param source_id Source node identifier (7-bit effective value, positioned in bits 0-6)
 *                  Note: Bit 7 is automatically masked to 0
 *
 * @return uint32_t Complete CAN 2.0B extended identifier with:
 *                  - Bit 31: Start of frame indicator (always 1)
 *                  - Bits 24-30: Priority field
 *                  - Bits 8-23: Message ID field
 *                  - Bits 0-6: Source ID field
 *                  - Bit 7: Reserved (always 0)
 */
static uint32_t createID_field(can_priority_t prio, uint16_t msg_id, uint8_t source_id)
{
    uint32_t id = 0; // Initialize ID to zero

    // Ensure the highest bit (bit 7) of source_id is 0 by masking with 0x7F (01111111 in binary)
    source_id &= (uint8_t)0x7F;

    // Position each part in the correct byte by shifting and use bitwise OR to assemble the ID
    id |= (uint32_t)source_id;   // Source ID is already in the correct position (low byte)
    id |= (uint32_t)msg_id << 8; // Shift message ID left by 8 bits to place it in byte 1-2
    id |= (uint32_t)prio << 24;  // Shift priority left by 24 bits to place it in the high byte
    id = id | (uint32_t)(1UL
                         << 31U); // Start of frame indicator according to CAN 2.0B extension version

    return id;
}

/**
 * @brief Creates a tail byte for transmission protocol
 *
 * This function constructs a tail byte by combining transmission control flags
 * and a transfer ID into a single 8-bit value. The tail byte format is:
 * - Bits 0-4: Transfer ID (tf_id)
 * - Bit 5: Toggle bit
 * - Bit 6: Transmission end flag (tx_end)
 * - Bit 7: Transmission start flag (tx_start)
 *
 * @param tx_start Boolean flag indicating start of transmission (placed in bit 7)
 * @param tx_end Boolean flag indicating end of transmission (placed in bit 6)
 * @param toggle Boolean toggle bit for frame sequencing (placed in bit 5)
 * @param tf_id Transfer ID value (0-31, placed in bits 0-4)
 *
 * @return uint8_t The constructed tail byte with all flags and ID encoded
 *
 * @note If tf_id exceeds 31, an error message is printed but execution continues
 * @warning No validation is performed on the tf_id parameter beyond printing an error
 */
static uint8_t createTailbyte(bool tx_start, bool tx_end, bool toggle, uint8_t tf_id)
{
    if (tf_id > 31U)
    {
        printf("Error tf id must be < 31 (but rec.: %d) \r\n", tf_id);
    }
    uint8_t tail_byte = tf_id & (uint8_t)0x1F;

    tail_byte |= (uint8_t)((uint8_t)toggle << 5);
    tail_byte |= (uint8_t)((uint8_t)tx_end << 6);
    tail_byte |= (uint8_t)((uint8_t)tx_start << 7);

    return tail_byte;
}