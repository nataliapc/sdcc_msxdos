#include "dos.h"


char* fgets(char *str, uint16_t size)
{
	char *p = str;
	uint16_t ret;

	while (--size) {
		ret = fread(p, 1);
		if (*p++ == '\n') break;
		if (ret == 0xffc7) break;	// ERR_EOF
		if (ret & 0xff00) return NULL;
	}
	*p = '\0';
	return str;
}
