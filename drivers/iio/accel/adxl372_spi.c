// SPDX-License-Identifier: GPL-2.0+
/*
<<<<<<< HEAD
 * ADXL371/ADXL372 3-Axis Digital Accelerometer SPI driver
=======
 * ADXL372 3-Axis Digital Accelerometer SPI driver
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 *
 * Copyright 2018 Analog Devices Inc.
 */

#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/regmap.h>
#include <linux/spi/spi.h>

#include "adxl372.h"

static const struct regmap_config adxl372_spi_regmap_config = {
	.reg_bits = 7,
	.pad_bits = 1,
	.val_bits = 8,
	.read_flag_mask = BIT(0),
	.readable_noinc_reg = adxl372_readable_noinc_reg,
};

static int adxl372_spi_probe(struct spi_device *spi)
{
<<<<<<< HEAD
	const struct adxl372_chip_info *chip_info;
	struct regmap *regmap;

	chip_info = spi_get_device_match_data(spi);

=======
	const struct spi_device_id *id = spi_get_device_id(spi);
	struct regmap *regmap;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	regmap = devm_regmap_init_spi(spi, &adxl372_spi_regmap_config);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

<<<<<<< HEAD
	return adxl372_probe(&spi->dev, regmap, spi->irq, chip_info);
}

static const struct spi_device_id adxl372_spi_id[] = {
	{ "adxl371", (kernel_ulong_t)&adxl371_chip_info },
	{ "adxl372", (kernel_ulong_t)&adxl372_chip_info },
=======
	return adxl372_probe(&spi->dev, regmap, spi->irq, id->name);
}

static const struct spi_device_id adxl372_spi_id[] = {
	{ "adxl372", 0 },
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	{ }
};
MODULE_DEVICE_TABLE(spi, adxl372_spi_id);

static const struct of_device_id adxl372_of_match[] = {
<<<<<<< HEAD
	{ .compatible = "adi,adxl371", .data = &adxl371_chip_info },
	{ .compatible = "adi,adxl372", .data = &adxl372_chip_info },
=======
	{ .compatible = "adi,adxl372" },
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	{ }
};
MODULE_DEVICE_TABLE(of, adxl372_of_match);

static struct spi_driver adxl372_spi_driver = {
	.driver = {
		.name = "adxl372_spi",
		.of_match_table = adxl372_of_match,
	},
	.probe = adxl372_spi_probe,
	.id_table = adxl372_spi_id,
};

module_spi_driver(adxl372_spi_driver);

MODULE_AUTHOR("Stefan Popa <stefan.popa@analog.com>");
<<<<<<< HEAD
MODULE_AUTHOR("Antoniu Miclaus <antoniu.miclaus@analog.com>");
MODULE_DESCRIPTION("Analog Devices ADXL371/ADXL372 3-axis accelerometer SPI driver");
=======
MODULE_DESCRIPTION("Analog Devices ADXL372 3-axis accelerometer SPI driver");
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
MODULE_LICENSE("GPL");
MODULE_IMPORT_NS("IIO_ADXL372");
