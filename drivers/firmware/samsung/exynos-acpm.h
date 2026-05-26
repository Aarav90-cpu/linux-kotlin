/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright 2020 Samsung Electronics Co., Ltd.
 * Copyright 2020 Google LLC.
 * Copyright 2024 Linaro Ltd.
 */
#ifndef __EXYNOS_ACPM_H__
#define __EXYNOS_ACPM_H__

struct acpm_xfer {
<<<<<<< HEAD
	const u32 *txd __counted_by_ptr(txcnt);
	u32 *rxd __counted_by_ptr(rxcnt);
	size_t txcnt;
	size_t rxcnt;
=======
	const u32 *txd;
	u32 *rxd;
	size_t txlen;
	size_t rxlen;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	unsigned int acpm_chan_id;
};

struct acpm_handle;

int acpm_do_xfer(struct acpm_handle *handle,
		 const struct acpm_xfer *xfer);

#endif /* __EXYNOS_ACPM_H__ */
