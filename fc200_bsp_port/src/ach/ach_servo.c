/****************************************************
 *  ach_servo.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class ach_servo
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "ach_servo.h"
#include "uart_interface.h"
#include "timer_interface.h"
#include "generic_util.h"

#define EPS (0.00001f)
#define MAX_CONFIGURED_SERVO (10)
#define FRAME_HEAD (0xFE)
#define VERSION_ID (0xCA)
#define FRAME_END (0x0A)
#define BYTES_PER_MSG (10)
#define SERVO_BUFFER_SIZE (255)
#define MAX_CONTROL_SURFACE (MAX_CNTRL_SURFACE)
#define COMBINE_BYTES(lowByte, highByte) \
	((int16_t)(((uint16_t)(highByte) << 8) | ((uint16_t)(lowByte))))

typedef enum
{

	STAGE1_FRAME_HEAD = 0,
	STAGE2_VERSION_ID,
	STAGE3_SERVO_ID,
	STAGE4_CMD_CODE,
	STAGE5_DATA_CHKSUM,
	STAGE6_FRAME_END

} e_servo_deframe_states_t;

typedef struct
{
	float servo_min_range;
	float servo_max_range;
	bool is_configured;
} servo_range_lim_t;

typedef union
{
	int16_t value;
	struct
	{
		int8_t low;
		int8_t high;
	} d;
} convertor_t;

const servo_range_lim_t ServoRangeCheck[MAX_CNTRL_SURFACE] =
	{{0.0f, 0.0f, false},
	 {0.0f, 0.0f, false},
	 {FLAPERON_OUT_LH_MIN, FLAPERON_OUT_LH_MAX, true},
	 {FLAPERON_IN_LH_MIN, FLAPERON_IN_LH_MAX, true},
	 {FLAP_LH_MIN, FLAP_LH_MAX, true},
	 {FLAP_RH_MIN, FLAP_RH_MAX, true},
	 {FLAPERON_IN_RH_MIN, FLAPERON_IN_RH_MAX, true},
	 {FLAPERON_OUT_RH_MIN, FLAPERON_OUT_RH_MAX, true},
	 {0.0f, 0.0f, false},
	 {RUDDER_LH_MIN, RUDDER_LH_MAX, true},
	 {RUDDER_RH_MIN, RUDDER_RH_MAX, true},
	 {ELEVATOR_LH_MIN, ELEVATOR_LH_MAX, true},
	 {ELEVATOR_RH_MIN, ELEVATOR_RH_MAX, true},
	 {STEERING_MIN, STEERING_MAX, true}};

static void ach_servo_parse(const uint8_t *ptr_byte, kst_rs485_msg_t *msg_decode);
static void ach_servo_decode(const int8_t *data, kst_rs485_msg_t *msg_decode);
static uint8_t ach_servo_compute_checksum(const uint8_t *msg);
static int16_t ach_servo_combineBytes(int8_t lowByte, int8_t highByte);
static float ach_servo_clamp_to_range(e_servo_positions_t servo_id, float pos_cmd_deg);
static void ach_cmd_servo_pos_deg(e_servo_positions_t servo_id, float position_deg);
static void ach_cmd_req_servo_pos(e_servo_positions_t servo_id);
static void ach_cmd_req_servo_status(e_servo_positions_t servo_id);
static void ach_servo_command_msg(e_servo_positions_t servo_id, ach_servo_kst_cmd_codes_t cmd_code, const uint8_t *data);

static servo_s Servo[MAX_CONTROL_SURFACE];
static std_servo_cmd_t ServoCmd = {0};
static kst_rs485_msg_t ServoMsg = {0};
static uint8_t ServoRxPacket[SERVO_BUFFER_SIZE] = {0};
static s_timer_data_t LowVoltageTimer;
static e_servo_positions_t ServoLowVoltFaultId = MAX_CNTRL_SURFACE;
static bool servo_test_in_progress = false;

/**
 * @brief Retrieves the current position (in degrees) of the specified servo.
 *
 * This function returns the position of the servo identified by the given
 * servo_id. The position is returned as a floating-point value representing
 * degrees.
 *
 * @param servo_id The ID of the servo (1-based index).
 * @return float The current position of the servo in degrees.
 *
 */
