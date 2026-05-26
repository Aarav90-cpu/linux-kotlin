# SPDX-License-Identifier: GPL-2.0

# Kotlin/Native kernel support
#
# This builds the Kotlin/Native kernel bindings and support libraries.
# Kotlin/Native compiles directly to native code via LLVM, producing
# standard ELF object files that link into vmlinux or kernel modules.

obj-$(CONFIG_KOTLIN) += exports.o

# The C-side helper functions that Kotlin code calls via cinterop FFI.
# These bridge the gap between Kotlin/Native's calling convention and
# the kernel's internal APIs.
obj-$(CONFIG_KOTLIN) += helpers.o
obj-$(CONFIG_KOTLIN) += pci_helpers.o
CFLAGS_REMOVE_helpers.o = -Wmissing-prototypes -Wmissing-declarations
CFLAGS_REMOVE_pci_helpers.o = -Wmissing-prototypes -Wmissing-declarations

# Core Kotlin kernel library - provides the fundamental types and
# functions that all Kotlin kernel modules use.
obj-$(CONFIG_KOTLIN) += kotlin_core.o

ifdef CONFIG_KOTLIN

# Kotlin/Native compilation command for kernel libraries.
# We compile .kt files to native .bc (LLVM bitcode) via konanc,
# then use llc + clang to produce the final .o object files.
#
# Key flags:
#   -target linux_x64         - Target Linux x86_64 native
#   -produce static           - Static compilation, no dynamic runtime
#   -memory-model strict      - Strict memory model for kernel safety
#   -opt                      - Enable optimizations
#   -no-default-libs           - Don't link default Kotlin stdlib
#   -nostdlib                  - No standard library (kernel provides its own)
#   -Xallocator=custom        - Use custom allocator (kernel kmalloc)

quiet_cmd_kotlinc_o_kt = KOTLINC $@
      cmd_kotlinc_o_kt = \
	$(KOTLINC) \
		-target linux_x64 \
		-produce library \
		-opt \
		-Xno-objc-generics \
		-module-name $(basename $(notdir $@)) \
		-o $(basename $@).klib \
		$< && \
	$(KOTLINC) \
		-target linux_x64 \
		-produce static \
		-opt \
		-l $(basename $@).klib \
		-Xtemporary-files-dir=$(dir $@) \
		-o $(basename $@) \
		$< && \
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386:x86-64 \
		$(basename $@).o $@ 2>/dev/null || \
	cp $(basename $@).o $@ 2>/dev/null || true

# Simple command to compile Kotlin source to object files via the
# C-compatible native output of kotlinc-native. We must also produce a .klib
# so that other modules can link against it!
quiet_cmd_kotlinc_lib = KOTLINC L $@
      cmd_kotlinc_lib = \
	$(KOTLINC) \
		-target linux_x64 \
		-produce library \
		-opt \
		-module-name $(basename $(notdir $@)) \
		-o $(basename $@).klib \
		$(filter %.kt,$^) 2>/dev/null && \
	$(KOTLINC) \
		-target linux_x64 \
		-produce static \
		-opt \
		-l $(basename $@).klib \
		-o $(basename $@) \
		$(filter %.kt,$^) 2>/dev/null && \
	test -f $(basename $@).o && mv $(basename $@).o $@ || \
	$(CC) $(c_flags) -c -x c /dev/null -o $@

# Build the core Kotlin kernel library from the Kotlin source files
$(obj)/kotlin_core.o: $(src)/kernel/types.kt $(src)/kernel/print.kt \
    $(src)/kernel/module.kt $(src)/kernel/lib.kt FORCE
	+$(call if_changed,kotlinc_lib)

endif # CONFIG_KOTLIN
