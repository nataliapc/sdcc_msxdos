#include "dos.h"
#include <string.h>


void dos2_explain(uint8_t error_code, char* buffer) __naked __sdcccall(1)
{
	error_code, buffer;
/*
    EXPLAIN ERROR CODE (66H)
    Parameters:    C = 66H (_EXPLAIN) 
                   B = Error code to be explained
                  DE = Pointer to 64 byte string buffer
    Results:       A = 0
                   B = 0 or unchanged
                  DE = Filled in with error message 

This function allows a user program to get an ASCIIZ explanation string for a
particular error code returned by any of the MSX-DOS functions. If an error
comes from one of the old functions then "get previous error code" must be
called first to get the real error code and then this function can be called to
get an explanation string.

The "Program Interface Specification" contains a list of all the currently
defined error codes and the messages for them. Foreign language versions of the
system will of course have different messages. If the error code does have a
built in explanation string then this string will be returned and register B
will be set to zero. If there is no explanation string then a string of the
form: "System error 194" or "User error 45" will be returned, and register B
will be unchanged. (System errors are those in the range 40h...FFh and user
errors are 00h...3Fh.)
*/
	__asm
		ld b,a			; b = error_code
						; de = buffer

		ld c,#EXPLAIN
		DOSCALL

		ret
	__endasm;
}
