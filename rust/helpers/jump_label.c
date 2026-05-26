// SPDX-License-Identifier: GPL-2.0

/*
 * Copyright (C) 2024 Google LLC.
 */

#include <linux/jump_label.h>

#ifndef CONFIG_JUMP_LABEL
<<<<<<< HEAD
__rust_helper int rust_helper_static_key_count(struct static_key *key)
=======
int rust_helper_static_key_count(struct static_key *key)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return static_key_count(key);
}
#endif
