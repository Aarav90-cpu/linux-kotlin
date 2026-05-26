// SPDX-License-Identifier: GPL-2.0-only
#include "linux/types.h"
#include "linux/bitmap.h"
#include "linux/atomic.h"

#include "kvm_util.h"
#include "ucall_common.h"


#define GUEST_UCALL_FAILED -1

struct ucall_header {
	DECLARE_BITMAP(in_use, KVM_MAX_VCPUS);
	struct ucall ucalls[KVM_MAX_VCPUS];
};

<<<<<<< HEAD
int ucall_nr_pages_required(u64 page_size)
=======
int ucall_nr_pages_required(uint64_t page_size)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return align_up(sizeof(struct ucall_header), page_size) / page_size;
}

/*
 * ucall_pool holds per-VM values (global data is duplicated by each VM), it
 * must not be accessed from host code.
 */
static struct ucall_header *ucall_pool;

<<<<<<< HEAD
void ucall_init(struct kvm_vm *vm, gpa_t mmio_gpa)
{
	struct ucall_header *hdr;
	struct ucall *uc;
	gva_t gva;
	int i;

	gva = vm_alloc_shared(vm, sizeof(*hdr), KVM_UTIL_MIN_VADDR,
				MEM_REGION_DATA);
	hdr = (struct ucall_header *)addr_gva2hva(vm, gva);
=======
void ucall_init(struct kvm_vm *vm, vm_paddr_t mmio_gpa)
{
	struct ucall_header *hdr;
	struct ucall *uc;
	vm_vaddr_t vaddr;
	int i;

	vaddr = vm_vaddr_alloc_shared(vm, sizeof(*hdr), KVM_UTIL_MIN_VADDR,
				      MEM_REGION_DATA);
	hdr = (struct ucall_header *)addr_gva2hva(vm, vaddr);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	memset(hdr, 0, sizeof(*hdr));

	for (i = 0; i < KVM_MAX_VCPUS; ++i) {
		uc = &hdr->ucalls[i];
		uc->hva = uc;
	}

<<<<<<< HEAD
	write_guest_global(vm, ucall_pool, (struct ucall_header *)gva);
=======
	write_guest_global(vm, ucall_pool, (struct ucall_header *)vaddr);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	ucall_arch_init(vm, mmio_gpa);
}

static struct ucall *ucall_alloc(void)
{
	struct ucall *uc;
	int i;

	if (!ucall_pool)
		goto ucall_failed;

	for (i = 0; i < KVM_MAX_VCPUS; ++i) {
		if (!test_and_set_bit(i, ucall_pool->in_use)) {
			uc = &ucall_pool->ucalls[i];
			memset(uc->args, 0, sizeof(uc->args));
			return uc;
		}
	}

ucall_failed:
	/*
	 * If the vCPU cannot grab a ucall structure, make a bare ucall with a
	 * magic value to signal to get_ucall() that things went sideways.
	 * GUEST_ASSERT() depends on ucall_alloc() and so cannot be used here.
	 */
	ucall_arch_do_ucall(GUEST_UCALL_FAILED);
	return NULL;
}

static void ucall_free(struct ucall *uc)
{
	/* Beware, here be pointer arithmetic.  */
	clear_bit(uc - ucall_pool->ucalls, ucall_pool->in_use);
}

<<<<<<< HEAD
void ucall_assert(u64 cmd, const char *exp, const char *file,
=======
void ucall_assert(uint64_t cmd, const char *exp, const char *file,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		  unsigned int line, const char *fmt, ...)
{
	struct ucall *uc;
	va_list va;

	uc = ucall_alloc();
	uc->cmd = cmd;

<<<<<<< HEAD
	WRITE_ONCE(uc->args[GUEST_ERROR_STRING], (u64)(exp));
	WRITE_ONCE(uc->args[GUEST_FILE], (u64)(file));
=======
	WRITE_ONCE(uc->args[GUEST_ERROR_STRING], (uint64_t)(exp));
	WRITE_ONCE(uc->args[GUEST_FILE], (uint64_t)(file));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	WRITE_ONCE(uc->args[GUEST_LINE], line);

	va_start(va, fmt);
	guest_vsnprintf(uc->buffer, UCALL_BUFFER_LEN, fmt, va);
	va_end(va);

<<<<<<< HEAD
	ucall_arch_do_ucall((gva_t)uc->hva);
=======
	ucall_arch_do_ucall((vm_vaddr_t)uc->hva);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	ucall_free(uc);
}

<<<<<<< HEAD
void ucall_fmt(u64 cmd, const char *fmt, ...)
=======
void ucall_fmt(uint64_t cmd, const char *fmt, ...)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct ucall *uc;
	va_list va;

	uc = ucall_alloc();
	uc->cmd = cmd;

	va_start(va, fmt);
	guest_vsnprintf(uc->buffer, UCALL_BUFFER_LEN, fmt, va);
	va_end(va);

<<<<<<< HEAD
	ucall_arch_do_ucall((gva_t)uc->hva);
=======
	ucall_arch_do_ucall((vm_vaddr_t)uc->hva);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	ucall_free(uc);
}

<<<<<<< HEAD
void ucall(u64 cmd, int nargs, ...)
=======
void ucall(uint64_t cmd, int nargs, ...)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct ucall *uc;
	va_list va;
	int i;

	uc = ucall_alloc();

	WRITE_ONCE(uc->cmd, cmd);

	nargs = min(nargs, UCALL_MAX_ARGS);

	va_start(va, nargs);
	for (i = 0; i < nargs; ++i)
<<<<<<< HEAD
		WRITE_ONCE(uc->args[i], va_arg(va, u64));
	va_end(va);

	ucall_arch_do_ucall((gva_t)uc->hva);
=======
		WRITE_ONCE(uc->args[i], va_arg(va, uint64_t));
	va_end(va);

	ucall_arch_do_ucall((vm_vaddr_t)uc->hva);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	ucall_free(uc);
}

<<<<<<< HEAD
u64 get_ucall(struct kvm_vcpu *vcpu, struct ucall *uc)
=======
uint64_t get_ucall(struct kvm_vcpu *vcpu, struct ucall *uc)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct ucall ucall;
	void *addr;

	if (!uc)
		uc = &ucall;

	addr = ucall_arch_get_ucall(vcpu);
	if (addr) {
		TEST_ASSERT(addr != (void *)GUEST_UCALL_FAILED,
			    "Guest failed to allocate ucall struct");

		memcpy(uc, addr, sizeof(*uc));
		vcpu_run_complete_io(vcpu);
	} else {
		memset(uc, 0, sizeof(*uc));
	}

	return uc->cmd;
}
