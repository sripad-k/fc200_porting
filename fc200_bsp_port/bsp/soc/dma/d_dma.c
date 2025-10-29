/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : DMA

  Abstract           : DMA transfers

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-146
                       SDD References: 136T-2200-131000-001-D22 SWDES-120
                                                                SWDES-121
                                                                SWDES-122
                                                                SWDES-123
                                                                SWDES-124
                                                                SWDES-125
                                                                SWDES-126
                                                                SWDES-127
                                                                SWDES-128
\note
  CSC ID             : SWDES-49
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"
#include "soc/memory_manager/d_memory_cache.h"
#include "soc/interrupt_manager/d_int_irq_handler.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */

#include "xzdma_hw.h"
#include "d_dma_cfg.h"
#include "d_dma.h"

/***** Macros ***********************************************************/

/* Read a DMA register */
#define dmaReadReg(channel, RegOffset) \
    d_GEN_RegisterRead(dmaChannel[(channel)].baseAddress + (Uint32_t)(RegOffset))

/* Write a DMA register */
#define dmaWriteReg(channel, RegOffset, Data) \
    d_GEN_RegisterWrite((dmaChannel[(channel)].baseAddress + (Uint32_t)(RegOffset)), (Uint32_t)(Data))

/* Read the interrupt status register */
#define dmaInterruptStatus(channel) \
  dmaReadReg((channel), XZDMA_CH_ISR_OFFSET)

/* Clear the interrupt status */
#define dmaInterruptClear(channel, Mask) \
  dmaWriteReg((channel), \
  XZDMA_CH_ISR_OFFSET, ((Uint32_t)(Mask) & (Uint32_t)XZDMA_IXR_ALL_INTR_MASK))

/* Read the interrupt mask register */
#define dmaInterruptMask(channel) \
  dmaReadReg((channel), (Uint32_t)(XZDMA_CH_IMR_OFFSET))

/* Diasable specific interrupts */
#define dmaInterruptDisable(channel, Mask) \
  dmaWriteReg((channel), \
      XZDMA_CH_IDS_OFFSET, \
  ((Uint32_t)dmaReadReg((channel), \
  XZDMA_CH_IDS_OFFSET) | ((Uint32_t)(Mask) & (Uint32_t)XZDMA_IXR_ALL_INTR_MASK)))

/* Enable the DMA channel to initiate transfer */
#define dmaEnableCh(channel) \
  dmaWriteReg((channel), \
      (XZDMA_CH_CTRL2_OFFSET), (XZDMA_CH_CTRL2_EN_MASK))

/* Disable DMA channel */
#define dmaDisableCh(channel) \
  dmaWriteReg((channel),\
    (XZDMA_CH_CTRL2_OFFSET), (XZDMA_CH_CTRL2_DIS_MASK))

/***** Type Definitions *************************************************/

/* DMA Operation modes. */
typedef enum
{
  DMA_MODE_NORMAL,  /**< Normal transfer from source to destination*/
  DMA_MODE_WRONLY,  /**< Write only mode */
  DMA_MODE_RDONLY   /**< Read only mode */
} dmaMode_t;

/* AXI Burst type definitions. */
typedef enum
{
  DMA_BURST_FIXED = 0,  /**< Fixed burst type */
  DMA_BURST_INCR        /**< Increment burst type */
} dmaBurstType;

/* Channel data configuration */
typedef struct
{
  Uint8_t OverFetch;             /**< Enable Over fetch */
  Uint8_t SrcIssue;              /**< Outstanding transactions for Source */
  dmaBurstType SrcBurstType;     /**< Burst type for SRC */
  Uint8_t SrcBurstLen;           /**< AXI length for data read */
  dmaBurstType DstBurstType;     /**< Burst type for DST */
  Uint8_t DstBurstLen;           /**< AXI length for data write */
  Uint8_t SrcCache;              /**< AXI cache bits for data read */
  Uint8_t SrcQos;                /**< AXI QOS bits for data read */
  Uint8_t DstCache;              /**< AXI cache bits for data write */
  Uint8_t DstQos;                /**< AXI QOS bits for data write */
} dmaDataConfig;