float ach_get_servo_pos_deg(e_servo_positions_t servo_id)
{
	float position = 0.0f;

	/* Check if the requested Servo ID is Valid */
	switch (servo_id)
	{
	case FLAPERON_OUTER_LH:
	case FLAPERON_INNER_LH:
	case FLAP_LH:
	case FLAP_RH:
	case FLAPERON_INNER_RH:
	case FLAPERON_OUTER_RH:
	case RUDDER_LH:
	case RUDDER_RH:
	case ELEVATOR_LH:
	case ELEVATOR_RH:
	case STEERING:
		/* Servo IDs are 1-based, so we subtract 1 to access the correct index */
		position = Servo[servo_id].deg;
		break;
	default:
		position = 0.0f;
		break;
	}
	return (position);
}

/**
 * @brief Checks if the specified servo has a low voltage alarm.
 *
 * This function verifies whether the servo identified by the given servo ID
 * currently has its low voltage alarm flag set. It first validates the servo ID
 * against a list of known, valid servo positions. If the ID is valid, it returns
 * the state of the low voltage alarm for that servo. If the ID is invalid, it
 * returns false.
 *
 * @param servo_id The ID of the servo to check (of type e_servo_positions_t).
 * @return true if the specified servo has a low voltage alarm, false otherwise.
 */
bool ach_get_servo_lv_alarm(e_servo_positions_t servo_id)
{
	bool low_volt_alarm = false;

	/* Check if the requested Servo ID is Valid */
	switch (servo_id)
	{
	case FLAPERON_OUTER_LH:
	case FLAPERON_INNER_LH:
	case FLAP_LH:
	case FLAP_RH:
	case FLAPERON_INNER_RH:
	case FLAPERON_OUTER_RH:
	case RUDDER_LH:
	case RUDDER_RH:
	case ELEVATOR_LH:
	case ELEVATOR_RH:
	case STEERING:
		/* Servo IDs are 1-based, so we subtract 1 to access the correct index */
		low_volt_alarm = Servo[servo_id].low_volt_alarm;
		break;
	default:
		low_volt_alarm = false;
		break;
	}
	return (low_volt_alarm);
}

/**
 * @brief Retrieves the current and temperature status of a specified servo device.
 *
 * This function checks if the provided servo ID is valid and, if so, retrieves the current (I)
 * and temperature (T) values for the corresponding servo from the Servo array. If the servo ID
 * is invalid, the function sets the output values to 0.0f.
 *
 * @param servo_id      The ID of the servo device (of type e_servo_positions_t).
 * @param current       Pointer to a float where the current value will be stored. Can be NULL if not needed.
 * @param temperature   Pointer to a float where the temperature value will be stored. Can be NULL if not needed.
 */
void ach_get_servo_device_status(e_servo_positions_t servo_id, float *current, float *temperature)
{
	/* Check if the requested Servo ID is Valid */
	switch (servo_id)
	{
	case FLAPERON_OUTER_LH:
	case FLAPERON_INNER_LH:
	case FLAP_LH:
	case FLAP_RH:
	case FLAPERON_INNER_RH:
	case FLAPERON_OUTER_RH:
	case RUDDER_LH:
	case RUDDER_RH:
	case ELEVATOR_LH:
	case ELEVATOR_RH:
	case STEERING:
		/* Servo IDs are 1-based, so we subtract 1 to access the correct index */
		if (current != NULL)
		{
			*current = Servo[servo_id].I;
		}
		if (temperature != NULL)
		{
			*temperature = Servo[servo_id].T;
		}
		break;
	default:
		if (current != NULL)
		{
			*current = 0.0f;
		}
		if (temperature != NULL)
		{
			*temperature = 0.0f;
		}
		break;
	}
}

