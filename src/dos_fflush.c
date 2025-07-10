#include "dos.h"


inline bool fflush(void)
{
	return fclose();
}
