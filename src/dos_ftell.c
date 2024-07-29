#include "dos.h"


inline RETDW ftell(void)
{
	return ((FCB*)SYSFCB)->rndRecord;
}