/**
 * @brief Sets the servo command degrees from the provided command structure.
 *
 * This function copies the servo command degrees from the input structure
 * to the global ServoCmd structure, for all control surfaces.
 *
 * @param[in] servo_cmd Pointer to a std_servo_cmd_t structure containing
 *  the desired servo command degrees. If NULL, the function does nothing.
 */
void ach_set_servo_cmd_deg(const std_servo_cmd_t *servo_cmd)
{
	/* Check if the pointer is valid */
	if (servo_cmd != NULL)
	{
		/* Copy the command degrees to the ServoCmd structure */
		for (e_servo_positions_t id = FLAPERON_OUTER_LH; id < STEERING; id++)
		{
			ServoCmd.servo_cmd_deg[id] = servo_cmd->servo_cmd_deg[id];
		}
	}
}

/**
 * @brief Initializes the servo UART interface.
 *
 * This function initializes the UART interface used for servo communication.
 * It calls the uart_init() function with the UART_SERVO parameter.
 *
 * @return true if the UART initialization for the servo was successful, false otherwise.
 */
bool ach_servo_init(void)
{
	bool init_status = true;

	/* Initialize the Servo Command Degrees to Zero */
	for (int i = 0; i < MAX_CONTROL_SURFACE; i++)
	{
		ServoCmd.servo_cmd_deg[i] = 0.0f;
	}

	/* Initialize the Low Voltage Timer */
	timer_reset(&LowVoltageTimer);

//	 /* Initialize the UART for Servo Communication */
//	 if (true == uart_init(UART_SERVO))
//	 {
//	 	/* If UART initialization fails, return false */
//	 	init_status = true;
//	 }

	/* Return Servo Initialization Status */
	return (init_status);
}

/**
 * @brief Periodically sends position commands to all servos.
 *
 * This function iterates through all available servos and checks if a position command
 * (in degrees) has been set for each servo. If a non-zero command is present, it sends
 * the corresponding position command to the servo using ach_cmd_servo_pos_deg().
 *
 * @note Only servos with a non-zero command value will receive a position update.
 *
 * @return void
 */
void ach_cmd_servo_periodic(void)
{
	float bounded_cmd = 0.0f;

	/* Iterate through all servos and send position commands */
	for (e_servo_positions_t id = FLAPERON_OUTER_LH; id < STEERING; id++)
	{
		/* Range check the command to ensure operating Servo within Limits */
		bounded_cmd = ach_servo_clamp_to_range(id, ServoCmd.servo_cmd_deg[id]);

		/* If Servo Test is in Progress stop from commanding the normal commands */
		if (servo_test_in_progress != true)
		{
			/* Send the command to set the servo position */
			ach_cmd_servo_pos_deg(id, bounded_cmd);
		}
	}
}

/**
 * @brief Clamps the given servo position command to the allowed range for the specified servo.
 *
 * This function checks if the provided position command (in degrees) for a given servo
 * is within the allowed minimum and maximum range as defined in the ServoRangeCheck array.
 * If the command is below the minimum, it returns the minimum value.
 * If the command is above the maximum, it returns the maximum value.
 * Otherwise, it returns the original command value.
 *
 * @param servo_id      The identifier of the servo (of type e_servo_positions_t).
 * @param pos_cmd_deg   The desired servo position command in degrees.
 *
 * @return The clamped servo position command within the allowed range.
 */
static float ach_servo_clamp_to_range(e_servo_positions_t servo_id, float pos_cmd_deg)
{
	float return_val = 0.0f;
	float min = ServoRangeCheck[servo_id].servo_min_range;
	float max = ServoRangeCheck[servo_id].servo_max_range;

	/* if it is less than min */
	if (pos_cmd_deg < min)
	{
		/* clamp it to min */
		return_val = min;
	}
	/* else if it is more than max */
	else if (pos_cmd_deg > max)
	{
		/* clamp it down to max */
		return_val = max;
	}
	else
	{
		/* return as is if within range */
		return_val = pos_cmd_deg;
	}

	return return_val;
}

