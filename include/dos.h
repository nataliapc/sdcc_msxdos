#ifndef ____DOS_H____
#define ____DOS_H____

#include <stdint.h>

#define MSX2
#define MSXDOS2

#define DOSCALL  call 5
#define BIOSCALL ld iy,(#EXPTBL-1) \
                 call CALSLT

#define MAX_PATH 64
#ifndef NULL
	#define NULL     0
#endif

/* SYSTEM vars */
#if defined(MSX2) || defined(CPM)
	#define SYSFCB	0x005c	// File control block in the CP/M system area
#endif
#ifdef MSX2
	#define EXPTBL  0xfcc1	// BIOS slot
#endif

/* DOS calls */
// MSXDOS 1
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
#define RDABS   0x2F		// Absolute sector read			    MSX1
#define WRABS   0x30		// Absolute sector write		    MSX1
// MSXDOS 2
#define DPARM   0x31		// Get disk parameters			         NEW
#define FFIRST  0x40		// Find first entry				         NEW
#define FNEXT   0x41		// Find next entry				         NEW

#define OPEN    0x43		// Open file handle				         NEW
#define CREATE  0x44		// Create file handle			         NEW
#define CLOSE   0x45		// Close file handle			         NEW
#define READ    0x48		// Read from file handle		         NEW
#define WRITE   0x49		// Write from file handle		         NEW
#define SEEK    0x4A		// Move file handle pointer 	         NEW
#define IOCTL   0x4B		// I/O control for devices		         NEW

#define DELETE  0x4D		// Delete file or subdirectory	         NEW

#define GETCD   0x59		// Get current directory		         NEW
#define PARSE   0x5B		// Parse pathname				         NEW

#define TERM    0x62		// Terminate with error code	         NEW
#define EXPLAIN 0x66		// Explain error code			         NEW

#define GENV    0x6B		// Get environment item			         NEW
#define DOSVER  0x6F		// Get MSX-DOS version			         NEW

// Nextor
// https://github.com/Konamiman/Nextor/blob/v2.1/docs/Nextor%202.1%20Programmers%20Reference.md#3-new-function-calls
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

/* MSX DOS versions from dosver() */
#define VER_UNKNOWN     0
#define VER_MSXDOS1x    1
#define VER_MSXDOS2x    2
#define VER_NextorDOS   3

/* DOS errors */
#define NOFIL   0xD7
#define IATTR   0xCF
#define DIRX    0xCC

/* open/create flags */
#define O_RDWR     0x00
#define O_RDONLY   0x01
#define O_WRONLY   0x02
#define O_INHERIT  0x04

/* seek modes */
#define SEEK_SET	0	//Beginning of file
#define SEEK_CUR	1	//Current position of the file pointer
#define SEEK_END	2	//End of file *

/* file attributes */
#define ATTR_NONE      0		// None.
#define ATTR_READONLY  1		// If set then the file cannot be written to or deleted, but can be read, renamed or moved.
#define ATTR_HIDDEN    2		// If set then the file will only be found by the FFIRST function if ATTR_HIDDEN bit is set in attributes byte.
#define ATTR_SYSTEM    4		// For MSX-DOS functions, this bit has exactly the same effect as ATTR_HIDDEN bit except that the FNEW and CREATE calls will not automatically delete a SYSTEM file.
#define ATTR_VOLUME    8		// If set then this entry defines the name of the volume. Only occur in the root directory, and only once. Rest of bits are ignored.
#define ATTR_DIRECTORY 16		// The entry is a subdirectory and cannot be opened for reading and writing. Only the hidden bit has any meaning for sub-directories.
#define ATTR_ARCHIVE   32		// Is set when a file was written to and closed. This bit can be examined by, for example, the XCOPY command to determine whether the file has been changed.
#define ATTR_RESERVED  64		// Reserved (always 0).
#define ATTR_DEVICE    128		// This is set to indicate that the FIB refers to a character device (eg. "CON") rather than a disk file. All of the other attributes bits are ignored.

/* DPARM.fsType filesystem types */
#define FS_FAT12		0x00
#define FS_FAT16		0x01
#define FS_UNKNOWN		0xff

