#include <string.h>
#include <stdio.h>
#include "heap.h"
#include "assert.h"
#include "dos.h"

#pragma opt_code_size

#define ADDR_POINTER_BYTE(X)	(*((uint8_t*)X))
#define ADDR_POINTER_WORD(X)	(*((uint16_t*)X))

static const char __THIS_FILE__[] = __FILE__;

#undef ASSERT
#undef ASSERT_EQUAL
#undef FAIL
#undef SUCCEED
#undef TODO
#define ASSERT(cond, failMsg)					_ASSERT_TRUE(cond, failMsg, __THIS_FILE__, _func, __LINE__)
#define ASSERT_EQUAL(value, expected, failMsg)	_ASSERT_EQUAL((uint16_t)(value), (uint16_t)(expected), failMsg, __THIS_FILE__, _func, __LINE__)
#define FAIL(failMsg)							_FAIL(failMsg, __THIS_FILE__, _func, __LINE__)
#define SUCCEED()								_SUCCEED(__THIS_FILE__, _func)
#define TODO(infoMsg)							_TODO(infoMsg, __THIS_FILE__, __func__)


const char ERROR[] = "Error";
const char TODO_GENERIC[] = "-----";
const char TODO_UI[] = "UI not mocked";
const char TODO_ONLY_DOS2[] = "Only for DOS2";


// =============================================================================
// Global variables

const char* const NO_FILE = "nofile";
const char* const TEMP_FILE = "file.tmp";
const char* const AUTOEXEC_FILE = "autoexec.bat";
const char* const CCC_FILE = "ccc.com";
const char* const BAD_FILE = "file.bad";
const char* const TEXT_CRLF = "this text\r\nhave carriage";
const char* const TEXT_CRLF_15 = "this text\r\nhave";

extern void dos_initializeFCB();

// =============================================================================

static void beforeAll()
{
	if (dosver() >= VER_MSXDOS2x) {
		mapperInit();
	}
}

static void beforeEach()
{
	memcpy((void*)SYSFCB, 0, sizeof(FCB));
}

static void create_temp_file()
{
	FILEH fh = fcreate(TEMP_FILE, O_RDWR, ATTR_NONE);
	fwrite(TEMP_FILE, strlen(TEMP_FILE), fh);
	fflush(fh);
	fclose(fh);
}

static void create_file(char *text)
{
	FILEH fh = fcreate(TEMP_FILE, O_RDWR, ATTR_NONE);
	fwrite(text, strlen(text), fh);
	fflush(fh);
	fclose(fh);
}


// =============================================================================
// TESTS
// =============================================================================

void test_get_current_drive()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint8_t drive = get_current_drive();

	//BDD then
	ASSERT_EQUAL(drive, 0, ERROR);
	SUCCEED();
}

void test_get_current_directory()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	char drive = get_current_drive();

	//BDD when
	char result = get_current_directory(drive, heap_top);

	//BDD then
	ASSERT(strcmp(heap_top, "")==0, ERROR);
	ASSERT_EQUAL(result, 0, ERROR);
	SUCCEED();
}

void test_get_current_directory_FAILS()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	char drive = 8;

	//BDD when
	char result = get_current_directory(drive, heap_top);

	//BDD then
	ASSERT(strcmp(heap_top, "")==0, ERROR);
	ASSERT_EQUAL(result, ERR_IDRV, ERROR);
	SUCCEED();
}

void test_get_program_path()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	char *result = get_program_path(heap_top);

	//BDD then
	ASSERT(strcmp(heap_top, "A:\\")==0, ERROR);
	ASSERT_EQUAL(result, heap_top, ERROR);
	SUCCEED();
}

void test_get_drive_params()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	char drive = get_current_drive();
	*(heap_top) = 0xFF;

	//BDD when
	char result = get_drive_params(drive, (DPARM_info*)heap_top);

	//BDD then
	if (DOS_VER == 1) {
		ASSERT_EQUAL(result, ERR_IBDOS, ERROR);
	}
	if (DOS_VER == 2) {
		ASSERT_EQUAL(result, 0, ERROR);
		ASSERT_EQUAL(*heap_top, 0, ERROR);
	}
	SUCCEED();
}

void test_get_drive_params_FAILS()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	char drive = 9;

	//BDD when
	char result = get_drive_params(drive, (DPARM_info*)heap_top);

	//BDD then
	ASSERT_EQUAL(result, ERR_IBDOS, ERROR);
	SUCCEED();
}

void test_fopen()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);

	//BDD then
	ASSERT_EQUAL(fh & 0xff, 0, ERROR);
	SUCCEED();
}

void test_fopen_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(NO_FILE);

	//BDD when
	FILEH fh = fopen(NO_FILE, O_RDONLY);

	//BDD then
	ASSERT_EQUAL(fh, 0xff00|ERR_NOFIL, ERROR);
	SUCCEED();
}

