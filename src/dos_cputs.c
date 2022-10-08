#include "dos.h"


void cputs(char *str)
{
	while (*str) putchar(*str++);
	putchar('\n');
}
