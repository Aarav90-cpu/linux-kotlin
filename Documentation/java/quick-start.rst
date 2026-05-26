.. SPDX-License-Identifier: GPL-2.0

Quick Start
===========

This document describes how to get started with Java and Gradle support in the Linux kernel build system.

Requirements
------------

To build the kernel with Java and Gradle support, you will need:

- Java JDK 17 or later
- Gradle (handled automatically by the build system)

Installing the Java JDK
-----------------------

You must install a Java JDK on your host system.

- **Debian/Ubuntu:** ``sudo apt install openjdk-17-jdk``
- **Arch Linux:** ``sudo pacman -S jdk17-openjdk``
- **Fedora/RHEL:** ``sudo dnf install java-17-openjdk-devel``

Verifying the Setup
-------------------

The kernel provides targets to check if Java and Gradle are available:

.. code-block:: bash

   make javaavailable
   make gradleavailable

The ``gradleavailable`` target will automatically download Gradle if it is not found on your system.

Building with Java/Gradle Support
---------------------------------

1. Enable Java support in your kernel config:

.. code-block:: bash

   make menuconfig
   # Navigate to: General setup -> Java language support
   # And optionally: General setup -> Gradle build system support

   # Or directly:
   scripts/config --enable JAVA --enable GRADLE

2. Build the kernel as normal:

.. code-block:: bash

   make -j$(nproc)
