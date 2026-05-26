// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native module parameters sample.
 *
 * Demonstrates how to declare and use module parameters in Kotlin
 * kernel modules. Parameters can be set at module load time and
 * optionally exposed via sysfs.
 *
 * Usage:
 *   insmod kotlin_parameters.ko myInt=42 myString="hello" myBool=true
 *   cat /sys/module/kotlin_parameters/parameters/myInt
 */

package samples.kotlin_parameters

import kernel.types.*
import kernel.print.*
import kernel.module.*

@KernelModule(
    name = "kotlin_parameters",
    author = "Kotlin for Linux Contributors",
    description = "Kotlin/Native module parameters sample",
    license = "GPL"
)
class KotlinParameters : Module {

    /**
     * Integer parameter with a default value.
     * Can be set via: insmod kotlin_parameters.ko myInt=42
     */
    @ModuleParam(description = "An integer parameter", permissions = 0x124)
    var myInt: Int = 1

    /**
     * Boolean parameter.
     * Can be set via: insmod kotlin_parameters.ko myBool=true
     */
    @ModuleParam(description = "A boolean parameter")
    var myBool: Boolean = false

    /**
     * String parameter.
     * Can be set via: insmod kotlin_parameters.ko myString="hello"
     */
    @ModuleParam(description = "A string parameter")
    var myString: String = "default"

    override fun init(): KernelResult {
        pr_info("Kotlin/Native parameters sample (init)\n")
        pr_info("Parameters:\n")
        pr_info("  myInt    = $myInt\n")
        pr_info("  myBool   = $myBool\n")
        pr_info("  myString = $myString\n")

        // Demonstrate parameter validation
        if (myInt < 0) {
            pr_err("myInt must be non-negative, got $myInt\n")
            return Errno.EINVAL
        }

        pr_info("All parameters validated successfully\n")
        return Errno.SUCCESS
    }

    override fun exit() {
        pr_info("Kotlin/Native parameters sample (exit)\n")
        pr_info("Final parameter values:\n")
        pr_info("  myInt    = $myInt\n")
        pr_info("  myBool   = $myBool\n")
        pr_info("  myString = $myString\n")
    }
}
