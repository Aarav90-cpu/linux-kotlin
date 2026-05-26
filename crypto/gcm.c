// SPDX-License-Identifier: GPL-2.0-only
/*
 * GCM: Galois/Counter Mode.
 *
 * Copyright (c) 2007 Nokia Siemens Networks - Mikko Herranen <mh1@iki.fi>
 */

<<<<<<< HEAD
#include <crypto/internal/aead.h>
#include <crypto/internal/skcipher.h>
#include <crypto/scatterwalk.h>
#include <crypto/gcm.h>
#include <crypto/gf128hash.h>
=======
#include <crypto/gf128mul.h>
#include <crypto/internal/aead.h>
#include <crypto/internal/skcipher.h>
#include <crypto/internal/hash.h>
#include <crypto/scatterwalk.h>
#include <crypto/gcm.h>
#include <crypto/hash.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

struct gcm_instance_ctx {
	struct crypto_skcipher_spawn ctr;
<<<<<<< HEAD
=======
	struct crypto_ahash_spawn ghash;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

struct crypto_gcm_ctx {
	struct crypto_skcipher *ctr;
<<<<<<< HEAD
	struct ghash_key ghash;
=======
	struct crypto_ahash *ghash;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

struct crypto_rfc4106_ctx {
	struct crypto_aead *child;
	u8 nonce[4];
};

struct crypto_rfc4106_req_ctx {
	struct scatterlist src[3];
	struct scatterlist dst[3];
	struct aead_request subreq;
};

struct crypto_rfc4543_instance_ctx {
	struct crypto_aead_spawn aead;
};

struct crypto_rfc4543_ctx {
	struct crypto_aead *child;
	u8 nonce[4];
};

struct crypto_rfc4543_req_ctx {
	struct aead_request subreq;
};

<<<<<<< HEAD
=======
struct crypto_gcm_ghash_ctx {
	unsigned int cryptlen;
	struct scatterlist *src;
	int (*complete)(struct aead_request *req, u32 flags);
};

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
struct crypto_gcm_req_priv_ctx {
	u8 iv[16];
	u8 auth_tag[16];
	u8 iauth_tag[16];
	struct scatterlist src[3];
	struct scatterlist dst[3];
<<<<<<< HEAD
	struct skcipher_request skreq; /* Must be last */
};

=======
	struct scatterlist sg;
	struct crypto_gcm_ghash_ctx ghash_ctx;
	union {
		struct ahash_request ahreq;
		struct skcipher_request skreq;
	} u;
};

static struct {
	u8 buf[16];
	struct scatterlist sg;
} *gcm_zeroes;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static inline struct crypto_gcm_req_priv_ctx *crypto_gcm_reqctx(
	struct aead_request *req)
{
	unsigned long align = crypto_aead_alignmask(crypto_aead_reqtfm(req));

	return (void *)PTR_ALIGN((u8 *)aead_request_ctx(req), align + 1);
}

static int crypto_gcm_setkey(struct crypto_aead *aead, const u8 *key,
			     unsigned int keylen)
{
	struct crypto_gcm_ctx *ctx = crypto_aead_ctx(aead);
<<<<<<< HEAD
	struct crypto_skcipher *ctr = ctx->ctr;
	struct {
		u8 h[GHASH_BLOCK_SIZE];
=======
	struct crypto_ahash *ghash = ctx->ghash;
	struct crypto_skcipher *ctr = ctx->ctr;
	struct {
		be128 hash;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		u8 iv[16];

		struct crypto_wait wait;

		struct scatterlist sg[1];
		struct skcipher_request req;
	} *data;
	int err;

	crypto_skcipher_clear_flags(ctr, CRYPTO_TFM_REQ_MASK);
	crypto_skcipher_set_flags(ctr, crypto_aead_get_flags(aead) &
				       CRYPTO_TFM_REQ_MASK);
	err = crypto_skcipher_setkey(ctr, key, keylen);
	if (err)
		return err;

	data = kzalloc(sizeof(*data) + crypto_skcipher_reqsize(ctr),
		       GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	crypto_init_wait(&data->wait);
<<<<<<< HEAD
	sg_init_one(data->sg, data->h, sizeof(data->h));
=======
	sg_init_one(data->sg, &data->hash, sizeof(data->hash));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	skcipher_request_set_tfm(&data->req, ctr);
	skcipher_request_set_callback(&data->req, CRYPTO_TFM_REQ_MAY_SLEEP |
						  CRYPTO_TFM_REQ_MAY_BACKLOG,
				      crypto_req_done,
				      &data->wait);
	skcipher_request_set_crypt(&data->req, data->sg, data->sg,
<<<<<<< HEAD
				   sizeof(data->h), data->iv);
=======
				   sizeof(data->hash), data->iv);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	err = crypto_wait_req(crypto_skcipher_encrypt(&data->req),
							&data->wait);

	if (err)
		goto out;

<<<<<<< HEAD
	ghash_preparekey(&ctx->ghash, data->h);
=======
	crypto_ahash_clear_flags(ghash, CRYPTO_TFM_REQ_MASK);
	crypto_ahash_set_flags(ghash, crypto_aead_get_flags(aead) &
			       CRYPTO_TFM_REQ_MASK);
	err = crypto_ahash_setkey(ghash, (u8 *)&data->hash, sizeof(be128));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
out:
	kfree_sensitive(data);
	return err;
}

static int crypto_gcm_setauthsize(struct crypto_aead *tfm,
				  unsigned int authsize)
{
	return crypto_gcm_check_authsize(authsize);
}

static void crypto_gcm_init_common(struct aead_request *req)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	__be32 counter = cpu_to_be32(1);
	struct scatterlist *sg;

	memset(pctx->auth_tag, 0, sizeof(pctx->auth_tag));
	memcpy(pctx->iv, req->iv, GCM_AES_IV_SIZE);
	memcpy(pctx->iv + GCM_AES_IV_SIZE, &counter, 4);

	sg_init_table(pctx->src, 3);
	sg_set_buf(pctx->src, pctx->auth_tag, sizeof(pctx->auth_tag));
	sg = scatterwalk_ffwd(pctx->src + 1, req->src, req->assoclen);
	if (sg != pctx->src + 1)
		sg_chain(pctx->src, 2, sg);

	if (req->src != req->dst) {
		sg_init_table(pctx->dst, 3);
		sg_set_buf(pctx->dst, pctx->auth_tag, sizeof(pctx->auth_tag));
		sg = scatterwalk_ffwd(pctx->dst + 1, req->dst, req->assoclen);
		if (sg != pctx->dst + 1)
			sg_chain(pctx->dst, 2, sg);
	}
}

static void crypto_gcm_init_crypt(struct aead_request *req,
				  unsigned int cryptlen)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_gcm_ctx *ctx = crypto_aead_ctx(aead);
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
<<<<<<< HEAD
	struct skcipher_request *skreq = &pctx->skreq;
=======
	struct skcipher_request *skreq = &pctx->u.skreq;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	struct scatterlist *dst;

