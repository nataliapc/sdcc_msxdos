#include "dos.h"


FILEH fcreate(char *filename, char mode, char attributes) __naked __sdcccall(0)
{
	filename;
	mode;
	attributes;

#ifdef MSXDOS1
/*
    CREATE FILE [FCB] (16H)
    Parameters:    C = 16H (_FMAKE)
                  DE = Pointer to unopened FCB
    Results:     L=A = 0FFH if unsuccessful
                     =   0  if successful   

This function creates a new file in the current directory of the specified
drive and opens it ready for reading and writing. The drive, filename and low
byte of the extent number must be set up in the FCB and the filename must not
be ambiguous. Checks will be done to ensure that invalid filenames are not
created.

If there is already a file of the required name then the action depends on the
value of the extent number byte. Normally this will be zero and in this case
the old file will be deleted and a new one created. However if the extent
number is non-zero then the existing file will be opened without creating a new
file. This ensures compatibility with early versions of CP/M where each extent
had to be explicitly created.

In all cases the resulting file will be opened with the required extent number
exactly as if an OPEN function call had been done.
*/
	__asm
		call    dos_initializeFCB

		push ix                     ; Put filename in DE
		ld   ix,#4
		add  ix,sp
		ld   e,0(ix)
		ld   d,1(ix)
		pop ix

		call dos_copyFilenameToFCB

		ld   de,#SYSFCB
		ld   c,#FMAKE               ; Call FOPEN Bios function
		DOSCALL

		ld h, #0
		or a
		ret z						; return no error
		dec h
		ret							; return error HL = $FFFF
	__endasm;
#endif

#ifdef MSXDOS2
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

		ld e,0(ix)
		ld d,1(ix)
		ld a,2(ix)
		ld b,3(ix)
		pop ix

		ld c, #CREATE
		DOSCALL

		ld  h, #0
		ld  l, b
		or  a
		ret z						; return no error
		dec h
		ld  l, a
		ret							; return error HL = $FFxx
	__endasm;
#endif

}
