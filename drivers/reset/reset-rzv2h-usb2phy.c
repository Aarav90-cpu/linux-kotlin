// SPDX-License-Identifier: GPL-2.0
/*
 * Renesas RZ/V2H(P) USB2PHY Port reset control driver
 *
 * Copyright (C) 2025 Renesas Electronics Corporation
 */

<<<<<<< HEAD
#include <linux/auxiliary_bus.h>
#include <linux/delay.h>
#include <linux/idr.h>
=======
#include <linux/cleanup.h>
#include <linux/delay.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/reset-controller.h>

static DEFINE_IDA(auxiliary_ids);

struct rzv2h_usb2phy_reset_of_data {
	const struct reg_sequence *init_seq;
	unsigned int init_nseq;

	const struct reg_sequence *assert_seq;
	unsigned int assert_nseq;

	const struct reg_sequence *deassert_seq;
	unsigned int deassert_nseq;

	u16 reset_reg;
	u16 reset_status_bits;
=======
#include <linux/reset.h>
#include <linux/reset-controller.h>

struct rzv2h_usb2phy_regval {
	u16 reg;
	u16 val;
};

struct rzv2h_usb2phy_reset_of_data {
	const struct rzv2h_usb2phy_regval *init_vals;
	unsigned int init_val_count;

	u16 reset_reg;
	u16 reset_assert_val;
	u16 reset_deassert_val;
	u16 reset_status_bits;
	u16 reset_release_val;

