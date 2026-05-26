/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * time definitions for NOLIBC
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
 */

/* make sure to include all global symbols */
#include "../nolibc.h"

#ifndef _NOLIBC_SYS_TIME_H
#define _NOLIBC_SYS_TIME_H

#include "../arch.h"
#include "../sys.h"

<<<<<<< HEAD
static int _sys_clock_gettime(clockid_t clockid, struct timespec *tp);
=======
static int sys_clock_gettime(clockid_t clockid, struct timespec *tp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/*
 * int gettimeofday(struct timeval *tv, struct timezone *tz);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_gettimeofday(struct timeval *tv, struct timezone *tz)
=======
int sys_gettimeofday(struct timeval *tv, struct timezone *tz)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	(void) tz; /* Non-NULL tz is undefined behaviour */

	struct timespec tp;
	int ret;

<<<<<<< HEAD
	ret = _sys_clock_gettime(CLOCK_REALTIME, &tp);
=======
	ret = sys_clock_gettime(CLOCK_REALTIME, &tp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!ret && tv) {
		tv->tv_sec = tp.tv_sec;
		tv->tv_usec = (uint32_t)tp.tv_nsec / 1000;
	}

	return ret;
}

static __attribute__((unused))
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
<<<<<<< HEAD
	return __sysret(_sys_gettimeofday(tv, tz));
=======
	return __sysret(sys_gettimeofday(tv, tz));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_SYS_TIME_H */
