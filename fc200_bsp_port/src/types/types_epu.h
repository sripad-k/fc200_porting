#ifndef TYPES_EPU_H
#define TYPES_EPU_H

#include "type.h"

// Number of ESCs on bus
#define NUM_ESCS 8

typedef enum
{
    INVALID_ESC_ID = 0x00, // Invalid ESC ID
    ESC_ID_1 = 0x01,
    ESC_ID_2 = 0x02,
    ESC_ID_3 = 0x03,
    ESC_ID_4 = 0x04,
    ESC_ID_5 = 0x05,
    ESC_ID_6 = 0x06,
    ESC_ID_7 = 0x07,
    ESC_ID_8 = 0x08,
    MAX_ESCS

} e_esc_id_t;

typedef struct
{
    /* Status word with bitfield union */
    union
    {
        uint32_t raw_status; // Bytes 0-3: Status bits
        struct
        {
            uint32_t overvoltage : 1;
            uint32_t undervoltage : 1;
            uint32_t overcurrent : 1;
            uint32_t throttle_loss : 1;
            uint32_t throttle_abnormal : 1;
            uint32_t mos_overtemp : 1;
            uint32_t cap_overtemp : 1;
            uint32_t stall : 1;

            uint32_t op_amp_fault : 1;
            uint32_t up_bridge_fault : 1;
            uint32_t low_bridge_fault : 1;
            uint32_t encoder_error : 1;

            uint32_t mode : 4;
            uint32_t encoder_value : 16; // Bits 16–31
        };
    } status_bits;

    float voltage_f16;     // Bytes 4–5
    float current_f16;     // Bytes 6–7
    float temperature_f16; // Bytes 8–9

    int32_t rpm;      // Bytes 10–11: RPM (18 bits)
    uint8_t throttle; // Bytes 12–13: Throttle (7 bits)
    uint8_t index;    // Bytes 14–15: Index (5 bits)
    bool valid;       // Flag to indicate if this frame is valid

} s_esc_status_frame_t;

/**
 * @brief Electric Motor Data Structure
 */
typedef struct motor_s
{
    union
    {
        uint8_t value; //!< The entire 8-bit value representing the motor status (0 == All good)
        struct
        {
            bool comm_err : 1;  //!< [Bit 0] Communication  flag,
            bool P_err : 1;     //!< [Bit 1] reserved (Power loss error) true := Power < 20W
            bool resv_2 : 1;    //!< [Bit 2] reserved error flag
            bool resv_3 : 1;    //!< [Bit 3] reserved error flag
            bool usr_1 : 1;     //!< [Bit 4] user defined   flag
            bool usr_2 : 1;     //!< [Bit 5] user defined   flag
            bool usr_3 : 1;     //!< [Bit 6] user defined   flag
            bool pfbit_err : 1; //!< [Bit 7] pre-flight check OK
        } __attribute__((packed));
    } status;            //!< Zero if no Error
    bool spin;           //!< true if clockwise spin
    uint16_t c_bytes;    //!< Command in counts                  [ ]
    uint16_t pwm;        //!< Command in PWM (for RC Servos)     [ms]
    float T;             //!< Commanded Thrust                   [N]
    float c_RPM;         //!< Commanded RPM                      [RPM]
    float c_Q;           //!< Commanded Torque                   [Nm]
    float c_val;         //!< Commanded Value                    [0,1]
    float idle_RPM;      //!< Idle RPM                           [RPM]
    float idle_val;      //!< Idle Value                         [0,1]
    float max_RPM;       //!< Max RPM when in flight             [RPM]
    float min_RPM;       //!< Min RPM when in flight             [RPM]
    float max_val;       //!< Max Throttle when in flight        [0,1]
    float min_val;       //!< Min Throttle when in flight        [0,1]
    float V;             //!< Motor Voltage                      [V]
    float I;             //!< Motor Current                      [A]
    float Q;             //!< Motor Torque^1                     [Nm]
    float RPM;           //!< Actual Motor RPM                   [RPM]
    float val;           //!< Actual Value                       [0,1]
    float Temp;          //!< Actual Temperature of motor        [Celsius]
    uint32_t statusbyte; //!< T-Motor Status byte                [ ]
} motor_s;

typedef struct
{
    float motor_cmd_cval[NUM_ESCS]; //!< Commanded degrees for each servo [deg]
} std_epu_cmd_t;

#endif // TYPES_EPU_H