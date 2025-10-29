/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_cstart.s

  Abstract           : C startup code before main.

  Software Structure : SRS References: 136T-2200-131100-001-D20 SWREQ-318
                                                                SWREQ-323
                       SDD References: 136T-2200-131000-001-D22 SWDES-292

\note
  CSC_ID             : SWDES-54
*************************************************************************/

#include "xparameters.h"
#include "bspconfig.h"

	.file	"d_cstart.s"
	.section ".got2","aw"
	.align	2
/*
 * 0th bit of PROCESSOR_ACCESS_VALUE macro signifies trustzone
 * setting for RPU address space
 */
#define RPU_TZ_MASK	0x1
	.text
.Lsbss_start:
	.long	__sbss_start

.Lsbss_end:
	.long	__sbss_end

.Lbss_start:
	.long	__bss_start__

.Lbss_end:
	.long	__bss_end__

.Lstack:
	.long	__stack

.set RPU_0_PWRCTL, 0xFF9A0108
.set RPU_1_PWRCTL, 0xFF9A0208
.set MPIDR_AFF0,   0xFF
.set PWRCTL_MASK,  0x1

	.globl	_startup

_startup:
/* Clear cp15 regs with unknown reset values */
	mov	r0, #0x0
	mcr	p15, 0, r0, c5, c0, 0	/* DFSR */
	mcr	p15, 0, r0, c5, c0, 1	/* IFSR */
	mcr	p15, 0, r0, c6, c0, 0	/* DFAR */
	mcr	p15, 0, r0, c6, c0, 2	/* IFAR */
	mcr	p15, 0, r0, c9, c13, 2	/* PMXEVCNTR */
	mcr	p15, 0, r0, c13, c0, 2	/* TPIDRURW */
	mcr	p15, 0, r0, c13, c0, 3	/* TPIDRURO */

/* Reset and start Cycle Counter */
	mov	r2, #0x80000000		/* clear overflow */
	mcr	p15, 0, r2, c9, c12, 3
	mov	r2, #0xd		/* D, C, E */
	mcr	p15, 0, r2, c9, c12, 0
	mov	r2, #0x80000000		/* enable cycle counter */
	mcr	p15, 0, r2, c9, c12, 1

	mov	r0, #0

	/* clear sbss */
	ldr 	r1,.Lsbss_start		/* calculate beginning of the SBSS */
	ldr	r2,.Lsbss_end		/* calculate end of the SBSS */

.Lloop_sbss:
	cmp	r1,r2
	bge	.Lenclsbss		/* If no SBSS, no clearing required */
	str	r0, [r1], #4
	b	.Lloop_sbss

.Lenclsbss:
	/* clear bss */
	ldr	r1,.Lbss_start		/* calculate beginning of the BSS */
	ldr	r2,.Lbss_end		/* calculate end of the BSS */

.Lloop_bss:
	cmp	r1,r2
	bge	.Lenclbss		/* If no BSS, no clearing required */
	str	r0, [r1], #4
	b	.Lloop_bss

.Lenclbss:

	/* set stack pointer */
	ldr	r13,.Lstack		/* stack address */

	/* configure the timer if TTC is present */
#ifdef SLEEP_TIMER_BASEADDR
    /* May be used by Xilinx code; LWIP, MMC, FATFS, other */
    bl XTime_StartTTCTimer  /* Required by bootloader */
#endif
	bl 	d_MEMORY_InitialiseExistingMpuRegConfig	/* Initialize MPU config */
	/* run global constructors */
	bl __libc_init_array

	/* make sure argc and argv are valid */
	mov	r0, #0
	mov	r1, #0

	bl	main			/* Jump to main C code */

	/* Cleanup global constructors */
	bl __libc_fini_array

	bl	exit

.Lexit:	/* should never get here */
	b .Lexit

.Lstart:
	.size	_startup,.Lstart-_startup
