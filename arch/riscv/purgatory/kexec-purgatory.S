/* SPDX-License-Identifier: GPL-2.0 */

	.section .rodata, "a"

	.align	8
kexec_purgatory:
	.globl	kexec_purgatory
	.incbin	"arch/riscv/purgatory/purgatory.ro"
<<<<<<< HEAD
.Lkexec_purgatory_end:
=======
.Lkexec_purgatroy_end:
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	.align	8
kexec_purgatory_size:
	.globl	kexec_purgatory_size
<<<<<<< HEAD
	.quad	.Lkexec_purgatory_end - kexec_purgatory
=======
	.quad	.Lkexec_purgatroy_end - kexec_purgatory
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
