/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_int_vector.s

  Abstract           : nterrupt vector handling, FreeRTOS platform.

  Software Structure : SRS References: 136T-2200-131000-001-D22 SWREQ-164
                                                                SWREQ-165
                                                                SWREQ-166
                                                                SWREQ-167
                       SDD References: 136T-2200-131000-001-D22 SWDES-172
                                                                SWDES-173
                                                                SWDES-174
                                                                SWDES-175
                                                                SWDES-176
                                                                SWDES-177
                                                                SWDES-178
                                                                SWDES-179
                                                                SWDES-180
                                                                SWDES-181
                                                                SWDES-182
                                                                SWDES-183
                                                                SWDES-184
                                                                SWDES-185
                                                                SWDES-186
                                                                SWDES-187
                                                                SWDES-188
\note
  CSC_ID             : SWDES-52
*************************************************************************/

.org 0
.text

#ifdef FREERTOS
.global vPortRestoreTaskContext
#endif

.globl _boot
.globl _vector_table

.globl d_ERROR_ExceptionFiqHandler
.globl d_ERROR_ExceptionUndefHandler
.globl d_ERROR_ExceptionSwintHandler
.globl d_ERROR_ExceptionDataHandler
.globl d_ERROR_ExceptionAddrHandler

.globl d_INT_IrqHandler

.section .vectors, "a"
_vector_table:
    ldr    pc,=_boot
    ldr    pc,=Undefined
#ifdef FREERTOS
    ldr    pc,=FreeRTOS_SWI_Handler
#else
    ldr    pc,=SVCHandler
#endif
    ldr    pc,=PrefetchAbortHandler
    ldr    pc,=DataAbortHandler
    NOP    /* Placeholder for address exception vector*/
#ifdef FREERTOS
    ldr    pc,=FreeRTOS_IRQ_Handler
#else
    ldr    pc,=IRQ_Handler
#endif
    ldr    pc,=FIQHandler

.text

	.set SVC_MODE,	0x13
	.set IRQ_MODE,	0x12
	.extern ulICCIAR
	.extern ulICCEOIR

#ifdef FREERTOS
	.arm

	.set SYS_MODE,	0x1f

	/* Hardware registers. */
	.extern ulICCPMR

.macro portSAVE_CONTEXT

	/* Save the LR and SPSR onto the system mode stack before switching to
	system mode to save the remaining system mode registers. */
	SRSDB	sp!, #SYS_MODE
	CPS		#SYS_MODE
	PUSH	{R0-R12, R14}

	/* Push the critical nesting count. */
	LDR		R2, ulCriticalNestingConst
	LDR		R1, [R2]
	PUSH	{R1}

	/* Does the task have a floating point context that needs saving?  If
	ulPortTaskHasFPUContext is 0 then no. */
	LDR		R2, ulPortTaskHasFPUContextConst
	LDR		R3, [R2]
	CMP		R3, #0

	/* Save the floating point context, if any. */
	FMRXNE  R1,  FPSCR
	VPUSHNE {D0-D15}
	PUSHNE	{R1}

	/* Save ulPortTaskHasFPUContext itself. */
	PUSH	{R3}

	/* Save the stack pointer in the TCB. */
	LDR		R0, pxCurrentTCBConst
	LDR		R1, [R0]
	STR		SP, [R1]

	.endm

; /**********************************************************************/

.macro portRESTORE_CONTEXT

	/* Set the SP to point to the stack of the task being restored. */
	LDR		R0, pxCurrentTCBConst
	LDR		R1, [R0]
	LDR		SP, [R1]

	/* Is there a floating point context to restore?  If the restored
	ulPortTaskHasFPUContext is zero then no. */
	LDR		R0, ulPortTaskHasFPUContextConst
	POP		{R1}
	STR		R1, [R0]
	CMP		R1, #0

	/* Restore the floating point context, if any. */
	POPNE 	{R0}
	VPOPNE	{D0-D15}
	VMSRNE  FPSCR, R0

	/* Restore the critical section nesting depth. */
	LDR		R0, ulCriticalNestingConst
	POP		{R1}
	STR		R1, [R0]

	/* Ensure the priority mask is correct for the critical nesting depth. */
	LDR		R2, ulICCPMRConst
	LDR		R2, [R2]
	CMP		R1, #0
	MOVEQ	R4, #255
	LDRNE	R4, ulMaxAPIPriorityMaskConst
	LDRNE	R4, [R4]
	STR		R4, [R2]

	/* Restore all system mode registers other than the SP (which is already
	being used). */
	POP		{R0-R12, R14}

	/* Return to the task code, loading CPSR on the way. */
	RFEIA	sp!

	.endm

