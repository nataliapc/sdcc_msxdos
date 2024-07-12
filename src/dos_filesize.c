#include "dos.h"


RETDW filesize(char *filename)
{
	ERRB result = fopen(filename);
	int32_t size;
	char *error;
	if (result != 0) {
		size = -1;
	} else {
		size = fseek(0, SEEK_END);
		fclose();
	}
	return size;
}
