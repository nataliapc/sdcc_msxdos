#include "dos.h"


ERRB dos2_fclose(char fh) __naked __sdcccall(1)
{
    fh;
/*
    CLOSE FILE HANDLE (45H)
    Parameters:    C = 45H (_CLOSE) 
                   B = File handle
    Results:       A = Error

    This function releases the specified file handle for re-use. If the
    associated file has been written to then its directory entry will be
    updated with a new date and time, the archive attributes bit will be set,
    and any buffered data will be flushed to disk. Any subsequent attempt to
    use this file handle will return an error. If there are any other copies of
    this file handle, created by "duplicate file handle" or "fork", then these
    other copies may still be used.
*/
	__asm
		ld b,a			; B = Param fh

		ld c,#CLOSE
		DOSJP			; Return A
	__endasm;
}