/**
 * @brief Sends a position command (in degrees) to a specified servo via UART.
 *
 * This function constructs and sends a command message to set the position of a servo motor.
 * The position is specified in degrees and is converted to a 16-bit integer value (scaled by 10).
 * The message is transmitted over the UART interface.
 *
 * @param servo_id      The ID of the target servo motor.
 * @param position_deg  The desired position in degrees (float).
 */
static void ach_cmd_servo_pos_deg(e_servo_positions_t servo_id, float position_deg)
{
	int16_t posByte = 0;
	uint8_t msg[BYTES_PER_MSG] = {0U};

	/* MISRA C:2012 Rule 10.6 - Explicit cast for floating-point to integer conversion */
	posByte = (int16_t)(position_deg * 10.0F);

	/* Populate the data bytes */
	msg[0] = (uint8_t)(posByte & 0xFF);
	msg[1] = (uint8_t)((posByte >> 8) & 0xFF);
	msg[2] = 0x00U; /* supplemented with 0x00 */
	msg[3] = 0x00U; /* supplemented with 0x00 */

	ach_servo_command_msg(servo_id, KST_CMD_SET_POSITION, &msg[0]);
}

/**
 * @brief Sends a command to request position feedback from a specified servo.
 *
 * This function prepares a message buffer with default values and sends a
 * position feedback request command to the servo identified by the given
 * servo ID. The message is formatted according to the protocol requirements
 * and transmitted using the ach_servo_command_msg function.
 *
 * @param servo_id The identifier of the servo for which position feedback is requested.
 */
static void ach_cmd_req_servo_pos(e_servo_positions_t servo_id)
{
	uint8_t msg[BYTES_PER_MSG] = {0U};

	/* Populate the data bytes */
	msg[0] = 0x00U; /* supplemented with 0x00 */
	msg[1] = 0x00U; /* supplemented with 0x00 */
	msg[2] = 0x00U; /* supplemented with 0x00 */
	msg[3] = 0x00U; /* supplemented with 0x00 */

	ach_servo_command_msg(servo_id, KST_CMD_GET_POSITION, &msg[0]);
}

/**
 * @brief Sends a command request to retrieve the status of a specified servo.
 *
 * This function constructs a message with default data bytes and sends a device status
 * command for the given servo ID. The message is populated with four bytes, each set to 0x00,
 * and then passed to the ach_servo_command_msg function along with the servo identifier and
 * the device status command code.
 *
 * @param servo_id The identifier of the servo whose status is being requested.
 */
static void ach_cmd_req_servo_status(e_servo_positions_t servo_id)
{
	uint8_t msg[BYTES_PER_MSG] = {0U};

	/* Populate the data bytes */
	msg[0] = 0x00U; /* supplemented with 0x00 */
	msg[1] = 0x00U; /* supplemented with 0x00 */
	msg[2] = 0x00U; /* supplemented with 0x00 */
	msg[3] = 0x00U; /* supplemented with 0x00 */

	ach_servo_command_msg(servo_id, KST_CMD_DEVICE_STATUS, &msg[0]);
}

/**
 * @brief Constructs and sends a command message to a servo via UART.
 *
 * This function creates a message frame according to the servo communication protocol,
 * populates it with the specified servo ID, command code, and reserved data bytes,
 * computes the checksum, and sends the message over the UART interface.
 *
 * @param[in] servo_id   The ID of the target servo.
 * @param[in] cmd_code   The command code to be sent to the servo.
 * @param[in] data       Pointer to an array of 4 bytes containing reserved data.
 */
