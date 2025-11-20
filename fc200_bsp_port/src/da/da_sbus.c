/****************************************************
 *  da_sbus.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_sbus
 *  Copyright: LODD (c) 2025
 ****************************************************/
#include "da_sbus.h"
#include "uart_interface.h"
#include "timer_interface.h"
#include "generic_util.h"

#define SBUS_BUFFER_SIZE (2 * SBUS_FRAME_SIZE)
#define SBUS_HEADER_BYTE (0x0FU)
#define SBUS_END_BYTE (0x00U)
#define SBUS_END_BYTE_2 (0x04U)
#define SBUS_CH_DATA_MASK (0x07FF)
#define SBUS_CH_17_MASK (0x01)
#define SBUS_CH_18_MASK (0x02)
#define SBUS_FRAMES_LOST_MASK (0x04)
#define SBUS_FAILSAFE_MASK (0x08)
#define MAX_RC_IP_DOWN_TIME (10)     /* Link lost for 0.1 sec (Depends on superloop - 10 ms currently) */
#define SBUS_TIMEOUT (100)           /* SBUS timeout in ms */
#define SBUS_RSSI_PERIOD (1000)      /* SBUS RSSI update period in ms */
#define MAX_SBUS_FRAMES_PER_SEC (67) /* Maximum SBUS frames per second */

/* private function prototypes */
static void da_sbus_process_data(void);
static void da_sbus_parse_data(uint8_t byte);
static void da_sbus_decode(void);
static void da_sbus_map_ep_input(void);

static uint8_t Sbus_Uart_Rx_Pkt[SBUS_BUFFER_SIZE] = {0};
static uint8_t Sbus_Buffer[SBUS_FRAME_SIZE] = {0};
static rc_input_s Rc_Input;
static bool Frame_Ready = false;
static s_timer_data_t SbusMonitorTimer;
static s_timer_data_t SbusRssiTimer;
static uint32_t RssiCounter = 0;
static bool SbusCommTimeout = false;
static rc_input_t RcInputEP = {0};

/**
 * @brief Retrieves the latest decoded SBUS frame if available.
 *
 * This function checks if a new SBUS frame has been successfully decoded.
 * If a new frame is available, it copies the decoded frame data to the
 * provided pointer and resets the frame ready flag. The function then
 * returns the status of the frame ready flag.
 *
 * @param[out] ptr_sbus_frame Pointer to a structure where the latest SBUS
 * frame will be copied if available.
 *
 * @return true if a new frame was available before the call, false otherwise.
 */
bool da_get_sbus_latest_frame(rc_input_s *ptr_sbus_frame)
{
    bool status = false;
    /* If last received frame is successfully decoded */
    if (Frame_Ready)
    {
        /* Copy the decoded frame to the pointer */
        *ptr_sbus_frame = Rc_Input;
        /* return true is frame is ready */
        status = true;
        /* Reset the frame ready flag */
        Frame_Ready = false;
    }
    /* Return the frame ready status */
    return status;
}

/**
 * @brief Retrieves the current status of the SBUS communication timeout flag.
 *
 * This function returns the value of the SbusCommTimeout flag, which indicates
 * whether a timeout has occurred in the SBUS communication.
 *
 * @return true if an SBUS communication timeout has occurred, false otherwise.
 */
bool da_get_sbus_timeout(void)
{
    return SbusCommTimeout;
}

bool da_get_ep_data(rc_input_t *rc_input)
{
    bool status = false;
    /* Check if the pointer is valid */
    if ((rc_input != NULL) && (Frame_Ready))
    {
        /* Copy the RC Input data to the pointer */
        *rc_input = RcInputEP;
        status = true;
    }

    return status;
}

/**
 * @brief Initializes the SBUS UART interface.
 *
 * This function initializes the UART interface used for SBUS communication.
 * It calls the uart_init function with the UART_SBUS parameter to set up
 * the required UART hardware for SBUS protocol.
 *
 * @return true if the UART initialization was successful, false otherwise.
 */
/* Operation 'da_sbus_init' of Class 'DA_sbus' */
bool da_sbus_init(void)
{

    /* SyncableUserCode{BE749DAE-8F50-4cf8-BD16-475B6BE67B3C}:Nbrlk8aPUZ */
	bool sbus_init_success = false ;
    /* Initialize the communication timeout */
    SbusCommTimeout = false;
    /* Start the timer */
    timer_start(&SbusMonitorTimer, SBUS_TIMEOUT);
    timer_start(&SbusRssiTimer, SBUS_RSSI_PERIOD);

    /* Init S-BUS UART */
    sbus_init_success = uart_init(UART_SBUS);

	/* If successful */
	if(true == sbus_init_success)
	{
		/* Send a message indicating ONLINE from the same channel */
		uart_write(UART_SBUS, (uint8_t *)"SBUS ONLINE\r\n", 16);
	}

    return(sbus_init_success);

    /* SyncableUserCode{BE749DAE-8F50-4cf8-BD16-475B6BE67B3C} */
}

