#ifndef SERDES__CRC_H
#define SERDES__CRC_H

#include <stdint.h>

// CRC16 calculation function
uint16_t calculate_crc16(const uint8_t *data, uint16_t len);

#endif // SERDES__CRC_H