/*
 * ***************************************************
 * File: generic_util.c
 *
 * Created: 2025-05-15
 * ***************************************************
 */

#include "generic_util.h"

/******************************************************************************
 * @brief   Converts an ASCII hexadecimal string to a float value.
 *
 * This function takes a null-terminated ASCII string representing a 32-bit
 * hexadecimal value and converts it to a float (IEEE 754 single-precision).
 * The result is stored at the location pointed to by ptr_float_data.
 *
 * @param[in]  hex_string       Pointer to a null-terminated ASCII hex string
 *                             (e.g., "40490FDB" for 3.1415927f).
 * @param[out] ptr_float_data   Pointer to a float variable where the result
 *                             will be stored.
 *
 * @return    true  if the conversion was successful and the input string was valid.
 * @return    false if the input string contains invalid characters or pointers are NULL.
 *
 * @note      The function does not handle optional "0x" prefixes.
 * @note      Assumes the system uses IEEE 754 single-precision float representation.
 ******************************************************************************/
bool util_ascii_hex_to_float(const unsigned char *hex_string, float *ptr_float_data)
{
    bool is_valid = true;
    uint32_t int_representation = 0;
    uint16_t index;

    if ((hex_string != NULL) && (ptr_float_data != NULL))
    {
        /* Iterate through the hexadecimal string */
        for (index = 0; hex_string[index] != '\0'; index++)
        {
            char ch = hex_string[index];
            uint8_t nibble;

            if (ch >= '0' && ch <= '9')
            {
                nibble = ch - '0';
            }
            else if (ch >= 'A' && ch <= 'F')
            {
                nibble = ch - 'A' + 10;
            }
            else if (ch >= 'a' && ch <= 'f')
            {
                nibble = ch - 'a' + 10;
            }
            else
            {
                /* Handle invalid characters (optional: return an error value) */
                is_valid = false;
            }

            /* Shift the existing integer representation and add the new nibble */
            int_representation = (int_representation << 4) | nibble;
        }

        /* If Valid, then Assign Pointer to float data with 32 bit value */
        if (true == is_valid)
        {
            /* Interpret the integer representation as a float (assuming IEEE 754 single-precision) */
            *ptr_float_data = *((float *)&int_representation);
        }
    }
    else
    {
        is_valid = false;
    }

    return is_valid;
}

/**
 ******************************************************************************
 * @brief      Converts an ASCII hexadecimal string to a 16-bit unsigned integer.
 * @param[in]  hex_string     Pointer to a null-terminated ASCII string representing a hexadecimal number.
 * @param[out] ptr_int_data   Pointer to a uint16_t variable where the converted value will be stored.
 * @retval     true           Conversion was successful and the string was a valid hexadecimal number.
 * @retval     false          Conversion failed due to invalid characters in the input string.
 *
 * @details
 * This function iterates through the input hexadecimal string, converts each
 * character to its corresponding 4-bit value, and accumulates the result into
 * a 16-bit unsigned integer. If any character in the string is not a valid
 * hexadecimal digit ('0'-'9', 'A'-'F', 'a'-'f'), the function sets the output
 * pointer only if all characters are valid, and returns false otherwise.
 *
 * Example:
 *   const char *hex = "1A3F";
 *   uint16_t value;
 *   bool result = ascii_hex_to_uword(hex, &value);
 *   // value == 0x1A3F if result is true
 ******************************************************************************
 */
bool util_ascii_hex_to_uword(const char *hex_string, uint16_t *ptr_int_data)
{
    bool is_valid = true;
    uint16_t int_representation = 0;
    int index;

    /* Iterate through the hexadecimal string */
    for (index = 0; hex_string[index] != '\0'; index++)
    {
        char ch = hex_string[index];
        uint8_t nibble;

        if (ch >= '0' && ch <= '9')
        {
            nibble = ch - '0';
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            nibble = ch - 'A' + 10;
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            nibble = ch - 'a' + 10;
        }
        else
        {
            // Handle invalid characters (optional: return an error value)
            is_valid = false;
        }

        /* Shift the existing integer representation and add the new nibble */
        int_representation = (int_representation << 4) | nibble;
    }

    if (true == is_valid)
    {
        /* Assign Pointer to float data with 32 bit value */
        *ptr_int_data = *((uint16_t *)&int_representation);
    }

    return is_valid;
}

/**
 * @brief Sets a block of memory to a specified value.
 * !!Compliant to MISRA 2012 and Part of our configuration control!!
 *
 * This function fills the first 'num' bytes of the memory area
 * pointed to by 'ptr' with the constant byte 'value'.
 *
 * @param ptr   Pointer to the memory area to be set.
 * @param value Value to be set. Only the least significant byte is used.
 * @param num   Number of bytes to be set.
 * @return      The original pointer 'ptr'.
 */
void *util_memset(void *ptr, int value, uint16_t num)
{
    unsigned char *p = ptr;
    while (num--)
    {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

/**
 * @brief Copies a block of memory from source to destination.
 * !!Compliant to MISRA 2012 and Part of our configuration control!!
 *
 * This function copies 'num' bytes from the memory area pointed to by 'src'
 * to the memory area pointed to by 'dest'.
 *
 * @param dest Pointer to the destination memory area.
 * @param src  Pointer to the source memory area.
 * @param num  Number of bytes to copy.
 * @return     The original pointer 'dest'.
 */
void *util_memcpy(void *dest, const void *src, uint16_t num)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (num--)
    {
        *d++ = *s++;
    }
    return dest;
}

/* Write own memcmp function */
/**
 * @brief Compares two blocks of memory.
 * !!Compliant to MISRA 2012 and Part of our configuration control!!
 *
 * This function compares the first 'num' bytes of the memory areas
 * pointed to by 'ptr1' and 'ptr2'.
 *
 * @param ptr1 Pointer to the first memory area.
 * @param ptr2 Pointer to the second memory area.
 * @param num  Number of bytes to compare.
 * @return     An integer less than, equal to, or greater than zero if
 *             the first 'num' bytes of 'ptr1' is found, respectively,
 *             to be less than, equal to, or greater than the first 'num'
 *             bytes of 'ptr2'.
 */
int util_memcmp(const void *ptr1, const void *ptr2, uint16_t num)
{
    const unsigned char *p1 = ptr1;
    const unsigned char *p2 = ptr2;

    while (num--)
    {
        if (*p1 != *p2)
        {
            return (*p1 < *p2) ? -1 : 1;
        }
        p1++;
        p2++;
    }
    return 0; // Memory blocks are equal
}