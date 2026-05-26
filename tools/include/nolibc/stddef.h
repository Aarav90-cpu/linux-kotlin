/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * Stddef definitions for NOLIBC
 * Copyright (C) 2017-2021 Willy Tarreau <w@1wt.eu>
 */

/* make sure to include all global symbols */
#include "nolibc.h"

#ifndef _NOLIBC_STDDEF_H
#define _NOLIBC_STDDEF_H

#include "stdint.h"

/* note: may already be defined */
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef offsetof
<<<<<<< HEAD
#define offsetof(TYPE, FIELD) __builtin_offsetof(TYPE, FIELD)
=======
#define offsetof(TYPE, FIELD) ((size_t) &((TYPE *)0)->FIELD)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif

#endif /* _NOLIBC_STDDEF_H */
