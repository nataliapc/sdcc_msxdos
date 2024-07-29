#include <stdbool.h>
#include "dos.h"


RETDW filesize(char *filename)
{
	RETDW size = -1;
	if (fopen(filename)) {
		size = ((FCB*)SYSFCB)->fileSize;
	}
	dos_initializeFCB();
	return size;
}
