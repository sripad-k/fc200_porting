#ifndef TYPES_SBUS_H
#define TYPES_SBUS_H

#include "type.h"

#define SBUS_MAX_CHANNELS 16

typedef enum
{
    RC_SW_POS_LOW = 0, //!< Switch is in low position
    RC_SW_POS_MID = 1, //!< Switch is in middle position
    RC_SW_POS_HIGH = 2 //!< Switch is in high position
} rc_sw_position_t;

typedef struct
{
    bool link_lost;    //!< true if the SBUS link is lost
    bool data_timeout; //!< true if the SBUS data has timed out
    float rssi;        //!< RSSI value in percentage, range [0, 100] %

    float axis_r;   //!< Roll channel value [-1, 1]
    float axis_p;   //!< Pitch channel value [-1, 1]
    float axis_y;   //!< Yaw channel value [-1, 1]
    float axis_t;   //!< Throttle channel value [-1, 1]
    float slider_r; //!< Pusher channel value [-1, 1]
    float slider_l; //!< Arm channel value [-1, 1]
    float knob_r;   //!< Pusher channel value [-1, 1]
    float knob_l;   //!< Arm channel value [-1, 1]

    rc_sw_position_t sw_a; //!< Switch a position
    rc_sw_position_t sw_b; //!< Switch b position
    rc_sw_position_t sw_c; //!< Switch c position
    rc_sw_position_t sw_d; //!< Switch d position
    rc_sw_position_t sw_e; //!< Switch e position
    rc_sw_position_t sw_f; //!< Switch f position

} rc_input_t;

#endif // TYPES_SBUS_H