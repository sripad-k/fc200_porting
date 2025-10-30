// #include "stdint.h"
//#include "stdbool.h"
#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/general/d_gen_string.h"

typedef Int8_t      int8_t;
typedef Uint8_t     uint8_t;
typedef Int16_t     int16_t;
typedef Uint16_t    uint16_t;
typedef Int32_t     int32_t;
typedef Uint32_t    uint32_t;
typedef Int64_t     int64_t;
typedef Uint64_t    uint64_t;

typedef Bool_t bool;           // typedef the type
#define false d_FALSE          // alias the literal
#define true d_TRUE            // alias the literal

// 8-bit types
#define INT8_MAX    127
#define INT8_MIN    (-128)
#define UINT8_MAX   255

// 16-bit types
#define INT16_MAX   32767
#define INT16_MIN   (-32768)
#define UINT16_MAX  65535

// 32-bit types
#define INT32_MAX   2147483647
#define INT32_MIN   (-2147483648)
#define UINT32_MAX  4294967295U

// 64-bit types
#define INT64_MAX   9223372036854775807LL
#define INT64_MIN   (-9223372036854775807LL - 1)
#define UINT64_MAX  18446744073709551615ULL
