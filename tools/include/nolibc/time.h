/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * time function definitions for NOLIBC
 * Copyright (C) 2017-2022 Willy Tarreau <w@1wt.eu>
 */

/* make sure to include all global symbols */
#include "nolibc.h"

#ifndef _NOLIBC_TIME_H
#define _NOLIBC_TIME_H

#include "std.h"
#include "arch.h"
#include "types.h"
#include "sys.h"

#include <linux/signal.h>
#include <linux/time.h>

#define __nolibc_assert_time64_type(t) \
	__nolibc_static_assert(sizeof(t) == 8)

#define __nolibc_assert_native_time64() \
	__nolibc_assert_time64_type(__kernel_old_time_t)

/*
 * int clock_getres(clockid_t clockid, struct timespec *res);
 * int clock_gettime(clockid_t clockid, struct timespec *tp);
 * int clock_settime(clockid_t clockid, const struct timespec *tp);
 * int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *rqtp,
 *                     struct timespec *rmtp)
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_clock_getres(clockid_t clockid, struct timespec *res)
{
#if defined(__NR_clock_getres_time64)
	__nolibc_assert_time64_type(res->tv_sec);
	return __nolibc_syscall2(__NR_clock_getres_time64, clockid, res);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall2(__NR_clock_getres, clockid, res);
=======
int sys_clock_getres(clockid_t clockid, struct timespec *res)
{
#if defined(__NR_clock_getres_time64)
	__nolibc_assert_time64_type(res->tv_sec);
	return my_syscall2(__NR_clock_getres_time64, clockid, res);
#else
	__nolibc_assert_native_time64();
	return my_syscall2(__NR_clock_getres, clockid, res);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int clock_getres(clockid_t clockid, struct timespec *res)
{
<<<<<<< HEAD
	return __sysret(_sys_clock_getres(clockid, res));
}

static __attribute__((unused))
int _sys_clock_gettime(clockid_t clockid, struct timespec *tp)
{
#if defined(__NR_clock_gettime64)
	__nolibc_assert_time64_type(tp->tv_sec);
	return __nolibc_syscall2(__NR_clock_gettime64, clockid, tp);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall2(__NR_clock_gettime, clockid, tp);
=======
	return __sysret(sys_clock_getres(clockid, res));
}

static __attribute__((unused))
int sys_clock_gettime(clockid_t clockid, struct timespec *tp)
{
#if defined(__NR_clock_gettime64)
	__nolibc_assert_time64_type(tp->tv_sec);
	return my_syscall2(__NR_clock_gettime64, clockid, tp);
#else
	__nolibc_assert_native_time64();
	return my_syscall2(__NR_clock_gettime, clockid, tp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int clock_gettime(clockid_t clockid, struct timespec *tp)
{
<<<<<<< HEAD
	return __sysret(_sys_clock_gettime(clockid, tp));
}

static __attribute__((unused))
int _sys_clock_settime(clockid_t clockid, struct timespec *tp)
{
#if defined(__NR_clock_settime64)
	__nolibc_assert_time64_type(tp->tv_sec);
	return __nolibc_syscall2(__NR_clock_settime64, clockid, tp);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall2(__NR_clock_settime, clockid, tp);
=======
	return __sysret(sys_clock_gettime(clockid, tp));
}

static __attribute__((unused))
int sys_clock_settime(clockid_t clockid, struct timespec *tp)
{
#if defined(__NR_clock_settime64)
	__nolibc_assert_time64_type(tp->tv_sec);
	return my_syscall2(__NR_clock_settime64, clockid, tp);
#else
	__nolibc_assert_native_time64();
	return my_syscall2(__NR_clock_settime, clockid, tp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int clock_settime(clockid_t clockid, struct timespec *tp)
{
<<<<<<< HEAD
	return __sysret(_sys_clock_settime(clockid, tp));
}

static __attribute__((unused))
int _sys_clock_nanosleep(clockid_t clockid, int flags, const struct timespec *rqtp,
			 struct timespec *rmtp)
{
#if defined(__NR_clock_nanosleep_time64)
	__nolibc_assert_time64_type(rqtp->tv_sec);
	return __nolibc_syscall4(__NR_clock_nanosleep_time64, clockid, flags, rqtp, rmtp);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall4(__NR_clock_nanosleep, clockid, flags, rqtp, rmtp);
=======
	return __sysret(sys_clock_settime(clockid, tp));
}

static __attribute__((unused))
int sys_clock_nanosleep(clockid_t clockid, int flags, const struct timespec *rqtp,
			struct timespec *rmtp)
{
#if defined(__NR_clock_nanosleep_time64)
	__nolibc_assert_time64_type(rqtp->tv_sec);
	return my_syscall4(__NR_clock_nanosleep_time64, clockid, flags, rqtp, rmtp);
#else
	__nolibc_assert_native_time64();
	return my_syscall4(__NR_clock_nanosleep, clockid, flags, rqtp, rmtp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *rqtp,
		    struct timespec *rmtp)
{
	/* Directly return a positive error number */
