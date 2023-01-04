#include "dos.h"

ERR8 remove(char *filename) __naked __sdcccall(0)
{
	filename;

#ifdef MSXDOS1
/*
    DELETE FILE [FCB] (13H)
    Parameters:    C = 13H (_FDEL)
                  DE = Pointer to unopened FCB
    Results:     L=A = 0FFH if no files deleted
                     =   0  if files deleted OK

All files in the current directory of the disk specified by the FCB, and which
match the ambiguous filename in the FCB, are deleted. Sub-directories, system
files, hidden files and read only files are not deleted. If any files at all
are successfully deleted then this function returns with A=0. A return with
A=FFh indicates that no files were deleted.
*/
	__asm
		call    dos_initializeFCB

		push ix
		ld ix,#4
		add ix,sp

		ld e,0(ix)
		ld d,1(ix)
		pop ix

		call dos_copyFilenameToFCB

		ld de,#SYSFCB
		ld c,#FDEL
		DOSCALL

		ret
	__endasm;
#endif

#ifdef MSXDOS2
/*
    DELETE FILE OR SUBDIRECTORY (4DH)
    Parameters:    C = 4DH (_DELETE) 
                  DE = Drive/path/file ASCIIZ string
                       or fileinfo block pointer
    Results:       A = Error

This function deletes the object (file or sub-directory) specified by the
drive/path/file string or the fileinfo block. Global filename characters are
not allowed so only one file or sub-directory can be deleted with this
function. A sub-directory can only be deleted if it is empty or an error
(".DIRNE") occurs if not). The "." and ".." entries in a sub-directory cannot
be deleted (".DOT" error) and neither can the root directory. A file cannot be
deleted if there is a file handle open to it (.FOPEN error) or if it is read
only (.FILRO error).

If it is a file then any disk space which was allocated to it will be freed. If
the disk is an MSX-DOS 2 disk then enough information is retained on the disk
to allow the "UNDEL" utility program do undelete the file. This information is
only retained until the next disk space allocation (usually a write to a file)
is done on this disk. After making this function call, if a fileinfo block was
passed then it must not be used again (other than passing it to a "find next
entry" function) since the file to which it refers no longer exists.

If a device name such as "CON" is specified then no error will be returned but
the device will not actually be deleted.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp

		ld e,0(ix)
		ld d,1(ix)
		pop ix

		ld c,#DELETE
		DOSCALL

		ld l, a
		ret
	__endasm;
#endif
}