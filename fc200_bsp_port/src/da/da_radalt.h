/****************************************************
 *  da_radalt.h
 *  Created on: 07-Jul-2025 3:53:26 PM
 *  Implementation of the Class da_radalt
 *  Copyright: LODD (c) 2025
 ****************************************************/
#ifndef H_DA_RADALT
#define H_DA_RADALT

#include "da_interface.h"
#include "uart_interface.h"
#include "types_radalt.h"

/* Operation 'da_radalt_init' of Class 'DA_radalt' */
bool da_radalt_init(void);

/* Operation 'da_radalt_read_periodic' of Class 'DA_radalt' */
void da_radalt_read_periodic(void);

#endif /* H_DA_RADALT */
