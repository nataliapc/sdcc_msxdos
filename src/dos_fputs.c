#include <string.h>
#include "dos.h"


/**
 * Writes a string to the current output FCB file.
 *
 * @param str The null-terminated string to be written.
 * @return The number of characters written, or -1 if an error occurred.
 */
RETW fputs(char *str)
{
	uint16_t size = strlen(str);
	return fwrite(str, size);
}