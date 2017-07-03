#include "stm32f10x.h"
#include "stm32_p103.h"
#include "stm32f10x_usart.h"
#include <stdlib.h>
#include <stdio.h>
#include "task.h"
#include "keyboard.h"

#define KB_BUFFER_SIZE 64


char in_char;
int start = 0;
int end = 0;
int overflow = 0;
char key_buffer[KB_BUFFER_SIZE];
char command[KB_BUFFER_SIZE];
volatile int command_in  = 0;

volatile int perfer_shell = 0;

void key_board_handler(void)
{
	//Receive Data register not empty interrupt 
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		in_char = USART_ReceiveData(USART2);

		key_buffer[end] = in_char;
		end += 1;
		if ( end == KB_BUFFER_SIZE ) {
			overflow = 1;
			end = end % KB_BUFFER_SIZE;
		}

		if (in_char == '\r') {
			commit();
			start = end;
			overflow = 0;
			set_pendsv();
		}
		USART_SendData(USART2, in_char);
	}
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) {
		USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	}
}

void commit()
{
	int i;
	int j = 0;
	if (overflow) {
		for (i = start; i < KB_BUFFER_SIZE; i++) {
			command[j] = key_buffer[i];
			j++;
		}
		for (i = 0; i < end; i++) {
			command[j] = key_buffer[i];
			j++;
		}
	}
	else {
		for (i = start; i < end; i++) {
			command[j] = key_buffer[i];
			j++;
		}
	}
	command[j] = '\0';
	command_in = 1;

}
