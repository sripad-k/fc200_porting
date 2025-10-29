/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title : Coverage Measurement

  Abstract : Module to provide capability to measure statement coverage

  Software Structure : SDD References: N/A

*************************************************************************/

#ifndef D_COVERAGE_H_
#define D_COVERAGE_H_

/***** Includes *********************************************************/

#include "soc/defines/d_common_types.h"
#include "soc/defines/d_common_status.h"

/***** Literals ********************************************************/

/**
 * Use the macro DO_NOTHING() to insert a line of local code which is recognized
 * by gcov. This will aid in achieving 'decision' coverage.
 */
#ifdef COVERAGE
  #define DO_NOTHING() __asm__ __volatile__("NOP")
#else
  #define DO_NOTHING()
#endif

/***** Constants ********************************************************/

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/* Initialise coverage network interface. Should only be called after d_ETH_Initialise has returned */
d_Status_t d_COV_Initialise(void);

/* Return a flag indicating if the coverage dump has been completed */
Bool_t d_COV_Complete(void);

/* Continue capturing data after a download */
void d_COV_Continue(void);

/* Background process. Check for request to dump coverage data */
void d_COV_Background(void);

#endif /* D_COVERAGE_H_ */
