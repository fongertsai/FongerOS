#ifndef TASK_H
#define TASK_H

#define TASK_LIMIT 11
#define STACK_SIZE 384
#define TIME_QUANTUM 20


#define TASK_READY      0
#define TASK_RUNNING      1
#define TASK_EMPTY      2


struct SW_trapframe{
		unsigned int r4;
		unsigned int r5;
		unsigned int r6;
		unsigned int r7;
		unsigned int r8;
		unsigned int r9;
		unsigned int r10;
		unsigned int r12;
};

struct HW_trapframe{
		unsigned int r0;
		unsigned int r1;
		unsigned int r2;
		unsigned int r3;
		unsigned int ip;
		unsigned int lr;	/* Back to user thread code */
		unsigned int pc;
		unsigned int xpsr;
};

/* Task Control Block */
struct PCB
{
		unsigned int *stack;

		int pid;
		int status;
		int ticks;
		int used;  // 0:no, 1:yes

};

unsigned int *task_init_stack(unsigned int *stack, void (*start)());
struct PCB* task_create(void *func);
void init_system_PCB();
int alloc_PCB();
int find_next_ready();
void task_kill();
#endif
