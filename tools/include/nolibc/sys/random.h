/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * random definitions for NOLIBC
 * Copyright (C) 2025 Thomas Weißschuh <thomas.weissschuh@linutronix.de>
 */

/* make sure to include all global symbols */
#include "../nolibc.h"

#ifndef _NOLIBC_SYS_RANDOM_H
#define _NOLIBC_SYS_RANDOM_H

#include "../arch.h"
#include "../sys.h"

#include <linux/random.h>

/*
 * ssize_t getrandom(void *buf, size_t buflen, unsigned int flags);
 */

static __attribute__((unused))
<<<<<<< HEAD
ssize_t _sys_getrandom(void *buf, size_t buflen, unsigned int flags)
{
	return __nolibc_syscall3(__NR_getrandom, buf, buflen, flags);
=======
ssize_t sys_getrandom(void *buf, size_t buflen, unsigned int flags)
{
	return my_syscall3(__NR_getrandom, buf, buflen, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
ssize_t getrandom(void *buf, size_t buflen, unsigned int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_getrandom(buf, buflen, flags));
=======
	return __sysret(sys_getrandom(buf, buflen, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_SYS_RANDOM_H */
