/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_discrete

  Abstract           : General Purpose Input Output functions for the
                       Processing System and the Programmable Logic.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-114
                       SDD References: 136T-2200-131100-001-D22 SWDES-86
                                                                SWDES-88
                                                                SWDES-89
                                                                SWDES-90
                                                                SWDES-91
                                                                SWDES-92
                                                                SWDES-93

\note
  CSC ID             : SWDES-42
*************************************************************************/

#ifndef D_DISCRETE_H
#define D_DISCRETE_H

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"   /* Common type definitions */
#include "soc/defines/d_common_status.h"  /* Common status and error definitions */

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

typedef enum
{
  /* PS external GPIO */
  d_DISC_MIO_0 = 0u,
  d_DISC_MIO_1,
  d_DISC_MIO_2,
  d_DISC_MIO_3,
  d_DISC_MIO_4,
  d_DISC_MIO_5,
  d_DISC_MIO_6,
  d_DISC_MIO_7,
  d_DISC_MIO_8,
  d_DISC_MIO_9,
  d_DISC_MIO_10,
  d_DISC_MIO_11,
  d_DISC_MIO_12,
  d_DISC_MIO_13,
  d_DISC_MIO_14,
  d_DISC_MIO_15,
  d_DISC_MIO_16,
  d_DISC_MIO_17,
  d_DISC_MIO_18,
  d_DISC_MIO_19,
  d_DISC_MIO_20,
  d_DISC_MIO_21,
  d_DISC_MIO_22,
  d_DISC_MIO_23,
  d_DISC_MIO_24,
  d_DISC_MIO_25,
  d_DISC_MIO_26,
  d_DISC_MIO_27,
  d_DISC_MIO_28,
  d_DISC_MIO_29,
  d_DISC_MIO_30,
  d_DISC_MIO_31,
  d_DISC_MIO_32,
  d_DISC_MIO_33,
  d_DISC_MIO_34,
  d_DISC_MIO_35,
  d_DISC_MIO_36,
  d_DISC_MIO_37,
  d_DISC_MIO_38,
  d_DISC_MIO_39,
  d_DISC_MIO_40,
  d_DISC_MIO_41,
  d_DISC_MIO_42,
  d_DISC_MIO_43,
  d_DISC_MIO_44,
  d_DISC_MIO_45,
  d_DISC_MIO_46,
  d_DISC_MIO_47,
  d_DISC_MIO_48,
  d_DISC_MIO_49,
  d_DISC_MIO_50,
  d_DISC_MIO_51,
  d_DISC_MIO_52,
  d_DISC_MIO_53,
  d_DISC_MIO_54,
  d_DISC_MIO_55,
  d_DISC_MIO_56,
  d_DISC_MIO_57,
  d_DISC_MIO_58,
  d_DISC_MIO_59,
  d_DISC_MIO_60,
  d_DISC_MIO_61,
  d_DISC_MIO_62,
  d_DISC_MIO_63,
  d_DISC_MIO_64,
  d_DISC_MIO_65,
  d_DISC_MIO_66,
  d_DISC_MIO_67,
  d_DISC_MIO_68,
  d_DISC_MIO_69,
  d_DISC_MIO_70,
  d_DISC_MIO_71,
  d_DISC_MIO_72,
  d_DISC_MIO_73,
  d_DISC_MIO_74,
  d_DISC_MIO_75,
  d_DISC_MIO_76,
  d_DISC_MIO_77,

  /* PS to PL GPIO */
  d_DISC_EMIO_0,
  d_DISC_EMIO_1,
  d_DISC_EMIO_2,
  d_DISC_EMIO_3,
  d_DISC_EMIO_4,
  d_DISC_EMIO_5,
  d_DISC_EMIO_6,
  d_DISC_EMIO_7,
  d_DISC_EMIO_8,
  d_DISC_EMIO_9,
  d_DISC_EMIO_10,
  d_DISC_EMIO_11,
  d_DISC_EMIO_12,
  d_DISC_EMIO_13,
  d_DISC_EMIO_14,
  d_DISC_EMIO_15,
  d_DISC_EMIO_16,
  d_DISC_EMIO_17,
  d_DISC_EMIO_18,
  d_DISC_EMIO_19,
  d_DISC_EMIO_20,
  d_DISC_EMIO_21,
  d_DISC_EMIO_22,
  d_DISC_EMIO_23,
  d_DISC_EMIO_24,
  d_DISC_EMIO_25,
  d_DISC_EMIO_26,
  d_DISC_EMIO_27,
  d_DISC_EMIO_28,
  d_DISC_EMIO_29,
  d_DISC_EMIO_30,
  d_DISC_EMIO_31,
  d_DISC_EMIO_32,
  d_DISC_EMIO_33,
  d_DISC_EMIO_34,
  d_DISC_EMIO_35,
  d_DISC_EMIO_36,
  d_DISC_EMIO_37,
  d_DISC_EMIO_38,
  d_DISC_EMIO_39,
  d_DISC_EMIO_40,
  d_DISC_EMIO_41,
  d_DISC_EMIO_42,
  d_DISC_EMIO_43,
  d_DISC_EMIO_44,
  d_DISC_EMIO_45,
  d_DISC_EMIO_46,
  d_DISC_EMIO_47,
  d_DISC_EMIO_48,
  d_DISC_EMIO_49,
  d_DISC_EMIO_50,
  d_DISC_EMIO_51,
  d_DISC_EMIO_52,
  d_DISC_EMIO_53,
  d_DISC_EMIO_54,
  d_DISC_EMIO_55,
  d_DISC_EMIO_56,
  d_DISC_EMIO_57,
  d_DISC_EMIO_58,
  d_DISC_EMIO_59,
  d_DISC_EMIO_60,
  d_DISC_EMIO_61,
  d_DISC_EMIO_62,
  d_DISC_EMIO_63,
  d_DISC_EMIO_64,
  d_DISC_EMIO_65,
  d_DISC_EMIO_66,
  d_DISC_EMIO_67,
  d_DISC_EMIO_68,
  d_DISC_EMIO_69,
  d_DISC_EMIO_70,
  d_DISC_EMIO_71,
  d_DISC_EMIO_72,
  d_DISC_EMIO_73,
  d_DISC_EMIO_74,
  d_DISC_EMIO_75,
  d_DISC_EMIO_76,
  d_DISC_EMIO_77,
  d_DISC_EMIO_78,
  d_DISC_EMIO_79,
  d_DISC_EMIO_80,
  d_DISC_EMIO_81,
  d_DISC_EMIO_82,
  d_DISC_EMIO_83,
  d_DISC_EMIO_84,
  d_DISC_EMIO_85,
  d_DISC_EMIO_86,
  d_DISC_EMIO_87,
  d_DISC_EMIO_88,
  d_DISC_EMIO_89,
  d_DISC_EMIO_90,
  d_DISC_EMIO_91,
  d_DISC_EMIO_92,
  d_DISC_EMIO_93,
  d_DISC_EMIO_94,
  d_DISC_EMIO_95,
  d_DISC_IO_COUNT
} d_DISC_IO_t;

