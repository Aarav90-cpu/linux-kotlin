/*
 *  linux/drivers/video/fb_defio.c
 *
 *  Copyright (C) 2006 Jaya Kumar
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/list.h>

/* to support deferred IO */
#include <linux/rmap.h>
#include <linux/pagemap.h>

<<<<<<< HEAD
struct address_space;

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
/*
 * struct fb_deferred_io_state
 */

struct fb_deferred_io_state {
	struct kref ref;

<<<<<<< HEAD
	int open_count; /* number of opened files; protected by fb_info lock */
	struct address_space *mapping; /* page cache object for fb device */

	struct mutex lock; /* mutex that protects the pageref list */
	/* fields protected by lock */
	struct fb_info *info;
	struct list_head pagereflist; /* list of pagerefs for touched pages */
	unsigned long npagerefs;
	struct fb_deferred_io_pageref *pagerefs;
};

static struct fb_deferred_io_state *fb_deferred_io_state_alloc(unsigned long len)
{
	struct fb_deferred_io_state *fbdefio_state;
	struct fb_deferred_io_pageref *pagerefs;
	unsigned long npagerefs;
=======
	struct mutex lock; /* mutex that protects the pageref list */
	/* fields protected by lock */
	struct fb_info *info;
};

static struct fb_deferred_io_state *fb_deferred_io_state_alloc(void)
{
	struct fb_deferred_io_state *fbdefio_state;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	fbdefio_state = kzalloc_obj(*fbdefio_state);
	if (!fbdefio_state)
		return NULL;

<<<<<<< HEAD
	npagerefs = DIV_ROUND_UP(len, PAGE_SIZE);

	/* alloc a page ref for each page of the display memory */
	pagerefs = kvzalloc_objs(*pagerefs, npagerefs);
	if (!pagerefs)
		goto err_kfree;
	fbdefio_state->npagerefs = npagerefs;
	fbdefio_state->pagerefs = pagerefs;

	kref_init(&fbdefio_state->ref);
	mutex_init(&fbdefio_state->lock);

	INIT_LIST_HEAD(&fbdefio_state->pagereflist);

	return fbdefio_state;

err_kfree:
	kfree(fbdefio_state);
	return NULL;
=======
	kref_init(&fbdefio_state->ref);
	mutex_init(&fbdefio_state->lock);

