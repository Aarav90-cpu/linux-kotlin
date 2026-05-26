// SPDX-License-Identifier: GPL-2.0

#include <asm/barrier.h>
#include <asm/rwonce.h>
#include <linux/atomic.h>

<<<<<<< HEAD
#define GEN_READ_HELPER(tname, type)						\
__rust_helper type rust_helper_atomic_##tname##_read(type *ptr)			\
{										\
	return READ_ONCE(*ptr);							\
}

#define GEN_SET_HELPER(tname, type)						\
__rust_helper void rust_helper_atomic_##tname##_set(type *ptr, type val)	\
{										\
	WRITE_ONCE(*ptr, val);							\
}

#define GEN_READ_ACQUIRE_HELPER(tname, type)					\
__rust_helper type rust_helper_atomic_##tname##_read_acquire(type *ptr)		\
{										\
	return smp_load_acquire(ptr);						\
}

#define GEN_SET_RELEASE_HELPER(tname, type)					\
__rust_helper void rust_helper_atomic_##tname##_set_release(type *ptr, type val)\
{										\
	smp_store_release(ptr, val);						\
}

#define GEN_READ_SET_HELPERS(tname, type)					\
	GEN_READ_HELPER(tname, type)						\
	GEN_SET_HELPER(tname, type)						\
	GEN_READ_ACQUIRE_HELPER(tname, type)					\
	GEN_SET_RELEASE_HELPER(tname, type)					\

GEN_READ_SET_HELPERS(i8, s8)
GEN_READ_SET_HELPERS(i16, s16)
GEN_READ_SET_HELPERS(ptr, const void *)
=======
__rust_helper s8 rust_helper_atomic_i8_read(s8 *ptr)
{
	return READ_ONCE(*ptr);
}

__rust_helper s8 rust_helper_atomic_i8_read_acquire(s8 *ptr)
{
	return smp_load_acquire(ptr);
}

__rust_helper s16 rust_helper_atomic_i16_read(s16 *ptr)
{
	return READ_ONCE(*ptr);
}

__rust_helper s16 rust_helper_atomic_i16_read_acquire(s16 *ptr)
{
	return smp_load_acquire(ptr);
}

__rust_helper void rust_helper_atomic_i8_set(s8 *ptr, s8 val)
{
	WRITE_ONCE(*ptr, val);
}

__rust_helper void rust_helper_atomic_i8_set_release(s8 *ptr, s8 val)
{
	smp_store_release(ptr, val);
}

__rust_helper void rust_helper_atomic_i16_set(s16 *ptr, s16 val)
{
	WRITE_ONCE(*ptr, val);
}

__rust_helper void rust_helper_atomic_i16_set_release(s16 *ptr, s16 val)
{
	smp_store_release(ptr, val);
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/*
 * xchg helpers depend on ARCH_SUPPORTS_ATOMIC_RMW and on the
 * architecture provding xchg() support for i8 and i16.
 *
 * The architectures that currently support Rust (x86_64, armv7,
 * arm64, riscv, and loongarch) satisfy these requirements.
 */
<<<<<<< HEAD
#define GEN_XCHG_HELPER(tname, type, suffix)					\
__rust_helper type								\
rust_helper_atomic_##tname##_xchg##suffix(type *ptr, type new)			\
{										\
	return xchg##suffix(ptr, new);					\
}

#define GEN_XCHG_HELPERS(tname, type)						\
	GEN_XCHG_HELPER(tname, type, )						\
	GEN_XCHG_HELPER(tname, type, _acquire)					\
	GEN_XCHG_HELPER(tname, type, _release)					\
	GEN_XCHG_HELPER(tname, type, _relaxed)					\

GEN_XCHG_HELPERS(i8, s8)
GEN_XCHG_HELPERS(i16, s16)
GEN_XCHG_HELPERS(ptr, const void *)
=======
__rust_helper s8 rust_helper_atomic_i8_xchg(s8 *ptr, s8 new)
{
	return xchg(ptr, new);
}

__rust_helper s16 rust_helper_atomic_i16_xchg(s16 *ptr, s16 new)
{
	return xchg(ptr, new);
}

__rust_helper s8 rust_helper_atomic_i8_xchg_acquire(s8 *ptr, s8 new)
{
	return xchg_acquire(ptr, new);
}

__rust_helper s16 rust_helper_atomic_i16_xchg_acquire(s16 *ptr, s16 new)
{
	return xchg_acquire(ptr, new);
}

__rust_helper s8 rust_helper_atomic_i8_xchg_release(s8 *ptr, s8 new)
{
	return xchg_release(ptr, new);
}

__rust_helper s16 rust_helper_atomic_i16_xchg_release(s16 *ptr, s16 new)
{
	return xchg_release(ptr, new);
}

__rust_helper s8 rust_helper_atomic_i8_xchg_relaxed(s8 *ptr, s8 new)
{
	return xchg_relaxed(ptr, new);
}

__rust_helper s16 rust_helper_atomic_i16_xchg_relaxed(s16 *ptr, s16 new)
{
	return xchg_relaxed(ptr, new);
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/*
 * try_cmpxchg helpers depend on ARCH_SUPPORTS_ATOMIC_RMW and on the
 * architecture provding try_cmpxchg() support for i8 and i16.
 *
 * The architectures that currently support Rust (x86_64, armv7,
 * arm64, riscv, and loongarch) satisfy these requirements.
 */
<<<<<<< HEAD
#define GEN_TRY_CMPXCHG_HELPER(tname, type, suffix)				\
__rust_helper bool								\
rust_helper_atomic_##tname##_try_cmpxchg##suffix(type *ptr, type *old, type new)\
{										\
	return try_cmpxchg##suffix(ptr, old, new);				\
}

#define GEN_TRY_CMPXCHG_HELPERS(tname, type)					\
	GEN_TRY_CMPXCHG_HELPER(tname, type, )					\
	GEN_TRY_CMPXCHG_HELPER(tname, type, _acquire)				\
	GEN_TRY_CMPXCHG_HELPER(tname, type, _release)				\
	GEN_TRY_CMPXCHG_HELPER(tname, type, _relaxed)				\

GEN_TRY_CMPXCHG_HELPERS(i8, s8)
GEN_TRY_CMPXCHG_HELPERS(i16, s16)
GEN_TRY_CMPXCHG_HELPERS(ptr, const void *)
=======
__rust_helper bool rust_helper_atomic_i8_try_cmpxchg(s8 *ptr, s8 *old, s8 new)
{
	return try_cmpxchg(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i16_try_cmpxchg(s16 *ptr, s16 *old, s16 new)
{
	return try_cmpxchg(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i8_try_cmpxchg_acquire(s8 *ptr, s8 *old, s8 new)
{
	return try_cmpxchg_acquire(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i16_try_cmpxchg_acquire(s16 *ptr, s16 *old, s16 new)
{
	return try_cmpxchg_acquire(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i8_try_cmpxchg_release(s8 *ptr, s8 *old, s8 new)
{
	return try_cmpxchg_release(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i16_try_cmpxchg_release(s16 *ptr, s16 *old, s16 new)
{
	return try_cmpxchg_release(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i8_try_cmpxchg_relaxed(s8 *ptr, s8 *old, s8 new)
{
	return try_cmpxchg_relaxed(ptr, old, new);
}

__rust_helper bool rust_helper_atomic_i16_try_cmpxchg_relaxed(s16 *ptr, s16 *old, s16 new)
{
	return try_cmpxchg_relaxed(ptr, old, new);
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
