#include "dos.h"


ERRB dos2_findfirst(const char *pathname, FFBLK *ffblk, uint8_t attrib) __naked __sdcccall(0)
{
	pathname, ffblk, attrib;
/*
     Parameters:    C = 40H (_FFIRST) 
                   DE = Drive/path/file ASCIIZ string or fileinfo block pointer
                   HL = filename ASCIIZ string (only if DE = fileinfo pointer)
                    B = Search attributes
                   IX = Pointer to new fileinfo block
     Results:       A = Error
                 (IX) = Filled in with matching entry

The "drive/path" portion of the string, or the fileinfo block, specifies a directory 
which is to be searched. A ".IATTR" error will be returned if a fileinfo block which 
specifies a file is passed. The "file" portion of the string, or the filename ASCIIZ 
string in HL, determines what filenames will be matched. If no match is found then a 
".NOFIL" error is returned, otherwise the fileinfo block pointed to by IX is filled 
in with the details of the matching entry.

The filename may contain ambiguous filename characters ("?" or "*") in which case the 
first matching entry will be returned. If the filename is null (either the ASCIIZ 
string pointed to by DE is null or ends in a "\" or the filename string pointed to by 
HL is null), then this function will behave exactly as if the filename was "*.*" so 
any name will match.

The attributes byte in register B specifies what type of entry will be matched. If it 
is zero then only non-hidden, non-system files will be found. If the directory, hidden 
or system bits in register B are set then entries with these attributes will be matched 
as well as ordinary files. The read only and archive bits of register B are ignored.

If the volume name bit of register B is set then the search is exclusive, only the 
volume label entry will be found. In this case also the fileinfo block and filename or 
the drive/path/file string are ignored apart from specifying the drive. This means that 
the volume name will always be found in the root directory if it exists whether or not 
it matches the filename given.

If DE points to a fileinfo block, then if desired, IX can point to the same fileinfo 
block. In this case when a match is found the new fileinfo block will overwrite the old 
one.
*/
	__asm
		push ix
		ld   ix,#4
		add  ix,sp

		ld e,0(ix)			; pathname
		ld d,1(ix)
		ld l,2(ix)			; ffblk
		ld h,3(ix)
		ld b,4(ix)			; attrib
		push hl
		pop  ix

		ld c,#FFIRST
		DOSCALL
		pop  ix

		ld l, a				; Returns L
		ret
	__endasm;
}