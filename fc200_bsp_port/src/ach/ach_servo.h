/****************************************************
 *  ach_servo.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class ach_servo
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_ACH_SERVO
#define H_ACH_SERVO

#include "ach_interface.h"

#define DATA_LEN (4)
#define MSG_LEN (10)
#define KST_SUCCESS_OFFSET (0x40)
#define KST_ERROR_OFFSET (0xC0)

/**
 * @brief Command Codes of KST Servo Motor.
 *        Placed in RS485 massage at Byte[3]
 *
 */
typedef enum
{
    KST_CMD_START_REPORT = 0x01,          // start report
    KST_CMD_STOP_REPORT = 0x02,           // stop report
    KST_CMD_REPORT_INTERVAL = 0x07,       // set interval between messages
    KST_CMD_SET_NODE_ID = 0x08,           // set Node ID / Number (by default ID = 0x25)
    KST_CMD_GET_NODE_ID = 0x18,           // returns current node ID / Node Number
    KST_CMD_SAVE = 0x0F,                  // Save after: set Node ID || Baud rate change || range changes
    KST_CMD_SET_BAUDRATE = 0x06,          // baud rate setting (default: 1152000)
    KST_CMD_SET_POSITION = 0x09,          // command new Position
    KST_CMD_GET_POSITION = 0x19,          // read current Position
    KST_CMD_SET_MID_POINT = 0x29,         // command current position as mid point (fine tune mid point)
    KST_CMD_DEVICE_STATUS = 0x15,         // read device current [A] & work Temp in [Celsius]
    KST_CMD_SET_NEG_RANGE = 0x2B,         // set negative range of servo (SAVE needed)
    KST_CMD_SET_POS_RANGE = 0x2D,         // set positive range of servo (SAVE needed)
    KST_CMD_GET_NEG_RANGE = 0x2C,         // get negative range
    KST_CMD_GET_POS_RANGE = 0x2E,         // get positive range
    KST_CMD_GET_SW_VERSION = 0x1A,        // get Software version
    KST_CMD_GET_FAILSAFE_INTERVAL = 0x3A, // read FAILSAFE interval in [ms] 0: disabled; 100-500ms
    KST_CMD_SET_FAILSAFE_INTERVAL = 0x1F, // set interval: 0: disabled, 100-500ms
    KST_CMD_GET_FAILSAFE_POS = 0x3B,      // read failsafe position
    KST_CMD_SET_FAILSAFE_POS = 0x14,      // set failsafe position
    KST_CMD_WATCHDOG_FAILSAFE = 0x37,     // FCC sends periodic msg to avoid failsafe enabling
    KST_CMD_FACTORY_RESET = 0x11,         // Restore factory configuration
    KST_CMD_GET_REV_LOGIC = 0x3F,         // read motion reversed & position read rev.
    KST_CMD_SET_REV_LOGIC = 0x3E,         // set function for motion reversed & position read rev.

} ach_servo_kst_cmd_codes_t;

/**
 * @brief Recievable Return Codes of KST Servo Motor.
 *        Placed in RS485 massage at Byte[3]
 *
 */