static void ach_servo_command_msg(e_servo_positions_t servo_id, ach_servo_kst_cmd_codes_t cmd_code, const uint8_t *data)
{
	uint8_t msg[BYTES_PER_MSG] = {0U};
	uint8_t *p_msg = msg;

	/* Set the Frame Head Byte */
	p_msg[0] = FRAME_HEAD;
	/* Set the Version ID */
	p_msg[1] = VERSION_ID;
	/* Set the Servo ID */
	p_msg[2] = (uint8_t)servo_id;
	/* Set the Command Code */
	p_msg[3] = (uint8_t)cmd_code;
	/* Update the data bytes */
	p_msg[4] = data[0];
	p_msg[5] = data[1];
	p_msg[6] = data[2];
	p_msg[7] = data[3];
	/* Calculate the Checksum */
	p_msg[8] = ach_servo_compute_checksum(p_msg);
	/* Append the Frame End Byte */
	p_msg[9] = FRAME_END;

	/* Write the message to the UART peripheral */
	//(void)uart_write(UART_SERVO, p_msg, BYTES_PER_MSG);
}

/**
 * @brief Computes the checksum for a servo message.
 *
 * This function calculates the checksum by summing all bytes in the message,
 * starting from the second byte (index 1) up to the byte before the last two bytes.
 * The first byte (frame head) and the last two bytes (checksum and frame end)
 * are excluded from the calculation. The checksum is computed by simple addition,
 * and any overflow is omitted.
 *
 * @param msg Pointer to the message buffer (array of bytes).
 * @return uint8_t The computed checksum value.
 */
static uint8_t ach_servo_compute_checksum(const uint8_t *msg)
{
	uint8_t checksum = 0U;
	uint8_t index;

	/* Check for null pointer */
	if (msg != NULL)
	{
		/*  Calculate the checksum by summing all bytes except the frame head and
		the last 2 bytes (checksum and frame end) */
		for (index = 1U; index < (uint8_t)(BYTES_PER_MSG - 2U); index++)
		{
			/* Add the bytes, omit the overflow */
			checksum = (uint8_t)(checksum + msg[index]);
		}
	}

	/* Return the computed checksum value */
	return checksum;
}

/**
 * @brief Periodically reads and processes servo data from UART.
 *
 * This function handles the periodic reading of servo feedback and status messages
 * from the UART interface. It parses incoming RS485 packets, updates servo status,
 * handles low voltage alarms, and alternates between requesting servo position and
 * status to avoid flooding the servos with requests. The function maintains state
 * across calls to ensure each servo is polled in turn.
 *
 * Message types handled:
 * - Position feedback (auto report, get position)
 * - Low voltage alarm
 * - Device status (current, temperature)
 * - Watchdog failsafe (TODO)
 *
 * Low voltage alarms are reset if the warning condition clears and the timer expires.
 * The function uses static variables to track the current servo slot and feedback state.
 *
 * @note This function should be called periodically from the main control loop.
 */
