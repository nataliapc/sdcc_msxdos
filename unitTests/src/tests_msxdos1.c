#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"
#include "assert.h"
#include "dos.h"
#include "msx_const.h"

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
const char* const TEXT_CRLF_15 = "this text\r\n";

static bool  resultbool;
static char *resultp;
static RETB  result8;
static RETW  result16;
static RETDW result32;
static char  buff[512];

extern void dos_initializeFCB();

// =============================================================================

static void beforeAll()
{
}

static void beforeEach()
{
	dos_initializeFCB();
	memset(heap_top, 0, 2048);
}

static void create_temp_file()
{
	fcreate(TEMP_FILE);
	fwrite(TEMP_FILE, strlen(TEMP_FILE));
	fclose();
}

static void create_file(char *text)
{
	fcreate(TEMP_FILE);
	fwrite(text, strlen(text));
	fclose();
}


// =============================================================================
// TESTS

void test_getchar()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	ADDR_POINTER_BYTE(ADDR_POINTER_WORD(PUTPNT)) = '#';
	ADDR_POINTER_WORD(PUTPNT)++;

	//BDD when
	result8 = getchar();

	//BDD then
	ASSERT_EQUAL(result8, '#', ERROR);
	SUCCEED();
}

void test_kbhit_TRUE()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	ADDR_POINTER_WORD(PUTPNT)++;

	//BDD when
	result8 = kbhit();

	//BDD then
	ASSERT_EQUAL(result8, true, ERROR);
	SUCCEED();
}

void test_kbhit_FALSE()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	ADDR_POINTER_WORD(PUTPNT) = ADDR_POINTER_WORD(GETPNT);

	//BDD when
	resultbool = kbhit();

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_get_current_drive()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = getCurrentDrive();

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	SUCCEED();
}

void test_get_program_path()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	resultp = getProgramPath(heap_top);

	//BDD then
	ASSERT(strcmp(resultp, "A:\\TESTDOS1.COM")==0 ||
		   strcmp(resultp, "A:\\")==0, heap_top);
	ASSERT_EQUAL(resultp, heap_top, ERROR);
	SUCCEED();
}

void test_fopen()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	resultbool = fopen(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, true, ERROR);
	SUCCEED();
}

void test_fopen_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(NO_FILE);

	//BDD when
	resultbool = fopen(NO_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_fclose()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	resultbool = fclose();

	//BDD then
	ASSERT_EQUAL(resultbool, true, ERROR);
	SUCCEED();
}

void test_fclose_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);
	dos_initializeFCB();

	//BDD when
	resultbool = fclose();

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_fcreate()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);

	//BDD when
	resultbool = fcreate(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, true, ERROR);
	SUCCEED();
}

void test_fcreate_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	resultbool = fcreate("A\n");

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_remove()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	resultbool = remove(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, true, ERROR);
	SUCCEED();
}

void test_remove_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	resultbool = remove(NO_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_fread()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	result16 = fread(heap_top, strlen(TEMP_FILE));
	fclose();

	//BDD then
	ASSERT_EQUAL(result16, strlen(TEMP_FILE), ERROR);
	ASSERT_EQUAL(strncmp(TEMP_FILE, heap_top, strlen(TEMP_FILE)), 0, ERROR);
	SUCCEED();
}

void test_fread_EOF()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	result16 = fread(heap_top, strlen(TEMP_FILE)+20);
	fclose();

	//BDD then
	ASSERT_EQUAL(result16, 0, ERROR);
	ASSERT_EQUAL(strncmp(TEMP_FILE, heap_top, strlen(TEMP_FILE)), 0, ERROR);
	SUCCEED();
}

void test_fread_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);
	dos_initializeFCB();

	//BDD when
	result16 = fread(heap_top, strlen(TEMP_FILE));

	//BDD then
	ASSERT_EQUAL(result16, 0, ERROR);
	SUCCEED();
}