/* FOUT set_fast_out() constants */
#define FASTOUT_OFF   0x00
#define FASTOUT_ON    0xff

/* GDLI Drive Status values */
#define DRVSTAT_UNASSIGNED   0
#define DRVSTAT_ASSIGNED     1
#define DRVSTAT_UNUSED       2
#define DRVSTAT_FILEMOUNTED  3
#define DRVSTAT_RAMDISK      4

/*
	MSX-DOS Call Errors
*/
#define ERR_ISBFN   0xb8	//Invalid sub-function number: The sub-function number passed to the IOCTL function (function 4Bh) was invalid.
#define ERR_EOL     0xb9	//Internal error should never occur.
#define ERR_HDEAD   0xba	//File handle has been deleted: The file associate with a file handle has been deleted so the file handle can no longer be used.
#define ERR_NRAMD   0xbb	//RAM disk does not exist: Attempt to delete the RAM disk when it does not currently exist. A function which tries to access a non-existent RAM disk will get a .IDRV error.
#define ERR_RAMDX   0xbc	//RAM disk (drive H:) already exists: Returned from the "ramdisk" function if trying to create a RAM disk when one already exists.
#define ERR_ITIME   0xbd	//Invalid time: Time parameters passed to "set time" are invalid.
#define ERR_IDATE   0xbe	//Invalid date: Date parameters passed to "set date" are invalid.
#define ERR_ELONG   0xbf	//Environment string too long: Environment item name or value string is either longer than the maximum allowed length of 255, or is too long for the user's buffer.
#define ERR_IENV    0xc0	//Invalid environment string: Environment item name string contains an invalid character.
#define ERR_IDEV    0xc1	//Invalid device operation: Attempt to use a device file handle or fileinfo block for an invalid operation such as searching in it or moving it.
#define ERR_NOPEN   0xc2	//File handle not open: The specified file handle is not currently open.
#define ERR_IHAND   0xc3	//Invalid file handle: The specified file handle is greater than the maximum allowed file handle number.
#define ERR_NHAND   0xc4	//No spare file handles: Attempt to open or create a file handle when all file handles are already in use. There are 64 file handles available in the current version.
#define ERR_IPROC   0xc5	//Invalid process id: Process id number passed to "join" function is invalid.
#define ERR_ACCV    0xc6	//File access violation: Attempt to read or write to a file handle which was opened with the appropriate access bit set. Some of the standard file handles are opened in read only or write only mode.
#define ERR_EOF     0xc7	//End of file: Attempt to read from a file when the file pointer is already at or beyond the end of file.
#define ERR_FILE    0xc8	//File allocation error: The cluster chain for a file was corrupt. Use CHKDSK to recover as much of the file as possible.
#define ERR_OV64K   0xc9	//Cannot transfer above 64K: Disk transfer area would have extended above 0FFFFh.
#define ERR_FOPEN   0xca	//File already in use: Attempt to delete, rename, move, or change the attributes or date and time of a file which has a file handle already open to it, other than by using the file handle itself.
#define ERR_FILEX   0xcb	//File exists: Attempt to create a sub-directory of the same name as an existing file. Files are not automatically deleted when creating sub-directories.
#define ERR_DIRX    0xcc	//Directory exists: Attempt to create a file or sub-directory of the same name as an existing sub-directory. Sub-directories are not automatically deleted.
#define ERR_SYSX    0xcd	//System file exists: Attempt to create a file or sub-directory of the same name as an existing system file. System files are not automatically deleted.
#define ERR_DOT     0xce	//Invalid . or .. operation: Attempt to do an illegal operation on the "." or ".." entries in a sub-directory, such as rename or move them.
#define ERR_IATTR   0xcf	//Invalid attribute: Can result from an attempt to change a file's attributes in an illegal way, or trying to do an operation on a file which is only possible on a sub-directory. Also results from illegal use of volume name fileinfo blocks.
#define ERR_DIRNE   0xd0	//Directory not empty: Attempt to delete a sub-directory which is not empty.
#define ERR_FILRO   0xd1	//Read only file: Attempt to write to or delete a file which has the "read only" attribute bit set.
#define ERR_DIRE    0xd2	//Invalid directory move: Results from an attempt to move a sub-directory into one of its own descendants. This is not allowed as it would create an isolated loop in the directory structure.
#define ERR_DUPF    0xd3	//Duplicate filename: Results from "rename" or "move" if the destination filename already exists in the destination directory.
#define ERR_DKFUL   0xd4	//Disk full: Usually results from a write operation if there was insufficient room on the disk for the amount of data being written. May also result from trying to create or extend a sub-directory if the disk is completely full.
#define ERR_DRFUL   0xd5	//Root directory full: Returned by "create" or "move" if a new entry is required in the root directory and it is already full. The root directory cannot be extended.
#define ERR_NODIR   0xd6	//Directory not found: Returned if a directory item in a drive/path/file string could not be found.
#define ERR_NOFIL   0xd7	//File not found: Can be returned by any function which looks for files on a disk if it does not find one. This error is also returned if a directory was specified but not found. In other cases, .NODIR error (see below) will be returned.
#define ERR_PLONG   0xd8	//Pathname too long: Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the complete path being specified (including current directory if used) is longer than 63 characters.
#define ERR_IPATH   0xd9	//Invalid pathname: Can be returned by any function call which is given an ASCIIZ drive/path/file string. Indicates that the syntax of the string is incorrect in some way.
#define ERR_IFNM    0xda	//Invalid filename: A filename string is illegal. This is only generated for pure filename strings, not drive/path/file strings.
#define ERR_IDRV    0xdb	//Invalid drive: A drive number parameter, or a drive letter in a drive/path/file string is one which does not exist in the current system.
#define ERR_IBDOS   0xdc	//Invalid MSX-DOS call: An MSX-DOS call was made with an illegal function number. Most illegal function calls return no error, but this error may be returned if a "get previous error code" function call is made.
#define ERR_NORAM   0xde	//Not enough memory: MSX-DOS has run out of memory in its 16k kernel data segment. Try reducing the number of sector buffers or removing some environment strings. Also occurs if there are no free segments for creating the RAMdisk.
#define ERR_INTER   0xdf	//Internal error: Should never occur.

