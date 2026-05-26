// SPDX-License-Identifier: GPL-2.0+
/*
 * Unit test for the clocksource watchdog.
 *
 * Copyright (C) 2021 Facebook, Inc.
<<<<<<< HEAD
 * Copyright (C) 2026 Intel Corp.
 *
 * Author: Paul E. McKenney <paulmck@kernel.org>
 * Author: Thomas Gleixner <tglx@kernel.org>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/clocksource.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kthread.h>

#include "tick-internal.h"
#include "timekeeping_internal.h"
=======
 *
 * Author: Paul E. McKenney <paulmck@kernel.org>
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/device.h>
#include <linux/clocksource.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h> /* for spin_unlock_irq() using preempt_count() m68k */
#include <linux/tick.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/prandom.h>
#include <linux/cpu.h>

#include "tick-internal.h"
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Clocksource watchdog unit test");
MODULE_AUTHOR("Paul E. McKenney <paulmck@kernel.org>");
<<<<<<< HEAD
MODULE_AUTHOR("Thomas Gleixner <tglx@kernel.org>");

enum wdtest_states {
	WDTEST_INJECT_NONE,
	WDTEST_INJECT_DELAY,
	WDTEST_INJECT_POSITIVE,
	WDTEST_INJECT_NEGATIVE,
	WDTEST_INJECT_PERCPU	= 0x100,
};

static enum wdtest_states wdtest_state;
static unsigned long wdtest_test_count;
static ktime_t wdtest_last_ts, wdtest_offset;

#define SHIFT_4000PPM	8

static ktime_t wdtest_get_offset(struct clocksource *cs)
{
	if (wdtest_state < WDTEST_INJECT_PERCPU)
		return wdtest_test_count & 0x1 ? 0 : wdtest_offset >> SHIFT_4000PPM;

	/* Only affect the readout of the "remote" CPU */
	return cs->wd_cpu == smp_processor_id() ? 0 : NSEC_PER_MSEC;
}

static u64 wdtest_ktime_read(struct clocksource *cs)
{
	ktime_t now = ktime_get_raw_fast_ns();
	ktime_t intv = now - wdtest_last_ts;

	/*
	 * Only increment the test counter once per watchdog interval and
	 * store the interval for the offset calculation of this step. This
	 * guarantees a consistent behaviour even if the other side needs
	 * to repeat due to a watchdog read timeout.
	 */
	if (intv > (NSEC_PER_SEC / 4)) {
		WRITE_ONCE(wdtest_test_count, wdtest_test_count + 1);
		wdtest_last_ts = now;
		wdtest_offset = intv;
	}

	switch (wdtest_state & ~WDTEST_INJECT_PERCPU) {
	case WDTEST_INJECT_POSITIVE:
		return now + wdtest_get_offset(cs);
	case WDTEST_INJECT_NEGATIVE:
		return now - wdtest_get_offset(cs);
	case WDTEST_INJECT_DELAY:
		udelay(500);
		return now;
	default:
		return now;
	}
}

#define KTIME_FLAGS (CLOCK_SOURCE_IS_CONTINUOUS |	\
		     CLOCK_SOURCE_CALIBRATED |		\
		     CLOCK_SOURCE_MUST_VERIFY |		\
		     CLOCK_SOURCE_WDTEST)

static struct clocksource clocksource_wdtest_ktime = {
	.name			= "wdtest-ktime",
	.rating			= 10,
	.read			= wdtest_ktime_read,
	.mask			= CLOCKSOURCE_MASK(64),
	.flags			= KTIME_FLAGS,
	.list			= LIST_HEAD_INIT(clocksource_wdtest_ktime.list),
};

static void wdtest_clocksource_reset(enum wdtest_states which, bool percpu)
{
	clocksource_unregister(&clocksource_wdtest_ktime);

	pr_info("Test: State %d percpu %d\n", which, percpu);

	wdtest_state = which;
	if (percpu)
		wdtest_state |= WDTEST_INJECT_PERCPU;
	wdtest_test_count = 0;
	wdtest_last_ts = 0;

	clocksource_wdtest_ktime.rating = 10;
	clocksource_wdtest_ktime.flags = KTIME_FLAGS;
	if (percpu)
		clocksource_wdtest_ktime.flags |= CLOCK_SOURCE_WDTEST_PERCPU;
	clocksource_register_khz(&clocksource_wdtest_ktime, 1000 * 1000);
}

