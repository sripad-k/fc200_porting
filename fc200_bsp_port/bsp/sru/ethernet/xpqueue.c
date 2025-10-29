/*********************************************************************//**
\file
\brief
  Module Title       : Ethernet Interface Module

  Abstract           : Queue functions for LWIP network stack library.

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-109
                                                                SWREQ-110
                       SDD References: 136T-2200-131000-001-D22 SWDES-551
                                                                SWDES-675
                                                                SWDES-676
                                                                SWDES-677
                                                                SWDES-678
                                                                SWDES-679
                                                                SWDES-680
                                                                SWDES-681

\note
  CSC ID             : SWDES-67
*************************************************************************/

/***** Includes *********************************************************/

#include "netif/xpqueue.h"
#include "soc/defines/d_common_status.h"
#include "kernel/error_handler/d_error_handler.h"

/***** Constants ********************************************************/

#define NUM_QUEUES	4u

/***** Type Definitions *************************************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*********************************************************************//**
  <!-- pq_create_queue -->

  Create a queue.
*************************************************************************/
pq_queue_t *       /** \return Pointer to the queue */
pq_create_queue
(
void
)
{
  static pq_queue_t pq_queue[NUM_QUEUES];
  static Uint32_t queueCount = 0u;

	pq_queue_t *q = NULL;

	if (queueCount >= NUM_QUEUES)
  {
    d_ERROR_Logger(d_STATUS_LIMIT_EXCEEDED, d_ERROR_CRITICALITY_UNKNOWN, NUM_QUEUES, 0, 0, 0);
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
		return q;
	}

	q = &pq_queue[queueCount];
  queueCount++;

	if (!q)
	{
		// gcov-jst 2 It is not practical to generate this failure during bench testing.
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
		return q;
	}

	q->head = 0;
  q->tail = 0;
  q->len = 0;

	return q;
}

/*********************************************************************//**
  <!-- pq_enqueue -->

  Add data to a queue.
*************************************************************************/
Int32_t            /** \return Zero if successful, else -1 */
pq_enqueue
(
pq_queue_t *q,     /**< [in] Pointer to the queue */
void *p            /**< [in] Pointer to data to add */
)
{
  if (q->len == PQ_QUEUE_SIZE)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
    return -1;
  }
  
	q->data[q->head] = p;
	q->head = (q->head + 1) % PQ_QUEUE_SIZE;
	q->len++;

	return 0;
}

/*********************************************************************//**
  <!-- pq_dequeue -->

  Remove data from a queue.
*************************************************************************/
void *             /** \return Pointer to the data extracted from the queue */
pq_dequeue
(
pq_queue_t *q      /**< [in] Pointer to the queue */
)
{
	int ptail;

	if (q->len == 0)
  {
    // cppcheck-suppress misra-c2012-15.5; Coding standard allows function to return if parameters are invalid
		return NULL;
  }
  
	ptail = q->tail;
	q->tail = (q->tail + 1)%PQ_QUEUE_SIZE;
	q->len--;

	return q->data[ptail];
}

/*********************************************************************//**
  <!-- pq_qlength -->

  Get the length of a queue.
*************************************************************************/
Int32_t            /** \return Length of the queue */
pq_qlength
(
pq_queue_t *q      /**< [in] Pointer to the queue */
)
{
	return q->len;
}
