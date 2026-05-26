// SPDX-License-Identifier: GPL-2.0-or-later
/*
    Dell Airplane Mode Switch driver
    Copyright (C) 2014-2015  Pali Rohár <pali@kernel.org>

*/

#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/rfkill.h>
#include <linux/input.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#include "dell-rbtn.h"

enum rbtn_type {
	RBTN_UNKNOWN,
	RBTN_TOGGLE,
	RBTN_SLIDER,
};

struct rbtn_data {
	enum rbtn_type type;
	struct rfkill *rfkill;
	struct input_dev *input_dev;
	bool suspended;
};


/*
 * acpi functions
 */

static enum rbtn_type rbtn_check(struct acpi_device *device)
{
	unsigned long long output;
	acpi_status status;

	status = acpi_evaluate_integer(device->handle, "CRBT", NULL, &output);
	if (ACPI_FAILURE(status))
		return RBTN_UNKNOWN;

	switch (output) {
	case 0:
	case 1:
		return RBTN_TOGGLE;
	case 2:
	case 3:
		return RBTN_SLIDER;
	default:
		return RBTN_UNKNOWN;
	}
}

static int rbtn_get(struct acpi_device *device)
{
	unsigned long long output;
	acpi_status status;

	status = acpi_evaluate_integer(device->handle, "GRBT", NULL, &output);
	if (ACPI_FAILURE(status))
		return -EINVAL;

	return !output;
}

static int rbtn_acquire(struct acpi_device *device, bool enable)
{
	struct acpi_object_list input;
	union acpi_object param;
	acpi_status status;

	param.type = ACPI_TYPE_INTEGER;
	param.integer.value = enable;
	input.count = 1;
	input.pointer = &param;

	status = acpi_evaluate_object(device->handle, "ARBT", &input, NULL);
	if (ACPI_FAILURE(status))
		return -EINVAL;

	return 0;
}


/*
 * rfkill device
 */

static void rbtn_rfkill_query(struct rfkill *rfkill, void *data)
{
	struct acpi_device *device = data;
	int state;

	state = rbtn_get(device);
	if (state < 0)
		return;

	rfkill_set_states(rfkill, state, state);
}

static int rbtn_rfkill_set_block(void *data, bool blocked)
{
	/* NOTE: setting soft rfkill state is not supported */
	return -EINVAL;
}

static const struct rfkill_ops rbtn_ops = {
	.query = rbtn_rfkill_query,
	.set_block = rbtn_rfkill_set_block,
};

