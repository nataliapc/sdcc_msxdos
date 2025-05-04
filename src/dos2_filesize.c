#include <stdbool.h>
#include "dos.h"


RETDW dos2_filesize(char *filename)
{
	FILEH fh;
	RETDW size = 0;

	fh = dos2_fopen(filename, O_RDONLY);
	if (fh < ERR_FIRST) {
		// Seek to end of file to get size
		size = dos2_fseek(fh, 0, SEEK_END);
		dos2_fclose(fh);
	}
	return size;
}
