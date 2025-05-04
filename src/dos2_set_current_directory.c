#include "dos.h"


ERRB dos2_setCurrentDirectory(char *path) __naked __sdcccall(1)
{
	path;
/*
    CHANGE CURRENT DIRECTORY (5AH)
    Parameters:    C = 5AH (_CHDIR) 
                   DE = Drive/path/file ASCIIZ string
    Results:       A = Error

The drive/path/file string must specify a directory rather than a file. The 
current directory of the drive will be changed to be this directory. If the 
specified directory does not exist then the current setting will be unaltered 
and a ".NODIR" error returned.
*/
	__asm
		ex de,hl		; DE = Param path

		ld c, #CHDIR
		DOSJP			; Returns A
	__endasm;
}
