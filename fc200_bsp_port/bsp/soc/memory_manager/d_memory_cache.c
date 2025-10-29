/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Memory Cache Manager

  Abstract : Memory cache control

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-153
                       SDD References: 136T-2200-131000-001-D22 SWDES-139
                                                                SWDES-140
                                                                SWDES-141
                                                                SWDES-142
                                                                SWDES-143
                                                                SWDES-144
                                                                SWDES-145
                                                                SWDES-146
                                                                SWDES-147

\note
  CSC ID             : SWDES-51
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/defines/d_common_asm.h"
#include "xreg_cortexr5.h"
#include "kernel/general/d_gen_register.h"
#include "d_memory_cache.h"

#define asm_inval_dc_line_mva_poc(param) __asm__ __volatile__("mcr " \
    XREG_CP15_INVAL_DC_LINE_MVA_POC :: "r" (param))

#define asm_clean_inval_dc_line_sw(param) __asm__ __volatile__("mcr " \
    XREG_CP15_CLEAN_INVAL_DC_LINE_SW :: "r" (param))

#define asm_clean_inval_dc_line_mva_poc(param) __asm__ __volatile__("mcr " \
    XREG_CP15_CLEAN_INVAL_DC_LINE_MVA_POC :: "r" (param))

/***** Constants ********************************************************/

#define IRQ_FIQ_MASK 0xC0u  /* Mask IRQ and FIQ interrupts in cpsr */

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

static void dCacheFlushLine(Pointer_t address);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_MEMORY_DCacheEnable -->

  Enable the Data cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_DCacheEnable
