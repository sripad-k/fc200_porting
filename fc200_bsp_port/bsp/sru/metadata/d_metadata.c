/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_metadata

  Abstract           : Metadata CSC module, Revision 03

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-157
                                                                SWREQ-158
                                                                SWREQ-159
                       SDD References: 136T-2200-131000-001-D22 SWDES-546
                                                                SWDES-604
\note
  CSCID              : SWDES-60
*************************************************************************/

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"            /* Common type definitions */
#include "soc/defines/d_common_status.h"           /* Common status and error definitions */
#include "sru/qspiFlash/d_qspiFlash.h"             /* QSPI Flash */
#include "sru/fcu/d_fcu_cfg.h"                         /* FCU CSC */
#include "kernel/crc32/d_crc32.h"                  /* CRC calculations */
#include "kernel/error_handler/d_error_handler.h"  /* Error handler */

#include "d_metadata.h"                            /* This CSC - Metadata */
#include "d_metadata_cfg.h"                        /* This CSC - Metadata configuration */

/***** Constants ********************************************************/

const Uint32_t d_META_DATA_OFFSET_IN_QSPI_FLASH = 0x03FC0000u;

const Uint8_t d_META_FORMAT_REVISION = 0x03u;

// cppcheck-suppress misra-c2012-8.9; Defining constants at the start of the module is more maintainable. Violation of 'Advisory' rule does not present a risk.
static const Uint32_t CONFIG_START_IDENTIFIER = 0xA5C300AAu;

/***** Type Definitions *************************************************/
// The following typedefs force a compile-time struct binary alignment and size check, if the struct sizes are incorrect compilation will fail.
// The typedefs are not intended to be used for any other purpose other than to verify struct binary alignment and sizes.
typedef Uint8_t CompileTimeAlignmentSanityCheck_MetaData_Header_t[1 - 2 *(sizeof(d_META_Header_t) % 4u != 0u)];
typedef Uint8_t CompileTimeAlignmentSanityCheck_MetaData_HWblock_t[1 - 2 *(sizeof(d_META_HWblock_t) % 4u != 0u)];
typedef Uint8_t CompileTimeAlignmentSanityCheck_MetaData_FWblock_t[1 - 2 *(sizeof(d_META_FWblock_t) % 4u != 0u)];
typedef Uint8_t CompileTimeAlignmentSanityCheck_MetaData_SWblock_t[1 - 2 *(sizeof(d_META_SWblock_t) % 4u != 0u)];
typedef Uint8_t CompileTimeAlignmentSanityCheck_MetaData_MISCblock_t[1 - 2 *(sizeof(d_META_MISCblock_t) % 4u != 0u)];

/***** Variables ********************************************************/

static Bool_t initialised = d_FALSE;

/***** Function Declarations ********************************************/

static d_Status_t verifyBlocks(const d_META_Header_t * const pMetaHeader);
static d_Status_t verifyHardware(const d_META_Header_t * const pMetaHeader);
static d_Status_t verifyFirmware(const d_META_Header_t * const pMetaHeader);
static d_Status_t verifySoftware(const d_META_Header_t * const pMetaHeader);
static d_Status_t verifyMiscellaneous(const d_META_Header_t * const pMetaHeader);

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- d_META_Initialise -->

  Initialise function for the Metadata CSC
*************************************************************************/
d_Status_t                        /** \return Status of Metadata initialisation */
d_META_Initialise
(
void
)
{
  d_Status_t returnValue = d_STATUS_SUCCESS;        /* Return parameter: Status of operation */
  if (initialised == d_FALSE)
  {
    returnValue = d_QSPI_Initialise();

    if (returnValue == d_STATUS_SUCCESS)
    {
      // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
      returnValue = d_QSPI_Read((d_META_DATA_OFFSET_IN_QSPI_FLASH), d_METADATA_MAXIMUM_SIZE, (Uint32_t *)d_METADATA_BaseAddress[0], d_METADATA_MAXIMUM_SIZE);
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }

    if (returnValue == d_STATUS_SUCCESS)
    {
      initialised = d_TRUE;
    }
    else
    {
      // gcov-jst 1 It is not practical to generate this failure during bench testing.
      DO_NOTHING();
    }
  }
  
  return (returnValue);
}

