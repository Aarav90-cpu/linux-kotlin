// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2010 Red Hat, Inc.
 * Copyright (C) 2016-2023 Christoph Hellwig.
 */
<<<<<<< HEAD
#include <linux/bio-integrity.h>
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/iomap.h>
#include <linux/pagemap.h>
#include "internal.h"
#include "trace.h"

static DEFINE_SPINLOCK(failed_read_lock);
static struct bio_list failed_read_list = BIO_EMPTY_LIST;

<<<<<<< HEAD
static u32 __iomap_read_end_io(struct bio *bio, int error)
{
	struct folio_iter fi;
	u32 folio_count = 0;

	bio_for_each_folio_all(fi, bio) {
		iomap_finish_folio_read(fi.folio, fi.offset, fi.length, error);
		folio_count++;
	}
	if (bio_integrity(bio))
		fs_bio_integrity_free(bio);
	bio_put(bio);
	return folio_count;
=======
static void __iomap_read_end_io(struct bio *bio)
{
	int error = blk_status_to_errno(bio->bi_status);
	struct folio_iter fi;

	bio_for_each_folio_all(fi, bio)
		iomap_finish_folio_read(fi.folio, fi.offset, fi.length, error);
	bio_put(bio);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void
iomap_fail_reads(
	struct work_struct	*work)
{
	struct bio		*bio;
	struct bio_list		tmp = BIO_EMPTY_LIST;
	unsigned long		flags;

	spin_lock_irqsave(&failed_read_lock, flags);
	bio_list_merge_init(&tmp, &failed_read_list);
	spin_unlock_irqrestore(&failed_read_lock, flags);

	while ((bio = bio_list_pop(&tmp)) != NULL) {
<<<<<<< HEAD
		__iomap_read_end_io(bio, blk_status_to_errno(bio->bi_status));
=======
		__iomap_read_end_io(bio);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		cond_resched();
	}
}

static DECLARE_WORK(failed_read_work, iomap_fail_reads);

static void iomap_fail_buffered_read(struct bio *bio)
{
	unsigned long flags;

	/*
	 * Bounce I/O errors to a workqueue to avoid nested i_lock acquisitions
	 * in the fserror code.  The caller no longer owns the bio reference
	 * after the spinlock drops.
	 */
	spin_lock_irqsave(&failed_read_lock, flags);
	if (bio_list_empty(&failed_read_list))
		WARN_ON_ONCE(!schedule_work(&failed_read_work));
	bio_list_add(&failed_read_list, bio);
	spin_unlock_irqrestore(&failed_read_lock, flags);
}

static void iomap_read_end_io(struct bio *bio)
{
	if (bio->bi_status) {
		iomap_fail_buffered_read(bio);
		return;
	}

<<<<<<< HEAD
	__iomap_read_end_io(bio, 0);
}

u32 iomap_finish_ioend_buffered_read(struct iomap_ioend *ioend)
{
	return __iomap_read_end_io(&ioend->io_bio, ioend->io_error);
}

static void iomap_bio_submit_read(const struct iomap_iter *iter,
		struct iomap_read_folio_ctx *ctx)
{
	struct bio *bio = ctx->read_ctx;

	if (iter->iomap.flags & IOMAP_F_INTEGRITY)
		fs_bio_integrity_alloc(bio);
	submit_bio(bio);
}

static struct bio_set *iomap_read_bio_set(struct iomap_read_folio_ctx *ctx)
{
	if (ctx->ops && ctx->ops->bio_set)
		return ctx->ops->bio_set;
	return &fs_bio_set;
}

static void iomap_read_alloc_bio(const struct iomap_iter *iter,
		struct iomap_read_folio_ctx *ctx, size_t plen)
{
	const struct iomap *iomap = &iter->iomap;
	unsigned int nr_vecs = DIV_ROUND_UP(iomap_length(iter), PAGE_SIZE);
	struct bio_set *bio_set = iomap_read_bio_set(ctx);
	struct folio *folio = ctx->cur_folio;
	gfp_t gfp = mapping_gfp_constraint(folio->mapping, GFP_KERNEL);
	gfp_t orig_gfp = gfp;
	struct bio *bio;

	/* Submit the existing range if there was one. */
	if (ctx->read_ctx)
		ctx->ops->submit_read(iter, ctx);

	/* Same as readahead_gfp_mask: */
	if (ctx->rac)
		gfp |= __GFP_NORETRY | __GFP_NOWARN;

	/*
	 * If the bio_alloc fails, try it again for a single page to avoid
	 * having to deal with partial page reads.  This emulates what
	 * do_mpage_read_folio does.
	 */
	bio = bio_alloc_bioset(iomap->bdev, bio_max_segs(nr_vecs), REQ_OP_READ,
			gfp, bio_set);
	if (!bio)
		bio = bio_alloc_bioset(iomap->bdev, 1, REQ_OP_READ, orig_gfp,
				bio_set);
	if (ctx->rac)
		bio->bi_opf |= REQ_RAHEAD;
	bio->bi_iter.bi_sector = iomap_sector(iomap, iter->pos);
	bio->bi_end_io = iomap_read_end_io;
	bio_add_folio_nofail(bio, folio, plen,
			offset_in_folio(folio, iter->pos));
	ctx->read_ctx = bio;
	ctx->read_ctx_file_offset = iter->pos;
}

int iomap_bio_read_folio_range(const struct iomap_iter *iter,
		struct iomap_read_folio_ctx *ctx, size_t plen)
{
	struct folio *folio = ctx->cur_folio;
	struct bio *bio = ctx->read_ctx;

	if (!bio ||
	    bio_end_sector(bio) != iomap_sector(&iter->iomap, iter->pos) ||
	    bio->bi_iter.bi_size > iomap_max_bio_size(&iter->iomap) - plen ||
	    !bio_add_folio(bio, folio, plen, offset_in_folio(folio, iter->pos)))
		iomap_read_alloc_bio(iter, ctx, plen);
	return 0;
}
EXPORT_SYMBOL_GPL(iomap_bio_read_folio_range);

const struct iomap_read_ops iomap_bio_read_ops = {
	.read_folio_range	= iomap_bio_read_folio_range,
	.submit_read		= iomap_bio_submit_read,
=======
	__iomap_read_end_io(bio);
}

static void iomap_bio_submit_read(struct iomap_read_folio_ctx *ctx)
{
	struct bio *bio = ctx->read_ctx;

	if (bio)
		submit_bio(bio);
}

static int iomap_bio_read_folio_range(const struct iomap_iter *iter,
		struct iomap_read_folio_ctx *ctx, size_t plen)
{
	struct folio *folio = ctx->cur_folio;
	const struct iomap *iomap = &iter->iomap;
	loff_t pos = iter->pos;
	size_t poff = offset_in_folio(folio, pos);
	loff_t length = iomap_length(iter);
	sector_t sector;
	struct bio *bio = ctx->read_ctx;

	sector = iomap_sector(iomap, pos);
	if (!bio || bio_end_sector(bio) != sector ||
	    !bio_add_folio(bio, folio, plen, poff)) {
		gfp_t gfp = mapping_gfp_constraint(folio->mapping, GFP_KERNEL);
		gfp_t orig_gfp = gfp;
		unsigned int nr_vecs = DIV_ROUND_UP(length, PAGE_SIZE);

		if (bio)
			submit_bio(bio);

		if (ctx->rac) /* same as readahead_gfp_mask */
			gfp |= __GFP_NORETRY | __GFP_NOWARN;
		bio = bio_alloc(iomap->bdev, bio_max_segs(nr_vecs), REQ_OP_READ,
				     gfp);
		/*
		 * If the bio_alloc fails, try it again for a single page to
		 * avoid having to deal with partial page reads.  This emulates
		 * what do_mpage_read_folio does.
		 */
		if (!bio)
			bio = bio_alloc(iomap->bdev, 1, REQ_OP_READ, orig_gfp);
		if (ctx->rac)
			bio->bi_opf |= REQ_RAHEAD;
		bio->bi_iter.bi_sector = sector;
		bio->bi_end_io = iomap_read_end_io;
		bio_add_folio_nofail(bio, folio, plen, poff);
		ctx->read_ctx = bio;
	}
	return 0;
}

const struct iomap_read_ops iomap_bio_read_ops = {
	.read_folio_range = iomap_bio_read_folio_range,
	.submit_read = iomap_bio_submit_read,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};
EXPORT_SYMBOL_GPL(iomap_bio_read_ops);

int iomap_bio_read_folio_range_sync(const struct iomap_iter *iter,
		struct folio *folio, loff_t pos, size_t len)
{
	const struct iomap *srcmap = iomap_iter_srcmap(iter);
<<<<<<< HEAD
	sector_t sector = iomap_sector(srcmap, pos);
	struct bio_vec bvec;
	struct bio bio;
	int error;

	bio_init(&bio, srcmap->bdev, &bvec, 1, REQ_OP_READ);
	bio.bi_iter.bi_sector = sector;
	bio_add_folio_nofail(&bio, folio, len, offset_in_folio(folio, pos));
	if (srcmap->flags & IOMAP_F_INTEGRITY)
		fs_bio_integrity_alloc(&bio);
	error = submit_bio_wait(&bio);
	if (srcmap->flags & IOMAP_F_INTEGRITY) {
		if (!error)
			error = fs_bio_integrity_verify(&bio, sector, len);
		fs_bio_integrity_free(&bio);
	}
	return error;
=======
	struct bio_vec bvec;
	struct bio bio;

	bio_init(&bio, srcmap->bdev, &bvec, 1, REQ_OP_READ);
	bio.bi_iter.bi_sector = iomap_sector(srcmap, pos);
	bio_add_folio_nofail(&bio, folio, len, offset_in_folio(folio, pos));
	return submit_bio_wait(&bio);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
