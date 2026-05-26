// SPDX-License-Identifier: GPL-2.0-only
/*
 * Test Hyper-V extended hypercall, HV_EXT_CALL_QUERY_CAPABILITIES (0x8001),
 * exit to userspace and receive result in guest.
 *
 * Negative tests are present in hyperv_features.c
 *
 * Copyright 2022 Google LLC
 * Author: Vipin Sharma <vipinsh@google.com>
 */
#include "kvm_util.h"
#include "processor.h"
#include "hyperv.h"

/* Any value is fine */
#define EXT_CAPABILITIES 0xbull

<<<<<<< HEAD
static void guest_code(gpa_t in_pg_gpa, gpa_t out_pg_gpa,
		       gva_t out_pg_gva)
{
	u64 *output_gva;
=======
static void guest_code(vm_paddr_t in_pg_gpa, vm_paddr_t out_pg_gpa,
		       vm_vaddr_t out_pg_gva)
{
	uint64_t *output_gva;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	wrmsr(HV_X64_MSR_GUEST_OS_ID, HYPERV_LINUX_OS_ID);
	wrmsr(HV_X64_MSR_HYPERCALL, in_pg_gpa);

<<<<<<< HEAD
	output_gva = (u64 *)out_pg_gva;

	hyperv_hypercall(HV_EXT_CALL_QUERY_CAPABILITIES, in_pg_gpa, out_pg_gpa);

	/* TLFS states output will be a u64 value */
=======
	output_gva = (uint64_t *)out_pg_gva;

	hyperv_hypercall(HV_EXT_CALL_QUERY_CAPABILITIES, in_pg_gpa, out_pg_gpa);

	/* TLFS states output will be a uint64_t value */
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	GUEST_ASSERT_EQ(*output_gva, EXT_CAPABILITIES);

	GUEST_DONE();
}

int main(void)
{
<<<<<<< HEAD
	gva_t hcall_out_page;
	gva_t hcall_in_page;
	struct kvm_vcpu *vcpu;
	struct kvm_run *run;
	struct kvm_vm *vm;
	u64 *outval;
=======
	vm_vaddr_t hcall_out_page;
	vm_vaddr_t hcall_in_page;
	struct kvm_vcpu *vcpu;
	struct kvm_run *run;
	struct kvm_vm *vm;
	uint64_t *outval;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	struct ucall uc;

	TEST_REQUIRE(kvm_has_cap(KVM_CAP_HYPERV_CPUID));

	/* Verify if extended hypercalls are supported */
	if (!kvm_cpuid_has(kvm_get_supported_hv_cpuid(),
			   HV_ENABLE_EXTENDED_HYPERCALLS)) {
		print_skip("Extended calls not supported by the kernel");
		exit(KSFT_SKIP);
	}

	vm = vm_create_with_one_vcpu(&vcpu, guest_code);
	run = vcpu->run;
	vcpu_set_hv_cpuid(vcpu);

	/* Hypercall input */
<<<<<<< HEAD
	hcall_in_page = vm_alloc_pages(vm, 1);
	memset(addr_gva2hva(vm, hcall_in_page), 0x0, vm->page_size);

	/* Hypercall output */
	hcall_out_page = vm_alloc_pages(vm, 1);
=======
	hcall_in_page = vm_vaddr_alloc_pages(vm, 1);
	memset(addr_gva2hva(vm, hcall_in_page), 0x0, vm->page_size);

	/* Hypercall output */
	hcall_out_page = vm_vaddr_alloc_pages(vm, 1);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	memset(addr_gva2hva(vm, hcall_out_page), 0x0, vm->page_size);

	vcpu_args_set(vcpu, 3, addr_gva2gpa(vm, hcall_in_page),
		      addr_gva2gpa(vm, hcall_out_page), hcall_out_page);

	vcpu_run(vcpu);

	TEST_ASSERT(run->exit_reason == KVM_EXIT_HYPERV,
		    "Unexpected exit reason: %u (%s)",
		    run->exit_reason, exit_reason_str(run->exit_reason));

	outval = addr_gpa2hva(vm, run->hyperv.u.hcall.params[1]);
	*outval = EXT_CAPABILITIES;
	run->hyperv.u.hcall.result = HV_STATUS_SUCCESS;

	vcpu_run(vcpu);

	TEST_ASSERT(run->exit_reason == KVM_EXIT_IO,
		    "Unexpected exit reason: %u (%s)",
		    run->exit_reason, exit_reason_str(run->exit_reason));

	switch (get_ucall(vcpu, &uc)) {
	case UCALL_ABORT:
		REPORT_GUEST_ASSERT(uc);
		break;
	case UCALL_DONE:
		break;
	default:
		TEST_FAIL("Unhandled ucall: %ld", uc.cmd);
	}

	kvm_vm_free(vm);
	return 0;
}
