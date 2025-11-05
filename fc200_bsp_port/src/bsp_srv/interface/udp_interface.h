/****************************************************
 *  udp_interface.h                                         
 *  Created on: 03-Nov-2025 14:42:36                      
 *  Implementation of the Interface udp_interface       
 *  Copyright: LODD (c) 2025                    
 ****************************************************/

#ifndef H_UDP_INTERFACE
#define H_UDP_INTERFACE

#include "type.h"

typedef enum 
{
	UDP_SRC_INVALID = -1,
	UDP_SRC_REDUND_FCS = 0,
	UDP_SRC_GCS = 1,
	UDP_SRC_PIL = 2,
	UDP_SRC_RPI = 3
}udp_source_t;


void udp_setup_server(void);
void udp_sync_periodic(void);
void udp_send_gcs(const uint8_t *buffer, uint32_t len);
void udp_send_rpi(const uint8_t *buffer, uint32_t len);
void udp_send_pil(const uint8_t *buffer, uint32_t len);
void udp_receive(uint8_t *buffer, uint32_t* len, udp_source_t udp_source);


#endif /*!defined(H_UDP_INTERFACE)*/
 
