#include "dos.h"


bool fcreate(char *filename) __naked __sdcccall(1)
{
	filename;
/*
    CREATE FILE [FCB] (16H)
    Parameters:    C = 16H (_FMAKE)
                  DE = Pointer to unopened FCB
    Results:     L=A = 0FFH if unsuccessful
                     =   0  if successful   

This function creates a new file in the current directory of the specified
drive and opens it ready for reading and writing. The drive, filename and low
byte of the extent number must be set up in the FCB and the filename must not
be ambiguous. Checks will be done to ensure that invalid filenames are not
created.

If there is already a file of the required name then the action depends on the
value of the extent number byte. Normally this will be zero and in this case
the old file will be deleted and a new one created. However if the extent
number is non-zero then the existing file will be opened without creating a new
file. This ensures compatibility with early versions of CP/M where each extent
had to be explicitly created.

In all cases the resulting file will be opened with the required extent number
exactly as if an OPEN function call had been done.
*/
	__asm
		push hl						; HL = Param filename
		call dos_initializeFCB
		ld   hl,#SYSFCB+12			; Extent set to 1 for existing file error
		ld   (hl),#0xff
				
		pop de						; Put filename pointer in DE
		call dos_copyFilenameToFCB

		ld   de,#SYSFCB				; DE = Pointer to unopened FCB
		ld   c,#FMAKE				; Call FMAKE Bios function
		DOSCALL

		inc a						; error = 0 | no error = 1
		ret							; Returns A
	__endasm;
}