/* Channel data */
typedef struct
{
  Bool_t IsReady;              /**< Device and the driver are initialized */
  dmaDataConfig DataConfig;    /**< Current configurations */
  d_DMA_State_t ChannelState;  /**< Channel state */
} dmaInstance_t;

/* Fields for data transfer */
typedef struct
{
  Pointer_t SrcAddr;      /**< Source address */
  Pointer_t DstAddr;      /**< Destination Address */
  Uint32_t Size;          /**< Size of the data to be transferred */
  Uint8_t SrcCoherent;    /**< Source coherent */
  Uint8_t DstCoherent;    /**< Destination coherent */
  Uint8_t Pause;          /**< Will pause data transmission after
                               this transfer only for SG mode */
} dmaTransfer;

/* DMA channel hardware definition */
typedef struct
{
  Uint32_t baseAddress;
  Uint32_t interruptNumber;
} dmaChannel_t;

/***** Constants ********************************************************/

static const dmaChannel_t dmaChannel[(Uint32_t)XPAR_XZDMA_NUM_INSTANCES] =
{
  {
    XPAR_PSU_ADMA_0_BASEADDR,
    XPS_ADMA_CH0_INT_ID
  },
  {
    XPAR_PSU_ADMA_1_BASEADDR,
    XPS_ADMA_CH1_INT_ID
  },
  {
    XPAR_PSU_ADMA_2_BASEADDR,
    XPS_ADMA_CH2_INT_ID
  },
  {
    XPAR_PSU_ADMA_3_BASEADDR,
    XPS_ADMA_CH3_INT_ID
  },
  {
    XPAR_PSU_ADMA_4_BASEADDR,
    XPS_ADMA_CH4_INT_ID
  },
  {
    XPAR_PSU_ADMA_5_BASEADDR,
    XPS_ADMA_CH5_INT_ID
  },
  {
    XPAR_PSU_ADMA_6_BASEADDR,
    XPS_ADMA_CH6_INT_ID
  },
  {
    XPAR_PSU_ADMA_7_BASEADDR,
    XPS_ADMA_CH7_INT_ID
  },
  {
    XPAR_PSU_GDMA_0_BASEADDR,
	XPS_ZDMA_CH0_INT_ID
  },
  {
    XPAR_PSU_GDMA_1_BASEADDR,
    XPS_ZDMA_CH1_INT_ID
  },
  {
    XPAR_PSU_GDMA_2_BASEADDR,
    XPS_ZDMA_CH2_INT_ID
  },
  {
    XPAR_PSU_GDMA_3_BASEADDR,
    XPS_ZDMA_CH3_INT_ID
  },
  {
    XPAR_PSU_GDMA_4_BASEADDR,
    XPS_ZDMA_CH4_INT_ID
  },
  {
    XPAR_PSU_GDMA_5_BASEADDR,
    XPS_ZDMA_CH5_INT_ID
  },
  {
    XPAR_PSU_GDMA_6_BASEADDR,
    XPS_ZDMA_CH6_INT_ID
  },
  {
    XPAR_PSU_GDMA_7_BASEADDR,
    XPS_ZDMA_CH7_INT_ID
  }
};

/***** Variables ********************************************************/

static dmaInstance_t channelStatus[(Uint32_t)XPAR_XZDMA_NUM_INSTANCES];

/***** Function Declarations ********************************************/

static d_Status_t dmaSetChDataConfig(const Uint32_t channel, const dmaDataConfig * const pConfigure);

static void dmaGetChDataConfig(const Uint32_t channel, dmaDataConfig * const pConfigure);

static void dmaReset(const Uint32_t channel);

static d_Status_t dmaStart(const Uint32_t channel, dmaTransfer *Data);

static void dmaEnable(const Uint32_t channel);

static void dmaSimpleMode(const Uint32_t channel, dmaTransfer *Data);

