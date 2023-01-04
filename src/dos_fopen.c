#include "dos.h"


FILEH fopen(char *filename, char mode) __naked __sdcccall(0)
{
	filename, mode;

#ifdef MSXDOS1
/*
    OPEN FILE [FCB] (0FH)
    Parameters:    C = 0FH (_FOPEN)
                  DE = Pointer to unopened FCB 
    Results:     L=A = 0FFH if file not found
                     =   0  if file is found

The unopened FCB must contain a drive which may be zero to indicate the current
drive and a filename and extension which may be ambiguous. The current
directory of the specified drive will be searched for a matching file and if
found it will be opened. Matching entries which are sub-directories or system
files will be ignored, and if the filename is ambiguous then the first suitable
matching entry will be opened.

Device names may be put in the FCB (without a colon) to allow devices to be
accessed as if they were actually disk files. The standard device names are
defined in the "Program Interface Specification".

The low byte of the extent number is not altered by this function, and a file
will only be opened if it is big enough to contain the specified extent.
Normally the transient program will set the extent number to zero before
calling this function. The high byte of the extent number will be set to zero
to ensure compatibility with CP/M.

The filename and extension in the FCB will be replaced by the actual name of
the file opened from the directory entry. This will normally be the same as
what was there before but may be different if an ambiguous filename or one
with lower case letters in was used.

The record count will be set to the number of 128 byte records in the specified
extent, which is calculated from the file size. The file size field itself, the
volume-id and the 8 reserved bytes will also be set up. The current record and
random record fields will not be altered by this function, it is the
application program's responsibility to initialize them before using the read
or write functions.

If the file cannot be found, then the "APPEND" environment item will be
examined. If this is set then it is interpreted as a drive/path string which
specifies a second directory in which to look for the file. The specified
directory will be searched for the file and if found it will be opened as
above. In this case the drive byte of the FCB will be set to the drive on
which the file was found to ensure correct accessing of the file if the
original drive byte was zero (default).
*/
	__asm
		call    dos_initializeFCB

		push    ix                    ; Put filename in DE
		ld      ix,#4
		add     ix,sp
		ld      e,0(ix)
		ld      d,1(ix)
		pop ix

		call    dos_copyFilenameToFCB

		ld      de,#SYSFCB
		ld      c,#FOPEN              ; Call FOPEN Bios function
		DOSCALL

		ld h, #0
		or a                          ; Check result
		ret z
		ld hl, #0xffd7                ; Set return value ERR
		ret
	__endasm;
#endif

#ifdef MSXDOS2
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

		ld e,0(ix)
		ld d,1(ix)
		ld a,2(ix)
		pop ix

		ld c, #OPEN
		DOSCALL

		ld h, #0
		ld l, b
		or a
		ret z
		dec h
		ld l, a
		ret
	__endasm;
#endif

}
