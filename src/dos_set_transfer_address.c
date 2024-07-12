#include "dos.h"


void setTransferAddress(void *memaddress) __naked __sdcccall(1)
{
	memaddress;
/*
    SET DISK TRANSFER ADDRESS (1AH)
    Parameters:    C = 1AH (_SETDTA)
                  DE = Required Disk Transfer Address
    Results:       None

This function simply records the address passed in DE as the disk transfer
address. This address will be used for all subsequent FCB read and write calls,
for "search for first" and "search for next" calls to store the directory
entry, and for absolute read and write calls. It is not used by the new
MSX-DOS read and write functions. The address is set back to 80h by a DISK
RESET call.
*/
	__asm
		ex de,hl		; DE = Param memaddress

		ld c,#SETDTA
		DOSJP
	__endasm;
}
