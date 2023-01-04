#include <stdbool.h>
#include "dos.h"


bool fileexists(char *filename)
{
	FILEH fh = fopen(filename, O_RDONLY);
	if (fh < 0xff00) {
		fclose(fh);
		return true;
	}
	return false;
}
