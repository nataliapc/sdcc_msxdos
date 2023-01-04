#include <string.h>
#include "dos.h"


char* get_program_path(char *path)
{
#ifdef MSXDOS2
	if (get_env("PROGRAM", path, MAX_PATH)) {
		return NULL;
	}
#endif
#ifdef MSXDOS1
	strcpy(path, "A:\\");
	path[0] = get_current_drive() + 'A';
#endif
	return path;
}
