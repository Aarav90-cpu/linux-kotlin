// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native kernel library - top-level entry point.
 *
 * This is the main library file for the Kotlin kernel crate, analogous
 * to rust/kernel/lib.rs. It re-exports all sub-modules that Kotlin
 * kernel modules can use.
 *
 * The Kotlin/Native kernel API is structured as follows:
 *
 *   kernel.types   - Core type definitions (errno, GFP flags, KResult)
 *   kernel.print   - Kernel logging (pr_info, pr_err, etc.)
 *   kernel.module  - Module declaration infrastructure
 *   kernel.sync    - Synchronization primitives (mutex, spinlock)
 *   kernel.alloc   - Kernel memory allocation wrappers
 *
 * Kotlin kernel modules import from these packages:
 *   import kernel.types.*
 *   import kernel.print.*
 *   import kernel.module.*
 */

package kernel

// This file serves as the compilation unit root.
// All Kotlin kernel modules depend on the packages defined under kotlin/kernel/.

/**
 * Kotlin/Native kernel library version.
 * This tracks the kernel version this library was built against.
 */
const val KOTLIN_KERNEL_VERSION: String = "7.0.9-zen"

/**
 * Check if we're building as a module or built-in.
 */
const val IS_MODULE: Boolean = false  // Set by build system via -D flag
