#ifndef TYPES_SERVO_H
#define TYPES_SERVO_H

#include "type.h"

#define FLAPERON_OUT_LH_MIN (-31.0f) /* SERVO ID - 2*/
#define FLAPERON_OUT_LH_MAX (31.0f)

#define FLAPERON_IN_LH_MIN (-31.0f) /* SERVO ID - 3*/
#define FLAPERON_IN_LH_MAX (31.0f)

#define FLAP_LH_MIN (-39.0f) /* SERVO ID - 4*/
#define FLAP_LH_MAX (1.0f)

#define FLAP_RH_MIN (-1.0f) /* SERVO ID - 5*/
#define FLAP_RH_MAX (39.0f)

#define FLAPERON_IN_RH_MIN (-31.0f) /* SERVO ID - 6*/
#define FLAPERON_IN_RH_MAX (31.0f)

#define FLAPERON_OUT_RH_MIN (-31.0f) /* SERVO ID - 7*/
#define FLAPERON_OUT_RH_MAX (31.0f)

#define RUDDER_LH_MIN (-26.0f) /* SERVO ID - 9*/
#define RUDDER_LH_MAX (26.0f)

#define RUDDER_RH_MIN (-26.0f) /* SERVO ID - 10*/
#define RUDDER_RH_MAX (26.0f)

#define ELEVATOR_LH_MIN (-31.0f) /* SERVO ID - 11*/
#define ELEVATOR_LH_MAX (31.0f)

#define ELEVATOR_RH_MIN (-31.0f) /* SERVO ID - 12*/
#define ELEVATOR_RH_MAX (31.0f)

#define STEERING_MIN (-45.0f) /* SERVO ID - 13*/
#define STEERING_MAX (45.0f)

/**
 * @brief Electric Actuator Data Structure
 */
typedef struct servo_s
{
    bool status;         //!< Actuator Status true = ALL Good
    bool flag;           //!< true if new message is received
    bool comm;           //!< True Servo communication established
    bool low_volt_alarm; //!< Low Voltage Alarm Triggered
    unsigned counter;    //!< Generic Counter
    uint16_t c_bytes;    //!< Command in bytes                   [ ]
    uint16_t pwm;        //!< Command in PWM (for RC Servos)     [ms]
    int8_t sign;         //!< Rotation definition of servo, CCW:-1, CW:1  [-]
    float c_deg;         //!< Commanded degrees                  [deg]
    float c_val;         //!< Commanded Value                    [0,1]
    float V;             //!< Actuator Voltage                   [V]
    float I;             //!< Actuator Current                   [A]
    float T;             //!< Actuator Temperature               [deg C]
    float Q;             //!< Actuator Torque                    [Nm]
    float deg;           //!< Actual degrees                     [deg]
    float val;           //!< Actual Value                       [0,1]
    float lb_deg;        //!< Degrees Lower Bound                [deg]
    float ub_deg;        //!< Degrees Upper Bound                [deg]
    float rate;          //!< Actuator Rate                      [deg/s]
} servo_s;

typedef enum
{
    BROADCAST_SERVO = 0,
    AILERON_L = 1,
    FLAPERON_OUTER_LH = 2,
    FLAPERON_INNER_LH = 3,
    FLAP_LH = 4,
    FLAP_RH = 5,
    FLAPERON_INNER_RH = 6,
    FLAPERON_OUTER_RH = 7,
    AILERON_R = 8,
    RUDDER_LH = 9,
    RUDDER_RH = 10,
    ELEVATOR_LH = 11,
    ELEVATOR_RH = 12,
    STEERING = 13,
    MAX_CNTRL_SURFACE

} e_servo_positions_t;

typedef struct
{
    float servo_cmd_deg[MAX_CNTRL_SURFACE]; //!< Commanded degrees for each servo [deg]
} std_servo_cmd_t;

#endif // TYPES_SERVO_H