void test_fwrite()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);

	//BDD when
	fcreate(TEMP_FILE);
	result16 = fwrite(TEMP_FILE, strlen(TEMP_FILE));
	fclose();

	//BDD then
	ASSERT_EQUAL(result16, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_fwrite_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	fopen("A\n");

	//BDD when
	result16 = fwrite(TEMP_FILE, 10);

	//BDD then
	ASSERT_EQUAL(result16, 10, ERROR);
	SUCCEED();
}

void test_fputs()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);

	//BDD when
	fcreate(TEMP_FILE);
	result16 = fputs(TEMP_FILE);
	fclose();

	//BDD then
	ASSERT_EQUAL(result16, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_fputs_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	dos_initializeFCB();

	//BDD when
	result16 = fputs(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(result16, 8, ERROR);
	SUCCEED();
}

void test_fgets()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();
	memset(heap_top, 0, 100);

	//BDD when
	fopen(TEMP_FILE);
	char *result = fgets(heap_top, strlen(TEMP_FILE)+1); //gets the content + '\0'
	fclose();

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
	beforeEach();

	//BDD given
	create_file(TEXT_CRLF);
	memset(heap_top, 0, 100);

	//BDD when
	fopen(TEMP_FILE);
	resultp = fgets(heap_top, 16);
	fclose();

	//BDD then
	ASSERT_EQUAL(resultp, heap_top, ERROR);
	ASSERT_EQUAL(strlen(resultp), 11, ERROR);
	ASSERT_EQUAL(strcmp(resultp, TEXT_CRLF_15), 0, ERROR);
	SUCCEED();
}

void test_fgets_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	remove(TEMP_FILE);
	dos_initializeFCB();

	//BDD when
	resultp = fgets(heap_top, 100);

	//BDD then
	ASSERT_EQUAL(resultp, heap_top, ERROR);
	ASSERT_EQUAL(*resultp, 0, ERROR);
	SUCCEED();
}

void test_fseek_SET()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	result32 = fseek(5, SEEK_SET);
	fread(heap_top, 13);
	fclose();

	//BDD then
	ASSERT_EQUAL(result32, 5, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	SUCCEED();
}

void test_fseek_CUR()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	fread(heap_top, 2);
	result32 = fseek(3, SEEK_CUR);
	fread(heap_top, 13);
	fclose();

	//BDD then
	ASSERT_EQUAL(result32, 5, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	SUCCEED();
}

void test_fseek_END()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	result32 = fseek(-3, SEEK_END);
	fread(heap_top, 13);
	fclose();

	//BDD then
	ASSERT_EQUAL(result32, 5, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	SUCCEED();
}

void test_fseek_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();
	dos_initializeFCB();

	//BDD when 1
	result32 = fseek(3, SEEK_SET);
	ASSERT_EQUAL(result32, 3, ERROR);

	//BDD when 2
	fopen(TEMP_FILE);
	result32 = fseek(3, 0xff);
	fclose();
	ASSERT_EQUAL(result32, -1, ERROR);

	//BDD then
	SUCCEED();
}

void test_fflush()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	fopen(TEMP_FILE);
	fseek(0, SEEK_END);
	fwrite(heap_top, 10);
	fflush();
	fclose();
	result32 = filesize(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(result32, strlen(TEMP_FILE)+10, ERROR);
	SUCCEED();
}

void test_filesize()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result32 = filesize(CCC_FILE);

	//BDD then
	ASSERT_EQUAL(result32, 749, ERROR);
	SUCCEED();
}

void test_filesize_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result32 = filesize(BAD_FILE);

	//BDD then
	ASSERT_EQUAL(result32, -1, ERROR);
	SUCCEED();
}

void test_fileexists()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	resultbool = fileexists(AUTOEXEC_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, true, ERROR);
	SUCCEED();
}

void test_fileexists_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	resultbool = fileexists(BAD_FILE);

	//BDD then
	ASSERT_EQUAL(resultbool, false, ERROR);
	SUCCEED();
}

void test_dosversion()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = dosVersion();

	//BDD then
	ASSERT(result8==VER_MSXDOS1x || result8==VER_MSXDOS2x, ERROR);
	SUCCEED();
}

