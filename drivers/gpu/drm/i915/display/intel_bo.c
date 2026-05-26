// SPDX-License-Identifier: MIT
<<<<<<< HEAD
/* Copyright © 2026 Intel Corporation */

#include <drm/drm_gem.h>
#include <drm/intel/display_parent_interface.h>

#include "intel_bo.h"
#include "intel_display_core.h"
#include "intel_display_types.h"

bool intel_bo_is_tiled(struct drm_gem_object *obj)
{
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->is_tiled && display->parent->bo->is_tiled(obj);
=======
/* Copyright © 2024 Intel Corporation */

#include <drm/drm_panic.h>

#include "gem/i915_gem_mman.h"
#include "gem/i915_gem_object.h"
#include "gem/i915_gem_object_frontbuffer.h"
#include "pxp/intel_pxp.h"
#include "i915_debugfs.h"
#include "intel_bo.h"

bool intel_bo_is_tiled(struct drm_gem_object *obj)
{
	return i915_gem_object_is_tiled(to_intel_bo(obj));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

bool intel_bo_is_userptr(struct drm_gem_object *obj)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->is_userptr && display->parent->bo->is_userptr(obj);
=======
	return i915_gem_object_is_userptr(to_intel_bo(obj));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

bool intel_bo_is_shmem(struct drm_gem_object *obj)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->is_shmem && display->parent->bo->is_shmem(obj);
=======
	return i915_gem_object_is_shmem(to_intel_bo(obj));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

bool intel_bo_is_protected(struct drm_gem_object *obj)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->is_protected(obj);
=======
	return i915_gem_object_is_protected(to_intel_bo(obj));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

int intel_bo_key_check(struct drm_gem_object *obj)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->key_check(obj);
=======
	return intel_pxp_key_check(obj, false);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

int intel_bo_fb_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->fb_mmap(obj, vma);
=======
	return i915_gem_fb_mmap(to_intel_bo(obj), vma);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

int intel_bo_read_from_page(struct drm_gem_object *obj, u64 offset, void *dst, int size)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->read_from_page(obj, offset, dst, size);
=======
	return i915_gem_object_read_from_page(to_intel_bo(obj), offset, dst, size);
}

struct intel_frontbuffer *intel_bo_frontbuffer_get(struct drm_gem_object *_obj)
{
	struct drm_i915_gem_object *obj = to_intel_bo(_obj);
	struct i915_frontbuffer *front;

	front = i915_gem_object_frontbuffer_get(obj);
	if (!front)
		return NULL;

	return &front->base;
}

void intel_bo_frontbuffer_ref(struct intel_frontbuffer *_front)
{
	struct i915_frontbuffer *front =
		container_of(_front, typeof(*front), base);

	i915_gem_object_frontbuffer_ref(front);
}

void intel_bo_frontbuffer_put(struct intel_frontbuffer *_front)
{
	struct i915_frontbuffer *front =
		container_of(_front, typeof(*front), base);

	return i915_gem_object_frontbuffer_put(front);
}

void intel_bo_frontbuffer_flush_for_display(struct intel_frontbuffer *_front)
{
	struct i915_frontbuffer *front =
		container_of(_front, typeof(*front), base);

	i915_gem_object_flush_if_display(front->obj);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

void intel_bo_describe(struct seq_file *m, struct drm_gem_object *obj)
{
<<<<<<< HEAD
	struct intel_display *display = to_intel_display(obj->dev);

	if (display->parent->bo->describe)
		display->parent->bo->describe(m, obj);
}

int intel_bo_framebuffer_init(struct drm_gem_object *obj, struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct intel_display *display = to_intel_display(obj->dev);

	return display->parent->bo->framebuffer_init(obj, mode_cmd);
}

void intel_bo_framebuffer_fini(struct drm_gem_object *obj)
{
	struct intel_display *display = to_intel_display(obj->dev);

	display->parent->bo->framebuffer_fini(obj);
}

struct drm_gem_object *intel_bo_framebuffer_lookup(struct intel_display *display,
						   struct drm_file *filp,
						   const struct drm_mode_fb_cmd2 *user_mode_cmd)
{
	return display->parent->bo->framebuffer_lookup(display->drm, filp, user_mode_cmd);
=======
	i915_debugfs_describe_obj(m, to_intel_bo(obj));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
