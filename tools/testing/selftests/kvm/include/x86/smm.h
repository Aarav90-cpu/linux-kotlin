// SPDX-License-Identifier: GPL-2.0-only
#ifndef SELFTEST_KVM_SMM_H
#define SELFTEST_KVM_SMM_H

#include "kvm_util.h"

#define SMRAM_SIZE	65536
#define SMRAM_MEMSLOT	((1 << 16) | 1)
#define SMRAM_PAGES	(SMRAM_SIZE / PAGE_SIZE)

<<<<<<< HEAD
void setup_smram(struct kvm_vm *vm, struct kvm_vcpu *vcpu, u64 smram_gpa,
=======
void setup_smram(struct kvm_vm *vm, struct kvm_vcpu *vcpu,
		 uint64_t smram_gpa,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		 const void *smi_handler, size_t handler_size);

void inject_smi(struct kvm_vcpu *vcpu);

#endif /* SELFTEST_KVM_SMM_H */