void test_fclose()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);
	uint16_t result = fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, 0, ERROR);
	SUCCEED();
}

void test_fclose_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	uint16_t result = fclose(255);

	//BDD then
	ASSERT_EQUAL(result, 0xffff, ERROR);
	SUCCEED();
}

void test_fcreate()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	remove(TEMP_FILE);

	//BDD when
	FILEH fh = fcreate(TEMP_FILE, O_RDONLY, ATTR_NONE);

	//BDD then
	ASSERT_EQUAL(fh & 0xff, 0, ERROR);
	SUCCEED();
}

void test_fcreate_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	FILEH fh = fcreate(TEMP_FILE, O_RDONLY, ATTR_NONE);

	//BDD then
	ASSERT(fh < 0xff00, ERROR);
	SUCCEED();
}

void test_remove()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	uint8_t result = remove(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(result, 0, ERROR);
	SUCCEED();
}

void test_remove_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	uint8_t result = remove(NO_FILE);

	//BDD then
	ASSERT(result != 0, ERROR);
	SUCCEED();
}

void test_fread()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);
	uint16_t result = fread(heap_top, strlen(TEMP_FILE), fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(strncmp(TEMP_FILE, heap_top, strlen(TEMP_FILE)), 0, ERROR);
	ASSERT_EQUAL(result, strlen(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_fread_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	uint16_t result = fread(heap_top, strlen(TEMP_FILE), 10);

	//BDD then
	ASSERT(result >= 0xff00, ERROR);
	SUCCEED();
}

void test_fwrite()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	remove(TEMP_FILE);

	//BDD when
	FILEH fh = fcreate(TEMP_FILE, O_WRONLY, ATTR_NONE);
	uint16_t result = fwrite(TEMP_FILE, strlen(TEMP_FILE), fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_fwrite_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	uint16_t result = fwrite(heap_top, 10, 5);

	//BDD then
	ASSERT(result >= 0xff00, ERROR);
	SUCCEED();
}

void test_fputs()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	remove(TEMP_FILE);

	//BDD when
	FILEH fh = fcreate(TEMP_FILE, O_WRONLY, ATTR_NONE);
	uint16_t result = fputs(TEMP_FILE, fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_fputs_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	uint16_t result = fputs(TEMP_FILE, 5);

	//BDD then
	ASSERT(result >= 0xff00, ERROR);
	SUCCEED();
}

void test_fgets()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();
	memset(heap_top, 0, 100);

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);
	char *result = fgets(heap_top, strlen(TEMP_FILE)+1, fh); //gets the content + '\0'
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, heap_top, ERROR);
	ASSERT_EQUAL(strlen(heap_top), strlen(TEMP_FILE), ERROR);
	ASSERT_EQUAL(strcmp(heap_top, TEMP_FILE), 0, ERROR);
	ASSERT_EQUAL(strcmp(result, TEMP_FILE), 0, ERROR);
	SUCCEED();
}

void test_fgets_CRLF()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_file(TEXT_CRLF);
	memset(heap_top, 0, 100);

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);
	char *result = fgets(heap_top, 16, fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, heap_top, ERROR);
	ASSERT_EQUAL(strlen(heap_top), 15, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, TEXT_CRLF_15), 0, ERROR);
	ASSERT_EQUAL(strcmp(result, TEXT_CRLF_15), 0, ERROR);
	SUCCEED();
}

void test_fgets_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	char *result = fgets(heap_top, 100, 5);

	//BDD then
	ASSERT_EQUAL(result, 0, ERROR);
	SUCCEED();
}

void test_fseek_SET()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memcpy(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDWR);
	uint16_t result = fseek(fh, 5, SEEK_SET);
	fread(heap_top, 13, fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	ASSERT_EQUAL(result, 5, ERROR);
	SUCCEED();
}

void test_fseek_CUR()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memcpy(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDWR);
	fread(heap_top, 2, fh);
	uint16_t result = fseek(fh, 3, SEEK_CUR);
	fread(heap_top, 13, fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	ASSERT_EQUAL(result, 5, ERROR);
	SUCCEED();
}

void test_fseek_END()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memcpy(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDWR);
	uint16_t result = fseek(fh, 3, SEEK_END);
	fread(heap_top, 13, fh);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	ASSERT_EQUAL(result, 5, ERROR);
	SUCCEED();
}

void test_fseek_FAILS()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	FILEH fh = 10;
	uint16_t result1 = fseek(fh, 3, SEEK_SET);

	fh = fopen(TEMP_FILE, O_RDWR);
	uint16_t result2 = fseek(fh, 3, 0xff);
	fclose(fh);

	//BDD then
	if (dosver() == VER_MSXDOS1x) {
		ASSERT_EQUAL(result1, 0xffff, ERROR);
	} else {
		ASSERT_EQUAL(result1, 3, ERROR);
	}
	ASSERT_EQUAL(result2, 0xff00|ERR_INTER, ERROR);
	SUCCEED();
}