<<<<<<< HEAD
	return -_sys_clock_nanosleep(clockid, flags, rqtp, rmtp);
=======
	return -sys_clock_nanosleep(clockid, flags, rqtp, rmtp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __inline__
double difftime(time_t time1, time_t time2)
{
	return time1 - time2;
}

static __inline__
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
<<<<<<< HEAD
	return __sysret(_sys_clock_nanosleep(CLOCK_REALTIME, 0, rqtp, rmtp));
=======
	return __sysret(sys_clock_nanosleep(CLOCK_REALTIME, 0, rqtp, rmtp));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


static __attribute__((unused))
time_t time(time_t *tptr)
{
	struct timeval tv;

	/* note, cannot fail here */
<<<<<<< HEAD
	_sys_gettimeofday(&tv, NULL);
=======
	sys_gettimeofday(&tv, NULL);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (tptr)
		*tptr = tv.tv_sec;
	return tv.tv_sec;
}


/*
 * int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
 * int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
 * int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid)
{
	return __nolibc_syscall3(__NR_timer_create, clockid, evp, timerid);
=======
int sys_timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid)
{
	return my_syscall3(__NR_timer_create, clockid, evp, timerid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid)
{
<<<<<<< HEAD
	return __sysret(_sys_timer_create(clockid, evp, timerid));
}

static __attribute__((unused))
int _sys_timer_delete(timer_t timerid)
{
	return __nolibc_syscall1(__NR_timer_delete, timerid);
=======
	return __sysret(sys_timer_create(clockid, evp, timerid));
}

static __attribute__((unused))
int sys_timer_delete(timer_t timerid)
{
	return my_syscall1(__NR_timer_delete, timerid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int timer_delete(timer_t timerid)
{
<<<<<<< HEAD
	return __sysret(_sys_timer_delete(timerid));
}

static __attribute__((unused))
int _sys_timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
#if defined(__NR_timer_gettime64)
	__nolibc_assert_time64_type(curr_value->it_value.tv_sec);
	return __nolibc_syscall2(__NR_timer_gettime64, timerid, curr_value);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall2(__NR_timer_gettime, timerid, curr_value);
=======
	return __sysret(sys_timer_delete(timerid));
}

static __attribute__((unused))
int sys_timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
#if defined(__NR_timer_gettime64)
	__nolibc_assert_time64_type(curr_value->it_value.tv_sec);
	return my_syscall2(__NR_timer_gettime64, timerid, curr_value);
#else
	__nolibc_assert_native_time64();
	return my_syscall2(__NR_timer_gettime, timerid, curr_value);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int timer_gettime(timer_t timerid, struct itimerspec *curr_value)
{
<<<<<<< HEAD
	return __sysret(_sys_timer_gettime(timerid, curr_value));
}

static __attribute__((unused))
int _sys_timer_settime(timer_t timerid, int flags,
		       const struct itimerspec *new_value, struct itimerspec *old_value)
{
#if defined(__NR_timer_settime64)
	__nolibc_assert_time64_type(new_value->it_value.tv_sec);
	return __nolibc_syscall4(__NR_timer_settime64, timerid, flags, new_value, old_value);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall4(__NR_timer_settime, timerid, flags, new_value, old_value);
=======
	return __sysret(sys_timer_gettime(timerid, curr_value));
}

static __attribute__((unused))
int sys_timer_settime(timer_t timerid, int flags,
		      const struct itimerspec *new_value, struct itimerspec *old_value)
{
#if defined(__NR_timer_settime64)
	__nolibc_assert_time64_type(new_value->it_value.tv_sec);
	return my_syscall4(__NR_timer_settime64, timerid, flags, new_value, old_value);
#else
	__nolibc_assert_native_time64();
	return my_syscall4(__NR_timer_settime, timerid, flags, new_value, old_value);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int timer_settime(timer_t timerid, int flags,
		  const struct itimerspec *new_value, struct itimerspec *old_value)
{
<<<<<<< HEAD
	return __sysret(_sys_timer_settime(timerid, flags, new_value, old_value));
=======
	return __sysret(sys_timer_settime(timerid, flags, new_value, old_value));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_TIME_H */
