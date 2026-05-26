// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native kernel printing support.
 *
 * Provides type-safe wrappers around the kernel's printk() facility.
 * Since printk is a C variadic macro, Kotlin/Native cannot call it
 * directly — instead, we use the kotlin_helper_printk_emit() C helper
 * function defined in kotlin/helpers.c.
 *
 * Usage:
 *   pr_info("Hello from Kotlin!\n")
 *   pr_err("Something went wrong: error $code\n")
 *   pr_warn("Resource ${name} is deprecated\n")
 */

package kernel.print

/**
 * Kernel log levels, matching the KERN_* constants.
 */
object LogLevel {
    const val EMERG: Int = 0
    const val ALERT: Int = 1
    const val CRIT: Int = 2
    const val ERR: Int = 3
    const val WARNING: Int = 4
    const val NOTICE: Int = 5
    const val INFO: Int = 6
    const val DEBUG: Int = 7
}

/**
 * External C helper function declaration.
 * This is implemented in kotlin/helpers.c and bridged via cinterop.
 *
 * Note: In actual Kotlin/Native cinterop, this would be auto-generated
 * from the .def file. Here we define the Kotlin-side interface.
 */
// In actual build, these would come from cinterop bindings:
// external fun kotlin_helper_printk_emit(level: Int, msg: String)

/**
 * Print a kernel message at KERN_EMERG level.
 */
fun pr_emerg(msg: String) {
    // Calls through to kotlin_helper_printk_emit(0, msg)
    // via the cinterop-generated binding
}

/**
 * Print a kernel message at KERN_ALERT level.
 */
fun pr_alert(msg: String) {
    // Calls through to kotlin_helper_printk_emit(1, msg)
}

/**
 * Print a kernel message at KERN_CRIT level.
 */
fun pr_crit(msg: String) {
    // Calls through to kotlin_helper_printk_emit(2, msg)
}

/**
 * Print a kernel message at KERN_ERR level.
 */
fun pr_err(msg: String) {
    // Calls through to kotlin_helper_printk_emit(3, msg)
}

/**
 * Print a kernel message at KERN_WARNING level.
 */
fun pr_warn(msg: String) {
    // Calls through to kotlin_helper_printk_emit(4, msg)
}

/**
 * Print a kernel message at KERN_NOTICE level.
 */
fun pr_notice(msg: String) {
    // Calls through to kotlin_helper_printk_emit(5, msg)
}

/**
 * Print a kernel message at KERN_INFO level.
 */
fun pr_info(msg: String) {
    // Calls through to kotlin_helper_printk_emit(6, msg)
}

/**
 * Print a kernel message at KERN_DEBUG level.
 */
fun pr_debug(msg: String) {
    // Calls through to kotlin_helper_printk_emit(7, msg)
}