	dst = req->src == req->dst ? pctx->src : pctx->dst;

	skcipher_request_set_tfm(skreq, ctx->ctr);
	skcipher_request_set_crypt(skreq, pctx->src, dst,
				     cryptlen + sizeof(pctx->auth_tag),
				     pctx->iv);
}

<<<<<<< HEAD
static void ghash_update_sg_and_pad(struct ghash_ctx *ghash,
				    struct scatterlist *sg, unsigned int len)
{
	static const u8 zeroes[GHASH_BLOCK_SIZE];

	if (len) {
		unsigned int pad_len = -len % GHASH_BLOCK_SIZE;
		struct scatter_walk walk;

		scatterwalk_start(&walk, sg);
		do {
			unsigned int n = scatterwalk_next(&walk, len);

			ghash_update(ghash, walk.addr, n);
			scatterwalk_done_src(&walk, n);
			len -= n;
		} while (len);

		if (pad_len)
			ghash_update(ghash, zeroes, pad_len);
	}
}

static void gcm_hash(struct aead_request *req, struct scatterlist *ctext,
		     unsigned int datalen, u8 out[GHASH_BLOCK_SIZE])
{
	const struct crypto_gcm_ctx *ctx =
		crypto_aead_ctx(crypto_aead_reqtfm(req));
	__be64 lengths[2] = {
		cpu_to_be64(8 * (u64)req->assoclen),
		cpu_to_be64(8 * (u64)datalen),
	};
	struct ghash_ctx ghash;

	ghash_init(&ghash, &ctx->ghash);

	/* Associated data, then zero-padding to the next 16-byte boundary */
	ghash_update_sg_and_pad(&ghash, req->src, req->assoclen);

	/* Ciphertext, then zero-padding to the next 16-byte boundary */
	ghash_update_sg_and_pad(&ghash, ctext, datalen);

	/* Lengths block */
	ghash_update(&ghash, (const u8 *)lengths, sizeof(lengths));

	ghash_final(&ghash, out);
}

static int gcm_add_auth_tag(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);

	gcm_hash(req, sg_next(req->src == req->dst ? pctx->src : pctx->dst),
		 req->cryptlen, pctx->iauth_tag);
	crypto_xor(pctx->auth_tag, pctx->iauth_tag, 16);
	memcpy_to_sglist(req->dst, req->assoclen + req->cryptlen,
			 pctx->auth_tag, crypto_aead_authsize(aead));
	return 0;
}

=======
static inline unsigned int gcm_remain(unsigned int len)
{
	len &= 0xfU;
	return len ? 16 - len : 0;
}

static void gcm_hash_len_done(void *data, int err);

static int gcm_hash_update(struct aead_request *req,
			   crypto_completion_t compl,
			   struct scatterlist *src,
			   unsigned int len, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct ahash_request *ahreq = &pctx->u.ahreq;

	ahash_request_set_callback(ahreq, flags, compl, req);
	ahash_request_set_crypt(ahreq, src, NULL, len);

	return crypto_ahash_update(ahreq);
}

static int gcm_hash_remain(struct aead_request *req,
			   unsigned int remain,
			   crypto_completion_t compl, u32 flags)
{
	return gcm_hash_update(req, compl, &gcm_zeroes->sg, remain, flags);
}

