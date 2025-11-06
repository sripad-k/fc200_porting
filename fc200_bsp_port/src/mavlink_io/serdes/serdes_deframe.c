#include "serdes_deframe.h"
#include "serdes_crc.h"
#include <string.h>

// State machine for deframing
static uint8_t deframe_buffer[1031];  // start + len(2) + id + payload + crc + end
static uint16_t deframe_index = 0;
static int deframe_state = 0;  // 0: start, 1: len1, 2: len2, 3: id+payload, 4: crc1, 5: crc2, 6: end
static uint16_t payload_len = 0;

uint8_t serdes_deframe(uint8_t byte, uint8_t *buffer, uint16_t buffer_size) {
    switch (deframe_state) {
        case 0:  // Wait for start byte
            if (byte == 0x7E) {
                deframe_buffer[0] = byte;
                deframe_index = 1;
                deframe_state = 1;
            }
            return 0;  // No valid message yet
        case 1:  // Wait for length byte 1 (MSB)
            deframe_buffer[1] = byte;
            deframe_index = 2;
            deframe_state = 2;
            return 0;
        case 2:  // Wait for length byte 2 (LSB)
            deframe_buffer[2] = byte;
            payload_len = (deframe_buffer[1] << 8) | deframe_buffer[2];
            if (payload_len == 0 || buffer_size < payload_len + 6 || payload_len > 1024) {
                deframe_state = 0;
                deframe_index = 0;
                return 0;
            }
            deframe_index = 3;
            deframe_state = 3;
            return 0;
        case 3:  // Wait for id + payload
            deframe_buffer[deframe_index++] = byte;
            if (deframe_index == payload_len + 3) {  // id + payload received
                deframe_state = 4;
            }
            return 0;
        case 4:  // Wait for CRC first byte
            deframe_buffer[deframe_index++] = byte;
            deframe_state = 5;
            return 0;
        case 5:  // Wait for CRC second byte
            deframe_buffer[deframe_index++] = byte;
            deframe_state = 6;
            return 0;
        case 6:  // Wait for end byte
            if (byte == 0xAA) {
                deframe_buffer[deframe_index++] = byte;  // Store end byte
                // Verify CRC
                uint16_t received_crc = (deframe_buffer[deframe_index - 3] << 8) | deframe_buffer[deframe_index - 2];
                uint16_t calculated_crc = calculate_crc16(deframe_buffer, payload_len + 3);
                if (received_crc == calculated_crc) {
                    // Copy full message to output buffer
                    memcpy(buffer, deframe_buffer, payload_len + 6);
                    uint8_t msg_id = deframe_buffer[3];  // Return message ID
                    deframe_state = 0;  // Reset state
                    deframe_index = 0;
                    return msg_id;
                }
            }
            // Invalid end byte or CRC, reset
            deframe_state = 0;
            deframe_index = 0;
            return 0;
        default:
            deframe_state = 0;
            deframe_index = 0;
            return 0;
    }
}