static bool wdtest_execute(enum wdtest_states which, bool percpu, unsigned int expect,
			   unsigned long calls)
{
	wdtest_clocksource_reset(which, percpu);

	for (; READ_ONCE(wdtest_test_count) < calls; msleep(100)) {
		unsigned int flags = READ_ONCE(clocksource_wdtest_ktime.flags);

		if (kthread_should_stop())
			return false;

		if (flags & CLOCK_SOURCE_UNSTABLE) {
			if (expect & CLOCK_SOURCE_UNSTABLE)
				return true;
			pr_warn("Fail: Unexpected unstable\n");
			return false;
		}
		if (flags & CLOCK_SOURCE_VALID_FOR_HRES) {
			if (expect & CLOCK_SOURCE_VALID_FOR_HRES)
				return true;
			pr_warn("Fail: Unexpected valid for highres\n");
			return false;
		}
	}

	if (!expect)
		return true;

	pr_warn("Fail: Timed out\n");
	return false;
}

static bool wdtest_run(bool percpu)
{
	if (!wdtest_execute(WDTEST_INJECT_NONE, percpu, CLOCK_SOURCE_VALID_FOR_HRES, 8))
		return false;

	if (!wdtest_execute(WDTEST_INJECT_DELAY, percpu, 0, 4))
		return false;

	if (!wdtest_execute(WDTEST_INJECT_POSITIVE, percpu, CLOCK_SOURCE_UNSTABLE, 8))
		return false;

	if (!wdtest_execute(WDTEST_INJECT_NEGATIVE, percpu, CLOCK_SOURCE_UNSTABLE, 8))
		return false;

	return true;
}

static int wdtest_func(void *arg)
{
	clocksource_register_khz(&clocksource_wdtest_ktime, 1000 * 1000);
	if (wdtest_run(false)) {
		if (wdtest_run(true))
			pr_info("Success: All tests passed\n");
	}
	clocksource_unregister(&clocksource_wdtest_ktime);

	if (!IS_MODULE(CONFIG_TEST_CLOCKSOURCE_WATCHDOG))
		return 0;

	while (!kthread_should_stop())
		schedule_timeout_interruptible(3600 * HZ);
	return 0;
}

static struct task_struct *wdtest_thread;

static int __init clocksource_wdtest_init(void)
{
	struct task_struct *t = kthread_run(wdtest_func, NULL, "wdtest");

	if (IS_ERR(t)) {
		pr_warn("Failed to create wdtest kthread.\n");
		return PTR_ERR(t);
	}
	wdtest_thread = t;
	return 0;
}
module_init(clocksource_wdtest_init);

static void clocksource_wdtest_cleanup(void)
{
	if (wdtest_thread)
		kthread_stop(wdtest_thread);
}
=======

static int holdoff = IS_BUILTIN(CONFIG_TEST_CLOCKSOURCE_WATCHDOG) ? 10 : 0;
module_param(holdoff, int, 0444);
MODULE_PARM_DESC(holdoff, "Time to wait to start test (s).");

/* Watchdog kthread's task_struct pointer for debug purposes. */
static struct task_struct *wdtest_task;

static u64 wdtest_jiffies_read(struct clocksource *cs)
{
	return (u64)jiffies;
}

static struct clocksource clocksource_wdtest_jiffies = {
	.name			= "wdtest-jiffies",
	.rating			= 1, /* lowest valid rating*/
	.uncertainty_margin	= TICK_NSEC,
	.read			= wdtest_jiffies_read,
	.mask			= CLOCKSOURCE_MASK(32),
	.flags			= CLOCK_SOURCE_MUST_VERIFY,
	.mult			= TICK_NSEC << JIFFIES_SHIFT, /* details above */
	.shift			= JIFFIES_SHIFT,
	.max_cycles		= 10,
};

static int wdtest_ktime_read_ndelays;
static bool wdtest_ktime_read_fuzz;

static u64 wdtest_ktime_read(struct clocksource *cs)
{
	int wkrn = READ_ONCE(wdtest_ktime_read_ndelays);
	static int sign = 1;
	u64 ret;

	if (wkrn) {
		udelay(cs->uncertainty_margin / 250);
		WRITE_ONCE(wdtest_ktime_read_ndelays, wkrn - 1);
	}
	ret = ktime_get_real_fast_ns();
	if (READ_ONCE(wdtest_ktime_read_fuzz)) {
		sign = -sign;
		ret = ret + sign * 100 * NSEC_PER_MSEC;
	}
	return ret;
}

static void wdtest_ktime_cs_mark_unstable(struct clocksource *cs)
{
	pr_info("--- Marking %s unstable due to clocksource watchdog.\n", cs->name);
}

#define KTIME_FLAGS (CLOCK_SOURCE_IS_CONTINUOUS | \
		     CLOCK_SOURCE_VALID_FOR_HRES | \
		     CLOCK_SOURCE_MUST_VERIFY | \
		     CLOCK_SOURCE_VERIFY_PERCPU)

static struct clocksource clocksource_wdtest_ktime = {
	.name			= "wdtest-ktime",
	.rating			= 300,
	.read			= wdtest_ktime_read,
	.mask			= CLOCKSOURCE_MASK(64),
	.flags			= KTIME_FLAGS,
	.mark_unstable		= wdtest_ktime_cs_mark_unstable,
	.list			= LIST_HEAD_INIT(clocksource_wdtest_ktime.list),
};

