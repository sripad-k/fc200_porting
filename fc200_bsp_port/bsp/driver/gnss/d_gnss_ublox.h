/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : GNSS Ublox Configuration

  Abstract           : Driver for the u-blox F9 GNSS

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-333
                       SDD References: 136T-2200-131100-001-D22 SWDES-296
                                                                SWDES-297
                                                                SWDES-298
                                                                SWDES-299
                                                                SWDES-300
                                                                SWDES-301
\note
  CSC ID             : SWDES-72
*************************************************************************/

#ifndef D_GNSS_UBLOX_H
#define D_GNSS_UBLOX_H

#ifdef GNSS_UBLOX_PUBLIC_VARIABLES
#define XTERN
#else
#define XTERN extern
#endif

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Constants ********************************************************/

/* Supported messages */
enum
{
  GNSS_UBLOX_MESSSAGE_UBX_MON_HW = 0,
  GNSS_UBLOX_MESSSAGE_UBX_TIM_TP,
  GNSS_UBLOX_MESSSAGE_UBX_NAV_PVT,
  GNSS_UBLOX_MESSSAGE_UBX_NAV_COV,
  GNSS_UBLOX_MESSSAGE_UBX_NAV_DOP,
  GNSS_UBLOX_MESSSAGE_UBX_NAV_STATUS,
  GNSS_UBLOX_MESSSAGE_UBX_NAV_VELECEF,
  GNSS_UBLOX_MESSSAGE_UBX_MON_SYS,
  GNSS_UBLOX_MESSSAGE_UBX_SEC_SIG,

  GNSS_UBLOX_MESSSAGE_COUNT
};

#define GNSS_UBLOX_COVARIANCE_COUNT   6u

/***** Type Definitions *************************************************/

/* Variable message data */
typedef struct
{
  Uint32_t period;
  Uint32_t lastReceiveTime;
  Uint32_t counter;
  Uint32_t interval;
  Bool_t   valid;
} d_GNSS_Ublox_messageStatus_t;

typedef enum
{
  GNSS_UBLOX_MON_RF_ASTATE_INIT     = 0,
  GNSS_UBLOX_MON_RF_ASTATE_DONTKNOW = 1,
  GNSS_UBLOX_MON_RF_ASTATE_OK       = 2,
  GNSS_UBLOX_MON_RF_ASTATE_SHORT    = 3,
  GNSS_UBLOX_MON_RF_ASTATE_OPEN     = 4
} d_GNSS_Ublox_AntennaStatus_t;

typedef enum
{
  GNSS_UBLOX_MON_RF_APOWER_OFF      = 0,
  GNSS_UBLOX_MON_RF_APOWER_ON       = 1,
  GNSS_UBLOX_MON_RF_APOWER_DONTKNOW = 2
} d_GNSS_Ublox_AntennaPower_t;
  
typedef enum
{
  GNSS_UBLOX_MON_RF_JAMMING_UNKNOWN  = 0,
  GNSS_UBLOX_MON_RF_JAMMING_OK       = 1,
  GNSS_UBLOX_MON_RF_JAMMING_WARNING  = 2,
  GNSS_UBLOX_MON_RF_JAMMING_CRITICAL = 3
} d_GNSS_Ublox_JammingState_t;

typedef struct
{
  d_GNSS_Ublox_AntennaStatus_t antennaStatus;
  d_GNSS_Ublox_AntennaPower_t powerStatus;
  d_GNSS_Ublox_JammingState_t jammingState;
  Uint8_t jammingIndicator;
  Bool_t valid;
} d_GNSS_Ublox_MonHw_t;

typedef enum
{
  GNSS_UBLOX_TIM_TP_TIMEBASE_GNSS = 0,
  GNSS_UBLOX_TIM_TP_TIMEBASE_UTC = 1
} d_GNSS_Ublox_Timebase_t;