/**
 * @brief Periodically reads and processes SBUS data.
 *
 * This function is intended to be called at regular intervals to handle
 * the periodic reading and processing of SBUS data. It delegates the
 * actual data processing to the da_sbus_process_data() function.
 *
 * @note This function is part of the DA_sbus class operations.
 */
/* Operation 'da_sbus_read_periodic' of Class 'DA_sbus' */
void da_sbus_read_periodic(void)
{
    /* SyncableUserCode{DF6A33C1-E9AE-4bf6-B1B9-2F7D56FA0794}:Nbrlk8aPUZ */
    da_sbus_process_data();
    /* SyncableUserCode{DF6A33C1-E9AE-4bf6-B1B9-2F7D56FA0794} */
}

/**
 * @brief Processes incoming SBUS data from the UART interface.
 *
 * This function reads SBUS data from the UART, parses each received byte,
 * and updates the RC input status. It also monitors the SBUS link status,
 * declaring it as bad if no data is received for a specified timeout period.
 *
 * Operations performed:
 * - Reads available SBUS data from the UART interface.
 * - Increments the SBUS counter to track missing or unsuccessful packets.
 * - Parses each received byte using the SBUS parser.
 * - Checks if the SBUS link is down based on the counter and updates link status.
 *
 * @note This function should be called periodically to ensure timely processing
 *       of SBUS data and accurate link status monitoring.
 */
/* Operation 'da_sbus_process_data' of Class 'DA_sbus' */
static void da_sbus_process_data(void)
{
    /* SyncableUserCode{AB88EF54-29A7-42d3-AC3A-B8A789BB1913}:Nbrlk8aPUZ */
    uint16_t bytes_read;
    float rssi_value = 0.0f;

    /* Read the SBUS data from UART */
    bytes_read = uart_read(UART_SBUS, &Sbus_Uart_Rx_Pkt[0], SBUS_BUFFER_SIZE);

    /* Increment the counter to keep track of */
    Rc_Input.Sbus_Counter++;

    /* If bytes are read in the current cycle */
    if (bytes_read > 0)
    {

        /* Parse all the read bytes */
        for (uint16_t byte_index = 0; byte_index < bytes_read; ++byte_index)
        {
            da_sbus_parse_data(Sbus_Uart_Rx_Pkt[byte_index]);
        }
    }

    /* If the RC Input is not received for 100 ms */
    if (Rc_Input.Sbus_Counter > MAX_RC_IP_DOWN_TIME)
    {
        /* Then declare link status is bad */
        Rc_Input.Sbus_Link = false;
    }

    /* Check for timer expiry */
    SbusCommTimeout = timer_check_expiry(&SbusMonitorTimer);

    if (timer_check_expiry(&SbusRssiTimer))
    {
        /* Reset the SBUS RSSI timer */
        timer_reload(&SbusRssiTimer);
        /* Calculate the RSSI value based on the number of frames received */
        rssi_value = (float)((RssiCounter * 100.0f) / MAX_SBUS_FRAMES_PER_SEC);
        /* Clamp the RSSI value to the range [0, 100] */
        if (rssi_value > 100.0f)
        {
            RcInputEP.rssi = 100.0f;
        }
        else
        {
            RcInputEP.rssi = rssi_value;
        }
        /* Reset the RSSI counter */
        RssiCounter = 0;
        // printf("SBUS RSSI Counter = %d ", RssiCounter);
        // printf("RSSI: %.2f %% \r\n", RcInputEP.rssi);
    }

    da_sbus_map_ep_input();
    /* Reset the SBUS UART RX Buffer */
    util_memset(Sbus_Uart_Rx_Pkt, 0, SBUS_BUFFER_SIZE);
    /* SyncableUserCode{AB88EF54-29A7-42d3-AC3A-B8A789BB1913} */
}

/**
 * @brief Parses incoming SBUS data byte-by-byte to detect and process complete SBUS frames.
 *
 * This function should be called for each received SBUS byte. It detects the start of a new SBUS frame
 * by checking for the SBUS header byte, accumulates bytes into a buffer, and upon receiving a complete
 * frame (of size SBUS_FRAME_SIZE), it verifies the end byte and triggers frame decoding.
 *
 * @param byte The incoming SBUS data byte to be parsed.
 *
 * @note This function maintains internal static state to track frame boundaries and buffer index.
 *       It calls da_sbus_decode() when a valid SBUS frame is received.
 */
