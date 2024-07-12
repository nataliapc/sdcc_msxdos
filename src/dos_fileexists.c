#include <stdbool.h>
#include "dos.h"


bool fileexists(char *filename)
{
	ERRB result = fopen(filename);
	if (result == 0) {
		fclose();
		return true;
	}
	return false;
}