typedef struct {			// Off ID  Siz CP/M Function           MSXDOS Function
	uint8_t  drvNum;		//  0 [DR] 1   Drive number containing the file (0:default drive, 1:A, 2:B, ..., 8:H)
	uint8_t  filename[11];	//  1 [Fn] 11  8 bytes for filename and 3 bytes for extension.
	uint8_t  currBlkLo;		// 12 [EX] 1   Extent                  Current block LO / Extent number LO
	uint8_t  currBlkHi;		// 13 [S1] 1   Reserved                Current block HI / File attributes (DOS2)
	uint8_t  recordSizeLo;	// 14 [S2] 1   Reserved                Record size LO / Extent number HI
	uint8_t  recordSizeHi;	// 15 [RC] 1   Record count in extent  Record size HI
	uint32_t fileSize;		// 16 [AL] 4   Allocation              File size in bytes
	uint16_t date;			// 20 [  ] 2   Allocation              Date (DOS1) / Volume ID (DOS2)
	uint16_t time;			// 22 [  ] 2   Allocation              Time (DOS1) / Volume ID (DOS2)
	uint8_t  deviceId;		// 24 [  ] 1   Allocation              Device ID. (DOS1) -> FBh:PRN FCh:LST FCh:NUL FEh:AUX FFh:CON
	uint8_t  dirLoc;		// 25 [  ] 1   Allocation              Directory location (DOS1)
	uint16_t topCluster;	// 26 [  ] 2   Allocation              Top cluster number of the file (DOS1)
	uint16_t lastCluster;	// 28 [  ] 2   Allocation              Last cluster number accessed (DOS1)
	uint16_t relCluster;	// 30 [  ] 2   Allocation              Relative location from top cluster of the file number of clusters from top of the file to the last cluster accessed (DOS1)
	uint8_t  currRecord;	// 32 [CR] 1   Current record within extent (0...127)
	uint32_t rndRecord;		// 33 [Rn] 4   Not Used                Random record number. If record size <64 then all 4 bytes will be used, otherwise 3 bytes.
} FCB;	// File control block definition

