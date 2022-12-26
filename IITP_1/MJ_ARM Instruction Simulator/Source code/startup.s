	.syntax unified
	.cpu cortex-m4
	.thumb

.global g_vectors
.global Reset_Handler

	.section	.isr_vector,"a",%progbits
	.type		g_vectors, %object
	.size		g_vectors, .-g_vectors


g_vectors:
	.word	_estack
	.word	Reset_Handler
	.word	0//NMI_Handler
	.word	0//HardFault_Handler
	.word	0//MemManage_Handler
	.word	0//BusFault_handler
	.word	0//UsageFault_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0//SVC_Handler
	.word	0//DebugMon_handler
	.word	0
	.word	0//PendSV_Handler
	.word	SysTick_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0


Reset_Handler:
	bl main

SysTick_Handler:

/* NMI_Handler:
 subroutine here
*/

