/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ARCH_ARM64_TLBBATCH_H
#define _ARCH_ARM64_TLBBATCH_H

<<<<<<< HEAD
#include <linux/cpumask.h>

struct arch_tlbflush_unmap_batch {
#ifdef CONFIG_ARM64_ERRATUM_4193714
	/*
	 * Track CPUs that need SME DVMSync on completion of this batch.
	 * Otherwise, the arm64 HW can do tlb shootdown, so we don't need to
	 * record cpumask for sending IPI
	 */
	cpumask_var_t cpumask;
#endif
=======
struct arch_tlbflush_unmap_batch {
	/*
	 * For arm64, HW can do tlb shootdown, so we don't
	 * need to record cpumask for sending IPI
	 */
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

#endif /* _ARCH_ARM64_TLBBATCH_H */
