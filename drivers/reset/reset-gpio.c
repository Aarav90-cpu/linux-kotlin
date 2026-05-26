// SPDX-License-Identifier: GPL-2.0

#include <linux/auxiliary_bus.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/property.h>
=======
#include <linux/of.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/reset-controller.h>

struct reset_gpio_priv {
	struct reset_controller_dev rc;
	struct gpio_desc *reset;
};

static inline struct reset_gpio_priv
*rc_to_reset_gpio(struct reset_controller_dev *rc)
{
	return container_of(rc, struct reset_gpio_priv, rc);
}

static int reset_gpio_assert(struct reset_controller_dev *rc, unsigned long id)
{
	struct reset_gpio_priv *priv = rc_to_reset_gpio(rc);

	return gpiod_set_value_cansleep(priv->reset, 1);
}

static int reset_gpio_deassert(struct reset_controller_dev *rc,
			       unsigned long id)
{
	struct reset_gpio_priv *priv = rc_to_reset_gpio(rc);

	return gpiod_set_value_cansleep(priv->reset, 0);
}

static int reset_gpio_status(struct reset_controller_dev *rc, unsigned long id)
{
	struct reset_gpio_priv *priv = rc_to_reset_gpio(rc);

	return gpiod_get_value_cansleep(priv->reset);
}

static const struct reset_control_ops reset_gpio_ops = {
	.assert = reset_gpio_assert,
	.deassert = reset_gpio_deassert,
	.status = reset_gpio_status,
};

<<<<<<< HEAD
static int reset_gpio_fwnode_xlate(struct reset_controller_dev *rcdev,
				   const struct fwnode_reference_args *reset_spec)
=======
static int reset_gpio_of_xlate(struct reset_controller_dev *rcdev,
			       const struct of_phandle_args *reset_spec)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	return reset_spec->args[0];
}

<<<<<<< HEAD
=======
static void reset_gpio_of_node_put(void *data)
{
	of_node_put(data);
}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static int reset_gpio_probe(struct auxiliary_device *adev,
			    const struct auxiliary_device_id *id)
{
	struct device *dev = &adev->dev;
<<<<<<< HEAD
	struct reset_gpio_priv *priv;
=======
	struct of_phandle_args *platdata = dev_get_platdata(dev);
	struct reset_gpio_priv *priv;
	int ret;

	if (!platdata)
		return -EINVAL;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
=======
	auxiliary_set_drvdata(adev, &priv->rc);

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	priv->reset = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->reset))
		return dev_err_probe(dev, PTR_ERR(priv->reset),
				     "Could not get reset gpios\n");

	priv->rc.ops = &reset_gpio_ops;
	priv->rc.owner = THIS_MODULE;
	priv->rc.dev = dev;
<<<<<<< HEAD

	/* Cells to match GPIO specifier, but it's not really used */
	priv->rc.fwnode_reset_n_cells = 2;
	priv->rc.fwnode_xlate = reset_gpio_fwnode_xlate;
=======
	priv->rc.of_args = platdata;
	ret = devm_add_action_or_reset(dev, reset_gpio_of_node_put,
				       priv->rc.of_node);
	if (ret)
		return ret;

	/* Cells to match GPIO specifier, but it's not really used */
	priv->rc.of_reset_n_cells = 2;
	priv->rc.of_xlate = reset_gpio_of_xlate;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	priv->rc.nr_resets = 1;

	return devm_reset_controller_register(dev, &priv->rc);
}

static const struct auxiliary_device_id reset_gpio_ids[] = {
	{ .name = "reset.gpio" },
	{}
};
MODULE_DEVICE_TABLE(auxiliary, reset_gpio_ids);

static struct auxiliary_driver reset_gpio_driver = {
	.probe		= reset_gpio_probe,
	.id_table	= reset_gpio_ids,
	.driver	= {
		.name = "reset-gpio",
		.suppress_bind_attrs = true,
	},
};
module_auxiliary_driver(reset_gpio_driver);

MODULE_AUTHOR("Krzysztof Kozlowski <krzysztof.kozlowski@linaro.org>");
MODULE_DESCRIPTION("Generic GPIO reset driver");
MODULE_LICENSE("GPL");
