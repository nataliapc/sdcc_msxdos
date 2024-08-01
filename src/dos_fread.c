#include "dos.h"


RETW fread(char* buf, uint16_t size) __naked __sdcccall(1)
{
	buf, size;
/*
    RANDOM BLOCK READ [FCB] (27H)
    Parameters:    C = 27H (_RDBLK)
                  DE = Pointer to opened FCB
                  HL = Number of records to read
    Results:       A = 01H if error (usually caused by end-of-file)
                     =  0  if no error
                  HL = Number of records actually read

This function is the complement of the BLOCK WRITE and most of the same
comments apply as regards its use. Again if large blocks are read then it will
be much faster than the normal CP/M operation.

For example if it is desired to read 20k from a file, it is better to read the
20k with one function call rather than 20 separate function calls of 1k each.
However it makes no difference whether the 20k read is done with a record size
of 1 and a record count of 20k, with a record size of 20k and a record count of
1, or any intermediate combination.

The number of records actually read is returned in HL. This may be smaller than
the number of records requested if the end of the file was encountered. In this
case any partial record will be padded out with zeroes before being copied to
the users DTA. The random record field is adjusted to the first record not
read, ie. the value returned in HL is added on to it.
*/
	__asm
		push ix
		ex de,hl			; DE = Param buf
		push hl				; HL = Param size

		ld hl,#1			; Set FCB Record size to 1 byte
		ld (#SYSFCB+14),hl
							; DE = Required Disk Transfer Address
		ld c,#SETDTA		; Set Disk transfer address (DTA)
		DOSCALL

		pop hl				; HL = Number of records to read
		ld de,#SYSFCB		; DE = Pointer to opened FCB
		ld c,#RDBLK
		DOSCALL

		pop ix
		ex de,hl			; Returns DE
		or a
		ret z				; no error = number of bytes read
		ld de, #0
		ret					; error = $0 [ERR_EOF]
	__endasm;
}
