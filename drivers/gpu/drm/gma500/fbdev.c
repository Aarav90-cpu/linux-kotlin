// SPDX-License-Identifier: GPL-2.0-only
/**************************************************************************
 * Copyright (c) 2007-2011, Intel Corporation.
 * All Rights Reserved.
 *
 **************************************************************************/

#include <linux/fb.h>

#include <drm/drm_crtc_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_framebuffer.h>

#include "gem.h"
#include "psb_drv.h"

/*
 * VM area struct
 */

static vm_fault_t psb_fbdev_vm_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct fb_info *info = vma->vm_private_data;
	unsigned long address = vmf->address - (vmf->pgoff << PAGE_SHIFT);
	unsigned long pfn = info->fix.smem_start >> PAGE_SHIFT;
	vm_fault_t err = VM_FAULT_SIGBUS;
	unsigned long page_num = vma_pages(vma);
	unsigned long i;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	for (i = 0; i < page_num; ++i) {
		err = vmf_insert_mixed(vma, address, pfn);
		if (unlikely(err & VM_FAULT_ERROR))
			break;
		address += PAGE_SIZE;
		++pfn;
	}

	return err;
}

static const struct vm_operations_struct psb_fbdev_vm_ops = {
	.fault	= psb_fbdev_vm_fault,
};

/*
 * struct fb_ops
 */

static int psb_fbdev_fb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	if (vma->vm_pgoff != 0)
		return -EINVAL;
	if (vma->vm_pgoff > (~0UL >> PAGE_SHIFT))
		return -EINVAL;

	/*
	 * If this is a GEM object then info->screen_base is the virtual
	 * kernel remapping of the object. FIXME: Review if this is
	 * suitable for our mmap work
	 */
	vma->vm_ops = &psb_fbdev_vm_ops;
	vma->vm_private_data = info;
	vm_flags_set(vma, VM_IO | VM_MIXEDMAP | VM_DONTEXPAND | VM_DONTDUMP);

	return 0;
}

static void psb_fbdev_fb_destroy(struct fb_info *info)
{
	struct drm_fb_helper *fb_helper = info->par;
<<<<<<< HEAD

	drm_fb_helper_fini(fb_helper);

	drm_client_buffer_delete(fb_helper->buffer);
=======
	struct drm_framebuffer *fb = fb_helper->fb;
	struct drm_gem_object *obj = fb->obj[0];

	drm_fb_helper_fini(fb_helper);

	drm_framebuffer_unregister_private(fb);
	drm_framebuffer_cleanup(fb);
	kfree(fb);

	drm_gem_object_put(obj);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	drm_client_release(&fb_helper->client);
}

static const struct fb_ops psb_fbdev_fb_ops = {
	.owner = THIS_MODULE,
	__FB_DEFAULT_IOMEM_OPS_RDWR,
	DRM_FB_HELPER_DEFAULT_OPS,
	__FB_DEFAULT_IOMEM_OPS_DRAW,
	.fb_mmap = psb_fbdev_fb_mmap,
	.fb_destroy = psb_fbdev_fb_destroy,
};

static const struct drm_fb_helper_funcs psb_fbdev_fb_helper_funcs = {
};

/*
 * struct drm_driver
 */

int psb_fbdev_driver_fbdev_probe(struct drm_fb_helper *fb_helper,
				 struct drm_fb_helper_surface_size *sizes)
{
<<<<<<< HEAD
	struct drm_client_dev *client = &fb_helper->client;
	struct drm_device *dev = client->dev;
	struct drm_file *file = client->file;
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct fb_info *info = fb_helper->info;
	u32 fourcc, pitch;
	u64 size;
	const struct drm_format_info *format;
	struct drm_client_buffer *buffer;
	struct psb_gem_object *backing;
	struct drm_gem_object *obj;
	u32 handle;
	int ret;
=======
	struct drm_device *dev = fb_helper->dev;
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	struct fb_info *info = fb_helper->info;
	struct drm_framebuffer *fb;
	struct drm_mode_fb_cmd2 mode_cmd = { };
	int size;
	int ret;
	struct psb_gem_object *backing;
	struct drm_gem_object *obj;
	u32 bpp, depth;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* No 24-bit packed mode */
	if (sizes->surface_bpp == 24) {
		sizes->surface_bpp = 32;
		sizes->surface_depth = 24;
	}
<<<<<<< HEAD

try_psb_gem_create:
	fourcc = drm_mode_legacy_fb_format(sizes->surface_bpp, sizes->surface_depth);
	format = drm_get_format_info(dev, fourcc, DRM_FORMAT_MOD_LINEAR);
	pitch = ALIGN(drm_format_info_min_pitch(format, 0, sizes->surface_width), SZ_64);
	size = ALIGN(pitch * sizes->surface_height, PAGE_SIZE);