static void dmaGetConfigurations(const Uint32_t channel);

/************************** Function Definitions *****************************/

/*********************************************************************//**
  <!-- d_DMA_Initialise -->

  Initialise a DMA channel.
*************************************************************************/
d_Status_t                 /** \return Success or Failure */
d_DMA_Initialise
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  channelStatus[channel].IsReady = d_TRUE;

  channelStatus[channel].ChannelState = d_DMA_STATE_IDLE;

  dmaReset(channel);
  dmaGetConfigurations(channel);

  return d_STATUS_SUCCESS;
}

/*********************************************************************//**
  <!-- d_DMA_StartMemToMemTransfer -->

  Start a DMA PS to PS memory transfer
*************************************************************************/
d_Status_t                          /** \return Success or Failure */
d_DMA_StartMemToMemTransfer
(
const Uint32_t channel,             /**< [in] DMA channel number */
const Uint8_t * const destination,  /**< [in] Destination address */
const Uint8_t * const source,       /**< [in] Source address */
const Uint32_t byteCount            /**< [in] Number of bytes to transfer */
)
{
  d_Status_t status = d_STATUS_SUCCESS;
  dmaDataConfig dataConfig;
  dmaTransfer dmaTransfer;

  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  if (channelStatus[channel].IsReady != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (destination == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (source == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  dmaGetChDataConfig(channel, &dataConfig);

  dataConfig.DstBurstLen = 0x0Fu;
  dataConfig.DstBurstType = DMA_BURST_INCR;
  dataConfig.DstQos = 0;
  dataConfig.OverFetch = 0;
  dataConfig.SrcBurstLen = 0x0Fu;
  dataConfig.SrcBurstType = DMA_BURST_INCR;
  dataConfig.SrcIssue = 0x1Fu;
  dataConfig.SrcQos = 0;
  status = dmaSetChDataConfig(channel, &dataConfig);

  if (status == d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    d_MEMORY_DCacheFlushRange((Pointer_t)source, byteCount);
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    d_MEMORY_DCacheInvalidateRange((Pointer_t)destination, byteCount);

    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    dmaTransfer.SrcAddr = (Pointer_t)source;
    dmaTransfer.SrcCoherent = d_FALSE;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    dmaTransfer.DstAddr = (Pointer_t)destination;
    dmaTransfer.DstCoherent = d_FALSE;
    dmaTransfer.Size = byteCount;
    dmaTransfer.Pause = d_FALSE;
    status = dmaStart(channel, &dmaTransfer);
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DMA_InterruptEnable -->

  Enable the DMA interrupt in the DMA controller.
*************************************************************************/
d_Status_t                 /** \return Success or Failure */
d_DMA_InterruptEnable
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = d_INT_IrqEnable(dmaChannel[channel].interruptNumber);

  return status;
}

/*********************************************************************//**
  <!-- d_DMA_InterruptDisable -->

  Disable the DMA interrupt in the interrupt controller.
*************************************************************************/
d_Status_t                 /** \return Success or Failure */
d_DMA_InterruptDisable
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  status = d_INT_IrqDisable(dmaChannel[channel].interruptNumber);

  return status;
}

/*********************************************************************//**
  <!-- d_DMA_ChannelState -->

  Get the DMA channel status.
*************************************************************************/
d_DMA_State_t              /** \return DMA channel state */
d_DMA_ChannelState
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  d_DMA_State_t status = d_STATUS_SUCCESS;

  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (channelStatus[channel].IsReady != d_TRUE)
  {
    status = d_STATUS_NOT_INITIALISED;
  }
  else
  {
    Uint32_t value;

    value = dmaReadReg(channel, (XZDMA_CH_STS_OFFSET)) & (XZDMA_STS_ALL_MASK);

    if ((value == XZDMA_STS_DONE_MASK) || (value == XZDMA_STS_DONE_ERR_MASK))
    {
      status = d_DMA_STATE_IDLE;
    }
    else if (value == XZDMA_STS_PAUSE_MASK)
    {
      // gcov-jst 1 It is not possible to generate this condition during bench testing.
      status = d_DMA_STATE_PAUSE;
    }
    else
    {
      status = d_DMA_STATE_BUSY;
    }
  }

  return status;
}

/*********************************************************************//**
  <!-- d_DMA_SelfTest -->

  Perform check of DMA register access.
*************************************************************************/
d_Status_t                 /** \return Success or Failure */
d_DMA_SelfTest
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  d_Status_t status = d_STATUS_SUCCESS;

  if (channel >= (Uint32_t)XPAR_XZDMA_NUM_INSTANCES)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (channelStatus[channel].IsReady != d_TRUE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, channel, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  Uint32_t data;

  data = dmaReadReg(channel, XZDMA_CH_CTRL0_OFFSET);

  /* Changing DMA channel to over fetch */
  dmaWriteReg(channel, XZDMA_CH_CTRL0_OFFSET,
              (data | XZDMA_CTRL0_OVR_FETCH_MASK));

  if (((Uint32_t)dmaReadReg(channel,
      XZDMA_CH_CTRL0_OFFSET) & XZDMA_CTRL0_OVR_FETCH_MASK) !=
      XZDMA_CTRL0_OVR_FETCH_MASK)
  {
    // gcov-jst 1 It is not possible to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    DO_NOTHING();
  }
  
  /* Retrieving the change settings */
  dmaWriteReg(channel, XZDMA_CH_CTRL0_OFFSET, data);

  return status;
}

/*********************************************************************//**
  <!-- d_DMA_InterruptHandler -->

  DMA interrupt handler.
*************************************************************************/
void                       /** \return None */
d_DMA_InterruptHandler
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  Uint32_t PendingIntr;
  Uint32_t ErrorStatus;

  /* Get pending interrupts */
  PendingIntr = (Uint32_t)(dmaInterruptStatus(channel));
  PendingIntr &= (~dmaInterruptMask(channel));

  /* DMA transfer has completed */
  ErrorStatus = PendingIntr & (XZDMA_IXR_DMA_DONE_MASK);
  if ((ErrorStatus) != 0u)
  {
    dmaInterruptDisable(channel, XZDMA_IXR_ALL_INTR_MASK);
    channelStatus[channel].ChannelState = d_DMA_STATE_IDLE;
    if (d_DMA_Config[channel].doneHandler != NULL)
    {
      d_DMA_Config[channel].doneHandler(channel);
    }
  }

  /* An error has occurred */
  ErrorStatus = PendingIntr & (XZDMA_IXR_ERR_MASK);
  if ((ErrorStatus) != 0u)
  {
    // gcov-jst 25 It is not possible to generate this failure during bench testing.
    if ((ErrorStatus & XZDMA_IXR_DMA_PAUSE_MASK) == XZDMA_IXR_DMA_PAUSE_MASK)
    {
      channelStatus[channel].ChannelState = d_DMA_STATE_PAUSE;
    }
    else
    {
      if ((ErrorStatus & (XZDMA_IXR_AXI_WR_DATA_MASK |
                          XZDMA_IXR_AXI_RD_DATA_MASK |
                          XZDMA_IXR_AXI_RD_DST_DSCR_MASK |
                          XZDMA_IXR_AXI_RD_SRC_DSCR_MASK)) != 0x00u)
      {
        channelStatus[channel].ChannelState = d_DMA_STATE_IDLE;
      }
      else
      {
        DO_NOTHING();
      }
    }
    if (d_DMA_Config[channel].errorHandler != NULL)
    {
      d_DMA_Config[channel].errorHandler(channel, ErrorStatus);
    }
    else
    {
      DO_NOTHING();
    }
  }

  /* Clear pending interrupt(s) */
  dmaInterruptClear(channel, PendingIntr);

  return;
}

/*********************************************************************//**
  <!-- dmaSetChDataConfig -->

  Set the data attributes and control configurations of a DMA channel
  based on the inputs provided.
*************************************************************************/
static d_Status_t                       /** \return Success or Failure */
dmaSetChDataConfig
(
const Uint32_t channel,                 /**< [in] DMA channel number */
const dmaDataConfig * const pConfigure  /**< [in] Pointer to configuration structure */
)
{
  d_Status_t status;

  if (channelStatus[channel].ChannelState != d_DMA_STATE_IDLE)
  {
    status = d_STATUS_FAILURE;
  }
  else
  {
    Uint32_t data;

    channelStatus[channel].DataConfig.DstBurstType = pConfigure->DstBurstType;
    channelStatus[channel].DataConfig.DstBurstLen = pConfigure->DstBurstLen;
    channelStatus[channel].DataConfig.SrcBurstType = pConfigure->SrcBurstType;
    channelStatus[channel].DataConfig.SrcBurstLen = pConfigure->SrcBurstLen;
    channelStatus[channel].DataConfig.OverFetch = pConfigure->OverFetch;
    channelStatus[channel].DataConfig.SrcIssue = pConfigure->SrcIssue;
    channelStatus[channel].DataConfig.SrcCache = pConfigure->SrcCache;
    channelStatus[channel].DataConfig.SrcQos = pConfigure->SrcQos;
    channelStatus[channel].DataConfig.DstCache = pConfigure->DstCache;
    channelStatus[channel].DataConfig.DstQos = pConfigure->DstQos;

    /* Setting over fetch */
    data = dmaReadReg(channel,
        XZDMA_CH_CTRL0_OFFSET) & (~XZDMA_CTRL0_OVR_FETCH_MASK);

    data |= (((Uint32_t)(pConfigure->OverFetch) <<
        XZDMA_CTRL0_OVR_FETCH_SHIFT) &
          XZDMA_CTRL0_OVR_FETCH_MASK);

    /* Normal mode */
    data &= (~XZDMA_CTRL0_MODE_MASK);

    dmaWriteReg(channel,
          XZDMA_CH_CTRL0_OFFSET, data);

    /* Setting source issue */
    data = dmaReadReg(channel,
            XZDMA_CH_CTRL1_OFFSET) & (~XZDMA_CTRL1_SRC_ISSUE_MASK);
    data |= (Uint32_t)(pConfigure->SrcIssue & XZDMA_CTRL1_SRC_ISSUE_MASK);

    dmaWriteReg(channel,
            XZDMA_CH_CTRL1_OFFSET, data);

    /* Setting Burst length and burst type */
    data = dmaReadReg(channel,
            XZDMA_CH_DATA_ATTR_OFFSET);
    data = (data & (~(XZDMA_DATA_ATTR_ARBURST_MASK |
         XZDMA_DATA_ATTR_ARLEN_MASK |
         XZDMA_DATA_ATTR_AWBURST_MASK |
         XZDMA_DATA_ATTR_AWLEN_MASK |
         XZDMA_DATA_ATTR_ARCACHE_MASK |
         XZDMA_DATA_ATTR_AWCACHE_MASK |
         XZDMA_DATA_ATTR_AWQOS_MASK |
         XZDMA_DATA_ATTR_ARQOS_MASK)));

    data |= ((((Uint32_t)(pConfigure->SrcBurstType) <<
        XZDMA_DATA_ATTR_ARBURST_SHIFT) &
        XZDMA_DATA_ATTR_ARBURST_MASK) |
        (((Uint32_t)(pConfigure->SrcCache) <<
        XZDMA_DATA_ATTR_ARCACHE_SHIFT) &
        XZDMA_DATA_ATTR_ARCACHE_MASK) |
        (((Uint32_t)(pConfigure->SrcQos) <<
        XZDMA_DATA_ATTR_ARQOS_SHIFT) &
        XZDMA_DATA_ATTR_ARQOS_MASK) |
      (((Uint32_t)(pConfigure->SrcBurstLen) <<
        XZDMA_DATA_ATTR_ARLEN_SHIFT) &
        XZDMA_DATA_ATTR_ARLEN_MASK) |
      (((Uint32_t)(pConfigure->DstBurstType) <<
        XZDMA_DATA_ATTR_AWBURST_SHIFT) &
        XZDMA_DATA_ATTR_AWBURST_MASK) |
      (((Uint32_t)(pConfigure->DstCache) <<
        XZDMA_DATA_ATTR_AWCACHE_SHIFT) &
        XZDMA_DATA_ATTR_AWCACHE_MASK) |
      (((Uint32_t)(pConfigure->DstQos) <<
        XZDMA_DATA_ATTR_AWQOS_SHIFT) &
        XZDMA_DATA_ATTR_AWQOS_MASK) |
      (((Uint32_t)(pConfigure->DstBurstLen)) &
        XZDMA_DATA_ATTR_AWLEN_MASK));

    dmaWriteReg(channel,
          XZDMA_CH_DATA_ATTR_OFFSET, data);
    status = d_STATUS_SUCCESS;
  }

  return status;
}

/*********************************************************************//**
  <!-- dmaGetChDataConfig -->

  Get the channel configuration data.
*************************************************************************/
static void                       /** \return None */
dmaGetChDataConfig
(
const Uint32_t channel,           /**< [in] DMA channel number */
dmaDataConfig * const pConfigure  /**< [in] Pointer to configuration structure */
)
{
  pConfigure->SrcBurstType = channelStatus[channel].DataConfig.SrcBurstType;
  pConfigure->SrcCache = channelStatus[channel].DataConfig.SrcCache;
  pConfigure->SrcQos = channelStatus[channel].DataConfig.SrcQos;
  pConfigure->SrcBurstLen = channelStatus[channel].DataConfig.SrcBurstLen;

  pConfigure->DstBurstType = channelStatus[channel].DataConfig.DstBurstType;
  pConfigure->DstCache = channelStatus[channel].DataConfig.DstCache;
  pConfigure->DstQos = channelStatus[channel].DataConfig.DstQos;
  pConfigure->DstBurstLen = channelStatus[channel].DataConfig.DstBurstLen;

  pConfigure->OverFetch = channelStatus[channel].DataConfig.OverFetch;
  pConfigure->SrcIssue = channelStatus[channel].DataConfig.SrcIssue;

  return;
}

/*********************************************************************//**
  <!-- dmaReset -->

  Reset a DMA channel.
*************************************************************************/
static void                /** \return None */
dmaReset
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  /* Disable's the channel */
  dmaDisableCh(channel);

  /* Disables all interrupts */
  dmaInterruptDisable(channel, XZDMA_IXR_ALL_INTR_MASK);
  dmaInterruptClear(channel, XZDMA_IXR_ALL_INTR_MASK);

  /* All configurations are being reset */
  dmaWriteReg(channel, XZDMA_CH_CTRL0_OFFSET,
          XZDMA_CTRL0_RESET_VALUE);
  dmaWriteReg(channel, XZDMA_CH_CTRL1_OFFSET,
          XZDMA_CTRL1_RESET_VALUE);
  dmaWriteReg(channel,
    XZDMA_CH_DATA_ATTR_OFFSET, XZDMA_DATA_ATTR_RESET_VALUE);
  dmaWriteReg(channel,
    XZDMA_CH_DSCR_ATTR_OFFSET, XZDMA_DSCR_ATTR_RESET_VALUE);

  channelStatus[channel].ChannelState = d_DMA_STATE_IDLE;

  return;
}

