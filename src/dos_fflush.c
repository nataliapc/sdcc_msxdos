#include "dos.h"


inline bool fflush()
{
	return fclose();
}
