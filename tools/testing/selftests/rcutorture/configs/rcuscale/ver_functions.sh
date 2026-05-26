#!/bin/bash
# SPDX-License-Identifier: GPL-2.0+
#
# Torture-suite-dependent shell functions for the rest of the scripts.
#
# Copyright (C) IBM Corporation, 2015
#
# Authors: Paul E. McKenney <paulmck@linux.ibm.com>

# per_version_boot_params bootparam-string config-file seconds
#
# Adds per-version torture-module parameters to kernels supporting them.
per_version_boot_params () {
<<<<<<< HEAD
	echo	rcuscale.shutdown_secs=$3 \
=======
	echo	rcuscale.shutdown=1 \
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		rcuscale.verbose=0 \
		$1
}
