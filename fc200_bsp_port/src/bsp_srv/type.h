#ifndef TYPES_H
#define TYPES_H

// ===================================================================
// COMPLETELY BLOCK ALL stdint.h VARIANTS FROM BEING INCLUDED
// ===================================================================
#ifndef _STDINT_H
#define _STDINT_H
#endif

#ifndef _SYS__STDINT_H
#define _SYS__STDINT_H
#endif

#ifndef _GCC_STDINT_H
#define _GCC_STDINT_H
#endif

#ifndef _STDINT
#define _STDINT
#endif

#ifndef __STDINT_H
#define __STDINT_H
#endif

// This is the key one for newlib (used by Xilinx toolchain)
#ifndef _SYS__STDINT_H_
#define _SYS__STDINT_H_
#endif

// include your base type definitions
#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"
#include "printf.h"

// ===================================================================
// Prevent double typedef by checking if already defined
// ===================================================================
#ifndef _INT8_T
typedef Int8_t   int8_t;
#define _INT8_T
#endif

#ifndef _UINT8_T
typedef Uint8_t  uint8_t;
#define _UINT8_T
#endif

#ifndef _INT16_T
typedef Int16_t  int16_t;
#define _INT16_T
#endif

#ifndef _UINT16_T
typedef Uint16_t uint16_t;
#define _UINT16_T
#endif

#ifndef _INT32_T
typedef Int32_t  int32_t;
#define _INT32_T
#endif

#ifndef _UINT32_T
typedef Uint32_t uint32_t;
#define _UINT32_T
#endif

#ifndef _INT64_T
typedef Int64_t  int64_t;
#define _INT64_T
#endif

#ifndef _UINT64_T
typedef Uint64_t uint64_t;
#define _UINT64_T
#endif

// ===================================================================
// Pointer-sized integers (32-bit for ARM Cortex-R5)
// ===================================================================
#ifndef _INTPTR_T
typedef int32_t  intptr_t;
#define _INTPTR_T
#endif

#ifndef _UINTPTR_T
typedef uint32_t uintptr_t;
#define _UINTPTR_T
#endif

#ifndef bool
typedef Bool_t bool;
#endif

#ifndef false
#define false d_FALSE
#endif

#ifndef true
#define true  d_TRUE
#endif

// Integer limits
#ifndef INT8_MAX
#define INT8_MAX    (127)
#endif

#ifndef INT8_MIN
#define INT8_MIN    (-128)
#endif

#ifndef UINT8_MAX
#define UINT8_MAX   (255)
#endif

#ifndef INT16_MAX
#define INT16_MAX   (32767)
#endif

#ifndef INT16_MIN
#define INT16_MIN   (-32768)
#endif

#ifndef UINT16_MAX
#define UINT16_MAX  (65535)
#endif

#ifndef INT32_MAX
#define INT32_MAX   (2147483647)
#endif

#ifndef INT32_MIN
#define INT32_MIN   (-2147483648)
#endif

#ifndef UINT32_MAX
#define UINT32_MAX  (4294967295U)
#endif

#ifndef INT64_MAX
#define INT64_MAX   (9223372036854775807LL)
#endif

#ifndef INT64_MIN
#define INT64_MIN   (-9223372036854775807LL - 1)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX  (18446744073709551615ULL)
#endif

// Pointer limits (32-bit architecture)
#ifndef INTPTR_MAX
#define INTPTR_MAX  INT32_MAX
#endif

#ifndef INTPTR_MIN
#define INTPTR_MIN  INT32_MIN
#endif

#ifndef UINTPTR_MAX
#define UINTPTR_MAX UINT32_MAX
#endif

// Size type
#ifndef _SIZE_T
typedef unsigned int size_t;
#define _SIZE_T
#endif

// Signed size type
#ifndef _SSIZE_T
typedef int ssize_t;
#define _SSIZE_T
#endif

#endif // TYPES_H
