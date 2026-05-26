// SPDX-License-Identifier: GPL-2.0

package samples.kotlin_pci_driver

import kernel.types.*
import kernel.print.*
import kernel.module.*

/**
 * Kotlin/Native PCI driver sample.
 *
 * Demonstrates how to manage PCI device structures in Kotlin/Native.
 * In a fully bound system, this would register with `pci_register_driver`.
 */

data class KotlinPciDevice(
    val vendorId: UShort,
    val deviceId: UShort,
    val name: String,
    var enabled: Boolean = false
)

@KernelModule(
    name = "kotlin_pci_driver",
    author = "Kotlin for Linux Contributors",
    description = "Kotlin/Native PCI driver sample",
    license = "GPL"
)
class KotlinPciDriver : Module {

    private var activeDevice: KotlinPciDevice? = null

    /**
     * PCI Probe callback.
     *
     * Called when the PCI subsystem discovers a matching vendor/device ID.
     */
    private fun probe(vendor: UShort, device: UShort): KResult<KotlinPciDevice> {
        val vStr = vendor.toString(16).padStart(4, '0')
        val dStr = device.toString(16).padStart(4, '0')
        
        pr_info("kotlin_pci_driver: [PROBE] Detected PCI device! Vendor: 0x$vStr, Device: 0x$dStr\n")

        val pciDev = KotlinPciDevice(
            vendorId = vendor,
            deviceId = device,
            name = "Kotlin-PCI-Endpoint",
            enabled = true
        )

        pr_info("kotlin_pci_driver: Device ${pciDev.name} successfully initialized and enabled for Kotlin control.\n")
        return KResult.Ok(pciDev)
    }

    /**
     * PCI Remove callback.
     *
     * Called when the device is hot-unplugged or driver is unloaded.
     */
    private fun remove(dev: KotlinPciDevice) {
        val vStr = dev.vendorId.toString(16).padStart(4, '0')
        val dStr = dev.deviceId.toString(16).padStart(4, '0')
        pr_info("kotlin_pci_driver: [REMOVE] Detaching device 0x$vStr:0x$dStr (${dev.name})\n")
        dev.enabled = false
    }

    override fun init(): KernelResult {
        pr_info("Kotlin/Native PCI driver sample (init)\n")
        pr_info("Registering driver into PCI subsystem...\n")

        // Simulate the PCI subsystem finding our hardware on the bus
        val result = probe(0x10ECu, 0x8168u) // e.g. Realtek NIC
        
        when (result) {
            is KResult.Ok -> {
                activeDevice = result.value
                pr_info("Kotlin PCI driver bound successfully!\n")
            }
            is KResult.Err -> {
                pr_err("Kotlin PCI Driver probe failed with error ${result.code}\n")
                return result.code
            }
        }

        return Errno.SUCCESS
    }

    override fun exit() {
        pr_info("Kotlin/Native PCI driver sample (exit)\n")
        activeDevice?.let { remove(it) }
        pr_info("Kotlin PCI Driver deregistered.\n")
    }
}
