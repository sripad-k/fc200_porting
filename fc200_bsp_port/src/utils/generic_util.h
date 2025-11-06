/*
 * ***************************************************
 * File: generic_util.h
 *
 * Created: 2025-05-15
 * ***************************************************
 */
#ifndef H_GENERIC_UTIL
#define H_GENERIC_UTIL

#include "type.h"

bool util_ascii_hex_to_float(const unsigned char *hex_string, float *ptr_float_data);

bool util_ascii_hex_to_uword(const char *hex_string, uint16_t *ptr_int_data);

void *util_memset(void *ptr, int value, uint16_t num);

void *util_memcpy(void *dest, const void *src, uint16_t num);

int util_memcmp(const void *ptr1, const void *ptr2, uint16_t num);

void util_uint32_to_hex_string(uint32_t value, char *str);

void util_uint16_to_hex_string(uint16_t value, char *str);

void util_uint8_to_hex_string(uint8_t value, char *str);

#endif /* H_GENERIC_UTIL */