void ach_servo_read_periodic(void)
{

	const e_servo_positions_t servo_read_slot[MAX_CONFIGURED_SERVO] = {FLAPERON_OUTER_LH,
																	   FLAPERON_INNER_LH,
																	   FLAP_LH,
																	   FLAP_RH,
																	   FLAPERON_INNER_RH,
																	   FLAPERON_OUTER_RH,
																	   RUDDER_LH,
																	   RUDDER_RH,
																	   ELEVATOR_LH,
																	   ELEVATOR_RH};

	static uint8_t slot_id = 0;
	static bool pos_feedback = false;
	// Make sure that message structure is reset
	util_memset(&ServoMsg, 0, sizeof(kst_rs485_msg_t));

	/* Read the UART Buffer */
	uint16_t bytes_read = 0 /*uart_read(UART_SERVO, &ServoRxPacket[0], SERVO_BUFFER_SIZE)*/;

	if (bytes_read > 0)
	{
		// INFO("bytes_read = %d", bytes_read);
		for (uint16_t cnt = 0; cnt < bytes_read; cnt++)
		{
			/* Parse the data received */
			ach_servo_parse(&ServoRxPacket[cnt], &ServoMsg);

			if (ServoMsg.msg_status == RS485_RETURN_OK)
			{

				/* Copy the Servo Node ID */
				e_servo_positions_t idx = ServoMsg.ID;

				/* Check if the current servo ID had a Low Voltage Alarm Set Previously and
				   also if the 1 Second timer has expired and the warning does not persists */
				if ((ServoLowVoltFaultId == ServoMsg.ID) && (true == Servo[idx].low_volt_alarm) && (timer_check_expiry(&LowVoltageTimer)) && (true != ServoMsg.warning.low_volt))
				{
					/* Reset the Low Voltage Alarm Flag */
					Servo[idx].low_volt_alarm = false;
					ServoLowVoltFaultId = MAX_CNTRL_SURFACE;
					timer_reset(&LowVoltageTimer);
				}

				/* Based on received code update the */
				switch (ServoMsg.receive_code)
				{
				case KST_RECEIVE_AUTO_REPORT:
				case KST_RECEIVE_CMD_GET_POSITION:
					Servo[idx].deg = ServoMsg.report.position;
					Servo[idx].flag = true; // new data flag
					Servo[idx].counter = 0;
					Servo[idx].comm = true;
					break;

				case KST_RECEIVE_LOW_VOLT_ALARM:
					/* If low voltage alarm is set */
					if (ServoMsg.warning.low_volt)
					{
						/* Set the low voltage alarm bit */
						Servo[idx].low_volt_alarm = ServoMsg.warning.low_volt;
						/* Record the servo */
						ServoLowVoltFaultId = ServoMsg.ID;
					}
					break;

				case KST_RECEIVE_CMD_DEVICE_STATUS:
					Servo[idx].I = ServoMsg.status.current / 100.0f; // from [mA] to [A]
					Servo[idx].T = ServoMsg.status.temp;			 // Temperature in °C
					break;

				case KST_RECEIVE_CMD_WATCHDOG_FAILSAFE:
					// TODO:
					// - think about failsafe return msg action
					// - resets the timer within the Servo to enable failsafe
					// - needs to be send periodically
					break;

				default:
					break;
				}

				// mark msg as read
				ServoMsg.msg_status = RS485_RETURN_READ;
			}
		}
	}

	if (pos_feedback)
	{
		/* Read Position of One Servo at a time */
		ach_cmd_req_servo_pos(servo_read_slot[slot_id]);
		/* reset the flag after reading one servo */
		pos_feedback = false;
	}
	else
	{
		/* To avoid flooding the Servos with request Command the Servo*/
		ach_cmd_req_servo_status(servo_read_slot[(slot_id + 1) % 10]);
		/*  set the flag to read position next time */
		pos_feedback = true;
		/* Increment to the next Servo */
		slot_id = (slot_id + 1) % 10;
	}
}

/**
 * @brief deframing and decoding of KST Servo Motor msg
 *
 * @param ptr_byte   pointer to next byte in RS485 msg
 * @param msg_decode decoded rs485 message struct
 */