void test_set_transfer_address()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, 100);
	create_temp_file();
	fopen(TEMP_FILE);

	//BDD when
	setTransferAddress(heap_top);
	__asm
		push hl
		push de
		ld hl, #1				; Set FCB Record size to 1 byte
		ld (#SYSFCB+14),hl
		ld hl, #8				; Num. bytes to read
		ld de, #SYSFCB
		ld c,#RDBLK				; Read from file
		DOSCALL
		pop de
		pop hl
	__endasm;
	fclose();

	//BDD then
	ASSERT_EQUAL(strncmp(heap_top, TEMP_FILE, 8), 0, ERROR);
	SUCCEED();
}

void test_read_abs_sector()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(&buff[0], 0, 512);
	setTransferAddress(&buff[0]);

	//BDD when
	result8 = readAbsoluteSector(0, 0, 1);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(strncmp(&buff[133], "Boot error\r\nPress any key", 25), 0, ERROR);
	SUCCEED();
}

void test_read_abs_sector_IDRV()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	setTransferAddress(&buff[0]);

	//BDD when
	result8 = readAbsoluteSector(10, 0, 1);

	//BDD then
	assert(result8==ERR_IDRV);
	SUCCEED();
}

// =============================================================================
// TESTS DOS2

void test_mapperGetTotalSegments()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = mapperGetTotalSegments();

	//BDD then
	ASSERT_EQUAL(result8, 32, ERROR);
	SUCCEED();
}

void test_mapperGetFreeSegments()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = mapperGetFreeSegments();

	//BDD then
	ASSERT_EQUAL(result8, mapperGetTotalSegments()-(4+7), ERROR);
	SUCCEED();
}

void test_mapperGetCurrentSegmentInPage()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

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

void test_dos2_get_drive_params()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(DPARM_info));
	DPARM_info *info = (DPARM_info*)heap_top;
	char drive = getCurrentDrive();

	//BDD when
	result8 = dos2_getDriveParams(drive+1, info);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(info->drvnum, 1, ERROR);
	ASSERT_EQUAL(info->secSize, 512, ERROR);
	ASSERT_EQUAL(info->numFats, 2, ERROR);
	SUCCEED();
}

void test_dos2_get_drive_params_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(DPARM_info));
	DPARM_info *info = (DPARM_info*)heap_top;
	char drive = 9;

	//BDD when
	result8 = dos2_getDriveParams(drive, info);

	//BDD then
	ASSERT_EQUAL(result8, ERR_IDRV, ERROR);
	SUCCEED();
}

void test_dos2_get_env()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = dos2_getEnv("SHELL", heap_top, 255);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, "A:\\COMMAND2.COM"), 0, ERROR);
	SUCCEED();
}

void test_dos2_get_env_ELONG()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = dos2_getEnv("SHELL", heap_top, 256);

	//BDD then
	ASSERT_EQUAL(result8, ERR_ELONG, ERROR);
	SUCCEED();
}

void test_dos2_get_env_EMPTY()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = dos2_getEnv("EMPTY", heap_top, 200);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, ""), 0, ERROR);
	SUCCEED();
}

void test_dos2_fopen()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDWR);
	dos2_fclose(fh);

	//BDD then
	ASSERT(fh < MAX_HANDLERS, ERROR);
	SUCCEED();
}

void test_dos2_fopen_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	dos2_remove(NO_FILE);

	//BDD when
	FILEH fh = dos2_fopen(NO_FILE, O_RDONLY);

	//BDD then
	ASSERT_EQUAL(fh, ERR_NOFIL, ERROR);
	SUCCEED();
}

void test_dos2_fclose()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDONLY);
	result8 = dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	SUCCEED();
}

void test_dos2_fclose_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	ERRB result1 = dos2_fclose(10);
	ERRB result2 = dos2_fclose(MAX_HANDLERS);

	//BDD then
	ASSERT_EQUAL(result1, ERR_NOPEN, ERROR);
	ASSERT_EQUAL(result2, ERR_IHAND, ERROR);
	SUCCEED();
}

