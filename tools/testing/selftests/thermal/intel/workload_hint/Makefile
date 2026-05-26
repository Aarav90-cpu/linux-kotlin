# SPDX-License-Identifier: GPL-2.0
ifndef CROSS_COMPILE
<<<<<<< HEAD
ARCH ?= $(shell uname -m 2>/dev/null || echo not)
override ARCH := $(shell echo $(ARCH) | sed -e s/i.86/x86/ -e s/x86_64/x86/)
=======
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/x86/ -e s/x86_64/x86/)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

ifeq ($(ARCH),x86)
TEST_GEN_PROGS := workload_hint_test

include ../../../lib.mk

endif
endif
