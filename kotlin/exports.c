// SPDX-License-Identifier: GPL-2.0
/*
 * Kotlin/Native symbol exports.
 *
 * This file re-exports symbols that Kotlin/Native kernel modules need.
 * Similar to rust/exports.c, this ensures that the necessary symbols
 * are available for Kotlin kernel modules that are built as loadable
 * modules (CONFIG_MODULES).
 */

#include <linux/module.h>
