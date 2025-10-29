/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Memory Manager

  Abstract : Write protection of memory

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

// NOLINTBEGIN(hicpp-no-assembler)

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */
#include "soc/defines/d_common_asm.h"
#include "xreg_cortexr5.h"
#include "kernel/general/d_gen_register.h"
#include "d_memory.h"
#include "d_memory_cache.h"
#include "d_memory_cfg.h"

/***** Constants ********************************************************/

#define MPU_REGION_SIZE_MIN 0x20u

/* REGION encodings are defined in xreg_cortexr5.h */
static const struct
{
  Uint32_t size;
  unsigned int encoding;
} RegionSizeTable[] =
{
  { 0x20u, REGION_32B },
  { 0x40u, REGION_64B },
  { 0x80u, REGION_128B },
  { 0x100u, REGION_256B },
  { 0x200u, REGION_512B },
  { 0x400u, REGION_1K },
  { 0x800u, REGION_2K },
  { 0x1000u, REGION_4K },
  { 0x2000u, REGION_8K },
  { 0x4000u, REGION_16K },
  { 0x8000u, REGION_32K },
  { 0x10000u, REGION_64K },
  { 0x20000u, REGION_128K },
  { 0x40000u, REGION_256K },
  { 0x80000u, REGION_512K },
  { 0x100000u, REGION_1M },
  { 0x200000u, REGION_2M },
  { 0x400000u, REGION_4M },
  { 0x800000u, REGION_8M },
  { 0x1000000u, REGION_16M },
  { 0x2000000u, REGION_32M },
  { 0x4000000u, REGION_64M },
  { 0x8000000u, REGION_128M },
  { 0x10000000u, REGION_256M },
  { 0x20000000u, REGION_512M },
  { 0x40000000u, REGION_1G },
  { 0x80000000u, REGION_2G },
};

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

static d_XMpuConfig d_Mpu_Config;

/***** Function Declarations ********************************************/

static Uint32_t getNextMpuRegion(void);
static void updateMpuConfig(Uint32_t reg_num, Uint32_t address, Uint32_t size, Uint32_t attrib);
static void setAttribute(Uint32_t addr, Uint32_t reg_size, Int32_t reg_num, Uint32_t attrib);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_MEMORY_SetMPURegion -->

  Set the memory attributes for a section of memory in the translation table.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
d_MEMORY_SetMpuRegion
(
Uint32_t addr,           /**< [in] 32-bit address for which memory attributes need to be set. */
Uint32_t size,           /**< [in] The size of the region. */
Uint32_t attrib          /**< [in] Attribute for the given memory region. */
)
{
  d_Status_t returnVal;
  Uint32_t NextAvailableMemRegion;

  NextAvailableMemRegion = getNextMpuRegion();
  if (NextAvailableMemRegion == 0xFFu)
  {
	  returnVal = d_STATUS_FAILURE;
  }
  else
  {
    returnVal = d_MEMORY_SetMpuRegionByRegNum (NextAvailableMemRegion, addr, size, attrib);
  }

  return returnVal;
}

