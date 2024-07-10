#include "dos.h"


FILEH fclose(char fh) __naked __sdcccall(0)
{
    fh;
/*
    CLOSE FILE [FCB] (10H)
    Parameters:    C = 10H (_FCLOSE)
                  DE = Pointer to opened FCB
    Results:     L=A = 0FFH if not successful
                     =   0  if successful

    The FCB must have previously been opened with either an OPEN or a CREATE
    function call. If the file has only been read then this function does
    nothing. If the file has been written to then any buffered data will be
    written to disk and the directory entry updated appropriately. The file may
    still be accessed after a close, so the function can be regarded as doing
    an "ensure" function.
*/
#ifdef MSXDOS1
    __asm
        ld de,#SYSFCB
        ld c,#FCLOSE
        DOSCALL

        ld h, a
        ret
    __endasm;
#endif

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
#ifdef MSXDOS2
    __asm
        push ix
        ld ix,#4
        add ix,sp

        ld b,(ix)   // File handle
        pop ix

        ld c,#CLOSE
        DOSCALL

        ld h, #0
        ld l, a
        or a
        ret z
        dec h
        ret
    __endasm;
#endif
}
