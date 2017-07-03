	.syntax unified

	.global   SysTick_Handler
	.type   SysTick_Handler, %function
SysTick_Handler:
	push {lr}
	bl my_SysTick_Handler
	pop {lr}
	bx lr


	.global   PendSV_Handler
	.type   PendSV_Handler, %function
PendSV_Handler:
	cpsid i
	
	mrs r0, psp
	ldr r3, =current_task
	ldr r2, [r3]
	ldr r1, [r2]
	stmdb r0!, {r4-r11}
	str r0, [r2]
	
	stmdb sp!, {r3, lr}
	bl my_pendsv
	ldmia sp!, {r3, lr}

	ldr r1, [r3]
	ldr r0, [r1]
 	ldmia r0!, {r4-r11}
 	#ldmia r0!, {r7}
	msr psp, r0


	cpsie i
	bx lr


	.global   USART2_IRQHandler
	.type	USART2_IRQHandler, %function
USART2_IRQHandler:
	push {lr}
	bl key_board_handler
	nop
	pop {lr}
	bx lr


	.type task_start, %function
	.global task_start
task_start:
	ldr r3, =current_task
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, {r4-r11}

	msr psp, r0
	mov r0, #3
	msr control, r0
	isb
	mov lr, #0xfffffffd
	bx lr



	.type save_state, %function
	.global save_state
save_state:
	cpsid i
	mrs r0, psp
	ldr r3, =current_task
	ldr r2, [r3]
	ldr r1, [r2]
	stmdb r0!, {r4-r11}
	str r0, [r2]
	bx lr


	.type context_switch, %function
	.global context_switch
context_switch:
	ldr r3, =current_task
	ldr r1, [r3]
	ldr r0, [r1]
	ldmia r0!, {r4-r11}
	msr psp, r0

	cpsie i
	bx lr




	.type set_pendsv, %function
	.global set_pendsv
set_pendsv:
	ldr r4, =0xe000ed04
	mov r5, #0x12000000
	str r5, [r4]

	bx lr

