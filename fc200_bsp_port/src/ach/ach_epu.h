/****************************************************
 *  ach_epu.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class ach_epu
 *  Copyright: LODD (c) 2025
 ****************************************************/

#ifndef H_ACH_EPU
#define H_ACH_EPU

#include "ach_interface.h"

// Max number of CAN frames that can be buffered (tune as needed)
#define CAN_FRAME_RING_SIZE 32

// Number of CAN frames per UAVCAN ESC Status message
#define FRAMES_PER_ESC_MSG 3

// Total payload in UAVCAN ESC Status message (as per your deframe, 2+7+2 = 11, but your memcpy is 14, check this)
#define ESC_STATUS_PAYLOAD_SIZE 14

// Each CAN frame can carry up to 8 bytes of data (CAN 2.0 standard maximum DLC is 8 bytes)
#define CAN_FRAME_SIZE 8

bool ach_epu_init(void);
void ach_epu_read_periodic(void);
void ach_cmd_epu_periodic(void);

#endif
