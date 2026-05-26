# SPDX-License-Identifier: GPL-2.0
lib-y		:= clear_user.o delay.o copy_from_user.o		\
		   copy_to_user.o copy_page.o				\
		   clear_page.o csum.o insn.o memchr.o memcpy.o		\
		   memset.o memcmp.o strcmp.o strncmp.o strlen.o	\
		   strnlen.o strchr.o strrchr.o tishift.o

<<<<<<< HEAD
=======
ifeq ($(CONFIG_KERNEL_MODE_NEON), y)
obj-$(CONFIG_XOR_BLOCKS)	+= xor-neon.o
CFLAGS_xor-neon.o		+= $(CC_FLAGS_FPU)
CFLAGS_REMOVE_xor-neon.o	+= $(CC_FLAGS_NO_FPU)
endif

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
lib-$(CONFIG_ARCH_HAS_UACCESS_FLUSHCACHE) += uaccess_flushcache.o

obj-$(CONFIG_FUNCTION_ERROR_INJECTION) += error-inject.o

obj-$(CONFIG_ARM64_MTE) += mte.o

obj-$(CONFIG_KASAN_SW_TAGS) += kasan_sw_tags.o
