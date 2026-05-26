# SPDX-License-Identifier: GPL-2.0
# Taken from perf makefile
<<<<<<< HEAD
ARCH ?= $(shell uname -m 2>/dev/null || echo not)
override ARCH := $(shell echo $(ARCH) | sed -e s/i.86/x86/ -e s/x86_64/x86/)
=======
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/x86/ -e s/x86_64/x86/)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

TEST_GEN_PROGS := step_after_suspend_test

ifeq ($(ARCH),x86)
TEST_GEN_PROGS += breakpoint_test
endif
ifneq (,$(filter $(ARCH),aarch64 arm64))
TEST_GEN_PROGS += breakpoint_test_arm64
endif

include ../lib.mk

