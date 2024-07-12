#include <string.h>
#include "dos.h"

RETW fputs(char *str)
{
	uint16_t size = strlen(str);
	return fwrite(str, size);
}