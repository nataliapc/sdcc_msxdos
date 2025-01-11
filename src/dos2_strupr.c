#include "dos.h"


char* dos2_strupr(char *str)
{
	char *ptr = str;
	while (*ptr) {
		*ptr = dos2_toupper((unsigned char)*ptr);
		ptr++;
	}
	return str;
}