	/* Allocate the framebuffer in the GTT with stolen page backing */
	backing = psb_gem_create(dev, size, "fb", true, PAGE_SIZE);
	if (IS_ERR(backing)) {
		ret = PTR_ERR(backing);
		if (ret == -EBUSY && sizes->surface_bpp > 16) {
			/*
			 * If the mode does not fit in 32 bit then switch to 16 bit to
			 * get a console on full resolution. User-space compositors will
			 * allocate their own 32-bit framebuffers.
			 */
			sizes->surface_bpp = 16;
			sizes->surface_depth = 16;
			goto try_psb_gem_create;
		}
		return ret;
	}
	obj = &backing->base;

	ret = drm_gem_handle_create(file, obj, &handle);
	if (ret)
		goto err_drm_gem_object_put;

	buffer = drm_client_buffer_create(client, sizes->surface_width, sizes->surface_height,
					  fourcc, handle, pitch);
	if (IS_ERR(buffer)) {
		ret = PTR_ERR(buffer);
		goto err_drm_gem_handle_delete;
	}

	fb_helper->funcs = &psb_fbdev_fb_helper_funcs;
	fb_helper->buffer = buffer;
	fb_helper->fb = buffer->fb;
=======
	bpp = sizes->surface_bpp;
	depth = sizes->surface_depth;

	/*
	 * If the mode does not fit in 32 bit then switch to 16 bit to get
	 * a console on full resolution. The X mode setting server will
	 * allocate its own 32-bit GEM framebuffer.
	 */
	size = ALIGN(sizes->surface_width * DIV_ROUND_UP(bpp, 8), 64) *
		     sizes->surface_height;
	size = ALIGN(size, PAGE_SIZE);

	if (size > dev_priv->vram_stolen_size) {
		sizes->surface_bpp = 16;
		sizes->surface_depth = 16;
	}
	bpp = sizes->surface_bpp;
	depth = sizes->surface_depth;

	mode_cmd.width = sizes->surface_width;
	mode_cmd.height = sizes->surface_height;
	mode_cmd.pitches[0] = ALIGN(mode_cmd.width * DIV_ROUND_UP(bpp, 8), 64);
	mode_cmd.pixel_format = drm_mode_legacy_fb_format(bpp, depth);

	size = mode_cmd.pitches[0] * mode_cmd.height;
	size = ALIGN(size, PAGE_SIZE);

	/* Allocate the framebuffer in the GTT with stolen page backing */
	backing = psb_gem_create(dev, size, "fb", true, PAGE_SIZE);
	if (IS_ERR(backing))
		return PTR_ERR(backing);
	obj = &backing->base;

	fb = psb_framebuffer_create(dev,
				    drm_get_format_info(dev, mode_cmd.pixel_format,
							mode_cmd.modifier[0]),
				    &mode_cmd, obj);
	if (IS_ERR(fb)) {
		ret = PTR_ERR(fb);
		goto err_drm_gem_object_put;
	}

	fb_helper->funcs = &psb_fbdev_fb_helper_funcs;
	fb_helper->fb = fb;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	info->fbops = &psb_fbdev_fb_ops;

	/* Accessed stolen memory directly */
	info->screen_base = dev_priv->vram_addr + backing->offset;
<<<<<<< HEAD
	info->screen_size = obj->size;
=======
	info->screen_size = size;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	drm_fb_helper_fill_info(info, fb_helper, sizes);

	info->fix.smem_start = dev_priv->stolen_base + backing->offset;
<<<<<<< HEAD
	info->fix.smem_len = obj->size;
=======
	info->fix.smem_len = size;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	info->fix.ywrapstep = 0;
	info->fix.ypanstep = 0;
	info->fix.mmio_start = pci_resource_start(pdev, 0);
	info->fix.mmio_len = pci_resource_len(pdev, 0);

	fb_memset_io(info->screen_base, 0, info->screen_size);

	/* Use default scratch pixmap (info->pixmap.flags = FB_PIXMAP_SYSTEM) */

<<<<<<< HEAD
	dev_dbg(dev->dev, "allocated %dx%d fb\n", buffer->fb->width, buffer->fb->height);

	/* The handle is only needed for creating the framebuffer.*/
	drm_gem_handle_delete(file, handle);

	/* The framebuffer still holds a references on the GEM object. */
	drm_gem_object_put(obj);

	return 0;

err_drm_gem_handle_delete:
	drm_gem_handle_delete(file, handle);
=======
	dev_dbg(dev->dev, "allocated %dx%d fb\n", fb->width, fb->height);

	return 0;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
err_drm_gem_object_put:
	drm_gem_object_put(obj);
	return ret;
}
