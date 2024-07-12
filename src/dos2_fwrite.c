#include "dos.h"


RETW dos2_fwrite(char* buf, uint16_t size, char fh) __naked __sdcccall(0)
{
	buf, size, fh;
/*
    WRITE TO FILE HANDLE (49H)
    Parameters:    C = 49H (_WRITE) 
                   B = File handle
                  DE = Buffer address
                  HL = Number of bytes to write
    Results:       A = Error
                  HL = Number of bytes actually written

This function is very similar to the "read" function above (function 48h). The
number of bytes specified will be written to the current file pointer position
in the file, and the file pointer will be adjusted to point to just after the
last byte written. If the file was opened with the "no write" access bit set
then a ".ACCV" error will be returned, and if the file is read only then a
".FILRO" error will be returned.

If the write goes beyond the current end of file then the file will be extended
as necessary. If the file pointer is already beyond the end of the file then
disk space will be allocated to fill the gap and will not be initialized. If
there is insufficient disk space then a ".DKFUL" error will be returned and no
data will be written, even if there was room for some of the data.

The number of bytes written can usually be ignored since it will either be zero
if an error is returned or it will be equal to the number requested if the
write was successful. It is very much more efficient to write files in a few
large blocks rather than many small ones, so programs should always try to
write in as large blocks as possible.

This function sets a "modified" bit for the file handle which ensures that when
the file handle is closed or ensured, either explicitly or implicitly, the
directory entry will be updated with the new date, time and allocation
information. Also the archive bit will be set to indicate that this file has
been modified since it was last archived.

Writing to device file handles is not a complicated as reading from them
because there are no end of file conditions or line input to worry about. There
are some differences between ASCII and binary mode when writing to the "CON"
device, in that a console status check is done in ASCII mode only. Also printer
echo if enabled will only be done in ASCII mode.
*/
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld e,0(ix)
		ld d,1(ix)
		ld l,2(ix)
		ld h,3(ix)
		ld b,4(ix)
		pop ix

		ld c,#WRITE
		DOSCALL

		or a
		ret z
		ld h, #0xff
		ld l, a
		ret
	__endasm;
}

