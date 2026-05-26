// SPDX-License-Identifier: MIT
/*
 * Copyright 2023 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/kref.h>
#include <linux/slab.h>
#include <linux/dma-fence-unwrap.h>

#include <drm/drm_exec.h>
#include <drm/drm_syncobj.h>

#include "amdgpu.h"
#include "amdgpu_userq_fence.h"

<<<<<<< HEAD
#define AMDGPU_USERQ_MAX_HANDLES	(1U << 16)

static const struct dma_fence_ops amdgpu_userq_fence_ops;
=======
static const struct dma_fence_ops amdgpu_userq_fence_ops;
static struct kmem_cache *amdgpu_userq_fence_slab;

#define AMDGPU_USERQ_MAX_HANDLES	(1U << 16)

int amdgpu_userq_fence_slab_init(void)
{
	amdgpu_userq_fence_slab = kmem_cache_create("amdgpu_userq_fence",
						    sizeof(struct amdgpu_userq_fence),
						    0,
						    SLAB_HWCACHE_ALIGN,
						    NULL);
	if (!amdgpu_userq_fence_slab)
		return -ENOMEM;

	return 0;
}

void amdgpu_userq_fence_slab_fini(void)
{
	rcu_barrier();
	kmem_cache_destroy(amdgpu_userq_fence_slab);
}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

static inline struct amdgpu_userq_fence *to_amdgpu_userq_fence(struct dma_fence *f)
{
	if (!f || f->ops != &amdgpu_userq_fence_ops)
		return NULL;

	return container_of(f, struct amdgpu_userq_fence, base);
}

static u64 amdgpu_userq_fence_read(struct amdgpu_userq_fence_driver *fence_drv)
{
	return le64_to_cpu(*fence_drv->cpu_addr);
}

static void
amdgpu_userq_fence_write(struct amdgpu_userq_fence_driver *fence_drv,
			 u64 seq)
{
	if (fence_drv->cpu_addr)
		*fence_drv->cpu_addr = cpu_to_le64(seq);
}

int amdgpu_userq_fence_driver_alloc(struct amdgpu_device *adev,
<<<<<<< HEAD
				    struct amdgpu_userq_fence_driver **fence_drv_req)
{
	struct amdgpu_userq_fence_driver *fence_drv;
	int r;

	if (!fence_drv_req)
		return -EINVAL;
	*fence_drv_req = NULL;

=======
				    struct amdgpu_usermode_queue *userq)
{
	struct amdgpu_userq_fence_driver *fence_drv;
	unsigned long flags;
	int r;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	fence_drv = kzalloc_obj(*fence_drv);
	if (!fence_drv)
		return -ENOMEM;

	/* Acquire seq64 memory */
	r = amdgpu_seq64_alloc(adev, &fence_drv->va, &fence_drv->gpu_addr,
			       &fence_drv->cpu_addr);
	if (r)
		goto free_fence_drv;

	memset(fence_drv->cpu_addr, 0, sizeof(u64));

	kref_init(&fence_drv->refcount);
	INIT_LIST_HEAD(&fence_drv->fences);
	spin_lock_init(&fence_drv->fence_list_lock);

	fence_drv->adev = adev;
	fence_drv->context = dma_fence_context_alloc(1);
	get_task_comm(fence_drv->timeline_name, current);

<<<<<<< HEAD
	*fence_drv_req = fence_drv;

	return 0;

=======
	xa_lock_irqsave(&adev->userq_xa, flags);
	r = xa_err(__xa_store(&adev->userq_xa, userq->doorbell_index,
			      fence_drv, GFP_KERNEL));
	xa_unlock_irqrestore(&adev->userq_xa, flags);
	if (r)
		goto free_seq64;

	userq->fence_drv = fence_drv;

	return 0;

free_seq64:
	amdgpu_seq64_free(adev, fence_drv->va);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
free_fence_drv:
	kfree(fence_drv);

	return r;
}

static void amdgpu_userq_walk_and_drop_fence_drv(struct xarray *xa)
{
	struct amdgpu_userq_fence_driver *fence_drv;
	unsigned long index;

	if (xa_empty(xa))
		return;

	xa_lock(xa);
	xa_for_each(xa, index, fence_drv) {
		__xa_erase(xa, index);
		amdgpu_userq_fence_driver_put(fence_drv);
	}

	xa_unlock(xa);
}

void
amdgpu_userq_fence_driver_free(struct amdgpu_usermode_queue *userq)
{
	dma_fence_put(userq->last_fence);
<<<<<<< HEAD
	userq->last_fence = NULL;
	amdgpu_userq_walk_and_drop_fence_drv(&userq->fence_drv_xa);
	xa_destroy(&userq->fence_drv_xa);
	mutex_destroy(&userq->fence_drv_lock);
	/* Drop the queue's ownership reference to fence_drv explicitly */
	amdgpu_userq_fence_driver_put(userq->fence_drv);
}

static void
amdgpu_userq_fence_put_fence_drv_array(struct amdgpu_userq_fence *userq_fence)
{
	unsigned long i;
	for (i = 0; i < userq_fence->fence_drv_array_count; i++)
		amdgpu_userq_fence_driver_put(userq_fence->fence_drv_array[i]);
	userq_fence->fence_drv_array_count = 0;
}

/*
 * Returns:
 * -ENOENT when no fences were processes
 * 1 when more fences are pending
 * 0 when no fences are pending any more
 */
int
amdgpu_userq_fence_driver_process(struct amdgpu_userq_fence_driver *fence_drv)
{
	struct amdgpu_userq_fence *userq_fence, *tmp;
	LIST_HEAD(to_be_signaled);
	struct dma_fence *fence;
	unsigned long flags;
	u64 rptr;
=======

	amdgpu_userq_walk_and_drop_fence_drv(&userq->fence_drv_xa);
	xa_destroy(&userq->fence_drv_xa);
	/* Drop the fence_drv reference held by user queue */
	amdgpu_userq_fence_driver_put(userq->fence_drv);
}

void amdgpu_userq_fence_driver_process(struct amdgpu_userq_fence_driver *fence_drv)
{
	struct amdgpu_userq_fence *userq_fence, *tmp;
	struct dma_fence *fence;
	unsigned long flags;
	u64 rptr;
	int i;

	if (!fence_drv)
		return;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	spin_lock_irqsave(&fence_drv->fence_list_lock, flags);
	rptr = amdgpu_userq_fence_read(fence_drv);

<<<<<<< HEAD
	list_for_each_entry(userq_fence, &fence_drv->fences, link) {
		if (rptr < userq_fence->base.seqno)
			break;
	}

	list_cut_before(&to_be_signaled, &fence_drv->fences,
				&userq_fence->link);
	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);

	if (list_empty(&to_be_signaled))
		return -ENOENT;

	list_for_each_entry_safe(userq_fence, tmp, &to_be_signaled, link) {
		fence = &userq_fence->base;
		list_del_init(&userq_fence->link);
		dma_fence_signal(fence);
		/* Drop fence_drv_array outside fence_list_lock
		 * to avoid the recursion lock.
		 */
		amdgpu_userq_fence_put_fence_drv_array(userq_fence);
		dma_fence_put(fence);
	}

	/* That doesn't need to be accurate so no locking */
	return list_empty(&fence_drv->fences) ? 0 : 1;
