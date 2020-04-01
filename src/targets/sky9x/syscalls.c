
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#undef errno
extern int errno;
extern int _end;
extern int __ram_end__;
unsigned char* heap = NULL;

extern caddr_t _sbrk(int incr);
extern int link(char *old, char *new);
extern int _close(int file);
extern int _fstat(int file, struct stat *st);
extern int _isatty(int file);
extern int _lseek(int file, int ptr, int dir);
extern void _exit(int status);
extern void _kill(int pid, int sig);
extern int _getpid(void);

extern caddr_t _sbrk(int incr)
{
	unsigned char *prev_heap;
	int ramend = (int)&__ram_end__;

	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;

	if (((int)prev_heap + incr) > ramend) {
		return (caddr_t) -1;	
	}

	heap += incr;

	return (caddr_t) prev_heap;
}

extern int link(char *old, char *new)
{
	return -1;
}

extern int _close(int file)
{
	return -1;
}

extern int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;

	return 0;
}

extern int _isatty(int file)
{
	return 1;
}

extern int _lseek(int file, int ptr, int dir)
{
	return 0;
}

extern void _exit(int status)
{
	asm("BKPT #0");
	for (;;);
}

extern void _kill(int pid, int sig)
{
	return;
}

extern int _getpid(void)
{
	return -1;
}

extern int _read(int file, char* ptr, int len)
{
	return 0;
}

extern int _write(int file, char* ptr, int len)
{
	return 0;
}

extern int _gettimeofday(void* p1, void* p2)
{
	return 0;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
