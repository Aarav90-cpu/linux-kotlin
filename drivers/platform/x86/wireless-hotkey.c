// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Airplane mode button for AMD, HP & Xiaomi laptops
 *
 *  Copyright (C) 2014-2017 Alex Hung <alex.hung@canonical.com>
 *  Copyright (C) 2021 Advanced Micro Devices
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/acpi.h>
#include <acpi/acpi_bus.h>

MODULE_DESCRIPTION("Airplane mode button for AMD, HP & Xiaomi laptops");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alex Hung");
MODULE_ALIAS("acpi*:HPQ6001:*");
MODULE_ALIAS("acpi*:WSTADEF:*");
MODULE_ALIAS("acpi*:AMDI0051:*");
MODULE_ALIAS("acpi*:LGEX0815:*");

struct wl_button {
	struct input_dev *input_dev;
	char phys[32];
};

static const struct acpi_device_id wl_ids[] = {
	{"HPQ6001", 0},
	{"WSTADEF", 0},
	{"AMDI0051", 0},
	{"LGEX0815", 0},
	{"", 0},
};

<<<<<<< HEAD
static int wireless_input_setup(struct device *dev)
{
	struct wl_button *button = dev_get_drvdata(dev);
=======
static int wireless_input_setup(struct acpi_device *device)
{
	struct wl_button *button = acpi_driver_data(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int err;

	button->input_dev = input_allocate_device();
	if (!button->input_dev)
		return -ENOMEM;

<<<<<<< HEAD
	snprintf(button->phys, sizeof(button->phys), "%s/input0",
		 acpi_device_hid(ACPI_COMPANION(dev)));
=======
	snprintf(button->phys, sizeof(button->phys), "%s/input0", acpi_device_hid(device));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	button->input_dev->name = "Wireless hotkeys";
	button->input_dev->phys = button->phys;
	button->input_dev->id.bustype = BUS_HOST;
	button->input_dev->evbit[0] = BIT(EV_KEY);
	set_bit(KEY_RFKILL, button->input_dev->keybit);

	err = input_register_device(button->input_dev);
	if (err)
		goto err_free_dev;

	return 0;

err_free_dev:
	input_free_device(button->input_dev);
	return err;
}

<<<<<<< HEAD
static void wireless_input_destroy(struct device *dev)
{
	struct wl_button *button = dev_get_drvdata(dev);
=======
static void wireless_input_destroy(struct acpi_device *device)
{
	struct wl_button *button = acpi_driver_data(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	input_unregister_device(button->input_dev);
	kfree(button);
}

<<<<<<< HEAD
static void wl_notify(acpi_handle handle, u32 event, void *data)
{
	struct wl_button *button = data;
=======
static void wl_notify(struct acpi_device *acpi_dev, u32 event)
{
	struct wl_button *button = acpi_driver_data(acpi_dev);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (event != 0x80) {
		pr_info("Received unknown event (0x%x)\n", event);
		return;
	}

	input_report_key(button->input_dev, KEY_RFKILL, 1);
	input_sync(button->input_dev);
	input_report_key(button->input_dev, KEY_RFKILL, 0);
	input_sync(button->input_dev);
}

<<<<<<< HEAD
static int wl_probe(struct platform_device *pdev)
=======
static int wl_add(struct acpi_device *device)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct acpi_device *adev;
	struct wl_button *button;
	int err;

	adev = ACPI_COMPANION(&pdev->dev);
	if (!adev)
		return -ENODEV;

	button = kzalloc_obj(struct wl_button);
	if (!button)
		return -ENOMEM;

<<<<<<< HEAD
	platform_set_drvdata(pdev, button);

	err = wireless_input_setup(&pdev->dev);
	if (err) {
		pr_err("Failed to setup wireless hotkeys\n");
		kfree(button);
		return err;
	}
	err = acpi_dev_install_notify_handler(adev, ACPI_DEVICE_NOTIFY,
					      wl_notify, button);
	if (err) {
		pr_err("Failed to install ACPI notify handler\n");
		wireless_input_destroy(&pdev->dev);
=======
	device->driver_data = button;

	err = wireless_input_setup(device);
	if (err) {
		pr_err("Failed to setup wireless hotkeys\n");
		kfree(button);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	}

	return err;
}

<<<<<<< HEAD
static void wl_remove(struct platform_device *pdev)
{
	acpi_dev_remove_notify_handler(ACPI_COMPANION(&pdev->dev),
				       ACPI_DEVICE_NOTIFY, wl_notify);
	wireless_input_destroy(&pdev->dev);
}

static struct platform_driver wl_driver = {
	.probe = wl_probe,
	.remove = wl_remove,
	.driver = {
		.name = "wireless-hotkey",
		.acpi_match_table = wl_ids,
	},
};

module_platform_driver(wl_driver);
=======
static void wl_remove(struct acpi_device *device)
{
	wireless_input_destroy(device);
}

static struct acpi_driver wl_driver = {
	.name	= "wireless-hotkey",
	.ids	= wl_ids,
	.ops	= {
		.add	= wl_add,
		.remove	= wl_remove,
		.notify	= wl_notify,
	},
};

module_acpi_driver(wl_driver);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