=======
	list_for_each_entry_safe(userq_fence, tmp, &fence_drv->fences, link) {
		fence = &userq_fence->base;

		if (rptr < fence->seqno)
			break;

		dma_fence_signal(fence);

		for (i = 0; i < userq_fence->fence_drv_array_count; i++)
			amdgpu_userq_fence_driver_put(userq_fence->fence_drv_array[i]);

		list_del(&userq_fence->link);
		dma_fence_put(fence);
	}
	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

void amdgpu_userq_fence_driver_destroy(struct kref *ref)
{
	struct amdgpu_userq_fence_driver *fence_drv = container_of(ref,
					 struct amdgpu_userq_fence_driver,
					 refcount);
<<<<<<< HEAD
	struct amdgpu_device *adev = fence_drv->adev;
	struct amdgpu_userq_fence *fence, *tmp;
	unsigned long flags;
=======
	struct amdgpu_userq_fence_driver *xa_fence_drv;
	struct amdgpu_device *adev = fence_drv->adev;
	struct amdgpu_userq_fence *fence, *tmp;
	struct xarray *xa = &adev->userq_xa;
	unsigned long index, flags;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	struct dma_fence *f;

	spin_lock_irqsave(&fence_drv->fence_list_lock, flags);
	list_for_each_entry_safe(fence, tmp, &fence_drv->fences, link) {
		f = &fence->base;

		if (!dma_fence_is_signaled(f)) {
			dma_fence_set_error(f, -ECANCELED);
			dma_fence_signal(f);
		}

		list_del(&fence->link);
		dma_fence_put(f);
	}
	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);

<<<<<<< HEAD
=======
	xa_lock_irqsave(xa, flags);
	xa_for_each(xa, index, xa_fence_drv)
		if (xa_fence_drv == fence_drv)
			__xa_erase(xa, index);
	xa_unlock_irqrestore(xa, flags);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	/* Free seq64 memory */
	amdgpu_seq64_free(adev, fence_drv->va);
	kfree(fence_drv);
}

void amdgpu_userq_fence_driver_get(struct amdgpu_userq_fence_driver *fence_drv)
{
	kref_get(&fence_drv->refcount);
}

void amdgpu_userq_fence_driver_put(struct amdgpu_userq_fence_driver *fence_drv)
{
	kref_put(&fence_drv->refcount, amdgpu_userq_fence_driver_destroy);
}

<<<<<<< HEAD
static int amdgpu_userq_fence_alloc(struct amdgpu_usermode_queue *userq,
				    struct amdgpu_userq_fence **pfence)
{
	struct amdgpu_userq_fence_driver *fence_drv = userq->fence_drv;
	struct amdgpu_userq_fence *userq_fence;
	void *entry;

	userq_fence = kmalloc(sizeof(*userq_fence), GFP_KERNEL);
	if (!userq_fence)
		return -ENOMEM;

	/*
	 * Get the next unused entry, since we fill from the start this can be
	 * used as size to allocate the array.
	 */
	mutex_lock(&userq->fence_drv_lock);
	XA_STATE(xas, &userq->fence_drv_xa, 0);

	rcu_read_lock();
	do {
		entry = xas_find_marked(&xas, ULONG_MAX, XA_FREE_MARK);
	} while (xas_retry(&xas, entry));
	rcu_read_unlock();

	userq_fence->fence_drv_array = kvmalloc_array(xas.xa_index,
						      sizeof(fence_drv),
						      GFP_KERNEL);
	if (!userq_fence->fence_drv_array) {
		mutex_unlock(&userq->fence_drv_lock);
		kfree(userq_fence);
		return -ENOMEM;
	}

	userq_fence->fence_drv_array_count = xas.xa_index;
	xa_extract(&userq->fence_drv_xa, (void **)userq_fence->fence_drv_array,
		   0, ULONG_MAX, xas.xa_index, XA_PRESENT);
	xa_destroy(&userq->fence_drv_xa);

	mutex_unlock(&userq->fence_drv_lock);

	amdgpu_userq_fence_driver_get(fence_drv);
	userq_fence->fence_drv = fence_drv;

	*pfence = userq_fence;
	return 0;
}

static void amdgpu_userq_fence_init(struct amdgpu_usermode_queue *userq,
				    struct amdgpu_userq_fence *fence,
				    u64 seq)
{
	struct amdgpu_userq_fence_driver *fence_drv = userq->fence_drv;
	unsigned long flags;
	bool signaled = false;

	spin_lock_init(&fence->lock);
	dma_fence_init64(&fence->base, &amdgpu_userq_fence_ops, &fence->lock,
			 fence_drv->context, seq);

	/* Make sure the fence is visible to the hang detect worker */
	dma_fence_put(userq->last_fence);
	userq->last_fence = dma_fence_get(&fence->base);

	/* Check if hardware has already processed the fence */
	spin_lock_irqsave(&fence_drv->fence_list_lock, flags);
	if (!dma_fence_is_signaled(&fence->base)) {
		dma_fence_get(&fence->base);
		list_add_tail(&fence->link, &fence_drv->fences);
	} else {
		INIT_LIST_HEAD(&fence->link);
		signaled = true;
	}
	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);

	if (signaled)
		amdgpu_userq_fence_put_fence_drv_array(fence);
	else
		amdgpu_userq_start_hang_detect_work(userq);
=======
static int amdgpu_userq_fence_alloc(struct amdgpu_userq_fence **userq_fence)
{
	*userq_fence = kmem_cache_alloc(amdgpu_userq_fence_slab, GFP_ATOMIC);
	return *userq_fence ? 0 : -ENOMEM;
}

static int amdgpu_userq_fence_create(struct amdgpu_usermode_queue *userq,
				     struct amdgpu_userq_fence *userq_fence,
				     u64 seq, struct dma_fence **f)
{
	struct amdgpu_userq_fence_driver *fence_drv;
	struct dma_fence *fence;
	unsigned long flags;

	fence_drv = userq->fence_drv;
	if (!fence_drv)
		return -EINVAL;

	spin_lock_init(&userq_fence->lock);
	INIT_LIST_HEAD(&userq_fence->link);
	fence = &userq_fence->base;
	userq_fence->fence_drv = fence_drv;

	dma_fence_init64(fence, &amdgpu_userq_fence_ops, &userq_fence->lock,
			 fence_drv->context, seq);

	amdgpu_userq_fence_driver_get(fence_drv);
	dma_fence_get(fence);

	if (!xa_empty(&userq->fence_drv_xa)) {
		struct amdgpu_userq_fence_driver *stored_fence_drv;
		unsigned long index, count = 0;
		int i = 0;

		xa_lock(&userq->fence_drv_xa);
		xa_for_each(&userq->fence_drv_xa, index, stored_fence_drv)
			count++;

		userq_fence->fence_drv_array =
			kvmalloc_objs(struct amdgpu_userq_fence_driver *, count,
				      GFP_ATOMIC);

		if (userq_fence->fence_drv_array) {
			xa_for_each(&userq->fence_drv_xa, index, stored_fence_drv) {
				userq_fence->fence_drv_array[i] = stored_fence_drv;
				__xa_erase(&userq->fence_drv_xa, index);
				i++;
			}
		}

		userq_fence->fence_drv_array_count = i;
		xa_unlock(&userq->fence_drv_xa);
	} else {
		userq_fence->fence_drv_array = NULL;
		userq_fence->fence_drv_array_count = 0;
	}

	/* Check if hardware has already processed the job */
	spin_lock_irqsave(&fence_drv->fence_list_lock, flags);
	if (!dma_fence_is_signaled(fence))
		list_add_tail(&userq_fence->link, &fence_drv->fences);
	else
		dma_fence_put(fence);

	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);

	*f = fence;

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static const char *amdgpu_userq_fence_get_driver_name(struct dma_fence *f)
{
	return "amdgpu_userq_fence";
}