void test_fflush()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	create_temp_file();

	//BDD when
	FILEH fh = fopen(TEMP_FILE, O_RDWR);
	fseek(fh, 0, SEEK_END);
	fwrite(heap_top, 10, fh);
	fflush(fh);
	uint32_t result = filesize(TEMP_FILE);
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(result, strlen(TEMP_FILE)+10, ERROR);
	SUCCEED();
}

void test_filesize()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint16_t result = filesize(CCC_FILE);

	//BDD then
	ASSERT_EQUAL(result, 749, ERROR);
	SUCCEED();
}

void test_filesize_FAILS()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint16_t result = filesize(BAD_FILE);

	//BDD then
	ASSERT_EQUAL(result, 0xff00 | ERR_NOFIL, ERROR);
	SUCCEED();
}

void test_fileexists()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	bool result = fileexists(AUTOEXEC_FILE);

	//BDD then
	ASSERT_EQUAL(result, true, ERROR);
	SUCCEED();
}

void test_fileexists_FAILS()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	bool result = fileexists(BAD_FILE);

	//BDD then
	ASSERT_EQUAL(result, false, ERROR);
	SUCCEED();
}

void test_dosver()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint8_t result = dosver();

	//BDD then
	ASSERT_EQUAL(result, VER_MSXDOS1x, ERROR);
	SUCCEED();
}

void test_parse_pathname()
{
	const char *_func = __func__;

	beforeEach();
	uint8_t result;
	PATH_parsed info;

	const char *PATH_TEST1 = "b:\\dir1\\dir2\\file1 param1";
	const char *PATH_TEST2 = "a:\\dir1\\";
	const char *PATH_TEST3 = "\\dir1\\file1 param1";

	//Test 1
	result = parse_pathname(PATH_TEST1, &info);
	ASSERT_EQUAL(result, 0, ERROR);
	ASSERT_EQUAL(info.drive, 2, ERROR);
	ASSERT_EQUAL(info.termChar-PATH_TEST1, 18, ERROR);
	ASSERT_EQUAL(info.lastItem-PATH_TEST1, 13, ERROR);

	//Test 2
	result = parse_pathname(PATH_TEST2, &info);
	ASSERT_EQUAL(result, 0, ERROR);
	ASSERT_EQUAL(info.drive, 1, ERROR);
	ASSERT_EQUAL(info.termChar-PATH_TEST2, 8, ERROR);
	ASSERT_EQUAL(info.lastItem-PATH_TEST2, 8, ERROR);

	//Test 3
	result = parse_pathname(PATH_TEST3, &info);
	ASSERT_EQUAL(result, 0, ERROR);
	ASSERT_EQUAL(info.drive, 1, ERROR);
	ASSERT_EQUAL(info.termChar-PATH_TEST3, 11, ERROR);
	ASSERT_EQUAL(info.lastItem-PATH_TEST3, 6, ERROR);

	//BDD then
	SUCCEED();
}

void test_get_env()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when

	//BDD then
	SUCCEED();
}

void test_explain()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memset(heap_top, 0, 100);

	//BDD when
	if (dosver() == VER_MSXDOS1x) {
		explain(heap_top, ERR_NOFIL);
		ASSERT_EQUAL(strcmp(heap_top, "Only DOS2"), 0, ERROR);
	} else {
		explain(heap_top, ERR_ISBFN);
		ASSERT_EQUAL(strcmp(heap_top, "Invalid sub-function number"), 0, ERROR);
		explain(heap_top, ERR_NOFIL);
		ASSERT_EQUAL(strcmp(heap_top, "File not found"), 0, ERROR);
		explain(heap_top, ERR_NORAM);
		ASSERT_EQUAL(strcmp(heap_top, "Not enough memory"), 0, ERROR);
		explain(heap_top, ERR_INTER);
		ASSERT_EQUAL(strcmp(heap_top, "Internal error"), 0, ERROR);
	}

	//BDD then
	SUCCEED();
}

void test_set_transfer_address()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memset(heap_top, 0, 100);
	FILEH fh = fopen(TEMP_FILE, O_RDONLY);

	//BDD when
	set_transfer_address(heap_top);
	__asm
		push hl
		push de
		ld hl, #1				; Set FCB Record size to 1 byte
		ld (#SYSFCB+14),hl
		ld hl, #8				; Num. bytes to read
		ld de,#SYSFCB
		ld c,#RDBLK				; Read from file
		DOSCALL
		pop de
		pop hl
	__endasm;
	fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, TEMP_FILE), 0, ERROR);
	SUCCEED();
}

