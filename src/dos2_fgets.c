#include "dos.h"


char* dos2_fgets(char *str, uint16_t size, FILEH fh)
{
	char *p = str;
	uint16_t ret;

	while (--size) {
		ret = dos2_fread(p, 1, fh);
		if (ret == (0xff00|ERR_EOF) && str != p) break;	// ERR_EOF with content
		if (*p++ == '\n') break;
		if (ret & 0xff00) return 0;
	}
	*p = '\0';
	return str;
}
