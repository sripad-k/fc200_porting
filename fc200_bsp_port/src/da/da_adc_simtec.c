/****************************************************
 *  da_adc_simtec.c
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_adc_simtec
 *  Copyright: LODD (c) 2025
 ****************************************************/

#include "da_adc_simtec.h"
#include "generic_util.h"
#include "timer_interface.h"

#define MAX_CHARS (8)
#define MAX_STATUS_CHARS (4)
#define MAX_OVERHEAD (4) /* SOH + Header + NULL Character + CR */
#define PACKET_SIZE (MAX_CHARS + MAX_OVERHEAD)
#define MARKER_BIT (0x80)
#define LABEL_ID_FIELD (0x0F)
#define DATA_FLAG_FIELD (0x70)
#define CARRIAGE_RETURN (0x0D)
#define INVALID_LABEL (0xFF)
#define STATUS_LABEL (MAX_ADC_LABEL_COUNT + 1)
#define MAX_SOH_COUNT (6)
#define MAX_LABEL_PER_SOH (16)
#define ADC_RX_BUFFER_SIZE (256)
#define SOH1 (0x01)
#define SOH2 (0x02)
#define SOH3 (0x03)
#define SOH4 (0x04)
#define SOH5 (0x05)
#define SOH6 (0x06)

#define ADC_9_TIMEOUT (60) /* 60 msec timeout for ADC-9 */

static s_timer_data_t Adc9MonitorTimer;
static bool Adc9CommTimeout;

typedef enum
{
	SOH,
	HEADER,
	STATUS,
	DATA
} adc_deframe_states_t;

/* private function prototypes */
static void da_ads_parse_data(const uint8_t *ptr_byte);
static void da_ads_process_data(void);
static void da_map_adc_data_and_flag(da_ads_label_t label, da_ads_data_t *data);

static uint16_t Status_Label = 0;
static da_ads_data_t ADC_Data_Pool[MAX_ADC_LABEL_COUNT];

