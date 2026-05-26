/* SPDX-License-Identifier: GPL-2.0 */
/*
 * RISC-V Arch Timer(sstc) specific interface
 *
 * Copyright (c) 2024 Intel Corporation
 */

#ifndef SELFTEST_KVM_ARCH_TIMER_H
#define SELFTEST_KVM_ARCH_TIMER_H

#include <asm/csr.h>
#include <asm/vdso/processor.h>

static unsigned long timer_freq;

#define msec_to_cycles(msec)	\
<<<<<<< HEAD
	((timer_freq) * (u64)(msec) / 1000)

#define usec_to_cycles(usec)	\
	((timer_freq) * (u64)(usec) / 1000000)

#define cycles_to_usec(cycles) \
	((u64)(cycles) * 1000000 / (timer_freq))

static inline u64 timer_get_cycles(void)
=======
	((timer_freq) * (uint64_t)(msec) / 1000)

#define usec_to_cycles(usec)	\
	((timer_freq) * (uint64_t)(usec) / 1000000)

#define cycles_to_usec(cycles) \
	((uint64_t)(cycles) * 1000000 / (timer_freq))

static inline uint64_t timer_get_cycles(void)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return csr_read(CSR_TIME);
}

<<<<<<< HEAD
static inline void timer_set_cmp(u64 cval)
=======
static inline void timer_set_cmp(uint64_t cval)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	csr_write(CSR_STIMECMP, cval);
}

<<<<<<< HEAD
static inline u64 timer_get_cmp(void)
=======
static inline uint64_t timer_get_cmp(void)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return csr_read(CSR_STIMECMP);
}

static inline void timer_irq_enable(void)
{
	csr_set(CSR_SIE, IE_TIE);
}

static inline void timer_irq_disable(void)
{
	csr_clear(CSR_SIE, IE_TIE);
}

<<<<<<< HEAD
static inline void timer_set_next_cmp_ms(u32 msec)
{
	u64 now_ct = timer_get_cycles();
	u64 next_ct = now_ct + msec_to_cycles(msec);
=======
static inline void timer_set_next_cmp_ms(uint32_t msec)
{
	uint64_t now_ct = timer_get_cycles();
	uint64_t next_ct = now_ct + msec_to_cycles(msec);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	timer_set_cmp(next_ct);
}

<<<<<<< HEAD
static inline void __delay(u64 cycles)
{
	u64 start = timer_get_cycles();
=======
static inline void __delay(uint64_t cycles)
{
	uint64_t start = timer_get_cycles();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	while ((timer_get_cycles() - start) < cycles)
		cpu_relax();
}

static inline void udelay(unsigned long usec)
{
	__delay(usec_to_cycles(usec));
}

#endif /* SELFTEST_KVM_ARCH_TIMER_H */
