#include "dos.h"


ERRB fclose(void) __naked __z88dk_fastcall
{
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
	__asm
		ld de,#SYSFCB		; DE = Pointer to opened FCB
		ld c,#FCLOSE
		DOSCALL

		ret					; Returns L
	__endasm;
}