/*********************************************************************//**
  <!-- dmaStart -->

  Start a DMA transfer.
*************************************************************************/
static d_Status_t          /** \return Success or Failure */
dmaStart
(
const Uint32_t channel,    /**< [in] DMA channel number */
dmaTransfer *pData         /**< [in] Pointer to data transfer structure */
)
{
  d_Status_t status;

  if (channelStatus[channel].ChannelState != d_DMA_STATE_IDLE)
  {
    // gcov-jst 1 It is not possible to generate this failure during bench testing.
    status = d_STATUS_FAILURE;
  }
  else
  {
    dmaSimpleMode(channel, pData);
    status = d_STATUS_SUCCESS;

    dmaEnable(channel);
  }

  return status;
}

/*********************************************************************//**
  <!-- dmaSimpleMode -->

  Sets all the required fields for initiating data transfer in simple mode.
*************************************************************************/
static void                /** \return None */
dmaSimpleMode
(
const Uint32_t channel,    /**< [in] DMA channel number */
dmaTransfer *pData         /**< [in] Pointer to data transfer structure */
)
{

  Uint32_t value;
  Uint64_t localAddr;

  dmaWriteReg(channel,
    XZDMA_CH_SRC_DSCR_WORD0_OFFSET,
      (pData->SrcAddr & XZDMA_WORD0_LSB_MASK));
  localAddr = (Uint64_t)pData->SrcAddr;
  dmaWriteReg(channel,
    XZDMA_CH_SRC_DSCR_WORD1_OFFSET,
    ((localAddr >> XZDMA_WORD1_MSB_SHIFT) &
    XZDMA_WORD1_MSB_MASK));

  dmaWriteReg(channel,
    XZDMA_CH_DST_DSCR_WORD0_OFFSET,
    (pData->DstAddr & XZDMA_WORD0_LSB_MASK));
  localAddr = (Uint64_t)pData->DstAddr;
  dmaWriteReg(channel,
    XZDMA_CH_DST_DSCR_WORD1_OFFSET,
    ((localAddr >> XZDMA_WORD1_MSB_SHIFT) &
    XZDMA_WORD1_MSB_MASK));

  dmaWriteReg(channel,
    XZDMA_CH_SRC_DSCR_WORD2_OFFSET,
    (pData->Size & XZDMA_WORD2_SIZE_MASK));
  dmaWriteReg(channel,
    XZDMA_CH_DST_DSCR_WORD2_OFFSET,
    (pData->Size & XZDMA_WORD2_SIZE_MASK));

  value = (Uint32_t)(pData->SrcCoherent & XZDMA_WORD3_COHRNT_MASK);
  dmaWriteReg(channel,
    XZDMA_CH_SRC_DSCR_WORD3_OFFSET, value);

  value = (Uint32_t)(pData->DstCoherent & XZDMA_WORD3_COHRNT_MASK);
  dmaWriteReg(channel,
      XZDMA_CH_DST_DSCR_WORD3_OFFSET, value);

}