static void ach_servo_parse(const uint8_t *ptr_byte, kst_rs485_msg_t *msg_decode)
{
	/* State Machine State Tracker */
	static e_servo_deframe_states_t state = STAGE1_FRAME_HEAD;
	/* Checksum */
	static uint8_t checksum = 0;
	/* counter of payload */
	static int payload_byte_count = 0;
	/* array to hold the data */
	static int8_t data[DATA_LEN] = {0};

	/* If pointers passed are not equal to NULL */
	if (msg_decode != NULL && ptr_byte != NULL)
	{
		/* reset a decoded and saved msg*/
		if (msg_decode->msg_status == RS485_RETURN_READ)
		{
			state = STAGE1_FRAME_HEAD;
			checksum = 0;
			payload_byte_count = 0;
		}

		switch (state)
		{
		case STAGE1_FRAME_HEAD:
		{
			/* Check if Frame Head is received */
			if (FRAME_HEAD == *ptr_byte)
			{
				/* Advance to the next state */
				state = STAGE2_VERSION_ID;
				/* Reset the Payload count */
				payload_byte_count = 0;
				/* Reset the Checksum */
				checksum = 0;
				/* Reset the data array */
				for (uint8_t index = 0; index < DATA_LEN; index++)
				{
					data[index] = 0;
				}
				/* Set the message status to pending */
				msg_decode->msg_status = RS485_PENDING;
			}
			break;
		}

		case STAGE2_VERSION_ID:
		{
			/* Check if the Version ID is received correctly */
			if (VERSION_ID == *ptr_byte)
			{
				/* Advance to the next state */
				state = STAGE3_SERVO_ID;
				/* Accumulate the checksum, omit the overflow bytes*/
				checksum = (uint8_t)(checksum + *ptr_byte);
			}
			else
			{
				/* Reset state back to FRAME HEAD */
				state = STAGE1_FRAME_HEAD;
				/* Indicate Message decode error */
				msg_decode->msg_status = RS485_DECODE_ERROR;
			}
			break;
		}

		case STAGE3_SERVO_ID:
		{
			/* Check if the Servo ID is valid and Servo ID is not equal to 1 and 8 (not used on subscale flights) */
			if ((*ptr_byte < MAX_CONTROL_SURFACE) && (*ptr_byte != BROADCAST_SERVO) &&
				(*ptr_byte != AILERON_L) && (*ptr_byte != AILERON_R))
			{
				/* Set the Servo ID in the message decode structure */
				msg_decode->ID = (e_servo_positions_t)*ptr_byte;
				/* Advance to the next state */
				state = STAGE4_CMD_CODE;
				/* Accumulate the checksum, omit the overflow bytes*/
				checksum = (uint8_t)(checksum + *ptr_byte);
			}
			else
			{
				/* Reset state back to FRAME HEAD */
				state = STAGE1_FRAME_HEAD;
				/* Indicate Message decode error */
				msg_decode->msg_status = RS485_DECODE_ERROR;
			}
			break;
		}

		case STAGE4_CMD_CODE:
		{
			/* Check if the Command Code is valid */
			msg_decode->receive_code = (ach_servo_kst_rx_codes_t)*ptr_byte;
			/* Advance to the next state */
			state = STAGE5_DATA_CHKSUM;
			/* Accumulate the checksum, omit the overflow bytes*/
			checksum = (uint8_t)(checksum + *ptr_byte);

			break;
		}

		case STAGE5_DATA_CHKSUM:
		{
			/*	if Payload Byte Count is less than 4 */
			if (payload_byte_count < 4)
			{
				/* copy the incoming bytes */
				data[payload_byte_count] = (int8_t)*ptr_byte;
				/* Accumulate the checksum, omit the overflow bytes*/
				checksum = (uint8_t)(checksum + *ptr_byte);
				/* Increment the Payload Byte Count */
				payload_byte_count++;
			}
			else
			{
				/* Compare the computed checksum with the inbound checksum */
				if (checksum == *ptr_byte)
				{
					/* Advance to the next state */
					state = STAGE6_FRAME_END;
					/* Reset the checksum for the next message */
					checksum = 0;
				}
				else
				{
					/* Reset state back to FRAME HEAD */
					state = STAGE1_FRAME_HEAD;
					/* Indicate Message decode error */
					msg_decode->msg_status = RS485_DECODE_ERROR;
				}
			}
			break;
		}

		case STAGE6_FRAME_END:
		{
			/* Check if the Frame End Byte is received correctly */
			if (FRAME_END == *ptr_byte)
			{
				/* Decode the received data */
				msg_decode->msg_status = RS485_RETURN_OK;

				/* Decode the received message soon after ensuring the integrity of the received packet */
				ach_servo_decode(&data[0], msg_decode);
			}
			else
			{
				/* Indicate a decoding error */
				msg_decode->msg_status = RS485_DECODE_ERROR;
				printf("SERVO fail\r\n");
			}
			/* Reset back to Frame Head in either case */
			state = STAGE1_FRAME_HEAD;
			break;
		}

		default:
		{
			/* If the state is not valid, reset to Frame Head */
			state = STAGE1_FRAME_HEAD;
			msg_decode->msg_status = RS485_DECODE_ERROR;
			break;
		}
		}
	}
}

