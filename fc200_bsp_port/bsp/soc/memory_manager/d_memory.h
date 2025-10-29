/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Memory Manager

  Abstract : Write protection of memory

  Software Structure : SDD References: 136T-2200-131100-001-D22 SWDES-???

  Refer to xreg_cortexr5.h for region attribute literals.

*************************************************************************/

#ifndef D_MEMORY_H
#define D_MEMORY_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

#define MPU_REG_DISABLED		0U
#define MPU_REG_ENABLED			1U

#define d_MEMORY_MAX_REGIONS  16u

/***** Type Definitions *************************************************/

typedef struct
{
  Uint32_t RegionStatus;  /* Enabled or disabled */
  Uint32_t BaseAddress;   /* MPU region base address */
  Uint32_t Size;          /* MPU region size address */
  Uint32_t Attribute;     /* MPU region size attribute */
} d_XMpuConfig_t;

typedef d_XMpuConfig_t d_XMpuConfig[d_MEMORY_MAX_REGIONS];

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Set the memory attributes for a section of memory in the translation table */
d_Status_t d_MEMORY_SetMpuRegion(Uint32_t addr, Uint32_t size, Uint32_t attrib);

/* Memory Protection Unit enable */
void d_MEMORY_EnableMpu(void);

/* Memory Protection Unit disable */
void d_MEMORY_DisableMpu(void);

/* The MPU configuration table is passed to the caller */
d_Status_t d_MEMORY_GetMpuConfig(d_XMpuConfig mpuconfig);

/* Disables the corresponding region number as passed by the user */
d_Status_t d_MEMORY_DisableMpuRegionByRegNum (Uint32_t reg_num);

/* Enables the corresponding region number as passed by the user */
d_Status_t d_MEMORY_SetMpuRegionByRegNum (Uint32_t reg_num, Uint32_t addr, Uint32_t size, Uint32_t attrib);

/* Initializes the MPU configuration table cased on setup in the
   R5 boot code in the Init_Mpu function called before C main */
void d_MEMORY_InitialiseExistingMpuRegConfig(void);

/* Initialise MPU */
void d_MEMORY_InitMpu(void);

void Xil_SetTlbAttributes(const Uint32_t addr, const Uint32_t attrib);

#endif /* D_MEMORY_H */