/*********************************************************************//**
  <!-- dmaEnable -->

  Initiate data transfer.
*************************************************************************/
static void                /** \return None */
dmaEnable
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  dmaWriteReg(channel, XZDMA_CH_IEN_OFFSET, (Uint32_t)XZDMA_IXR_ERR_MASK | (Uint32_t)XZDMA_IXR_DMA_DONE_MASK);
  channelStatus[channel].ChannelState = d_DMA_STATE_BUSY;
  dmaEnableCh(channel);

  return;
}

/*********************************************************************//**
  <!-- dmaGetConfigurations -->

  Get the reset configuration.
*************************************************************************/
static void                /** \return None */
dmaGetConfigurations
(
const Uint32_t channel     /**< [in] DMA channel number */
)
{
  channelStatus[channel].DataConfig.SrcIssue = (Uint8_t)XZDMA_CTRL1_SRC_ISSUE_MASK;
  channelStatus[channel].DataConfig.SrcBurstType = DMA_BURST_INCR;
  channelStatus[channel].DataConfig.SrcBurstLen = 0xFU;
  channelStatus[channel].DataConfig.OverFetch = 1U;
  channelStatus[channel].DataConfig.DstBurstType = DMA_BURST_INCR;
  channelStatus[channel].DataConfig.DstBurstLen = 0xFU;
  channelStatus[channel].DataConfig.SrcCache = 0x2U;
  channelStatus[channel].DataConfig.DstCache = 0x2U;
  channelStatus[channel].DataConfig.SrcQos = 0x0u;
  channelStatus[channel].DataConfig.DstQos = 0x0u;

  return;
}


