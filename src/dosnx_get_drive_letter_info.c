#include "dos.h"


ERRB nxtr_getDriveLetterInfo(char drive, DRIVE_info *info) __naked __sdcccall(1)
{
	drive, info;
/*
    Get information about a drive letter (_GDLI, 79h)
    Parameters:  C = 79H (_GDLI)
                 A = physical drive (0=A:, 1=B:, etc)
                 HL = Pointer to 64 byte data buffer

    Results:     A  = Error code
                 HL = Filled with data about the drive
*/
	__asm
						; A = Param drive
		ex de,hl		; HL = Param info

		ld c,#GDLI
		DOSJP			; Returns A
	__endasm;
}
