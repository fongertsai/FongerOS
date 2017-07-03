#include "string.h"

int strcmp(char *a, char *b)
{
	int i;
	if (*a == '\r')
		return 0;
	while(a) {
		if(*a == '\r')
			break;
		if (*a != *b)
			return 0;
		a++;
		b++;
	}
	return 1;
}

char *int2char(char *a, int b)
{
	if (b / 10 == 0) {
		*a++ = b + '0';
		*a = '\0';
		return a;
	}
	a = int2char(a, b/10);
	*a++ = b % 10 + '0';
	*a = '\0';
	return a;
}