typedef enum
{
  d_DISC_TRIGGER_LEVEL_LOW = 0,
  d_DISC_TRIGGER_LEVEL_HIGH,
  d_DISC_TRIGGER_EDGE_FALLING,
  d_DISC_TRIGGER_EDGE_RISING,
  d_DISC_TRIGGER_COUNT
} d_DISC_Trigger_t;

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Set the direction of a discrete pin to an input */
d_Status_t d_DISC_SetAsInputPin(const d_DISC_IO_t pin);

/* Set the direction of a discrete pin to an output */
d_Status_t d_DISC_SetAsOutputPin(const d_DISC_IO_t pin);

/* Read the status of a discrete pin */
d_Status_t d_DISC_ReadPin(const d_DISC_IO_t pin, Uint32_t * const pPinStatus);

/* Set the status of a discrete pin */
d_Status_t d_DISC_SetPin(const d_DISC_IO_t pin);

/* Clear the status of a discrete pin */
d_Status_t d_DISC_ClearPin(const d_DISC_IO_t pin);

/* Toggle the status of a discrete pin */
d_Status_t d_DISC_TogglePin(const d_DISC_IO_t pin);

/* Enable the interrupt on a discrete pin */
d_Status_t d_DISC_InterruptEnable(const d_DISC_IO_t pin, const d_DISC_Trigger_t trigger);

/* Disable the interrupt on a discrete pin */
d_Status_t d_DISC_InterruptDisable(const d_DISC_IO_t pin);

void d_DISC_InterruptHandler(const Uint32_t parameter);

#endif /* D_DISCRETE_H */