	u16 reset2_reg;
	u16 reset2_acquire_val;
	u16 reset2_release_val;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

struct rzv2h_usb2phy_reset_priv {
	const struct rzv2h_usb2phy_reset_of_data *data;
<<<<<<< HEAD
	struct regmap *regmap;
	struct device *dev;
	struct reset_controller_dev rcdev;
=======
	void __iomem *base;
	struct device *dev;
	struct reset_controller_dev rcdev;
	spinlock_t lock; /* protects register accesses */
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

static inline struct rzv2h_usb2phy_reset_priv
*rzv2h_usbphy_rcdev_to_priv(struct reset_controller_dev *rcdev)
{
	return container_of(rcdev, struct rzv2h_usb2phy_reset_priv, rcdev);
}

static int rzv2h_usbphy_reset_assert(struct reset_controller_dev *rcdev,
				     unsigned long id)
{
	struct rzv2h_usb2phy_reset_priv *priv = rzv2h_usbphy_rcdev_to_priv(rcdev);
<<<<<<< HEAD

	return regmap_multi_reg_write(priv->regmap, priv->data->assert_seq,
				      priv->data->assert_nseq);
=======
	const struct rzv2h_usb2phy_reset_of_data *data = priv->data;

	scoped_guard(spinlock, &priv->lock) {
		writel(data->reset2_acquire_val, priv->base + data->reset2_reg);
		writel(data->reset_assert_val, priv->base + data->reset_reg);
	}

	usleep_range(11, 20);

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int rzv2h_usbphy_reset_deassert(struct reset_controller_dev *rcdev,
				       unsigned long id)
{
	struct rzv2h_usb2phy_reset_priv *priv = rzv2h_usbphy_rcdev_to_priv(rcdev);
<<<<<<< HEAD

	return regmap_multi_reg_write(priv->regmap, priv->data->deassert_seq,
				      priv->data->deassert_nseq);
=======
	const struct rzv2h_usb2phy_reset_of_data *data = priv->data;

	scoped_guard(spinlock, &priv->lock) {
		writel(data->reset_deassert_val, priv->base + data->reset_reg);
		writel(data->reset2_release_val, priv->base + data->reset2_reg);
		writel(data->reset_release_val, priv->base + data->reset_reg);
	}

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int rzv2h_usbphy_reset_status(struct reset_controller_dev *rcdev,
				     unsigned long id)
{
	struct rzv2h_usb2phy_reset_priv *priv = rzv2h_usbphy_rcdev_to_priv(rcdev);
	u32 reg;

<<<<<<< HEAD
	regmap_read(priv->regmap, priv->data->reset_reg, &reg);
=======
	reg = readl(priv->base + priv->data->reset_reg);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	return (reg & priv->data->reset_status_bits) == priv->data->reset_status_bits;
}

static const struct reset_control_ops rzv2h_usbphy_reset_ops = {
	.assert = rzv2h_usbphy_reset_assert,
	.deassert = rzv2h_usbphy_reset_deassert,
	.status = rzv2h_usbphy_reset_status,
};

static int rzv2h_usb2phy_reset_of_xlate(struct reset_controller_dev *rcdev,
					const struct of_phandle_args *reset_spec)
{
	/* No special handling needed, we have only one reset line per device */
	return 0;
}

<<<<<<< HEAD
static void rzv2h_usb2phy_reset_ida_free(void *data)
{
	struct auxiliary_device *adev = data;

	ida_free(&auxiliary_ids, adev->id);
}

static int rzv2h_usb2phy_reset_mux_register(struct device *dev,
					    const char *mux_name)
{
	struct auxiliary_device *adev;
	int id;

	id = ida_alloc(&auxiliary_ids, GFP_KERNEL);
	if (id < 0)
		return id;

	adev = __devm_auxiliary_device_create(dev, dev->driver->name,
					      mux_name, NULL, id);
	if (!adev) {
		ida_free(&auxiliary_ids, id);
		return -ENOMEM;
	}

	return devm_add_action_or_reset(dev, rzv2h_usb2phy_reset_ida_free, adev);
}

static const struct regmap_config rzv2h_usb2phy_reset_regconf = {
	.reg_bits = 32,
	.val_bits = 32,
	.reg_stride = 4,
	.can_sleep = true,
};

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static void rzv2h_usb2phy_reset_pm_runtime_put(void *data)
{
	pm_runtime_put(data);
}

static int rzv2h_usb2phy_reset_probe(struct platform_device *pdev)
{
	const struct rzv2h_usb2phy_reset_of_data *data;
	struct rzv2h_usb2phy_reset_priv *priv;
	struct device *dev = &pdev->dev;
	struct reset_control *rstc;
<<<<<<< HEAD
	void __iomem *base;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int error;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	data = of_device_get_match_data(dev);
	priv->data = data;
	priv->dev = dev;
<<<<<<< HEAD
	base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(base))
		return PTR_ERR(base);

	priv->regmap = devm_regmap_init_mmio(dev, base, &rzv2h_usb2phy_reset_regconf);
	if (IS_ERR(priv->regmap))
		return PTR_ERR(priv->regmap);
=======
	priv->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	rstc = devm_reset_control_get_shared_deasserted(dev, NULL);
	if (IS_ERR(rstc))
		return dev_err_probe(dev, PTR_ERR(rstc),
				     "failed to get deasserted reset\n");

<<<<<<< HEAD
=======
	spin_lock_init(&priv->lock);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	error = devm_pm_runtime_enable(dev);
	if (error)
		return dev_err_probe(dev, error, "Failed to enable pm_runtime\n");

	error = pm_runtime_resume_and_get(dev);
	if (error)
		return dev_err_probe(dev, error, "pm_runtime_resume_and_get failed\n");

	error = devm_add_action_or_reset(dev, rzv2h_usb2phy_reset_pm_runtime_put,
					 dev);
	if (error)
		return dev_err_probe(dev, error, "unable to register cleanup action\n");

<<<<<<< HEAD
	error = regmap_multi_reg_write(priv->regmap, data->init_seq, data->init_nseq);
	if (error)
		return dev_err_probe(dev, error, "failed to initialize PHY registers\n");
=======
	for (unsigned int i = 0; i < data->init_val_count; i++)
		writel(data->init_vals[i].val, priv->base + data->init_vals[i].reg);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	priv->rcdev.ops = &rzv2h_usbphy_reset_ops;
	priv->rcdev.of_reset_n_cells = 0;
	priv->rcdev.nr_resets = 1;
	priv->rcdev.of_xlate = rzv2h_usb2phy_reset_of_xlate;
	priv->rcdev.of_node = dev->of_node;
	priv->rcdev.dev = dev;

	error = devm_reset_controller_register(dev, &priv->rcdev);
	if (error)
		return dev_err_probe(dev, error, "could not register reset controller\n");

<<<<<<< HEAD
	error = rzv2h_usb2phy_reset_mux_register(dev, "vbenctl");
	if (error)
		return dev_err_probe(dev, error, "could not register aux mux\n");

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return 0;
}

/*
 * initialization values required to prepare the PHY to receive
 * assert and deassert requests.
 */
<<<<<<< HEAD
static const struct reg_sequence rzv2h_init_seq[] = {
	{ .reg = 0xc10, .def = 0x67c },
	{ .reg = 0xc14, .def = 0x01f },
	{ .reg = 0x600, .def = 0x909 },
};

static const struct reg_sequence rzv2h_assert_seq[] = {
	{ .reg = 0xb04, .def = 0x303 },
	{ .reg = 0x000, .def = 0x206, .delay_us = 11 },
};

static const struct reg_sequence rzv2h_deassert_seq[] = {
	{ .reg = 0x000, .def = 0x200 },
	{ .reg = 0xb04, .def = 0x003 },
	{ .reg = 0x000, .def = 0x000 },
};

static const struct rzv2h_usb2phy_reset_of_data rzv2h_reset_of_data = {
	.init_seq = rzv2h_init_seq,
	.init_nseq = ARRAY_SIZE(rzv2h_init_seq),
	.assert_seq = rzv2h_assert_seq,
	.assert_nseq = ARRAY_SIZE(rzv2h_assert_seq),
	.deassert_seq = rzv2h_deassert_seq,
	.deassert_nseq = ARRAY_SIZE(rzv2h_deassert_seq),
	.reset_reg = 0,
	.reset_status_bits = BIT(2),
=======
static const struct rzv2h_usb2phy_regval rzv2h_init_vals[] = {
	{ .reg = 0xc10, .val = 0x67c },
	{ .reg = 0xc14, .val = 0x1f },
	{ .reg = 0x600, .val = 0x909 },
};

static const struct rzv2h_usb2phy_reset_of_data rzv2h_reset_of_data = {
	.init_vals = rzv2h_init_vals,
	.init_val_count = ARRAY_SIZE(rzv2h_init_vals),
	.reset_reg = 0,
	.reset_assert_val = 0x206,
	.reset_status_bits = BIT(2),
	.reset_deassert_val = 0x200,
	.reset_release_val = 0x0,
	.reset2_reg = 0xb04,
	.reset2_acquire_val = 0x303,
	.reset2_release_val = 0x3,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

static const struct of_device_id rzv2h_usb2phy_reset_of_match[] = {
	{ .compatible = "renesas,r9a09g057-usb2phy-reset", .data = &rzv2h_reset_of_data },
	{ /* Sentinel */ }
};
MODULE_DEVICE_TABLE(of, rzv2h_usb2phy_reset_of_match);

static struct platform_driver rzv2h_usb2phy_reset_driver = {
	.driver = {
		.name		= "rzv2h_usb2phy_reset",
		.of_match_table	= rzv2h_usb2phy_reset_of_match,
	},
	.probe = rzv2h_usb2phy_reset_probe,
};
module_platform_driver(rzv2h_usb2phy_reset_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lad Prabhakar <prabhakar.mahadev-lad.rj@bp.renesas.com>");
MODULE_DESCRIPTION("Renesas RZ/V2H(P) USB2PHY Control");