typedef enum
{
    // set Node ID (by default ID = 0x25)
    KST_RECEIVE_CMD_SET_NODE_ID = KST_CMD_SET_NODE_ID + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_NODE_ID = KST_CMD_GET_NODE_ID + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_BAUDRATE = KST_CMD_SET_BAUDRATE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_REPORT_INTERVAL = KST_CMD_REPORT_INTERVAL + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_POSITION = KST_CMD_GET_POSITION + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_MID_POINT = KST_CMD_SET_MID_POINT + KST_SUCCESS_OFFSET,
    KST_RECEIVE_AUTO_REPORT = 0x40, // Report format from device
    KST_RECEIVE_CMD_DEVICE_STATUS = KST_CMD_DEVICE_STATUS + KST_SUCCESS_OFFSET,
    KST_RECEIVE_LOW_VOLT_ALARM = 0x42,
    KST_RECEIVE_CMD_SET_NEG_RANGE = KST_CMD_SET_NEG_RANGE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_POS_RANGE = KST_CMD_SET_POS_RANGE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_NEG_RANGE = KST_CMD_GET_NEG_RANGE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_POS_RANGE = KST_CMD_GET_POS_RANGE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_SW_VERSION = KST_CMD_GET_SW_VERSION + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_FAILSAFE_INTERVAL = KST_CMD_GET_FAILSAFE_INTERVAL + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_FAILSAFE_INTERVAL = KST_CMD_SET_FAILSAFE_INTERVAL + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_FAILSAFE_POS = KST_CMD_GET_FAILSAFE_POS + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_FAILSAFE_POS = KST_CMD_SET_FAILSAFE_POS + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_WATCHDOG_FAILSAFE = KST_CMD_WATCHDOG_FAILSAFE + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_FACTORY_RESET = KST_CMD_FACTORY_RESET + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_GET_REV_LOGIC = KST_CMD_GET_REV_LOGIC + KST_SUCCESS_OFFSET,
    KST_RECEIVE_CMD_SET_REV_LOGIC = KST_CMD_SET_REV_LOGIC + KST_SUCCESS_OFFSET,

} ach_servo_kst_rx_codes_t;

/**
 * @brief status of decoded msg:
 *          1: decode success
 *          0: pending
 *         <0: error
 */
typedef enum
{
    RS485_PENDING = 0,       /**< No complete message has been decoded yet*/
    RS485_RETURN_OK = 1,     /**< Msg is complete and decoded*/
    RS485_RETURN_READ = 2,   /**< Msg was read into buffer */
    RS485_DECODE_ERROR = -1, /**< An error happened during the decoding of the message */
    RS485_RETURN_ERROR = -2, /**< An error returned from device */

} ach_servo_kst_rs485_msg_status_t;

/**
 * @brief status struct for KST Servo
 *  containg Temperature & Current of device
 */
typedef struct
{
    float current; // current in [mA]
    float temp;    // temperature in [C]

} ach_servo_kst_status_t;

typedef struct
{
    uint8_t X1; // bit[6]: Logic rev.; bit[5]=low Volt.
    uint8_t X2; // bit[0]: motion reverse

} ach_servo_kst_logic_t;

/**
 * @brief position report of device
 */
typedef struct
{
    float position; // position in [deg]

} ach_servo_kst_report_t;

typedef struct
{
    char sw[5]; // position in [deg]

} ach_servo_kst_sw_t;

/**
 * @brief returned value of device, which was requested
 */
typedef struct kst_return_t
{
    float value; // decoded value
} ach_servo_kst_return_t;

/**
 * @brief low voltage warning of device (ID)
 */
typedef struct kst_warn_t
{
    bool low_volt; // low voltage detected == true
} ach_servo_kst_warn_t;

/** Decoded RS485 air data message sent by a KST Servo*/
typedef struct
{
    ach_servo_kst_rs485_msg_status_t msg_status; // message status
    ach_servo_kst_rx_codes_t receive_code;       // data type received
    e_servo_positions_t ID;                      // Device Number / ID: 0..255 (default: 0x25)
    union
    {
        ach_servo_kst_report_t report;   // receive_code: RS485_RETURNED_REPORT
        ach_servo_kst_status_t status;   // receive_code: RS485_RETURNED_STATUS
        ach_servo_kst_return_t decoded;  // receive_code: RS485_RETURNED_DATA
        ach_servo_kst_warn_t warning;    // receive_code: RS485_LOW_VOLT_WARN
        ach_servo_kst_logic_t rev_logic; // logical bytes to read / set reverse motion
        ach_servo_kst_sw_t sw_array;
    };

} kst_rs485_msg_t;

bool ach_servo_init(void);
void ach_servo_read_periodic(void);
void ach_cmd_servo_periodic(void);
void ach_servo_powerup_test(void);

#endif /* H_ACH_SERVO */
