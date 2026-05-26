// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native kernel core types.
 *
 * This file defines the fundamental type aliases and constants used
 * throughout Kotlin kernel modules, mapping Kotlin types to their
 * kernel C equivalents.
 *
 * These types form the foundation of the Kotlin-to-kernel type bridge,
 * ensuring type safety while maintaining binary compatibility with
 * the C kernel ABI.
 */

package kernel.types

/** Result type for kernel operations that can fail with an errno. */
typealias KernelResult = Int

/** GFP flags for memory allocation. */
typealias GfpFlags = UInt

/** Kernel error codes (negative errno values). */
object Errno {
    const val SUCCESS: Int = 0
    const val EPERM: Int = -1
    const val ENOENT: Int = -2
    const val ESRCH: Int = -3
    const val EINTR: Int = -4
    const val EIO: Int = -5
    const val ENXIO: Int = -6
    const val E2BIG: Int = -7
    const val ENOEXEC: Int = -8
    const val EBADF: Int = -9
    const val ECHILD: Int = -10
    const val EAGAIN: Int = -11
    const val ENOMEM: Int = -12
    const val EACCES: Int = -13
    const val EFAULT: Int = -14
    const val EBUSY: Int = -16
    const val EEXIST: Int = -17
    const val ENODEV: Int = -19
    const val EINVAL: Int = -22
    const val ENOSPC: Int = -28
    const val ERANGE: Int = -34
    const val ENOSYS: Int = -38
    const val ENODATA: Int = -61
    const val EOVERFLOW: Int = -75
    const val EOPNOTSUPP: Int = -95
    const val ETIMEDOUT: Int = -110
}

/** GFP (Get Free Pages) allocation flags. */
object Gfp {
    /** Can sleep during allocation - the normal flag for most kernel allocations. */
    val KERNEL: GfpFlags = 0xCC0u   // __GFP_RECLAIM | __GFP_IO | __GFP_FS

    /** Cannot sleep - for use in interrupt context or with spinlocks held. */
    val ATOMIC: GfpFlags = 0x800u   // __GFP_HIGH | __GFP_ATOMIC

    /** Zero the allocated memory. */
    val ZERO: GfpFlags = 0x100u     // __GFP_ZERO
}

/**
 * A kernel-compatible Result type that wraps a value or an error code.
 *
 * This is Kotlin's equivalent of the kernel's ERR_PTR/IS_ERR/PTR_ERR pattern.
 */
sealed class KResult<out T> {
    data class Ok<T>(val value: T) : KResult<T>()
    data class Err(val code: Int) : KResult<Nothing>()

    fun isOk(): Boolean = this is Ok
    fun isErr(): Boolean = this is Err

    fun unwrap(): T = when (this) {
        is Ok -> value
        is Err -> throw IllegalStateException("Called unwrap() on Err($code)")
    }

    fun unwrapOr(default: @UnsafeVariance T): T = when (this) {
        is Ok -> value
        is Err -> default
    }

    inline fun <R> map(transform: (T) -> R): KResult<R> = when (this) {
        is Ok -> Ok(transform(value))
        is Err -> Err(code)
    }
}
