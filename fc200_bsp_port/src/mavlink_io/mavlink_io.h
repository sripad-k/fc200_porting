/****************************************************
 *  mavlink_io.h
 *  Created on: 07-Jul-2025 12:41:23 PM
 *  Implementation of the Class mavlink_io
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_MAVLINK_IO
#define H_MAVLINK_IO

#include "mavlink_io_interface.h"
#include "mavlink_io_types.h"

mavio_in_t MavioIn;
mavio_out_t MavioOut;

pil_in_t PilIn;
pil_out_t PilOut;

union lodd_custom_mode_t
{
    struct
    {
        uint16_t flags;
        uint8_t main_mode;
        uint8_t sub_mode;
    };
    uint32_t data;
};

#endif /* H_MAVLINK_IO */