<<<<<<< HEAD
static int rbtn_rfkill_init(struct device *dev)
{
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
static int rbtn_rfkill_init(struct acpi_device *device)
{
	struct rbtn_data *rbtn_data = device->driver_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int ret;

	if (rbtn_data->rfkill)
		return 0;

	/*
	 * NOTE: rbtn controls all radio devices, not only WLAN
	 *       but rfkill interface does not support "ANY" type
	 *       so "WLAN" type is used
	 */
<<<<<<< HEAD
	rbtn_data->rfkill = rfkill_alloc("dell-rbtn", dev, RFKILL_TYPE_WLAN,
					 &rbtn_ops, ACPI_COMPANION(dev));
=======
	rbtn_data->rfkill = rfkill_alloc("dell-rbtn", &device->dev,
					 RFKILL_TYPE_WLAN, &rbtn_ops, device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!rbtn_data->rfkill)
		return -ENOMEM;

	ret = rfkill_register(rbtn_data->rfkill);
	if (ret) {
		rfkill_destroy(rbtn_data->rfkill);
		rbtn_data->rfkill = NULL;
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
static void rbtn_rfkill_exit(struct device *dev)
{
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
static void rbtn_rfkill_exit(struct acpi_device *device)
{
	struct rbtn_data *rbtn_data = device->driver_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (!rbtn_data->rfkill)
		return;

	rfkill_unregister(rbtn_data->rfkill);
	rfkill_destroy(rbtn_data->rfkill);
	rbtn_data->rfkill = NULL;
}

<<<<<<< HEAD
static void rbtn_rfkill_event(struct device *dev)
{
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);

	if (rbtn_data->rfkill)
		rbtn_rfkill_query(rbtn_data->rfkill, ACPI_COMPANION(dev));
=======
static void rbtn_rfkill_event(struct acpi_device *device)
{
	struct rbtn_data *rbtn_data = device->driver_data;

	if (rbtn_data->rfkill)
		rbtn_rfkill_query(rbtn_data->rfkill, device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}


/*
 * input device
 */

static int rbtn_input_init(struct rbtn_data *rbtn_data)
{
	int ret;

	rbtn_data->input_dev = input_allocate_device();
	if (!rbtn_data->input_dev)
		return -ENOMEM;

	rbtn_data->input_dev->name = "DELL Wireless hotkeys";
	rbtn_data->input_dev->phys = "dellabce/input0";
	rbtn_data->input_dev->id.bustype = BUS_HOST;
	rbtn_data->input_dev->evbit[0] = BIT(EV_KEY);
	set_bit(KEY_RFKILL, rbtn_data->input_dev->keybit);

	ret = input_register_device(rbtn_data->input_dev);
	if (ret) {
		input_free_device(rbtn_data->input_dev);
		rbtn_data->input_dev = NULL;
		return ret;
	}

	return 0;
}

static void rbtn_input_exit(struct rbtn_data *rbtn_data)
{
	input_unregister_device(rbtn_data->input_dev);
	rbtn_data->input_dev = NULL;
}

static void rbtn_input_event(struct rbtn_data *rbtn_data)
{
	input_report_key(rbtn_data->input_dev, KEY_RFKILL, 1);
	input_sync(rbtn_data->input_dev);
	input_report_key(rbtn_data->input_dev, KEY_RFKILL, 0);
	input_sync(rbtn_data->input_dev);
}


/*
 * acpi driver
 */

<<<<<<< HEAD
static int rbtn_probe(struct platform_device *pdev);
static void rbtn_remove(struct platform_device *pdev);
static void rbtn_notify(acpi_handle handle, u32 event, void *data);
=======
static int rbtn_add(struct acpi_device *device);
static void rbtn_remove(struct acpi_device *device);
static void rbtn_notify(struct acpi_device *device, u32 event);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

static const struct acpi_device_id rbtn_ids[] = {
	{ "DELRBTN", 0 },
	{ "DELLABCE", 0 },

	/*
	 * This driver can also handle the "DELLABC6" device that
	 * appears on the XPS 13 9350, but that device is disabled by
	 * the DSDT unless booted with acpi_osi="!Windows 2012"
	 * acpi_osi="!Windows 2013".
	 *
	 * According to Mario at Dell:
	 *
	 *  DELLABC6 is a custom interface that was created solely to
	 *  have airplane mode support for Windows 7.  For Windows 10
	 *  the proper interface is to use that which is handled by
	 *  intel-hid. A OEM airplane mode driver is not used.
	 *
	 *  Since the kernel doesn't identify as Windows 7 it would be
	 *  incorrect to do attempt to use that interface.
	 *
	 * Even if we override _OSI and bind to DELLABC6, we end up with
	 * inconsistent behavior in which userspace can get out of sync
	 * with the rfkill state as it conflicts with events from
	 * intel-hid.
	 *
	 * The upshot is that it is better to just ignore DELLABC6
	 * devices.
	 */

	{ "", 0 },
};

#ifdef CONFIG_PM_SLEEP
static void ACPI_SYSTEM_XFACE rbtn_clear_suspended_flag(void *context)
{
	struct rbtn_data *rbtn_data = context;

	rbtn_data->suspended = false;
}

static int rbtn_suspend(struct device *dev)
{
<<<<<<< HEAD
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
	struct acpi_device *device = to_acpi_device(dev);
	struct rbtn_data *rbtn_data = acpi_driver_data(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	rbtn_data->suspended = true;

	return 0;
}

static int rbtn_resume(struct device *dev)
{
<<<<<<< HEAD
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
	struct acpi_device *device = to_acpi_device(dev);
	struct rbtn_data *rbtn_data = acpi_driver_data(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	acpi_status status;

	/*
	 * Upon resume, some BIOSes send an ACPI notification thet triggers
	 * an unwanted input event. In order to ignore it, we use a flag
	 * that we set at suspend and clear once we have received the extra
	 * ACPI notification. Since ACPI notifications are delivered
	 * asynchronously to drivers, we clear the flag from the workqueue
	 * used to deliver the notifications. This should be enough
	 * to have the flag cleared only after we received the extra
	 * notification, if any.
	 */
	status = acpi_os_execute(OSL_NOTIFY_HANDLER,
			 rbtn_clear_suspended_flag, rbtn_data);
	if (ACPI_FAILURE(status))
		rbtn_clear_suspended_flag(rbtn_data);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(rbtn_pm_ops, rbtn_suspend, rbtn_resume);

<<<<<<< HEAD
static struct platform_driver rbtn_driver = {
	.probe = rbtn_probe,
	.remove = rbtn_remove,
	.driver = {
		.name = "dell-rbtn",
		.acpi_match_table = rbtn_ids,
		.pm = &rbtn_pm_ops,
=======
static struct acpi_driver rbtn_driver = {
	.name = "dell-rbtn",
	.ids = rbtn_ids,
	.drv.pm = &rbtn_pm_ops,
	.ops = {
		.add = rbtn_add,
		.remove = rbtn_remove,
		.notify = rbtn_notify,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	},
};


/*
 * notifier export functions
 */

static bool auto_remove_rfkill = true;

static ATOMIC_NOTIFIER_HEAD(rbtn_chain_head);

static int rbtn_inc_count(struct device *dev, void *data)
{
<<<<<<< HEAD
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
	struct acpi_device *device = to_acpi_device(dev);
	struct rbtn_data *rbtn_data = device->driver_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int *count = data;

	if (rbtn_data->type == RBTN_SLIDER)
		(*count)++;

	return 0;
}

static int rbtn_switch_dev(struct device *dev, void *data)
{
<<<<<<< HEAD
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
	struct acpi_device *device = to_acpi_device(dev);
	struct rbtn_data *rbtn_data = device->driver_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	bool enable = data;

	if (rbtn_data->type != RBTN_SLIDER)
		return 0;

	if (enable)
<<<<<<< HEAD
		rbtn_rfkill_init(dev);
	else
		rbtn_rfkill_exit(dev);
=======
		rbtn_rfkill_init(device);
	else
		rbtn_rfkill_exit(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	return 0;
}

int dell_rbtn_notifier_register(struct notifier_block *nb)
{
	bool first;
	int count;
	int ret;

	count = 0;
<<<<<<< HEAD
	ret = driver_for_each_device(&rbtn_driver.driver, NULL, &count,
=======
	ret = driver_for_each_device(&rbtn_driver.drv, NULL, &count,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
				     rbtn_inc_count);
	if (ret || count == 0)
		return -ENODEV;

	first = !rbtn_chain_head.head;

	ret = atomic_notifier_chain_register(&rbtn_chain_head, nb);
	if (ret != 0)
		return ret;

	if (auto_remove_rfkill && first)
<<<<<<< HEAD
		ret = driver_for_each_device(&rbtn_driver.driver, NULL,
=======
		ret = driver_for_each_device(&rbtn_driver.drv, NULL,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
					     (void *)false, rbtn_switch_dev);

	return ret;
}
EXPORT_SYMBOL_GPL(dell_rbtn_notifier_register);

int dell_rbtn_notifier_unregister(struct notifier_block *nb)
{
	int ret;

	ret = atomic_notifier_chain_unregister(&rbtn_chain_head, nb);
	if (ret != 0)
		return ret;

	if (auto_remove_rfkill && !rbtn_chain_head.head)
<<<<<<< HEAD
		ret = driver_for_each_device(&rbtn_driver.driver, NULL,
=======
		ret = driver_for_each_device(&rbtn_driver.drv, NULL,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
					     (void *)true, rbtn_switch_dev);

	return ret;
}
EXPORT_SYMBOL_GPL(dell_rbtn_notifier_unregister);


/*
 * acpi driver functions
 */

<<<<<<< HEAD
static void rbtn_cleanup(struct device *dev)
{
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);

	switch (rbtn_data->type) {
	case RBTN_TOGGLE:
		rbtn_input_exit(rbtn_data);
		break;
	case RBTN_SLIDER:
		rbtn_rfkill_exit(dev);
		break;
	default:
		break;
	}
}

static int rbtn_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
=======
	struct acpi_device *device = ACPI_COMPANION(&pdev->dev);
=======
static int rbtn_add(struct acpi_device *device)
{
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
>>>>>>> 7fb39c93c52e (Sync)
	struct rbtn_data *rbtn_data;
	struct acpi_device *device;
	enum rbtn_type type;
	int ret = 0;

	device = ACPI_COMPANION(&pdev->dev);
	if (!device)
		return -ENODEV;

	type = rbtn_check(device);
	if (type == RBTN_UNKNOWN) {
<<<<<<< HEAD
		dev_info(&pdev->dev, "Unknown device type\n");
		return -EINVAL;
	}

	rbtn_data = devm_kzalloc(&pdev->dev, sizeof(*rbtn_data), GFP_KERNEL);
=======
		dev_info(&device->dev, "Unknown device type\n");
		return -EINVAL;
	}

	rbtn_data = devm_kzalloc(&device->dev, sizeof(*rbtn_data), GFP_KERNEL);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (!rbtn_data)
		return -ENOMEM;

	ret = rbtn_acquire(device, true);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(&pdev->dev, "Cannot enable device\n");
		return ret;
	}

	platform_set_drvdata(pdev, rbtn_data);

	rbtn_data->type = type;
=======
		dev_err(&device->dev, "Cannot enable device\n");
		return ret;
	}

	rbtn_data->type = type;
	device->driver_data = rbtn_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	switch (rbtn_data->type) {
	case RBTN_TOGGLE:
		ret = rbtn_input_init(rbtn_data);
		break;
	case RBTN_SLIDER:
		if (auto_remove_rfkill && rbtn_chain_head.head)
			ret = 0;
		else
<<<<<<< HEAD
			ret = rbtn_rfkill_init(&pdev->dev);
=======
			ret = rbtn_rfkill_init(device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		break;
	default:
		ret = -EINVAL;
		break;
	}
	if (ret)
<<<<<<< HEAD
		goto err;

	ret = acpi_dev_install_notify_handler(device, ACPI_DEVICE_NOTIFY,
					      rbtn_notify, &pdev->dev);
	if (ret)
		goto err_cleanup;

	return 0;

err_cleanup:
	rbtn_cleanup(&pdev->dev);
err:
	rbtn_acquire(device, false);
	return ret;
}

static void rbtn_remove(struct platform_device *pdev)
{
	struct acpi_device *device = ACPI_COMPANION(&pdev->dev);

	acpi_dev_remove_notify_handler(device, ACPI_DEVICE_NOTIFY, rbtn_notify);
	rbtn_cleanup(&pdev->dev);
	rbtn_acquire(device, false);
}

static void rbtn_notify(acpi_handle handle, u32 event, void *data)
{
	struct device *dev = data;
	struct rbtn_data *rbtn_data = dev_get_drvdata(dev);
=======
		rbtn_acquire(device, false);

	return ret;
}

static void rbtn_remove(struct acpi_device *device)
{
	struct rbtn_data *rbtn_data = device->driver_data;

	switch (rbtn_data->type) {
	case RBTN_TOGGLE:
		rbtn_input_exit(rbtn_data);
		break;
	case RBTN_SLIDER:
		rbtn_rfkill_exit(device);
		break;
	default:
		break;
	}

	rbtn_acquire(device, false);
}

static void rbtn_notify(struct acpi_device *device, u32 event)
{
	struct rbtn_data *rbtn_data = device->driver_data;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/*
	 * Some BIOSes send a notification at resume.
	 * Ignore it to prevent unwanted input events.
	 */
	if (rbtn_data->suspended) {
<<<<<<< HEAD
		dev_dbg(dev, "ACPI notification ignored\n");
=======
		dev_dbg(&device->dev, "ACPI notification ignored\n");
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		return;
	}

	if (event != 0x80) {
<<<<<<< HEAD
		dev_info(dev, "Received unknown event (0x%x)\n",
=======
		dev_info(&device->dev, "Received unknown event (0x%x)\n",
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
			 event);
		return;
	}

	switch (rbtn_data->type) {
	case RBTN_TOGGLE:
		rbtn_input_event(rbtn_data);
		break;
	case RBTN_SLIDER:
<<<<<<< HEAD
		rbtn_rfkill_event(dev);
		atomic_notifier_call_chain(&rbtn_chain_head, event, NULL);
=======
		rbtn_rfkill_event(device);
		atomic_notifier_call_chain(&rbtn_chain_head, event, device);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
module_platform_driver(rbtn_driver);
=======

/*
 * module functions
 */

module_acpi_driver(rbtn_driver);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

module_param(auto_remove_rfkill, bool, 0444);

MODULE_PARM_DESC(auto_remove_rfkill, "Automatically remove rfkill devices when "
				     "other modules start receiving events "
				     "from this module and re-add them when "
				     "the last module stops receiving events "
				     "(default true)");
MODULE_DEVICE_TABLE(acpi, rbtn_ids);
MODULE_DESCRIPTION("Dell Airplane Mode Switch driver");
MODULE_AUTHOR("Pali Rohár <pali@kernel.org>");
MODULE_LICENSE("GPL");
