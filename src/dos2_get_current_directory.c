#include "dos.h"


ERRB dos2_getCurrentDirectory(char drive, char *path) __naked __sdcccall(1)
{
	drive, path;
/*
    GET CURRENT DIRECTORY (59H)
    Parameters:    C = 59H (_GETCD) 
                   B = Drive number (0=current, 1=A: etc)
                  DE = Pointer to 64 byte buffer 

    Results:       A = Error
                  DE = Filled in with current path

This function simply gets an ASCIIZ string representing the current directory 
of the specified drive into the buffer pointed to by DE. The string will not 
include a drive name or a leading or trailing "\" character, so the root 
directory is represented by a null string. The drive will be accessed to make 
sure that the current directory actually exists on the current disk, and if 
not then the current directory will be set back to the root and a null string 
returned.
*/
	__asm
		ld b,a			; B  = Param drive
						; DE = Param path

		ld c, #GETCD
		DOSJP			; Returns A
	__endasm;
}
