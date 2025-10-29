/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_sata

  Abstract           : SOC SATA controller interface

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-247
                       SDD References: 136T-2200-131000-001-D22 SWDES-542
                                                                SWDES-700
                                                                SWDES-701
                                                                SWDES-702
                                                                SWDES-703
                                                                SWDES-704
                                                                SWDES-705
                                                                SWDES-706
                                                                SWDES-707
                                                                SWDES-708
                                                                SWDES-709
                                                                SWDES-710
\note
  CSC ID             : SWDES-48
*************************************************************************/

/***** Includes *********************************************************/

#include "kernel/error_handler/d_error_handler.h"
#include "kernel/general/d_gen_register.h"
#include "kernel/general/d_gen_memory.h"
#include "soc/timer/d_timer.h"

#include "d_sata.h"
#include "d_sata_ops.h"

#include "xil_printf.h"

/***** Constants ********************************************************/

static const Uint32_t SATA_PORT = 0u;  // the Devkit SATA port is connected to the port1 on the Ultrascale

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

// cppcheck-suppress misra-c2012-8.9; Defining constants at module level ensures initial value set when excuting after reset from Vitis. Violation of 'Advisory' rule does not present a risk.
static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- WaitForSataLink -->

  Wait for the SATA link to come up

*************************************************************************/
static d_Status_t
WaitForSataLink
(void)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t registerValue;
  const Uint32_t SATA_LINK_TIME_OUT_IN_MSEC = 200u;
  Uint32_t startTime = d_TIMER_ReadValueInTicks();
  do
  {
    registerValue = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
    if ((registerValue & PORT_SSTS_DET_MASK) == PORT_STS_DET_PHYRDY)
    {
      xil_printf("SATA Link on port %d up\n\r", SATA_PORT);
      break;
    }
    ELSE_DO_NOTHING

  } while (d_TIMER_ElapsedMilliseconds(startTime, NULL) < SATA_LINK_TIME_OUT_IN_MSEC);

  registerValue = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
  if ((registerValue & PORT_SSTS_DET_MASK) != PORT_STS_DET_PHYRDY)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    xil_printf("SATA Link timeout on port %d!!\n\r", SATA_PORT);
    returnValue = d_STATUS_TIMEOUT;
  }
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- WaitForSataReset -->

  Wait for the SATA controller to complete the reset

*************************************************************************/
static d_Status_t
WaitForSataReset
(void)
{
  const Uint32_t TIMEOUT_IN_MSEC = 1000u;
  Uint32_t registerValue = 0u;
  d_Status_t returnValue = d_STATUS_SUCCESS;
  Uint32_t startTime = d_TIMER_ReadValueInTicks();
  do
  {
    registerValue = d_SATA_RegisterRead(GHC_REGISTER);
  } while (((registerValue & HOST_CONTROL_HBA_RESET) != 0u) && (d_TIMER_ElapsedMilliseconds(startTime, NULL) < TIMEOUT_IN_MSEC));

  if ((registerValue & HOST_CONTROL_HBA_RESET) != 0u)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_TIMEOUT;
  }
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- AhciInit -->

  Initialise the Ahci controller

