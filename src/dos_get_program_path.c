#include <string.h>
#include "dos.h"


char* getProgramPath(char *path)
{
	switch (dosVersion()) {
		case VER_MSXDOS1x:
			strcpy(path, "A:\\");
			path[0] = getCurrentDrive() + 'A';
			break;
		case VER_MSXDOS2x:
		case VER_NextorDOS:
			if (dos2_getEnv("PROGRAM", path, MAX_PATH_SIZE)) {
				return NULL;
			}
			break;
		default:
			return NULL;
	}
	return path;
}