static int gcm_hash_len(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct ahash_request *ahreq = &pctx->u.ahreq;
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;
	be128 lengths;

	lengths.a = cpu_to_be64(req->assoclen * 8);
	lengths.b = cpu_to_be64(gctx->cryptlen * 8);
	memcpy(pctx->iauth_tag, &lengths, 16);
	sg_init_one(&pctx->sg, pctx->iauth_tag, 16);
	ahash_request_set_callback(ahreq, flags, gcm_hash_len_done, req);
	ahash_request_set_crypt(ahreq, &pctx->sg,
				pctx->iauth_tag, sizeof(lengths));

	return crypto_ahash_finup(ahreq);
}

static int gcm_hash_len_continue(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;

	return gctx->complete(req, flags);
}

static void gcm_hash_len_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_len_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash_crypt_remain_continue(struct aead_request *req, u32 flags)
{
	return gcm_hash_len(req, flags) ?:
	       gcm_hash_len_continue(req, flags);
}

static void gcm_hash_crypt_remain_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_crypt_remain_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash_crypt_continue(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;
	unsigned int remain;

	remain = gcm_remain(gctx->cryptlen);
	if (remain)
		return gcm_hash_remain(req, remain,
				       gcm_hash_crypt_remain_done, flags) ?:
		       gcm_hash_crypt_remain_continue(req, flags);

	return gcm_hash_crypt_remain_continue(req, flags);
}

static void gcm_hash_crypt_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_crypt_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash_assoc_remain_continue(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;

	if (gctx->cryptlen)
		return gcm_hash_update(req, gcm_hash_crypt_done,
				       gctx->src, gctx->cryptlen, flags) ?:
		       gcm_hash_crypt_continue(req, flags);

	return gcm_hash_crypt_remain_continue(req, flags);
}

static void gcm_hash_assoc_remain_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_assoc_remain_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash_assoc_continue(struct aead_request *req, u32 flags)
{
	unsigned int remain;

	remain = gcm_remain(req->assoclen);
	if (remain)
		return gcm_hash_remain(req, remain,
				       gcm_hash_assoc_remain_done, flags) ?:
		       gcm_hash_assoc_remain_continue(req, flags);

	return gcm_hash_assoc_remain_continue(req, flags);
}

static void gcm_hash_assoc_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_assoc_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash_init_continue(struct aead_request *req, u32 flags)
{
	if (req->assoclen)
		return gcm_hash_update(req, gcm_hash_assoc_done,
				       req->src, req->assoclen, flags) ?:
		       gcm_hash_assoc_continue(req, flags);

	return gcm_hash_assoc_remain_continue(req, flags);
}

static void gcm_hash_init_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

	err = gcm_hash_init_continue(req, 0);
	if (err == -EINPROGRESS)
		return;

out:
	aead_request_complete(req, err);
}

static int gcm_hash(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct ahash_request *ahreq = &pctx->u.ahreq;
	struct crypto_gcm_ctx *ctx = crypto_aead_ctx(crypto_aead_reqtfm(req));

	ahash_request_set_tfm(ahreq, ctx->ghash);

	ahash_request_set_callback(ahreq, flags, gcm_hash_init_done, req);
	return crypto_ahash_init(ahreq) ?:
	       gcm_hash_init_continue(req, flags);
}

static int gcm_enc_copy_hash(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	u8 *auth_tag = pctx->auth_tag;

	crypto_xor(auth_tag, pctx->iauth_tag, 16);
	scatterwalk_map_and_copy(auth_tag, req->dst,
				 req->assoclen + req->cryptlen,
				 crypto_aead_authsize(aead), 1);
	return 0;
}

static int gcm_encrypt_continue(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;

	gctx->src = sg_next(req->src == req->dst ? pctx->src : pctx->dst);
	gctx->cryptlen = req->cryptlen;
	gctx->complete = gcm_enc_copy_hash;

	return gcm_hash(req, flags);
}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static void gcm_encrypt_done(void *data, int err)
{
	struct aead_request *req = data;

	if (err)
		goto out;

<<<<<<< HEAD
	err = gcm_add_auth_tag(req);
=======
	err = gcm_encrypt_continue(req, 0);
	if (err == -EINPROGRESS)
		return;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

out:
	aead_request_complete(req, err);
}

static int crypto_gcm_encrypt(struct aead_request *req)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
<<<<<<< HEAD
	struct skcipher_request *skreq = &pctx->skreq;
=======
	struct skcipher_request *skreq = &pctx->u.skreq;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	u32 flags = aead_request_flags(req);

	crypto_gcm_init_common(req);
	crypto_gcm_init_crypt(req, req->cryptlen);
	skcipher_request_set_callback(skreq, flags, gcm_encrypt_done, req);

<<<<<<< HEAD
	return crypto_skcipher_encrypt(skreq) ?: gcm_add_auth_tag(req);