/*********************************************************************//**
  <!-- d_META_ValidTest -->

  Test the validity of the Metadata on the QSPI flash memory
*************************************************************************/
d_Status_t                        /** \return Status of Metadata validity */
d_META_ValidTest
(
const Uint32_t index              /**< [in] index into metadata structures list */
)
{
  d_Status_t returnStatus = d_STATUS_SUCCESS;        /* Return parameter: Status of operation */
  d_META_Header_t * pMetaHeader;

  if (initialised == d_FALSE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (index >= d_METADATA_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, index, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }
  
  returnStatus = d_FCU_IocAddressCheck(d_METADATA_BaseAddress[index]);
  if (returnStatus == d_STATUS_SUCCESS)
  {
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaHeader =  (d_META_Header_t *)d_METADATA_BaseAddress[index]; /**< [in] Metadata pointer */

    /* Check start identifier */
    if (pMetaHeader->configStartIdentifier != CONFIG_START_IDENTIFIER)
    {
      returnStatus = d_STATUS_BAD_DATA;
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    // gcov-jst 1 It is not practical to generate this failure during bench testing.
    DO_NOTHING();
  }
  
  /* Check header checksum */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    Uint32_t calculatedChecksum = d_CRC32_Calculate((const Uint8_t * const)&pMetaHeader->configStartIdentifier,
                                                     sizeof(d_META_Header_t) - sizeof(pMetaHeader->checksum));

    if (calculatedChecksum != pMetaHeader->checksum)
    {
      returnStatus = d_STATUS_BAD_DATA;
    }
    else
    {
      DO_NOTHING();
    }
  }
  else
  {
    DO_NOTHING();
  }
  
  /* Verify checksum of each block */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    returnStatus = verifyBlocks(pMetaHeader);
  }
  else
  {
    DO_NOTHING();
  }

  return (returnStatus);
}