typedef enum
{
  GNSS_UBLOX_TIM_TP_UTC_NOT_AVAILABLE = 0,
  GNSS_UBLOX_TIM_TP_UTC_AVAILABLE = 1
} d_GNSS_Ublox_Utc_t;

typedef enum
{
  GNSS_UBLOX_TIM_TP_RAIM_NOT_AVAILABLE = 0,
  GNSS_UBLOX_TIM_TP_RAIM_NOT_ACTIVE = 1,
  GNSS_UBLOX_TIM_TP_RAIM_ACTIVE = 2
} d_GNSS_Ublox_Raim_t;

typedef enum
{
  GNSS_UBLOX_MON_SYS_BOOT_UNKNOWN = 0,
  GNSS_UBLOX_MON_SYS_BOOT_COLD_START = 1,
  GNSS_UBLOX_MON_SYS_BOOT_WATCHDOG = 2,
  GNSS_UBLOX_MON_SYS_BOOT_HARDWARE_RESET = 3,
  GNSS_UBLOX_MON_SYS_BOOT_HARDWARE_BACKUP = 4,
  GNSS_UBLOX_MON_SYS_BOOT_SOFTWARE_BACKUP = 5,
  GNSS_UBLOX_MON_SYS_BOOT_SOFTWARE_RESET = 6,
  GNSS_UBLOX_MON_SYS_BOOT_VIO_FAIL = 7,
  GNSS_UBLOX_MON_SYS_BOOT_VDD_D_FAIL = 8,
  GNSS_UBLOX_MON_SYS_BOOT_VDD_RF_FAIL = 9,
  GNSS_UBLOX_MON_SYS_BOOT_V_CORE_HIGH_FAIL = 10
} d_GNSS_Ublox_Boot_t;

typedef struct
{
  Uint32_t towMs;
  Uint32_t towSubMs;
  Uint16_t week;
  d_GNSS_Ublox_Timebase_t timebase;
  d_GNSS_Ublox_Utc_t utc;
  d_GNSS_Ublox_Raim_t raim;
  Bool_t valid;
} d_GNSS_Ublox_TimTp_t;

typedef enum
{
  GNSS_UBLOX_NAV_PVT_FIX_NONE    = 0,
  GNSS_UBLOX_NAV_PVT_FIX_DR      = 1,
  GNSS_UBLOX_NAV_PVT_FIX_2D      = 2,
  GNSS_UBLOX_NAV_PVT_FIX_3D      = 3,
  GNSS_UBLOX_NAV_PVT_FIX_GNSS_DR = 4,
  GNSS_UBLOX_NAV_PVT_FIX_TIME    = 5
} d_GNSS_Ublox_FixType_t;

typedef enum
{
  GNSS_UBLOX_NAV_PVT_CARR_NONE            = 0,
  GNSS_UBLOX_NAV_PVT_CARR_PHASE_FLOAT_AMB = 1,
  GNSS_UBLOX_NAV_PVT_CARR_PHASE_FIXED_AMB = 2
} d_GNSS_Ublox_CarrierPhaseSolution_t;

typedef struct
{
  Uint32_t iTow;
  d_GNSS_Ublox_FixType_t fixType;
  Bool_t gnssFixOk;
  Bool_t diffSoln;
  d_GNSS_Ublox_CarrierPhaseSolution_t carrierPhaseSolution;
  Uint8_t numSv;
  Int32_t longitude;
  Int32_t latitude;
  Int32_t height;
  Int32_t hMsl;
  Uint32_t hAcc;
  Uint32_t vAcc;
  Int32_t velN;
  Int32_t velE;
  Int32_t velD;
  Int32_t gSpeed;
  Uint32_t sAcc;
  Uint16_t pDop;
  Bool_t invalidLlh;
  Uint16_t year;
  Uint8_t month;
  Uint8_t day;
  Uint8_t hour;
  Uint8_t min;
  Uint8_t sec;
  Bool_t validDate;
  Bool_t validTime;
  Bool_t fullyResolved;
  Bool_t valid;
} d_GNSS_Ublox_NavPvt_t;

