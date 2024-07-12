#include "dos.h"


RETB getCurrentDrive(void) __naked __sdcccall(1)
{
/*
    GET CURRENT DRIVE (19H)
    Parameters:    C = 19H (_CURDRV)
    Results:     L=A = Current drive (0=A: etc)

    This function just returns the current drive number.
*/
	__asm
		ld c,#CURDRV
		DOSJP
	__endasm;
}
