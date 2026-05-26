// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2020 Facebook */
#include <stddef.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include "bpf_misc.h"

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f1(struct __sk_buff *skb)
{
	return skb->len;
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f2(int val, struct __sk_buff *skb)
{
	return f1(skb) + val;
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f3(int val, struct __sk_buff *skb, int var)
{
	return f2(var, skb) + val;
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f4(struct __sk_buff *skb)
{
	return f3(1, skb, 2);
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f5(struct __sk_buff *skb)
{
	return f4(skb);
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f6(struct __sk_buff *skb)
{
	return f5(skb);
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f7(struct __sk_buff *skb)
{
	return f6(skb);
}

<<<<<<< HEAD
static __attribute__ ((noinline))
=======
__attribute__ ((noinline))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int f8(struct __sk_buff *skb)
{
	return f7(skb);
}

SEC("tc")
<<<<<<< HEAD
__failure __msg("the call stack of 9 frames")
=======
__failure __msg("the call stack of 8 frames")
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int global_func3(struct __sk_buff *skb)
{
	return f8(skb);
}
