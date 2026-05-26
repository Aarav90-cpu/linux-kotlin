// SPDX-License-Identifier: GPL-2.0
/*
 * ucall support. A ucall is a "hypercall to userspace".
 *
 * Copyright (C) 2018, Red Hat, Inc.
 */
#include "kvm_util.h"

<<<<<<< HEAD
gva_t *ucall_exit_mmio_addr;

void ucall_arch_init(struct kvm_vm *vm, gpa_t mmio_gpa)
{
	gva_t mmio_gva = vm_unused_gva_gap(vm, vm->page_size, KVM_UTIL_MIN_VADDR);
=======
vm_vaddr_t *ucall_exit_mmio_addr;

void ucall_arch_init(struct kvm_vm *vm, vm_paddr_t mmio_gpa)
{
	vm_vaddr_t mmio_gva = vm_vaddr_unused_gap(vm, vm->page_size, KVM_UTIL_MIN_VADDR);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	virt_map(vm, mmio_gva, mmio_gpa, 1);

	vm->ucall_mmio_addr = mmio_gpa;

<<<<<<< HEAD
	write_guest_global(vm, ucall_exit_mmio_addr, (gva_t *)mmio_gva);
=======
	write_guest_global(vm, ucall_exit_mmio_addr, (vm_vaddr_t *)mmio_gva);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

void *ucall_arch_get_ucall(struct kvm_vcpu *vcpu)
{
	struct kvm_run *run = vcpu->run;

	if (run->exit_reason == KVM_EXIT_MMIO &&
	    run->mmio.phys_addr == vcpu->vm->ucall_mmio_addr) {
<<<<<<< HEAD
		TEST_ASSERT(run->mmio.is_write && run->mmio.len == sizeof(u64),
			    "Unexpected ucall exit mmio address access");
		return (void *)(*((u64 *)run->mmio.data));
=======
		TEST_ASSERT(run->mmio.is_write && run->mmio.len == sizeof(uint64_t),
			    "Unexpected ucall exit mmio address access");
		return (void *)(*((uint64_t *)run->mmio.data));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	}

	return NULL;
}
