/* SPDX-License-Identifier: GPL-2.0 */
/*
 * KVM userfaultfd util
 *
 * Copyright (C) 2018, Red Hat, Inc.
 * Copyright (C) 2019-2022 Google LLC
 */
#include <inttypes.h>
#include <time.h>
#include <pthread.h>
#include <linux/userfaultfd.h>

#include "test_util.h"

typedef int (*uffd_handler_t)(int uffd_mode, int uffd, struct uffd_msg *msg);

struct uffd_reader_args {
	int uffd_mode;
	int uffd;
	useconds_t delay;
	uffd_handler_t handler;
	/* Holds the read end of the pipe for killing the reader. */
	int pipe;
};

struct uffd_desc {
	int uffd;
<<<<<<< HEAD
	u64 num_readers;
=======
	uint64_t num_readers;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	/* Holds the write ends of the pipes for killing the readers. */
	int *pipefds;
	pthread_t *readers;
	struct uffd_reader_args *reader_args;
};

struct uffd_desc *uffd_setup_demand_paging(int uffd_mode, useconds_t delay,
<<<<<<< HEAD
					   void *hva, u64 len,
					   u64 num_readers,
=======
					   void *hva, uint64_t len,
					   uint64_t num_readers,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
					   uffd_handler_t handler);

void uffd_stop_demand_paging(struct uffd_desc *uffd);

#ifdef PRINT_PER_PAGE_UPDATES
#define PER_PAGE_DEBUG(...) printf(__VA_ARGS__)
#else
#define PER_PAGE_DEBUG(...) _no_printf(__VA_ARGS__)
#endif

#ifdef PRINT_PER_VCPU_UPDATES
#define PER_VCPU_DEBUG(...) printf(__VA_ARGS__)
#else
#define PER_VCPU_DEBUG(...) _no_printf(__VA_ARGS__)
#endif