typedef struct
{
  Float32_t posCov[GNSS_UBLOX_COVARIANCE_COUNT];
  Float32_t velCov[GNSS_UBLOX_COVARIANCE_COUNT];
  Bool_t valid;
} d_GNSS_Ublox_NavCov_t;

typedef struct
{
  Uint16_t pDop;
  Uint16_t vDop;
  Uint16_t hDop;
  Uint16_t nDop;
  Uint16_t eDop;
  Bool_t valid;
} d_GNSS_Ublox_NavDop_t;

typedef enum
{
  GNSS_UBLOX_NAV_STATUS_SPOOF_UNKNOWN   = 0,
  GNSS_UBLOX_NAV_STATUS_SPOOF_NONE      = 1,
  GNSS_UBLOX_NAV_STATUS_SPOOF_INDICATED = 2,
  GNSS_UBLOX_NAV_STATUS_SPOOF_MULTIPLE  = 3
} d_GNSS_Ublox_SpoofDetState_t;

typedef struct
{
  d_GNSS_Ublox_FixType_t fixType;
  Bool_t gpsFixOk;
  Bool_t diffSoln;
  Bool_t diffCorr;
  d_GNSS_Ublox_SpoofDetState_t spoofDetState;
  Bool_t valid;
} d_GNSS_Ublox_NavStatus_t;

typedef struct
{
  Uint32_t iTow;
  Int32_t ecefVX;
  Int32_t ecefVY;
  Int32_t ecefVZ;
  Uint32_t sAcc;
  Bool_t valid;
} d_GNSS_Ublox_NavVelecef_t;

typedef struct
{
  d_GNSS_Ublox_Boot_t bootType;
  Uint8_t cpuLoad;
  Uint8_t cpuLoadMax;
  Uint8_t memUsage;
  Uint8_t memUsageMax;
  Uint8_t ioUsage;
  Uint8_t ioUsageMax;
  Uint32_t runTime;
  Uint16_t noticeCount;
  Uint16_t warnCount;
  Uint16_t errorCount;
  Int8_t tempValue;
  Bool_t valid;
} d_GNSS_Ublox_MonSys_t;

typedef struct
{
  Bool_t jamEnabled;
  d_GNSS_Ublox_JammingState_t jammingState;
  Bool_t spoofDetEnabled;
  d_GNSS_Ublox_SpoofDetState_t spoofDetState;
  Bool_t valid;
} d_GNSS_Ublox_SecSig_t;

typedef struct
{
  d_GNSS_Ublox_MonHw_t monHw;
  d_GNSS_Ublox_TimTp_t timTp;
  d_GNSS_Ublox_NavPvt_t navPvt;
  d_GNSS_Ublox_NavCov_t navCov;
  d_GNSS_Ublox_NavDop_t navDop;
  d_GNSS_Ublox_NavStatus_t navStatus;
  d_GNSS_Ublox_NavVelecef_t navVelecef;
  d_GNSS_Ublox_MonSys_t monSys;
  d_GNSS_Ublox_SecSig_t secSig;
} d_GNSS_Ublox_t;

/***** Variables ********************************************************/

/* This is included in the header file for test purposes only */
// cppcheck-suppress misra-c2012-8.4;  The following statement is also the declaration, using the XTERN macro.
XTERN d_GNSS_Ublox_messageStatus_t d_GNSS_Ublox_MessageStatus[GNSS_UBLOX_MESSSAGE_COUNT];

/***** Function Declarations ********************************************/

/* Initialise the GNSS interface */
d_Status_t d_GNSS_Ublox_Initialise(const Uint32_t channel);

/* Process messages received from the GNSS */
void d_GNSS_Ublox_ProcessMessages(void);

/* Reset the GNSS */
void d_GNSS_Ublox_Reset(void);

/* 1pps interrupt handler */
void d_GNSS_Ublox_1ppsInterrupt(const Uint32_t parameter);

#endif /* D_GNSS_UBLOX_H */