=======
	return crypto_skcipher_encrypt(skreq) ?:
	       gcm_encrypt_continue(req, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int crypto_gcm_verify(struct aead_request *req)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	u8 *auth_tag = pctx->auth_tag;
	u8 *iauth_tag = pctx->iauth_tag;
	unsigned int authsize = crypto_aead_authsize(aead);
	unsigned int cryptlen = req->cryptlen - authsize;

	crypto_xor(auth_tag, iauth_tag, 16);
	scatterwalk_map_and_copy(iauth_tag, req->src,
				 req->assoclen + cryptlen, authsize, 0);
	return crypto_memneq(iauth_tag, auth_tag, authsize) ? -EBADMSG : 0;
}

static void gcm_decrypt_done(void *data, int err)
{
	struct aead_request *req = data;

	if (!err)
		err = crypto_gcm_verify(req);

	aead_request_complete(req, err);
}

<<<<<<< HEAD
=======
static int gcm_dec_hash_continue(struct aead_request *req, u32 flags)
{
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
	struct skcipher_request *skreq = &pctx->u.skreq;
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;

	crypto_gcm_init_crypt(req, gctx->cryptlen);
	skcipher_request_set_callback(skreq, flags, gcm_decrypt_done, req);
	return crypto_skcipher_decrypt(skreq) ?: crypto_gcm_verify(req);
}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static int crypto_gcm_decrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_gcm_req_priv_ctx *pctx = crypto_gcm_reqctx(req);
<<<<<<< HEAD
	struct skcipher_request *skreq = &pctx->skreq;
	unsigned int datalen = req->cryptlen - crypto_aead_authsize(aead);

	crypto_gcm_init_common(req);

	gcm_hash(req, sg_next(pctx->src), datalen, pctx->iauth_tag);

	crypto_gcm_init_crypt(req, datalen);
	skcipher_request_set_callback(skreq, aead_request_flags(req),
				      gcm_decrypt_done, req);
	return crypto_skcipher_decrypt(skreq) ?: crypto_gcm_verify(req);
=======
	struct crypto_gcm_ghash_ctx *gctx = &pctx->ghash_ctx;
	unsigned int authsize = crypto_aead_authsize(aead);
	unsigned int cryptlen = req->cryptlen;
	u32 flags = aead_request_flags(req);

	cryptlen -= authsize;

	crypto_gcm_init_common(req);

	gctx->src = sg_next(pctx->src);
	gctx->cryptlen = cryptlen;
	gctx->complete = gcm_dec_hash_continue;

	return gcm_hash(req, flags);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int crypto_gcm_init_tfm(struct crypto_aead *tfm)
{
	struct aead_instance *inst = aead_alg_instance(tfm);
	struct gcm_instance_ctx *ictx = aead_instance_ctx(inst);
	struct crypto_gcm_ctx *ctx = crypto_aead_ctx(tfm);
	struct crypto_skcipher *ctr;
<<<<<<< HEAD
	unsigned long align;

	ctr = crypto_spawn_skcipher(&ictx->ctr);
	if (IS_ERR(ctr))
		return PTR_ERR(ctr);

	ctx->ctr = ctr;
=======
	struct crypto_ahash *ghash;
	unsigned long align;
	int err;

	ghash = crypto_spawn_ahash(&ictx->ghash);
	if (IS_ERR(ghash))
		return PTR_ERR(ghash);

	ctr = crypto_spawn_skcipher(&ictx->ctr);
	err = PTR_ERR(ctr);
	if (IS_ERR(ctr))
		goto err_free_hash;

	ctx->ctr = ctr;
	ctx->ghash = ghash;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	align = crypto_aead_alignmask(tfm);
	align &= ~(crypto_tfm_ctx_alignment() - 1);
	crypto_aead_set_reqsize(tfm,
<<<<<<< HEAD
				align + sizeof(struct crypto_gcm_req_priv_ctx) +
					crypto_skcipher_reqsize(ctr));
	return 0;
=======
		align + offsetof(struct crypto_gcm_req_priv_ctx, u) +
		max(sizeof(struct skcipher_request) +
		    crypto_skcipher_reqsize(ctr),
		    sizeof(struct ahash_request) +
		    crypto_ahash_reqsize(ghash)));

	return 0;

err_free_hash:
	crypto_free_ahash(ghash);
	return err;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void crypto_gcm_exit_tfm(struct crypto_aead *tfm)
{
	struct crypto_gcm_ctx *ctx = crypto_aead_ctx(tfm);

<<<<<<< HEAD
=======
	crypto_free_ahash(ctx->ghash);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	crypto_free_skcipher(ctx->ctr);
}

static void crypto_gcm_free(struct aead_instance *inst)
{
	struct gcm_instance_ctx *ctx = aead_instance_ctx(inst);

	crypto_drop_skcipher(&ctx->ctr);
<<<<<<< HEAD
=======
	crypto_drop_ahash(&ctx->ghash);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	kfree(inst);
}

static int crypto_gcm_create_common(struct crypto_template *tmpl,
<<<<<<< HEAD
				    struct rtattr **tb, const char *ctr_name)
=======
				    struct rtattr **tb,
				    const char *ctr_name,
				    const char *ghash_name)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct skcipher_alg_common *ctr;
	u32 mask;
	struct aead_instance *inst;
	struct gcm_instance_ctx *ctx;
