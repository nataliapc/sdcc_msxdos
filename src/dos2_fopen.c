#include "dos.h"


FILEH dos2_fopen(char *filename, char mode) __naked __sdcccall(0)
{
	filename, mode;
/*
    OPEN FILE HANDLE (43H)
    Parameters:    C = 43H (_OPEN) 
                  DE = Drive/path/file ASCIIZ string
                       or fileinfo block pointer
                   A = Open mode. b0 set => no write
                                  b1 set => no read
                                  b2 set => inheritable   
                                  b3..b7 -  must be clear
    Results:       A = Error [NOFIL, IATTR, DIRX, NHAND, NORAM, ACCV, FILRO]
                   B = New file handle

The drive/path/file string or the fileinfo block should normally refer to a
file rather than a sub-directory or volume name. If it is a volume name then a
".IATTR" error will be returned. If it is a sub-directory then ".DIRX" error
will be returned.

Assuming that a file is specified then it will be opened ready for reading
and/or writing (depending on the open mode in A) and a new file handle for it
will be returned in register B. The lowest available file handle number will be
used and an error will result if there are no spare file handles (".NHAND"
error), or insufficient memory (".NORAM" error).

If the "no read" bit of register A is set then reads from the file handle will
be rejected and if the "no write" bit is set then writes will be rejected, in
both cases with an ".ACCV" error. Writes will also be rejected if the file is
read only (".FILRO" error). If the "inheritable" bit of register A is set then
the file handle will be inherited by a new process created by the "fork"
function call (see function 60h).

If a device file handle is opened by giving a filename which matches one of the
built in devices (for example "CON" or "NUL"), then it will always be opened
initially in ASCII mode. The IOCTL function (function 4Bh) can be used to
change this to binary mode but great care must be taken in reading from devices
in binary mode because there is no end of file condition.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld e,0(ix)			; DE = Param filename
		ld d,1(ix)
		ld a,2(ix)			; A  = Param mode
		pop ix

		ld c, #OPEN
		DOSCALL

		ld l,b				; Returns L
		or a
		ret z				; no error = B (handle)
		ld l,a
		ret					; error = A (error code)
	__endasm;
}
