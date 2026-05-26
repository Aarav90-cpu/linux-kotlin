// SPDX-License-Identifier: GPL-2.0-only
/*
 * Maxim Integrated
 * 7-bit, Multi-Channel Sink/Source Current DAC Driver
 * Copyright (C) 2017 Maxim Integrated
 */

<<<<<<< HEAD
#include <linux/array_size.h>
#include <linux/bits.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/time64.h>
#include <linux/types.h>

#include <linux/iio/driver.h>
#include <linux/iio/iio.h>
=======
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/iio/iio.h>
#include <linux/iio/driver.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/iio/machine.h>

#define DS4422_MAX_DAC_CHANNELS		2
#define DS4424_MAX_DAC_CHANNELS		4

<<<<<<< HEAD
#define DS4424_DAC_MASK			GENMASK(6, 0)
#define DS4404_DAC_MASK			GENMASK(4, 0)
#define DS4424_DAC_SOURCE		BIT(7)

#define DS4424_DAC_ADDR(chan)   ((chan) + 0xf8)
=======
#define DS4424_DAC_ADDR(chan)   ((chan) + 0xf8)
#define DS4424_SOURCE_I		1
#define DS4424_SINK_I		0
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#define DS4424_CHANNEL(chan) { \
	.type = IIO_CURRENT, \
	.indexed = 1, \
	.output = 1, \
	.channel = chan, \
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW), \
}

<<<<<<< HEAD
#define DS4424_CHANNEL_WITH_SCALE(chan) { \
	.type = IIO_CURRENT, \
	.indexed = 1, \
	.output = 1, \
	.channel = chan, \
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) | \
			      BIT(IIO_CHAN_INFO_SCALE), \
}

struct ds4424_chip_info {
	const char *name;
	int vref_mV;
	int scale_denom;
	u8 result_mask;
	u8 num_channels;
};

static const struct ds4424_chip_info ds4402_info = {
	.name = "ds4402",
	.vref_mV = 1230,
	.scale_denom = 4,
	.result_mask = DS4404_DAC_MASK,
	.num_channels = DS4422_MAX_DAC_CHANNELS,
};

static const struct ds4424_chip_info ds4404_info = {
	.name = "ds4404",
	.vref_mV = 1230,
	.scale_denom = 4,
	.result_mask = DS4404_DAC_MASK,
	.num_channels = DS4424_MAX_DAC_CHANNELS,
};

static const struct ds4424_chip_info ds4422_info = {
	.name = "ds4422",
	.vref_mV = 976,
	.scale_denom = 16,
	.result_mask = DS4424_DAC_MASK,
	.num_channels = DS4422_MAX_DAC_CHANNELS,
};

static const struct ds4424_chip_info ds4424_info = {
	.name = "ds4424",
	.vref_mV = 976,
	.scale_denom = 16,
	.result_mask = DS4424_DAC_MASK,
	.num_channels = DS4424_MAX_DAC_CHANNELS,
};

struct ds4424_data {
	struct regmap *regmap;
	struct regulator *vcc_reg;
	const struct ds4424_chip_info *chip_info;
	u32 rfs_ohms[DS4424_MAX_DAC_CHANNELS];
	bool has_rfs;
=======
/*
 * DS4424 DAC control register 8 bits
 * [7]		0: to sink; 1: to source
 * [6:0]	steps to sink/source
 * bit[7] looks like a sign bit, but the value of the register is
 * not a two's complement code considering the bit[6:0] is a absolute
 * distance from the zero point.
 */
union ds4424_raw_data {
	struct {
		u8 dx:7;
		u8 source_bit:1;
	};
	u8 bits;
};

enum ds4424_device_ids {
	ID_DS4422,
	ID_DS4424,
};

struct ds4424_data {
	struct i2c_client *client;
	struct mutex lock;
	uint8_t save[DS4424_MAX_DAC_CHANNELS];
	struct regulator *vcc_reg;
	uint8_t raw[DS4424_MAX_DAC_CHANNELS];
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

static const struct iio_chan_spec ds4424_channels[] = {
	DS4424_CHANNEL(0),
	DS4424_CHANNEL(1),
	DS4424_CHANNEL(2),
	DS4424_CHANNEL(3),
};

<<<<<<< HEAD
static const struct iio_chan_spec ds4424_channels_with_scale[] = {
	DS4424_CHANNEL_WITH_SCALE(0),
	DS4424_CHANNEL_WITH_SCALE(1),
	DS4424_CHANNEL_WITH_SCALE(2),
	DS4424_CHANNEL_WITH_SCALE(3),
};

static const struct regmap_range ds44x2_ranges[] = {
	regmap_reg_range(DS4424_DAC_ADDR(0), DS4424_DAC_ADDR(1)),
};

static const struct regmap_range ds44x4_ranges[] = {
	regmap_reg_range(DS4424_DAC_ADDR(0), DS4424_DAC_ADDR(3)),
};

static const struct regmap_access_table ds44x2_table = {
	.yes_ranges = ds44x2_ranges,
	.n_yes_ranges = ARRAY_SIZE(ds44x2_ranges),
};

static const struct regmap_access_table ds44x4_table = {
	.yes_ranges = ds44x4_ranges,
	.n_yes_ranges = ARRAY_SIZE(ds44x4_ranges),
};

static const struct regmap_config ds44x2_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.cache_type = REGCACHE_MAPLE,
	.max_register = DS4424_DAC_ADDR(1),
	.rd_table = &ds44x2_table,
	.wr_table = &ds44x2_table,
};

static const struct regmap_config ds44x4_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.cache_type = REGCACHE_MAPLE,
	.max_register = DS4424_DAC_ADDR(3),
	.rd_table = &ds44x4_table,
	.wr_table = &ds44x4_table,
};

