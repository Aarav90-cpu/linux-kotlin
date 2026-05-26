/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2025 Intel Corporation
 */

#ifndef _XE_VM_MADVISE_H_
#define _XE_VM_MADVISE_H_

struct drm_device;
struct drm_file;
<<<<<<< HEAD
struct xe_bo;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

int xe_vm_madvise_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file);

#endif
