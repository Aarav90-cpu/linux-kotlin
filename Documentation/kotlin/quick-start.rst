.. SPDX-License-Identifier: GPL-2.0

Quick Start
===========

This document describes how to get started with Kotlin/Native kernel development.

Requirements
------------

To build the kernel with Kotlin/Native support, you will need:

- Kotlin/Native compiler (``konanc`` / ``kotlinc-native``) version 2.0.0 or later
- LLVM toolchain (shared with clang/Rust support)
- Standard Linux kernel build dependencies

Installing the Kotlin/Native Compiler
--------------------------------------

Option 1: SDKMAN (recommended)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

   curl -s https://get.sdkman.io | bash
   source "$HOME/.sdkman/bin/sdkman-init.sh"
   sdk install kotlin

Option 2: Direct Download
~~~~~~~~~~~~~~~~~~~~~~~~~

Download the latest Kotlin/Native release from:
https://github.com/JetBrains/kotlin/releases

Extract and add to your PATH:

.. code-block:: bash

   wget https://github.com/JetBrains/kotlin/releases/download/v2.1.0/kotlin-native-linux-x86_64-2.1.0.tar.gz
   tar xzf kotlin-native-linux-x86_64-2.1.0.tar.gz
   export PATH="$PWD/kotlin-native-linux-x86_64-2.1.0/bin:$PATH"

Option 3: Standalone kotlinc-native
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you have Kotlin installed via your distribution's package manager,
ensure that ``kotlinc-native`` or ``konanc`` is available in your PATH.

Verifying the Setup
-------------------

The kernel provides a target to check if Kotlin/Native support is available:

.. code-block:: bash

   make LLVM=1 kotlinavailable

This will either print "Kotlin/Native is available!" or explain what is missing.

Building with Kotlin/Native Support
------------------------------------

The linux-kotlin project provides a unified build wrapper called ``kotnix`` which handles compiling both the Kotlin userspace and the native kernel simultaneously, while transparently passing through standard kernel ``make`` commands.

1. Enable Kotlin support in your kernel config:

.. code-block:: bash

   ./kotnix menuconfig
   # Navigate to: General setup -> Kotlin support

   # Or directly:
   scripts/config --enable KOTLIN

2. Build the kernel and userspace:

.. code-block:: bash

   ./kotnix build

3. Launch the Kernel in QEMU:

.. code-block:: bash

   ./kotnix run

You can also build and run immediately using ``./kotnix all``.

The kotlib Package Manager
--------------------------

``linux-kotlin`` includes a system-wide Kotlin/Android/Compose library manager called ``kotlib``. 
It manages dependencies dynamically for internal Gradle builds within the kernel environment.

Usage:

.. code-block:: bash

   # Add a library to the native Gradle project
   kotlib add io.github.kyant0:backdrop:2.0.0-alpha03

   # Remove a library
   kotlib remove kyant0-backdrop

   # Inject a custom Maven repository
   kotlib --from jitpack https://jitpack.io

   # Sync or clean the project natively
   kotlib sync
   kotlib clean

Building Sample Modules
------------------------

To build the Kotlin/Native sample modules:

.. code-block:: bash

   make LLVM=1 menuconfig
   # Navigate to: Kernel hacking -> Sample kernel code -> Kotlin samples

   make LLVM=1 -j$(nproc)

Writing a Kotlin Kernel Module
-------------------------------

A minimal Kotlin kernel module looks like this:

.. code-block:: kotlin

   // SPDX-License-Identifier: GPL-2.0

   package my.module

   import kernel.types.*
   import kernel.print.*
   import kernel.module.*

   @KernelModule(
       name = "my_module",
       author = "Your Name",
       description = "My first Kotlin kernel module",
       license = "GPL"
   )
   class MyModule : Module {
       override fun init(): KernelResult {
           pr_info("Hello from Kotlin!\n")
           return Errno.SUCCESS
       }

       override fun exit() {
           pr_info("Goodbye from Kotlin!\n")
       }
   }

Place this file as ``my_module.kt`` in the appropriate directory, add
the corresponding ``Kconfig`` and ``Makefile`` entries, and build.

How It Works
------------

Kotlin/Native compiles directly to native machine code via LLVM, without
requiring a JVM. The compilation pipeline is:

1. ``.kt`` source files are compiled by ``kotlinc-native``
2. The compiler produces LLVM bitcode
3. LLVM backend generates native ELF object files
4. Object files are linked into ``vmlinux`` or ``.ko`` modules

The C interop layer (``cinterop``) generates Kotlin bindings from C kernel
headers, allowing Kotlin code to call kernel functions through a type-safe
FFI bridge.

Architecture
~~~~~~~~~~~~

::

   .kt source → kotlinc-native → LLVM IR → native .o → vmlinux / .ko
                                    ↑
   C headers → cinterop → Kotlin bindings (.klib)

Differences from Rust Support
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- **No ``unsafe`` blocks**: Kotlin/Native uses ``@SharedImmutable`` and
  ``@ThreadLocal`` annotations for concurrency safety
- **Garbage Collection**: Disabled in kernel mode; manual memory management
  via ``kmalloc``/``kfree`` helpers
- **Exceptions**: Replaced with ``KResult`` sealed class for error handling
- **Coroutines**: Not available in kernel mode (no runtime support)
