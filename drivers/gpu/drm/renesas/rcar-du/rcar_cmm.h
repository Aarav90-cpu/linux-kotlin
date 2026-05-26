/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * R-Car Display Unit Color Management Module
 *
 * Copyright (C) 2019 Jacopo Mondi <jacopo+renesas@jmondi.org>
 */

#ifndef __RCAR_CMM_H__
#define __RCAR_CMM_H__

#define CM2_LUT_SIZE		256

<<<<<<< HEAD
struct device;
struct drm_color_lut;
=======
struct drm_color_lut;
struct platform_device;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

/**
 * struct rcar_cmm_config - CMM configuration
 *
 * @lut:	1D-LUT configuration
 * @lut.table:	1D-LUT table entries. Disable LUT operations when NULL
 */
struct rcar_cmm_config {
	struct {
		struct drm_color_lut *table;
	} lut;
};

#if IS_ENABLED(CONFIG_DRM_RCAR_CMM)
<<<<<<< HEAD
int rcar_cmm_init(struct device *dev);

int rcar_cmm_enable(struct device *dev);
void rcar_cmm_disable(struct device *dev);

int rcar_cmm_setup(struct device *dev,
		   const struct rcar_cmm_config *config);
#else
static inline int rcar_cmm_init(struct device *dev)
=======
int rcar_cmm_init(struct platform_device *pdev);

int rcar_cmm_enable(struct platform_device *pdev);
void rcar_cmm_disable(struct platform_device *pdev);

int rcar_cmm_setup(struct platform_device *pdev,
		   const struct rcar_cmm_config *config);
#else
static inline int rcar_cmm_init(struct platform_device *pdev)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return -ENODEV;
}

<<<<<<< HEAD
static inline int rcar_cmm_enable(struct device *dev)
=======
static inline int rcar_cmm_enable(struct platform_device *pdev)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return 0;
}

<<<<<<< HEAD
static inline void rcar_cmm_disable(struct device *dev)
{
}

static inline int rcar_cmm_setup(struct device *dev,
=======
static inline void rcar_cmm_disable(struct platform_device *pdev)
{
}

static inline int rcar_cmm_setup(struct platform_device *pdev,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
				 const struct rcar_cmm_config *config)
{
	return 0;
}
#endif /* IS_ENABLED(CONFIG_DRM_RCAR_CMM) */

#endif /* __RCAR_CMM_H__ */
