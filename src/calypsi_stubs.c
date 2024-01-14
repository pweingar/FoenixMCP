/* Calypsi's C library requires some basic functions to be implemented. These functions
 * normally rely on the operating system, but as we *are* the operating system implementing
 * these functions, we cannot easily support them. So these are just empty stubs to make the
 * compiler happy.
 */

#include <stubs.h>


int _Stub_open(const char *path, int oflag, ...) {
	return -1;
}

int _Stub_close(int h) {
	return -1;
}

size_t _Stub_read(int h, void *p, size_t l) {
	return -1;
}

	
size_t _Stub_write(int h, const void *p, size_t l) {
	return 0L;
}

void _Stub_exit(int exitCode) __noreturn_function {
}

long _Stub_lseek(int h, long offset, int origin) {
	return -1;
}


