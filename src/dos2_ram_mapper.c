//https://www.msx.org/wiki/Extended_Bios#004h_.284.29_-_Memory_Mapper_.28MSX-DOS_2.29
//https://www.msx.org/wiki/Memory_Mapper
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

static uint8_t _getJumpTable(void) __sdcccall(1);
static uint8_t _mapperGetSegment(uint8_t page) __sdcccall(1);
static void    _mapperSetSegment(uint8_t page, uint8_t segment) __sdcccall(1);


//###################################################################
// Public Functions

/**
 * mapperInit
 * Initialize RAM mapper.
 *
 * @return Count of free segments.
 */
RETB mapperInit(void)
{
	int i;
	uint8_t freeSegments = _getJumpTable();

	static bool initialized = false;
	if (initialized) return freeSegments;
	initialized = true;

	uint16_t *table = &_jumpTableAddress[1];

	for (i = 0; i < 3; i++) {
		*(table++) += *_jumpTableAddress;
	}

	for (i = 0; i < 4; i++) {
		_originalSegment[i] = _mapperGetSegment(i);
	}

	return freeSegments;
}

/**
 * mapperGetSlot
 * Gets the slot of the main mapper.
 *
 * @return The slot number where main mapper is located.
 */
inline RETB mapperGetSlot(void)
{
	return _slotMainMapper;
}

/**
 * mapperGetTotalSegments
 * Gets the total number of segments available in the mapper.
 *
 * @return The total number of segments in the mapper.
 */
inline RETB mapperGetTotalSegments(void)
{
	return _totalSegments;
}

/**
 * mapperGetFreeSegments
 * Gets the number of free segments available in the mapper.
 *
 * @return The number of free segments in the mapper.
 */
inline RETB mapperGetFreeSegments(void)
{
	return _getJumpTable();
}

/**
 * mapperAllocateSegment
 * Allocates a new segment from the memory mapper.
 * 
 * @param returnedData Pointer to store the allocated segment information.
 * @return Error code (0 if successful, ERR_NORAM if no free RAM available).
 * 
 * This function allocates a new segment from the primary mapper.
 * It uses inline assembly to call the memory mapper's allocation routine.
 * The allocated segment information (page and slot) is stored in the MAPPER_Segment structure.
 */
ERRB mapperAllocateSegment(MAPPER_Segment *returnedData) __naked __sdcccall(1)
{
	__asm
							// HL = Param returnedData
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
		xor a				// Returns A = no error = $0
		ret

	mapAll_error$:
		// error no ram free
		ld a, #ERR_NORAM	// Returns A = error = ERR_NORAM
		ret
	__endasm;
}

/**
 * mapperFreeSegment
 * Frees a previously allocated segment from the memory mapper.
 * 
 * @param segmentToFree Pointer to the MAPPER_Segment structure containing the segment information to be freed.
 * @return Error code (0 if successful, ERR_INTER if the segment couldn't be freed).
 * 
 * This function frees a segment from the memory mapper using inline assembly.
 * It calls the memory mapper's free segment routine and handles potential errors.
 */
ERRB mapperFreeSegment(MAPPER_Segment *segmentToFree) __naked __z88dk_fastcall
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

		ld l, #0			; Returns L
		ret nc				; no error = $0
		// error if carry
		ld l, #ERR_INTER	; error = ERR_INTER
		ret
	__endasm;
}

/**
 * mapperSetSegment
 * Sets a specific segment to a given page in the memory mapper.
 * 
 * @param page The page number where the segment will be set.
 * @param segment Pointer to the MAPPER_Segment structure containing the segment information.
 * 
 * This function sets a segment to a specified page in the memory mapper.
 * It calls the internal _mapperSetSegment function to perform the actual mapping.
 */
inline void mapperSetSegment(uint8_t page, MAPPER_Segment *segment)
{
	_mapperSetSegment(page, segment->segment);
}

/**
 * mapperGetCurrentSegment
 * Retrieves the current segment number for a given page in the memory mapper.
 * 
 * @param page The page number for which to get the current segment.
 * @return The segment number currently mapped to the specified page.
 * 
 * This function calls the internal _mapperGetSegment function to retrieve
 * the current segment number for the specified page in the memory mapper.
 */
inline RETB mapperGetCurrentSegment(uint8_t page)
{
	return _mapperGetSegment(page);
}

/**
 * mapperSetOriginalSegmentBack
 * Restores the original segment for a given page in the memory mapper.
 * 
 * @param page The page number for which to restore the original segment.
 * 
 * This function sets the segment of the specified page back to its original value.
 * It uses the internal _mapperSetSegment function to perform the actual mapping,
 * and retrieves the original segment value from the _originalSegment array.
 */
inline void mapperSetOriginalSegmentBack(uint8_t page)
{
	_mapperSetSegment(page, _originalSegment[page]);
}


//###################################################################
// Private Functions

/**
 * _getJumpTable
 * Retrieves the memory mapper jump table and initializes related variables.
 * 
 * This function performs the following operations:
 * 1. Calls the EXTBIO routine to get the memory mapper jump table.
 * 2. Stores the jump table address in __jumpTableAddress.
 * 3. Stores the total number of segments in __totalSegments.
 * 4. Stores the slot of the main mapper in __slotMainMapper.
 * 5. Returns the number of free segments in the primary mapper.
 * 
 * @return The number of free segments in the primary mapper.
 */
static uint8_t _getJumpTable(void) __naked __sdcccall(1)
{
	__asm
		xor a
		ld  de, #0x0402			// D=4 (Memory mapper DevID) E=2 (Get the routines jump table)
		EXTBIOCALL
		ld  (__jumpTableAddress), hl

		ld  (__totalSegments), a
		ld  a, b
		ld  (__slotMainMapper), a
		ld  a, c				// Returns A = the free segments of primary mapper
		ret
	__endasm;
}

/**
 * _mapperGetSegment
 * Retrieves the current segment number for a given page in the memory mapper.
 *
 * This function performs the following operations:
 * 1. Takes the page number as input in register A.
 * 2. Calls the GET_Px routine from the memory mapper jump table.
 * 3. Returns the segment number in register A.
 *
 * @param page The page number for which to get the current segment.
 * @return The current segment number for the specified page.
 */
static uint8_t _mapperGetSegment(uint8_t page) __naked __sdcccall(1)
{
	__asm
		ld c,a			// C = Param page

		// call to HL+1bh [GET_Px: Get the segment number on page x]
		//    Output: A = Segment number
		ld  hl, (__jumpTableAddress + 6)

		jp pageMult6andCall$	// Returns A
	__endasm;
}

/**
 * _mapperSetSegment
 * Sets a specific segment for a given page in the memory mapper.
 *
 * This function performs the following operations:
 * 1. Takes the page number as input in register A.
 * 2. Takes the segment number as input in register L.
 * 3. Calls the PUT_Px routine from the memory mapper jump table.
 * 4. Sets the specified segment for the given page.
 *
 * @param page The page number for which to set the segment.
 * @param segment The segment number to set for the specified page.
 */
static void _mapperSetSegment(uint8_t page, uint8_t segment) __naked __sdcccall(1)
{
	page, segment;
	__asm
		ld c,a			// C = Param page
		ld a,l			// A = Param segment

		// call to HL+18h [PUT_Px: Select a segment on page x]
		//     Entry: A = Segment number 
		ld hl,(__jumpTableAddress + 4)

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
