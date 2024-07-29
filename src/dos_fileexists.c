#include <stdbool.h>
#include "dos.h"


bool fileexists(char *filename)
{
	bool result = fopen(filename);
	dos_initializeFCB();
	return result;
}
