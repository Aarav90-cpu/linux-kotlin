.. SPDX-License-Identifier: GPL-2.0

Quick Start
===========

This document describes how to get started with Android SDK support in the Linux kernel build system.
With this enabled, you can build full Android Applications (APKs) natively as part of the kernel build.

Requirements
------------

To build Android apps in the kernel tree, you will need:

- Java JDK 17 or later (see Documentation/java/quick-start.rst)
- Gradle (automatically downloaded by the build system)
- Android SDK (automatically downloaded by the build system)

Verifying the Setup
-------------------

The kernel provides a target to check if the Android SDK is available:

.. code-block:: bash

   make androidavailable

If the Android SDK is not found (and ``ANDROID_HOME`` is not set), the build system will automatically download the Android command-line tools and accept the necessary licenses.

Building Android Samples
------------------------

1. Enable the Android sample in your kernel config:

.. code-block:: bash

   make menuconfig
   # Navigate to: Kernel hacking -> Sample kernel code -> Android app samples -> Minimal Android App

   # Or directly:
   scripts/config --enable JAVA --enable GRADLE --enable ANDROID_SDK --module SAMPLE_ANDROID_MINIMAL
   make olddefconfig

2. Build the Android app:

.. code-block:: bash

   make samples/android/minimal.apk

The resulting APK will be placed in the `samples/android/` output directory.
