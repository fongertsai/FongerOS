#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include "task.h"
#include "keyboard.h"
#include "user.h"
#include "string.h"

extern struct PCB tasks[];
extern volatile int tick;

extern char command[];
extern volatile int command_in;

struct command commands[] = 
{
	{ "help", "Display this list of commands", print_help },
	{ "tick", "Display system tick", print_tick },
	{ "ps", "PCB status in system", ps },
	{ "fork", "fork", fork }
};

int NCOMMANDS = (sizeof(commands)/sizeof(struct command));


void shell()
{
        rs232_print_str("Shell: ");
        while (1) {
                if (command_in) {
                        rs232_print_str("\n\r");
                        command_in = 0;
                        //rs232_print_str(command);
			int k = command_cmp(command);
			if (k != -1) {
				commands[k].func();
			}
			
			else {
                        	rs232_print_str(command);
				rs232_print_str("\t");
				rs232_print_str(" :command not found");
			}
			
                        rs232_print_str("\n\r");
                        rs232_print_str("Shell: ");
                        //ps();
                }
        }
}

int command_cmp(char *src)
{
	int i;
	for (i = 0; i < NCOMMANDS; i++) {
		if (strcmp(src, commands[i].name)) {
			return i;
		}
	}
	return -1;
}

void print_help()
{
	int i;
	for (i = 0; i < NCOMMANDS; i++) {
		rs232_print_str(commands[i].name);
		rs232_print_str("\t\t  :");
		rs232_print_str(commands[i].describe);
                rs232_print_str("\n\r");
	}
}

void ps()
{
    int i;
    char tmp[5];
    for (i = 0; i < TASK_LIMIT; i++) {
	int2char(tmp, i);
	rs232_print_str(tmp);
	rs232_print_str("\t");
        if (tasks[i].used == 1) {
            rs232_print_str("USED");
            rs232_print_str("\n\r");
        } 
	else {
            rs232_print_str("EMP");
            rs232_print_str("\n\r");
	} 

    }   
}

void print_tick()
{
	rs232_print_str("Now tick: ");
	char tmp[20];
	int2char(tmp, tick);
	rs232_print_str(tmp);
        rs232_print_str("\n\r");
}

void fork()
{
	task_create(idle3);
}


void idle()
{
        static int test = 0;
        while(1);
}

void idle2()
{
        rs232_print_str("HI I'm idle 2\r\n");

}
void idle3()
{
        static int test = 0;
        while(1) {
                test += 1;
                if (test %50000000 ==0) {
                        rs232_print_str("\n\rHI I'm idle 3\r\n");
                }

        }
}

