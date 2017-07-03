#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include "task.h"
#include <stddef.h>

/* System resources */
struct PCB tasks[TASK_LIMIT];
unsigned int stacks[TASK_LIMIT][STACK_SIZE];
volatile struct PCB *current_task;
volatile int current_task_pid;


unsigned int *task_init_stack(unsigned int *stack, void (*start)())
{
	stack += STACK_SIZE - (sizeof(struct SW_trapframe) + sizeof(struct HW_trapframe)) / sizeof(unsigned int);
	struct SW_trapframe *sw_stack = (struct SW_trapframe *)stack;
	struct HW_trapframe *hw_stack = (struct HW_trapframe *) (stack + sizeof(struct SW_trapframe) / sizeof(unsigned int));

	sw_stack -> r4 = (unsigned int)0x00000000;
	sw_stack -> r5 = (unsigned int)0x00000000;
	sw_stack -> r6 = (unsigned int)0x00000000;
	sw_stack -> r7 = (unsigned int)0x00000000;
	sw_stack -> r8 = (unsigned int)0x00000000;
	sw_stack -> r9 = (unsigned int)0x00000000;
	sw_stack -> r10 = (unsigned int)0x00000000;

	hw_stack -> ip = (unsigned int)start;
	hw_stack -> lr = (unsigned int)task_kill;
	hw_stack -> pc = (unsigned int)start;
	hw_stack -> xpsr = (unsigned int)0x21000000;

	return stack;
}

struct PCB* task_create(void *func)
{
	int pid = alloc_PCB();

	tasks[pid].stack = (void*)task_init_stack(stacks[pid], func);
	tasks[pid].pid = pid;
	tasks[pid].used = 1;
	tasks[pid].status = TASK_READY;


	return &tasks[pid];
}


int alloc_PCB()
{
	int i;
	for (i = 0; i < TASK_LIMIT; i++) {
		if (tasks[i].used == 0) {
			return i;
		}
		else {
			continue;
		}
	}
	return -1;
}


void init_system_PCB()
{
	int i;
	for (i = 0; i < TASK_LIMIT; i++) {
		tasks[i].used = 0;
		tasks[i].ticks = TIME_QUANTUM;
		tasks[i].status = TASK_EMPTY;
	}
}


int find_next_ready() 
{
	/*
	int i;
	int next;
	for (i = 0; i < TASK_LIMIT; i++) {
		next = (current_task_pid + i + 1) % TASK_LIMIT;
		// Task 0 is idle task, so it's priority is lowest
		if (next == 0)
			continue;
		if (tasks[next].used == 1 && tasks[next].status ==TASK_READY) {
			return next;
		}
	}
	return 0;
	*/
	// for performance //
	// don't sched to 0 //
	static int next = 1;
	int i;
	for ( ;next < TASK_LIMIT; next++) {
		if (tasks[next].used == 1 && tasks[next].status ==TASK_READY) {
			return next++;
		}
	}
	for (i = 1; i < TASK_LIMIT; i++) {
		if (tasks[i].used == 1 && tasks[i].status ==TASK_READY) {
			next = i + 1;
			return i;
		}
	}
	return 0;
	

}


void task_kill()
{
	current_task->used = 0;
	current_task->ticks = 0;
	current_task->status = TASK_EMPTY;
	set_pendsv();
	// Never return
	while(1);
}

