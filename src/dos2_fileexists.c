#include <stdbool.h>
#include "dos.h"


bool dos2_fileexists(char *filename)
{
	FILEH fh = dos2_fopen(filename, O_RDONLY);
	if (fh < ERR_FIRST) {
		dos2_fclose(fh);
		return true;
	}
	return false;
}