/**
 * @brief decodes the data (payload) of the RS485 message based on the
 *        received command code (Byte[3] in full message)
 *
 * @param data          array 4 bytes containing the msg payload
 * @param msg_decode    decoded rs485 message struct
 *
 * @note:   msg_decode->receive_code is the answer from a requested command (specifies the data
 * type inside the data)
 */
static void ach_servo_decode(const int8_t *data, kst_rs485_msg_t *msg_decode)
{

	switch (msg_decode->receive_code)
	{
	case KST_RECEIVE_AUTO_REPORT:
	{

		float value = 0.0f;

		value = (float)ach_servo_combineBytes(data[0], data[1]);
		msg_decode->report.position = (value / 10.0f); // deg
		break;
	}

	case KST_RECEIVE_CMD_GET_POSITION:
	{

		float value = 0.0f;

		value = (float)ach_servo_combineBytes(data[0], data[1]);
		msg_decode->report.position = (value / 10.0f); // deg
		break;
	}

	case KST_RECEIVE_LOW_VOLT_ALARM:
	{

		/* if low voltage alarm is TRUE */
		if (0x0A == data[0] && 0x02 == data[1])
		{
			/* Set the Low Voltage Flag */
			msg_decode->warning.low_volt = true;
			/* Kick start the timer */
			timer_start(&LowVoltageTimer, 1000);
		}
		else
		{
			// expected a different msg type
			// trigger error
			msg_decode->msg_status = RS485_DECODE_ERROR;
		}
		break;
	}

	case KST_RECEIVE_CMD_DEVICE_STATUS:
	{
		/* Decode the Temperature and Current */
		float curr = (float)ach_servo_combineBytes(data[0], data[1]);
		/* Store the decoded values in the message structure */
		msg_decode->status.temp = (float)data[2];  // temperature
		msg_decode->status.current = curr / 10.0f; // current in mA

		break;
	}

	// FAILSAFE:
	case KST_RECEIVE_CMD_WATCHDOG_FAILSAFE:
	{
		// INFO("WATCHDOG Failsafe reset!\n");
		break;
	}

	default:
	{
		if (msg_decode->receive_code >= KST_ERROR_OFFSET)
		{
			// INFO("Servo ID: %d - Error Code received: 0x%X\n",
			//      msg_decode->ID,
			//      msg_decode->receive_code - KST_ERROR_OFFSET);
			msg_decode->msg_status = RS485_RETURN_ERROR;
			msg_decode->decoded.value = (float)msg_decode->receive_code - KST_ERROR_OFFSET;
		}

		break;
	}
	}
}

/**
 * @brief Combines two 8-bit signed bytes into a single 16-bit signed integer.
 *
 * This function takes a low byte and a high byte, and merges them into a 16-bit
 * signed integer using a convertor_t union or struct. The low byte is assigned
 * to the lower 8 bits, and the high byte to the upper 8 bits of the result.
 *
 * @param lowByte  The lower 8 bits (int8_t) of the combined value.
 * @param highByte The upper 8 bits (int8_t) of the combined value.
 * @return int16_t The resulting 16-bit signed integer.
 */
static int16_t ach_servo_combineBytes(int8_t lowByte, int8_t highByte)
{
	convertor_t combo;
	combo.d.high = highByte;
	combo.d.low = lowByte;
	return combo.value;
}
