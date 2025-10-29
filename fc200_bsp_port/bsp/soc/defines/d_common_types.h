/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Common Type Definitions

  Abstract           : Define general types for Cortex R5.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-181
                       SDD References: 136T-2200-131000-001-D22 SWDES-13
\note
  CSCI_ID            : SWDES-12
*************************************************************************/

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

/***** Includes *********************************************************/

/***** Constants ********************************************************/

#define NULL ((void *)0)

/* Allow decision coverage by always including an 'else' block */
#ifdef COVERAGE
  #define DO_NOTHING() __asm__ __volatile__("NOP")
#else
  #define DO_NOTHING()
#endif

/* This macro is as above but includes the 'else' to minimise impact on complexity */
#ifdef COVERAGE
  #define ELSE_DO_NOTHING \
    else \
    { \
      __asm__ __volatile__("NOP"); \
    }
#else
  #define ELSE_DO_NOTHING 
#endif

/* Unused parameter definition used to prevent static analysis violation */
#define UNUSED_PARAMETER(x) (void)(x)

#define d_MAC_ADDRESS_LENGTH (6u)

/***** Type Definitions *************************************************/

typedef enum
{
  d_FALSE = 0,
  d_TRUE = 1
} Bool_t;

typedef char               Char_t;          // 8-bit character
typedef signed char        Int8_t;          // 8-bit signed integer
typedef unsigned char      Uint8_t;         // 8-bit unsigned integer
typedef signed short       Int16_t;         // 16-bit signed integer
typedef unsigned short     Uint16_t;        // 16-bit unsigned integer
typedef int                Int32_t;         // 32-bit signed integer
typedef unsigned int       Uint32_t;        // 32-bit unsigned integer
typedef long long          Int64_t;         // 64-bit signed integer
typedef unsigned long long Uint64_t;        // 64-bit unsigned integer
typedef float              Float32_t;       // 32-bit float
typedef double             Float64_t;       // 64-bit float

typedef Uint32_t           Pointer_t;       // Pointer size

/* MAC address type */
typedef Uint8_t d_MacAddress_t[d_MAC_ADDRESS_LENGTH];

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

#endif /* COMMON_TYPES_H */