void test_dos2_fcreate()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	dos2_remove(TEMP_FILE);

	//BDD when
	FILEH fh = dos2_fcreate(TEMP_FILE, O_RDWR, ATTR_NONE);
	dos2_fclose(fh);

	//BDD then
	ASSERT(fh < MAX_HANDLERS, ERROR);
	SUCCEED();
}

void test_dos2_fcreate_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fcreate(TEMP_FILE, O_RDONLY, ATTR_NONE);

	//BDD then
	ASSERT_EQUAL(fh, ERR_FILEX, ERROR);
	SUCCEED();
}

void test_dos2_remove()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	result8 = dos2_remove(TEMP_FILE);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	SUCCEED();
}

void test_dos2_remove_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result8 = dos2_remove(NO_FILE);

	//BDD then
	ASSERT_EQUAL(result8, ERR_NOFIL, ERROR);
	SUCCEED();
}

void test_dos2_fread()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDWR);
	result16 = dos2_fread(heap_top, strlen(TEMP_FILE), fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(result16, strlen(TEMP_FILE), ERROR);
	ASSERT_EQUAL(strncmp(TEMP_FILE, heap_top, strlen(TEMP_FILE)), 0, ERROR);
	SUCCEED();
}

void test_dos2_fread_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result16 = dos2_fread(heap_top, strlen(TEMP_FILE), 10);

	//BDD then
	ASSERT_EQUAL(result16, 0xff00|ERR_NOPEN, ERROR);
	SUCCEED();
}

void test_dos2_fwrite()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	dos2_remove(TEMP_FILE);

	//BDD when
	FILEH fh = dos2_fcreate(TEMP_FILE, O_RDWR, ATTR_NONE);
	result16 = dos2_fwrite(TEMP_FILE, strlen(TEMP_FILE), fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(result16, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_dos2_fwrite_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result16 = dos2_fwrite(heap_top, 5, 10);

	//BDD then
	ASSERT_EQUAL(result16, 0xff00|ERR_NOPEN, ERROR);
	SUCCEED();
}

void test_dos2_fputs()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	dos2_remove(TEMP_FILE);

	//BDD when
	FILEH fh = dos2_fcreate(TEMP_FILE, O_RDWR, ATTR_NONE);
	result16 = dos2_fputs(TEMP_FILE, fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(result16, filesize(TEMP_FILE), ERROR);
	SUCCEED();
}

void test_dos2_fputs_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result16 = dos2_fputs(TEMP_FILE, 10);

	//BDD then
	ASSERT_EQUAL(result16, 0xff00|ERR_NOPEN, ERROR);
	SUCCEED();
}

void test_dos2_fseek_SET()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDONLY);
	RETDW newpos = dos2_fseek(fh, 5, SEEK_SET);
	dos2_fread(heap_top, 13, fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	ASSERT_EQUAL(newpos, 5, ERROR);
	SUCCEED();
}

void test_dos2_fseek_CUR()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDONLY);
	dos2_fread(heap_top, 2, fh);
	RETDW newpos = dos2_fseek(fh, 3, SEEK_CUR);
	dos2_fread(heap_top, 13, fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	ASSERT_EQUAL(newpos, 5, ERROR);
	SUCCEED();
}

void test_dos2_fseek_END()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, '\0', 13);
	create_temp_file();

	//BDD when
	FILEH fh = dos2_fopen(TEMP_FILE, O_RDONLY);
	RETDW newpos = dos2_fseek(fh, -3, SEEK_END);
	dos2_fread(heap_top, 13, fh);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(newpos, 5, ERROR);
	ASSERT_EQUAL(strcmp(heap_top, "tmp"), 0, ERROR);
	SUCCEED();
}