typedef struct {
	uint8_t  drvnum;		//     DE+0      - Physical drive number (1=A: etc)
	uint16_t secSize;		//     DE+1,2    - Sector size (always 512 currently)
	uint8_t  secClus;		//     DE+3      - Sectors per cluster (non-zero power of 2)
	uint16_t resvSec;		//     DE+4,5    - Number of reserved sectors (usually 1)
	uint8_t  numFats;		//     DE+6      - Number of copies of the FAT (usually 2)
	uint16_t rootNum;		//     DE+7,8    - Number of root directory entries
	uint16_t totalSec16;	//     DE+9,10   - Total number of logical sectors | NEXTOR: (0h fill)
	uint8_t  mediaDesc;		//     DE+11     - Media descriptor byte
	uint8_t  secFat;		//     DE+12     - Number of sectors per FAT (0=256)
	uint16_t rootSec;		//     DE+13..14 - First root directory sector number
	uint16_t dataSec;		//     DE+15..16 - First data sector number
	uint16_t maxClus;		//     DE+17..18 - Maximum cluster number
	uint8_t  dirtyFlag;		//     DE+19     - Dirty disk flag
	uint32_t volId;			//     DE+20..23 - Volume id. (-1 => no volume id)
	uint32_t totalSec32;	//     DE+24..27 - Reserved (0h fill) | NEXTOR: Logical sectors as a 32 bit value
	uint8_t  fsType;		//     DE+28     - Reserved (0h fill) | NEXTOR: Filesystem type (0:FAT12 1:FAT16 255:Other)
	uint8_t  reserved[3];	//     DE+29..31 - Reserved (0h fill)
} DPARM_info;	// Disk drive parameters info

typedef struct {
	uint8_t driveStatus;	// 0: Unassigned
							// 1: Assigned to a storage device attached to a Nextor or MSX-DOS driver
							// 2: Unused
							// 3: A file is mounted in the drive
							// 4: Assigned to the RAM disk (all other fields will be zero)
	uint8_t driveSlot;		// Driver slot number
	uint8_t driveSegment;	// Driver segment number, FFh if driver is embedded within a Nextor or MSX-DOS kernel ROM (always FFh in current version)
	uint8_t relDriveNum;	// Relative drive number within the driver (for drive-based drivers only; FFh if device-based driver)
	uint8_t deviceIndex;	// Device index (for device-based drivers only; 0 for drive-based drivers and MSX-DOS drivers)
	uint8_t logicalIndex;	// Logical unit index (for device-based drivers only; 0 for drive-based drivers and MSX-DOS drivers)
	uint32_t firstDevSec;	// First device sector number (for devices in device-based drivers only;  always zero for drive-based drivers and MSX-DOS drivers)
	char reserved[54];		// +10...+63: Reserved (currently always zero)
} DRIVE_info;	// Used by GDLI function (Nextor only)

typedef struct {
	uint16_t fatSector;		// +0: FAT sector number that contains the entry for the cluster (2 bytes)
	uint16_t fatSecOffset;	// +2: Offset in the FAT sector where the entry for the cluster is located (0-511) (2 bytes)
	uint32_t sectorData;	// +4: First data sector number the cluster refers to (4 bytes)
	uint16_t clusterValue;	// +8: Value of the FAT entry for the cluster (2 bytes)
	uint8_t  clusterSize;	// +10: Size of a cluster in sectors for the drive (1 byte)
	union{					// +11: Flags (1 byte)
		struct {
			unsigned isFat12:  1;	// bit 0: set if the drive is FAT12
			unsigned isFat16:  1;	// bit 1: set if the drive is FAT16
			unsigned isOdd:    1;	// bit 2: set if the FAT entry for the cluster is an odd entry (FAT12 only)
			unsigned isLast:   1;	// bit 3: set if the cluster is the last one of a file
			unsigned isFree:   1;	// bit 4: set if the cluster is free
			unsigned unused:   3;	// bits 5-7: unused, always zero
		};
		uint8_t raw;
	} flags;
	char     unused[4];		// +12-+15: Unused, always zero
} CLUSTER_info;	// Used by GETCLUS function (Nextor only)

