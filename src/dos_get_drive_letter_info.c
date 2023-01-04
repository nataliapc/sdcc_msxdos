#include "dos.h"


/*
Get information about a drive letter (_GDLI, 79h)
Parameters:  C = 79H (_GDLI)
             A = physical drive (0=A:, 1=B:, etc)
             HL = Pointer to 64 byte data buffer

Results:     A  = Error code
             HL = Filled with data about the drive
*/
ERR8 get_drive_letter_info(char drive, DRIVE_info *info) __naked __sdcccall(0)
{
	drive, info;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld a,0(ix)    // drive
		ld l,1(ix)    // *info
		ld h,2(ix)
		pop ix

		ld c,#GDLI
		DOSCALL

		ld l, a
		ret
	__endasm;
}