static int ds4424_init_regmap(struct i2c_client *client,
			      struct iio_dev *indio_dev)
{
	struct ds4424_data *data = iio_priv(indio_dev);
	const struct regmap_config *regmap_config;
	u8 vals[DS4424_MAX_DAC_CHANNELS];
	int ret;

	if (indio_dev->num_channels == DS4424_MAX_DAC_CHANNELS)
		regmap_config = &ds44x4_regmap_config;
	else
		regmap_config = &ds44x2_regmap_config;

	data->regmap = devm_regmap_init_i2c(client, regmap_config);
	if (IS_ERR(data->regmap))
		return dev_err_probe(&client->dev, PTR_ERR(data->regmap),
				     "Failed to init regmap.\n");

	/*
	 * Prime the cache with the bootloader's configuration.
	 * regmap_bulk_read() will automatically populate the cache with
	 * the values read from the hardware.
	 */
	ret = regmap_bulk_read(data->regmap, DS4424_DAC_ADDR(0), vals,
			       indio_dev->num_channels);
	if (ret)
		return dev_err_probe(&client->dev, ret,
				     "Failed to read hardware values\n");

	return 0;
=======
static int ds4424_get_value(struct iio_dev *indio_dev,
			     int *val, int channel)
{
	struct ds4424_data *data = iio_priv(indio_dev);
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_read_byte_data(data->client, DS4424_DAC_ADDR(channel));
	if (ret < 0)
		goto fail;

	*val = ret;

fail:
	mutex_unlock(&data->lock);
	return ret;
}

static int ds4424_set_value(struct iio_dev *indio_dev,
			     int val, struct iio_chan_spec const *chan)
{
	struct ds4424_data *data = iio_priv(indio_dev);
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(data->client,
			DS4424_DAC_ADDR(chan->channel), val);
	if (ret < 0)
		goto fail;

	data->raw[chan->channel] = val;

fail:
	mutex_unlock(&data->lock);
	return ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int ds4424_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val, int *val2, long mask)
{
<<<<<<< HEAD
	struct ds4424_data *data = iio_priv(indio_dev);
	unsigned int regval;
=======
	union ds4424_raw_data raw;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int ret;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
<<<<<<< HEAD
		ret = regmap_read(data->regmap, DS4424_DAC_ADDR(chan->channel),
				  &regval);
		if (ret < 0) {
			dev_err_ratelimited(indio_dev->dev.parent,
					    "Failed to read channel %d: %pe\n",
					    chan->channel, ERR_PTR(ret));
			return ret;
		}

		*val = regval & data->chip_info->result_mask;
		if (!(regval & DS4424_DAC_SOURCE))
			*val = -*val;

		return IIO_VAL_INT;
	case IIO_CHAN_INFO_SCALE:
		if (!data->has_rfs)
			return -EINVAL;

		/* SCALE is mA/step: mV / Ohm = mA. */
		*val = data->chip_info->vref_mV;
		*val2 = data->rfs_ohms[chan->channel] *
			data->chip_info->scale_denom;
		return IIO_VAL_FRACTIONAL;
=======
		ret = ds4424_get_value(indio_dev, val, chan->channel);
		if (ret < 0) {
			pr_err("%s : ds4424_get_value returned %d\n",
							__func__, ret);
			return ret;
		}
		raw.bits = *val;
		*val = raw.dx;
		if (raw.source_bit == DS4424_SINK_I)
			*val = -*val;
		return IIO_VAL_INT;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	default:
		return -EINVAL;
	}
}

