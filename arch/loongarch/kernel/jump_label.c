// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2023 Loongson Technology Corporation Limited
 *
 * Based on arch/arm64/kernel/jump_label.c
 */
#include <linux/kernel.h>
#include <linux/jump_label.h>
<<<<<<< HEAD
#include <asm/cacheflush.h>
#include <asm/inst.h>

bool arch_jump_label_transform_queue(struct jump_entry *entry, enum jump_label_type type)
=======
#include <asm/inst.h>

void arch_jump_label_transform(struct jump_entry *entry, enum jump_label_type type)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	u32 insn;
	void *addr = (void *)jump_entry_code(entry);

	if (type == JUMP_LABEL_JMP)
		insn = larch_insn_gen_b(jump_entry_code(entry), jump_entry_target(entry));
	else
		insn = larch_insn_gen_nop();

<<<<<<< HEAD
	larch_insn_write(addr, insn);

	return true;
}

void arch_jump_label_transform_apply(void)
{
	flush_icache_all();
=======
	larch_insn_patch_text(addr, insn);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
