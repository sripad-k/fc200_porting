/****************************************************
 *  da_radalt.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_radalt
 *  Copyright: LODD (c) 2025
 ****************************************************/
#include "da_radalt.h"
#include "generic_util.h"
#include "timer_interface.h"

/* Antenna Distance to Ground in meters */
#define RADALT_OFFSET (0.014f)

/* Minimum SNR value to consider the data valid */
#define RADALT_SNR_THRESHOLD (13.0f)

/* Double the size of message struct to make sure at least 1 radalt message gets captured */
#define RADALT_BUFFER_SIZE (2 * sizeof(us_d1_msg_s))

/* Correspond to number of cycles to consider the radalt alive */
#define CNT_RADALT_TRESHOLD (100)

#define PACKET_HEAD_SIGN (0xFE)

#define VERSION_ID_SIGN (0x02)

#define RADALT_TIMEOUT (60)

typedef enum
{
    PACKET_HEAD,
    VERSION_ID,
    ALTITUDE_LSB,
    ALTITUDE_MSB,
    SNR,
    CHECKSUM
} radalt_deframe_states_t;

/* private function prototypes */
static void da_radalt_parse_data(radalt_msg_s *msg, const uint8_t *ptr_byte);
static void da_radalt_process_data(void);
static void da_radalt_msg_decode(radalt_s *ptr_radalt, const radalt_msg_s *msg);

static radalt_s radalt;

static s_timer_data_t RadaltMonitorTimer;
static bool RadaltCommTimeout;

/**
 * @brief Actual message structs
 */
static radalt_msg_s radalt_msg;

/**
 * @brief Radalt read buffer
 */
static uint8_t radalt_recv_packet[RADALT_BUFFER_SIZE] = {0};

/**
 * @brief Retrieves the radar altimeter data.
 *
 * @param agl Pointer to a float variable where the altitude above ground level (AGL) will be stored.
 * @return true if the SNR value is within threshold, false otherwise.
 */
bool da_get_radalt_data(float *agl)
{
    bool status = false;
    /* Validate the message if Checksum is verified OK,
       otherwise retain the last good value */
    if (radalt.flag == true)
    {
        /* Updated values are copied */
        *agl = radalt.agl;
        status = true;
    }
    return status;
}

/**
 * @brief Retrieves the status of the radar altimeter communication timeout.
 *
 * This function returns the current value of the RadaltCommTimeout flag,
 * indicating whether a timeout has occurred in the radar altimeter communication.
 *
 * @return true if a radar altimeter communication timeout has occurred, false otherwise.
 */
bool da_get_radalt_timeout(void)
{
    return RadaltCommTimeout;
}

/* Operation 'da_radalt_init' of Class 'DA_radalt' */
bool da_radalt_init(void)
{
    /* SyncableUserCode{6DDE5974-58DF-47fa-85E7-57F176257FB5}:Nbrlk8aPUZ */
	bool radalt_init_success = false ;

    /* Set the RADALT Offset */
    radalt.offset = RADALT_OFFSET;

    /* Radalt timeout set to False initially*/
    RadaltCommTimeout = false;
    /* Start the timer */
    timer_start(&RadaltMonitorTimer, RADALT_TIMEOUT);

    /* Initialise the UART RADALT Channel */
    radalt_init_success = uart_init(UART_RADALT);

	/* If successful */
	if(true == radalt_init_success)
	{
		/* Send a message indicating ONLINE from the same channel */
		uart_write(UART_RADALT, (uint8_t *)"RADALT ONLINE\r\n", 18);
	}

	return radalt_init_success;

    /* SyncableUserCode{6DDE5974-58DF-47fa-85E7-57F176257FB5} */
}

/* Operation 'da_radalt_read_periodic' of Class 'DA_radalt' */
void da_radalt_read_periodic(void)
{

    /* SyncableUserCode{D95893C1-FCFA-40f7-A5DF-8FB31C0A0A01}:Nbrlk8aPUZ */
    da_radalt_process_data();
    /* SyncableUserCode{D95893C1-FCFA-40f7-A5DF-8FB31C0A0A01} */
}

/* Operation 'da_radalt_process_data' of Class 'DA_radalt' */
static void da_radalt_process_data(void)
{

    /* SyncableUserCode{64A23C09-6281-48f2-9446-ADC1EF2EBAAB}:Nbrlk8aPUZ */
    // Check if the status counter has reached the threshold value
    if (radalt.counter >= CNT_RADALT_TRESHOLD)
    {
        if (radalt.status == true)
        {
            radalt.status = false;
            // INFO("RADALT is dead!");
        }
    }
    else
    {
        if (radalt.status == false)
        {
            radalt.status = true;
            // INFO("RADALT is alive!");
        }
    }

    // Increment counter for monitoring radalt alive status
    if (radalt.counter < UINT8_MAX) // Prevent counter overflow
    {
        radalt.counter++;
    }

    // Make sure that message structure is reset
    util_memset(&radalt_msg, 0, sizeof(us_d1_msg_s));

    uint16_t bytes_read = uart_read((uint8_t)UART_RADALT, &radalt_recv_packet[0], RADALT_BUFFER_SIZE);

    if (bytes_read > 0)
    {
        for (uint16_t i = 0; i < bytes_read; i++)
        {
            da_radalt_parse_data(&radalt_msg, &radalt_recv_packet[i]);

            if (radalt_msg.flag == RADALT_DCODE_CRC_OK)
            {
                da_radalt_msg_decode(&radalt, &radalt_msg);

                // Reset message flag
                radalt_msg.flag = 0;

                // Reset radalt status counter
                radalt.counter = 0;

                return;
            }
            else
            {
                switch (radalt_msg.flag)
                {
                case RADALT_DCODE_FAILED_START:
                    // INFO("Start byte fail!");
                    break;
                case RADALT_DCODE_FAILED_VERSION:
                    // INFO("Version fail!");
                    break;
                case RADALT_DCODE_FAILED_CRC:
                    // INFO("CRC fail!");
                    break;
                case RADALT_DCODE_FAILED_ERROR:
                    // INFO("Generic fail!");
                    break;
                default:
                    break;
                }
            }
        }
    }

    /* Check for RADALT timeout counter */
    RadaltCommTimeout = timer_check_expiry(&RadaltMonitorTimer);

    return;
    /* SyncableUserCode{64A23C09-6281-48f2-9446-ADC1EF2EBAAB} */
}

