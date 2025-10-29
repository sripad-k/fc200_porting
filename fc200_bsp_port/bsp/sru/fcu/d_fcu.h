/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_fcu.h

  Abstract           : FCU Specific interface.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-759
                                                                SWREQ-760
                                                                SWREQ-761
                       SDD References: 136T-2200-131100-001-D22 SWDES-890
                                                                SWDES-2446
                                                                SWDES-2447
                                                                SWDES-2448
                                                                SWDES-2449
                                                                SWDES-2450
                                                                SWDES-2451
                                                                SWDES-2452
                                                                SWDES-2453
\note
  CSC ID             : SWDES-887
*************************************************************************/

#ifndef D_FCU_H
#define D_FCU_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/discrete/d_discrete.h"

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/* PS MIO Discrete allocation */
#define d_FCU_LED1             d_DISC_MIO_44  /* LED1 */
#define d_FCU_LED2             d_DISC_MIO_45  /* LED2 */
#define d_FCU_LED3             d_DISC_MIO_46  /* LED3 */
#define d_FCU_LED4             d_DISC_MIO_47  /* LED4 */
#define d_FCU_LED5             d_DISC_MIO_48  /* LED5 */
#define d_FCU_LED6             d_DISC_MIO_49  /* LED6 */
#define d_FCU_LED7             d_DISC_MIO_50  /* LED7 */
#define d_FCU_LED8             d_DISC_MIO_51  /* LED8 */

#define d_FCU_GSE              d_DISC_MIO_52  /* MAINT_DISC_1 - Ground Support Equipment Connected */
#define d_FCU_PROGRAMMING      d_DISC_MIO_53  /* MAINT_DISC_2 - Programming mode */
#define d_FCU_MAINTENANCE      d_DISC_MIO_54  /* MAINT_DISC_3 - Maintenance mode */
#define d_FCU_HILS             d_DISC_MIO_55  /* MAINT_DISC_4 - Hardware in the Loop Simulation */

#define d_FCU_SAFE_LOOPBACK1   d_DISC_MIO_60  /* SAFE_DISC_OUT1_LB */
#define d_FCU_SAFE_LOOPBACK2   d_DISC_MIO_61  /* SAFE_DISC_OUT2_LB */
#define d_FCU_SAFE_FAULT1      d_DISC_MIO_62  /* SAFE_DISC_OUT1_FAULT */
#define d_FCU_SAFE_FAULT2      d_DISC_MIO_63  /* SAFE_DISC_OUT2_FAULT */

#define d_FCU_MASTER_FCU1      d_DISC_MIO_65  /* MCU1_MSEL */
#define d_FCU_MASTER_FCU2      d_DISC_MIO_66  /* MCU2_MSEL */
#define d_FCU_MASTER_FCU3      d_DISC_MIO_67  /* MCU3_MSEL */
#define d_FCU_NOGO_FCU1        d_DISC_MIO_68  /* IOC_NOGO1 */
#define d_FCU_NOGO_FCU2        d_DISC_MIO_69  /* IOC_NOGO2 */
#define d_FCU_NOGO_FCU3        d_DISC_MIO_70  /* IOC_NOGO3 */

#define d_FCU_SLOT1            d_DISC_MIO_72  /* FCU_SLOT_ADDR1 */
#define d_FCU_SLOT2            d_DISC_MIO_73  /* FCU_SLOT_ADDR2 */

/* Arbiter status register bit identification */
#define d_FCU_ARBITER_MASTER_0       0x00000001
#define d_FCU_ARBITER_MASTER_1       0x00000002
#define d_FCU_ARBITER_FCU1_LINK_UP   0x00000004
#define d_FCU_ARBITER_FCU2_LINK_UP   0x00000008
#define d_FCU_ARBITER_FCU1_WD_RESET  0x00000010
#define d_FCU_ARBITER_FCU2_WD_RESET  0x00000020
#define d_FCU_ARBITER_FCU1_NOGO1     0x00000040
#define d_FCU_ARBITER_FCU1_NOGO2     0x00000080
#define d_FCU_ARBITER_FCU2_NOGO1     0x00000100
#define d_FCU_ARBITER_FCU2_NOGO2     0x00000200