*************************************************************************/
static d_Status_t    /** \return 0 Upon success otherwise error code */
AhciInit
(void)              /**< [in] None */
{
  // read and store capabilities register
  Uint32_t capSave = d_SATA_RegisterRead(CAP_REGISTER);
  capSave &= (CAP_SPM | CAP_SMPS);
  capSave |= CAP_SSS;

  // **** Copied from Linux driver *************
  // to perform host reset, OS should set HOST_RESET
  // and poll until this bit is read to be "0".
  // reset must complete within 1 second, or
  // the hardware should be considered fried.
  d_Status_t returnValue = d_STATUS_SUCCESS;


  Uint32_t registerValue = d_SATA_RegisterRead(GHC_REGISTER);
  if ((registerValue & HOST_CONTROL_HBA_RESET) == 0u)
  {
    registerValue |= HOST_CONTROL_HBA_RESET;
    d_SATA_RegisterWrite(GHC_REGISTER, registerValue);
    (void)d_SATA_RegisterRead(GHC_REGISTER); // flush
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  returnValue = WaitForSataReset();

  if (returnValue == d_STATUS_SUCCESS)
  {
    d_SATA_RegisterWrite(GHC_REGISTER, HOST_CONTROL_AHCI_ENABLE);
    (void)d_SATA_RegisterRead(GHC_REGISTER); // flush

    // restore capabilities register
    d_SATA_RegisterWrite(CAP_REGISTER, capSave);

    d_SATA_RegisterWrite(PI_REGISTER, 0x03u); // enable ports 0 and 1
    (void)d_SATA_RegisterRead(PI_REGISTER); // flush
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  for (Uint32_t retryCount = 0u; retryCount < 3u; retryCount++)
  {
    if (returnValue == d_STATUS_SUCCESS)
    {
      // make sure the port is not active
      registerValue = d_SATA_RegisterRead(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      if ((registerValue & (PORT_CMD_LIST_ON | PORT_CMD_FIS_ON | PORT_CMD_FRE | PORT_CMD_ST)) != 0u)
      {
         // gcov-jst 5 It is not practical to generate this failure during bench testing.
        registerValue &= ~(PORT_CMD_LIST_ON | PORT_CMD_FIS_ON | PORT_CMD_FRE | PORT_CMD_ST);
        d_SATA_RegisterWrite(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT), registerValue);
        (void)d_SATA_RegisterRead(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT)); // flush

        (void)d_TIMER_DelayMilliseconds(500u); // Spec says 500msec sleep between each bit
      }
      ELSE_DO_NOTHING

      // Add spin up command
      registerValue = d_SATA_RegisterRead(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      registerValue |= PORT_CMD_SPIN_UP;
      d_SATA_RegisterWrite(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT), registerValue);
      (void)d_SATA_RegisterRead(PxCMD_REGISTER + (PORT_REG_OFFSET * SATA_PORT)); // flush

      // Wait for SATA link to start
      returnValue = WaitForSataLink();
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    if (returnValue == d_STATUS_SUCCESS)
    {
      // Clear error status
      registerValue = d_SATA_RegisterRead(PxSERR_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      d_SATA_RegisterWrite(PxSERR_REGISTER + (PORT_REG_OFFSET * SATA_PORT), registerValue);

      xil_printf("Spinning up device on SATA port %d... ", SATA_PORT);

      const Uint32_t SATA_SPINUP_TIME_OUT_IN_MSEC = 20000u;
      Uint32_t startTime = d_TIMER_ReadValueInTicks();
      Uint32_t portTaskFileRegister;
      Uint32_t portSataStatusRegister;
      do
      {
        portTaskFileRegister = d_SATA_RegisterRead(PxTFD_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
        portSataStatusRegister = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      } while ((d_TIMER_ElapsedMilliseconds(startTime, NULL) < SATA_SPINUP_TIME_OUT_IN_MSEC) &&
	           ((portTaskFileRegister & (PORT_TFD_ATA_BUSY | PORT_TFD_ATA_DRQ)) != 0u) &&
	           ((portSataStatusRegister & PORT_SSTS_DET_MASK) != PORT_STS_DET_PHYRDY));

      registerValue = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      if ((registerValue & PORT_SSTS_DET_MASK) == PORT_STS_DET_COMINIT)
      {
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        xil_printf("SATA Link %d down (COMINIT received), retrying...\n\r", SATA_PORT);
        continue;
      }
      ELSE_DO_NOTHING

      xil_printf("Target spinup took %d ms\n\r", d_TIMER_ElapsedMilliseconds(startTime, NULL));

      registerValue = d_SATA_RegisterRead(PxSERR_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      d_SATA_RegisterWrite(PxSERR_REGISTER + (PORT_REG_OFFSET * SATA_PORT), registerValue);

      // ack any pending irq events for this port
      registerValue = d_SATA_RegisterRead(PxIS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      d_SATA_RegisterWrite(PxIS_REGISTER + (PORT_REG_OFFSET * SATA_PORT), registerValue);
      d_SATA_RegisterWrite(IS_REGISTER, (1u << SATA_PORT));

      // register linkup ports
      registerValue = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * SATA_PORT));
      if ((registerValue & PORT_SSTS_DET_MASK) == PORT_STS_DET_PHYRDY)
      {
        xil_printf("SATA Port %d link up\n\r", SATA_PORT);
        break; // success, break from retry loop
      }
      // gcov-jst 6 It is not practical to generate this failure during bench testing.
      ELSE_DO_NOTHING

      registerValue = d_SATA_RegisterRead(GHC_REGISTER);
      registerValue |= HOST_CONTROL_INETRRUPT_ENABLE;
      d_SATA_RegisterWrite(GHC_REGISTER, registerValue);
      (void)d_SATA_RegisterRead(GHC_REGISTER); // flush
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING
  }

  return returnValue;
}

/*********************************************************************//**
  <!-- AhciStartPort -->

  Starts the Ahci port

*************************************************************************/
static d_Status_t             /** \return 0 upon Success, error code on failure */
AhciStartPort
(Uint32_t port)              /**< [in] SATA port number to use */
{
  const Uint32_t SATA_PORT_COUNT = 2u;

  Uint32_t registerValue;
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (port >= SATA_PORT_COUNT)
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (returnValue == d_STATUS_SUCCESS)
  {
    registerValue = d_SATA_RegisterRead(PxSSTS_REGISTER + (PORT_REG_OFFSET * port));
    if ((registerValue & PORT_SSTS_DET_MASK) != PORT_STS_DET_PHYRDY)
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_DEVICE_NOT_READY;
    }
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  if (returnValue == d_STATUS_SUCCESS)
  {
    const Uint32_t AHCI_CMD_SLOT_SIZE = 256u;
    const Uint32_t AHCI_RX_FIS_SIZE = 256u;
    const Uint32_t AHCI_CMD_TBL_HDR = 0x80u;

    d_GEN_MemorySet(dmaMem, 0u, AHCI_PORT_DMA_SIZE);

    Uint32_t index = 0;

    // cppcheck-suppress misra-c2012-11.3; Conversion between pointers cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    cmdInfo.cmdSlot = (AhchiCmdHdr_t*)&dmaMem[index];
    index += AHCI_CMD_SLOT_SIZE;

    // cppcheck-suppress misra-c2012-11.4; Conversion between pointer and integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    cmdInfo.rxFis = (Uint32_t)&dmaMem[index];
    index += AHCI_RX_FIS_SIZE;

    // cppcheck-suppress misra-c2012-11.4; Conversion between pointer and integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    cmdInfo.cmdTbl = (Uint32_t)&dmaMem[index];
    index += AHCI_CMD_TBL_HDR;

    // cppcheck-suppress misra-c2012-11.3; Conversion between pointers cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    cmdInfo.cmdTblSg = (AhciSg_t*)&dmaMem[index];

    // cppcheck-suppress misra-c2012-11.4; Conversion between pointer and integer cannot be avoided in this case. Violation of 'Advisory' rule does not present a risk.
    Uint32_t addrValue = (Uint32_t)cmdInfo.cmdSlot; // first cast the pointer to 32bit
    Uint64_t dmaAddr = (Uint64_t)addrValue; // now cast 32bit value to 64bit for the DMA
    d_SATA_RegisterWrite(PxCLB_REGISTER + (PORT_REG_OFFSET * port), dmaAddr);
    (void)d_SATA_RegisterRead(PxCLB_REGISTER + (PORT_REG_OFFSET * port)); // flush
    d_SATA_RegisterWrite(PxCLBU_REGISTER + (PORT_REG_OFFSET * port), dmaAddr >> 32u);
    (void)d_SATA_RegisterRead(PxCLBU_REGISTER + (PORT_REG_OFFSET * port)); // flush

    dmaAddr = (Uint64_t)cmdInfo.rxFis;
    d_SATA_RegisterWrite(PxFB_REGISTER + (PORT_REG_OFFSET * port), dmaAddr);
    (void)d_SATA_RegisterRead(PxFB_REGISTER + (PORT_REG_OFFSET * port)); // flush
    d_SATA_RegisterWrite(PxFBU_REGISTER + (PORT_REG_OFFSET * port), dmaAddr >> 32u);
    (void)d_SATA_RegisterRead(PxFBU_REGISTER + (PORT_REG_OFFSET * port)); // flush

    // now start port
    registerValue = PORT_CMD_ICC_ACTIVE | PORT_CMD_FRE | PORT_CMD_POD | PORT_CMD_SPIN_UP | PORT_CMD_ST;
    d_SATA_RegisterWrite(PxCMD_REGISTER + (PORT_REG_OFFSET * port), registerValue);
    (void)d_SATA_RegisterRead(PxCMD_REGISTER + (PORT_REG_OFFSET * port)); // flush

    // wait for spin-up
    const Uint32_t SPINUP_TIEMOUT_IN_MSEC = 20000u;
    Uint32_t startTime = d_TIMER_ReadValueInTicks();
    while (d_TIMER_ElapsedMilliseconds(startTime, NULL) < SPINUP_TIEMOUT_IN_MSEC)
    {
      registerValue = d_SATA_RegisterRead(PxTFD_REGISTER + (PORT_REG_OFFSET * port));
      if ((registerValue & PORT_TFD_ATA_BUSY) == 0u)
      {
        break;
      }
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      ELSE_DO_NOTHING
    }

    if ((registerValue & PORT_TFD_ATA_BUSY) != 0u) // timeout occurred busy flag still set
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      returnValue = d_STATUS_TIMEOUT;
    }
    ELSE_DO_NOTHING
  }
  // gcov-jst 1 It is not practical to generate this failure during bench testing.
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- SataSetup -->

  Sata Vendor specific Setup

*************************************************************************/
static void       /** \return None */
SataSetup
(void)        /**< [in] None */
{
  // TPSS TPRS scalars, CISE and Port Addr
  Uint32_t registerValue = PORT_CONTROL_TPSS_VAL | PORT_CONTROL_TPRS_VAL | PORT_CONTROL_PAD_VAL;
  d_SATA_RegisterWrite(PCFG_REGISTER, registerValue);

  // AXI Data bus width to 64
  // Set Mem Addr Read, Write ID for data transfers
  // Transfer limit to 72 DWord
  registerValue = PAXIC_BUS_WIDTH_64 | PAXIC_MEM_ADDR_WRITE_ID_DATA | PAXIC_MEM_ADDR_READ_ID_DATA | PAXIC_TRANSFER_LIMIT;
  d_SATA_RegisterWrite(PAXIC_REGISTER, registerValue);

  // Port Phy Cfg register enables
  registerValue = PPCFG_TTA | PPCFG_PSS_EN | PPCFG_ESDF_EN;
  d_SATA_RegisterWrite(PPCFG_REGISTER, registerValue);

  // Rx Watermark setting
  registerValue = PTC_RX_WM_VAL | PTC_RSVD;
  d_SATA_RegisterWrite(PTC_REGISTER, registerValue);
}


/*********************************************************************//**
  <!-- d_SATA_Flush -->

  Performs a Flush to the sata device cache

*************************************************************************/
d_Status_t                      /** \return 0 If successful */
d_SATA_Flush
(void)
{
  return d_SATAOP_Flush(SATA_PORT);
}

/*********************************************************************//**
  <!-- d_SATA_Write -->

  Performs a write to the sata device
  The buffer passed to the function needs to be 1k aligned for DMA purposes

*************************************************************************/
d_Status_t                      /** \return 0 If successful */
d_SATA_Write
(const Uint32_t lba,            /**< [in] logical block address to start read */
const Uint8_t * const pBuffer,              /**< [in] buffer containing the data to be written */
const Uint32_t writeLength)      /**< [in] the amount of bytes to be written */
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (lba + (writeLength / ATA_SECT_SIZE) >= d_SATA_DeviceInfo.capacity) // check that the write will not go past the end of the drive
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1, lba, writeLength, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_ReadWrite(SATA_PORT, lba, pBuffer, writeLength, d_TRUE);
  }
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_Read -->

  Performs a read from the sata device
  The buffer passed to the function needs to be 1k aligned for DMA purposes

*************************************************************************/
d_Status_t                  /** \return 0 If successful */
d_SATA_Read
(const Uint32_t lba,        /**< [in] logical block address to start read */
Uint8_t * pBuffer,          /**< [Out] buffer to place the read data */
const Uint32_t readLength)  /**< [in] the amount of bytes to be read */
{
  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (lba + (readLength / ATA_SECT_SIZE) >= d_SATA_DeviceInfo.capacity) // check that the read will not go past the end of the drive
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1, lba, readLength, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (pBuffer == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 2, 0, 0, 0);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_ReadWrite(SATA_PORT, lba, pBuffer, readLength, d_FALSE);
  }
  ELSE_DO_NOTHING

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_GetDriveInfo -->

  Retrieves a copy of the connected drive info structure

*************************************************************************/
d_SATA_DeviceInfo_t        /** \return a copy of the drive info */
d_SATA_GetDriveInfo
(void)                    /**< [in] None */
{
  return d_SATA_DeviceInfo;
}

/*********************************************************************//**
  <!-- d_SATA_GetSMARTInfo -->

  Reads the S.M.A.R.T info from the drive

*************************************************************************/
d_Status_t                            /** \return success if the info could be read */
d_SATA_GetSMARTInfo
(d_SATA_SmartInfo_t * pSmartInfo)       /**< [out] Pointer to structure to store the result */
{
  if (pSmartInfo == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_NON_CRITICAL, 1, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  ELSE_DO_NOTHING

  return d_SATAOP_ReadSmartData(SATA_PORT, pSmartInfo);
}

/*********************************************************************//**
  <!-- d_SATA_Initialise -->

  Initialises the SATA controller

*************************************************************************/
d_Status_t                /** \return 0 If successful */
d_SATA_Initialise
(void)                    /**< [in] None */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_Status_t returnValue = d_STATUS_SUCCESS;

  if (initialised != d_TRUE)  // prevent re-initialisation
  {
    Uint32_t registerValue;

    // Setup the Phy PLL clock select, this should have been set in FW but for some reason the value is wrong...
    d_GEN_RegisterWrite(PLL_REF_SEL0, 0x0Fu);

    // Reset device - sata controller
    registerValue = d_GEN_RegisterRead(XRESETPS_CRF_APB_RST_FPD_TOP);
    registerValue |= SATA_RESET_MASK;
    d_GEN_RegisterWrite(XRESETPS_CRF_APB_RST_FPD_TOP, registerValue);

    (void)d_TIMER_DelayMicroseconds(10u); // short break for the reset

    // Bring up SATA by de-asserting the reset
    registerValue = d_GEN_RegisterRead(XRESETPS_CRF_APB_RST_FPD_TOP);
    registerValue &= (~SATA_RESET_MASK);
    d_GEN_RegisterWrite(XRESETPS_CRF_APB_RST_FPD_TOP, registerValue);

    SataSetup();

    returnValue = AhciInit();

    if (returnValue == d_STATUS_SUCCESS)
    {
      returnValue = AhciStartPort(SATA_PORT);
    }
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    ELSE_DO_NOTHING

    // Sata Up an running, now get some info from the drive connected to the controller
    if (returnValue == d_STATUS_SUCCESS)
    {
      (void)d_TIMER_DelayMilliseconds(100);
      returnValue = d_SATAOP_Inquiry(SATA_PORT);
    }

    if (returnValue == d_STATUS_SUCCESS)
    {
      initialised = d_TRUE;
    }
  }
  ELSE_DO_NOTHING

  d_LOG_SATA_FUNCTION_EXIT(returnValue);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_IsSelfEncryptingDrive -->

  Verifies whether the SATA drive is a self encrypting drive

*************************************************************************/
Bool_t                          /** \return True if the drive supports Self Encryption */
d_SATA_IsSelfEncryptingDrive
(void)                          /**< [in] None */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  d_GEN_MemorySet((Uint8_t*)&sedStatus, 0, sizeof(sedStatus));
  d_Status_t returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  if (returnValue != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    sedStatus.lockingSupported = d_FALSE;
  }

  d_LOG_SATA_FUNCTION_EXIT(sedStatus.lockingSupported);

  return sedStatus.lockingSupported;
}

/*********************************************************************//**
  <!-- d_SATA_IsDriveLocked -->

  Verifies whether the Self encrypting SATA drive is locked or not

*************************************************************************/
Bool_t                  /** \return True if the drive is locked */
d_SATA_IsDriveLocked
(void)                  /**< [in] None */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  d_GEN_MemorySet((Uint8_t*)&sedStatus, 0, sizeof(sedStatus));
  d_Status_t returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  if (returnValue != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    sedStatus.locked = d_FALSE;
  }

  d_LOG_SATA_FUNCTION_EXIT(sedStatus.locked);

  return sedStatus.locked;
}

/*********************************************************************//**
  <!-- d_SATA_IsEncryptionEnabled -->

  Verifies whether the Self encrypting SATA drive is configured for encryption or not

*************************************************************************/
Bool_t                  /** \return True if encryption is enabled */
d_SATA_IsEncryptionEnabled
(void)                  /**< [in] None */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  d_GEN_MemorySet((Uint8_t*)&sedStatus, 0, sizeof(sedStatus));
  d_Status_t returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  if (returnValue != d_STATUS_SUCCESS)
  {
    // gcov-jst 2 It is not practical to generate this failure during bench testing.
    d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    sedStatus.lockingEnabled = d_FALSE;
  }
  ELSE_DO_NOTHING

  d_LOG_SATA_FUNCTION_EXIT(sedStatus.lockingEnabled);

  return sedStatus.lockingEnabled;
}

/*********************************************************************//**
  <!-- d_SATA_EnableEncryption -->

  Enables the encryption feature of a Self Encrypting Drive

*************************************************************************/
d_Status_t                        /** \return 0 If successful */
d_SATA_EnableEncryption
(const Char_t * const password,   /**< [in] Password to be used for encryption */
const Uint32_t passwordLength)    /**< [in] Length of the password */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_Status_t returnValue = d_STATUS_SUCCESS;
  if (password == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (sedStatus.lockingSupported == d_TRUE)
    {
      if (sedStatus.lockingEnabled == d_FALSE)
      {
        returnValue = d_SATAOP_EncInitialSetup(SATA_PORT, password, passwordLength);
      }
      else
      {
        // Drive already configured for locking
        d_ERROR_Logger(d_STATUS_INVALID_MODE, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
        returnValue = d_STATUS_INVALID_MODE;
      }
    }
    else
    {
      // Drive does not support Self Encryption
      d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
      returnValue = d_STATUS_HARDWARE_ERROR;
    }
  }

  d_LOG_SATA_FUNCTION_EXIT(returnValue);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_DisableEncryption -->

  Disables the encryption on the Self Encrypting Drive

*************************************************************************/
d_Status_t
d_SATA_DisableEncryption
(const Char_t * const password,
const Uint32_t passwordLength)
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_Status_t returnValue = d_STATUS_SUCCESS;
  if (password == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (sedStatus.lockingSupported == d_TRUE)
    {
      if (sedStatus.lockingEnabled == d_TRUE)
      {
        returnValue = d_SATAOP_EnableDisableLockingRange(SATA_PORT, password, passwordLength, d_FALSE);
      }
      else
      {
        // Drive not configured for locking
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_MODE, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
        returnValue = d_STATUS_INVALID_MODE;
      }
    }
    else
    {
      // Drive does not support Self Encryption
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
      returnValue = d_STATUS_HARDWARE_ERROR;
    }
  }

  d_LOG_SATA_FUNCTION_EXIT(returnValue);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_UnlockDrive -->

  Unlocks a Self encrypting drive for use

*************************************************************************/
d_Status_t                        /** \return 0 If successful */
d_SATA_UnlockDrive
(const Char_t * const password,   /**< [in] Password to be used for encryption */
const Uint32_t passwordLength)    /**< [in] Length of the password */
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_Status_t returnValue = d_STATUS_SUCCESS;
  if (password == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }

  d_SATAENC_SelfEncryptionStatus_t sedStatus;
  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (sedStatus.lockingSupported == d_TRUE)
    {
      if (sedStatus.lockingEnabled == d_TRUE)
      {
        returnValue = d_SATAOP_SetLockingUnlocking(SATA_PORT, password, passwordLength, d_FALSE);
      }
      else
      {
        // Drive not configured for locking
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_MODE, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
        returnValue = d_STATUS_INVALID_MODE;
      }
    }
    else
    {
      // Drive does not support Self Encryption
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
      returnValue = d_STATUS_HARDWARE_ERROR;
    }
  }

  d_LOG_SATA_FUNCTION_EXIT(returnValue);

  return returnValue;
}

/*********************************************************************//**
  <!-- d_SATA_LockDrive -->

  Locks a Self encrypting drive

*************************************************************************/
d_Status_t d_SATA_LockDrive(const Char_t * const password, const Uint32_t passwordLength)
{
  d_LOG_SATA_FUNCTION_ENTRY();

  d_Status_t returnValue = d_STATUS_SUCCESS;
  d_SATAENC_SelfEncryptionStatus_t sedStatus;

  if (password == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
    returnValue = d_STATUS_INVALID_PARAMETER;
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    returnValue = d_SATAOP_Discovery0(SATA_PORT, &sedStatus);
  }

  if (returnValue == d_STATUS_SUCCESS)
  {
    if (sedStatus.lockingSupported == d_TRUE)
    {
      if (sedStatus.lockingEnabled == d_TRUE)
      {
        returnValue = d_SATAOP_SetLockingUnlocking(SATA_PORT, password, passwordLength, d_TRUE);
      }
      else
      {
        // Drive not configured for locking
        // gcov-jst 2 It is not practical to generate this failure during bench testing.
        d_ERROR_Logger(d_STATUS_INVALID_MODE, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
        returnValue = d_STATUS_INVALID_MODE;
      }
    }
    else
    {
      // Drive does not support Self Encryption
      // gcov-jst 2 It is not practical to generate this failure during bench testing.
      d_ERROR_Logger(d_STATUS_HARDWARE_ERROR, d_ERROR_CRITICALITY_UNKNOWN, 1u, 0u, 0u, 0u);
      returnValue = d_STATUS_HARDWARE_ERROR;
    }
  }

  d_LOG_SATA_FUNCTION_EXIT(returnValue);

  return returnValue;
}
