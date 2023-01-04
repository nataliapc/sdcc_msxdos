#include "dos.h"


char* fgets(char *str, uint16_t size, uint16_t fh)
{
	char *p = str;
	uint16_t ret;

	while (--size) {
		ret = fread(p++, 1, fh);
		if (ret == 0xffc7) break;	// ERR_EOF
		if (ret & 0xff00) return 0;
	}
	*p = '\0';
	return str;
}