static int ds4424_write_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int val, int val2, long mask)
{
<<<<<<< HEAD
	struct ds4424_data *data = iio_priv(indio_dev);
	unsigned int abs_val;
=======
	union ds4424_raw_data raw;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (val2 != 0)
		return -EINVAL;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
<<<<<<< HEAD
		abs_val = abs(val);
		if (abs_val > data->chip_info->result_mask)
			return -EINVAL;

		/*
		 * Currents exiting the IC (Source) are positive. 0 is a valid
		 * value for no current flow; the direction bit (Source vs Sink)
		 * is treated as don't-care by the hardware at 0.
		 */
		if (val > 0)
			abs_val |= DS4424_DAC_SOURCE;

		return regmap_write(data->regmap, DS4424_DAC_ADDR(chan->channel),
				    abs_val);
=======
		if (val <= S8_MIN || val > S8_MAX)
			return -EINVAL;

		if (val > 0) {
			raw.source_bit = DS4424_SOURCE_I;
			raw.dx = val;
		} else {
			raw.source_bit = DS4424_SINK_I;
			raw.dx = -val;
		}

		return ds4424_set_value(indio_dev, raw.bits, chan);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int ds4424_parse_rfs(struct i2c_client *client,
			    struct ds4424_data *data,
			    struct iio_dev *indio_dev)
{
	struct device *dev = &client->dev;
	int count, ret;

	if (!device_property_present(dev, "maxim,rfs-ohms"))
		return 0;

	count = device_property_count_u32(dev, "maxim,rfs-ohms");
	if (count < 0)
		return dev_err_probe(dev, count, "Failed to count maxim,rfs-ohms entries\n");
	if (count != indio_dev->num_channels)
		return dev_err_probe(dev, -EINVAL, "maxim,rfs-ohms must have %u entries\n",
				     indio_dev->num_channels);

	ret = device_property_read_u32_array(dev, "maxim,rfs-ohms",
					     data->rfs_ohms,
					     indio_dev->num_channels);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to read maxim,rfs-ohms property\n");

	for (unsigned int i = 0; i < indio_dev->num_channels; i++) {
		if (!data->rfs_ohms[i])
			return dev_err_probe(dev, -EINVAL, "maxim,rfs-ohms entry %u is zero\n", i);
	}

	data->has_rfs = true;

	return 0;
=======
static int ds4424_verify_chip(struct iio_dev *indio_dev)
{
	int ret, val;

	ret = ds4424_get_value(indio_dev, &val, 0);
	if (ret < 0)
		dev_err(&indio_dev->dev,
				"%s failed. ret: %d\n", __func__, ret);

	return ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int ds4424_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ds4424_data *data = iio_priv(indio_dev);
	u8 zero_buf[DS4424_MAX_DAC_CHANNELS] = { };
	int ret;

	/* Disable all outputs, bypass cache so the '0' isn't saved */
	regcache_cache_bypass(data->regmap, true);
	ret = regmap_bulk_write(data->regmap, DS4424_DAC_ADDR(0),
				zero_buf, indio_dev->num_channels);
	regcache_cache_bypass(data->regmap, false);
	if (ret) {
		dev_err(dev, "Failed to zero outputs: %pe\n", ERR_PTR(ret));
		return ret;
	}

	regcache_cache_only(data->regmap, true);
	regcache_mark_dirty(data->regmap);

	return 0;
=======
	struct i2c_client *client = to_i2c_client(dev);
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct ds4424_data *data = iio_priv(indio_dev);
	int ret = 0;
	int i;

	for (i = 0; i < indio_dev->num_channels; i++) {
		data->save[i] = data->raw[i];
		ret = ds4424_set_value(indio_dev, 0,
				&indio_dev->channels[i]);
		if (ret < 0)
			return ret;
	}
	return ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int ds4424_resume(struct device *dev)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ds4424_data *data = iio_priv(indio_dev);

	regcache_cache_only(data->regmap, false);
	return regcache_sync(data->regmap);
=======
	struct i2c_client *client = to_i2c_client(dev);
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct ds4424_data *data = iio_priv(indio_dev);
	int ret = 0;
	int i;

	for (i = 0; i < indio_dev->num_channels; i++) {
		ret = ds4424_set_value(indio_dev, data->save[i],
				&indio_dev->channels[i]);
		if (ret < 0)
			return ret;
	}
	return ret;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static DEFINE_SIMPLE_DEV_PM_OPS(ds4424_pm_ops, ds4424_suspend, ds4424_resume);

<<<<<<< HEAD
static const struct iio_info ds4424_iio_info = {
=======
static const struct iio_info ds4424_info = {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.read_raw = ds4424_read_raw,
	.write_raw = ds4424_write_raw,
};

static int ds4424_probe(struct i2c_client *client)
{
<<<<<<< HEAD
	const struct ds4424_chip_info *chip_info;
=======
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	struct ds4424_data *data;
	struct iio_dev *indio_dev;
	int ret;

<<<<<<< HEAD
	chip_info = i2c_get_match_data(client);
	if (!chip_info)
		return -ENODEV;

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*data));
	if (!indio_dev)
		return -ENOMEM;

	data = iio_priv(indio_dev);
	i2c_set_clientdata(client, indio_dev);
<<<<<<< HEAD
	indio_dev->name = chip_info->name;
	data->chip_info = chip_info;
=======
	data->client = client;
	indio_dev->name = id->name;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	data->vcc_reg = devm_regulator_get(&client->dev, "vcc");
	if (IS_ERR(data->vcc_reg))
		return dev_err_probe(&client->dev, PTR_ERR(data->vcc_reg),
				     "Failed to get vcc-supply regulator.\n");

<<<<<<< HEAD
=======
	mutex_init(&data->lock);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	ret = regulator_enable(data->vcc_reg);
	if (ret < 0) {
		dev_err(&client->dev,
				"Unable to enable the regulator.\n");
		return ret;
	}

<<<<<<< HEAD
	/*
	 * The datasheet does not specify a power-up to I2C ready time.
	 * Maintain the existing conservative 1ms delay to ensure the
	 * device is ready for communication.
	 */
	fsleep(1 * USEC_PER_MSEC);

	indio_dev->num_channels = chip_info->num_channels;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->info = &ds4424_iio_info;

	ret = ds4424_init_regmap(client, indio_dev);
	if (ret)
		goto fail;

	ret = ds4424_parse_rfs(client, data, indio_dev);
	if (ret)
		goto fail;

	if (data->has_rfs)
		indio_dev->channels = ds4424_channels_with_scale;
	else
		indio_dev->channels = ds4424_channels;
=======
	usleep_range(1000, 1200);
	ret = ds4424_verify_chip(indio_dev);
	if (ret < 0)
		goto fail;

	switch (id->driver_data) {
	case ID_DS4422:
		indio_dev->num_channels = DS4422_MAX_DAC_CHANNELS;
		break;
	case ID_DS4424:
		indio_dev->num_channels = DS4424_MAX_DAC_CHANNELS;
		break;
	default:
		dev_err(&client->dev,
				"ds4424: Invalid chip id.\n");
		ret = -ENXIO;
		goto fail;
	}

	indio_dev->channels = ds4424_channels;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->info = &ds4424_info;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	ret = iio_device_register(indio_dev);
	if (ret < 0) {
		dev_err(&client->dev,
				"iio_device_register failed. ret: %d\n", ret);
		goto fail;
	}

	return ret;

fail:
	regulator_disable(data->vcc_reg);
	return ret;
}

static void ds4424_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct ds4424_data *data = iio_priv(indio_dev);

	iio_device_unregister(indio_dev);
	regulator_disable(data->vcc_reg);
}

static const struct i2c_device_id ds4424_id[] = {
<<<<<<< HEAD
	{ "ds4402", (kernel_ulong_t)&ds4402_info },
	{ "ds4404", (kernel_ulong_t)&ds4404_info },
	{ "ds4422", (kernel_ulong_t)&ds4422_info },
	{ "ds4424", (kernel_ulong_t)&ds4424_info },
=======
	{ "ds4422", ID_DS4422 },
	{ "ds4424", ID_DS4424 },
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	{ }
};

MODULE_DEVICE_TABLE(i2c, ds4424_id);

static const struct of_device_id ds4424_of_match[] = {
<<<<<<< HEAD
	{ .compatible = "maxim,ds4402", .data = &ds4402_info },
	{ .compatible = "maxim,ds4404", .data = &ds4404_info },
	{ .compatible = "maxim,ds4422", .data = &ds4422_info },
	{ .compatible = "maxim,ds4424", .data = &ds4424_info },
=======
	{ .compatible = "maxim,ds4422" },
	{ .compatible = "maxim,ds4424" },
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	{ }
};

MODULE_DEVICE_TABLE(of, ds4424_of_match);

static struct i2c_driver ds4424_driver = {
	.driver = {
		.name	= "ds4424",
		.of_match_table = ds4424_of_match,
		.pm     = pm_sleep_ptr(&ds4424_pm_ops),
	},
	.probe		= ds4424_probe,
	.remove		= ds4424_remove,
	.id_table	= ds4424_id,
};
module_i2c_driver(ds4424_driver);

MODULE_DESCRIPTION("Maxim DS4424 DAC Driver");
MODULE_AUTHOR("Ismail H. Kose <ismail.kose@maximintegrated.com>");
MODULE_AUTHOR("Vishal Sood <vishal.sood@maximintegrated.com>");
MODULE_AUTHOR("David Jung <david.jung@maximintegrated.com>");
MODULE_LICENSE("GPL v2");
