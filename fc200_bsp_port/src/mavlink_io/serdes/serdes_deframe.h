#ifndef SERDES__DEFRAME_H
#define SERDES__DEFRAME_H

#include <stdint.h>

// Deframe function to process byte stream and return message ID
// Returns: msg_id (1-255) on success, 0 on incomplete/invalid message
uint8_t serdes_deframe(uint8_t byte, uint8_t *buffer, uint16_t buffer_size);

#endif // SERDES__DEFRAME_H