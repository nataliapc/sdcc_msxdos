#include "dos.h"


inline uint32_t ftell(FILEH fh)
{
	return fseek(fh, 0, SEEK_CUR);
}