typedef struct {
	uint8_t  magic;			// [1]   0 - Always 0xFF
	char     filename[13];	// [13]  1 - Filename as an ASCIIZ string
	uint8_t  attribs;		// [1]  14 - File attributes byte
	union {					// [2]  15 - Time of last modification
		struct {
			unsigned hours:   5;
			unsigned minutes: 6; 
			unsigned seconds: 5;
		};
		uint16_t raw;
	} modifTime;
	union {					// [2]  17 - Date of last modification
		struct {
			unsigned year:  7;
			unsigned month: 4;
			unsigned day:   5;
		};
		uint16_t raw;
	} modifDate;
	uint16_t startCluster;	// [2]  19 - Start cluster
	uint32_t fileSize;		// [4]  21 - File size
	uint8_t  drive;			// [1]  25 - Logical drive
	uint8_t  internal[38];	// [38] 26 - Internal information, must not be modified
} FFBLK;

typedef struct {
	char      name[8];		// 0x000 [8]  Short file name (padded with spaces). First char '0xE5' for deleted files.
	char      ext[3];		// 0x008 [3]  Short file extension (padded with spaces)
	uint8_t   attr;			// 0x00B [1]  File Attributes. Mask: 0x01:ReadOnly | 0x02:Hidden | 0x04:System | 0x08:Volume | 0x10:Directory | 0x20:Archive
	uint8_t   unused1;		// 0x00C [1]  MSX-DOS 2: For a deleted file, the original first character of the filename
	uint8_t   unused2;		// 0x00D [1]
	uint16_t  ctime;		// 0x00E [2]  Create time: #0-4:Seconds/2 #5-10:Minuts #11-15:Hours
	uint16_t  cdate;		// 0x010 [2]  Create date: #0-4:Day #5-8:Month #9-15:Year(0=1980)
	uint16_t  unused3;		// 0x012 [2]
	uint16_t  unused4;		// 0x014 [2]
	uint16_t  mtime;		// 0x016 [2]  Last modified time: #0-4:Seconds/2 #5-10:Minuts #11-15:Hours
	uint16_t  mdate;		// 0x018 [2]  Last modified date: #0-4:Day #5-8:Month #9-15:Year(0=1980)
	uint16_t  cluini;		// 0x01A [2]  Initial cluster for this file
	uint32_t  fsize;		// 0x01C [4]  File size in bytes
} DIRENTRY;

//char get_screen_size(void);

int  getchar(void);
int  putchar(int c);
int  cprintf(const char *format, ...);
int  kbhit(void);

char  get_current_drive(void);
char  get_current_directory(char drive, char *path);
char* get_program_path();
char  get_drive_params(char drive, DPARM_info *param);

uint16_t fopen(char *fn, char mode);
uint8_t  fclose(char fp);
uint16_t fcreate(char *fn, char mode, char attributes);
uint16_t remove(char *file);
uint16_t fread(char* buf, uint16_t size, char fp);
uint16_t fwrite(char* buf, uint16_t size, char fp);
uint16_t fputs(char* str, char fp);
uint16_t fgets(char* bug, uint16_t size, uint16_t fh);
uint32_t fseek (char fp, int32_t offset, char origin);
uint32_t filesize(char *fn);
char     fileexists(char *fn);

char dosver(void);
int  parse_pathname(char volume_name_flag, char* s);
void exit(int code);
void exit0();
void explain(char* buffer, char error_code);
char get_env(char* name, char* buffer, char buffer_size);

void set_transfer_address(uint8_t *memaddress);
char read_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec);
char write_abs_sector(uint8_t drv, uint16_t startsec, uint8_t nsec);

// Nextor Only
char get_drive_letter_info(char drive, DRIVE_info *info);
char get_FAT_cluster_info(char drive, uint16_t clusterNumber, CLUSTER_info *info);
char read_abs_sector_drv(uint8_t drv, uint32_t startsec, uint8_t nsec);
char write_abs_sector_drv(uint8_t drv, uint32_t startsec, uint8_t nsec);
void set_drive_lock(uint8_t drive, uint8_t value);
char get_drive_lock(uint8_t drive);
void set_fast_out(uint8_t value);
char get_fast_out();

// Memory mapper
void initializeMapper();
void setMapperPage2(uint8_t page);
void restoreMapperPage2();

#endif //____DOS_H____
