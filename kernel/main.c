#include "stm32f10x.h"
#include "stm32_p103.h"
#include "stm32f10x_usart.h"
#include <stdlib.h>
#include <stdio.h>
#include "task.h"
#include "user.h"

extern struct PCB *current_task;
extern struct PCB tasks[];
extern int current_task_pid;




int main(void)
{
	uint32_t hclk_ticks_per_sec, ext_clock_ticks_per_sec;

	init_rs232();
	enable_rs232_interrupts();
	enable_rs232();
	rs232_print_str("Hello World ! This is FongerOS\n\r");
	rs232_print_str("Type \"help\" to display all commands \n\r");

	hclk_ticks_per_sec = SystemCoreClock;
	ext_clock_ticks_per_sec = hclk_ticks_per_sec / 8;
	if (SysTick_Config(ext_clock_ticks_per_sec))
	{
		while (1);
	}
	init_system_PCB();
	task_create(idle);
	task_create(idle2);
	//task_create(idle3);
	task_create(shell);

	current_task_pid = find_next_ready();
	current_task =  &tasks[current_task_pid];
	current_task->status = TASK_RUNNING;

	task_start();

	// Never come back
	while(1);
}
