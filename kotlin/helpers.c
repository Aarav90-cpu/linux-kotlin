// SPDX-License-Identifier: GPL-2.0
/*
 * Kotlin/Native kernel helper functions.
 *
 * These C functions serve as the FFI bridge between Kotlin/Native code
 * and the Linux kernel's internal APIs. Kotlin/Native's cinterop mechanism
 * generates Kotlin stubs that call these helpers, which in turn call the
 * actual kernel functions (many of which are static inlines or macros
 * that cannot be called directly from foreign languages).
 *
 * This is analogous to rust/helpers/helpers.c in the Rust integration.
 */

#include <linux/bug.h>
#include <linux/build_bug.h>
#include <linux/err.h>
#include <linux/errname.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/printk.h>
#include <linux/refcount.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>

/*
 * printk helpers - Kotlin code calls these since printk() is a variadic
 * macro that cannot be called directly via FFI.
 */
void kotlin_helper_printk_emit(int level, const char *msg)
{
	switch (level) {
	case 0: /* KERN_EMERG */
		pr_emerg("%s", msg);
		break;
	case 1: /* KERN_ALERT */
		pr_alert("%s", msg);
		break;
	case 2: /* KERN_CRIT */
		pr_crit("%s", msg);
		break;
	case 3: /* KERN_ERR */
		pr_err("%s", msg);
		break;
	case 4: /* KERN_WARNING */
		pr_warn("%s", msg);
		break;
	case 5: /* KERN_NOTICE */
		pr_notice("%s", msg);
		break;
	case 6: /* KERN_INFO */
		pr_info("%s", msg);
		break;
	case 7: /* KERN_DEBUG */
		pr_debug("%s", msg);
		break;
	default:
		pr_info("%s", msg);
		break;
	}
}
EXPORT_SYMBOL_GPL(kotlin_helper_printk_emit);

/*
 * Memory allocation helpers.
 */
void *kotlin_helper_kmalloc(size_t size, gfp_t flags)
{
	return kmalloc(size, flags);
}
EXPORT_SYMBOL_GPL(kotlin_helper_kmalloc);

void kotlin_helper_kfree(const void *ptr)
{
	kfree(ptr);
}
EXPORT_SYMBOL_GPL(kotlin_helper_kfree);

void *kotlin_helper_kzalloc(size_t size, gfp_t flags)
{
	return kzalloc(size, flags);
}
EXPORT_SYMBOL_GPL(kotlin_helper_kzalloc);

void *kotlin_helper_krealloc(const void *p, size_t new_size, gfp_t flags)
{
	return krealloc(p, new_size, flags);
}
EXPORT_SYMBOL_GPL(kotlin_helper_krealloc);

/*
 * Error pointer helpers.
 */
long kotlin_helper_IS_ERR(const void *ptr)
{
	return IS_ERR(ptr);
}
EXPORT_SYMBOL_GPL(kotlin_helper_IS_ERR);

long kotlin_helper_PTR_ERR(const void *ptr)
{
	return PTR_ERR(ptr);
}
EXPORT_SYMBOL_GPL(kotlin_helper_PTR_ERR);

void *kotlin_helper_ERR_PTR(long error)
{
	return ERR_PTR(error);
}
EXPORT_SYMBOL_GPL(kotlin_helper_ERR_PTR);

/*
 * Mutex helpers.
 */
void kotlin_helper_mutex_lock(struct mutex *lock)
{
	mutex_lock(lock);
}
EXPORT_SYMBOL_GPL(kotlin_helper_mutex_lock);

void kotlin_helper_mutex_unlock(struct mutex *lock)
{
	mutex_unlock(lock);
}
EXPORT_SYMBOL_GPL(kotlin_helper_mutex_unlock);

int kotlin_helper_mutex_trylock(struct mutex *lock)
{
	return mutex_trylock(lock);
}
EXPORT_SYMBOL_GPL(kotlin_helper_mutex_trylock);

/*
 * Spinlock helpers.
 */
void kotlin_helper_spin_lock(spinlock_t *lock)
{
	spin_lock(lock);
}
EXPORT_SYMBOL_GPL(kotlin_helper_spin_lock);

void kotlin_helper_spin_unlock(spinlock_t *lock)
{
	spin_unlock(lock);
}
EXPORT_SYMBOL_GPL(kotlin_helper_spin_unlock);

void kotlin_helper_spin_lock_irqsave(spinlock_t *lock, unsigned long *flags)
{
	spin_lock_irqsave(lock, *flags);
}
EXPORT_SYMBOL_GPL(kotlin_helper_spin_lock_irqsave);

void kotlin_helper_spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
	spin_unlock_irqrestore(lock, flags);
}
EXPORT_SYMBOL_GPL(kotlin_helper_spin_unlock_irqrestore);

/*
 * Refcount helpers.
 */
void kotlin_helper_refcount_set(refcount_t *r, unsigned int n)
{
	refcount_set(r, n);
}
EXPORT_SYMBOL_GPL(kotlin_helper_refcount_set);

unsigned int kotlin_helper_refcount_read(const refcount_t *r)
{
	return refcount_read(r);
}
EXPORT_SYMBOL_GPL(kotlin_helper_refcount_read);

void kotlin_helper_refcount_inc(refcount_t *r)
{
	refcount_inc(r);
}
EXPORT_SYMBOL_GPL(kotlin_helper_refcount_inc);

bool kotlin_helper_refcount_dec_and_test(refcount_t *r)
{
	return refcount_dec_and_test(r);
}
EXPORT_SYMBOL_GPL(kotlin_helper_refcount_dec_and_test);

/*
 * Copy to/from userspace helpers.
 */
unsigned long kotlin_helper_copy_from_user(void *to, const void __user *from,
					   unsigned long n)
{
	return copy_from_user(to, from, n);
}
EXPORT_SYMBOL_GPL(kotlin_helper_copy_from_user);

unsigned long kotlin_helper_copy_to_user(void __user *to, const void *from,
					 unsigned long n)
{
	return copy_to_user(to, from, n);
}
EXPORT_SYMBOL_GPL(kotlin_helper_copy_to_user);

/*
 * Module registration helpers.
 * Kotlin modules register via these C trampolines.
 */

/* Callback function pointers set by Kotlin modules */
struct kotlin_module_ops {
	int (*init)(void);
	void (*exit)(void);
};

static struct kotlin_module_ops kotlin_ops;

void kotlin_helper_register_module(int (*init_fn)(void),
				   void (*exit_fn)(void))
{
	kotlin_ops.init = init_fn;
	kotlin_ops.exit = exit_fn;
}
EXPORT_SYMBOL_GPL(kotlin_helper_register_module);

int kotlin_helper_module_init_trampoline(void)
{
	if (kotlin_ops.init)
		return kotlin_ops.init();
	return -ENODEV;
}
EXPORT_SYMBOL_GPL(kotlin_helper_module_init_trampoline);

void kotlin_helper_module_exit_trampoline(void)
{
	if (kotlin_ops.exit)
		kotlin_ops.exit();
}
EXPORT_SYMBOL_GPL(kotlin_helper_module_exit_trampoline);
