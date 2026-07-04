#include "dos.h"


ERRB dos2_rename(char *oldname, char *newname) __naked __sdcccall(1)
{
	oldname, newname;
/*
	RENAME FILE OR SUBDIRECTORY (4EH)
	Parameters: C = 4EH (_RENAME)
	            DE = Drive/path/file ASCIIZ string or fileinfo block pointer
	            HL = New filename ASCIIZ string
	Results:    A = Error

	Returns 0 on success or the MSX-DOS error code on failure. Wildcards are not
	allowed, so only one file or subdirectory can be renamed.
*/
	__asm
		ex de,hl		; DE = oldname (1st arg), HL = newname (2nd arg)
		ld c,#RENAME
		DOSCALL
		ret				; Returns A
	__endasm;
}
