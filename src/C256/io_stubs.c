#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscalls.h"

/****************************************************************************
* Name: _Stub_open
*
* Description:
* Open a file.
* The oflag argument are POSIX style mode flags, e.g O_RDONLY which
* are defined in fcntl.h.
* This function is variadic as it optionally can take a mode_t that
* are permissions, e.g 0666. If the file system does not handle
* permissions you can ignore that this function is variadic.
* The return file descriptor shall be a positive number, larger
* than 2 (as 0-2 are used for stdin, stdout and stderr).
* The actual number does not matter and they need not to be
* consequtive, multiple numeric series with gaps between can be used.
*
* Return the obtained file descriptor or EOF (-1) on failure and set
* errno according to the error.
*
****************************************************************************/
int _Stub_open(const char *path, int oflag, ...) {
	return -1;
}

/****************************************************************************
* Name: _Stub_close
*
* Description:
* Close a file
*
* Return 0 if operation was OK, EOF otherwise and set errno according to
* the error.
* Note: This will only be invoked for streams opened by _Stub_open(),
* there is no need to check for the standard descriptor 0-2.
*
****************************************************************************/
int _Stub_close(int fd) {
	return 0;
}

/****************************************************************************
* Name: _Stub_access
*
* Description:
* Determine if a file can be accessed.
*
****************************************************************************/
int _Stub_access(const char *path, int mode) {
	return 0;
}

/****************************************************************************
* Name: _Stub_lseek
*
* Description:
* Change position in a file
*
* Returns the new position in the file in bytes from the beginning of the
* file, or -1 on failure.
*
****************************************************************************/
long _Stub_lseek(int fd, long offset, int whence) {
	return 0;
}

/****************************************************************************
* Name: _Stub_fgetpos
*
* Description:
* Change position in a file
*
* Returns 0 on success, non-zero otherwise. In case of an error also set
* errno according to error.
*
****************************************************************************/
int _Stub_fgetpos(int fd, fpos_t *pos) {
	return 0;
}

/****************************************************************************
* Name: _Stub_fsetpos
*
* Description:
* Change position in a file
*
* Returns 0 on success, non-zero otherwise. In case of an error also set
* errno according to error.
*
****************************************************************************/
int _Stub_fsetpos(int fd, const fpos_t *pos) {
	return 0;
}

/****************************************************************************
* Name: _Stub_read
*
* Description:
* Read from a file
*
* Returns the number of characters read. Return -1 on failure and set
* errno according to the error.
*
****************************************************************************/
size_t _Stub_read(int fd, void *buf, size_t count) {
	return 0;
}

/****************************************************************************
* Name: _Stub_write
*
* Description:
* Write to a file
*
* Returns the number of characters actually written. Return -1 on failure and
* set errno according to the error.
*
****************************************************************************/
size_t _Stub_write(int fd, const void *buf, size_t count) {
	short channel = 0;

	switch (fd) {
		case 0:
			channel = 0;
			break;

		case 1:
			channel = 0;
			break;

		case 2:
			channel = 1;
			break;

		default:
			channel = 0;
			break;
	}

	for (int i = 0; i < count; i++) {
		sys_txt_put(channel, ((const char *)buf)[i]);
	}

	return count;
}

/****************************************************************************
* Name: _Stub_rename
*
* Description:
* Rename a file or directory
*
* Return 0 on success, -1 otherwise and set errno according to the
* error.
*
****************************************************************************/
int _Stub_rename(const char *oldpath, const char *newpath) {
	return 0;
}

/****************************************************************************
* Name: _Stub_remove
*
* Description:
* Remove a file or directory
*
* Return 0 on success, -1 otherwise and set errno according to the
* error.
*
****************************************************************************/
int _Stub_remove(const char *path) {
	return 0;
}

/****************************************************************************
* Name: _Stub_exit
*
* Description:
* Terminate the program with an exit code, exit clean-ups are done
* before this function is (finally) called.
*
****************************************************************************/
void _Stub_exit(int exitCode) {
	while (1) ;
}

/****************************************************************************
* Name: _Stub_environ
*
* Description:
* Get the environment. On UNIX this is typically a global variable
* 'environ', but in order to make it more flexible and avoid having
* such global variable (which is not part of the C standard) it is
* obtained using the stub interface.
*
* Note:
* This stub function is not implemented by the semi-hosted debug stub
* interface.
*
****************************************************************************/
char** _Stub_environ(void) {
	return 0;
}







