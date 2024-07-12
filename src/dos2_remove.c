#include "dos.h"

ERRB dos2_remove(char *filename) __naked __sdcccall(1)
{
	filename;
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
		ex de,hl		; DE = Param filename

		ld c,#DELETE
		DOSCALL

		ret				; Returns A
	__endasm;
}