/* Operation 'da_sbus_parse_data' of Class 'DA_sbus' */
static void da_sbus_parse_data(uint8_t byte)
{
    /* SyncableUserCode{DC7753CD-5354-479d-BA44-5097D3F5DD16}:Nbrlk8aPUZ */
    static bool new_frame_detected = false;
    static uint8_t Sbus_Index = 0;

    /* Start of new frame sync */
    if (false == new_frame_detected)
    {
        /* Check for start of frame */
        if (byte == SBUS_HEADER_BYTE)
        {
            /* Reset Index to 0 */
            Sbus_Index = 0;
            /* Copy the header byte */
            Sbus_Buffer[Sbus_Index++] = byte;
            /* Set new frame detected flag */
            new_frame_detected = true;
        }
    }
    else
    {
        /* Copy the received byte to buffer */
        Sbus_Buffer[Sbus_Index++] = byte;

        /* if the buffer is full of a frame */
        if (Sbus_Index >= SBUS_FRAME_SIZE)
        {
            /* Reset new frame detected flag */
            new_frame_detected = false;
            /* Check if the end byte is valid */
            if ((Sbus_Buffer[24] == SBUS_END_BYTE) || ((Sbus_Buffer[24] & 0x0FU) == SBUS_END_BYTE_2))
            {
                /* Decode the received data */
                da_sbus_decode();
            }
            else
            {
            	printf("SBUS Fail\r\n");
            }
            /* Always reset index after full frame */
            Sbus_Index = 0;
        }
    }

    /* SyncableUserCode{DC7753CD-5354-479d-BA44-5097D3F5DD16} */
}

/**
 * @brief Decodes an SBUS frame from the Sbus_Buffer and updates RC input data.
 *
 * This function extracts up to 16 standard SBUS channels from the SBUS data buffer,
 * converts their values to normalized range [-1, 1], and updates the Rc_Input structure.
 * It also extracts the states of channels 17 and 18, frame lost, and failsafe flags.
 * The function maintains counters for lost frames and resets failsafe counters as needed.
 *
 * Buffer layout and bit manipulations are based on the SBUS protocol specification.
 *
 * Preconditions:
 *   - Sbus_Buffer must contain a valid SBUS frame.
 *   - SBUS_MAX_CHANNELS, SBUS_CH_DATA_MASK, and other SBUS_* macros must be defined.
 *   - Rc_Input structure must be defined and accessible.
 *
 * Side Effects:
 *   - Updates Rc_Input fields with decoded channel data and status flags.
 *   - Updates Frame_Ready flag to indicate new data is available.
 */
