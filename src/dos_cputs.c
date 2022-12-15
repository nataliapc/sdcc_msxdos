#include "dos.h"

#ifndef DISABLE_CONIO

void cputs(char *str)
{
	while (*str) putchar(*str++);
	putchar('\n');
}

#endif
