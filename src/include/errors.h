/**
 * Definition of error numbers
 */

#ifndef __ERRORS_H
#define __ERRORS_H

// #define MAX_ERROR_NUMBER    16      // Largest (absolute value) of the error number

#define ERR_GENERAL                     -1  // A general error condition
#define DEV_ERR_BADDEV                  -2  // Device number is bad (too big or no device assigned)
#define DEV_TIMEOUT                     -3  // The block device function timed out
#define DEV_CANNOT_INIT                 -4  // The block device could not initilize
#define DEV_CANNOT_READ                 -5  // The device cannot complete the READ
#define DEV_CANNOT_WRITE                -6  // The device cannot complete the WRITE
#define DEV_BOUNDS_ERR                  -7  // The buffer provided is not big enough
#define DEV_NOMEDIA                     -8  // There is no media present for that device
#define DEV_WRITEPROT                   -9  // The media is write protected
#define ERR_BADCHANNEL                  -10 // The channel number was bad
#define ERR_OUT_OF_HANDLES              -11 // There are no available handles for channels files etc.
#define ERR_BAD_HANDLE                  -12 // The handle passed was not valid
#define ERR_BAD_EXTENSION               -13 // The path to load did not have an extension that matched registered loaders
#define ERR_OUT_OF_MEMORY               -14 // Unable to allocate more memory
#define ERR_BAD_BINARY                  -15 // Bad binary file format... i.e. binary format does not match expectations
#define ERR_NOT_EXECUTABLE              -16 // Binary file format does not have a starting address
#define ERR_NOT_FOUND                   -17 // Resource not found

#define FSYS_ERR_DISK_ERR               -18 /* (1) A hard error occurred in the low level disk I/O layer */
#define FSYS_ERR_INT_ERR                -19 /* (2) Assertion failed */
#define FSYS_ERR_NOT_READY              -20 /* (3) The physical drive cannot work */
#define FSYS_ERR_NO_FILE                -21 /* (4) Could not find the file */
#define FSYS_ERR_NO_PATH                -22 /* (5) Could not find the path */
#define FSYS_ERR_INVALID_NAME		    -23 /* (6) The path name format is invalid */
#define FSYS_ERR_DENIED				    -24 /* (7) Access denied due to prohibited access or directory full */
#define FSYS_ERR_EXIST				    -25 /* (8) Access denied due to prohibited access */
#define FSYS_ERR_INVALID_OBJECT		    -26 /* (9) The file/directory object is invalid */
#define FSYS_ERR_WRITE_PROTECTED	    -27 /* (10) The physical drive is write protected */
#define FSYS_ERR_INVALID_DRIVE		    -28 /* (11) The logical drive number is invalid */
#define FSYS_ERR_NOT_ENABLED		    -29	/* (12) The volume has no work area */
#define FSYS_ERR_NO_FILESYSTEM		    -30 /* (13) There is no valid FAT volume */
#define FSYS_ERR_MKFS_ABORTED		    -31 /* (14) The f_mkfs() aborted due to any problem */
#define FSYS_ERR_TIMEOUT			    -32	/* (15) Could not get a grant to access the volume within defined period */
#define FSYS_ERR_LOCKED				    -33 /* (16) The operation is rejected according to the file sharing policy */
#define FSYS_ERR_NOT_ENOUGH_CORE	    -34 /* (17) LFN working buffer could not be allocated */
#define FSYS_ERR_TOO_MANY_OPEN_FILES    -35 /* (18) Number of open files > FF_FS_LOCK */
#define FSYS_ERR_INVALID_PARAMETER      -36 /* (19) Given parameter is invalid */

#define ERR_NOT_SUPPORTED               -37 /* Device does not support the file or operation */
#define ERR_BAD_ARGUMENT                -38 /* An invalid argument was provided */

#endif