/******************************************************************************
 * vPortRestoreTaskContext is used to start the scheduler.
 *****************************************************************************/
.type vPortRestoreTaskContext, %function
vPortRestoreTaskContext:
	/* Switch to system mode. */
	CPS		#SYS_MODE
	portRESTORE_CONTEXT

/* ----- FreeRTOS IRQ handler -----------------------------------------------*/
.align 4
.type FreeRTOS_IRQ_Handler, %function
FreeRTOS_IRQ_Handler:
	/* Return to the interrupted instruction. */
	SUB		lr, lr, #4

	/* Push the return address and SPSR. */
	PUSH	{lr}
	MRS		lr, SPSR
	PUSH	{lr}

	/* Change to supervisor mode to allow reentry. */
	CPS		#SVC_MODE

	/* Push used registers. */
	PUSH	{r0-r4, r12}

	/* Increment nesting count.  r3 holds the address of ulPortInterruptNesting
	for future use.  r1 holds the original ulPortInterruptNesting value for
	future use. */
	LDR		r3, ulPortInterruptNestingConst
	LDR		r1, [r3]
	ADD		r4, r1, #1
	STR		r4, [r3]

	/* Ensure bit 2 of the stack pointer is clear.  r2 holds the bit 2 value for
	future use.  _RB_ Is this ever needed provided the start of the stack is
	aligned on an 8-byte boundary? */
	MOV		r2, sp
	AND		r2, r2, #4
	SUB		sp, sp, r2

	/* Call the interrupt handler. */
	PUSH	{r0-r4, lr}
	bl d_INT_IrqHandler
	POP		{r0-r4, lr}
	ADD		sp, sp, r2

	CPSID	i
	DSB
	ISB

	/* Restore the old nesting count. */
	STR		r1, [r3]

	/* A context switch is never performed if the nesting count is not 0. */
	CMP		r1, #0
	BNE		exit_without_switch

	/* Did the interrupt request a context switch?  r1 holds the address of
	ulPortYieldRequired and r0 the value of ulPortYieldRequired for future
	use. */
	LDR		r1, =ulPortYieldRequired
	LDR		r0, [r1]
	CMP		r0, #0
	BNE		switch_before_exit

exit_without_switch:
	/* No context switch.  Restore used registers, LR_irq and SPSR before
	returning. */
	POP		{r0-r4, r12}
	CPS		#IRQ_MODE
	POP		{LR}
	MSR		SPSR_cxsf, LR
	POP		{LR}
	MOVS	PC, LR

switch_before_exit:
	/* A context switch is to be performed.  Clear the context switch pending
	flag. */
	MOV		r0, #0
	STR		r0, [r1]

	/* Restore used registers, LR-irq and SPSR before saving the context
	to the task stack. */
	POP		{r0-r4, r12}
	CPS		#IRQ_MODE
	POP		{LR}
	MSR		SPSR_cxsf, LR
	POP		{LR}
	portSAVE_CONTEXT

	/* Call the function that selects the new task to execute.
	vTaskSwitchContext() if vTaskSwitchContext() uses LDRD or STRD
	instructions, or 8 byte aligned stack allocated data.  LR does not need
	saving as a new LR will be loaded by portRESTORE_CONTEXT anyway. */
	LDR		R0, vTaskSwitchContextConst
	BLX		R0

	/* Restore the context of, and branch to, the task selected to execute
	next. */
	portRESTORE_CONTEXT
#else

/* ----- Non FreeRTOS IRQ handler -------------------------------------------*/
IRQ_Handler:

	/* Return to the interrupted instruction. */
	SUB		lr, lr, #4

	/* Push the return address and SPSR. */
	PUSH	{lr}
	MRS		lr, SPSR
	PUSH	{lr}

	/* Change to supervisor mode to allow reentry. */
	CPS		#SVC_MODE

	/* Push used registers. */
	PUSH	{r0-r4, r12}

    vpush  {d0-d7}                       /* Store floating point registers */
    vmrs   r1, FPSCR
    push   {r1}
    vmrs   r1, FPEXC
    push   {r1}

	/* Ensure bit 2 of the stack pointer is clear.  r2 holds the bit 2 value for
	future use.  _RB_ Is this ever needed provided the start of the stack is
	aligned on an 8-byte boundary? */
	MOV		r2, sp
	AND		r2, r2, #4
	SUB		sp, sp, r2

	/* Call the interrupt handler. */
	PUSH	{r0-r4, lr}
  LDR   r1, d_INT_IrqHandlerConst
	BLX		r1
	POP		{r0-r4, lr}
	ADD		sp, sp, r2

	CPSID	i
	DSB
	ISB

    pop    {r1}                          /* Restore floating point registers */
    vmsr   FPEXC, r1
    pop    {r1}
    vmsr   FPSCR, r1
    vpop   {d0-d7}

	/* No context switch.  Restore used registers, LR_irq and SPSR before
	returning. */
	POP		{r0-r4, r12}
	CPS		#IRQ_MODE
	POP		{LR}
	MSR		SPSR_cxsf, LR
	POP		{LR}
	MOVS	PC, LR