static const uint8_t ADC_SOH_Label_Ref[MAX_SOH_COUNT + 1][MAX_LABEL_PER_SOH] =
	{
		/* ---------- Invalid SOH -------------*/
		{
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/

		/* ---------- SOH1 Labels -------------*/
		{
			INVALID_LABEL,
			SOH1_QC, /* Impact pressure */
			SOH1_PS, /* Static pressure */
#if defined(ADC_9) || defined(ADC_10X)
			SOH1_AOA, /* Angle of attack */
			SOH1_AOS, /* Angle of slideslip */
#elif defined(ADC_10)
			INVALID_LABEL,
			INVALID_LABEL,
#endif
			SOH1_CAS,  /* Calibrate airspeed */
			SOH1_TAS,  /* True airspeed */
			SOH1_HP,   /* Pressure altitude (see note) */
			SOH1_MACH, /* Mach Number */
			SOH1_SAT,  /* Static air temperature */
			SOH1_TAT,  /* Total air temperature */
			INVALID_LABEL,
			INVALID_LABEL,
#ifdef ADC_9
			SOH1_PCB_TEMP,				  /* PCB Temperature */
#elif defined(ADC_10) || defined(ADC_10X) /* Combined ADC_10 and ADC_10X */
			INVALID_LABEL, /* Invalid Label D */
#endif
			SOH1_QNH, /* QNH Setting */
			STATUS_LABEL,
		},
		/* ------------------------------------*/

		/* ---------- SOH2 Labels -------------*/
		{
			INVALID_LABEL,
			SOH2_CR, /* Climb rate */
#ifdef ADC_9
			SOH2_PT, /* Total pressure */
#elif defined(ADC_10) || defined(ADC_10X)
			INVALID_LABEL,
#endif
			INVALID_LABEL,
			INVALID_LABEL,
#ifdef ADC_9
			INVALID_LABEL,
			INVALID_LABEL,
#elif defined(ADC_10) || defined(ADC_10X)
			SOH2_CAS_R, /* CAS Rate */
			SOH2_TAS_R, /* TAS Rate */
#endif
			SOH2_HBARO, /* Barometric Altitude (QNH) */
			INVALID_LABEL,
#ifdef ADC_9
			SOH2_IAS, /* Indicated Airspeed */
			SOH2_VMO, /* Maximum Operating Airspeed */
#elif defined(ADC_10) || defined(ADC_10X)
			INVALID_LABEL,
			INVALID_LABEL,
#endif
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/

		/* ---------- SOH3 Labels -------------*/
		{
			INVALID_LABEL,
#ifdef ADC_9
			SOH3_SP_QC, /* Sensor Pressure Qc */
			SOH3_SP_PS, /* Sensor Pressure Ps */
#elif ADC_10
			INVALID_LABEL,
			INVALID_LABEL,
#elif ADC_10X
			SOH3_QCU,
			SOH3_PSU,
#endif
#ifdef ADC_9
			SOH3_SP_AOA, /* Sensor Pressure Angle of Attack */
			SOH3_SP_AOS, /* Sensor Pressure Angle of Sideslip */
			SOH3_IAT,	 /* Indicated Temperature */
			SOH3_BAT,	 /* Base Temperature */
#elif defined(ADC_10) || defined(ADC_10X)
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
#endif
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			SOH3_ST_QC, /* Sensor Temperature Qc */
			SOH3_ST_PS, /* Sensor Temperature Ps */
#ifdef ADC_9
			SOH3_ST_AOA, /* Sensor Temperature Angle of Attack */
			SOH3_ST_AOS, /* Sensor Temperature Angle of Sideslip */
#elif defined(ADC_10) || defined(ADC_10X)
			INVALID_LABEL,
			INVALID_LABEL,
#endif
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/

		/* No Labels available under SOH4 in ADC-9, 10 and 10X */
		{
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/

		/* ---------- SOH5 Labels -------------*/
		{
			INVALID_LABEL,
#ifdef ADC_9
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
#elif defined(ADC_10) || defined(ADC_10X)
			SOH5_QC,	/* Impact Pressure */
			SOH5_PS,	/* Static Pressure */
			SOH5_HP,	/* Pressure Altitude */
			SOH5_HBARO, /* Barometric Altitude (QNH) */
			SOH5_CAS,	/* Calibrate Airspeed */
			SOH5_TAS,	/* True Airspeed */
			SOH5_CR,	/* Climb Rate */
#endif
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/

		/* ---------- SOH6 Labels -------------*/
		{
			INVALID_LABEL,
#ifdef ADC_9
			SOH6_QC_I,	   /* Indicated Impact Pressure */
			SOH6_PS_I,	   /* Indicated Static Pressure */
			SOH6_PS_I_AOA, /* Indicated Angle of Attack */
			SOH6_PS_I_AOS, /* Indicated Angle of Sideslip */
			SOH6_PT_I,	   /* Indicated Total Pressure */
#elif defined(ADC_10) || defined(ADC_10X)
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
#endif
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL,
			INVALID_LABEL},
		/* ------------------------------------*/
};

static uint8_t ADC_RxPacket[ADC_RX_BUFFER_SIZE];

/**
 * @brief Retrieves ADC data for the specified signal.
 *
 * This function obtains the ADC (Analog-to-Digital Converter) data corresponding to the given signal name.
 * It checks if the retrieved data is valid and, if so, stores the value in the provided data pointer.
 *
 * @param[in]  signal_name  The identifier for the ADC signal to be read.
 * @param[out] data         Pointer to a float where the ADC data will be stored if valid.
 *
 * @return true  If the ADC data is valid and successfully retrieved.
 * @return false If the ADC data is invalid or retrieval failed.
 */
bool da_get_adc_9_data(adc_data_type_t signal_name, float *data)
{
	bool status = false;
	da_ads_data_t adc_data;

	/* Get the ADC data and flag */
	da_map_adc_data_and_flag((da_ads_label_t)signal_name, &adc_data);

	/* Check if the data pointer is valid */
	if (data != NULL)
	{
		/* Provide the ADC Param value irrespective of validity, upto
		the calling function to decide what it wants to do with it*/
		*data = adc_data.Data;

		/* Check if the data is valid */
		if (adc_data.Data_Flag == FLAG_VALID)
		{
			/* If the data is valid, set the status to true */
			status = true;
		}
	}

	return status;
}

bool da_get_adc_9_cas(float *cas)
{
	*cas = ADC_Data_Pool[SOH1_CAS].Data;

	return ADC_Data_Pool[SOH1_CAS].Data_Flag == FLAG_VALID;
}

bool da_get_adc_9_aoa(float *aoa)
{
	*aoa = ADC_Data_Pool[SOH1_AOA].Data;

	return ADC_Data_Pool[SOH1_AOA].Data_Flag == FLAG_VALID;
}

bool da_get_adc_9_aos(float *aos)
{
	*aos = ADC_Data_Pool[SOH1_AOS].Data;

	return ADC_Data_Pool[SOH1_AOS].Data_Flag == FLAG_VALID;
}

bool da_get_adc_9_oat(float *oat)
{
	*oat = ADC_Data_Pool[SOH1_SAT].Data;

	return ADC_Data_Pool[SOH1_SAT].Data_Flag == FLAG_VALID;
}

bool da_get_adc_9_alt(float *alt)
{
	*alt = ADC_Data_Pool[SOH1_HP].Data;

	return ADC_Data_Pool[SOH1_HP].Data_Flag == FLAG_VALID;
}

bool da_get_adc_9_timeout(void)
{
	/* Check if the ADC communication has timed out */
	return Adc9CommTimeout;
}

/**
 * @brief Retrieves the current status of the ADC.
 *
 * This function returns the value of the Status_Label variable,
 * which represents the current status of the Analog-to-Digital Converter (ADC).
 *
 * @return uint16_t The current ADC status.
 */
uint16_t da_get_adc_9_status(void)
{
	return (Status_Label);
}

/* Operation 'da_ads_init' of Class 'DA_ads' */
/**
 * @brief Initializes the ADS UART interface.
 *
 * This function initializes the UART interface used for ADS (Analog-to-Digital System)
 * communication by calling the uart_init function with the UART_ADS parameter.
 *
 * @return true if the UART initialization is successful, false otherwise.
 */
bool da_adc_9_init(void)
{
	/* SyncableUserCode{B4BCDC35-D358-4bfd-9F57-EE1332701D39}:Nbrlk8aPUZ */
	bool adc_init_success = false;
	/* Initialise the timeout count to false */
	Adc9CommTimeout = false;
	/* Init the ADC timer */
	timer_start(&Adc9MonitorTimer, ADC_9_TIMEOUT);
	/* Init ADC UART */
	adc_init_success = uart_init(UART_ADS);
	/* If successful */
	if (true == adc_init_success)
	{
		/* Send a message indicating ONLINE from the same channel */
		uart_write(UART_ADS, (uint8_t *)"ADC ONLINE\r\n", 15);
	}
	return (adc_init_success);
	/* SyncableUserCode{B4BCDC35-D358-4bfd-9F57-EE1332701D39} */
}

/* Operation 'da_ads_read_periodic' of Class 'DA_ads' */
/**
 * @brief Periodically reads and processes ADS (Analog/Digital Signal) data.
 *
 * This function is intended to be called at regular intervals to handle
 * the acquisition and processing of ADS data. It delegates the actual
 * data processing to the da_ads_process_data() function.
 *
 * @note This function may be called from a timer interrupt or main loop,
 *       depending on the application architecture.
 */
void da_adc_9_read_periodic(void)
{
	/* SyncableUserCode{7121996B-F893-4085-92C1-FFF4ABC7C697}:MOf5bfT9uu */
	da_ads_process_data();
	Adc9CommTimeout = timer_check_expiry(&Adc9MonitorTimer);
	/* SyncableUserCode{7121996B-F893-4085-92C1-FFF4ABC7C697} */
}

/* Operation 'da_ads_parse_data' of Class 'DA_ads' */
/**
 * @brief Parses incoming data bytes according to the ADC deframing state machine.
 *
 * This static function implements a state machine to parse and process incoming bytes
 * from an ADC data stream. It identifies packet headers, validates marker bits and label IDs,
 * and extracts data or status information accordingly. The function manages internal state
 * transitions (SOH, HEADER, STATUS, DATA) and updates relevant data pools and flags.
 *
 * @param ptr_byte Pointer to the incoming byte to be parsed. If NULL, the function does nothing.
 *
 * @note
 * - The function uses static variables to maintain state across calls.
 * - Data is accumulated in a buffer and processed when a complete packet is received.
 * - Validity of data and labels is checked against predefined tables and flags.
 * - The function is not thread-safe due to static state.
 */
static void da_ads_parse_data(const uint8_t *ptr_byte)
{
	/* SyncableUserCode{9D076C2D-36A0-4c14-98E6-5C59543C1502}:MOf5bfT9uu */
	static adc_deframe_states_t state = SOH;
	static unsigned char data_buffer[MAX_CHARS + 1] = {0};
	static uint8_t soh = 0;
	static da_ads_data_validity_t validity_status = FLAG_INVALID;
	uint8_t label_index = 0;
	static da_ads_label_t param_id_ref = SOH1_QC;
	static uint8_t byte_count = 0;

	if (ptr_byte != NULL)
	{
		/* Check if the incoming byte is a Valid Start of Header */
		switch (state)
		{
		case SOH:
			/* Check for Valid Start of Header */
			if (*ptr_byte >= SOH1 && *ptr_byte <= SOH6)
			{
				soh = *ptr_byte;
				byte_count = 0;
				util_memset(data_buffer, 0, PACKET_SIZE);
				state = HEADER;
				validity_status = FLAG_INVALID;
			}
			break;

		case HEADER:
			/* Check if the incoming byte has a Valid Marker Bit */
			if (*ptr_byte & MARKER_BIT)
			{
				/* If the header is valid, store the label */
				label_index = *ptr_byte & LABEL_ID_FIELD;

				/* Check if the Label ID is Valid */
				if (STATUS_LABEL == ADC_SOH_Label_Ref[soh][label_index])
				{
					state = STATUS;
				}
				else if (INVALID_LABEL != ADC_SOH_Label_Ref[soh][label_index])
				{
					/* Get the reference to the parameter in the data pool table */
					param_id_ref = ADC_SOH_Label_Ref[soh][label_index];
					/* Check if the data is valid */
					validity_status = (da_ads_data_validity_t)((*ptr_byte & DATA_FLAG_FIELD) >> 4);
					/* Invalid Label ID, return to SOH state */
					state = DATA;
				}
				else
				{
					/* Valid Label ID, proceed back to read new data */
					state = SOH;
				}
			}
			else
			{
				/* If marker bit is not verified, packet failed return to verify new packet */
				state = SOH;
			}
			break;

		case STATUS:
			/* Read the data and process it */
			if (byte_count < MAX_STATUS_CHARS)
			{
				/* Store the incoming data */
				data_buffer[byte_count] = *ptr_byte;
				byte_count++;
			}
			else
			{
				/* Data buffer is full, process the data */
				data_buffer[byte_count] = '\0'; /* Null terminate the string */
				byte_count = 0;					/* Reset byte count for next packet */
				/* Check for carriage return */
				if (*ptr_byte != '\r')
				{
					/* Convert ASCII to Unsigned integer word */
					util_ascii_hex_to_uword((char *)data_buffer, (uint16_t *)&Status_Label);
				}

				/* Reset the state machine */
				state = SOH;
			}
			break;

		case DATA:
			/* Read the data and process it */
			if (byte_count < MAX_CHARS)
			{
				/* Store the incoming data */
				data_buffer[byte_count] = *ptr_byte;
				byte_count++;
			}
			else
			{
				/* Data buffer is full, process the data */
				data_buffer[byte_count] = '\0'; /* Null terminate the string */
				byte_count = 0;					/* Reset byte count for next packet */
				/* Check for carriage return */
				if (*ptr_byte != '\r')
				{
					validity_status = FLAG_INVALID;
				}
				else
				{
					validity_status = FLAG_VALID;

					/* Reload the timer on valid packet */
					timer_reload(&Adc9MonitorTimer);
					/* Decode the data Acquired  until now */
					util_ascii_hex_to_float((uint8_t *)data_buffer, &ADC_Data_Pool[(uint8_t)param_id_ref].Data);
					ADC_Data_Pool[(uint8_t)param_id_ref].Data_Flag = validity_status;
				}

				state = SOH;
			}
			break;

		default:
			/* Check for Valid Start of Header */
			if (*ptr_byte >= SOH1 && *ptr_byte <= SOH6)
			{
				soh = *ptr_byte;
				byte_count = 0;
				util_memset(data_buffer, 0, PACKET_SIZE);
				state = HEADER;
				validity_status = FLAG_INVALID;
			}
			else
			{
				/* If the header is not verified, packet failed return to verify new packet */
				state = SOH;
			}
			break;
		}
	}

	/* SyncableUserCode{9D076C2D-36A0-4c14-98E6-5C59543C1502} */
}

/* Operation 'da_ads_process_data' of Class 'DA_ads' */
/**
 * @brief Processes incoming data from the ADS UART interface.
 *
 * This function reads data from the UART interface associated with the ADS module
 * into the ADC_RxPacket buffer. If any bytes are read, it iterates through each byte
 * and applies the deframing logic by calling da_ads_parse_data() on each byte.
 *
 * The function is intended to be called periodically to handle incoming data streams.
 *
 * @note The function assumes that ADC_RxPacket and ADC_RX_BUFFER_SIZE are defined and accessible.
 * @note The UART_ADS identifier and uart_read() function must be properly configured.
 */
static void da_ads_process_data(void)
{
	/* SyncableUserCode{1CC49295-17BB-4466-8D04-4C79B38942A6}:Nbrlk8aPUZ */
	uint16_t bytes_read;
	uint16_t byte_id;

	/* UART Read IMU Buffer */
	bytes_read = uart_read((uint8_t)UART_ADS, &ADC_RxPacket[0], ADC_RX_BUFFER_SIZE);

	/* if bytes are read in the current cycle */
	if (bytes_read > 0)
	{
		/* Iterate through all the bytes read */
		for (byte_id = 0; byte_id < bytes_read; byte_id++)
		{
			/*Apply Deframing Logic */
			da_ads_parse_data(&ADC_RxPacket[byte_id]);
		}
	}
	/* SyncableUserCode{1CC49295-17BB-4466-8D04-4C79B38942A6} */
}

/**
 * @brief Retrieves ADC data and its validity flag for a given label.
 *
 * This function fetches the ADC data and its associated flag from the ADC data pool
 * corresponding to the specified label. If the label is invalid (i.e., out of range),
 * the function sets the data to 0.0f and the flag to FLAG_INVALID.
 *
 * @param[in]  label The ADC label index to retrieve data for.
 * @param[out] data  Pointer to a da_ads_data_t structure where the retrieved data and flag will be stored.
 */
static void da_map_adc_data_and_flag(da_ads_label_t label, da_ads_data_t *data)
{
	/* If the Label is Valid */
	if (label < MAX_ADC_LABEL_COUNT)
	{
		/* Fetch the data and the data validity flag */
		data->Data = ADC_Data_Pool[label].Data;
		data->Data_Flag = ADC_Data_Pool[label].Data_Flag;
	}
	else
	{
		/* Otherwise set the data and the data
			validity flag to default values */
		data->Data = 0.0f;
		data->Data_Flag = FLAG_INVALID;
	}
}