void test_dos2_fseek_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	create_temp_file();

	//BDD when
	RETDW result1 = dos2_fseek(10, 3, SEEK_SET);

	FILEH fh = dos2_fopen(TEMP_FILE, O_RDONLY);
	RETDW result2 = dos2_fseek(fh, 3, SEEK_SET|SEEK_CUR|SEEK_END);
	RETDW result3 = dos2_fseek(fh, 30, SEEK_SET);
	dos2_fclose(fh);

	//BDD then
	ASSERT_EQUAL(result1, 0xff00|ERR_NOPEN, ERROR);
	ASSERT_EQUAL(result2, 11, ERROR);
	ASSERT_EQUAL(result3, 30, ERROR);
	SUCCEED();
}

void test_dos2_get_current_directory()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 1, MAX_PATH_SIZE);
	char drive = getCurrentDrive();

	//BDD when
	result8 = dos2_getCurrentDirectory(drive, heap_top);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT(strcmp(heap_top, "")==0, ERROR);
	SUCCEED();
}

void test_dos2_get_current_directory_FAILS()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 1, MAX_PATH_SIZE);
	char drive = 8;

	//BDD when
	result8 = dos2_getCurrentDirectory(drive, heap_top);

	//BDD then
	ASSERT_EQUAL(result8, ERR_IDRV, ERROR);
	ASSERT(strcmp(heap_top, "")==0, ERROR);
	SUCCEED();
}

void test_dos2_findfirst()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(FFBLK));
	FFBLK *ffblk = (FFBLK*)heap_top;

	//BDD when
	result8 = dos2_findfirst("*.SYS", ffblk, ATTR_NONE);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(ffblk->magic, 0xff, ERROR);
	ASSERT_EQUAL(strcmp(ffblk->filename, "MSXDOS.SYS"), 0, ERROR);
	ASSERT_EQUAL(ffblk->attribs, 0, ERROR);
	ASSERT_EQUAL(ffblk->filesize, 2432, ERROR);
	ASSERT_EQUAL(ffblk->drive, 1, ERROR);
	SUCCEED();
}

void test_dos2_findfirst_NOFIL()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(FFBLK));
	FFBLK *ffblk = (FFBLK*)heap_top;

	//BDD when
	result8 = dos2_findfirst("*.BAD", ffblk, ATTR_NONE);

	//BDD then
	ASSERT_EQUAL(result8, ERR_NOFIL, ERROR);
	ASSERT_EQUAL(ffblk->magic, 0xff, ERROR);
	SUCCEED();
}

void test_dos2_findnext()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(FFBLK));
	FFBLK *ffblk = (FFBLK*)heap_top;

	//BDD when
	dos2_findfirst("*.SYS", ffblk, ATTR_NONE);
	result8 = dos2_findnext(ffblk);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(ffblk->magic, 0xff, ERROR);
	ASSERT_EQUAL(strcmp(ffblk->filename, "MSXDOS2.SYS"), 0, ERROR);
	ASSERT_EQUAL(ffblk->attribs, 0, ERROR);
	ASSERT_EQUAL(ffblk->filesize, 2565, ERROR);
	ASSERT_EQUAL(ffblk->drive, 1, ERROR);
	SUCCEED();
}

void test_dos2_findnext_NOFIL()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(FFBLK));
	FFBLK *ffblk = (FFBLK*)heap_top;

	//BDD when
	dos2_findfirst("*.BAT", ffblk, ATTR_NONE);
	result8 = dos2_findnext(ffblk);

	//BDD then
	ASSERT_EQUAL(result8, ERR_NOFIL, ERROR);
	ASSERT_EQUAL(ffblk->magic, 0xff, ERROR);
	SUCCEED();
}

void test_dos2_explain()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, 100);

	//BDD when
	dos2_explain(ERR_ISBFN, heap_top);
	ASSERT_EQUAL(strcmp(heap_top, "Invalid sub-function number"), 0, ERROR);
	dos2_explain(ERR_NOFIL, heap_top);
	ASSERT_EQUAL(strcmp(heap_top, "File not found"), 0, ERROR);
	dos2_explain(ERR_NORAM, heap_top);
	ASSERT_EQUAL(strcmp(heap_top, "Not enough memory"), 0, ERROR);
	dos2_explain(ERR_INTER, heap_top);
	ASSERT_EQUAL(strcmp(heap_top, "Internal error"), 0, ERROR);

	//BDD then
	SUCCEED();
}

