#include "dos.h"


RETW dos2_fread(char* buf, uint16_t size, char fh) __naked __sdcccall(0)
{
  buf, size, fh;
/*
    READ FROM FILE HANDLE (48H)
    Parameters:    C = 48H (_READ) 
                   B = File handle
                  DE = Buffer address
                  HL = Number of bytes to read  
    Results:       A = Error [ACCV, EOF]
                  HL = Number of bytes actually read

The specified number of bytes are read from the file at the current file
pointer position and copied to the buffer address specified in register DE. The
file pointer is then updated to the next sequential byte. A ".ACCV" error will
be returned if the file handle was opened with the "no read" access bit set.

The number of bytes read may be less than the number requested for various
reasons, and the number read will be returned in register HL if there is no
error. In general if less is read than requested then this should not be
treated as an error condition but another read should be done to read the next
portion, until a ".EOF" error is returned. An ".EOF" error will never be
returned for a partial read, only for a read which reads zero bytes. Reading
files in this way ensures that device file handles will work correctly (see
below).

For disk files the number of bytes read will only be less than the number
requested if the end of the file is reached and in this case the next read
operation will read zero bytes and will return an ".EOF" error. When reading
from a device file handle (for example the standard file handles 0 to 4), the
behaviour depends on the particular device, and on whether it is being read in
ASCII or binary mode (see function 4Bh below). The "CON" device will be
described as an example because it is the most commonly used device, but other
devices behave similarly.

When reading from the "CON" device in binary mode, characters will be read from
the keyboard, without any interpretation and without being echoed to the screen
or printer. The exact number of characters requested will always be read and
there is no end of file condition. Because of the lack of any end of file
indication, great care must be taken when reading from devices in binary mode.

A read function call to the "CON" device in ASCII mode (the default mode and
that which normally applies to the standard input channel), will only read one
line of input. The input line will be read from the keyboard with the normal
line editing facilities available to the user, and the character typed will be
echoed to the screen and to the printer if Ctrl-P is enabled. Special control
characters "Ctrl-P", "Ctrl-N", "Ctrl-S" and "Ctrl-C" will be tested for and
will be treated exactly as for the console status function 0Bh.

When the user types a carriage return the line will be copied to the read
buffer, terminated with a CR-LF sequence and the read function will return with
an appropriate byte count. The next read will start another buffered line input
operation. If the number of bytes requested in the read was less than the
length of the line input then as many character as requested will be returned,
and the next read function call will return immediately with the next portion
of the line until it has all been read.

If the user types a line which starts with a "Ctrl-Z" character then this will
be interpreted as indicating end of file. The line will be discarded and the
read function call will read zero bytes and return an ".EOF" error. A
subsequent read after this will be back to normal and will start another line
input. The end of file condition is thus not permanent.
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

		ld c,#READ
		DOSCALL

		or a
		ret z
		ld h, #0xff
		ld l, a
		ret
	__endasm;
}
