#include "dos.h"


ERRB dos2_move(char *filename, char *newpath) __naked __sdcccall(1)
{
	filename, newpath;
/*
	MOVE FILE OR SUBDIRECTORY (4FH)
	Parameters: C = 4FH (_MOVE)
	            DE = Drive/path/file ASCIIZ string or fileinfo block pointer
	            HL = New path ASCIIZ string
	Results:    A = Error

	Returns 0 on success or the MSX-DOS error code on failure. Wildcards are not
	allowed, so only one file or subdirectory can be moved.
*/
	__asm
		ex de,hl		; DE = filename (1st arg), HL = newpath (2nd arg)
		ld c,#MOVE
		DOSCALL
		ret				; Returns A
	__endasm;
}