static void da_sbus_decode(void)
{
    uint16_t ch[SBUS_MAX_CHANNELS] = {0};

    ch[0] = ((uint16_t)Sbus_Buffer[1] | ((uint16_t)Sbus_Buffer[2] << 8)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[1] = (((uint16_t)Sbus_Buffer[2] >> 3) | ((uint16_t)Sbus_Buffer[3] << 5)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[2] = (((uint16_t)Sbus_Buffer[3] >> 6) | ((uint16_t)Sbus_Buffer[4] << 2) | ((uint16_t)Sbus_Buffer[5] << 10)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[3] = (((uint16_t)Sbus_Buffer[5] >> 1) | ((uint16_t)Sbus_Buffer[6] << 7)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[4] = (((uint16_t)Sbus_Buffer[6] >> 4) | ((uint16_t)Sbus_Buffer[7] << 4)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[5] = (((uint16_t)Sbus_Buffer[7] >> 7) | ((uint16_t)Sbus_Buffer[8] << 1) | ((uint16_t)Sbus_Buffer[9] << 9)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[6] = (((uint16_t)Sbus_Buffer[9] >> 2) | ((uint16_t)Sbus_Buffer[10] << 6)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[7] = (((uint16_t)Sbus_Buffer[10] >> 5) | ((uint16_t)Sbus_Buffer[11] << 3)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[8] = ((uint16_t)Sbus_Buffer[12] | ((uint16_t)Sbus_Buffer[13] << 8)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[9] = (((uint16_t)Sbus_Buffer[13] >> 3) | ((uint16_t)Sbus_Buffer[14] << 5)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[10] = (((uint16_t)Sbus_Buffer[14] >> 6) | ((uint16_t)Sbus_Buffer[15] << 2) | ((uint16_t)Sbus_Buffer[16] << 10)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[11] = (((uint16_t)Sbus_Buffer[16] >> 1) | ((uint16_t)Sbus_Buffer[17] << 7)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[12] = (((uint16_t)Sbus_Buffer[17] >> 4) | ((uint16_t)Sbus_Buffer[18] << 4)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[13] = (((uint16_t)Sbus_Buffer[18] >> 7) | ((uint16_t)Sbus_Buffer[19] << 1) | ((uint16_t)Sbus_Buffer[20] << 9)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[14] = (((uint16_t)Sbus_Buffer[20] >> 2) | ((uint16_t)Sbus_Buffer[21] << 6)) & (uint16_t)SBUS_CH_DATA_MASK;
    ch[15] = (((uint16_t)Sbus_Buffer[21] >> 5) | ((uint16_t)Sbus_Buffer[22] << 3)) & (uint16_t)SBUS_CH_DATA_MASK;
    // clang-format on

    for (uint8_t ch_id = 0; ch_id < SBUS_MAX_CHANNELS; ch_id++) // CHECK: Why 11 ? We have 18 channels
    {
        if ((ch[ch_id] > 100U) && (ch[ch_id] < 1900U))
        {
            // PWM in ms (needs clamping [0.8ms, 2.2ms])
            float pwm_tmp = 500.0f + (1000.0f * ((float)ch[ch_id] / 1024.0f));

            // PWM ms the Hitec servos use  Pulse Width: 900~2100μs(Center:1500μs)
            uint16_t sbus_pwm_us = (uint16_t)pwm_tmp;                       // CHECK: Fraction part is lost
            Rc_Input.Sbus[ch_id] = ((float)sbus_pwm_us - 1500.0f) / 500.0f; // [-1, 1]
            Rc_Input.Sbus[ch_id] = Rc_Input.Sbus[ch_id] < -1.0f ? -1.0f : Rc_Input.Sbus[ch_id];
            Rc_Input.Sbus[ch_id] = Rc_Input.Sbus[ch_id] > 1.0f ? 1.0f : Rc_Input.Sbus[ch_id];
        }
    }

    /* Extract Bit 0: channel 17 and Bit 1: channel 18 */
    Rc_Input.Sbus_Ch_17 = (Sbus_Buffer[23] & SBUS_CH_17_MASK) != 0;
    Rc_Input.Sbus_Ch_18 = (Sbus_Buffer[23] & SBUS_CH_18_MASK) != 0;

    /* Extract Bit 2: frame lost and Bit 3: failsafe */
    Rc_Input.Sbus_Lost_Frame = (Sbus_Buffer[23] & SBUS_FRAMES_LOST_MASK) != 0;
    Rc_Input.Sbus_Failsafe = (Sbus_Buffer[23] & SBUS_FAILSAFE_MASK) != 0;

    /* Keep a count of frames lost */
    if (Rc_Input.Sbus_Lost_Frame == true)
    {
        Rc_Input.Sbus_Frames_Lost_Counter++;
        // printf("SBUS Frame Lost: %d\r\n", Rc_Input.Sbus_Frames_Lost_Counter);
    }
    else
    {
        /* Increment the RSSI Counter Indicating no frames were lost */
        RssiCounter++;
    }

    /* if failsafe is deactivated */
    if (Rc_Input.Sbus_Failsafe == false)
    {
        /* Reset the failsafe counter */
        Rc_Input.Sbus_Counter = 0;
        /* Set the Link Status flag to indicate link is good */
        Rc_Input.Sbus_Link = true;
    }

    /* Reload SBUS timer monitor */
    timer_reload(&SbusMonitorTimer);
    /* Indicate Frame Ready */
    Frame_Ready = true;
}

static void da_sbus_map_ep_input(void)
{
    RcInputEP.link_lost = !Rc_Input.Sbus_Link; // Link lost status
    RcInputEP.data_timeout = SbusCommTimeout;  // Data timeout status

    RcInputEP.axis_r = Rc_Input.Sbus[0];         // Roll
    RcInputEP.axis_p = -1.0f * Rc_Input.Sbus[1]; // Pitch
    RcInputEP.axis_t = Rc_Input.Sbus[2];         // Throttle
    RcInputEP.axis_y = Rc_Input.Sbus[3];         // Yaw
    RcInputEP.sw_b = Rc_Input.Sbus[4];           // Mode SW
    RcInputEP.sw_c = Rc_Input.Sbus[5];           // not used
    RcInputEP.knob_r = Rc_Input.Sbus[6];         // Arm Knob
    RcInputEP.slider_r = Rc_Input.Sbus[7];       // Pusher
}