<<<<<<< HEAD
=======
	struct hash_alg_common *ghash;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_AEAD, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*ctx), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;
	ctx = aead_instance_ctx(inst);

<<<<<<< HEAD
=======
	err = crypto_grab_ahash(&ctx->ghash, aead_crypto_instance(inst),
				ghash_name, 0, mask);
	if (err)
		goto err_free_inst;
	ghash = crypto_spawn_ahash_alg(&ctx->ghash);

	err = -EINVAL;
	if (strcmp(ghash->base.cra_name, "ghash") != 0 ||
	    ghash->digestsize != 16)
		goto err_free_inst;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	err = crypto_grab_skcipher(&ctx->ctr, aead_crypto_instance(inst),
				   ctr_name, 0, mask);
	if (err)
		goto err_free_inst;
	ctr = crypto_spawn_skcipher_alg_common(&ctx->ctr);

	/* The skcipher algorithm must be CTR mode, using 16-byte blocks. */
	err = -EINVAL;
	if (strncmp(ctr->base.cra_name, "ctr(", 4) != 0 ||
	    ctr->ivsize != 16 || ctr->base.cra_blocksize != 1)
		goto err_free_inst;

	err = -ENAMETOOLONG;
	if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
		     "gcm(%s", ctr->base.cra_name + 4) >= CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	if (snprintf(inst->alg.base.cra_driver_name, CRYPTO_MAX_ALG_NAME,
<<<<<<< HEAD
		     "gcm_base(%s,ghash-lib)",
		     ctr->base.cra_driver_name) >= CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	inst->alg.base.cra_priority = ctr->base.cra_priority;
=======
		     "gcm_base(%s,%s)", ctr->base.cra_driver_name,
		     ghash->base.cra_driver_name) >=
	    CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	inst->alg.base.cra_priority = (ghash->base.cra_priority +
				       ctr->base.cra_priority) / 2;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	inst->alg.base.cra_blocksize = 1;
	inst->alg.base.cra_alignmask = ctr->base.cra_alignmask;
	inst->alg.base.cra_ctxsize = sizeof(struct crypto_gcm_ctx);
	inst->alg.ivsize = GCM_AES_IV_SIZE;
	inst->alg.chunksize = ctr->chunksize;
	inst->alg.maxauthsize = 16;
	inst->alg.init = crypto_gcm_init_tfm;
	inst->alg.exit = crypto_gcm_exit_tfm;
	inst->alg.setkey = crypto_gcm_setkey;
	inst->alg.setauthsize = crypto_gcm_setauthsize;
	inst->alg.encrypt = crypto_gcm_encrypt;
	inst->alg.decrypt = crypto_gcm_decrypt;

	inst->free = crypto_gcm_free;

	err = aead_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		crypto_gcm_free(inst);
	}
	return err;
}