/* Reset the clocksource if needed. */
static void wdtest_ktime_clocksource_reset(void)
{
	if (clocksource_wdtest_ktime.flags & CLOCK_SOURCE_UNSTABLE) {
		clocksource_unregister(&clocksource_wdtest_ktime);
		clocksource_wdtest_ktime.flags = KTIME_FLAGS;
		schedule_timeout_uninterruptible(HZ / 10);
		clocksource_register_khz(&clocksource_wdtest_ktime, 1000 * 1000);
	}
}

/* Run the specified series of watchdog tests. */
static int wdtest_func(void *arg)
{
	unsigned long j1, j2;
	int i, max_retries;
	char *s;

	schedule_timeout_uninterruptible(holdoff * HZ);

	/*
	 * Verify that jiffies-like clocksources get the manually
	 * specified uncertainty margin.
	 */
	pr_info("--- Verify jiffies-like uncertainty margin.\n");
	__clocksource_register(&clocksource_wdtest_jiffies);
	WARN_ON_ONCE(clocksource_wdtest_jiffies.uncertainty_margin != TICK_NSEC);

	j1 = clocksource_wdtest_jiffies.read(&clocksource_wdtest_jiffies);
	schedule_timeout_uninterruptible(HZ);
	j2 = clocksource_wdtest_jiffies.read(&clocksource_wdtest_jiffies);
	WARN_ON_ONCE(j1 == j2);

	clocksource_unregister(&clocksource_wdtest_jiffies);

	/*
	 * Verify that tsc-like clocksources are assigned a reasonable
	 * uncertainty margin.
	 */
	pr_info("--- Verify tsc-like uncertainty margin.\n");
	clocksource_register_khz(&clocksource_wdtest_ktime, 1000 * 1000);
	WARN_ON_ONCE(clocksource_wdtest_ktime.uncertainty_margin < NSEC_PER_USEC);

	j1 = clocksource_wdtest_ktime.read(&clocksource_wdtest_ktime);
	udelay(1);
	j2 = clocksource_wdtest_ktime.read(&clocksource_wdtest_ktime);
	pr_info("--- tsc-like times: %lu - %lu = %lu.\n", j2, j1, j2 - j1);
	WARN_ONCE(time_before(j2, j1 + NSEC_PER_USEC),
		  "Expected at least 1000ns, got %lu.\n", j2 - j1);

	/* Verify tsc-like stability with various numbers of errors injected. */
	max_retries = clocksource_get_max_watchdog_retry();
	for (i = 0; i <= max_retries + 1; i++) {
		if (i <= 1 && i < max_retries)
			s = "";
		else if (i <= max_retries)
			s = ", expect message";
		else
			s = ", expect clock skew";
		pr_info("--- Watchdog with %dx error injection, %d retries%s.\n", i, max_retries, s);
		WRITE_ONCE(wdtest_ktime_read_ndelays, i);
		schedule_timeout_uninterruptible(2 * HZ);
		WARN_ON_ONCE(READ_ONCE(wdtest_ktime_read_ndelays));
		WARN_ON_ONCE((i <= max_retries) !=
			     !(clocksource_wdtest_ktime.flags & CLOCK_SOURCE_UNSTABLE));
		wdtest_ktime_clocksource_reset();
	}

	/* Verify tsc-like stability with clock-value-fuzz error injection. */
	pr_info("--- Watchdog clock-value-fuzz error injection, expect clock skew and per-CPU mismatches.\n");
	WRITE_ONCE(wdtest_ktime_read_fuzz, true);
	schedule_timeout_uninterruptible(2 * HZ);
	WARN_ON_ONCE(!(clocksource_wdtest_ktime.flags & CLOCK_SOURCE_UNSTABLE));
	clocksource_verify_percpu(&clocksource_wdtest_ktime);
	WRITE_ONCE(wdtest_ktime_read_fuzz, false);

	clocksource_unregister(&clocksource_wdtest_ktime);

	pr_info("--- Done with test.\n");
	return 0;
}

static void wdtest_print_module_parms(void)
{
	pr_alert("--- holdoff=%d\n", holdoff);
}

/* Cleanup function. */
static void clocksource_wdtest_cleanup(void)
{
}

static int __init clocksource_wdtest_init(void)
{
	int ret = 0;

	wdtest_print_module_parms();

	/* Create watchdog-test task. */
	wdtest_task = kthread_run(wdtest_func, NULL, "wdtest");
	if (IS_ERR(wdtest_task)) {
		ret = PTR_ERR(wdtest_task);
		pr_warn("%s: Failed to create wdtest kthread.\n", __func__);
		wdtest_task = NULL;
		return ret;
	}

	return 0;
}

module_init(clocksource_wdtest_init);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
module_exit(clocksource_wdtest_cleanup);
