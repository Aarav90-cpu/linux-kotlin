/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  Copyright (c) 2023 Meta Platforms, Inc. and affiliates
 *  Copyright (c) 2023 Intel and affiliates
 */

int dpll_device_create_ntf(struct dpll_device *dpll);

int dpll_device_delete_ntf(struct dpll_device *dpll);

int dpll_pin_create_ntf(struct dpll_pin *pin);

int dpll_pin_delete_ntf(struct dpll_pin *pin);
<<<<<<< HEAD
=======

int __dpll_pin_change_ntf(struct dpll_pin *pin);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
