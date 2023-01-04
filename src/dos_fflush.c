#include "dos.h"


#ifdef MSXDOS1
inline FILEH fflush(char fh)
{
	return fclose(fh);
}
#endif

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
#ifdef MSXDOS2
FILEH fflush(char *fh) __naked __sdcccall(0)
{
	__asm
		push ix
		ld ix,#4
		add ix,sp

		ld b,(ix)   // File handle
		pop ix

		ld c,#ENSURE
		DOSCALL

        ld h, #0
        ld l, a
        or a
        ret z
        dec h
		ret
	__endasm
}
#endif
