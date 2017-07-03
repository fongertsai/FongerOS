#ifndef USER_H
#define USER_H


struct command
{
	const char *name;
	const char *describe;
	int (*func)();
};


void shell();
void print_help();
int command_cmp(char *src);
void print_help();
void print_tick();
void fork();

void ps();
void idle();
void idle2();
void idle3();










#endif