static const char *amdgpu_userq_fence_get_timeline_name(struct dma_fence *f)
{
	struct amdgpu_userq_fence *fence = to_amdgpu_userq_fence(f);

	return fence->fence_drv->timeline_name;
}

static bool amdgpu_userq_fence_signaled(struct dma_fence *f)
{
	struct amdgpu_userq_fence *fence = to_amdgpu_userq_fence(f);
	struct amdgpu_userq_fence_driver *fence_drv = fence->fence_drv;
	u64 rptr, wptr;

	rptr = amdgpu_userq_fence_read(fence_drv);
	wptr = fence->base.seqno;

	if (rptr >= wptr)
		return true;

	return false;
}

static void amdgpu_userq_fence_free(struct rcu_head *rcu)
{
	struct dma_fence *fence = container_of(rcu, struct dma_fence, rcu);
	struct amdgpu_userq_fence *userq_fence = to_amdgpu_userq_fence(fence);
	struct amdgpu_userq_fence_driver *fence_drv = userq_fence->fence_drv;

	/* Release the fence driver reference */
	amdgpu_userq_fence_driver_put(fence_drv);

	kvfree(userq_fence->fence_drv_array);
<<<<<<< HEAD
	kfree(userq_fence);
=======
	kmem_cache_free(amdgpu_userq_fence_slab, userq_fence);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void amdgpu_userq_fence_release(struct dma_fence *f)
{
	call_rcu(&f->rcu, amdgpu_userq_fence_free);
}

static const struct dma_fence_ops amdgpu_userq_fence_ops = {
	.get_driver_name = amdgpu_userq_fence_get_driver_name,
	.get_timeline_name = amdgpu_userq_fence_get_timeline_name,
	.signaled = amdgpu_userq_fence_signaled,
	.release = amdgpu_userq_fence_release,
};

/**
 * amdgpu_userq_fence_read_wptr - Read the userq wptr value
 *
 * @adev: amdgpu_device pointer
 * @queue: user mode queue structure pointer
 * @wptr: write pointer value
 *
 * Read the wptr value from userq's MQD. The userq signal IOCTL
 * creates a dma_fence for the shared buffers that expects the
 * RPTR value written to seq64 memory >= WPTR.
 *
 * Returns wptr value on success, error on failure.
 */
static int amdgpu_userq_fence_read_wptr(struct amdgpu_device *adev,
					struct amdgpu_usermode_queue *queue,
					u64 *wptr)
{
	struct amdgpu_bo_va_mapping *mapping;
	struct amdgpu_bo *bo;
	struct drm_exec exec;
	u64 addr, *ptr;
	int ret;

	addr = queue->userq_prop->wptr_gpu_addr;
	addr &= AMDGPU_GMC_HOLE_MASK;

	drm_exec_init(&exec, DRM_EXEC_IGNORE_DUPLICATES, 2);
	drm_exec_until_all_locked(&exec) {
		ret = amdgpu_vm_lock_pd(queue->vm, &exec, 1);
		drm_exec_retry_on_contention(&exec);
		if (unlikely(ret))
			goto lock_error;

		mapping = amdgpu_vm_bo_lookup_mapping(queue->vm, addr >> PAGE_SHIFT);
		if (!mapping) {
			ret = -EINVAL;
			goto lock_error;
		}

		ret = drm_exec_lock_obj(&exec, &mapping->bo_va->base.bo->tbo.base);
		drm_exec_retry_on_contention(&exec);
		if (unlikely(ret))
			goto lock_error;
	}

	bo = mapping->bo_va->base.bo;
	ret = amdgpu_bo_kmap(bo, (void **)&ptr);
	if (ret) {
		DRM_ERROR("Failed mapping the userqueue wptr bo");
		goto lock_error;
	}

	*wptr = le64_to_cpu(*ptr);

	amdgpu_bo_kunmap(bo);
	drm_exec_fini(&exec);
	return 0;

lock_error:
	drm_exec_fini(&exec);
	return ret;
}

<<<<<<< HEAD
=======
static void amdgpu_userq_fence_cleanup(struct dma_fence *fence)
{
	dma_fence_put(fence);
}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static void
amdgpu_userq_fence_driver_set_error(struct amdgpu_userq_fence *fence,
				    int error)
{
	struct amdgpu_userq_fence_driver *fence_drv = fence->fence_drv;
	unsigned long flags;
	struct dma_fence *f;

	spin_lock_irqsave(&fence_drv->fence_list_lock, flags);

	f = rcu_dereference_protected(&fence->base,
				      lockdep_is_held(&fence_drv->fence_list_lock));
	if (f && !dma_fence_is_signaled_locked(f))
		dma_fence_set_error(f, error);
	spin_unlock_irqrestore(&fence_drv->fence_list_lock, flags);
}

void
amdgpu_userq_fence_driver_force_completion(struct amdgpu_usermode_queue *userq)
{
	struct dma_fence *f = userq->last_fence;

	if (f) {
		struct amdgpu_userq_fence *fence = to_amdgpu_userq_fence(f);
		struct amdgpu_userq_fence_driver *fence_drv = fence->fence_drv;
		u64 wptr = fence->base.seqno;

		amdgpu_userq_fence_driver_set_error(fence, -ECANCELED);
		amdgpu_userq_fence_write(fence_drv, wptr);
		amdgpu_userq_fence_driver_process(fence_drv);

	}
}

int amdgpu_userq_signal_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *filp)
{
	struct amdgpu_device *adev = drm_to_adev(dev);
<<<<<<< HEAD
	struct drm_amdgpu_userq_signal *args = data;
	const unsigned int num_write_bo_handles = args->num_bo_write_handles;
	const unsigned int num_read_bo_handles = args->num_bo_read_handles;
	struct amdgpu_fpriv *fpriv = filp->driver_priv;
	struct amdgpu_userq_mgr *userq_mgr = &fpriv->userq_mgr;

	struct drm_gem_object **gobj_write, **gobj_read;
	u32 *syncobj_handles, num_syncobj_handles;
	struct amdgpu_usermode_queue *queue;
	struct amdgpu_userq_fence *fence;
	struct drm_syncobj **syncobj;
	struct drm_exec exec;
	void __user *ptr;
	int r, i, entry;
=======
	struct amdgpu_fpriv *fpriv = filp->driver_priv;
	struct amdgpu_userq_mgr *userq_mgr = &fpriv->userq_mgr;
	struct drm_amdgpu_userq_signal *args = data;
	struct drm_gem_object **gobj_write = NULL;
	struct drm_gem_object **gobj_read = NULL;
	struct amdgpu_usermode_queue *queue = NULL;
	struct amdgpu_userq_fence *userq_fence;
	struct drm_syncobj **syncobj = NULL;
	u32 *bo_handles_write, num_write_bo_handles;
	u32 *syncobj_handles, num_syncobj_handles;
	u32 *bo_handles_read, num_read_bo_handles;
	int r, i, entry, rentry, wentry;
	struct dma_fence *fence;
	struct drm_exec exec;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	u64 wptr;

	if (!amdgpu_userq_enabled(dev))
		return -ENOTSUPP;

<<<<<<< HEAD
	if (args->num_bo_write_handles > AMDGPU_USERQ_MAX_HANDLES ||
=======
	if (args->num_syncobj_handles > AMDGPU_USERQ_MAX_HANDLES ||
	    args->num_bo_write_handles > AMDGPU_USERQ_MAX_HANDLES ||
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	    args->num_bo_read_handles > AMDGPU_USERQ_MAX_HANDLES)
		return -EINVAL;

	num_syncobj_handles = args->num_syncobj_handles;
<<<<<<< HEAD
	ptr = u64_to_user_ptr(args->syncobj_handles);
	syncobj_handles = memdup_array_user(ptr, num_syncobj_handles,
					    sizeof(u32));
	if (IS_ERR(syncobj_handles))
		return PTR_ERR(syncobj_handles);

	syncobj = kmalloc_array(num_syncobj_handles, sizeof(*syncobj),
				GFP_KERNEL);
=======
	syncobj_handles = memdup_user(u64_to_user_ptr(args->syncobj_handles),
				      size_mul(sizeof(u32), num_syncobj_handles));
	if (IS_ERR(syncobj_handles))
		return PTR_ERR(syncobj_handles);

	/* Array of pointers to the looked up syncobjs */
	syncobj = kmalloc_array(num_syncobj_handles, sizeof(*syncobj), GFP_KERNEL);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!syncobj) {
		r = -ENOMEM;
		goto free_syncobj_handles;
	}

	for (entry = 0; entry < num_syncobj_handles; entry++) {
		syncobj[entry] = drm_syncobj_find(filp, syncobj_handles[entry]);
		if (!syncobj[entry]) {
			r = -ENOENT;
			goto free_syncobj;
		}
	}

<<<<<<< HEAD
	ptr = u64_to_user_ptr(args->bo_read_handles);
	r = drm_gem_objects_lookup(filp, ptr, num_read_bo_handles, &gobj_read);
	if (r)
		goto free_syncobj;

	ptr = u64_to_user_ptr(args->bo_write_handles);
	r = drm_gem_objects_lookup(filp, ptr, num_write_bo_handles,
				   &gobj_write);
	if (r)
		goto put_gobj_read;

=======
	num_read_bo_handles = args->num_bo_read_handles;
	bo_handles_read = memdup_user(u64_to_user_ptr(args->bo_read_handles),
				      sizeof(u32) * num_read_bo_handles);
	if (IS_ERR(bo_handles_read)) {
		r = PTR_ERR(bo_handles_read);
		goto free_syncobj;
	}

	/* Array of pointers to the GEM read objects */
	gobj_read = kmalloc_array(num_read_bo_handles, sizeof(*gobj_read), GFP_KERNEL);
	if (!gobj_read) {
		r = -ENOMEM;
		goto free_bo_handles_read;
	}

	for (rentry = 0; rentry < num_read_bo_handles; rentry++) {
		gobj_read[rentry] = drm_gem_object_lookup(filp, bo_handles_read[rentry]);
		if (!gobj_read[rentry]) {
			r = -ENOENT;
			goto put_gobj_read;
		}
	}

	num_write_bo_handles = args->num_bo_write_handles;
	bo_handles_write = memdup_user(u64_to_user_ptr(args->bo_write_handles),
				       sizeof(u32) * num_write_bo_handles);
	if (IS_ERR(bo_handles_write)) {
		r = PTR_ERR(bo_handles_write);
		goto put_gobj_read;
	}

	/* Array of pointers to the GEM write objects */
	gobj_write = kmalloc_array(num_write_bo_handles, sizeof(*gobj_write), GFP_KERNEL);
	if (!gobj_write) {
		r = -ENOMEM;
		goto free_bo_handles_write;
	}

	for (wentry = 0; wentry < num_write_bo_handles; wentry++) {
		gobj_write[wentry] = drm_gem_object_lookup(filp, bo_handles_write[wentry]);
		if (!gobj_write[wentry]) {
			r = -ENOENT;
			goto put_gobj_write;
		}
	}

	/* Retrieve the user queue */
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	queue = amdgpu_userq_get(userq_mgr, args->queue_id);
	if (!queue) {
		r = -ENOENT;
		goto put_gobj_write;
	}

	r = amdgpu_userq_fence_read_wptr(adev, queue, &wptr);
	if (r)
<<<<<<< HEAD
		goto put_queue;

	r = amdgpu_userq_fence_alloc(queue, &fence);
	if (r)
		goto put_queue;
=======
		goto put_gobj_write;

	r = amdgpu_userq_fence_alloc(&userq_fence);
	if (r)
		goto put_gobj_write;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* We are here means UQ is active, make sure the eviction fence is valid */
	amdgpu_userq_ensure_ev_fence(&fpriv->userq_mgr, &fpriv->evf_mgr);

<<<<<<< HEAD
	/* Create the new fence */
	amdgpu_userq_fence_init(queue, fence, wptr);

	mutex_unlock(&userq_mgr->userq_mutex);

	/*
	 * This needs to come after the fence is created since
	 * amdgpu_userq_ensure_ev_fence() can't be called while holding the resv
	 * locks.
	 */
	drm_exec_init(&exec, DRM_EXEC_INTERRUPTIBLE_WAIT,
		      (num_read_bo_handles + num_write_bo_handles));

	drm_exec_until_all_locked(&exec) {
		r = drm_exec_prepare_array(&exec, gobj_read,
					   num_read_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto exec_fini;

		r = drm_exec_prepare_array(&exec, gobj_write,
					   num_write_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto exec_fini;
	}

	/* And publish the new fence in the BOs and syncobj */
	for (i = 0; i < num_read_bo_handles; i++)
		dma_resv_add_fence(gobj_read[i]->resv, &fence->base,
				   DMA_RESV_USAGE_READ);

	for (i = 0; i < num_write_bo_handles; i++)
		dma_resv_add_fence(gobj_write[i]->resv, &fence->base,
				   DMA_RESV_USAGE_WRITE);

	for (i = 0; i < num_syncobj_handles; i++)
		drm_syncobj_replace_fence(syncobj[i], &fence->base);

exec_fini:
	/* drop the reference acquired in fence creation function */
	dma_fence_put(&fence->base);

	drm_exec_fini(&exec);
put_queue:
	amdgpu_userq_put(queue);
put_gobj_write:
	for (i = 0; i < num_write_bo_handles; i++)
		drm_gem_object_put(gobj_write[i]);
	kvfree(gobj_write);
put_gobj_read:
	for (i = 0; i < num_read_bo_handles; i++)
		drm_gem_object_put(gobj_read[i]);
	kvfree(gobj_read);
free_syncobj:
	while (entry-- > 0)
		drm_syncobj_put(syncobj[entry]);
=======
	/* Create a new fence */
	r = amdgpu_userq_fence_create(queue, userq_fence, wptr, &fence);
	if (r) {
		mutex_unlock(&userq_mgr->userq_mutex);
		kmem_cache_free(amdgpu_userq_fence_slab, userq_fence);
		goto put_gobj_write;
	}

	dma_fence_put(queue->last_fence);
	queue->last_fence = dma_fence_get(fence);
	amdgpu_userq_start_hang_detect_work(queue);
	mutex_unlock(&userq_mgr->userq_mutex);

	drm_exec_init(&exec, DRM_EXEC_INTERRUPTIBLE_WAIT,
		      (num_read_bo_handles + num_write_bo_handles));

	/* Lock all BOs with retry handling */
	drm_exec_until_all_locked(&exec) {
		r = drm_exec_prepare_array(&exec, gobj_read, num_read_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r) {
			amdgpu_userq_fence_cleanup(fence);
			goto exec_fini;
		}

		r = drm_exec_prepare_array(&exec, gobj_write, num_write_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r) {
			amdgpu_userq_fence_cleanup(fence);
			goto exec_fini;
		}
	}

	for (i = 0; i < num_read_bo_handles; i++) {
		if (!gobj_read || !gobj_read[i]->resv)
			continue;

		dma_resv_add_fence(gobj_read[i]->resv, fence,
				   DMA_RESV_USAGE_READ);
	}

	for (i = 0; i < num_write_bo_handles; i++) {
		if (!gobj_write || !gobj_write[i]->resv)
			continue;

		dma_resv_add_fence(gobj_write[i]->resv, fence,
				   DMA_RESV_USAGE_WRITE);
	}

	/* Add the created fence to syncobj/BO's */
	for (i = 0; i < num_syncobj_handles; i++)
		drm_syncobj_replace_fence(syncobj[i], fence);

	/* drop the reference acquired in fence creation function */
	dma_fence_put(fence);

exec_fini:
	drm_exec_fini(&exec);
put_gobj_write:
	while (wentry-- > 0)
		drm_gem_object_put(gobj_write[wentry]);
	kfree(gobj_write);
free_bo_handles_write:
	kfree(bo_handles_write);
put_gobj_read:
	while (rentry-- > 0)
		drm_gem_object_put(gobj_read[rentry]);
	kfree(gobj_read);
free_bo_handles_read:
	kfree(bo_handles_read);
free_syncobj:
	while (entry-- > 0)
		if (syncobj[entry])
			drm_syncobj_put(syncobj[entry]);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	kfree(syncobj);
free_syncobj_handles:
	kfree(syncobj_handles);

<<<<<<< HEAD
	return r;
}

/* Count the number of expected fences so userspace can alloc a buffer */
static int
amdgpu_userq_wait_count_fences(struct drm_file *filp,
			       struct drm_amdgpu_userq_wait *wait_info,
			       u32 *syncobj_handles, u32 *timeline_points,
			       u32 *timeline_handles,
			       struct drm_gem_object **gobj_write,
			       struct drm_gem_object **gobj_read)
{
	int num_read_bo_handles, num_write_bo_handles;
	struct dma_fence_unwrap iter;
	struct dma_fence *fence, *f;
	unsigned int num_fences = 0;
	struct drm_exec exec;
	int i, r;

	/*
	 * This needs to be outside of the lock provided by drm_exec for
	 * DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT to work correctly.
	 */

	/* Count timeline fences */
	for (i = 0; i < wait_info->num_syncobj_timeline_handles; i++) {
		r = drm_syncobj_find_fence(filp, timeline_handles[i],
					   timeline_points[i],
					   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
					   &fence);
		if (r)
			return r;

		dma_fence_unwrap_for_each(f, &iter, fence)
			num_fences++;

		dma_fence_put(fence);
	}

	/* Count boolean fences */
	for (i = 0; i < wait_info->num_syncobj_handles; i++) {
		r = drm_syncobj_find_fence(filp, syncobj_handles[i], 0,
					   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
					   &fence);
		if (r)
			return r;

		num_fences++;
		dma_fence_put(fence);
	}

	/* Lock all the GEM objects */
	/* TODO: It is actually not necessary to lock them */
	num_read_bo_handles = wait_info->num_bo_read_handles;
	num_write_bo_handles = wait_info->num_bo_write_handles;
	drm_exec_init(&exec, DRM_EXEC_INTERRUPTIBLE_WAIT,
		      num_read_bo_handles + num_write_bo_handles);

	drm_exec_until_all_locked(&exec) {
		r = drm_exec_prepare_array(&exec, gobj_read,
					   num_read_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto error_unlock;

		r = drm_exec_prepare_array(&exec, gobj_write,
					   num_write_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto error_unlock;
	}

	/* Count read fences */
	for (i = 0; i < num_read_bo_handles; i++) {
		struct dma_resv_iter resv_cursor;
		struct dma_fence *fence;

		dma_resv_for_each_fence(&resv_cursor, gobj_read[i]->resv,
					DMA_RESV_USAGE_READ, fence)
			num_fences++;
	}

	/* Count write fences */
	for (i = 0; i < num_write_bo_handles; i++) {
		struct dma_resv_iter resv_cursor;
		struct dma_fence *fence;

		dma_resv_for_each_fence(&resv_cursor, gobj_write[i]->resv,
					DMA_RESV_USAGE_WRITE, fence)
			num_fences++;
	}

	wait_info->num_fences = min(num_fences, USHRT_MAX);
	r = 0;

error_unlock:
	/* Unlock all the GEM objects */
	drm_exec_fini(&exec);
	return r;
}

static int
amdgpu_userq_wait_add_fence(struct drm_amdgpu_userq_wait *wait_info,
			    struct dma_fence **fences, unsigned int *num_fences,
			    struct dma_fence *fence)
{
	/* As fallback shouldn't userspace allocate enough space */
	if (*num_fences >= wait_info->num_fences)
		return dma_fence_wait(fence, true);

	fences[(*num_fences)++] = dma_fence_get(fence);
	return 0;
}

static int
amdgpu_userq_wait_return_fence_info(struct drm_file *filp,
				    struct drm_amdgpu_userq_wait *wait_info,
				    u32 *syncobj_handles, u32 *timeline_points,
				    u32 *timeline_handles,
				    struct drm_gem_object **gobj_write,
				    struct drm_gem_object **gobj_read)
{
	struct amdgpu_fpriv *fpriv = filp->driver_priv;
	struct amdgpu_userq_mgr *userq_mgr = &fpriv->userq_mgr;
	struct drm_amdgpu_userq_fence_info *fence_info;
	int num_read_bo_handles, num_write_bo_handles;
	struct amdgpu_usermode_queue *waitq;
	struct dma_fence **fences, *fence, *f;
	struct dma_fence_unwrap iter;
	int num_points, num_syncobj;
	unsigned int num_fences = 0;
	struct drm_exec exec;
	int i, cnt, r;

	fence_info = kmalloc_array(wait_info->num_fences, sizeof(*fence_info),
				   GFP_KERNEL);
	if (!fence_info)
		return -ENOMEM;

	fences = kmalloc_array(wait_info->num_fences, sizeof(*fences),
			       GFP_KERNEL);
	if (!fences) {
		r = -ENOMEM;
		goto free_fence_info;
	}

	/* Retrieve timeline fences */
	num_points = wait_info->num_syncobj_timeline_handles;
	for (i = 0; i < num_points; i++) {
		r = drm_syncobj_find_fence(filp, timeline_handles[i],
					   timeline_points[i],
					   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
					   &fence);
		if (r)
			goto free_fences;

		dma_fence_unwrap_for_each(f, &iter, fence) {
			r = amdgpu_userq_wait_add_fence(wait_info, fences,
							&num_fences, f);
			if (r) {
				dma_fence_put(fence);
				goto free_fences;
			}
		}

		dma_fence_put(fence);
	}

	/* Retrieve boolean fences */
	num_syncobj = wait_info->num_syncobj_handles;
	for (i = 0; i < num_syncobj; i++) {
		struct dma_fence *fence;

		r = drm_syncobj_find_fence(filp, syncobj_handles[i], 0,
					   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
					   &fence);
		if (r)
			goto free_fences;

		r = amdgpu_userq_wait_add_fence(wait_info, fences,
						&num_fences, fence);
		dma_fence_put(fence);
		if (r)
			goto free_fences;

	}

	/* Lock all the GEM objects */
	num_read_bo_handles = wait_info->num_bo_read_handles;
	num_write_bo_handles = wait_info->num_bo_write_handles;
	drm_exec_init(&exec, DRM_EXEC_INTERRUPTIBLE_WAIT,
		      num_read_bo_handles + num_write_bo_handles);

	drm_exec_until_all_locked(&exec) {
		r = drm_exec_prepare_array(&exec, gobj_read,
					   num_read_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto error_unlock;

		r = drm_exec_prepare_array(&exec, gobj_write,
					   num_write_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r)
			goto error_unlock;
	}

	/* Retrieve GEM read objects fence */
	for (i = 0; i < num_read_bo_handles; i++) {
		struct dma_resv_iter resv_cursor;
		struct dma_fence *fence;

		dma_resv_for_each_fence(&resv_cursor, gobj_read[i]->resv,
					DMA_RESV_USAGE_READ, fence) {
			r = amdgpu_userq_wait_add_fence(wait_info, fences,
							&num_fences, fence);
			if (r)
				goto error_unlock;
		}
	}

	/* Retrieve GEM write objects fence */
	for (i = 0; i < num_write_bo_handles; i++) {
		struct dma_resv_iter resv_cursor;
		struct dma_fence *fence;

		dma_resv_for_each_fence(&resv_cursor, gobj_write[i]->resv,
					DMA_RESV_USAGE_WRITE, fence) {
			r = amdgpu_userq_wait_add_fence(wait_info, fences,
							&num_fences, fence);
			if (r)
				goto error_unlock;
		}
	}

	drm_exec_fini(&exec);

	/*
	 * Keep only the latest fences to reduce the number of values
	 * given back to userspace.
	 */
	num_fences = dma_fence_dedup_array(fences, num_fences);

	waitq = amdgpu_userq_get(userq_mgr, wait_info->waitq_id);
	if (!waitq) {
		r = -EINVAL;
		goto free_fences;
	}

	for (i = 0, cnt = 0; i < num_fences; i++) {
		struct amdgpu_userq_fence_driver *fence_drv;
		struct amdgpu_userq_fence *userq_fence;
		u32 index;

		userq_fence = to_amdgpu_userq_fence(fences[i]);
		if (!userq_fence) {
			/*
			 * Just waiting on other driver fences should
			 * be good for now
			 */
			r = dma_fence_wait(fences[i], true);
			if (r)
				goto put_waitq;

			continue;
		}

		fence_drv = userq_fence->fence_drv;
		/*
		 * We need to make sure the user queue release their reference
		 * to the fence drivers at some point before queue destruction.
		 * Otherwise, we would gather those references until we don't
		 * have any more space left and crash.
		 */
		mutex_lock(&waitq->fence_drv_lock);
		r = xa_alloc(&waitq->fence_drv_xa, &index, fence_drv,
			     xa_limit_32b, GFP_KERNEL);
		mutex_unlock(&waitq->fence_drv_lock);
		if (r)
			goto put_waitq;

		amdgpu_userq_fence_driver_get(fence_drv);

		/* Store drm syncobj's gpu va address and value */
		fence_info[cnt].va = fence_drv->va;
		fence_info[cnt].value = fences[i]->seqno;

		/* Increment the actual userq fence count */
		cnt++;
	}
	wait_info->num_fences = cnt;

	/* Copy userq fence info to user space */
	if (copy_to_user(u64_to_user_ptr(wait_info->out_fences),
			 fence_info, cnt * sizeof(*fence_info)))
		r = -EFAULT;
	else
		r = 0;

put_waitq:
	amdgpu_userq_put(waitq);

free_fences:
	while (num_fences--)
		dma_fence_put(fences[num_fences]);
	kfree(fences);

free_fence_info:
	kfree(fence_info);
	return r;

error_unlock:
	drm_exec_fini(&exec);
	goto free_fences;
}

int amdgpu_userq_wait_ioctl(struct drm_device *dev, void *data,
			    struct drm_file *filp)
{
	int num_points, num_syncobj, num_read_bo_handles, num_write_bo_handles;
	u32 *syncobj_handles, *timeline_points, *timeline_handles;
	struct drm_amdgpu_userq_wait *wait_info = data;
	struct drm_gem_object **gobj_write;
	struct drm_gem_object **gobj_read;
	void __user *ptr;
	int r;
=======
	if (queue)
		amdgpu_userq_put(queue);

	return r;
}

int amdgpu_userq_wait_ioctl(struct drm_device *dev, void *data,
			    struct drm_file *filp)
{
	u32 *syncobj_handles, *timeline_points, *timeline_handles, *bo_handles_read, *bo_handles_write;
	u32 num_syncobj, num_read_bo_handles, num_write_bo_handles;
	struct drm_amdgpu_userq_fence_info *fence_info = NULL;
	struct drm_amdgpu_userq_wait *wait_info = data;
	struct amdgpu_fpriv *fpriv = filp->driver_priv;
	struct amdgpu_userq_mgr *userq_mgr = &fpriv->userq_mgr;
	struct amdgpu_usermode_queue *waitq = NULL;
	struct drm_gem_object **gobj_write;
	struct drm_gem_object **gobj_read;
	struct dma_fence **fences = NULL;
	u16 num_points, num_fences = 0;
	int r, i, rentry, wentry, cnt;
	struct drm_exec exec;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (!amdgpu_userq_enabled(dev))
		return -ENOTSUPP;

<<<<<<< HEAD
	if (wait_info->num_bo_write_handles > AMDGPU_USERQ_MAX_HANDLES ||
	    wait_info->num_bo_read_handles > AMDGPU_USERQ_MAX_HANDLES)
		return -EINVAL;

	num_syncobj = wait_info->num_syncobj_handles;
	ptr = u64_to_user_ptr(wait_info->syncobj_handles);
	syncobj_handles = memdup_array_user(ptr, num_syncobj, sizeof(u32));
	if (IS_ERR(syncobj_handles))
		return PTR_ERR(syncobj_handles);

	num_points = wait_info->num_syncobj_timeline_handles;
	ptr = u64_to_user_ptr(wait_info->syncobj_timeline_handles);
	timeline_handles = memdup_array_user(ptr, num_points, sizeof(u32));
=======
	if (wait_info->num_syncobj_handles > AMDGPU_USERQ_MAX_HANDLES ||
	    wait_info->num_bo_write_handles > AMDGPU_USERQ_MAX_HANDLES ||
	    wait_info->num_bo_read_handles > AMDGPU_USERQ_MAX_HANDLES)
		return -EINVAL;

	num_read_bo_handles = wait_info->num_bo_read_handles;
	bo_handles_read = memdup_user(u64_to_user_ptr(wait_info->bo_read_handles),
				      size_mul(sizeof(u32), num_read_bo_handles));
	if (IS_ERR(bo_handles_read))
		return PTR_ERR(bo_handles_read);

	num_write_bo_handles = wait_info->num_bo_write_handles;
	bo_handles_write = memdup_user(u64_to_user_ptr(wait_info->bo_write_handles),
				       size_mul(sizeof(u32), num_write_bo_handles));
	if (IS_ERR(bo_handles_write)) {
		r = PTR_ERR(bo_handles_write);
		goto free_bo_handles_read;
	}

	num_syncobj = wait_info->num_syncobj_handles;
	syncobj_handles = memdup_user(u64_to_user_ptr(wait_info->syncobj_handles),
				      size_mul(sizeof(u32), num_syncobj));
	if (IS_ERR(syncobj_handles)) {
		r = PTR_ERR(syncobj_handles);
		goto free_bo_handles_write;
	}

	num_points = wait_info->num_syncobj_timeline_handles;
	timeline_handles = memdup_user(u64_to_user_ptr(wait_info->syncobj_timeline_handles),
				       sizeof(u32) * num_points);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (IS_ERR(timeline_handles)) {
		r = PTR_ERR(timeline_handles);
		goto free_syncobj_handles;
	}

<<<<<<< HEAD
	ptr = u64_to_user_ptr(wait_info->syncobj_timeline_points);
	timeline_points = memdup_array_user(ptr, num_points, sizeof(u32));
=======
	timeline_points = memdup_user(u64_to_user_ptr(wait_info->syncobj_timeline_points),
				      sizeof(u32) * num_points);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (IS_ERR(timeline_points)) {
		r = PTR_ERR(timeline_points);
		goto free_timeline_handles;
	}

<<<<<<< HEAD
	num_read_bo_handles = wait_info->num_bo_read_handles;
	ptr = u64_to_user_ptr(wait_info->bo_read_handles);
	r = drm_gem_objects_lookup(filp, ptr, num_read_bo_handles, &gobj_read);
	if (r)
		goto free_timeline_points;

	num_write_bo_handles = wait_info->num_bo_write_handles;
	ptr = u64_to_user_ptr(wait_info->bo_write_handles);
	r = drm_gem_objects_lookup(filp, ptr, num_write_bo_handles,
				   &gobj_write);
	if (r)
		goto put_gobj_read;

	/*
	 * Passing num_fences = 0 means that userspace doesn't want to
	 * retrieve userq_fence_info. If num_fences = 0 we skip filling
	 * userq_fence_info and return the actual number of fences on
	 * args->num_fences.
	 */
	if (!wait_info->num_fences) {
		r = amdgpu_userq_wait_count_fences(filp, wait_info,
						   syncobj_handles,
						   timeline_points,
						   timeline_handles,
						   gobj_write,
						   gobj_read);
	} else {
		r = amdgpu_userq_wait_return_fence_info(filp, wait_info,
							syncobj_handles,
							timeline_points,
							timeline_handles,
							gobj_write,
							gobj_read);
	}

	while (num_write_bo_handles--)
		drm_gem_object_put(gobj_write[num_write_bo_handles]);
	kvfree(gobj_write);

put_gobj_read:
	while (num_read_bo_handles--)
		drm_gem_object_put(gobj_read[num_read_bo_handles]);
	kvfree(gobj_read);

=======
	gobj_read = kmalloc_array(num_read_bo_handles, sizeof(*gobj_read), GFP_KERNEL);
	if (!gobj_read) {
		r = -ENOMEM;
		goto free_timeline_points;
	}

	for (rentry = 0; rentry < num_read_bo_handles; rentry++) {
		gobj_read[rentry] = drm_gem_object_lookup(filp, bo_handles_read[rentry]);
		if (!gobj_read[rentry]) {
			r = -ENOENT;
			goto put_gobj_read;
		}
	}

	gobj_write = kmalloc_array(num_write_bo_handles, sizeof(*gobj_write), GFP_KERNEL);
	if (!gobj_write) {
		r = -ENOMEM;
		goto put_gobj_read;
	}

	for (wentry = 0; wentry < num_write_bo_handles; wentry++) {
		gobj_write[wentry] = drm_gem_object_lookup(filp, bo_handles_write[wentry]);
		if (!gobj_write[wentry]) {
			r = -ENOENT;
			goto put_gobj_write;
		}
	}

	drm_exec_init(&exec, DRM_EXEC_INTERRUPTIBLE_WAIT,
		      (num_read_bo_handles + num_write_bo_handles));

	/* Lock all BOs with retry handling */
	drm_exec_until_all_locked(&exec) {
		r = drm_exec_prepare_array(&exec, gobj_read, num_read_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r) {
			drm_exec_fini(&exec);
			goto put_gobj_write;
		}

		r = drm_exec_prepare_array(&exec, gobj_write, num_write_bo_handles, 1);
		drm_exec_retry_on_contention(&exec);
		if (r) {
			drm_exec_fini(&exec);
			goto put_gobj_write;
		}
	}

	if (!wait_info->num_fences) {
		if (num_points) {
			struct dma_fence_unwrap iter;
			struct dma_fence *fence;
			struct dma_fence *f;

			for (i = 0; i < num_points; i++) {
				r = drm_syncobj_find_fence(filp, timeline_handles[i],
							   timeline_points[i],
							   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
							   &fence);
				if (r)
					goto exec_fini;

				dma_fence_unwrap_for_each(f, &iter, fence)
					num_fences++;

				dma_fence_put(fence);
			}
		}

		/* Count syncobj's fence */
		for (i = 0; i < num_syncobj; i++) {
			struct dma_fence *fence;

			r = drm_syncobj_find_fence(filp, syncobj_handles[i],
						   0,
						   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
						   &fence);
			if (r)
				goto exec_fini;

			num_fences++;
			dma_fence_put(fence);
		}

		/* Count GEM objects fence */
		for (i = 0; i < num_read_bo_handles; i++) {
			struct dma_resv_iter resv_cursor;
			struct dma_fence *fence;

			dma_resv_for_each_fence(&resv_cursor, gobj_read[i]->resv,
						DMA_RESV_USAGE_READ, fence)
				num_fences++;
		}

		for (i = 0; i < num_write_bo_handles; i++) {
			struct dma_resv_iter resv_cursor;
			struct dma_fence *fence;

			dma_resv_for_each_fence(&resv_cursor, gobj_write[i]->resv,
						DMA_RESV_USAGE_WRITE, fence)
				num_fences++;
		}

		/*
		 * Passing num_fences = 0 means that userspace doesn't want to
		 * retrieve userq_fence_info. If num_fences = 0 we skip filling
		 * userq_fence_info and return the actual number of fences on
		 * args->num_fences.
		 */
		wait_info->num_fences = num_fences;
	} else {
		/* Array of fence info */
		fence_info = kmalloc_array(wait_info->num_fences, sizeof(*fence_info), GFP_KERNEL);
		if (!fence_info) {
			r = -ENOMEM;
			goto exec_fini;
		}

		/* Array of fences */
		fences = kmalloc_array(wait_info->num_fences, sizeof(*fences), GFP_KERNEL);
		if (!fences) {
			r = -ENOMEM;
			goto free_fence_info;
		}

		/* Retrieve GEM read objects fence */
		for (i = 0; i < num_read_bo_handles; i++) {
			struct dma_resv_iter resv_cursor;
			struct dma_fence *fence;

			dma_resv_for_each_fence(&resv_cursor, gobj_read[i]->resv,
						DMA_RESV_USAGE_READ, fence) {
				if (num_fences >= wait_info->num_fences) {
					r = -EINVAL;
					goto free_fences;
				}

				fences[num_fences++] = fence;
				dma_fence_get(fence);
			}
		}

		/* Retrieve GEM write objects fence */
		for (i = 0; i < num_write_bo_handles; i++) {
			struct dma_resv_iter resv_cursor;
			struct dma_fence *fence;

			dma_resv_for_each_fence(&resv_cursor, gobj_write[i]->resv,
						DMA_RESV_USAGE_WRITE, fence) {
				if (num_fences >= wait_info->num_fences) {
					r = -EINVAL;
					goto free_fences;
				}

				fences[num_fences++] = fence;
				dma_fence_get(fence);
			}
		}

		if (num_points) {
			struct dma_fence_unwrap iter;
			struct dma_fence *fence;
			struct dma_fence *f;

			for (i = 0; i < num_points; i++) {
				r = drm_syncobj_find_fence(filp, timeline_handles[i],
							   timeline_points[i],
							   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
							   &fence);
				if (r)
					goto free_fences;

				dma_fence_unwrap_for_each(f, &iter, fence) {
					if (num_fences >= wait_info->num_fences) {
						r = -EINVAL;
						dma_fence_put(fence);
						goto free_fences;
					}

					dma_fence_get(f);
					fences[num_fences++] = f;
				}

				dma_fence_put(fence);
			}
		}

		/* Retrieve syncobj's fence */
		for (i = 0; i < num_syncobj; i++) {
			struct dma_fence *fence;

			r = drm_syncobj_find_fence(filp, syncobj_handles[i],
						   0,
						   DRM_SYNCOBJ_WAIT_FLAGS_WAIT_FOR_SUBMIT,
						   &fence);
			if (r)
				goto free_fences;

			if (num_fences >= wait_info->num_fences) {
				r = -EINVAL;
				dma_fence_put(fence);
				goto free_fences;
			}

			fences[num_fences++] = fence;
		}

		/*
		 * Keep only the latest fences to reduce the number of values
		 * given back to userspace.
		 */
		num_fences = dma_fence_dedup_array(fences, num_fences);

		waitq = amdgpu_userq_get(userq_mgr, wait_info->waitq_id);
		if (!waitq) {
			r = -EINVAL;
			goto free_fences;
		}

		for (i = 0, cnt = 0; i < num_fences; i++) {
			struct amdgpu_userq_fence_driver *fence_drv;
			struct amdgpu_userq_fence *userq_fence;
			u32 index;

			userq_fence = to_amdgpu_userq_fence(fences[i]);
			if (!userq_fence) {
				/*
				 * Just waiting on other driver fences should
				 * be good for now
				 */
				r = dma_fence_wait(fences[i], true);
				if (r) {
					dma_fence_put(fences[i]);
					goto free_fences;
				}

				dma_fence_put(fences[i]);
				continue;
			}

			fence_drv = userq_fence->fence_drv;
			/*
			 * We need to make sure the user queue release their reference
			 * to the fence drivers at some point before queue destruction.
			 * Otherwise, we would gather those references until we don't
			 * have any more space left and crash.
			 */
			r = xa_alloc(&waitq->fence_drv_xa, &index, fence_drv,
				     xa_limit_32b, GFP_KERNEL);
			if (r)
				goto free_fences;

			amdgpu_userq_fence_driver_get(fence_drv);

			/* Store drm syncobj's gpu va address and value */
			fence_info[cnt].va = fence_drv->va;
			fence_info[cnt].value = fences[i]->seqno;

			dma_fence_put(fences[i]);
			/* Increment the actual userq fence count */
			cnt++;
		}

		wait_info->num_fences = cnt;
		/* Copy userq fence info to user space */
		if (copy_to_user(u64_to_user_ptr(wait_info->out_fences),
				 fence_info, wait_info->num_fences * sizeof(*fence_info))) {
			r = -EFAULT;
			goto free_fences;
		}
	}

free_fences:
	if (fences) {
		while (num_fences-- > 0)
			dma_fence_put(fences[num_fences]);
		kfree(fences);
	}
free_fence_info:
	kfree(fence_info);
exec_fini:
	drm_exec_fini(&exec);
put_gobj_write:
	while (wentry-- > 0)
		drm_gem_object_put(gobj_write[wentry]);
	kfree(gobj_write);
put_gobj_read:
	while (rentry-- > 0)
		drm_gem_object_put(gobj_read[rentry]);
	kfree(gobj_read);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
free_timeline_points:
	kfree(timeline_points);
free_timeline_handles:
	kfree(timeline_handles);
free_syncobj_handles:
	kfree(syncobj_handles);
<<<<<<< HEAD
=======
free_bo_handles_write:
	kfree(bo_handles_write);
free_bo_handles_read:
	kfree(bo_handles_read);

	if (waitq)
		amdgpu_userq_put(waitq);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return r;
}