	return fbdefio_state;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void fb_deferred_io_state_release(struct fb_deferred_io_state *fbdefio_state)
{
<<<<<<< HEAD
	WARN_ON(!list_empty(&fbdefio_state->pagereflist));
	mutex_destroy(&fbdefio_state->lock);
	kvfree(fbdefio_state->pagerefs);
=======
	mutex_destroy(&fbdefio_state->lock);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	kfree(fbdefio_state);
}

static void fb_deferred_io_state_get(struct fb_deferred_io_state *fbdefio_state)
{
	kref_get(&fbdefio_state->ref);
}

static void __fb_deferred_io_state_release(struct kref *ref)
{
	struct fb_deferred_io_state *fbdefio_state =
		container_of(ref, struct fb_deferred_io_state, ref);

	fb_deferred_io_state_release(fbdefio_state);
}

static void fb_deferred_io_state_put(struct fb_deferred_io_state *fbdefio_state)
{
	kref_put(&fbdefio_state->ref, __fb_deferred_io_state_release);
}

/*
 * struct vm_operations_struct
 */

static void fb_deferred_io_vm_open(struct vm_area_struct *vma)
{
	struct fb_deferred_io_state *fbdefio_state = vma->vm_private_data;

<<<<<<< HEAD
	WARN_ON_ONCE(!try_module_get(THIS_MODULE));
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	fb_deferred_io_state_get(fbdefio_state);
}

static void fb_deferred_io_vm_close(struct vm_area_struct *vma)
{
	struct fb_deferred_io_state *fbdefio_state = vma->vm_private_data;

	fb_deferred_io_state_put(fbdefio_state);
<<<<<<< HEAD
	module_put(THIS_MODULE);
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static struct page *fb_deferred_io_get_page(struct fb_info *info, unsigned long offs)
{
	struct fb_deferred_io *fbdefio = info->fbdefio;
	const void *screen_buffer = info->screen_buffer;
	struct page *page = NULL;

	if (fbdefio->get_page)
		return fbdefio->get_page(info, offs);

	if (is_vmalloc_addr(screen_buffer + offs))
		page = vmalloc_to_page(screen_buffer + offs);
	else if (info->fix.smem_start)
		page = pfn_to_page((info->fix.smem_start + offs) >> PAGE_SHIFT);

	if (page)
		get_page(page);

	return page;
}

<<<<<<< HEAD
static struct fb_deferred_io_pageref *
fb_deferred_io_pageref_lookup(struct fb_deferred_io_state *fbdefio_state, unsigned long offset,
			      struct page *page)
{
	struct fb_info *info = fbdefio_state->info;
	unsigned long pgoff = offset >> PAGE_SHIFT;
	struct fb_deferred_io_pageref *pageref;

	if (fb_WARN_ON_ONCE(info, pgoff >= fbdefio_state->npagerefs))
		return NULL; /* incorrect allocation size */

	/* 1:1 mapping between pageref and page offset */
	pageref = &fbdefio_state->pagerefs[pgoff];
=======
static struct fb_deferred_io_pageref *fb_deferred_io_pageref_lookup(struct fb_info *info,
								    unsigned long offset,
								    struct page *page)
{
	unsigned long pgoff = offset >> PAGE_SHIFT;
	struct fb_deferred_io_pageref *pageref;

	if (fb_WARN_ON_ONCE(info, pgoff >= info->npagerefs))
		return NULL; /* incorrect allocation size */

	/* 1:1 mapping between pageref and page offset */
	pageref = &info->pagerefs[pgoff];
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (pageref->page)
		goto out;

	pageref->page = page;
	pageref->offset = pgoff << PAGE_SHIFT;
	INIT_LIST_HEAD(&pageref->list);

out:
	if (fb_WARN_ON_ONCE(info, pageref->page != page))
		return NULL; /* inconsistent state */
	return pageref;
}

static struct fb_deferred_io_pageref *fb_deferred_io_pageref_get(struct fb_info *info,
								 unsigned long offset,
								 struct page *page)
{
	struct fb_deferred_io *fbdefio = info->fbdefio;
<<<<<<< HEAD
	struct fb_deferred_io_state *fbdefio_state = info->fbdefio_state;
	struct list_head *pos = &fbdefio_state->pagereflist;
	struct fb_deferred_io_pageref *pageref, *cur;

	pageref = fb_deferred_io_pageref_lookup(fbdefio_state, offset, page);
=======
	struct list_head *pos = &fbdefio->pagereflist;
	struct fb_deferred_io_pageref *pageref, *cur;

	pageref = fb_deferred_io_pageref_lookup(info, offset, page);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!pageref)
		return NULL;

	/*
	 * This check is to catch the case where a new process could start
	 * writing to the same page through a new PTE. This new access
	 * can cause a call to .page_mkwrite even if the original process'
	 * PTE is marked writable.
	 */
	if (!list_empty(&pageref->list))
		goto pageref_already_added;

	if (unlikely(fbdefio->sort_pagereflist)) {
		/*
		 * We loop through the list of pagerefs before adding in
		 * order to keep the pagerefs sorted. This has significant
		 * overhead of O(n^2) with n being the number of written
		 * pages. If possible, drivers should try to work with
		 * unsorted page lists instead.
		 */
<<<<<<< HEAD
		list_for_each_entry(cur, &fbdefio_state->pagereflist, list) {
=======
		list_for_each_entry(cur, &fbdefio->pagereflist, list) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
			if (cur->offset > pageref->offset)
				break;
		}
		pos = &cur->list;
	}

	list_add_tail(&pageref->list, pos);

pageref_already_added:
	return pageref;
}

static void fb_deferred_io_pageref_put(struct fb_deferred_io_pageref *pageref,
				       struct fb_info *info)
{
	list_del_init(&pageref->list);
}

/* this is to find and return the vmalloc-ed fb pages */
static vm_fault_t fb_deferred_io_fault(struct vm_fault *vmf)
{
	struct fb_info *info;
	unsigned long offset;
	struct page *page;
	vm_fault_t ret;
	struct fb_deferred_io_state *fbdefio_state = vmf->vma->vm_private_data;

	mutex_lock(&fbdefio_state->lock);

	info = fbdefio_state->info;
	if (!info) {
		ret = VM_FAULT_SIGBUS; /* our device is gone */
		goto err_mutex_unlock;
	}

	offset = vmf->pgoff << PAGE_SHIFT;
	if (offset >= info->fix.smem_len) {
		ret = VM_FAULT_SIGBUS;
		goto err_mutex_unlock;
	}

	page = fb_deferred_io_get_page(info, offset);
	if (!page) {
		ret = VM_FAULT_SIGBUS;
		goto err_mutex_unlock;
	}

	if (!vmf->vma->vm_file)
		fb_err(info, "no mapping available\n");

<<<<<<< HEAD
	fb_WARN_ON_ONCE(info, !fbdefio_state->mapping);
=======
	BUG_ON(!info->fbdefio->mapping);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	mutex_unlock(&fbdefio_state->lock);

	vmf->page = page;

	return 0;

err_mutex_unlock:
	mutex_unlock(&fbdefio_state->lock);
	return ret;
}

int fb_deferred_io_fsync(struct file *file, loff_t start, loff_t end, int datasync)
{
	struct fb_info *info = file->private_data;
	struct inode *inode = file_inode(file);
	int err = file_write_and_wait_range(file, start, end);
	if (err)
		return err;

	/* Skip if deferred io is compiled-in but disabled on this fbdev */
	if (!info->fbdefio)
		return 0;

	inode_lock(inode);
	flush_delayed_work(&info->deferred_work);
	inode_unlock(inode);

	return 0;
}
EXPORT_SYMBOL_GPL(fb_deferred_io_fsync);

/*
 * Adds a page to the dirty list. Call this from struct
 * vm_operations_struct.page_mkwrite.
 */
static vm_fault_t fb_deferred_io_track_page(struct fb_deferred_io_state *fbdefio_state,
					    unsigned long offset, struct page *page)
{
	struct fb_info *info;
	struct fb_deferred_io *fbdefio;
	struct fb_deferred_io_pageref *pageref;
	vm_fault_t ret;

	/* protect against the workqueue changing the page list */
	mutex_lock(&fbdefio_state->lock);

	info = fbdefio_state->info;
	if (!info) {
		ret = VM_FAULT_SIGBUS; /* our device is gone */
		goto err_mutex_unlock;
	}

	fbdefio = info->fbdefio;

	pageref = fb_deferred_io_pageref_get(info, offset, page);
	if (WARN_ON_ONCE(!pageref)) {
		ret = VM_FAULT_OOM;
		goto err_mutex_unlock;
	}

	/*
	 * We want the page to remain locked from ->page_mkwrite until
	 * the PTE is marked dirty to avoid mapping_wrprotect_range()
	 * being called before the PTE is updated, which would leave
	 * the page ignored by defio.
	 * Do this by locking the page here and informing the caller
	 * about it with VM_FAULT_LOCKED.
	 */
	lock_page(pageref->page);

	mutex_unlock(&fbdefio_state->lock);

	/* come back after delay to process the deferred IO */
	schedule_delayed_work(&info->deferred_work, fbdefio->delay);
	return VM_FAULT_LOCKED;

err_mutex_unlock:
	mutex_unlock(&fbdefio_state->lock);
	return ret;
}

static vm_fault_t fb_deferred_io_page_mkwrite(struct fb_deferred_io_state *fbdefio_state,
					      struct vm_fault *vmf)
{
	unsigned long offset = vmf->pgoff << PAGE_SHIFT;
	struct page *page = vmf->page;

	file_update_time(vmf->vma->vm_file);

	return fb_deferred_io_track_page(fbdefio_state, offset, page);
}

static vm_fault_t fb_deferred_io_mkwrite(struct vm_fault *vmf)
{
	struct fb_deferred_io_state *fbdefio_state = vmf->vma->vm_private_data;

	return fb_deferred_io_page_mkwrite(fbdefio_state, vmf);
}

static const struct vm_operations_struct fb_deferred_io_vm_ops = {
	.open		= fb_deferred_io_vm_open,
	.close		= fb_deferred_io_vm_close,
	.fault		= fb_deferred_io_fault,
	.page_mkwrite	= fb_deferred_io_mkwrite,
};

static const struct address_space_operations fb_deferred_io_aops = {
	.dirty_folio	= noop_dirty_folio,
};

int fb_deferred_io_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
	vma->vm_page_prot = pgprot_decrypted(vma->vm_page_prot);

<<<<<<< HEAD
	if (!try_module_get(THIS_MODULE))
		return -EINVAL;

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	vma->vm_ops = &fb_deferred_io_vm_ops;
	vm_flags_set(vma, VM_DONTEXPAND | VM_DONTDUMP);
	if (!(info->flags & FBINFO_VIRTFB))
		vm_flags_set(vma, VM_IO);
	vma->vm_private_data = info->fbdefio_state;

	fb_deferred_io_state_get(info->fbdefio_state); /* released in vma->vm_ops->close() */

	return 0;
}
EXPORT_SYMBOL_GPL(fb_deferred_io_mmap);

/* workqueue callback */
static void fb_deferred_io_work(struct work_struct *work)
{
	struct fb_info *info = container_of(work, struct fb_info, deferred_work.work);
	struct fb_deferred_io_pageref *pageref, *next;
	struct fb_deferred_io *fbdefio = info->fbdefio;
	struct fb_deferred_io_state *fbdefio_state = info->fbdefio_state;

	/* here we wrprotect the page's mappings, then do all deferred IO. */
	mutex_lock(&fbdefio_state->lock);
#ifdef CONFIG_MMU
<<<<<<< HEAD
	list_for_each_entry(pageref, &fbdefio_state->pagereflist, list) {
		struct page *page = pageref->page;
		pgoff_t pgoff = pageref->offset >> PAGE_SHIFT;

		mapping_wrprotect_range(fbdefio_state->mapping, pgoff,
=======
	list_for_each_entry(pageref, &fbdefio->pagereflist, list) {
		struct page *page = pageref->page;
		pgoff_t pgoff = pageref->offset >> PAGE_SHIFT;

		mapping_wrprotect_range(fbdefio->mapping, pgoff,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
					page_to_pfn(page), 1);
	}
#endif

	/* driver's callback with pagereflist */
<<<<<<< HEAD
	fbdefio->deferred_io(info, &fbdefio_state->pagereflist);

	/* clear the list */
	list_for_each_entry_safe(pageref, next, &fbdefio_state->pagereflist, list)
=======
	fbdefio->deferred_io(info, &fbdefio->pagereflist);

	/* clear the list */
	list_for_each_entry_safe(pageref, next, &fbdefio->pagereflist, list)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		fb_deferred_io_pageref_put(pageref, info);

	mutex_unlock(&fbdefio_state->lock);
}

int fb_deferred_io_init(struct fb_info *info)
{
	struct fb_deferred_io *fbdefio = info->fbdefio;
	struct fb_deferred_io_state *fbdefio_state;
<<<<<<< HEAD
=======
	struct fb_deferred_io_pageref *pagerefs;
	unsigned long npagerefs;
	int ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	BUG_ON(!fbdefio);

	if (WARN_ON(!info->fix.smem_len))
		return -EINVAL;

<<<<<<< HEAD
	fbdefio_state = fb_deferred_io_state_alloc(info->fix.smem_len);
=======
	fbdefio_state = fb_deferred_io_state_alloc();
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!fbdefio_state)
		return -ENOMEM;
	fbdefio_state->info = info;

	INIT_DELAYED_WORK(&info->deferred_work, fb_deferred_io_work);
<<<<<<< HEAD
	if (fbdefio->delay == 0) /* set a default of 1 s */
		fbdefio->delay = HZ;

	info->fbdefio_state = fbdefio_state;

	return 0;
=======
	INIT_LIST_HEAD(&fbdefio->pagereflist);
	if (fbdefio->delay == 0) /* set a default of 1 s */
		fbdefio->delay = HZ;

	npagerefs = DIV_ROUND_UP(info->fix.smem_len, PAGE_SIZE);

	/* alloc a page ref for each page of the display memory */
	pagerefs = kvzalloc_objs(*pagerefs, npagerefs);
	if (!pagerefs) {
		ret = -ENOMEM;
		goto err;
	}
	info->npagerefs = npagerefs;
	info->pagerefs = pagerefs;

	info->fbdefio_state = fbdefio_state;

	return 0;

err:
	fb_deferred_io_state_release(fbdefio_state);
	return ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
EXPORT_SYMBOL_GPL(fb_deferred_io_init);

void fb_deferred_io_open(struct fb_info *info,
			 struct inode *inode,
			 struct file *file)
{
<<<<<<< HEAD
	struct fb_deferred_io_state *fbdefio_state = info->fbdefio_state;

	fbdefio_state->mapping = file->f_mapping;
	file->f_mapping->a_ops = &fb_deferred_io_aops;
	fbdefio_state->open_count++;
=======
	struct fb_deferred_io *fbdefio = info->fbdefio;

	fbdefio->mapping = file->f_mapping;
	file->f_mapping->a_ops = &fb_deferred_io_aops;
	fbdefio->open_count++;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
EXPORT_SYMBOL_GPL(fb_deferred_io_open);

static void fb_deferred_io_lastclose(struct fb_info *info)
{
	flush_delayed_work(&info->deferred_work);
}

void fb_deferred_io_release(struct fb_info *info)
{
<<<<<<< HEAD
	struct fb_deferred_io_state *fbdefio_state = info->fbdefio_state;

	if (!--fbdefio_state->open_count)
=======
	struct fb_deferred_io *fbdefio = info->fbdefio;

	if (!--fbdefio->open_count)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		fb_deferred_io_lastclose(info);
}
EXPORT_SYMBOL_GPL(fb_deferred_io_release);

void fb_deferred_io_cleanup(struct fb_info *info)
{
<<<<<<< HEAD
=======
	struct fb_deferred_io *fbdefio = info->fbdefio;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	struct fb_deferred_io_state *fbdefio_state = info->fbdefio_state;

	fb_deferred_io_lastclose(info);

	info->fbdefio_state = NULL;

	mutex_lock(&fbdefio_state->lock);
	fbdefio_state->info = NULL;
	mutex_unlock(&fbdefio_state->lock);

	fb_deferred_io_state_put(fbdefio_state);
<<<<<<< HEAD
=======

	kvfree(info->pagerefs);
	fbdefio->mapping = NULL;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
EXPORT_SYMBOL_GPL(fb_deferred_io_cleanup);
