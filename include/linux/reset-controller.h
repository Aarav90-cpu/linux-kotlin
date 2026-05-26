/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_RESET_CONTROLLER_H_
#define _LINUX_RESET_CONTROLLER_H_

#include <linux/list.h>
<<<<<<< HEAD
#include <linux/mutex.h>

struct fwnode_handle;
struct fwnode_reference_args;
=======

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
struct reset_controller_dev;

/**
 * struct reset_control_ops - reset controller driver callbacks
 *
 * @reset: for self-deasserting resets, does all necessary
 *         things to reset the device
 * @assert: manually assert the reset line, if supported
 * @deassert: manually deassert the reset line, if supported
 * @status: return the status of the reset line, if supported
 */
struct reset_control_ops {
	int (*reset)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*assert)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*deassert)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*status)(struct reset_controller_dev *rcdev, unsigned long id);
};

struct module;
struct device_node;
struct of_phandle_args;

/**
 * struct reset_controller_dev - reset controller entity that might
 *                               provide multiple reset controls
 * @ops: a pointer to device specific struct reset_control_ops
 * @owner: kernel module of the reset controller driver
 * @list: internal list of reset controller devices
 * @reset_control_head: head of internal list of requested reset controls
 * @dev: corresponding driver model device struct
 * @of_node: corresponding device tree node as phandle target
<<<<<<< HEAD
 * @of_reset_n_cells: number of cells in reset line specifiers
 * @of_xlate: translation function to translate from specifier as found in the
 *            device tree to id as given to the reset control ops
 * @fwnode: firmware node associated with this device
 * @fwnode_reset_n_cells: number of cells in reset line specifiers
 * @fwnode_xlate: translation function to translate from firmware specifier to
 *                id as given to the reset control ops, defaults to
 *                :c:func:`fwnode_reset_simple_xlate`
 * @nr_resets: number of reset controls in this reset controller device
 * @lock: protects the reset control list from concurrent access
=======
 * @of_args: for reset-gpios controllers: corresponding phandle args with
 *           of_node and GPIO number complementing of_node; either this or
 *           of_node should be present
 * @of_reset_n_cells: number of cells in reset line specifiers
 * @of_xlate: translation function to translate from specifier as found in the
 *            device tree to id as given to the reset control ops, defaults
 *            to :c:func:`of_reset_simple_xlate`.
 * @nr_resets: number of reset controls in this reset controller device
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 */
struct reset_controller_dev {
	const struct reset_control_ops *ops;
	struct module *owner;
	struct list_head list;
	struct list_head reset_control_head;
	struct device *dev;
	struct device_node *of_node;
<<<<<<< HEAD
	int of_reset_n_cells;
	int (*of_xlate)(struct reset_controller_dev *rcdev,
			const struct of_phandle_args *reset_spec);
	struct fwnode_handle *fwnode;
	int fwnode_reset_n_cells;
	int (*fwnode_xlate)(struct reset_controller_dev *rcdev,
			    const struct fwnode_reference_args *reset_spec);
	unsigned int nr_resets;
	struct mutex lock;
=======
	const struct of_phandle_args *of_args;
	int of_reset_n_cells;
	int (*of_xlate)(struct reset_controller_dev *rcdev,
			const struct of_phandle_args *reset_spec);
	unsigned int nr_resets;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

#if IS_ENABLED(CONFIG_RESET_CONTROLLER)
int reset_controller_register(struct reset_controller_dev *rcdev);
void reset_controller_unregister(struct reset_controller_dev *rcdev);

struct device;
int devm_reset_controller_register(struct device *dev,
				   struct reset_controller_dev *rcdev);
#else
static inline int reset_controller_register(struct reset_controller_dev *rcdev)
{
	return 0;
}

static inline void reset_controller_unregister(struct reset_controller_dev *rcdev)
{
}

static inline int devm_reset_controller_register(struct device *dev,
						 struct reset_controller_dev *rcdev)
{
	return 0;
}
#endif

#endif
