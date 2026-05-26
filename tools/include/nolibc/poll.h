/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * poll definitions for NOLIBC
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
 */

/* make sure to include all global symbols */
#include "nolibc.h"

#ifndef _NOLIBC_POLL_H
#define _NOLIBC_POLL_H

#include "arch.h"
#include "sys.h"

#include <linux/poll.h>
#include <linux/time.h>

/*
 * int poll(struct pollfd *fds, int nfds, int timeout);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_poll(struct pollfd *fds, int nfds, int timeout)
=======
int sys_poll(struct pollfd *fds, int nfds, int timeout)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
#if defined(__NR_ppoll_time64)
	struct __kernel_timespec t;

	if (timeout >= 0) {
		t.tv_sec  = timeout / 1000;
		t.tv_nsec = (timeout % 1000) * 1000000;
	}
<<<<<<< HEAD
	return __nolibc_syscall5(__NR_ppoll_time64, fds, nfds, (timeout >= 0) ? &t : NULL, NULL, 0);
=======
	return my_syscall5(__NR_ppoll_time64, fds, nfds, (timeout >= 0) ? &t : NULL, NULL, 0);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#else
	struct __kernel_old_timespec t;

	if (timeout >= 0) {
		t.tv_sec  = timeout / 1000;
		t.tv_nsec = (timeout % 1000) * 1000000;
	}
<<<<<<< HEAD
	return __nolibc_syscall5(__NR_ppoll, fds, nfds, (timeout >= 0) ? &t : NULL, NULL, 0);
=======
	return my_syscall5(__NR_ppoll, fds, nfds, (timeout >= 0) ? &t : NULL, NULL, 0);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int poll(struct pollfd *fds, int nfds, int timeout)
{
<<<<<<< HEAD
	return __sysret(_sys_poll(fds, nfds, timeout));
=======
	return __sysret(sys_poll(fds, nfds, timeout));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_POLL_H */