static int crypto_gcm_create(struct crypto_template *tmpl, struct rtattr **tb)
{
	const char *cipher_name;
	char ctr_name[CRYPTO_MAX_ALG_NAME];

	cipher_name = crypto_attr_alg_name(tb[1]);
	if (IS_ERR(cipher_name))
		return PTR_ERR(cipher_name);

	if (snprintf(ctr_name, CRYPTO_MAX_ALG_NAME, "ctr(%s)", cipher_name) >=
	    CRYPTO_MAX_ALG_NAME)
		return -ENAMETOOLONG;

<<<<<<< HEAD
	return crypto_gcm_create_common(tmpl, tb, ctr_name);
=======
	return crypto_gcm_create_common(tmpl, tb, ctr_name, "ghash");
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int crypto_gcm_base_create(struct crypto_template *tmpl,
				  struct rtattr **tb)
{
	const char *ctr_name;
	const char *ghash_name;

	ctr_name = crypto_attr_alg_name(tb[1]);
	if (IS_ERR(ctr_name))
		return PTR_ERR(ctr_name);

	ghash_name = crypto_attr_alg_name(tb[2]);
	if (IS_ERR(ghash_name))
		return PTR_ERR(ghash_name);

<<<<<<< HEAD
	/*
	 * Originally this parameter allowed requesting a specific
	 * implementation of GHASH.  This is no longer supported.  Now the best
	 * implementation of GHASH is just always used.
	 */
	if (strcmp(ghash_name, "ghash") != 0 &&
	    strcmp(ghash_name, "ghash-lib") != 0)
		return -EINVAL;

	return crypto_gcm_create_common(tmpl, tb, ctr_name);
=======
	return crypto_gcm_create_common(tmpl, tb, ctr_name, ghash_name);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int crypto_rfc4106_setkey(struct crypto_aead *parent, const u8 *key,
				 unsigned int keylen)
{
	struct crypto_rfc4106_ctx *ctx = crypto_aead_ctx(parent);
	struct crypto_aead *child = ctx->child;

	if (keylen < 4)
		return -EINVAL;

	keylen -= 4;
	memcpy(ctx->nonce, key + keylen, 4);

	crypto_aead_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_aead_set_flags(child, crypto_aead_get_flags(parent) &
				     CRYPTO_TFM_REQ_MASK);
	return crypto_aead_setkey(child, key, keylen);
}

static int crypto_rfc4106_setauthsize(struct crypto_aead *parent,
				      unsigned int authsize)
{
	struct crypto_rfc4106_ctx *ctx = crypto_aead_ctx(parent);
	int err;

	err = crypto_rfc4106_check_authsize(authsize);
	if (err)
		return err;

	return crypto_aead_setauthsize(ctx->child, authsize);
}

static struct aead_request *crypto_rfc4106_crypt(struct aead_request *req)
{
	struct crypto_rfc4106_req_ctx *rctx = aead_request_ctx(req);
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_rfc4106_ctx *ctx = crypto_aead_ctx(aead);
	struct aead_request *subreq = &rctx->subreq;
	struct crypto_aead *child = ctx->child;
	struct scatterlist *sg;
	u8 *iv = PTR_ALIGN((u8 *)(subreq + 1) + crypto_aead_reqsize(child),
			   crypto_aead_alignmask(child) + 1);

	scatterwalk_map_and_copy(iv + GCM_AES_IV_SIZE, req->src, 0, req->assoclen - 8, 0);

	memcpy(iv, ctx->nonce, 4);
	memcpy(iv + 4, req->iv, 8);

	sg_init_table(rctx->src, 3);
	sg_set_buf(rctx->src, iv + GCM_AES_IV_SIZE, req->assoclen - 8);
	sg = scatterwalk_ffwd(rctx->src + 1, req->src, req->assoclen);
	if (sg != rctx->src + 1)
		sg_chain(rctx->src, 2, sg);

	if (req->src != req->dst) {
		sg_init_table(rctx->dst, 3);
		sg_set_buf(rctx->dst, iv + GCM_AES_IV_SIZE, req->assoclen - 8);
		sg = scatterwalk_ffwd(rctx->dst + 1, req->dst, req->assoclen);
		if (sg != rctx->dst + 1)
			sg_chain(rctx->dst, 2, sg);
	}

	aead_request_set_tfm(subreq, child);
	aead_request_set_callback(subreq, req->base.flags, req->base.complete,
				  req->base.data);
	aead_request_set_crypt(subreq, rctx->src,
			       req->src == req->dst ? rctx->src : rctx->dst,
			       req->cryptlen, iv);
	aead_request_set_ad(subreq, req->assoclen - 8);

	return subreq;
}

static int crypto_rfc4106_encrypt(struct aead_request *req)
{
	int err;

	err = crypto_ipsec_check_assoclen(req->assoclen);
	if (err)
		return err;

	req = crypto_rfc4106_crypt(req);

	return crypto_aead_encrypt(req);
}

static int crypto_rfc4106_decrypt(struct aead_request *req)
{
	int err;

	err = crypto_ipsec_check_assoclen(req->assoclen);
	if (err)
		return err;

	req = crypto_rfc4106_crypt(req);

	return crypto_aead_decrypt(req);
}

static int crypto_rfc4106_init_tfm(struct crypto_aead *tfm)
{
	struct aead_instance *inst = aead_alg_instance(tfm);
	struct crypto_aead_spawn *spawn = aead_instance_ctx(inst);
	struct crypto_rfc4106_ctx *ctx = crypto_aead_ctx(tfm);
	struct crypto_aead *aead;
	unsigned long align;

	aead = crypto_spawn_aead(spawn);
	if (IS_ERR(aead))
		return PTR_ERR(aead);

	ctx->child = aead;

	align = crypto_aead_alignmask(aead);
	align &= ~(crypto_tfm_ctx_alignment() - 1);
	crypto_aead_set_reqsize(
		tfm,
		sizeof(struct crypto_rfc4106_req_ctx) +
		ALIGN(crypto_aead_reqsize(aead), crypto_tfm_ctx_alignment()) +
		align + 24);

	return 0;
}

static void crypto_rfc4106_exit_tfm(struct crypto_aead *tfm)
{
	struct crypto_rfc4106_ctx *ctx = crypto_aead_ctx(tfm);

	crypto_free_aead(ctx->child);
}

static void crypto_rfc4106_free(struct aead_instance *inst)
{
	crypto_drop_aead(aead_instance_ctx(inst));
	kfree(inst);
}

static int crypto_rfc4106_create(struct crypto_template *tmpl,
				 struct rtattr **tb)
{
	u32 mask;
	struct aead_instance *inst;
	struct crypto_aead_spawn *spawn;
	struct aead_alg *alg;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_AEAD, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*spawn), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	spawn = aead_instance_ctx(inst);
	err = crypto_grab_aead(spawn, aead_crypto_instance(inst),
			       crypto_attr_alg_name(tb[1]), 0, mask);
	if (err)
		goto err_free_inst;

	alg = crypto_spawn_aead_alg(spawn);

	err = -EINVAL;

	/* Underlying IV size must be 12. */
	if (crypto_aead_alg_ivsize(alg) != GCM_AES_IV_SIZE)
		goto err_free_inst;

	/* Not a stream cipher? */
	if (alg->base.cra_blocksize != 1)
		goto err_free_inst;

	err = -ENAMETOOLONG;
	if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
		     "rfc4106(%s)", alg->base.cra_name) >=
	    CRYPTO_MAX_ALG_NAME ||
	    snprintf(inst->alg.base.cra_driver_name, CRYPTO_MAX_ALG_NAME,
		     "rfc4106(%s)", alg->base.cra_driver_name) >=
	    CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = 1;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask;

	inst->alg.base.cra_ctxsize = sizeof(struct crypto_rfc4106_ctx);

	inst->alg.ivsize = GCM_RFC4106_IV_SIZE;
	inst->alg.chunksize = crypto_aead_alg_chunksize(alg);
	inst->alg.maxauthsize = crypto_aead_alg_maxauthsize(alg);

	inst->alg.init = crypto_rfc4106_init_tfm;
	inst->alg.exit = crypto_rfc4106_exit_tfm;

	inst->alg.setkey = crypto_rfc4106_setkey;
	inst->alg.setauthsize = crypto_rfc4106_setauthsize;
	inst->alg.encrypt = crypto_rfc4106_encrypt;
	inst->alg.decrypt = crypto_rfc4106_decrypt;

	inst->free = crypto_rfc4106_free;

	err = aead_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		crypto_rfc4106_free(inst);
	}
	return err;
}

