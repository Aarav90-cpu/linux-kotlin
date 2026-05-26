// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native kernel module infrastructure.
 *
 * Provides the annotation-based module declaration system for Kotlin
 * kernel modules. This is Kotlin's equivalent of the Rust `module!{}`
 * macro or C's `module_init()` / `module_exit()` macros.
 *
 * Usage:
 * ```kotlin
 * @KernelModule(
 *     name = "kotlin_minimal",
 *     author = "Kotlin for Linux Contributors",
 *     description = "Kotlin minimal sample module",
 *     license = "GPL"
 * )
 * class MyModule : Module {
 *     override fun init(): KernelResult {
 *         pr_info("Module loaded!\n")
 *         return Errno.SUCCESS
 *     }
 *
 *     override fun exit() {
 *         pr_info("Module unloaded!\n")
 *     }
 * }
 * ```
 */

package kernel.module

import kernel.types.KernelResult

/**
 * Annotation to declare a Kotlin kernel module.
 *
 * This annotation marks a class as a kernel module and provides the
 * metadata that the kernel requires: name, author, description, license,
 * and optional version and alias information.
 *
 * The build system processes this annotation to generate the necessary
 * C module_init/module_exit trampolines.
 *
 * @param name The module name as it appears in lsmod/modprobe
 * @param author The module author(s)
 * @param description Human-readable description of the module
 * @param license Module license (must be "GPL" for kernel symbols)
 * @param version Optional version string
 * @param alias Optional module alias for auto-loading
 */
@Target(AnnotationTarget.CLASS)
@Retention(AnnotationRetention.BINARY)
annotation class KernelModule(
    val name: String,
    val author: String = "",
    val description: String = "",
    val license: String = "GPL",
    val version: String = "",
    val alias: String = ""
)

/**
 * Interface that all Kotlin kernel modules must implement.
 *
 * This is the Kotlin equivalent of implementing `kernel::Module` in Rust
 * or providing `module_init()` / `module_exit()` functions in C.
 */
interface Module {
    /**
     * Module initialization function.
     *
     * Called when the module is loaded (either at boot time for built-in
     * modules, or at insmod/modprobe time for loadable modules).
     *
     * @return 0 (Errno.SUCCESS) on success, or a negative errno on failure.
     *         Returning an error will prevent the module from loading and
     *         the kernel will print an error message.
     */
    fun init(): KernelResult

    /**
     * Module exit/cleanup function.
     *
     * Called when the module is unloaded (rmmod). This function must
     * release all resources acquired during init() and the module's
     * lifetime. For built-in modules, this is never called.
     */
    fun exit()
}

/**
 * Annotation for module parameters.
 *
 * Declares a module parameter that can be set at load time via
 * insmod/modprobe or via the kernel command line for built-in modules.
 *
 * Usage:
 * ```kotlin
 * @ModuleParam(description = "Enable verbose logging")
 * var verbose: Boolean = false
 *
 * @ModuleParam(description = "Maximum buffer size", permissions = 0x124)
 * var maxBufSize: Int = 4096
 * ```
 *
 * @param description Human-readable description (shown in modinfo)
 * @param permissions sysfs permissions (0 = not visible in sysfs)
 */
@Target(AnnotationTarget.PROPERTY)
@Retention(AnnotationRetention.BINARY)
annotation class ModuleParam(
    val description: String = "",
    val permissions: Int = 0
)
