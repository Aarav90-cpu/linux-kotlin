# SPDX-License-Identifier: GPL-2.0
# Makefile for YNL tests

<<<<<<< HEAD
include ../Makefile.deps

CC=gcc
CFLAGS += -std=gnu11 -O2 -W -Wall -Wextra -Wno-unused-parameter -Wshadow \
	-I../lib/ -I../generated/ -I../../../testing/selftests/ \
	-idirafter $(UAPI_PATH)
ifneq ("$(NDEBUG)","1")
  CFLAGS += -g -fsanitize=address -fsanitize=leak -static-libasan
endif

LDLIBS=../lib/ynl.a ../generated/protos.a

TEST_PROGS := \
	devlink.sh \
	ethtool.sh \
	rt-addr.sh \
	rt-route.sh \
	test_ynl_cli.sh \
	test_ynl_ethtool.sh \
# end of TEST_PROGS

TEST_GEN_PROGS := \
	netdev \
	ovs \
	rt-link \
	tc \
# end of TEST_GEN_PROGS

TEST_GEN_FILES := \
	devlink \
	ethtool \
	rt-addr \
	rt-route \
# end of TEST_GEN_FILES

TEST_FILES := \
	ethtool.py \
	ynl_nsim_lib.sh \
# end of TEST_FILES

CFLAGS_netdev:=$(CFLAGS_netdev) $(CFLAGS_rt-link)
CFLAGS_ovs:=$(CFLAGS_ovs_datapath)

include $(wildcard *.d)

INSTALL_PATH ?= $(DESTDIR)/usr/share/kselftest

all: $(TEST_GEN_PROGS) $(TEST_GEN_FILES)

../lib/ynl.a:
	@$(MAKE) -C ../lib

../generated/protos.a:
	@$(MAKE) -C ../generated

$(TEST_GEN_PROGS) $(TEST_GEN_FILES): %: %.c ../lib/ynl.a ../generated/protos.a
	@echo -e '\tCC test $@'
	@$(COMPILE.c) $(CFLAGS_$@) $@.c -o $@.o
	@$(LINK.c) $@.o -o $@  $(LDLIBS)

run_tests:
	@for test in $(TEST_PROGS); do \
		./$$test; \
	done

install: $(TEST_GEN_PROGS) $(TEST_GEN_FILES)
	@mkdir -p $(INSTALL_PATH)/ynl
	@cp ../../../testing/selftests/kselftest/ktap_helpers.sh $(INSTALL_PATH)/
	@for test in $(TEST_PROGS); do \
		name=$$(basename $$test); \
		sed -e 's|^ynl=.*|ynl="ynl"|' \
		    -e 's|^ynl_ethtool=.*|ynl_ethtool="ynl-ethtool"|' \
		    -e 's|KSELFTEST_KTAP_HELPERS=.*|KSELFTEST_KTAP_HELPERS="$(INSTALL_PATH)/ktap_helpers.sh"|' \
		    $$test > $(INSTALL_PATH)/ynl/$$name; \
		chmod +x $(INSTALL_PATH)/ynl/$$name; \
	done
	@for file in $(TEST_FILES); do \
		cp $$file $(INSTALL_PATH)/ynl/$$file; \
	done
	@for bin in $(TEST_GEN_PROGS) $(TEST_GEN_FILES); do \
		cp $$bin $(INSTALL_PATH)/ynl/$$bin; \
	done
	@for test in $(TEST_PROGS) $(TEST_GEN_PROGS); do \
		echo "ynl:$$test"; \
	done > $(INSTALL_PATH)/kselftest-list.txt

clean:
	rm -f *.o *.d *~

distclean: clean
	rm -f $(TEST_GEN_PROGS) $(TEST_GEN_FILES)

.PHONY: all install clean distclean run_tests
.DEFAULT_GOAL=all
=======
TESTS := \
	test_ynl_cli.sh \
	test_ynl_ethtool.sh \
# end of TESTS

all: $(TESTS)

run_tests:
	@for test in $(TESTS); do \
		./$$test; \
	done

install: $(TESTS)
	@mkdir -p $(DESTDIR)/usr/bin
	@mkdir -p $(DESTDIR)/usr/share/kselftest
	@cp ../../../testing/selftests/kselftest/ktap_helpers.sh $(DESTDIR)/usr/share/kselftest/
	@for test in $(TESTS); do \
		name=$$(basename $$test .sh); \
		sed -e 's|^ynl=.*|ynl="ynl"|' \
		    -e 's|^ynl_ethtool=.*|ynl_ethtool="ynl-ethtool"|' \
		    -e 's|KSELFTEST_KTAP_HELPERS=.*|KSELFTEST_KTAP_HELPERS="/usr/share/kselftest/ktap_helpers.sh"|' \
		    $$test > $(DESTDIR)/usr/bin/$$name; \
		chmod +x $(DESTDIR)/usr/bin/$$name; \
	done

clean distclean:
	@# Nothing to clean

.PHONY: all install clean run_tests
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
