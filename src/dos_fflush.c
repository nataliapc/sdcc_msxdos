#include "dos.h"


inline ERRB fflush()
{
	return fclose();
}