void test_read_abs_sector()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memset(heap_top, 0, 512);
	set_transfer_address(heap_top);

	//BDD when
	uint8_t result = read_abs_sector(0, 0, 1);

	//BDD then
	ASSERT_EQUAL(result, 0, ERROR);
	ASSERT_EQUAL(strncmp(heap_top+133, "Boot error\r\nPress any key", 25), 0, ERROR);
	SUCCEED();
}

void test_mapperGetTotalSegments()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint8_t result = mapperGetTotalSegments();

	//BDD then
	ASSERT_EQUAL(result, 32, ERROR);
	SUCCEED();
}

void test_mapperGetFreeSegments()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint8_t result = mapperGetFreeSegments();

	//BDD then
	ASSERT_EQUAL(result, mapperGetTotalSegments()-(4+7), ERROR);
	SUCCEED();
}

void test_mapperGetCurrentSegmentInPage()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();

	//BDD when
	uint8_t result0 = mapperGetCurrentSegment(0);
	uint8_t result1 = mapperGetCurrentSegment(1);
	uint8_t result2 = mapperGetCurrentSegment(2);
	uint8_t result3 = mapperGetCurrentSegment(3);

	//BDD then
	ASSERT_EQUAL(result0, 3, ERROR);
	ASSERT_EQUAL(result1, 2, ERROR);
	ASSERT_EQUAL(result2, 1, ERROR);
	ASSERT_EQUAL(result3, 0, ERROR);
	SUCCEED();
}

void test_ram_mapper()
{
	const char *_func = __func__;
	if (dosver() == VER_MSXDOS1x) {
		TODO(TODO_ONLY_DOS2);
		return;
	}

	//BDD given
	MAPPER_Segment *segm = (MAPPER_Segment*)heap_top;
	beforeEach();

	//BDD when
	mapperSetOriginalSegmentBack(2);
	ADDR_POINTER_BYTE(0x8000) = 0xff;

	for (uint8_t i=0; i<20; i++) {
		ASSERT_EQUAL(mapperAllocateSegment(&segm[i]), 0, ERROR);
		mapperSetSegment(2, &segm[i]);
		ADDR_POINTER_BYTE(0x8000) = i;
	}

	for (uint8_t i=0; i<20; i++) {
		mapperSetSegment(2, &segm[i]);
		ASSERT_EQUAL(ADDR_POINTER_BYTE(0x8000), i, ERROR);
		ASSERT_EQUAL(mapperFreeSegment(&segm[i]), 0, ERROR);
	}

	mapperSetOriginalSegmentBack(2);
	ASSERT_EQUAL(ADDR_POINTER_BYTE(0x8000), 0xff, ERROR);

	ASSERT_EQUAL(mapperGetCurrentSegment(2), 1, ERROR);

	//BDD then
	SUCCEED();
}

void test_get_drive_letter_info()
{
	const char *_func = __func__;

	//BDD given
	beforeEach();
	memset(heap_top, 0, 100);

	//BDD when
//	uint8_t result = get_drive_letter_info();

	//BDD then
//	ASSERT_EQUAL(result, 0, ERROR);
	ASSERT_EQUAL(strncmp(heap_top+133, "Boot error\r\nPress any key", 25), 0, ERROR);
	SUCCEED();
}





// =============================================================================
// =============================================================================
// main
// =============================================================================
// =============================================================================

int main(char** argv, int argc)
{
	cputs("### UNIT TESTS for DOS.LIB ###");

	beforeAll();

	test_get_current_drive();
	test_get_current_directory(); test_get_current_directory_FAILS();
	test_get_program_path();
	test_get_drive_params(); test_get_drive_params_FAILS();

	test_fopen(); test_fopen_FAILS();
	test_fclose(); test_fclose_FAILS();
	test_fcreate(); test_fcreate_FAILS();
	test_remove(); test_remove_FAILS();
	test_fread(); test_fread_FAILS();
	test_fwrite(); test_fwrite_FAILS();
	test_fputs(); test_fputs_FAILS();
	test_fgets(); test_fgets_CRLF(); test_fgets_FAILS();
	test_fseek_SET(); test_fseek_CUR(); test_fseek_END(); test_fseek_FAILS();
	test_fflush();
	test_filesize(); test_filesize_FAILS();
	test_fileexists(); test_fileexists_FAILS();

	//test_dosver();
	//test_parse_pathname();
	//test_get_env();
	test_explain();
	//test_set_abort_routine();

	test_set_transfer_address();
	test_read_abs_sector();
	//test_write_abs_sector();

	test_mapperGetTotalSegments();
	test_mapperGetFreeSegments();
	test_mapperGetCurrentSegmentInPage();
	test_ram_mapper();

	//test_get_drive_letter_info();
}