(
void
)
{
  register Uint32_t ctrlReg;

  /* enable caches only if they are disabled */
  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  if ((ctrlReg & XREG_CP15_CONTROL_C_BIT) == 0x00000000U)
  {
    /* invalidate the Data cache */
    d_MEMORY_DCacheInvalidate();

    /* enable the Data cache */
    ctrlReg |= (XREG_CP15_CONTROL_C_BIT);
    d_mtcp(XREG_CP15_SYS_CONTROL, ctrlReg);
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DCacheDisable -->

  Disable the Data cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_DCacheDisable
(
void
)
{
  register Uint32_t ctrlReg;

  /* clean and invalidate the Data cache */
  d_MEMORY_DCacheFlush();

  /* disable the Data cache */
  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  ctrlReg &= ~(XREG_CP15_CONTROL_C_BIT);
  d_mtcp(XREG_CP15_SYS_CONTROL, ctrlReg);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DCacheInvalidate -->

  Invalidate the entire Data cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_DCacheInvalidate
(
void
)
{
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  d_mtcp(XREG_CP15_CACHE_SIZE_SEL, 0);

  /*invalidate all D cache*/
  d_mtcp(XREG_CP15_INVAL_DC_ALL, 0);

  /* Restore the PSR */
  d_mtcpsr(currmask);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DCacheInvalidateRange -->

  Invalidate a portion of Data cache.
*************************************************************************/
void                      /** \return None */
d_MEMORY_DCacheInvalidateRange
(
const Pointer_t address,  /**< [in] Address of memory to invalidate */
Uint32_t length           /**< [in] Length of memory to invalidate */
)
{
  const Uint32_t cacheline = 32U;
  Uint32_t tempadr = address;
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  if (length != 0U)
  {
    Uint32_t end;
    Uint32_t tempend;

    end = tempadr + length;
    tempend = end;
    /* Select L1 Data cache in CSSR */
    d_mtcp(XREG_CP15_CACHE_SIZE_SEL, 0U);

    if ((tempadr & (cacheline-1U)) != 0U)
    {
      tempadr &= (~(cacheline - 1U));

      dCacheFlushLine(tempadr);
    }
    else
    {
      DO_NOTHING();
    }

    if ((tempend & (cacheline-1U)) != 0U)
    {
      tempend &= (~(cacheline - 1U));

      dCacheFlushLine(tempend);
    }
    else
    {
      DO_NOTHING();
    }

    while (tempadr < tempend)
    {
      /* Invalidate Data cache line */
      asm_inval_dc_line_mva_poc(tempadr);

      tempadr += cacheline;
    }
  }
  else
  {
    DO_NOTHING();
  }

  d_dsb();
  d_mtcpsr(currmask);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DCacheFlush -->

  Flush the entire Data cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_DCacheFlush
(
void
)
{
  register Uint32_t CsidReg;
  Uint32_t CacheSize;
  Uint32_t LineSize;
  Uint32_t NumWays;
  Uint32_t Way;
  Uint32_t WayIndex;
  Uint32_t Set;
  Uint32_t SetIndex;
  Uint32_t NumSet;
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  /* Select cache level 0 and D cache in CSSR */
  d_mtcp(XREG_CP15_CACHE_SIZE_SEL, 0);

  CsidReg = d_mfcp(XREG_CP15_CACHE_SIZE_ID);

  /* Determine Cache Size */
  CacheSize = (CsidReg >> 13U) & 0x000001FFU;
  CacheSize += 0x00000001U;
  CacheSize *= (Uint32_t)128;    /* to get number of bytes */

  /* Number of Ways */
  NumWays = (CsidReg & 0x000003ffU) >> 3U;
  NumWays += 0x00000001U;

  /* Get the cacheline size, way size, index size from csidr */
  LineSize = (CsidReg & 0x00000007U) + 0x00000004U;

  NumSet = CacheSize/NumWays;
  NumSet /= (0x00000001U << LineSize);

  Way = 0U;
  Set = 0U;

  /* Invalidate all the cachelines */
  for (WayIndex = 0U; WayIndex < NumWays; WayIndex++)
  {
    for (SetIndex = 0U; SetIndex < NumSet; SetIndex++)
    {
       register Uint32_t C7Reg = Way | Set;
      /* Flush by Set/Way */
      asm_clean_inval_dc_line_sw(C7Reg);

      Set += (0x00000001U << LineSize);
    }
    Set = 0U;
    Way += 0x40000000U;
  }

  /* Wait for flush to complete */
  d_dsb();

  /* Restore the PSR */
  d_mtcpsr(currmask);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_DCacheFlushRange -->

  Flush a portion of Data cache.
*************************************************************************/
void                      /** \return None */
d_MEMORY_DCacheFlushRange
(
const Pointer_t address,  /**< [in] Address of memory to flush */
Uint32_t length           /**< [in] Length of memory to flush */
)
{
  Uint32_t LocalAddr = address;
  const Uint32_t cacheline = 32U;
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  if (length != 0x00000000u)
  {
    Uint32_t end;
    /* Back the starting address up to the start of a cache line
     * perform cache operations until adr+len
     */
    end = LocalAddr + length;
    LocalAddr &= ~(cacheline - 1U);

    while (LocalAddr < end)
    {
      /* Flush Data cache line */
      asm_clean_inval_dc_line_mva_poc(LocalAddr);

      LocalAddr += cacheline;
    }
  }
  else
  {
    DO_NOTHING();
  }

  d_dsb();
  d_mtcpsr(currmask);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_ICacheEnable -->

  Enable the instruction cache.
*************************************************************************/
void                      /** \return None */
d_MEMORY_ICacheEnable
(
void
)
{
  register Uint32_t ctrlReg;

  /* enable caches only if they are disabled */
  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  if ((ctrlReg & XREG_CP15_CONTROL_I_BIT) == 0x00000000U)
  {
    /* invalidate the instruction cache */
    d_mtcp(XREG_CP15_INVAL_IC_POU, 0);

    /* enable the instruction cache */
    ctrlReg |= (XREG_CP15_CONTROL_I_BIT);
    d_mtcp(XREG_CP15_SYS_CONTROL, ctrlReg);
  }
  else
  {
    DO_NOTHING();
  }

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_ICacheDisable -->

  Disable the instruction cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_ICacheDisable
(
void
)
{
  register Uint32_t ctrlReg;

  d_dsb();

  /* invalidate the instruction cache */
  d_mtcp(XREG_CP15_INVAL_IC_POU, 0);

  /* disable the instruction cache */
  ctrlReg = d_mfcp(XREG_CP15_SYS_CONTROL);
  ctrlReg &= ~(XREG_CP15_CONTROL_I_BIT);
  d_mtcp(XREG_CP15_SYS_CONTROL, ctrlReg);

  return;
}

/*********************************************************************//**
  <!-- d_MEMORY_ICacheInvalidate -->

  Invalidate the entire instruction cache.
*************************************************************************/
void                     /** \return None */
d_MEMORY_ICacheInvalidate
(
void
)
{
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  d_mtcp(XREG_CP15_CACHE_SIZE_SEL, 1);

  /* invalidate the instruction cache */
  d_mtcp(XREG_CP15_INVAL_IC_POU, 0);

  /* Wait for invalidate to complete */
  d_dsb();

  /* Restore the PSR */
  d_mtcpsr(currmask);

  return;
}

/*********************************************************************//**
  <!-- dCacheFlushLine -->

  Flush a Data cache line.
*************************************************************************/
static void              /** \return None */
dCacheFlushLine
(
// cppcheck-suppress misra-c2012-2.7; Parameter is used in macro function. which is not visible to cppcheck.
Pointer_t address        /**< [in] Address of line to flush */
)
{
  Uint32_t currmask;

  currmask = d_mfcpsr();
  d_mtcpsr(currmask | IRQ_FIQ_MASK);

  d_mtcp(XREG_CP15_CACHE_SIZE_SEL, 0);

  d_mtcp(XREG_CP15_CLEAN_INVAL_DC_LINE_MVA_POC, (address & (~0x1Fu)));

  /* Wait for flush to complete */
  d_dsb();
  d_mtcpsr(currmask);

  return;
}

