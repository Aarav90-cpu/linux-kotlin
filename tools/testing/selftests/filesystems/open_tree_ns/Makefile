# SPDX-License-Identifier: GPL-2.0
TEST_GEN_PROGS := open_tree_ns_test

<<<<<<< HEAD
CFLAGS += -Wall -O0 -g $(KHDR_INCLUDES) $(TOOLS_INCLUDES)
=======
CFLAGS := -Wall -Werror -g $(KHDR_INCLUDES)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
LDLIBS := -lcap

include ../../lib.mk

$(OUTPUT)/open_tree_ns_test: open_tree_ns_test.c ../utils.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
