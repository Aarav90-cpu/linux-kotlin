/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * SHA-1 optimized for ARM64
 *
 * Copyright 2025 Google LLC
 */
#include <asm/simd.h>
#include <linux/cpufeature.h>

static __ro_after_init DEFINE_STATIC_KEY_FALSE(have_ce);

<<<<<<< HEAD
asmlinkage void sha1_ce_transform(struct sha1_block_state *state,
				  const u8 *data, size_t nblocks);
=======
asmlinkage size_t __sha1_ce_transform(struct sha1_block_state *state,
				      const u8 *data, size_t nblocks);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

static void sha1_blocks(struct sha1_block_state *state,
			const u8 *data, size_t nblocks)
{
	if (static_branch_likely(&have_ce) && likely(may_use_simd())) {
<<<<<<< HEAD
		scoped_ksimd()
			sha1_ce_transform(state, data, nblocks);
=======
		do {
			size_t rem;

			scoped_ksimd()
				rem = __sha1_ce_transform(state, data, nblocks);

			data += (nblocks - rem) * SHA1_BLOCK_SIZE;
			nblocks = rem;
		} while (nblocks);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	} else {
		sha1_blocks_generic(state, data, nblocks);
	}
}

#define sha1_mod_init_arch sha1_mod_init_arch
static void sha1_mod_init_arch(void)
{
	if (cpu_have_named_feature(SHA1))
		static_branch_enable(&have_ce);
}