/*********************************************************************//**
  <!-- d_META_MemoruUsage -->

  Get memory usage.
*************************************************************************/
d_Status_t                        /** \return Status of Memory usage */
d_META_MemoryUsage
(
const Uint32_t index,             /**< [in] index into metadata structures list */
Uint32_t * const pFlashUsage,     /**< [out] pointer to memory to return Flash usage */
Uint32_t * const pRamUsage        /**< [out] pointer to memory to return RAM usage */
)
{
  d_Status_t returnStatus = d_STATUS_SUCCESS;        /* Return parameter: Status of operation */
  Uint32_t i;
  
  if (initialised == d_FALSE)
  {
    d_ERROR_Logger(d_STATUS_NOT_INITIALISED, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 0, 0, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_NOT_INITIALISED;
  }

  if (index >= d_METADATA_Count)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 1, index, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pFlashUsage == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 2, 0, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  if (pRamUsage == NULL)
  {
    d_ERROR_Logger(d_STATUS_INVALID_PARAMETER, d_ERROR_CRITICALITY_CRITICAL_SHUTDOWN, 3, 0, 0, 0);
    /* Following for robustness if the critical error handling returns */
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return d_STATUS_INVALID_PARAMETER;
  }

  // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
  d_META_Header_t * pMetaHeader =  (d_META_Header_t *)d_METADATA_BaseAddress[index]; /**< [in] Metadata pointer */

  Uint32_t flashUsage = 0u;
  Uint32_t ramUsage = 0u;
  
  /* Add metadata blocks */
  flashUsage = flashUsage + 
               sizeof(d_META_Header_t) +
               (pMetaHeader->numberHWblocks * sizeof(d_META_HWblock_t)) +
               (pMetaHeader->numberFWblocks * sizeof(d_META_FWblock_t)) +
               (pMetaHeader->numberSWblocks * sizeof(d_META_SWblock_t)) +
               (pMetaHeader->numberMISCblocks * sizeof(d_META_MISCblock_t));

  /* Add firmware blocks */
  for (i = 0u; i < pMetaHeader->numberFWblocks; i++)
  {
    d_META_FWblock_t * pMetaFirmware;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaFirmware = (d_META_FWblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetFWblock);

    flashUsage = flashUsage + pMetaFirmware[i].size;
  }
  
  /* Add software blocks */
  for (i = 0u; i < pMetaHeader->numberSWblocks; i++)
  {
    d_META_SWblock_t * pMetaSoftware;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaSoftware = (d_META_SWblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetSWblock);

    if (pMetaSoftware[i].startAddress == 0xFFFFFFFFu)
    {
      ramUsage = ramUsage + pMetaSoftware[i].size;
    }
    else
    {
      flashUsage = flashUsage + pMetaSoftware[i].size;
    }
  }
  
  /* Add miscellaneous blocks */
  for (i = 0u; i < pMetaHeader->numberMISCblocks; i++)
  {
    d_META_MISCblock_t * pMetaMisc;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaMisc = (d_META_MISCblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetMISCblock);

    flashUsage = flashUsage + pMetaMisc[i].length;
  }
  
  *pFlashUsage = flashUsage;
  *pRamUsage = ramUsage;
  
  return returnStatus;
}

/*********************************************************************//**
  <!-- verifyHardware -->

  Verify the checksum of each hardware block.
*************************************************************************/
static d_Status_t                          /** \return Status of metadata blockS check */
verifyBlocks
(
const d_META_Header_t * const pMetaHeader  /**< [in] Metadata pointer */
)
{
  d_Status_t returnStatus = d_STATUS_SUCCESS;

  /* Check revision */
  if (pMetaHeader->formatRevision != d_META_FORMAT_REVISION)
  {
    returnStatus = d_STATUS_INCOMPATIBLE;
  }
  else
  {
    DO_NOTHING();
  }

  /* Verify checksum of each HW block */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    returnStatus = verifyHardware(pMetaHeader);
  }
  else
  {
    DO_NOTHING();
  }
  
  /* Verify checksum of each FW block */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    returnStatus = verifyFirmware(pMetaHeader);
  }
  else
  {
    DO_NOTHING();
  }

  /* Verify checksum of each SW block */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    returnStatus = verifySoftware(pMetaHeader);
  }
  else
  {
    DO_NOTHING();
  }

  /* Verify checksum of each MISC block */
  if (returnStatus == d_STATUS_SUCCESS)
  {
    returnStatus = verifyMiscellaneous(pMetaHeader);
  }
  else
  {
    DO_NOTHING();
  }

  return returnStatus;
}

