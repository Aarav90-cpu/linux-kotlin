/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * Syscall definitions for NOLIBC (those in man(2))
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
 */

/* make sure to include all global symbols */
#include "nolibc.h"

#ifndef _NOLIBC_SYS_H
#define _NOLIBC_SYS_H

#include "std.h"

/* system includes */
#include <linux/unistd.h>
#include <linux/signal.h>  /* for SIGCHLD */
#include <linux/termios.h>
#include <linux/mman.h>
#include <linux/fs.h>
#include <linux/loop.h>
#include <linux/time.h>
#include <linux/auxvec.h>
#include <linux/fcntl.h> /* for O_* and AT_* */
#include <linux/sched.h> /* for CLONE_* */
#include <linux/stat.h>  /* for statx() */

#include "errno.h"
#include "stdarg.h"
#include "types.h"


/* Syscall return helper: takes the syscall value in argument and checks for an
 * error in it. This may only be used with signed returns (int or long), but
 * not with pointers. An error is any value < 0. When an error is encountered,
 * -ret is set into errno and -1 is returned. Otherwise the returned value is
 * passed as-is with its type preserved.
 */

#define __sysret(arg)							\
({									\
	__typeof__(arg) __sysret_arg = (arg);				\
	(__sysret_arg < 0)                              /* error ? */	\
		? (({ SET_ERRNO(-__sysret_arg); }), -1) /* ret -1 with errno = -arg */ \
		: __sysret_arg;                         /* return original value */ \
})

/* Syscall ENOSYS helper: Avoids unused-parameter warnings and provides a
 * debugging hook.
 */

static __inline__ int __nolibc_enosys(const char *syscall, ...)
{
	(void)syscall;
	return -ENOSYS;
}


/* Functions in this file only describe syscalls. They're declared static so
 * that the compiler usually decides to inline them while still being allowed
 * to pass a pointer to one of their instances. Each syscall exists in two
 * versions:
 *   - the "internal" ones, which matches the raw syscall interface at the
 *     kernel level, which may sometimes slightly differ from the documented
 *     libc-level ones. For example most of them return either a valid value
<<<<<<< HEAD
 *     or -errno. All of these are prefixed with "_sys_". They may be called
=======
 *     or -errno. All of these are prefixed with "sys_". They may be called
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 *     by non-portable applications if desired.
 *
 *   - the "exported" ones, whose interface must closely match the one
 *     documented in man(2), that applications are supposed to expect. These
 *     ones rely on the internal ones, and set errno.
 *
 * Each syscall will be defined with the two functions, sorted in alphabetical
 * order applied to the exported names.
 *
 * In case of doubt about the relevance of a function here, only those which
 * set errno should be defined here. Wrappers like those appearing in man(3)
 * should not be placed here.
 */


/*
 * int brk(void *addr);
 * void *sbrk(intptr_t inc)
 */

