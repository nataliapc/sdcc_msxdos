/*
#####################################################################
	 MSX Constants & structs [2024-07-15]
#####################################################################
*/
#ifndef  __MSX_CONST_H__
#define  __MSX_CONST_H__

#include <stdint.h>


// ========================================================
// Tools
#define DOSCALL     call 5
#define DOSJP       jp 5
#define BIOSCALL 	ld iy,(#EXPTBL-1) \
                	call CALSLT
#define JP_BIOSCALL ld iy,(#EXPTBL-1) \
                	jp CALSLT
#define ASM_HALT	__asm halt __endasm;

#define ADDR_POINTER_BYTE(X)	(*((uint8_t*)X))
#define ADDR_POINTER_WORD(X)	(*((uint16_t*)X))


// ========================================================
// MSX I/O Ports
__sfr __at (0x98) IO_VDP1;
__sfr __at (0x99) IO_VDP2;


// ========================================================
// MSX BIOS
// http://map.grauw.nl/resources/msxbios.php
//
#define CALSLT 	0x01c		// Executes inter-slot call [Input: IY-High byte with slot ID | IX-The address that will be called]
#define WRTVDP	0x047		// Write data in the VDP-register [Input: B-Data to write | C-Register] [Changes: AF,BC]
#define SETRD	0x050		// Sets VRAM address to read [Input: HL-VRAM address (00000h~03FFFh)][Changes: AF]
#define SETWRT	0x053		// Sets VRAM address to write [Input: HL-VRAM address (00000h~03FFFh)][Changes: AF]
#define FILVRM	0x056		// Fill VRAM with value [Input: A-Data byte|BC-Length|HL-VRAM address][Changes: AF,BC]
#define LDIRMV	0x059		// Block transfer to memory from VRAM [Input: BC-Length|DE-Start memory address|HL-VRAM address][Changes: ALL]
#define LDIRVM	0x05c		// Block transfer to VRAM from memory [Input: BC-Length|DE-VRAM address|HL-Start memory address][Changes: ALL]
#define CHGCLR	0x062		// Changes the screen colors [Input: Foreground color in FORCLR | Background color in BAKCLR | Border color in BDRCLR]
#define INITXT	0x06c		// Switches to SCREEN 0 (text screen with 40×24 characters)
#define POSIT	0x0c6		// Moves cursor to the specified position [Input: H-Y pos | L-X pos]


// ========================================================
// MSX-DOS 1.x BIOS
// http://map.grauw.nl/resources/dos2_functioncalls.php
//
#define TERM0   0x00		// Program terminate			CPM MSX1
#define CONIN   0x01		// Console input				CPM MSX1
#define CONOUT  0x02		// Console output				CPM MSX1
#define INNOE   0x08		// Console input w/o echo		    MSX1
#define CONST   0x0B		// Console status				CPM MSX1

#define FOPEN   0x0F		// Open file (FCB)				CPM MSX1
#define FCLOSE  0x10		// Close file (FCB)				CPM MSX1
#define FDELETE 0x13		// Delete file (FCB)			CPM MSX1
#define RDSEQ   0x14		// Sequential read (FCB)		CPM MSX1
#define WRSEQ   0x15		// Sequential write FCB)		CPM MSX1
#define FMAKE   0x16		// Create file (FCB)			CPM MSX1

#define CURDRV  0x19		// Get current drive			CPM MSX1
#define SETDTA  0x1A		// Set disk transfer address	CPM MSX1

#define RDRND   0x21		// Random read (FCB)			CPM MSX1
#define WRRND   0x22		// Random write (FCB)			CPM MSX1
#define FSIZE   0x23		// Get file size (FCB)			CPM MSX1
#define WRBLK   0x26		// Random block read (FCB)		    MSX1
#define RDBLK   0x27		// Random block write (FCB)		    MSX1


