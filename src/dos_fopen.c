#include "dos.h"


bool fopen(char *filename) __naked __sdcccall(1)
{
	filename;
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
		push hl						; HL = Param filename

		call dos_initializeFCB

		pop de
		call dos_copyFilenameToFCB

		ld de,#SYSFCB
		ld c,#FOPEN					; Call FOPEN Bios function
		DOSCALL

		inc a						; error = 0 | no error = 1
		ret							; Returns A
	__endasm;
}
