#include "dos.h"


RE16 fwrite(char* buf, uint16_t size, char fh) __naked __sdcccall(0)
{
	buf, size, fh;

#ifdef MSXDOS1
/*
    RANDOM BLOCK WRITE [FCB] (26H)
    Parameters:    C = 26H (_WRBLK)
                  DE = Pointer to opened FCB
                  HL = Number of records to write
    Results:       A = 01H if error 
                     =  0  if no error

Data is written from the current disk transfer address to the position in the
file defined by the random record number. The record size is determined by the
record size field in the FCB (bytes 0Eh and 0Fh) which must be set by the user
after opening the file and before calling this function. If the record size is
less than 64 bytes then all four bytes of the random record number are used,
otherwise only the first three are used.

The number of records to be written is specified by HL, and together with the
record size this determines the amount of data to be written. An error will be
returned if the size exceeds 64k, thus limiting the maximum size of a transfer.

After writing the data, the random record field is adjusted to the next record
number in the file (ie. HL is added on to it). The current record and extent
fields are not used or altered. The file size field is updated if the file has
been extended.

The record size can be any value from 1...0FFFFh. Small record sizes are no
less efficient that large record sizes so if desired the record size can be set
to one and the record count then becomes a byte count. It is desirable to write
as much as possible with one function call since one large transfer will be
quicker than several small ones.

If the number of records to write (HL) is zero then no data will be written,
but the size of the file will be altered to the value specified by the random
record field. This may be either longer or shorter than the file's current size
and disk space will be allocated or freed as required. Additional disk space
allocated in this way will not be initialized to any particular value.
*/
	__asm
		ld hl,#1                      ; Set FCB Record size to 1 byte
		ld (#SYSFCB+14),hl

		push ix
		ld ix,#4
		add ix,sp
		ld e,0(ix)  ; Disk trasfer address
		ld d,1(ix)

		ld c,#SETDTA                  ; Set Disk transfer address (DTA)
		DOSCALL

		ld l,2(ix)  ; Num. bytes to write
		ld h,3(ix)
		pop ix
		push hl

		ld de,#SYSFCB
		ld c,#WRBLK
		DOSCALL

		pop hl
		or a
		ret z
		ld hl, #0xffdf
		ret
	__endasm;
#endif

#ifdef MSXDOS2
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
#endif

}

