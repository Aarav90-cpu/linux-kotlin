/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * timerfd definitions for NOLIBC
 * Copyright (C) 2025 Thomas Weißschuh <thomas.weissschuh@linutronix.de>
 */

/* make sure to include all global symbols */
#include "../nolibc.h"

#ifndef _NOLIBC_SYS_TIMERFD_H
#define _NOLIBC_SYS_TIMERFD_H

#include "../sys.h"
#include "../time.h"

#include <linux/timerfd.h>


static __attribute__((unused))
<<<<<<< HEAD
int _sys_timerfd_create(int clockid, int flags)
{
	return __nolibc_syscall2(__NR_timerfd_create, clockid, flags);
=======
int sys_timerfd_create(int clockid, int flags)
{
	return my_syscall2(__NR_timerfd_create, clockid, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int timerfd_create(int clockid, int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_timerfd_create(clockid, flags));
=======
	return __sysret(sys_timerfd_create(clockid, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


static __attribute__((unused))
<<<<<<< HEAD
int _sys_timerfd_gettime(int fd, struct itimerspec *curr_value)
{
#if defined(__NR_timerfd_gettime64)
	__nolibc_assert_time64_type(curr_value->it_value.tv_sec);
	return __nolibc_syscall2(__NR_timerfd_gettime64, fd, curr_value);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall2(__NR_timerfd_gettime, fd, curr_value);
=======
int sys_timerfd_gettime(int fd, struct itimerspec *curr_value)
{
#if defined(__NR_timerfd_gettime64)
	__nolibc_assert_time64_type(curr_value->it_value.tv_sec);
	return my_syscall2(__NR_timerfd_gettime64, fd, curr_value);
#else
	__nolibc_assert_native_time64();
	return my_syscall2(__NR_timerfd_gettime, fd, curr_value);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int timerfd_gettime(int fd, struct itimerspec *curr_value)
{
<<<<<<< HEAD
	return __sysret(_sys_timerfd_gettime(fd, curr_value));
=======
	return __sysret(sys_timerfd_gettime(fd, curr_value));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


static __attribute__((unused))
<<<<<<< HEAD
int _sys_timerfd_settime(int fd, int flags,
			 const struct itimerspec *new_value, struct itimerspec *old_value)
{
#if defined(__NR_timerfd_settime64)
	__nolibc_assert_time64_type(new_value->it_value.tv_sec);
	return __nolibc_syscall4(__NR_timerfd_settime64, fd, flags, new_value, old_value);
#else
	__nolibc_assert_native_time64();
	return __nolibc_syscall4(__NR_timerfd_settime, fd, flags, new_value, old_value);
=======
int sys_timerfd_settime(int fd, int flags,
			const struct itimerspec *new_value, struct itimerspec *old_value)
{
#if defined(__NR_timerfd_settime64)
	__nolibc_assert_time64_type(new_value->it_value.tv_sec);
	return my_syscall4(__NR_timerfd_settime64, fd, flags, new_value, old_value);
#else
	__nolibc_assert_native_time64();
	return my_syscall4(__NR_timerfd_settime, fd, flags, new_value, old_value);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int timerfd_settime(int fd, int flags,
		    const struct itimerspec *new_value, struct itimerspec *old_value)
{
<<<<<<< HEAD
	return __sysret(_sys_timerfd_settime(fd, flags, new_value, old_value));
=======
	return __sysret(sys_timerfd_settime(fd, flags, new_value, old_value));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_SYS_TIMERFD_H */
