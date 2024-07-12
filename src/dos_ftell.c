#include "dos.h"


inline RETDW ftell(void)
{
	return fseek(0, SEEK_CUR);
}