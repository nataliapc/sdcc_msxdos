#include "dos.h"


FILEH dos2_fcreate(char *filename, char mode, char attributes) __naked __sdcccall(0)
{
	filename, mode, attributes;
/*
    CREATE FILE HANDLE (44H)
    Parameters:    C = 44H (_CREATE) 
                  DE = Drive/path/file ASCIIZ string
                   A = Open mode. b0 set => no write
                                  b1 set => no read
                                  b2 set => inheritable   
                                  b3..b7   -  must be clear
                   B = b0..b6 = Required attributes
                           b7 = Create new flag
    Results:       A = Error [IATTR, FILEX, DIRX, SYSX, FILRO, FOPEN, DRFUL, DKFUL]
                   B = New file handle

A file or sub-directory, as specified by the attributes in register B, will be
created with the name and in the directory specified by the drive/path/file
string. A ".IATTR" error is returned if register B specifies a volume name.

An error will be returned if the file or sub-directory cannot be created. The
error conditions in this case are the same as for the "find new entry" function
(function 42h) with the main error codes being ".FILEX", ".DIRX", ".SYSX",
".FILRO", ".FOPEN", ".DRFUL" and ".DKFUL". Like the "find new" function, if the
"create new" flag (bit-7 of register B) is set then an existing file will not
be deleted and will always return a ".FILEX" error.

If the attributes byte specifies a sub-directory then the hidden bit may also
be set to create a hidden sub-directory. For a file, the hidden, system or read
only bits may be set to create a file with the appropriate attributes. An
invalid attributes bits will simply be ignored. A file will always be created
with the archive attribute bit set.

A file will automatically be opened just as for the "open" function described
above, and a file handle returned in register B. The "open mode" parameter is
interpreted in the same way as for the "open" function. A sub-directory will
not be opened (because this is meaningless) so register B will be returned as
0FFh which can never be a valid file handle.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld e,0(ix)			; DE = Param filename
		ld d,1(ix)
		ld a,2(ix)			; A  = Param mode
		ld b,3(ix)			; B  = Param attributes
		pop ix

		set 7,b				; Set 'create new' flag. See up comment
		ld c, #CREATE
		DOSCALL

		ld l,b				; Returns L
		or a
		ret z				; no error = B (handle)
		ld l,a
		ret					; error = A (error code)
	__endasm;
}
