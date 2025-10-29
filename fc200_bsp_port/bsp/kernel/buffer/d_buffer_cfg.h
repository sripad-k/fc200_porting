/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : Buffer

  Abstract           : Buffer driver module

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-145
                       SDD References: 136T-2200-131100-001-D22 SWDES-200
                                                                SWDES-201
                                                                SWDES-202
                                                                SWDES-203
                                                                SWDES-204
                                                                SWDES-205
                                                                SWDES-206
                                                                SWDES-207
                                                                SWDES-208
                                                                SWDES-209

\note
  CSC ID             : SWDES-80
*************************************************************************/

#ifndef BUFFER_CFG_H
#define BUFFER_CFG_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* Fixed entry link buffers */
/* Buffer status */
typedef struct
{
  Uint32_t indexIn;      /* Index in buffer for next write */
  Uint32_t indexOut;     /* Index in buffer for next read */
  Uint32_t entryCount;   /* Number of entries in buffer */
  Uint32_t highWater;    /* Largest number of entries in buffer */
} BufferFixedStatus_t;

/* Buffer definition */
typedef struct
{
  Uint8_t * pBuffer;                         /* Buffer */
  BufferFixedStatus_t * pBufferFixedStatus;  /* Pointer to status structure */
  Uint32_t entrySize;                        /* Entry size */
  Uint32_t entryMax;                         /* Maximum number of entries */
} d_BUFFER_FixedDef_t;

/* Variable length buffers */
/* Buffer status */
typedef struct
{
  Uint32_t bufferIndexIn;       /* Index in buffer for next write */
  Uint32_t bufferUsedBytes;     /* Number of bytes of buffer used */
  Uint32_t indexIn;             /* Index in entries for next write */
  Uint32_t indexOut;            /* Index in entries for next read */
  Uint32_t entryCount;          /* Number of entries in buffer */
  Uint32_t highWater;           /* Largest number of entries in buffer */
} BufferVarStatus_t;

/* Item status */
typedef struct
{
  Uint32_t itemIndex;           /* Index in buffer of the message */
  Uint32_t itemSize;            /* Length of the message */
} BufferVarItem_t;

/* Buffer definition */
typedef struct
{
  Uint8_t * pBuffer;                     /* Buffer */
  BufferVarStatus_t * pBufferVarStatus;  /* Pointer to status structure */
  BufferVarItem_t * pBufferVarItem;      /* Pointer to items structure */
  Uint32_t entryMax;                     /* Maximum number of entries */
  Uint32_t totalSize;                    /* Total buffer size */
} d_BUFFER_VarDef_t;

// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_BUFFER_FixedDef_t d_BUFFER_FixedDef[];
// cppcheck-suppress misra-c2012-8.11; The constant array is defined by configuration data and is unknown to the driver. Violation of 'Advisory' rule does not present a risk.
extern const d_BUFFER_VarDef_t d_BUFFER_VarDef[];

/***** Macros (Inline Functions) Definitions ****************************/

// cppcheck-suppress misra-c2012-20.10; Use of the ## operator provides a simple method of defining configuration data. Violation of 'Advisory' rule does not present a risk.
#define d_BUFFER_FIXED(name, entrySize, entries)     \
static Uint8_t (name##_buffer[entries][entrySize]);  \
static BufferFixedStatus_t (name##_bufferFixedStatus);

// cppcheck-suppress misra-c2012-20.10; Use of the ## operator provides a simple method of defining configuration data. Violation of 'Advisory' rule does not present a risk.
#define d_BUFFER_FIXED_ENTRY(name) \
{                                 \
  &(name##_buffer)[0][0],         \
  &(name##_bufferFixedStatus),    \
  sizeof((name##_buffer)[0]),     \
  sizeof(name##_buffer)/ sizeof((name##_buffer)[0])  \
} 

#define d_BUFFER_FIXED_COUNT const Uint32_t bufferFixedCount = (sizeof(d_BUFFER_FixedDef) / sizeof(d_BUFFER_FixedDef_t))

// cppcheck-suppress misra-c2012-20.10; Use of the ## operator provides a simple method of defining configuration data. Violation of 'Advisory' rule does not present a risk.
#define d_BUFFER_VAR(name, totalSize, entries)     \
static Uint8_t (name##_buffer)[totalSize];         \
static BufferVarStatus_t (name##_bufferVarStatus); \
static BufferVarItem_t ((name##_bufferVarItem)[entries]);

// cppcheck-suppress misra-c2012-20.10; Use of the ## operator provides a simple method of defining configuration data. Violation of 'Advisory' rule does not present a risk.
#define d_BUFFER_VAR_ENTRY(name) \
{                               \
  &(name##_buffer[0]),          \
  &(name##_bufferVarStatus),    \
  &(name##_bufferVarItem[0]),   \
  (sizeof(name##_bufferVarItem) / sizeof(BufferVarItem_t)),   \
  sizeof(name##_buffer)         \
} 

#define d_BUFFER_VAR_COUNT const Uint32_t bufferVarCount = (sizeof(d_BUFFER_VarDef) / sizeof(d_BUFFER_VarDef_t))

/***** Variables ********************************************************/

extern const Uint32_t bufferFixedCount;
extern const Uint32_t bufferVarCount;

/***** Function Declarations ********************************************/

#endif /* BUFFER_CFG_H */