void test_dos2_parsePathname()
{
	const char *_func = __func__;
	beforeEach();

	PATH_parsed *info = (PATH_parsed*)heap_top;

	const char *PATH_TEST1 = "b:\\dir1\\dir2\\file1.com param1";
	const char *PATH_TEST2 = "a:\\dir1\\";
	const char *PATH_TEST3 = "\\dir1\\file1 param1";

	//Test 1
	result8 = dos2_parsePathname(PATH_TEST1, info);
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(info->drive, 2, ERROR);
	ASSERT_EQUAL(info->termChar-PATH_TEST1, 22, ERROR);
	ASSERT_EQUAL(info->lastItem-PATH_TEST1, 13, ERROR);
	ASSERT_EQUAL(info->flags.values.moreThanDrive, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDirectory, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDrive, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyFilename, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyExtension, true, ERROR);
	ASSERT_EQUAL(info->flags.values.lastAmbiguous, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDot, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDosDot, false, ERROR);

	//Test 2
	result8 = dos2_parsePathname(PATH_TEST2, info);
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(info->drive, 1, ERROR);
	ASSERT_EQUAL(info->termChar-PATH_TEST2, 8, ERROR);
	ASSERT_EQUAL(info->lastItem-PATH_TEST2, 8, ERROR);
	ASSERT_EQUAL(info->flags.values.moreThanDrive, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDirectory, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDrive, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyFilename, false, ERROR);
	ASSERT_EQUAL(info->flags.values.anyExtension, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastAmbiguous, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDot, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDosDot, false, ERROR);

	//Test 3
	result8 = dos2_parsePathname(PATH_TEST3, info);
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(info->drive, 1, ERROR);
	ASSERT_EQUAL(info->termChar-PATH_TEST3, 11, ERROR);
	ASSERT_EQUAL(info->lastItem-PATH_TEST3, 6, ERROR);
	ASSERT_EQUAL(info->flags.values.moreThanDrive, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDirectory, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyDrive, false, ERROR);
	ASSERT_EQUAL(info->flags.values.anyFilename, true, ERROR);
	ASSERT_EQUAL(info->flags.values.anyExtension, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastAmbiguous, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDot, false, ERROR);
	ASSERT_EQUAL(info->flags.values.lastIsDosDot, false, ERROR);

	//BDD then
	SUCCEED();
}

void test_dos2_getScreenSize()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given

	//BDD when
	result16 = dos2_getScreenSize();

	//BDD then
	ASSERT_EQUAL(result16, 0x1850, ERROR);	// 24 rows x 80 cols
	SUCCEED();
}

void test_dos2_ram_mapper()
{
	const char *_func = __func__;
	if (dosVersion() == VER_MSXDOS1x) {
		TODO(TODO_ONLY_DOS2);
		return;
	}
	beforeEach();

	//BDD given
	MAPPER_Segment *segm = (MAPPER_Segment*)heap_top;
	RETB freeSegments = mapperInit();
	ASSERT_EQUAL(freeSegments, 21, ERROR);

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

// =============================================================================
// TESTS NEXTOR

void test_nxtr_get_drive_letter_info()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, 100);
	DRIVE_info *info = (DRIVE_info*)heap_top;

	//BDD when
	result8 = nxtr_getDriveLetterInfo(0, info);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
	ASSERT_EQUAL(info->driveStatus, 0, ERROR); 
		// 0: Unassigned
		// 1: Assigned to a storage device attached to a Nextor or MSX-DOS driver
		// 2: Unused
		// 3: A file is mounted in the drive
		// 4: Assigned to the RAM disk (all other fields will be zero)
	ASSERT_EQUAL(info->driveSlot, 0, ERROR);	// Driver slot number
	ASSERT_EQUAL(info->driveSegment, 0, ERROR);	// Driver segment number, FFh if driver is embedded within a Nextor or MSX-DOS kernel ROM (always FFh in current version)
	ASSERT_EQUAL(info->relDriveNum, 0, ERROR);	// Relative drive number within the driver (for drive-based drivers only; FFh if device-based driver)
	ASSERT_EQUAL(info->deviceIndex, 0, ERROR);	// Device index (for device-based drivers only; 0 for drive-based drivers and MSX-DOS drivers)
	ASSERT_EQUAL(info->logicalIndex, 0, ERROR);	// Logical unit index (for device-based drivers only; 0 for drive-based drivers and MSX-DOS drivers)
	ASSERT_EQUAL(info->firstDevSec, 0, ERROR);	// First device sector number (for devices in device-based drivers only;  always zero for drive-based drivers and MSX-DOS drivers)
	ASSERT_EQUAL(strncmp(heap_top+133, "Boot error\r\nPress any key", 25), 0, ERROR);
	SUCCEED();
}