/*********************************************************************//**
  <!-- d_MEMORY_EnableMPU -->

  Enable MPU for Cortex R5 processor. This function invalidates I cache
  and flush the D Cache, and then enables the MPU.
*************************************************************************/
void                     /** \return None */
d_MEMORY_EnableMpu
(
void
)
{
  Uint32_t ctrlReg;
  Uint32_t reg;
  Int32_t dCacheStatus=0;
  Int32_t iCacheStatus=0;

  /* enable caches only if they are disabled */
  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  if ((ctrlReg & XREG_CP15_CONTROL_C_BIT) != 0x00000000U)
  {
    dCacheStatus=1;
  }
  else
  {
    DO_NOTHING();
  }
  
  if ((ctrlReg & XREG_CP15_CONTROL_I_BIT) != 0x00000000U)
  {
    iCacheStatus=1;
  }
  else
  {
    DO_NOTHING();
  }

  if (dCacheStatus != 0)
  {
    d_MEMORY_DCacheDisable();
  }
  else
  {
    DO_NOTHING();
  }

  if (iCacheStatus != 0)
  {
    d_MEMORY_ICacheDisable();
  }
  else
  {
    DO_NOTHING();
  }

  reg = d_mfcp(XREG_CP15_SYS_CONTROL);
  reg |= 0x00000001U;
  d_dsb();
  d_mtcp(XREG_CP15_SYS_CONTROL, reg);
  d_isb();

  /* enable caches only if they are disabled in routine*/
  if (dCacheStatus != 0)
  {
    d_MEMORY_DCacheEnable();
  }
  else
  {
    DO_NOTHING();
  }

  if (iCacheStatus != 0)
  {
    d_MEMORY_ICacheEnable();
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DisableMPU -->

  Disable MPU for Cortex R5 processors. This function invalidates I cache
  and flush the D Caches, and then disables the MPU.
*************************************************************************/
void                     /** \return None */
d_MEMORY_DisableMpu
(
void
)
{
  Uint32_t ctrlReg;
  Uint32_t reg;
  Int32_t dCacheStatus=0;
  Int32_t iCacheStatus=0;
  /* enable caches only if they are disabled */

  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  if ((ctrlReg & XREG_CP15_CONTROL_C_BIT) != 0x00000000U)
  {
    dCacheStatus=1;
  }
  else
  {
    DO_NOTHING();
  }

  if ((ctrlReg & XREG_CP15_CONTROL_I_BIT) != 0x00000000U)
  {
    iCacheStatus=1;
  }
  else
  {
    DO_NOTHING();
  }

  if (dCacheStatus != 0)
  {
    d_MEMORY_DCacheDisable();
  }
  else
  {
    DO_NOTHING();
  }

  if (iCacheStatus != 0)
  {
    d_MEMORY_ICacheDisable();
  }
  else
  {
    DO_NOTHING();
  }

  d_mtcp(XREG_CP15_INVAL_BRANCH_ARRAY, 0);
  reg = d_mfcp(XREG_CP15_SYS_CONTROL);
  reg &= ~(0x00000001U);
  d_dsb();
  d_mtcp(XREG_CP15_SYS_CONTROL, reg);
  d_isb();

  /* enable caches only if they are disabled in routine*/
  if (dCacheStatus != 0)
  {
    d_MEMORY_DCacheEnable();
  }
  else
  {
    DO_NOTHING();
  }

  if (iCacheStatus != 0)
  {
    d_MEMORY_ICacheEnable();
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_GetMPUConfig -->

  The MPU configuration table is passed to the caller.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
d_MEMORY_GetMpuConfig
(
d_XMpuConfig mpuconfig   /**< [in] This is of type XMpu_Config which is an array of
                                   16 entries of type structure representing the MPU
                                   config table. */
)
{
  if (mpuconfig == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  Uint32_t index = 0U;
  while (index < d_MEMORY_MAX_REGIONS)
  {
    mpuconfig[index].RegionStatus = d_Mpu_Config[index].RegionStatus;
    mpuconfig[index].BaseAddress = d_Mpu_Config[index].BaseAddress;
    mpuconfig[index].Attribute = d_Mpu_Config[index].Attribute;
    mpuconfig[index].Size = d_Mpu_Config[index].Size;
    index++;
  }

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_MEMORY_DisableMpuRegionByRegNum -->

  Disables the corresponding region number as passed by the user.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
d_MEMORY_DisableMpuRegionByRegNum
(
Uint32_t reg_num         /**< [in] The region number to be disabled */
)
{
  if (reg_num >= d_MEMORY_MAX_REGIONS)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, reg_num, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  d_MEMORY_DCacheFlush();
  d_MEMORY_ICacheInvalidate();

  d_mtcp(XREG_CP15_MPU_MEMORY_REG_NUMBER,reg_num);
  Uint32_t Temp = d_mfcp(XREG_CP15_MPU_REG_SIZE_EN);
  Temp &= (~REGION_EN);
  d_dsb();
  d_mtcp(XREG_CP15_MPU_REG_SIZE_EN,Temp);
  d_dsb();
  d_isb();
  updateMpuConfig(reg_num, 0U, 0U, 0U);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_MEMORY_SetMpuRegionByRegNum -->

 Enables the corresponding region number as passed by the user.
 Only allow region size up to 2GB.
*************************************************************************/
d_Status_t               /** \return Success or Failure */
d_MEMORY_SetMpuRegionByRegNum
(
Uint32_t reg_num,        /**< [in] The region number to be enabled. */
Uint32_t addr,           /**< [in] 32 bit address for start of the region. */
Uint32_t size,           /**< [in] Requested size of the region. */
Uint32_t attrib          /**< [in] Attribute for the corresponding region. */
)
{
  d_Status_t returnVal = d_STATUS_SUCCESS;
  Uint32_t localAddr = addr;

  if (reg_num >= d_MEMORY_MAX_REGIONS)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, reg_num, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  /* Check for address wrap */
  if ((addr + size) < addr)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, size, addr, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  /* Check for size too large */
  if (size > 0x80000000u)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, size, addr, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (d_Mpu_Config[reg_num].RegionStatus == MPU_REG_ENABLED)
  {
    returnVal = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }

  if (returnVal == d_STATUS_SUCCESS)
  {
    Uint32_t index;
    Uint32_t regionSize = 0u;

    d_MEMORY_DCacheFlush();
    d_MEMORY_ICacheInvalidate();

    /* Lookup the size.  */
    for (index = 0; index < (sizeof(RegionSizeTable) / sizeof(RegionSizeTable[0])); index++)
    {
      if (size <= RegionSizeTable[index].size)
      {
        regionSize = RegionSizeTable[index].encoding;
        localAddr &= ~(RegionSizeTable[index].size - 1u);
        break;
      }
      else
      {
        DO_NOTHING();
      }
    }

    setAttribute(localAddr, regionSize, (Int32_t)reg_num, attrib);

    updateMpuConfig(reg_num, localAddr, regionSize, attrib);
  }
  else
  {
    DO_NOTHING();
  }

  return returnVal;
}

/*********************************************************************//**
  <!-- d_MEMORY_InitialiseExistingMpuRegConfig -->

 Initializes the MPU configuration table that are setup in the
 R5 boot code in the Init_Mpu function called before C main.
*************************************************************************/
void                     /** \return None */
d_MEMORY_InitialiseExistingMpuRegConfig
(
void
)
{
  Uint32_t index = 0U;
  Uint32_t index1 = 0U;
  Uint32_t mPURegSize;
  Uint32_t mPURegBA;
  Uint32_t mPURegAttrib;
  Uint32_t tempSize;

  while (index < d_MEMORY_MAX_REGIONS)
  {
    d_mtcp(XREG_CP15_MPU_MEMORY_REG_NUMBER,index);
    mPURegSize = d_mfcp(XREG_CP15_MPU_REG_SIZE_EN);
    mPURegBA = d_mfcp(XREG_CP15_MPU_REG_BASEADDR);
    mPURegAttrib = d_mfcp(XREG_CP15_MPU_REG_ACCESS_CTRL);
    if ((mPURegSize & (Uint32_t)REGION_EN) != 0u)
    {
    	d_Mpu_Config[index].RegionStatus = MPU_REG_ENABLED;
    	d_Mpu_Config[index].BaseAddress = mPURegBA;
    	d_Mpu_Config[index].Attribute = mPURegAttrib;
      tempSize = mPURegSize & (~REGION_EN);
      tempSize >>= 1u;
      for (index1 = 0; index1 < (sizeof (RegionSizeTable) / sizeof (RegionSizeTable[0])); index1++)
      {
        if (tempSize <= RegionSizeTable[index1].encoding)
        {
        	d_Mpu_Config[index].Size = RegionSizeTable[index1].size;
          break;
        }
        else
        {
          DO_NOTHING();
        }
      }
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_InitMpu -->

  Initialise MPU.
*************************************************************************/
void                     /** \return None */
d_MEMORY_InitMpu
(
void
)
{
  Uint32_t index;
  Uint32_t temp;

  if (d_MEMORY_RegionCount > d_MEMORY_MAX_REGIONS)
  {
    // gcov-jst 3 It is not practical to execute this code during bench testing.
    d_ERROR_Logger(d_STATUS_INVALID_CONFIGURATION, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, d_MEMORY_RegionCount, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  d_MEMORY_DisableMpu();

  /* Disable all regions */
  for (index = 0; index < d_MEMORY_MAX_REGIONS; index++)
  {
    d_mtcp(XREG_CP15_MPU_MEMORY_REG_NUMBER, index);
    temp = d_mfcp(XREG_CP15_MPU_REG_SIZE_EN);
    temp &= (~REGION_EN);
    d_dsb();
    d_mtcp(XREG_CP15_MPU_REG_SIZE_EN, temp);
    d_dsb();
    d_isb();
  }

  /* Set regions as per configuration data */
  for (index = 0; index < d_MEMORY_RegionCount; index++)
  {
    setAttribute(d_MEMORY_Configuration[index].baseAddress, d_MEMORY_Configuration[index].sizeCode, (Int32_t)index, d_MEMORY_Configuration[index].attribute);
  }

  return;
}

/*********************************************************************//**
  <!-- getNextMpuRegion -->

  Returns the next available free MPU region.
*************************************************************************/
static Uint32_t          /** \return The next available free MPU region */
getNextMpuRegion
(
void
)
{
  Uint32_t index = 0U;
  Uint32_t nextAvailableReg = 0xFF;

  while (index < d_MEMORY_MAX_REGIONS)
  {
    if (d_Mpu_Config[index].RegionStatus != MPU_REG_ENABLED)
    {
      nextAvailableReg = index;
      break;
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }
  
  return nextAvailableReg;
}

/*********************************************************************//**
  <!-- updateMpuConfig -->

  Update the MPU configuration for the requested region number in the
  global MPU configuration table.
*************************************************************************/
static void              /** \return None */
updateMpuConfig
(
Uint32_t reg_num,        /**< [in] The requested region number to be updated information for. */
Uint32_t address,        /**< [in] 32 bit address for start of the region. */
Uint32_t size,           /**< [in] Requested size of the region. */
Uint32_t attrib          /**< [in] Attribute for the corresponding region. */
)
{
  Uint32_t tempSize = size;

  if ((size & (Uint32_t)REGION_EN) != 0u)
  {
    Uint32_t index;
    d_Mpu_Config[reg_num].RegionStatus = MPU_REG_ENABLED;
    d_Mpu_Config[reg_num].BaseAddress = address;
    tempSize &= (~REGION_EN);
    tempSize >>= 1u;
    /* Lookup the size.  */
    for (index = 0; index < (sizeof(RegionSizeTable) / sizeof(RegionSizeTable[0])); index++)
    {
      if (tempSize <= RegionSizeTable[index].encoding)
      {
        d_Mpu_Config[reg_num].Size = RegionSizeTable[index].size;
        break;
      }
      else
      {
        DO_NOTHING();
      }
    }
    d_Mpu_Config[reg_num].Attribute = attrib;
  }
  else
  {
    d_Mpu_Config[reg_num].RegionStatus = 0U;
    d_Mpu_Config[reg_num].BaseAddress = 0U;
    d_Mpu_Config[reg_num].Size = 0U;
    d_Mpu_Config[reg_num].Attribute = 0U;
  }

  return;
}

/*********************************************************************//**
  <!-- setAttribute -->

  Set the memory attributes for a section of memory with starting address addr
  of the region size defined by reg_size having attributes attrib of region number
  reg_num.
*************************************************************************/
static void              /** \return None */
setAttribute
(
// cppcheck-suppress misra-c2012-2.7; Parameter is used in macro function. which is not visible to cppcheck.
Uint32_t addr,           /**< [in] Address for which attributes are to be set. */
Uint32_t reg_size,       /**< [in] The size for that memory region. */
// cppcheck-suppress misra-c2012-2.7; Parameter is used in macro function. which is not visible to cppcheck.
Int32_t reg_num,         /**< [in] The region number to be set. */
// cppcheck-suppress misra-c2012-2.7; Parameter is used in macro function. which is not visible to cppcheck.
Uint32_t attrib          /**< [in] The attributes for that memory region. */
)
{
  if ((reg_size < REGION_32B) || (reg_size > REGION_4G))
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, reg_size, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return;
  }

  Uint32_t local_reg_size = reg_size;

  local_reg_size = local_reg_size << 1u;
  local_reg_size |= REGION_EN;
  d_dsb();
  d_mtcp(XREG_CP15_MPU_MEMORY_REG_NUMBER, reg_num);
  d_isb();
  d_mtcp(XREG_CP15_MPU_REG_BASEADDR, addr); 		/* Set base address of a region */
  d_mtcp(XREG_CP15_MPU_REG_ACCESS_CTRL, attrib); 	/* Set the control attribute */
  d_mtcp(XREG_CP15_MPU_REG_SIZE_EN, local_reg_size);	/* set the region size and enable it*/
  d_dsb();
  d_isb();						/* synchronize context on this processor */

  return;
}

/*********************************************************************//**
  <!-- Xil_SetTlbAttributes -->

  Set the memory attributes for a section covering 1MB, of memory
  in the translation table.
  This function is retained with Xilinx name as it is called by the LWIP stack.
*************************************************************************/
void                     /** \return None */
Xil_SetTlbAttributes
(
const Uint32_t addr,     /**< [in] Address for which attributes are to be set. */
const Uint32_t attrib    /**< [in] The attributes for that memory region. */
)
{
  Uint32_t Localaddr = addr;
  Localaddr &= (~(0xFFFFFU));
  /* Setting the MPU region with given attribute with 1MB size */
  (void)d_MEMORY_SetMpuRegion(Localaddr, 0x100000u, attrib);
  
  return;
}

