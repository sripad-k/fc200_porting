#include "serdes_crc.h"

// CRC16-CCITT calculation function
uint16_t calculate_crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;  // CRC-16-CCITT polynomial
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}