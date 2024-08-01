#include "dos.h"

#ifndef DISABLE_CONIO

bool kbhitBios(void) __naked __sdcccall(1)
{
/*
    CONSOLE STATUS (0BH)
    Parameters:    C = 0BH (_CONST)

    Results:     L=A = 00H if no key was pressed
                     = FFH if a key was pressed

A flag is returned in register A to indicate whether a character is ready 
(that is, a key was pressed) for input from the keyboard. If a character is 
ready then it will be read and tested for certain special control characters. 
If it is not one of these then it is stored in an internal single byte buffer 
and subsequent call to this function will return "character ready" immediately 
without checking the keyboard. If this function says that a character is ready 
then the character may be read by function 02h or 08h.

If the character is "Ctrl-C" then the program will be terminated with a ".CTRLC" 
error via the user's abort routine if one is defined.
If the character is "Ctrl-P" then printer echo will be enabled and it will be 
disabled if it is "Ctrl-N". 
If the character is "Ctrl-S" then the routine will hang up waiting for another 
character to be pressed and then return "no character ready", thus providing a 
"wait" facility. The character typed to continue operation will be ignored, 
except that of it is "Ctrl-C" then the program will be terminated. These same 
input checks are also done for functions 01h, 02h, 08h, 09h and 0Ah.
*/
	__asm
		push ix

		ld c,#CONST
		DOSCALL
		
		pop ix
		and #1			; Returns A
		ret
	__endasm;
}

#endif	//DISABLE_CONIO
