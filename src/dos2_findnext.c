#include "dos.h"

ERRB dos2_findnext(FFBLK *ffblk ) __naked __sdcccall(1)
{
	ffblk;
/*
    FIND NEXT ENTRY (41H)
    Parameters:    C = 41H (_FNEXT) 
                  IX = Pointer to fileinfo block from previous find first function.

    Results:       A = Error. If there are no more matching entries then a NOFIL error is returned
                (IX) = Filled in with next matching entry

This function should only be used after a "find first entry" function call.
It searches the directory for the next match to the (presumably ambiguous) 
filename which was given to the "find first entry" function call. If there 
are no more matching entries then a ".NOFIL" error is returned, otherwise 
the fileinfo block is filled in with the information about the new matching 
entry.
*/
	__asm
		push ix			; preserve callers IX (SDCC uses it as frame pointer)
		push hl
		pop ix			; IX = Param ffblk

		ld c,#FNEXT
		DOSCALL			; must CALL (not JP): we need to return to restore IX
		pop ix			; restore callers IX
		ret				; A = error code (sdcccall(1) returns in A; pop/ret keep A)
	__endasm;
}