#endif

FIQHandler:                              /* FIQ vector handler */
    stmdb  sp!,{r0-r3,r12,lr}            /* state save from compiled code */
    bl     d_ERROR_ExceptionFiqHandler   /* FIQ vector */
    ldmia  sp!,{r0-r3,r12,lr}            /* state restore from compiled code */
    subs   pc, lr, #4                    /* adjust return */

Undefined:                                /* Undefined handler */
    stmdb  sp!,{r0-r3,r12,lr}             /* state save from compiled code */
    ldr    r0, =UndefinedExceptionAddr
    sub    r1, lr, #4
    str    r1, [r0]                       /* Store address of instruction causing undefined exception */
    bl     d_ERROR_ExceptionUndefHandler  /* UndefinedException: call C function here */
    ldmia  sp!,{r0-r3,r12,lr}             /* state restore from compiled code */
    movs   pc, lr

/******************************************************************************
 * SVC handler.
 *****************************************************************************/
#ifdef FREERTOS
.align 4
.type FreeRTOS_SWI_Handler, %function
FreeRTOS_SWI_Handler:                               /* SWI handler */
	portSAVE_CONTEXT
	LDR R0, vTaskSwitchContextConst
	BLX	R0
	portRESTORE_CONTEXT
#else
SVCHandler:                               /* SWI handler */
    stmdb  sp!,{r0-r3,r12,lr}             /* state save from compiled code */
    tst    r0, #0x20                      /* check the T bit */
    ldrneh r0, [lr,#-2]                   /* Thumb mode */
    bicne  r0, r0, #0xff00                /* Thumb mode */
    ldreq  r0, [lr,#-4]                   /* ARM mode */
    biceq  r0, r0, #0xff000000            /* ARM mode */
    bl     d_ERROR_ExceptionSwintHandler  /* SWInterrupt: call C function here */
    ldmia  sp!,{r0-r3,r12,lr}             /* state restore from compiled code */
    movs   pc, lr                         /* adjust return */
#endif

DataAbortHandler:                         /* Data Abort handler */
    stmdb  sp!,{r0-r3,r12,lr}             /* state save from compiled code */
    ldr    r0, =DataAbortAddr
    sub    r1, lr, #8
    str    r1, [r0]                       /* Stores instruction causing data abort */
    bl     d_ERROR_ExceptionDataHandler   /* DataAbortInterrupt :call C function here */
    ldmia  sp!,{r0-r3,r12,lr}             /* state restore from compiled code */
    movs   pc, lr	                      /* adjust return */

PrefetchAbortHandler:                     /* Prefetch Abort handler */
    stmdb  sp!,{r0-r3,r12,lr}             /* state save from compiled code */
    ldr    r0, =PrefetchAbortAddr
    sub    r1, lr, #4
    str    r1, [r0]                       /* Stores instruction causing prefetch abort */
    bl     d_ERROR_ExceptionAddrHandler   /* PrefetchAbortInterrupt: call C function here */
    ldmia  sp!,{r0-r3,r12,lr}             /* state restore from compiled code */
    subs    pc, lr, #4                    /* adjust return */

#ifdef FREERTOS
ulICCPMRConst: .word ulICCPMR
pxCurrentTCBConst: .word pxCurrentTCB
ulCriticalNestingConst: .word ulCriticalNesting
ulPortTaskHasFPUContextConst: .word ulPortTaskHasFPUContext
ulMaxAPIPriorityMaskConst: .word ulMaxAPIPriorityMask
vTaskSwitchContextConst: .word vTaskSwitchContext
ulPortInterruptNestingConst: .word ulPortInterruptNesting
#endif

d_INT_IrqHandlerConst: .word d_INT_IrqHandler

.end
