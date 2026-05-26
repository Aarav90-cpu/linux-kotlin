/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright © 2019 Intel Corporation
 */

#ifndef IGT_MMAP_H
#define IGT_MMAP_H

#include <linux/types.h>

struct drm_i915_private;
struct drm_vma_offset_node;
<<<<<<< HEAD
struct file;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

unsigned long igt_mmap_offset(struct drm_i915_private *i915,
			      u64 offset,
			      unsigned long size,
			      unsigned long prot,
			      unsigned long flags);

<<<<<<< HEAD
unsigned long igt_mmap_offset_with_file(struct drm_i915_private *i915,
					u64 offset,
					unsigned long size,
					unsigned long prot,
					unsigned long flags,
					struct file *file);

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif /* IGT_MMAP_H */
