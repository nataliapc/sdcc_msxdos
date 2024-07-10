//https://www.msx.org/wiki/Extended_Bios#004h_.284.29_-_Memory_Mapper_.28MSX-DOS_2.29
#include "dos.h"


#define EXTBIO     0xffca	// Call to extended BIOS (SUB-ROM)


//###################################################################
// Variables & Func.Definitions

static uint8_t _slotMainMapper;
static uint8_t _totalSegments;
static uint8_t _originalSegment[4];
static const uint16_t const _jumpTableAddress[4] = {
	0x00,		// ALL_SEG
	0x03,		// FREE_SEG
	0x18,		// PUT_Px
	0x1b,		// GET_Px
};

static uint8_t _getJumpTable(void) __sdcccall(0);
static uint8_t _mapperGetSegment(uint8_t page) __z88dk_fastcall;
static void    _mapperSetSegment(uint8_t page, uint8_t segment) __sdcccall(0);


//###################################################################
// Public Functions

uint8_t mapperInit(void)
{
	int i;
	uint8_t freeSegments = _getJumpTable();
	uint16_t *table = &_jumpTableAddress[1];

	for (i = 0; i < 3; i++) {
		*(table++) += *_jumpTableAddress;
	}

	for (i = 0; i < 4; i++) {
		_originalSegment[i] = _mapperGetSegment(i);
	}

	return freeSegments;
}

inline uint8_t mapperGetSlot(void)
{
	return _slotMainMapper;
}

inline uint8_t mapperGetTotalSegments(void)
{
	return _totalSegments;
}

inline uint8_t mapperGetFreeSegments(void)
{
	return _getJumpTable();
}

ERR8 mapperAllocateSegment(MAPPER_Segment *returnedData) __naked __z88dk_fastcall
{
	__asm
		ex de,hl			// store _returnedData in DE

		xor a				// A=0 to allocate user segment (A=1 to allocate system segment)
		ld  b, a			// B=0 to allocate from primary mapper

		// to simulate CALL (PUSH + JP)
		ld  hl, #mapAll_cont$
		push hl

		// call to HL+0h [ALL_SEG: Allocate a segment]
		ld  hl, (__jumpTableAddress)
		jp (hl)

	mapAll_cont$:
		jr c, mapAll_error$	// error if carry

		// add page/slot to _returnedData
		ex de,hl			// recover _returnedData to HL
		ld (hl), a
		inc hl
		ld (hl), b
		ld l, #0
		ret

	mapAll_error$:
		// error no ram free
		ld l, #ERR_NORAM
		ret
	__endasm;
}

ERR8 mapperFreeSegment(MAPPER_Segment *segmentToFree) __naked __z88dk_fastcall
{
	__asm
		; HL = _segmentToFree pointer
		ld a, (hl)			; A = Segment number
		inc hl
		ld b, (hl)			; B = Mapper slot in the form FxxxSSPP (0 for the main Memory Mapper)

		// to simulate CALL (PUSH + JP)
		ld  hl, #mapFre_cont$
		push hl
		// call to HL+3h [FRE_SEG: Free a segment]
		// Entry:    A = Segment number
		//           B = Mapper slot in the form FxxxSSPP (0 for the main Memory Mapper)
		// Output:   F = Carry is set if nothing has been freed
		ld  hl, (__jumpTableAddress + 2)
		jp (hl)
	mapFre_cont$:

		ld l, #0
		ret nc
		// error if carry
		ld l, #ERR_INTER
		ret
	__endasm;
}

inline void mapperSetSegment(uint8_t page, MAPPER_Segment *segment)
{
	_mapperSetSegment(page, segment->segment);
}

inline uint8_t mapperGetCurrentSegment(uint8_t page)
{
	return _mapperGetSegment(page);
}

inline void mapperSetOriginalSegmentBack(uint8_t page)
{
	_mapperSetSegment(page, _originalSegment[page]);
}


//###################################################################
// Private Functions

static uint8_t _getJumpTable(void) __naked __sdcccall(0)
{
	__asm
		xor a
		ld  de, #0x0402		; D=4 (Memory mapper DevID) E=2 (Get the routines jump table)
		EXTBIOCALL
		ld  (__jumpTableAddress), hl

		ld  (__totalSegments), a
		ld  a, b
		ld  (__slotMainMapper), a
		ld  l, c			; returns the free segments of primary mapper
		ret
	__endasm;
}

static uint8_t _mapperGetSegment(uint8_t page) __naked __z88dk_fastcall
{
	__asm
		; L = page
		ld c, l				// page

		// to simulate CALL (PUSH + JP)
		ld  hl, #mapGet_cont$
		push hl

		// call to HL+1bh [GET_Px: Get the segment number on page x]
		//    Output: A = Segment number
		ld  hl, (__jumpTableAddress + 6)

		jp pageMult6andCall$
	mapGet_cont$:

		ld l, a
		ret
	__endasm;
}

static void _mapperSetSegment(uint8_t page, uint8_t segment) __naked __sdcccall(0)
{
	page, segment;
	__asm
		push ix
		ld ix,#4
		add ix,sp
		ld c,0(ix)			// page
		ld a,1(ix)			// segment
		pop ix

		// call to HL+18h [PUT_Px: Select a segment on page x]
		//     Entry: A = Segment number 
		ld  hl, (__jumpTableAddress + 4)

	pageMult6andCall$:
		ld  d,#0		// + (4 page)
		ld  e,c
		sla e
		sla e
		add hl,de

		ld  e,c			// + (2 page)
		add hl,de
		add hl,de

		jp (hl)
	__endasm;
}
