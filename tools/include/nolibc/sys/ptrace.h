/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * ptrace for NOLIBC
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
 * Copyright (C) 2025 Intel Corporation
 */

/* make sure to include all global symbols */
#include "../nolibc.h"

#ifndef _NOLIBC_SYS_PTRACE_H
#define _NOLIBC_SYS_PTRACE_H

#include "../sys.h"

#include <linux/ptrace.h>

/*
 * long ptrace(int op, pid_t pid, void *addr, void *data);
 */
static __attribute__((unused))
<<<<<<< HEAD
long _sys_ptrace(int op, pid_t pid, void *addr, void *data)
{
	return __nolibc_syscall4(__NR_ptrace, op, pid, addr, data);
=======
long sys_ptrace(int op, pid_t pid, void *addr, void *data)
{
	return my_syscall4(__NR_ptrace, op, pid, addr, data);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
ssize_t ptrace(int op, pid_t pid, void *addr, void *data)
{
<<<<<<< HEAD
	return __sysret(_sys_ptrace(op, pid, addr, data));
=======
	return __sysret(sys_ptrace(op, pid, addr, data));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_SYS_PTRACE_H */