void test_nxtr_get_cluster_info_fat()
{
	const char *_func = __func__;
	beforeEach();

	//BDD given
	memset(heap_top, 0, sizeof(CLUSTER_info));
	CLUSTER_info *info = (CLUSTER_info*)heap_top;

	//BDD when
	result8 = nxtr_getClusterInfoFAT(0, 0, info);

	//BDD then
	ASSERT_EQUAL(result8, 0, ERROR);
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

	test_getchar();
	test_kbhit_TRUE(); test_kbhit_FALSE();

	test_get_current_drive();
	test_get_program_path();

	test_fopen(); test_fopen_FAILS();
	test_fclose(); test_fclose_FAILS();
	test_fcreate(); test_fcreate_FAILS();
	test_remove(); test_remove_FAILS();
	test_fread(); test_fread_EOF(); test_fread_FAILS();
	test_fwrite(); test_fwrite_FAILS();
	test_fputs(); test_fputs_FAILS();
	test_fgets(); test_fgets_CRLF(); test_fgets_FAILS();
	test_fseek_SET(); test_fseek_CUR(); test_fseek_END(); test_fseek_FAILS();
	test_fflush();
	test_filesize(); test_filesize_FAILS();
	test_fileexists(); test_fileexists_FAILS();

	test_dosversion();

	test_set_transfer_address();
	test_read_abs_sector();
	//test_read_abs_sector_IDRV();
	//test_write_abs_sector();

	if (dosVersion() >= VER_MSXDOS2x) {
		mapperInit();
		test_mapperGetTotalSegments();
		test_mapperGetFreeSegments();
		test_mapperGetCurrentSegmentInPage();

		test_dos2_get_drive_params(); test_dos2_get_drive_params_FAILS();

		test_dos2_get_env(); test_dos2_get_env_ELONG(); test_dos2_get_env_EMPTY();

		test_dos2_fcreate(); test_dos2_fcreate_FAILS();
		test_dos2_fopen(); test_dos2_fopen_FAILS();
		test_dos2_fclose(); test_dos2_fclose_FAILS();
		//test_dos2_flush();
		test_dos2_remove(); test_dos2_remove_FAILS();
		test_dos2_fread(); test_dos2_fread_FAILS();
		test_dos2_fwrite(); test_dos2_fwrite_FAILS();
		test_dos2_fputs(); test_dos2_fputs_FAILS();
		test_dos2_fseek_SET(); test_dos2_fseek_CUR(); test_dos2_fseek_END(); test_dos2_fseek_FAILS();
		//test_dos2_ftell();

		//test_dos2_set_abort_routine();
		test_dos2_get_current_directory(); test_dos2_get_current_directory_FAILS();
		test_dos2_findfirst(); test_dos2_findfirst_NOFIL();
		test_dos2_findnext(); test_dos2_findnext_NOFIL();
		test_dos2_getScreenSize();
		test_dos2_explain();
		test_dos2_parsePathname();

		test_dos2_ram_mapper();
	}

	if (dosVersion() >= VER_NextorDOS) {
		test_nxtr_get_drive_letter_info();
		test_nxtr_get_cluster_info_fat();
	}

	return 0;
}