static int crypto_rfc4543_setkey(struct crypto_aead *parent, const u8 *key,
				 unsigned int keylen)
{
	struct crypto_rfc4543_ctx *ctx = crypto_aead_ctx(parent);
	struct crypto_aead *child = ctx->child;

	if (keylen < 4)
		return -EINVAL;

	keylen -= 4;
	memcpy(ctx->nonce, key + keylen, 4);

	crypto_aead_clear_flags(child, CRYPTO_TFM_REQ_MASK);
	crypto_aead_set_flags(child, crypto_aead_get_flags(parent) &
				     CRYPTO_TFM_REQ_MASK);
	return crypto_aead_setkey(child, key, keylen);
}

static int crypto_rfc4543_setauthsize(struct crypto_aead *parent,
				      unsigned int authsize)
{
	struct crypto_rfc4543_ctx *ctx = crypto_aead_ctx(parent);

	if (authsize != 16)
		return -EINVAL;

	return crypto_aead_setauthsize(ctx->child, authsize);
}

static int crypto_rfc4543_crypt(struct aead_request *req, bool enc)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);
	struct crypto_rfc4543_ctx *ctx = crypto_aead_ctx(aead);
	struct crypto_rfc4543_req_ctx *rctx = aead_request_ctx(req);
	struct aead_request *subreq = &rctx->subreq;
	unsigned int authsize = crypto_aead_authsize(aead);
	u8 *iv = PTR_ALIGN((u8 *)(rctx + 1) + crypto_aead_reqsize(ctx->child),
			   crypto_aead_alignmask(ctx->child) + 1);

	if (req->src != req->dst) {
		unsigned int nbytes = req->assoclen + req->cryptlen -
				      (enc ? 0 : authsize);

		memcpy_sglist(req->dst, req->src, nbytes);
	}

	memcpy(iv, ctx->nonce, 4);
	memcpy(iv + 4, req->iv, 8);

	aead_request_set_tfm(subreq, ctx->child);
	aead_request_set_callback(subreq, req->base.flags,
				  req->base.complete, req->base.data);
	aead_request_set_crypt(subreq, req->src, req->dst,
			       enc ? 0 : authsize, iv);
	aead_request_set_ad(subreq, req->assoclen + req->cryptlen -
				    subreq->cryptlen);

	return enc ? crypto_aead_encrypt(subreq) : crypto_aead_decrypt(subreq);
}

static int crypto_rfc4543_encrypt(struct aead_request *req)
{
	return crypto_ipsec_check_assoclen(req->assoclen) ?:
	       crypto_rfc4543_crypt(req, true);
}

static int crypto_rfc4543_decrypt(struct aead_request *req)
{
	return crypto_ipsec_check_assoclen(req->assoclen) ?:
	       crypto_rfc4543_crypt(req, false);
}

static int crypto_rfc4543_init_tfm(struct crypto_aead *tfm)
{
	struct aead_instance *inst = aead_alg_instance(tfm);
	struct crypto_rfc4543_instance_ctx *ictx = aead_instance_ctx(inst);
	struct crypto_aead_spawn *spawn = &ictx->aead;
	struct crypto_rfc4543_ctx *ctx = crypto_aead_ctx(tfm);
	struct crypto_aead *aead;
	unsigned long align;

	aead = crypto_spawn_aead(spawn);
	if (IS_ERR(aead))
		return PTR_ERR(aead);

	ctx->child = aead;

	align = crypto_aead_alignmask(aead);
	align &= ~(crypto_tfm_ctx_alignment() - 1);
	crypto_aead_set_reqsize(
		tfm,
		sizeof(struct crypto_rfc4543_req_ctx) +
		ALIGN(crypto_aead_reqsize(aead), crypto_tfm_ctx_alignment()) +
		align + GCM_AES_IV_SIZE);

	return 0;
}

static void crypto_rfc4543_exit_tfm(struct crypto_aead *tfm)
{
	struct crypto_rfc4543_ctx *ctx = crypto_aead_ctx(tfm);

	crypto_free_aead(ctx->child);
}