/**
 * @brief Processes a byte stream to decode messages from a radar altimeter.
 *
 *  @param msg Pointer to radalt_msg_s structure where the message data will be stored.
 * @param ptr_byte Pointer to the current byte in the byte stream being processed.
 */
static void da_radalt_parse_data(radalt_msg_s *msg, const uint8_t *ptr_byte)
{

    /* SyncableUserCode{D30B6B8D-CD96-4839-B73E-AA5EC3FEC7DB}:Nbrlk8aPUZ */
    static radalt_deframe_states_t state = PACKET_HEAD; // counter for switch state

    switch (state)
    {
    case PACKET_HEAD:
    {
        if (PACKET_HEAD_SIGN == *ptr_byte)
        {
            /* Copy the Header */
            msg->us_d1_msg.header = *ptr_byte;
            msg->flag = RADALT_DCODE_PENDING;
            state = VERSION_ID;
        }
        else
        {
            msg->flag = RADALT_DCODE_FAILED_START;
        }
        break;
    }

    case VERSION_ID:
    {
        if (VERSION_ID_SIGN == *ptr_byte)
        {
            /* Copy the version ID */
            msg->us_d1_msg.version_id = *ptr_byte;
            state = ALTITUDE_LSB;
        }
        else
        {
            state = PACKET_HEAD;
            msg->flag = RADALT_DCODE_FAILED_VERSION;
        }
        break;
    }

    case ALTITUDE_LSB:
    {
        /* Copy the altitude LSB */
        msg->us_d1_msg.altitude_lsb = *ptr_byte;
        state = ALTITUDE_MSB;
        break;
    }

    case ALTITUDE_MSB:
    {
        /* Copy the altitude MSB */
        msg->us_d1_msg.altitude_msb = *ptr_byte;
        state = SNR;
        break;
    }

    case SNR:
    {
        /* Copy the SNR */
        msg->us_d1_msg.snr = *ptr_byte;
        state = CHECKSUM;
        break;
    }

    case CHECKSUM:
    {
        msg->us_d1_msg.checksum = *ptr_byte;

        // Checksum control
        // According to User Manual D00.02.05
        uint8_t temp = (uint8_t)(msg->us_d1_msg.version_id + msg->us_d1_msg.altitude_lsb + msg->us_d1_msg.altitude_msb + msg->us_d1_msg.snr);
        if (msg->us_d1_msg.checksum == (temp & 0xFF))
        {
            msg->flag = RADALT_DCODE_CRC_OK;
        }
        else
        {
            msg->flag = RADALT_DCODE_FAILED_CRC;
        }

        /* Reload the timer on valid packet */
        timer_reload(&RadaltMonitorTimer);

        // either way restart switch state counter
        state = PACKET_HEAD;
        break;
    }

    default:
    {
        state = PACKET_HEAD;
        msg->flag = RADALT_DCODE_FAILED_ERROR;
        break;
    }
    }

    return;
    /* SyncableUserCode{D30B6B8D-CD96-4839-B73E-AA5EC3FEC7DB} */
}

/**
 * @brief Decodes the radar altimeter message and updates the radalt structure with the new data.
 *
 * @param radalt Pointer to the radalt_s structure where decoded data will be stored.
 * @param msg Pointer to the radalt_msg_s structure that holds the raw message data received from
 * the radar altimeter.
 */
static void da_radalt_msg_decode(radalt_s *ptr_radalt, const radalt_msg_s *msg)
{
    // New data available
    ptr_radalt->flag = true;

    // Save data Signal to Noise Ratio [dB]
    ptr_radalt->snr = (float)msg->us_d1_msg.snr;

    // Combine MSB and LSB altitude data into a uint16_t
    uint16_t combined = (uint16_t)((uint16_t)(msg->us_d1_msg.altitude_msb) << 8 | msg->us_d1_msg.altitude_lsb);

    // Save data Altitude Data [m]
    ptr_radalt->agl = ((float)combined) / 100.0f - ptr_radalt->offset;

    // Set status based on SNR value
    // User Manual D00.02.05:
    // Altitude measurements associated with a SNR value of 13dB or lower are considered erroneous
    if (ptr_radalt->snr < (float)RADALT_SNR_THRESHOLD)
    {
        ptr_radalt->status = false;
    }
    else
    {
        ptr_radalt->status = true;
    }

    return;
}
