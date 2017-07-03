#include "stm32f10x.h"
#include "stm32_p103.h"
#include "stm32f10x_usart.h"
#include <stdlib.h>
#include <stdio.h>
#include "task.h"

volatile int tick = 0;

extern volatile struct PCB *current_task;
extern struct PCB tasks[];
extern volatile int current_task_pid;


void scheduler()
{
	current_task->status = TASK_READY;
	current_task->ticks = TIME_QUANTUM;
	save_state();
	//rs232_print_str("SWITCH\n\r");
	current_task_pid = find_next_ready();
	current_task = &tasks[current_task_pid];
	current_task->status = TASK_RUNNING;
	context_switch();

}

void my_SysTick_Handler(void)
{
	tick += 1;
	current_task->ticks--;
	if (current_task->ticks == 0) {
		scheduler();
	}
}

void my_pendsv()
{
	/*
	current_task->status = TASK_READY;
	current_task->ticks = TIME_QUANTUM;
	save_state();
	current_task_pid = 3;
	current_task = &tasks[current_task_pid];
	current_task->status = TASK_RUNNING;
	context_switch();
	*/
	
	current_task->status = TASK_READY;
	current_task->ticks = TIME_QUANTUM;
	current_task_pid = 2;
	current_task = &tasks[current_task_pid];
	current_task->status = TASK_RUNNING;
}

