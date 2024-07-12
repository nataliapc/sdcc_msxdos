#include "dos.h"


FILEH dos2_fflush(FILEH fh) __naked __sdcccall(1)
{
/*
	ENSURE FILE HANDLE (46H)
	Parameters:    C = 46H (_ENSURE) 
	               B = File handle
	Results:       A = Error
	
	If the file associated with the file handle has been written to then its
	directory entry will be updated with a new date and time, the archive
	attributes bit will be set, and any buffered data will be flushed to disk.
	The file handle is not released and so it can still be used for accessing
	the file, and the current file pointer setting will not be altered.
*/
	__asm
		ld b,a			; B = Param fh

		ld c,#ENSURE
		DOSCALL

		or a			; Returns A
		ret nz			; error = A (error code)
		ld l,b
		ret				; no error = B (handle)
	__endasm;
}
