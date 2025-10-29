/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : discrete_cfg.c

  Abstract           : Weak table of discrete interrupt handlers.

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

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/discrete/d_discrete.h"
#include "soc/discrete/d_discrete_cfg.h"
#include "sru/fcu/d_fcu.h"

/***** Constants ********************************************************/

__attribute__((weak)) const d_DISC_IrqVectorTable_t d_DISC_IrqVectorTable[d_DISC_IO_COUNT] =
{
    {NULL, 0},                          /* d_DISC_MIO_0 */
    {NULL, 0},                          /* d_DISC_MIO_1 */
    {NULL, 0},                          /* d_DISC_MIO_2 */
    {NULL, 0},                          /* d_DISC_MIO_3 */
    {NULL, 0},                          /* d_DISC_MIO_4 */
    {NULL, 0},                          /* d_DISC_MIO_5 */
    {NULL, 0},                          /* d_DISC_MIO_6 */
    {NULL, 0},                          /* d_DISC_MIO_7 */
    {NULL, 0},                          /* d_DISC_MIO_8 */
    {NULL, 0},                          /* d_DISC_MIO_9 */
    {NULL, 0},                          /* d_DISC_MIO_10 */
    {NULL, 0},                          /* d_DISC_MIO_11 */
    {NULL, 0},                          /* d_DISC_MIO_12 */
    {NULL, 0},                          /* d_DISC_MIO_13 */
    {NULL, 0},                          /* d_DISC_MIO_14 */
    {NULL, 0},                          /* d_DISC_MIO_15 */
    {NULL, 0},                          /* d_DISC_MIO_16 */
    {NULL, 0},                          /* d_DISC_MIO_17 */
    {NULL, 0},                          /* d_DISC_MIO_18 */
    {NULL, 0},                          /* d_DISC_MIO_19 */
    {NULL, 0},                          /* d_DISC_MIO_20 */
    {NULL, 0},                          /* d_DISC_MIO_21 */
    {NULL, 0},                          /* d_DISC_MIO_22 */
    {NULL, 0},                          /* d_DISC_MIO_23 */
    {NULL, 0},                          /* d_DISC_MIO_24 */
    {NULL, 0},                          /* d_DISC_MIO_25 */
    {NULL, 0},                          /* d_DISC_MIO_26 */
    {NULL, 0},                          /* d_DISC_MIO_27 */
    {NULL, 0},                          /* d_DISC_MIO_28 */
    {NULL, 0},                          /* d_DISC_MIO_29 */
    {NULL, 0},                          /* d_DISC_MIO_30 */
    {NULL, 0},                          /* d_DISC_MIO_31 */
    {NULL, 0},                          /* d_DISC_MIO_32 */
    {NULL, 0},                          /* d_DISC_MIO_33 */
    {NULL, 0},                          /* d_DISC_MIO_34 */
    {NULL, 0},                          /* d_DISC_MIO_35 */
    {NULL, 0},                          /* d_DISC_MIO_36 */
    {NULL, 0},                          /* d_DISC_MIO_37 */
    {NULL, 0},                          /* d_DISC_MIO_38 */
    {NULL, 0},                          /* d_DISC_MIO_39 */
    {NULL, 0},                          /* d_DISC_MIO_40 */
    {NULL, 0},                          /* d_DISC_MIO_41 */
    {NULL, 0},                          /* d_DISC_MIO_42 */
    {NULL, 0},                          /* d_DISC_MIO_43 */
    {NULL, 0},                          /* d_DISC_MIO_44 */
    {NULL, 0},                          /* d_DISC_MIO_45 */
    {NULL, 0},                          /* d_DISC_MIO_46 */
    {NULL, 0},                          /* d_DISC_MIO_47 */
    {NULL, 0},                          /* d_DISC_MIO_48 */
    {NULL, 0},                          /* d_DISC_MIO_49 */
    {NULL, 0},                          /* d_DISC_MIO_50 */
    {NULL, 0},                          /* d_DISC_MIO_51 */
    {NULL, 0},                          /* d_DISC_MIO_52 */
    {NULL, 0},                          /* d_DISC_MIO_53 */
    {NULL, 0},                          /* d_DISC_MIO_54 */
    {NULL, 0},                          /* d_DISC_MIO_55 */
    {NULL, 0},                          /* d_DISC_MIO_56 */
    {NULL, 0},                          /* d_DISC_MIO_57 */
    {NULL, 0},                          /* d_DISC_MIO_58 */
    {NULL, 0},                          /* d_DISC_MIO_59 */
    {NULL, 0},                          /* d_DISC_MIO_60 */
    {NULL, 0},                          /* d_DISC_MIO_61 */
    {NULL, 0},                          /* d_DISC_MIO_62 */
    {NULL, 0},                          /* d_DISC_MIO_63 */
    {NULL, 0},                          /* d_DISC_MIO_64 */
    {d_FCU_Master_Handler, 0},          /* d_DISC_MIO_65 */
    {d_FCU_Master_Handler, 1},          /* d_DISC_MIO_66 */
    {d_FCU_Master_Handler, 2},          /* d_DISC_MIO_67 */
    {NULL, 0},                          /* d_DISC_MIO_68 */
    {NULL, 0},                          /* d_DISC_MIO_69 */
    {NULL, 0},                          /* d_DISC_MIO_70 */
    {NULL, 0},                          /* d_DISC_MIO_71 */
    {NULL, 0},                          /* d_DISC_MIO_72 */
    {NULL, 0},                          /* d_DISC_MIO_73 */
    {NULL, 0},                          /* d_DISC_MIO_74 */
    {NULL, 0},                          /* d_DISC_MIO_75 */
    {NULL, 0},                          /* d_DISC_MIO_76 */
    {NULL, 0},                          /* d_DISC_MIO_77 */

    {NULL, 0},                          /* d_DISC_EMIO_0 */
    {NULL, 0},                          /* d_DISC_EMIO_1 */
    {NULL, 0},                          /* d_DISC_EMIO_2 */
    {NULL, 0},                          /* d_DISC_EMIO_3 */
    {NULL, 0},                          /* d_DISC_EMIO_4 */
    {NULL, 0},                          /* d_DISC_EMIO_5 */
    {NULL, 0},                          /* d_DISC_EMIO_6 */
    {NULL, 0},                          /* d_DISC_EMIO_7 */
    {NULL, 0},                          /* d_DISC_EMIO_8 */
    {NULL, 0},                          /* d_DISC_EMIO_9 */
    {NULL, 0},                          /* d_DISC_EMIO_10 */
    {NULL, 0},                          /* d_DISC_EMIO_11 */
    {NULL, 0},                          /* d_DISC_EMIO_12 */
    {NULL, 0},                          /* d_DISC_EMIO_13 */
    {NULL, 0},                          /* d_DISC_EMIO_14 */
    {NULL, 0},                          /* d_DISC_EMIO_15 */
    {NULL, 0},                          /* d_DISC_EMIO_16 */
    {NULL, 0},                          /* d_DISC_EMIO_17 */
    {NULL, 0},                          /* d_DISC_EMIO_18 */
    {NULL, 0},                          /* d_DISC_EMIO_19 */
    {NULL, 0},                          /* d_DISC_EMIO_20 */
    {NULL, 0},                          /* d_DISC_EMIO_21 */
    {NULL, 0},                          /* d_DISC_EMIO_22 */
    {NULL, 0},                          /* d_DISC_EMIO_23 */
    {NULL, 0},                          /* d_DISC_EMIO_24 */
    {NULL, 0},                          /* d_DISC_EMIO_25 */
    {NULL, 0},                          /* d_DISC_EMIO_26 */
    {NULL, 0},                          /* d_DISC_EMIO_27 */
    {NULL, 0},                          /* d_DISC_EMIO_28 */
    {NULL, 0},                          /* d_DISC_EMIO_29 */
    {NULL, 0},                          /* d_DISC_EMIO_30 */
    {NULL, 0},                          /* d_DISC_EMIO_31 */
    {NULL, 0},                          /* d_DISC_EMIO_32 */
    {NULL, 0},                          /* d_DISC_EMIO_33 */
    {NULL, 0},                          /* d_DISC_EMIO_34 */
    {NULL, 0},                          /* d_DISC_EMIO_35 */
    {NULL, 0},                          /* d_DISC_EMIO_36 */
    {NULL, 0},                          /* d_DISC_EMIO_37 */
    {NULL, 0},                          /* d_DISC_EMIO_38 */
    {NULL, 0},                          /* d_DISC_EMIO_39 */
    {NULL, 0},                          /* d_DISC_EMIO_40 */
    {NULL, 0},                          /* d_DISC_EMIO_41 */
    {NULL, 0},                          /* d_DISC_EMIO_42 */
    {NULL, 0},                          /* d_DISC_EMIO_43 */
    {NULL, 0},                          /* d_DISC_EMIO_44 */
    {NULL, 0},                          /* d_DISC_EMIO_45 */
    {NULL, 0},                          /* d_DISC_EMIO_46 */
    {NULL, 0},                          /* d_DISC_EMIO_47 */
    {NULL, 0},                          /* d_DISC_EMIO_48 */
    {NULL, 0},                          /* d_DISC_EMIO_49 */
    {NULL, 0},                          /* d_DISC_EMIO_50 */
    {NULL, 0},                          /* d_DISC_EMIO_51 */
    {NULL, 0},                          /* d_DISC_EMIO_52 */
    {NULL, 0},                          /* d_DISC_EMIO_53 */
    {NULL, 0},                          /* d_DISC_EMIO_54 */
    {NULL, 0},                          /* d_DISC_EMIO_55 */
    {NULL, 0},                          /* d_DISC_EMIO_56 */
    {NULL, 0},                          /* d_DISC_EMIO_57 */
    {NULL, 0},                          /* d_DISC_EMIO_58 */
    {NULL, 0},                          /* d_DISC_EMIO_59 */
    {NULL, 0},                          /* d_DISC_EMIO_60 */
    {NULL, 0},                          /* d_DISC_EMIO_61 */
    {NULL, 0},                          /* d_DISC_EMIO_62 */
    {NULL, 0},                          /* d_DISC_EMIO_63 */
    {NULL, 0},                          /* d_DISC_EMIO_64 */
    {NULL, 0},                          /* d_DISC_EMIO_65 */
    {NULL, 0},                          /* d_DISC_EMIO_66 */
    {NULL, 0},                          /* d_DISC_EMIO_67 */
    {NULL, 0},                          /* d_DISC_EMIO_68 */
    {NULL, 0},                          /* d_DISC_EMIO_69 */
    {NULL, 0},                          /* d_DISC_EMIO_70 */
    {NULL, 0},                          /* d_DISC_EMIO_71 */
    {NULL, 0},                          /* d_DISC_EMIO_72 */
    {NULL, 0},                          /* d_DISC_EMIO_73 */
    {NULL, 0},                          /* d_DISC_EMIO_74 */
    {NULL, 0},                          /* d_DISC_EMIO_75 */
    {NULL, 0},                          /* d_DISC_EMIO_76 */
    {NULL, 0},                          /* d_DISC_EMIO_77 */
    {NULL, 0},                          /* d_DISC_EMIO_78 */
    {NULL, 0},                          /* d_DISC_EMIO_79 */
    {NULL, 0},                          /* d_DISC_EMIO_80 */
    {NULL, 0},                          /* d_DISC_EMIO_81 */
    {NULL, 0},                          /* d_DISC_EMIO_82 */
    {NULL, 0},                          /* d_DISC_EMIO_83 */
    {NULL, 0},                          /* d_DISC_EMIO_84 */
    {NULL, 0},                          /* d_DISC_EMIO_85 */
    {NULL, 0},                          /* d_DISC_EMIO_86 */
    {NULL, 0},                          /* d_DISC_EMIO_87 */
    {NULL, 0},                          /* d_DISC_EMIO_88 */
    {NULL, 0},                          /* d_DISC_EMIO_89 */
    {NULL, 0},                          /* d_DISC_EMIO_90 */
    {NULL, 0},                          /* d_DISC_EMIO_91 */
    {NULL, 0},                          /* d_DISC_EMIO_92 */
    {NULL, 0},                          /* d_DISC_EMIO_93 */
    {NULL, 0},                          /* d_DISC_EMIO_94 */
    {NULL, 0},                          /* d_DISC_EMIO_95 */
};

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/
