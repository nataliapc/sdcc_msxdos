#include "dos.h"


RETDW dos2_fseek(char fh, uint32_t offset, char origin) __naked __sdcccall(0)
{
	fh;
	offset;
	origin;
/*
    MOVE FILE HANDLE POINTER (4AH)
    Parameters:    C = 4AH (_SEEK) 
                   B = File handle
                   A = Method code
               DE:HL = Signed offset
    Results:       A = Error
               DE:HL = New file pointer

The file pointer associated with the specified file handle will be altered
according to the method code and offset, and the new pointer value returned in
DE:HL.
The method code specifies where the signed offset is relative to as follows:
     A=0  Relative to the beginning of the file
     A=1  Relative to the current position
     A=2  Relative to the end of the file.
Note that an offset of zero with an method code of 1 will simply return the
current pointer value, and with a method code of 2 will return the size of the
file. No end of file check is done so it is quite possible (and sometimes
useful) to set the file pointer beyond the end of the file. If there are any
copies of this file handle created by the "duplicate file handle" function
(function 47h) or the "fork" function (function 60h) then their file pointer
will also be changed.

The file pointer only has any real meaning on disk files since random access is
possible. On device files the file pointer is updated appropriately when any
read or write is done, and can be examined or altered by this function. However
changing will have no effect and examining it is very unlikely to be useful.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld b,0(ix)		; B  = Param fh
		ld l,1(ix)		; DE:HL = Param offset
		ld h,2(ix)
		ld e,3(ix)
		ld d,4(ix)
		ld a,5(ix)		; A     = Param origin
		pop ix

		ld c,#SEEK
		DOSCALL

		or a			; Returns DE:HL
		ret z			; no error = new file pointer
		ld d,#0xff
		ld e,d
		ld h,e
		ld l,a
		ret				; error = $FFFF:FFxx
	__endasm;
}
