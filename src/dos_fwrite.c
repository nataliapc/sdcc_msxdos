#include "dos.h"


RETW fwrite(char* buf, uint16_t size) __naked __sdcccall(1)
{
	buf, size;
/*
    RANDOM BLOCK WRITE [FCB] (26H)
    Parameters:    C = 26H (_WRBLK)
                  DE = Pointer to opened FCB
                  HL = Number of records to write
    Results:       A = 01H if error 
                     =  0  if no error

Data is written from the current disk transfer address to the position in the
file defined by the random record number. The record size is determined by the
record size field in the FCB (bytes 0Eh and 0Fh) which must be set by the user
after opening the file and before calling this function. If the record size is
less than 64 bytes then all four bytes of the random record number are used,
otherwise only the first three are used.

The number of records to be written is specified by HL, and together with the
record size this determines the amount of data to be written. An error will be
returned if the size exceeds 64k, thus limiting the maximum size of a transfer.

After writing the data, the random record field is adjusted to the next record
number in the file (ie. HL is added on to it). The current record and extent
fields are not used or altered. The file size field is updated if the file has
been extended.

The record size can be any value from 1...0FFFFh. Small record sizes are no
less efficient that large record sizes so if desired the record size can be set
to one and the record count then becomes a byte count. It is desirable to write
as much as possible with one function call since one large transfer will be
quicker than several small ones.

If the number of records to write (HL) is zero then no data will be written,
but the size of the file will be altered to the value specified by the random
record field (FCB). This may be either longer or shorter than the file's current 
size and disk space will be allocated or freed as required. Additional disk 
space allocated in this way will not be initialized to any particular value.
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

		pop hl				; HL = Number of records to write
		push hl
		ld de,#SYSFCB		; DE = Pointer to opened FCB
		ld c,#WRBLK
		DOSCALL

		pop de				; Returns DE
		pop ix
		or a
		ret z				; no error = number of bytes writed
		ld de, #0
		ret					; error = $0
	__endasm;
}
