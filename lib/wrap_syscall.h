/**
 * wrap_syscall.h
 *
 */

#ifndef WRAP_SYSCALL_H_
#define WRAP_SYSCALL_H_

#include <errno.h>

#define WRAP_SYSCALL( res, expr ) \
	do{\
		(res) = (expr);\
		if( errno != EINTR ) break;\
	}while(true)

#endif /* WRAP_SYSCALL_H_ */