// ========================================================
// MSX-DOS Nextor 2.1 BIOS
// https://github.com/Konamiman/Nextor/blob/v2.1/docs/Nextor%202.1%20Programmers%20Reference.md#3-new-function-calls
//
#ifndef __DOS_NEXTOR__
#define __DOS_NEXTOR__
#define FOUT    0x71		// Turn On/Off the fast STROUT mode. When enabled, the _STROUT and _ZSTROUT functions will work faster.
#define ZSTROUT 0x72		// Prints on the screen the string pointed by DE, the string must be terminated with a zero character.
#define RDDRV   0x73		// Reads sectors directly from a drive (no filesystem dependent)
#define WRDRV   0x74		// Writes sectors directly from a drive (no filesystem dependent)
#define RALLOC  0x75
#define DSPACE  0x76		// Get drive space information
#define LOCK    0x77		// Lock/unlock a drive, or get lock state for a drive
#define GDRVR   0x78		// Get information about a device driver
#define GDLI    0x79		// Get information about a drive letter
#define GPART   0x7A		// Get information about a device partition
#define CDRVR   0x7B		// Call a routine in a device driver
#define MAPDRV  0x7C		// Map a drive letter to a driver and device
#define Z80MODE 0x7D		// Enable or disable the Z80 access mode for a driver
#define GETCLUS 0x7E		// Get information for a cluster on a FAT drive
#endif	//__DOS_NEXTOR__


// ========================================================
// MSX/MSX2 system constants
// http://map.grauw.nl/resources/msxsystemvars.php
// https://www.msx.org/wiki/System_variables_and_work_area
//
#define LINL40		0xf3ae	// (BYTE) Width for SCREEN 0 (default 37)
#define LINLEN		0xf3b0	// (BYTE) Current screen width per line
#define CRTCNT		0xf3b1	// (BYTE) Number of lines of current screen (default 24)
#define CLIKSW		0xf3db	// (BYTE) Key click switch. (0 = Disabled / 1 = Enabled)
#define CSRY		0xf3dc	// (BYTE) Y-coordinate of text cursor
#define CSRX		0xf3dd	// (BYTE) X-coordinate of text cursor
#define FORCLR		0xf3e9	// (BYTE) Foreground colour
#define BAKCLR		0xf3eA	// (BYTE) Background colour
#define REPCNT		0xf3f7	// (BYTE) Delay until the auto-repeat of the key begins. (50 by default)
#define PUTPNT		0xf3f8	// (WORD) Address in the keyboard buffer where a character will be written
#define GETPNT		0xf3fa	// (WORD) Address in the keyboard buffer where the next character is read
#define CSRSW		0xfca9	// (BYTE) Cursor display switch (0:hidden | 1:visible)
#define CSTYLE		0xfcaa	// (BYTE) Cursor style (0:full cursor | other:halve cursor)
#define EXPTBL		0xfcc1	// (BYTE) BIOS slot
#define RG9SAV		0xffe8	// (BYTE) Mirror of VDP register 9 (Basic: VDP(10), note: +1)


// ========================================================
// MSX character codes and compatible sentences VT-52
// https://www.msx.org/wiki/MSX_Characters_and_Control_Codes
//
#ifndef __VT_KEY_CODES__
#define __VT_KEY_CODES__
#define  VT_BEEP		"\x07"		// A beep sound
#define  VT_UP			"\x1E"		//27,"A"	; Cursor up
#define  VT_DOWN		"\x1F"		//27,"B"	; Cursor down
#define  VT_RIGHT		"\x1C"		//27,"C"	; Cursor right
#define  VT_LEFT		"\x1D"		//27,"D"	; Cursor left
#define  VT_CLRSCR		"\x0C"		//27,"E"	; Clear screen:	Clears the screen and moves the cursor to home
#define  VT_HOME		"\x0B"		//27,"H"	; Cursor home:	Move cursor to the upper left corner.
#define  VT_CLREOS		"\x1B""J"	// Clear to end of screen:	Clear screen from cursor onwards.
#define  VT_CLREOL		"\x1B""K"	// Clear to end of line:	Deletes the line to the right of the cursor.
#define  VT_INSLINE		"\x1B""L"	// Insert line:	Insert a line.
#define  VT_DELLINE		"\x1B""M"	// Delete line:	Remove line.
#define  VT_CLLINE		"\x1B""l"	// Deletes the full line at the cursor
#define  VT_GOTOXY		"\x1B""Y"	// Set cursor position [rc]:	Move cursor to position [r+32]ow,[c+32]olumn encoded as single characters.
#define  VT_CURFULL		"\x1B""x4"	// Cursor to full size
#define  VT_CURHIDE		"\x1B""x5"	// Removes the cursor
#define  VT_CURHALF		"\x1B""y4"	// Cursor to half size
#define  VT_CURSHOW		"\x1B""y5"	// Shows the cursor

#define  KEY_TAB		 9
#define  KEY_UP			30
#define  KEY_DOWN		31
#define  KEY_RIGHT		28
#define  KEY_LEFT		29
#define  KEY_ENTER		13
#endif	//__VT_KEY_CODES__


#endif //__MSX_CONST_H__