static __attribute__((unused))
<<<<<<< HEAD
void *_sys_brk(void *addr)
{
	return (void *)__nolibc_syscall1(__NR_brk, addr);
=======
void *sys_brk(void *addr)
{
	return (void *)my_syscall1(__NR_brk, addr);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int brk(void *addr)
{
<<<<<<< HEAD
	void *ret = _sys_brk(addr);
=======
	void *ret = sys_brk(addr);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (!ret) {
		SET_ERRNO(ENOMEM);
		return -1;
	}
	return 0;
}

static __attribute__((unused))
void *sbrk(intptr_t inc)
{
	/* first call to find current end */
<<<<<<< HEAD
	void *ret = _sys_brk(NULL);

	if (ret && _sys_brk(ret + inc) == ret + inc)
=======
	void *ret = sys_brk(NULL);

	if (ret && sys_brk(ret + inc) == ret + inc)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		return ret + inc;

	SET_ERRNO(ENOMEM);
	return (void *)-1;
}


/*
 * int chdir(const char *path);
 * int fchdir(int fildes);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_chdir(const char *path)
{
	return __nolibc_syscall1(__NR_chdir, path);
=======
int sys_chdir(const char *path)
{
	return my_syscall1(__NR_chdir, path);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int chdir(const char *path)
{
<<<<<<< HEAD
	return __sysret(_sys_chdir(path));
}

static __attribute__((unused))
int _sys_fchdir(int fildes)
{
	return __nolibc_syscall1(__NR_fchdir, fildes);
=======
	return __sysret(sys_chdir(path));
}

static __attribute__((unused))
int sys_fchdir(int fildes)
{
	return my_syscall1(__NR_fchdir, fildes);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int fchdir(int fildes)
{
<<<<<<< HEAD
	return __sysret(_sys_fchdir(fildes));
=======
	return __sysret(sys_fchdir(fildes));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int chmod(const char *path, mode_t mode);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_chmod(const char *path, mode_t mode)
{
#if defined(__NR_fchmodat)
	return __nolibc_syscall4(__NR_fchmodat, AT_FDCWD, path, mode, 0);
#else
	return __nolibc_syscall2(__NR_chmod, path, mode);
=======
int sys_chmod(const char *path, mode_t mode)
{
#if defined(__NR_fchmodat)
	return my_syscall4(__NR_fchmodat, AT_FDCWD, path, mode, 0);
#else
	return my_syscall2(__NR_chmod, path, mode);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int chmod(const char *path, mode_t mode)
{
<<<<<<< HEAD
	return __sysret(_sys_chmod(path, mode));
=======
	return __sysret(sys_chmod(path, mode));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int chown(const char *path, uid_t owner, gid_t group);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_chown(const char *path, uid_t owner, gid_t group)
{
#if defined(__NR_fchownat)
	return __nolibc_syscall5(__NR_fchownat, AT_FDCWD, path, owner, group, 0);
#else
	return __nolibc_syscall3(__NR_chown, path, owner, group);
=======
int sys_chown(const char *path, uid_t owner, gid_t group)
{
#if defined(__NR_fchownat)
	return my_syscall5(__NR_fchownat, AT_FDCWD, path, owner, group, 0);
#else
	return my_syscall3(__NR_chown, path, owner, group);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int chown(const char *path, uid_t owner, gid_t group)
{
<<<<<<< HEAD
	return __sysret(_sys_chown(path, owner, group));
=======
	return __sysret(sys_chown(path, owner, group));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int chroot(const char *path);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_chroot(const char *path)
{
	return __nolibc_syscall1(__NR_chroot, path);
=======
int sys_chroot(const char *path)
{
	return my_syscall1(__NR_chroot, path);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int chroot(const char *path)
{
<<<<<<< HEAD
	return __sysret(_sys_chroot(path));
=======
	return __sysret(sys_chroot(path));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int close(int fd);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_close(int fd)
{
	return __nolibc_syscall1(__NR_close, fd);
=======
int sys_close(int fd)
{
	return my_syscall1(__NR_close, fd);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int close(int fd)
{
<<<<<<< HEAD
	return __sysret(_sys_close(fd));
=======
	return __sysret(sys_close(fd));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int dup(int fd);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_dup(int fd)
{
	return __nolibc_syscall1(__NR_dup, fd);
=======
int sys_dup(int fd)
{
	return my_syscall1(__NR_dup, fd);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int dup(int fd)
{
<<<<<<< HEAD
	return __sysret(_sys_dup(fd));
=======
	return __sysret(sys_dup(fd));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int dup2(int old, int new);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_dup2(int old, int new)
=======
int sys_dup2(int old, int new)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
#if defined(__NR_dup3)
	int ret, nr_fcntl;

#ifdef __NR_fcntl64
	nr_fcntl = __NR_fcntl64;
#else
	nr_fcntl = __NR_fcntl;
#endif

	if (old == new) {
<<<<<<< HEAD
		ret = __nolibc_syscall2(nr_fcntl, old, F_GETFD);
		return ret < 0 ? ret : old;
	}

	return __nolibc_syscall3(__NR_dup3, old, new, 0);
#else
	return __nolibc_syscall2(__NR_dup2, old, new);
=======
		ret = my_syscall2(nr_fcntl, old, F_GETFD);
		return ret < 0 ? ret : old;
	}

	return my_syscall3(__NR_dup3, old, new, 0);
#else
	return my_syscall2(__NR_dup2, old, new);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int dup2(int old, int new)
{
<<<<<<< HEAD
	return __sysret(_sys_dup2(old, new));
=======
	return __sysret(sys_dup2(old, new));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int dup3(int old, int new, int flags);
 */

#if defined(__NR_dup3)
static __attribute__((unused))
<<<<<<< HEAD
int _sys_dup3(int old, int new, int flags)
{
	return __nolibc_syscall3(__NR_dup3, old, new, flags);
=======
int sys_dup3(int old, int new, int flags)
{
	return my_syscall3(__NR_dup3, old, new, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int dup3(int old, int new, int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_dup3(old, new, flags));
=======
	return __sysret(sys_dup3(old, new, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
#endif


/*
 * int execve(const char *filename, char *const argv[], char *const envp[]);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_execve(const char *filename, char *const argv[], char *const envp[])
{
	return __nolibc_syscall3(__NR_execve, filename, argv, envp);
=======
int sys_execve(const char *filename, char *const argv[], char *const envp[])
{
	return my_syscall3(__NR_execve, filename, argv, envp);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int execve(const char *filename, char *const argv[], char *const envp[])
{
<<<<<<< HEAD
	return __sysret(_sys_execve(filename, argv, envp));
=======
	return __sysret(sys_execve(filename, argv, envp));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * void exit(int status);
 */

static __attribute__((noreturn,unused))
<<<<<<< HEAD
void _sys_exit(int status)
{
	__nolibc_syscall1(__NR_exit, status & 255);
=======
void sys_exit(int status)
{
	my_syscall1(__NR_exit, status & 255);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	while(1); /* shut the "noreturn" warnings. */
}

static __attribute__((noreturn,unused))
void _exit(int status)
{
<<<<<<< HEAD
	_sys_exit(status);
=======
	sys_exit(status);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((noreturn,unused))
void exit(int status)
{
	_exit(status);
}


/*
 * pid_t fork(void);
 */

<<<<<<< HEAD
#ifndef _sys_fork
static __attribute__((unused))
pid_t _sys_fork(void)
=======
#ifndef sys_fork
static __attribute__((unused))
pid_t sys_fork(void)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
#if defined(__NR_clone)
	/* note: some archs only have clone() and not fork(). Different archs
	 * have a different API, but most archs have the flags on first arg and
	 * will not use the rest with no other flag.
	 */
<<<<<<< HEAD
	return __nolibc_syscall5(__NR_clone, SIGCHLD, 0, 0, 0, 0);
#else
	return __nolibc_syscall0(__NR_fork);
=======
	return my_syscall5(__NR_clone, SIGCHLD, 0, 0, 0, 0);
#else
	return my_syscall0(__NR_fork);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}
#endif

static __attribute__((unused))
pid_t fork(void)
{
<<<<<<< HEAD
	return __sysret(_sys_fork());
}

#ifndef _sys_vfork
static __attribute__((unused))
pid_t _sys_vfork(void)
{
#if defined(__NR_clone)
	/* See the note in _sys_fork(). */
	return __nolibc_syscall5(__NR_clone, CLONE_VM | CLONE_VFORK | SIGCHLD, 0, 0, 0, 0);
#elif defined(__NR_vfork)
	return __nolibc_syscall0(__NR_vfork);
=======
	return __sysret(sys_fork());
}

#ifndef sys_vfork
static __attribute__((unused))
pid_t sys_vfork(void)
{
#if defined(__NR_clone)
	/* See the note in sys_fork(). */
	return my_syscall5(__NR_clone, CLONE_VM | CLONE_VFORK | SIGCHLD, 0, 0, 0, 0);
#elif defined(__NR_vfork)
	return my_syscall0(__NR_vfork);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}
#endif

static __attribute__((unused))
pid_t vfork(void)
{
<<<<<<< HEAD
	return __sysret(_sys_vfork());
=======
	return __sysret(sys_vfork());
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * int fsync(int fd);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_fsync(int fd)
{
	return __nolibc_syscall1(__NR_fsync, fd);
=======
int sys_fsync(int fd)
{
	return my_syscall1(__NR_fsync, fd);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int fsync(int fd)
{
<<<<<<< HEAD
	return __sysret(_sys_fsync(fd));
=======
	return __sysret(sys_fsync(fd));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int getdents64(int fd, struct linux_dirent64 *dirp, int count);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_getdents64(int fd, struct linux_dirent64 *dirp, int count)
{
	return __nolibc_syscall3(__NR_getdents64, fd, dirp, count);
=======
int sys_getdents64(int fd, struct linux_dirent64 *dirp, int count)
{
	return my_syscall3(__NR_getdents64, fd, dirp, count);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int getdents64(int fd, struct linux_dirent64 *dirp, int count)
{
<<<<<<< HEAD
	return __sysret(_sys_getdents64(fd, dirp, count));
=======
	return __sysret(sys_getdents64(fd, dirp, count));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * uid_t geteuid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
uid_t _sys_geteuid(void)
{
#if defined(__NR_geteuid32)
	return __nolibc_syscall0(__NR_geteuid32);
#else
	return __nolibc_syscall0(__NR_geteuid);
=======
uid_t sys_geteuid(void)
{
#if defined(__NR_geteuid32)
	return my_syscall0(__NR_geteuid32);
#else
	return my_syscall0(__NR_geteuid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
uid_t geteuid(void)
{
<<<<<<< HEAD
	return _sys_geteuid();
=======
	return sys_geteuid();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * pid_t getpgid(pid_t pid);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_getpgid(pid_t pid)
{
	return __nolibc_syscall1(__NR_getpgid, pid);
=======
pid_t sys_getpgid(pid_t pid)
{
	return my_syscall1(__NR_getpgid, pid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t getpgid(pid_t pid)
{
<<<<<<< HEAD
	return __sysret(_sys_getpgid(pid));
=======
	return __sysret(sys_getpgid(pid));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * pid_t getpgrp(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_getpgrp(void)
{
	return _sys_getpgid(0);
=======
pid_t sys_getpgrp(void)
{
	return sys_getpgid(0);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t getpgrp(void)
{
<<<<<<< HEAD
	return _sys_getpgrp();
=======
	return sys_getpgrp();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * pid_t getpid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_getpid(void)
{
	return __nolibc_syscall0(__NR_getpid);
=======
pid_t sys_getpid(void)
{
	return my_syscall0(__NR_getpid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t getpid(void)
{
<<<<<<< HEAD
	return _sys_getpid();
=======
	return sys_getpid();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * pid_t getppid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_getppid(void)
{
	return __nolibc_syscall0(__NR_getppid);
=======
pid_t sys_getppid(void)
{
	return my_syscall0(__NR_getppid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t getppid(void)
{
<<<<<<< HEAD
	return _sys_getppid();
=======
	return sys_getppid();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * pid_t gettid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_gettid(void)
{
	return __nolibc_syscall0(__NR_gettid);
=======
pid_t sys_gettid(void)
{
	return my_syscall0(__NR_gettid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t gettid(void)
{
<<<<<<< HEAD
	return _sys_gettid();
=======
	return sys_gettid();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#ifndef NOLIBC_NO_RUNTIME
static unsigned long getauxval(unsigned long key);

/*
 * int getpagesize(void);
 */

static __attribute__((unused))
int getpagesize(void)
{
	return __sysret((int)getauxval(AT_PAGESZ) ?: -ENOENT);
}
#endif /* NOLIBC_NO_RUNTIME */

/*
 * uid_t getuid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
uid_t _sys_getuid(void)
{
#if defined(__NR_getuid32)
	return __nolibc_syscall0(__NR_getuid32);
#else
	return __nolibc_syscall0(__NR_getuid);
=======
uid_t sys_getuid(void)
{
#if defined(__NR_getuid32)
	return my_syscall0(__NR_getuid32);
#else
	return my_syscall0(__NR_getuid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
uid_t getuid(void)
{
<<<<<<< HEAD
	return _sys_getuid();
=======
	return sys_getuid();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int kill(pid_t pid, int signal);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_kill(pid_t pid, int signal)
{
	return __nolibc_syscall2(__NR_kill, pid, signal);
=======
int sys_kill(pid_t pid, int signal)
{
	return my_syscall2(__NR_kill, pid, signal);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int kill(pid_t pid, int signal)
{
<<<<<<< HEAD
	return __sysret(_sys_kill(pid, signal));
=======
	return __sysret(sys_kill(pid, signal));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int link(const char *old, const char *new);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_link(const char *old, const char *new)
{
#if defined(__NR_linkat)
	return __nolibc_syscall5(__NR_linkat, AT_FDCWD, old, AT_FDCWD, new, 0);
#else
	return __nolibc_syscall2(__NR_link, old, new);
=======
int sys_link(const char *old, const char *new)
{
#if defined(__NR_linkat)
	return my_syscall5(__NR_linkat, AT_FDCWD, old, AT_FDCWD, new, 0);
#else
	return my_syscall2(__NR_link, old, new);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int link(const char *old, const char *new)
{
<<<<<<< HEAD
	return __sysret(_sys_link(old, new));
=======
	return __sysret(sys_link(old, new));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * off_t lseek(int fd, off_t offset, int whence);
 */

static __attribute__((unused))
<<<<<<< HEAD
off_t _sys_lseek(int fd, off_t offset, int whence)
=======
off_t sys_lseek(int fd, off_t offset, int whence)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
#if defined(__NR_llseek)
	__kernel_loff_t loff = 0;
	off_t result;
	int ret;

<<<<<<< HEAD
	ret = __nolibc_syscall5(__NR_llseek, fd, offset >> 32, (uint32_t)offset, &loff, whence);
=======
	ret = my_syscall5(__NR_llseek, fd, offset >> 32, (uint32_t)offset, &loff, whence);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (ret < 0)
		result = ret;
	else
		result = loff;

	return result;
#else
<<<<<<< HEAD
	return __nolibc_syscall3(__NR_lseek, fd, offset, whence);
=======
	return my_syscall3(__NR_lseek, fd, offset, whence);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
off_t lseek(int fd, off_t offset, int whence)
{
<<<<<<< HEAD
	return __sysret(_sys_lseek(fd, offset, whence));
=======
	return __sysret(sys_lseek(fd, offset, whence));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int mkdir(const char *path, mode_t mode);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_mkdir(const char *path, mode_t mode)
{
#if defined(__NR_mkdirat)
	return __nolibc_syscall3(__NR_mkdirat, AT_FDCWD, path, mode);
#else
	return __nolibc_syscall2(__NR_mkdir, path, mode);
=======
int sys_mkdir(const char *path, mode_t mode)
{
#if defined(__NR_mkdirat)
	return my_syscall3(__NR_mkdirat, AT_FDCWD, path, mode);
#else
	return my_syscall2(__NR_mkdir, path, mode);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int mkdir(const char *path, mode_t mode)
{
<<<<<<< HEAD
	return __sysret(_sys_mkdir(path, mode));
=======
	return __sysret(sys_mkdir(path, mode));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * int rmdir(const char *path);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_rmdir(const char *path)
{
#if defined(__NR_rmdir)
	return __nolibc_syscall1(__NR_rmdir, path);
#else
	return __nolibc_syscall3(__NR_unlinkat, AT_FDCWD, path, AT_REMOVEDIR);
=======
int sys_rmdir(const char *path)
{
#if defined(__NR_rmdir)
	return my_syscall1(__NR_rmdir, path);
#else
	return my_syscall3(__NR_unlinkat, AT_FDCWD, path, AT_REMOVEDIR);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int rmdir(const char *path)
{
<<<<<<< HEAD
	return __sysret(_sys_rmdir(path));
=======
	return __sysret(sys_rmdir(path));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int mknod(const char *path, mode_t mode, dev_t dev);
 */

static __attribute__((unused))
<<<<<<< HEAD
long _sys_mknod(const char *path, mode_t mode, dev_t dev)
{
#if defined(__NR_mknodat)
	return __nolibc_syscall4(__NR_mknodat, AT_FDCWD, path, mode, dev);
#else
	return __nolibc_syscall3(__NR_mknod, path, mode, dev);
=======
long sys_mknod(const char *path, mode_t mode, dev_t dev)
{
#if defined(__NR_mknodat)
	return my_syscall4(__NR_mknodat, AT_FDCWD, path, mode, dev);
#else
	return my_syscall3(__NR_mknod, path, mode, dev);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int mknod(const char *path, mode_t mode, dev_t dev)
{
<<<<<<< HEAD
	return __sysret(_sys_mknod(path, mode, dev));
=======
	return __sysret(sys_mknod(path, mode, dev));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int pipe2(int pipefd[2], int flags);
 * int pipe(int pipefd[2]);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_pipe2(int pipefd[2], int flags)
{
	return __nolibc_syscall2(__NR_pipe2, pipefd, flags);
=======
int sys_pipe2(int pipefd[2], int flags)
{
	return my_syscall2(__NR_pipe2, pipefd, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int pipe2(int pipefd[2], int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_pipe2(pipefd, flags));
=======
	return __sysret(sys_pipe2(pipefd, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int pipe(int pipefd[2])
{
	return pipe2(pipefd, 0);
}


/*
 * int pivot_root(const char *new, const char *old);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_pivot_root(const char *new, const char *old)
{
	return __nolibc_syscall2(__NR_pivot_root, new, old);
=======
int sys_pivot_root(const char *new, const char *old)
{
	return my_syscall2(__NR_pivot_root, new, old);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int pivot_root(const char *new, const char *old)
{
<<<<<<< HEAD
	return __sysret(_sys_pivot_root(new, old));
=======
	return __sysret(sys_pivot_root(new, old));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * ssize_t read(int fd, void *buf, size_t count);
 */

static __attribute__((unused))
<<<<<<< HEAD
ssize_t _sys_read(int fd, void *buf, size_t count)
{
	return __nolibc_syscall3(__NR_read, fd, buf, count);
=======
ssize_t sys_read(int fd, void *buf, size_t count)
{
	return my_syscall3(__NR_read, fd, buf, count);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
ssize_t read(int fd, void *buf, size_t count)
{
<<<<<<< HEAD
	return __sysret(_sys_read(fd, buf, count));
=======
	return __sysret(sys_read(fd, buf, count));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int sched_yield(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_sched_yield(void)
{
	return __nolibc_syscall0(__NR_sched_yield);
=======
int sys_sched_yield(void)
{
	return my_syscall0(__NR_sched_yield);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int sched_yield(void)
{
<<<<<<< HEAD
	return __sysret(_sys_sched_yield());
=======
	return __sysret(sys_sched_yield());
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int setpgid(pid_t pid, pid_t pgid);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_setpgid(pid_t pid, pid_t pgid)
{
	return __nolibc_syscall2(__NR_setpgid, pid, pgid);
=======
int sys_setpgid(pid_t pid, pid_t pgid)
{
	return my_syscall2(__NR_setpgid, pid, pgid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int setpgid(pid_t pid, pid_t pgid)
{
<<<<<<< HEAD
	return __sysret(_sys_setpgid(pid, pgid));
=======
	return __sysret(sys_setpgid(pid, pgid));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * pid_t setpgrp(void)
 */

static __attribute__((unused))
pid_t setpgrp(void)
{
	return setpgid(0, 0);
}


/*
 * pid_t setsid(void);
 */

static __attribute__((unused))
<<<<<<< HEAD
pid_t _sys_setsid(void)
{
	return __nolibc_syscall0(__NR_setsid);
=======
pid_t sys_setsid(void)
{
	return my_syscall0(__NR_setsid);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
pid_t setsid(void)
{
<<<<<<< HEAD
	return __sysret(_sys_setsid());
=======
	return __sysret(sys_setsid());
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int symlink(const char *old, const char *new);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_symlink(const char *old, const char *new)
{
#if defined(__NR_symlinkat)
	return __nolibc_syscall3(__NR_symlinkat, old, AT_FDCWD, new);
#else
	return __nolibc_syscall2(__NR_symlink, old, new);
=======
int sys_symlink(const char *old, const char *new)
{
#if defined(__NR_symlinkat)
	return my_syscall3(__NR_symlinkat, old, AT_FDCWD, new);
#else
	return my_syscall2(__NR_symlink, old, new);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int symlink(const char *old, const char *new)
{
<<<<<<< HEAD
	return __sysret(_sys_symlink(old, new));
=======
	return __sysret(sys_symlink(old, new));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * mode_t umask(mode_t mode);
 */

static __attribute__((unused))
<<<<<<< HEAD
mode_t _sys_umask(mode_t mode)
{
	return __nolibc_syscall1(__NR_umask, mode);
=======
mode_t sys_umask(mode_t mode)
{
	return my_syscall1(__NR_umask, mode);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
mode_t umask(mode_t mode)
{
<<<<<<< HEAD
	return _sys_umask(mode);
=======
	return sys_umask(mode);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int umount2(const char *path, int flags);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_umount2(const char *path, int flags)
{
	return __nolibc_syscall2(__NR_umount2, path, flags);
=======
int sys_umount2(const char *path, int flags)
{
	return my_syscall2(__NR_umount2, path, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int umount2(const char *path, int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_umount2(path, flags));
=======
	return __sysret(sys_umount2(path, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int unlink(const char *path);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_unlink(const char *path)
{
#if defined(__NR_unlinkat)
	return __nolibc_syscall3(__NR_unlinkat, AT_FDCWD, path, 0);
#else
	return __nolibc_syscall1(__NR_unlink, path);
=======
int sys_unlink(const char *path)
{
#if defined(__NR_unlinkat)
	return my_syscall3(__NR_unlinkat, AT_FDCWD, path, 0);
#else
	return my_syscall1(__NR_unlink, path);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif
}

static __attribute__((unused))
int unlink(const char *path)
{
<<<<<<< HEAD
	return __sysret(_sys_unlink(path));
=======
	return __sysret(sys_unlink(path));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * ssize_t write(int fd, const void *buf, size_t count);
 */

static __attribute__((unused))
<<<<<<< HEAD
ssize_t _sys_write(int fd, const void *buf, size_t count)
{
	return __nolibc_syscall3(__NR_write, fd, buf, count);
=======
ssize_t sys_write(int fd, const void *buf, size_t count)
{
	return my_syscall3(__NR_write, fd, buf, count);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
ssize_t write(int fd, const void *buf, size_t count)
{
<<<<<<< HEAD
	return __sysret(_sys_write(fd, buf, count));
=======
	return __sysret(sys_write(fd, buf, count));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * int memfd_create(const char *name, unsigned int flags);
 */

static __attribute__((unused))
<<<<<<< HEAD
int _sys_memfd_create(const char *name, unsigned int flags)
{
	return __nolibc_syscall2(__NR_memfd_create, name, flags);
=======
int sys_memfd_create(const char *name, unsigned int flags)
{
	return my_syscall2(__NR_memfd_create, name, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static __attribute__((unused))
int memfd_create(const char *name, unsigned int flags)
{
<<<<<<< HEAD
	return __sysret(_sys_memfd_create(name, flags));
=======
	return __sysret(sys_memfd_create(name, flags));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

#endif /* _NOLIBC_SYS_H */