/* PS EMIO Discrete allocation */
#define d_FCU_SAFE_DISC_OUT1   d_DISC_EMIO_0  /* Safety discrete output 1 */
#define d_FCU_SAFE_DISC_OUT2   d_DISC_EMIO_1  /* Safety discrete output 2 */
#define d_FCU_DISC_OUT1        d_DISC_EMIO_2  /* Discrete output 1 */
#define d_FCU_DISC_OUT2        d_DISC_EMIO_3  /* Discrete output 2 */
#define d_FCU_DISC_OUT_FAULT1  d_DISC_EMIO_4  /* Discrete output fault 1 */
#define d_FCU_DISC_OUT_FAULT2  d_DISC_EMIO_5  /* Discrete output fault 2 */
#define d_FCU_SSD_SLEEP        d_DISC_EMIO_6  /* SATA SSD sleep enable */
#define d_FCU_IMU_PWR_ON       d_DISC_EMIO_7  /* IMU power enable */
#define d_FCU_IMU_PWR_FAULT    d_DISC_EMIO_8  /* IMU power overcurrent */
#define d_FCU_GNSS_PWR_ON      d_DISC_EMIO_9  /* GNSS power enable */
#define d_FCU_GNSS_PWR_FAULT   d_DISC_EMIO_10 /* GNSS power overcurrent */
#define d_FCU_PSU_DISC1        d_DISC_EMIO_11 /* PSU status discrete 1 */
#define d_FCU_PSU_DISC2        d_DISC_EMIO_12 /* PSU status discrete 2 */
#define d_FCU_ETH0_LINK_UP     d_DISC_EMIO_13 /* CCDL Ethernet 0 link up */
#define d_FCU_ETH1_LINK_UP     d_DISC_EMIO_14 /* CCDL Ethernet 1 link up */
#define d_FCU_IOCA_LINK_UP     d_DISC_EMIO_15 /* IOCA Ethernet 0 link up */
#define d_FCU_IOCB_LINK_UP     d_DISC_EMIO_16 /* IOCA Ethernet 1 link up */
#define d_FCU_GNSS_RESET       d_DISC_EMIO_17 /* GNSS reset output */

typedef enum
{
  d_FCU_FCU_1 = 0,
  d_FCU_FCU_2,
  d_FCU_FCU_3,
  d_FCU_FCU_COUNT
} d_FCU_Fcu_t;

typedef enum
{
  d_FCU_IOC_A = 0,
  d_FCU_IOC_B,
  d_FCU_IOC_COUNT
} d_FCU_Ioc_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise discretes */
d_Status_t d_FCU_Initialise(void);

/* Tell the PL that this FCU is Master, this is for synchronisation only */
void d_FCU_SetMaster(void);

/* Tell the PL that this FCU is Slave, this is for synchronisation only */
void d_FCU_SetSlave(void);

/* Get FCU slot number */
Uint32_t d_FCU_SlotNumber(void);

/* Get master FCU number */
Int32_t d_FCU_GetMaster(void);

/* Read arbiter status register */
d_Status_t d_FCU_ArbiterStatusRead(const Uint32_t ioc, Uint32_t * const pData);

/* Set the arbitration score for the FCU */
d_Status_t d_FCU_Score(const d_FCU_Fcu_t fcu, const Uint8_t score);

/* Set the NOGO output for the FCU */
d_Status_t d_FCU_NoGo(const d_FCU_Fcu_t fcu);

/* Get state of Ground Suport Equipment input */
Bool_t d_FCU_GseActive(void);

/* Get state of Programming mode input */
Bool_t d_FCU_ProgActive(void);

/* Get state of Maintenance mode input */
Bool_t d_FCU_MaintActive(void);

/* Get state of HILS input */
Bool_t d_FCU_HilsActive(void);

/* Get state of IOC online discrete input */
Bool_t d_FCU_IocOnline(const d_FCU_Ioc_t ioc);

/* Interrupt handler on change in master */
void d_FCU_Master_Handler(const Uint32_t selectedMaster);

#endif /* D_FCU_H */
