/****************************************************
 *  da_sbus.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_sbus
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_DA_SBUS
#define H_DA_SBUS

#include "types_sbus.h"
#include "da_interface.h"

#define SBUS_FRAME_SIZE (25)
#define SBUS_DATA_LENGTH_IN_BYTE SBUS_FRAME_SIZE - 2

typedef struct rc_input_s
{
    bool Sbus_Active;                  //!< true if SBUS is being used to control the vehicle
    bool Sbus_Link;                    //!< true if SBUS linked false if link loss
    float Sbus[SBUS_MAX_CHANNELS];     //!< SBUS data channels                         [-1, 1]
    uint8_t Sbus_Ch_17;                //!< Boolean to indicate if channel 17 is used
    uint8_t Sbus_Ch_18;                //!< Boolean to indicate if channel 18 is used
    bool Sbus_Failsafe;                //!< Used for GCS SBUS monitoring
    bool Sbus_Lost_Frame;              //!< Used for GCS SBUS monitoring
    int8_t SA, SB, SC, SD, SE, SF;     //!< SBUS Selector Channels
    uint32_t Sbus_Frames_Lost_Counter; //!< Used for GCS SBUS monitoring
    uint32_t Sbus_Counter;             //!< Used for GCS SBUS monitoring
} rc_input_s;

/* Operation 'da_sbus_init' of Class 'DA_sbus' */
bool da_sbus_init(void);

/* Operation 'da_sbus_read_periodic' of Class 'DA_sbus' */
void da_sbus_read_periodic(void);

bool da_get_sbus_latest_frame(rc_input_s *ptr_sbus_frame);

#endif /* H_DA_SBUS */
