// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native kernel print sample module.
 *
 * Demonstrates all the kernel logging levels available from Kotlin/Native
 * code, and shows how Kotlin's string interpolation works with kernel
 * print functions.
 *
 * Equivalent to samples/rust/rust_print_main.rs
 */

package samples.kotlin_print

import kernel.types.*
import kernel.print.*
import kernel.module.*

@KernelModule(
    name = "kotlin_print",
    author = "Kotlin for Linux Contributors",
    description = "Kotlin/Native printing sample module",
    license = "GPL"
)
class KotlinPrint : Module {

    override fun init(): KernelResult {
        pr_info("Kotlin/Native print sample (init)\n")
        pr_info("\n")

        // Demonstrate different log levels
        pr_emerg("Emergency level message\n")
        pr_alert("Alert level message\n")
        pr_crit("Critical level message\n")
        pr_err("Error level message\n")
        pr_warn("Warning level message\n")
        pr_notice("Notice level message\n")
        pr_info("Info level message\n")
        pr_debug("Debug level message\n")

        pr_info("\n")

        // Demonstrate Kotlin string interpolation
        val moduleName = "kotlin_print"
        val version = 1
        pr_info("Module '$moduleName' version $version loaded\n")

        // Demonstrate formatted output with Kotlin expressions
        val values = listOf(1, 2, 3, 4, 5)
        pr_info("Values: ${values.joinToString(", ")}\n")
        pr_info("Sum: ${values.sum()}\n")
        pr_info("Max: ${values.max()}\n")

        // Demonstrate multiline with Kotlin raw strings
        val status = buildString {
            append("Module status:\n")
            append("  Name: $moduleName\n")
            append("  Version: $version\n")
            append("  Items: ${values.size}\n")
        }
        pr_info(status)

        return Errno.SUCCESS
    }

    override fun exit() {
        pr_info("Kotlin/Native print sample (exit)\n")
    }
}