/*********************************************************************//**
  <!-- verifyHardware -->

  Verify the checksum of each hardware block.
*************************************************************************/
static d_Status_t                          /** \return Status of metadata hardware block check */
verifyHardware
(
const d_META_Header_t * const pMetaHeader  /**< [in] Metadata pointer */
)
{
  Uint32_t index;
  d_Status_t returnStatus = d_STATUS_SUCCESS;;

  /* Verify checksum of each HW block */
  index = 0;
  while ((returnStatus == d_STATUS_SUCCESS) && (index < pMetaHeader->numberHWblocks))
  {
    d_META_HWblock_t * pMetaHardware;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaHardware = (d_META_HWblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetHWblock);

    Uint32_t calculatedChecksum = d_CRC32_Calculate((const Uint8_t * const)&pMetaHardware[index],
                                           sizeof(d_META_HWblock_t) - sizeof(pMetaHardware[index].blockChecksum));
    if (calculatedChecksum != pMetaHardware[index].blockChecksum)
    {
      returnStatus = d_STATUS_BAD_DATA;
      d_ERROR_Logger(d_STATUS_BAD_DATA, d_ERROR_CRITICALITY_UNKNOWN, 0, index, calculatedChecksum, pMetaHardware[index].blockChecksum);
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }

  return returnStatus;
}

/*********************************************************************//**
  <!-- verifyFirmware -->

  Verify the checksum of each firmware block.
*************************************************************************/
static d_Status_t                          /** \return Status of metadata firmware block check */
verifyFirmware
(
const d_META_Header_t * const pMetaHeader  /**< [in] Metadata pointer */
)
{
  Uint32_t index;
  d_Status_t returnStatus = d_STATUS_SUCCESS;;

  /* Verify checksum of each FW block */
  index = 0;
  while ((returnStatus == d_STATUS_SUCCESS) && (index < pMetaHeader->numberFWblocks))
  {
    d_META_FWblock_t * pMetaFirmware;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaFirmware = (d_META_FWblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetFWblock);

    Uint32_t calculatedChecksum = d_CRC32_Calculate((const Uint8_t * const)&pMetaFirmware[index],
                                           sizeof(d_META_FWblock_t) - sizeof(pMetaFirmware[index].blockChecksum));
    if (calculatedChecksum != pMetaFirmware[index].blockChecksum)
    {
      returnStatus = d_STATUS_BAD_DATA;
      d_ERROR_Logger(d_STATUS_BAD_DATA, d_ERROR_CRITICALITY_UNKNOWN, 1, index, calculatedChecksum, pMetaFirmware[index].blockChecksum);
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }

  return returnStatus;
}

/*********************************************************************//**
  <!-- verifySoftware -->

  Verify the checksum of each software block.
*************************************************************************/
static d_Status_t                          /** \return Status of metadata software block check */
verifySoftware
(
const d_META_Header_t * const pMetaHeader  /**< [in] Metadata pointer */
)
{
  Uint32_t index;
  d_Status_t returnStatus = d_STATUS_SUCCESS;;

  /* Verify checksum of each SW block */
  index = 0;
  while ((returnStatus == d_STATUS_SUCCESS) && (index < pMetaHeader->numberSWblocks))
  {
    d_META_SWblock_t * pMetaSoftware;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaSoftware = (d_META_SWblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetSWblock);

    Uint32_t calculatedChecksum = d_CRC32_Calculate((const Uint8_t * const)&pMetaSoftware[index],
                                           sizeof(d_META_SWblock_t) - sizeof(pMetaSoftware[index].blockChecksum));
    if (calculatedChecksum != pMetaSoftware[index].blockChecksum)
    {
      returnStatus = d_STATUS_BAD_DATA;
      d_ERROR_Logger(d_STATUS_BAD_DATA, d_ERROR_CRITICALITY_UNKNOWN, 2, index, calculatedChecksum, pMetaSoftware[index].blockChecksum);
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }

  return returnStatus;
}

/*********************************************************************//**
  <!-- verifyMiscellaneous -->

  Verify the checksum of each miscellaneous block.
*************************************************************************/
static d_Status_t                          /** \return Status of metadata miscellaneous block check */
verifyMiscellaneous
(
const d_META_Header_t * const pMetaHeader  /**< [in] Metadata pointer */
)
{
  Uint32_t index;
  d_Status_t returnStatus = d_STATUS_SUCCESS;;
  
  /* Verify checksum of each MISC block */
  index = 0;
  while ((returnStatus == d_STATUS_SUCCESS) && (index < pMetaHeader->numberMISCblocks))
  {
    d_META_MISCblock_t * pMetaMisc;
    // cppcheck-suppress misra-c2012-11.4;  Conversion required by non portable low level software. Violation of 'Advisory' rule does not present a risk.
    pMetaMisc = (d_META_MISCblock_t *)((Uint32_t)pMetaHeader + pMetaHeader->offsetMISCblock);

    Uint32_t calculatedChecksum = d_CRC32_Calculate((const Uint8_t * const)&pMetaMisc[index],
                                           sizeof(d_META_MISCblock_t) - sizeof(pMetaMisc[index].blockChecksum));
    if (calculatedChecksum != pMetaMisc[index].blockChecksum)
    {
      returnStatus = d_STATUS_BAD_DATA;
      d_ERROR_Logger(d_STATUS_BAD_DATA, d_ERROR_CRITICALITY_UNKNOWN, 3, index, calculatedChecksum, pMetaMisc[index].blockChecksum);
    }
    else
    {
      DO_NOTHING();
    }
    index++;
  }

  return returnStatus;
}

