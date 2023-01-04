#include "dos.h"


uint32_t filesize(char *filename)
{
	FILEH fh = fopen(filename, O_RDONLY);
	uint32_t size;
	char *error;
	if (fh & 0xff00) {
		size = fh;
	} else {
		size = fseek(fh, 0, SEEK_END);
		fclose(fh);
	}
	return size;
}
