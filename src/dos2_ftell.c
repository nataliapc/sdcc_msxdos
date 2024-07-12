#include "dos.h"


inline RETDW dos2_ftell(FILEH fh)
{
	return dos2_fseek(fh, 0, SEEK_CUR);
}