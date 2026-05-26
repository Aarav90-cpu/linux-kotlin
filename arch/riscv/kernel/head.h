/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2019 SiFive, Inc.
 */
#ifndef __ASM_HEAD_H
#define __ASM_HEAD_H

#include <linux/linkage.h>
#include <linux/init.h>

extern atomic_t hart_lottery;

asmlinkage void __init setup_vm(uintptr_t dtb_pa);
<<<<<<< HEAD
=======
#ifdef CONFIG_XIP_KERNEL
asmlinkage void __init __copy_data(void);
#endif
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#ifdef CONFIG_RISCV_BOOT_SPINWAIT
extern void *__cpu_spinwait_stack_pointer[];
extern void *__cpu_spinwait_task_pointer[];
#endif

#endif /* __ASM_HEAD_H */
