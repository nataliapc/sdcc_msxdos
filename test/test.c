#include "msx_const.h"
#include "dos.h"
#include "heap.h"
#include <stdio.h>


int main(char **argv, int argc)
{
	printf("DOS Version: %u\n\r", dosver());

	return 0;
}
