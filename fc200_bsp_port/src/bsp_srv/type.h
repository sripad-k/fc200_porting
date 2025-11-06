#ifndef TYPES_H
#define TYPES_H

// include your base type definitions
#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"

// If the system stdint.h hasn't been included yet,
// define your own types
#ifndef _STDINT_H
#ifndef _SYS__STDINT_H

#ifndef _INT8_T_DECLARED
typedef Int8_t int8_t;
#define _INT8_T_DECLARED
#endif

#ifndef _UINT8_T_DECLARED
typedef Uint8_t uint8_t;
#define _UINT8_T_DECLARED
#endif

#ifndef _INT16_T_DECLARED
typedef Int16_t int16_t;
#define _INT16_T_DECLARED
#endif

#ifndef _UINT16_T_DECLARED
typedef Uint16_t uint16_t;
#define _UINT16_T_DECLARED
#endif

#ifndef _INT32_T_DECLARED
typedef Int32_t int32_t;
#define _INT32_T_DECLARED
#endif

#ifndef _UINT32_T_DECLARED
typedef Uint32_t uint32_t;
#define _UINT32_T_DECLARED
#endif

#ifndef _INT64_T_DECLARED
typedef Int64_t int64_t;
#define _INT64_T_DECLARED
#endif

#ifndef _UINT64_T_DECLARED
typedef Uint64_t uint64_t;
#define _UINT64_T_DECLARED
#endif

#endif  // _SYS__STDINT_H
#endif  // _STDINT_H

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

#endif // TYPES_H
