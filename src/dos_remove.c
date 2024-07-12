#include "dos.h"

ERRB remove(char *filename) __naked __sdcccall(1)
{
	filename;
/*
    DELETE FILE [FCB] (13H)
    Parameters:    C = 13H (_FDEL)
                  DE = Pointer to unopened FCB
    Results:     L=A = 0FFH if no files deleted
                     =   0  if files deleted OK

All files in the current directory of the disk specified by the FCB, and which
match the ambiguous filename in the FCB, are deleted. Sub-directories, system
files, hidden files and read only files are not deleted. If any files at all
are successfully deleted then this function returns with A=0. A return with
A=FFh indicates that no files were deleted.
*/
	__asm
		push hl			; HL = Param filename

		call dos_initializeFCB

		pop de			; DE = filename
		call dos_copyFilenameToFCB

		ld de,#SYSFCB
		ld c,#FDEL
		DOSCALL

		inc a			; error = 0 | no error = 1
		ret				; Returns A
	__endasm;
}