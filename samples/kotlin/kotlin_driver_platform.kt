// SPDX-License-Identifier: GPL-2.0

/**
 * Kotlin/Native platform driver sample.
 *
 * Demonstrates how to write a basic platform driver in Kotlin/Native.
 * This mirrors the structure of samples/rust/rust_driver_platform.rs
 * and shows how Kotlin code interacts with the kernel's driver model.
 *
 * A platform driver is the simplest form of kernel driver - it binds
 * to devices described by devicetree or ACPI, or manually instantiated.
 */

package samples.kotlin_driver_platform

import kernel.types.*
import kernel.print.*
import kernel.module.*

/**
 * Represents a platform device instance.
 *
 * In a real driver, this would hold device-specific state:
 * mapped I/O regions, IRQ numbers, DMA channels, etc.
 */
data class KotlinPlatformDevice(
    val name: String,
    val id: Int,
    var initialized: Boolean = false
)

/**
 * Kotlin/Native platform driver sample.
 *
 * This driver demonstrates:
 * - Driver registration with the platform bus
 * - Probe/remove lifecycle
 * - Device state management using Kotlin data classes
 * - Type-safe error handling with KResult
 */
@KernelModule(
    name = "kotlin_driver_platform",
    author = "Kotlin for Linux Contributors",
    description = "Kotlin/Native platform driver sample",
    license = "GPL"
)
class KotlinDriverPlatform : Module {

    private var device: KotlinPlatformDevice? = null

    /**
     * Driver probe function.
     *
     * Called by the kernel when a matching device is found.
     * Initializes device-specific state and resources.
     */
    private fun probe(name: String, id: Int): KResult<KotlinPlatformDevice> {
        pr_info("kotlin_driver_platform: probe() called for $name (id=$id)\n")

        val dev = KotlinPlatformDevice(
            name = name,
            id = id,
            initialized = true
        )

        pr_info("kotlin_driver_platform: device ${dev.name} initialized\n")
        return KResult.Ok(dev)
    }

    /**
     * Driver remove function.
     *
     * Called when the device is removed or the driver is unloaded.
     * Must release all device resources.
     */
    private fun remove(dev: KotlinPlatformDevice) {
        pr_info("kotlin_driver_platform: remove() called for ${dev.name}\n")
        // In a real driver: unmap I/O, free IRQs, release DMA, etc.
    }

    override fun init(): KernelResult {
        pr_info("Kotlin/Native platform driver sample (init)\n")

        // Simulate probing a device
        val result = probe("kotlin-sample-device", 0)
        when (result) {
            is KResult.Ok -> {
                device = result.value
                pr_info("Driver registered successfully\n")
            }
            is KResult.Err -> {
                pr_err("Driver probe failed with error ${result.code}\n")
                return result.code
            }
        }

        return Errno.SUCCESS
    }

    override fun exit() {
        device?.let { remove(it) }
        pr_info("Kotlin/Native platform driver sample (exit)\n")
    }
}
