/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : MPU region definition

  Abstract           : This is a template for the MEMORY CSC definition
                       of the MPU regions.
                       The file should be copied to the application code
                       space, updated for the required buffers, and renamed
                       to a c file.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-153
                       SDD References: 136T-2200-131000-001-D22 SWDES-129
                                                                SWDES-130
                                                                SWDES-131
                                                                SWDES-132
                                                                SWDES-133
                                                                SWDES-134
                                                                SWDES-135
                                                                SWDES-136
                                                                SWDES-137
                                                                SWDES-138

\note
  CSC ID             : SWDES-51
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/memory_manager/d_memory_cfg.h"

#include "xreg_cortexr5.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

__attribute__((weak)) const d_MEMORY_Configuration_t d_MEMORY_Configuration[] =
{
  {
    /* Read only memory for code and constants */
    0x00000000u,                             /* Base address */
    REGION_1G,                               /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    NORM_NSHARED_WB_WA | PRIV_RO_USER_RO     /* Attribute */
  },
  {
    /* Data memory */
    0x40000000u,                             /* Base address */
    REGION_1G,                               /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    NORM_NSHARED_WB_WA | PRIV_RW_USER_RW     /* Attribute */
  },
  {
    /* 512 MB of strongly ordered memory from 0xA0000000 to 0xBFFFFFFF for PL */
    0xA0000000u,                             /* Base address */
    REGION_512M,                             /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    STRONG_ORDERD_SHARED | PRIV_RW_USER_RW   /* Attribute */
  },
  {
    /* 1M of device memory from 0xF9000000 to 0xF90FFFFF for RPU_A53_GIC */
    0xF9000000u,                             /* Base address */
    REGION_1M,                               /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    DEVICE_NONSHARED | PRIV_RW_USER_RW       /* Attribute */
  },
  {
    /* 16M of device memory from 0xFD000000 to 0xFDFFFFFF for FPS slaves */
    0xFD000000u,                             /* Base address */
    REGION_16M,                              /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    DEVICE_NONSHARED | PRIV_RW_USER_RW       /* Attribute */
  },
  {
    /* 16M of device memory from 0xFE000000 to 0xFEFFFFFF for Upper LPS slaves */
    0xFE000000u,                             /* Base address */
    REGION_16M,                              /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    DEVICE_NONSHARED | PRIV_RW_USER_RW       /* Attribute */
  },
  {
    /* 16M of device memory from 0xFF000000 to 0xFFFFFFFF for Lower LPS slaves */
    0xFF000000u,                             /* Base address */
    REGION_16M,                              /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    DEVICE_NONSHARED | PRIV_RW_USER_RW       /* Attribute */
  },
  {
    /* 256K of OCM RAM from 0xFFFC0000 to 0xFFFFFFFF marked as normal memory */
    0xFFFC0000u,                             /* Base address */
    REGION_256K,                             /* Size */
    // cppcheck-suppress misra-c2012-12.2; The left hand operand of the shift is a Uint32. The definition cannot be made specific as this is a Xilinx generated header file.
    NORM_NSHARED_WB_WA | PRIV_RW_USER_RW     /* Attribute */
  }
};

__attribute__((weak)) d_MEMORY_REGION_COUNT;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

