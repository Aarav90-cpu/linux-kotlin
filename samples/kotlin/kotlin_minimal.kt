// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native minimal sample kernel module.
 *
 * This is the "hello world" of Kotlin kernel modules, demonstrating
 * the basic structure: module annotation, init/exit lifecycle, and
 * kernel logging.
 *
 * Equivalent to samples/rust/rust_minimal.rs
 *
 * Load:   insmod kotlin_minimal.ko
 * Unload: rmmod kotlin_minimal
 * Check:  dmesg | grep kotlin_minimal
 */

package samples.kotlin_minimal

import kernel.types.*
import kernel.print.*
import kernel.module.*

/**
 * Minimal Kotlin kernel module.
 *
 * When loaded, this module prints a greeting to the kernel log.
 * When unloaded, it prints a farewell message.
 */
@KernelModule(
    name = "kotlin_minimal",
    author = "Kotlin for Linux Contributors",
    description = "Kotlin/Native minimal sample module",
    license = "GPL"
)
class KotlinMinimal : Module {

    /** Data allocated during init, demonstrating state management. */
    private val numbers = mutableListOf<Int>()

    /**
     * Module initialization.
     *
     * Called when the module is loaded into the kernel. Allocates some
     * test data and prints status information.
     */
    override fun init(): KernelResult {
        pr_info("Kotlin/Native minimal sample (init)\n")
        pr_info("Hello from Kotlin/Native in the Linux kernel!\n")

        // Demonstrate basic Kotlin data structures
        numbers.add(72)
        numbers.add(108)
        numbers.add(200)

        pr_info("Allocated ${numbers.size} numbers: $numbers\n")

        // Demonstrate Kotlin's type-safe error handling
        val result: KResult<String> = KResult.Ok("Kotlin is running in kernel space!")
        when (result) {
            is KResult.Ok -> pr_info("Result: ${result.value}\n")
            is KResult.Err -> pr_err("Error: ${result.code}\n")
        }

        return Errno.SUCCESS
    }

    /**
     * Module cleanup.
     *
     * Called when the module is unloaded. Releases all resources.
     */
    override fun exit() {
        pr_info("My numbers are: $numbers\n")
        pr_info("Kotlin/Native minimal sample (exit)\n")
    }
}
