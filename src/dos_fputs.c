#include <string.h>
#include "dos.h"

RE16 fputs(char *str, char fh)
{
	uint16_t size = strlen(str);
	return fwrite(str, size, fh);
}