static void crypto_rfc4543_free(struct aead_instance *inst)
{
	struct crypto_rfc4543_instance_ctx *ctx = aead_instance_ctx(inst);

	crypto_drop_aead(&ctx->aead);

	kfree(inst);
}

static int crypto_rfc4543_create(struct crypto_template *tmpl,
				struct rtattr **tb)
{
	u32 mask;
	struct aead_instance *inst;
	struct aead_alg *alg;
	struct crypto_rfc4543_instance_ctx *ctx;
	int err;

	err = crypto_check_attr_type(tb, CRYPTO_ALG_TYPE_AEAD, &mask);
	if (err)
		return err;

	inst = kzalloc(sizeof(*inst) + sizeof(*ctx), GFP_KERNEL);
	if (!inst)
		return -ENOMEM;

	ctx = aead_instance_ctx(inst);
	err = crypto_grab_aead(&ctx->aead, aead_crypto_instance(inst),
			       crypto_attr_alg_name(tb[1]), 0, mask);
	if (err)
		goto err_free_inst;

	alg = crypto_spawn_aead_alg(&ctx->aead);

	err = -EINVAL;

	/* Underlying IV size must be 12. */
	if (crypto_aead_alg_ivsize(alg) != GCM_AES_IV_SIZE)
		goto err_free_inst;

	/* Not a stream cipher? */
	if (alg->base.cra_blocksize != 1)
		goto err_free_inst;

	err = -ENAMETOOLONG;
	if (snprintf(inst->alg.base.cra_name, CRYPTO_MAX_ALG_NAME,
		     "rfc4543(%s)", alg->base.cra_name) >=
	    CRYPTO_MAX_ALG_NAME ||
	    snprintf(inst->alg.base.cra_driver_name, CRYPTO_MAX_ALG_NAME,
		     "rfc4543(%s)", alg->base.cra_driver_name) >=
	    CRYPTO_MAX_ALG_NAME)
		goto err_free_inst;

	inst->alg.base.cra_priority = alg->base.cra_priority;
	inst->alg.base.cra_blocksize = 1;
	inst->alg.base.cra_alignmask = alg->base.cra_alignmask;

	inst->alg.base.cra_ctxsize = sizeof(struct crypto_rfc4543_ctx);

	inst->alg.ivsize = GCM_RFC4543_IV_SIZE;
	inst->alg.chunksize = crypto_aead_alg_chunksize(alg);
	inst->alg.maxauthsize = crypto_aead_alg_maxauthsize(alg);

	inst->alg.init = crypto_rfc4543_init_tfm;
	inst->alg.exit = crypto_rfc4543_exit_tfm;

	inst->alg.setkey = crypto_rfc4543_setkey;
	inst->alg.setauthsize = crypto_rfc4543_setauthsize;
	inst->alg.encrypt = crypto_rfc4543_encrypt;
	inst->alg.decrypt = crypto_rfc4543_decrypt;

	inst->free = crypto_rfc4543_free;

	err = aead_register_instance(tmpl, inst);
	if (err) {
err_free_inst:
		crypto_rfc4543_free(inst);
	}
	return err;
}

static struct crypto_template crypto_gcm_tmpls[] = {
	{
		.name = "gcm_base",
		.create = crypto_gcm_base_create,
		.module = THIS_MODULE,
	}, {
		.name = "gcm",
		.create = crypto_gcm_create,
		.module = THIS_MODULE,
	}, {
		.name = "rfc4106",
		.create = crypto_rfc4106_create,
		.module = THIS_MODULE,
	}, {
		.name = "rfc4543",
		.create = crypto_rfc4543_create,
		.module = THIS_MODULE,
	},
};

static int __init crypto_gcm_module_init(void)
{
<<<<<<< HEAD
	return crypto_register_templates(crypto_gcm_tmpls,
					 ARRAY_SIZE(crypto_gcm_tmpls));
=======
	int err;

	gcm_zeroes = kzalloc_obj(*gcm_zeroes);
	if (!gcm_zeroes)
		return -ENOMEM;

	sg_init_one(&gcm_zeroes->sg, gcm_zeroes->buf, sizeof(gcm_zeroes->buf));

	err = crypto_register_templates(crypto_gcm_tmpls,
					ARRAY_SIZE(crypto_gcm_tmpls));
	if (err)
		kfree(gcm_zeroes);

	return err;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void __exit crypto_gcm_module_exit(void)
{
<<<<<<< HEAD
=======
	kfree(gcm_zeroes);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	crypto_unregister_templates(crypto_gcm_tmpls,
				    ARRAY_SIZE(crypto_gcm_tmpls));
}

module_init(crypto_gcm_module_init);
module_exit(crypto_gcm_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Galois/Counter Mode");
MODULE_AUTHOR("Mikko Herranen <mh1@iki.fi>");
MODULE_ALIAS_CRYPTO("gcm_base");
MODULE_ALIAS_CRYPTO("rfc4106");
MODULE_ALIAS_CRYPTO("rfc4543");
MODULE_ALIAS_CRYPTO("gcm");
