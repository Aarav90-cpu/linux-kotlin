// SPDX-License-Identifier: GPL-2.0-only
/*
 * AES-128-CMAC with TLen 16 for IEEE 802.11w BIP
 * Copyright 2008, Jouni Malinen <j@w1.fi>
 * Copyright (C) 2020 Intel Corporation
 */

#include <linux/kernel.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/export.h>
#include <linux/err.h>
#include <crypto/aes-cbc-macs.h>
=======
#include <linux/crypto.h>
#include <linux/export.h>
#include <linux/err.h>
#include <crypto/aes.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#include <net/mac80211.h>
#include "key.h"
#include "aes_cmac.h"

#define AAD_LEN 20

static const u8 zero[IEEE80211_CMAC_256_MIC_LEN];

<<<<<<< HEAD
void ieee80211_aes_cmac(const struct aes_cmac_key *key, const u8 *aad,
			const u8 *data, size_t data_len, u8 *mic,
			unsigned int mic_len)
{
	struct aes_cmac_ctx ctx;
	u8 out[AES_BLOCK_SIZE];
	const __le16 *fc;

	aes_cmac_init(&ctx, key);
	aes_cmac_update(&ctx, aad, AAD_LEN);
	fc = (const __le16 *)aad;
	if (ieee80211_is_beacon(*fc)) {
		/* mask Timestamp field to zero */
		aes_cmac_update(&ctx, zero, 8);
		aes_cmac_update(&ctx, data + 8, data_len - 8 - mic_len);
	} else {
		aes_cmac_update(&ctx, data, data_len - mic_len);
	}
	aes_cmac_update(&ctx, zero, mic_len);
	aes_cmac_final(&ctx, out);
	memcpy(mic, out, mic_len);
=======
int ieee80211_aes_cmac(struct crypto_shash *tfm, const u8 *aad,
		       const u8 *data, size_t data_len, u8 *mic,
		       unsigned int mic_len)
{
	int err;
	SHASH_DESC_ON_STACK(desc, tfm);
	u8 out[AES_BLOCK_SIZE];
	const __le16 *fc;

	desc->tfm = tfm;

	err = crypto_shash_init(desc);
	if (err)
		return err;
	err = crypto_shash_update(desc, aad, AAD_LEN);
	if (err)
		return err;
	fc = (const __le16 *)aad;
	if (ieee80211_is_beacon(*fc)) {
		/* mask Timestamp field to zero */
		err = crypto_shash_update(desc, zero, 8);
		if (err)
			return err;
		err = crypto_shash_update(desc, data + 8,
					  data_len - 8 - mic_len);
		if (err)
			return err;
	} else {
		err = crypto_shash_update(desc, data, data_len - mic_len);
		if (err)
			return err;
	}
	err = crypto_shash_finup(desc, zero, mic_len, out);
	if (err)
		return err;
	memcpy(mic, out, mic_len);

	return 0;
}

struct crypto_shash *ieee80211_aes_cmac_key_setup(const u8 key[],
						  size_t key_len)
{
	struct crypto_shash *tfm;

	tfm = crypto_alloc_shash("cmac(aes)", 0, 0);
	if (!IS_ERR(tfm)) {
		int err = crypto_shash_setkey(tfm, key, key_len);

		if (err) {
			crypto_free_shash(tfm);
			return ERR_PTR(err);
		}
	}

	return tfm;
}

void ieee80211_aes_cmac_key_free(struct crypto_shash *tfm)
{
	crypto_free_shash(tfm);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
