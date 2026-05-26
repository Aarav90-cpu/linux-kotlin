// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright Collabora Ltd., 2021
 *
 * futex cmp requeue test by André Almeida <andrealmeid@collabora.com>
 */

#include <pthread.h>
#include <limits.h>

#include "futextest.h"
#include "kselftest_harness.h"

#define timeout_ns  30000000
#define WAKE_WAIT_US 10000

volatile futex_t *f1;

void *waiterfn(void *arg)
{
	struct timespec to;

	to.tv_sec = 0;
	to.tv_nsec = timeout_ns;

	if (futex_wait(f1, *f1, &to, 0))
		printf("waiter failed errno %d\n", errno);

	return NULL;
}

TEST(requeue_single)
{
	volatile futex_t _f1 = 0;
	volatile futex_t f2 = 0;
	pthread_t waiter[10];
<<<<<<< HEAD
=======
	int res;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	f1 = &_f1;

	/*
	 * Requeue a waiter from f1 to f2, and wake f2.
	 */
<<<<<<< HEAD
	ASSERT_EQ(0, pthread_create(&waiter[0], NULL, waiterfn, NULL));

	usleep(WAKE_WAIT_US);

	EXPECT_EQ(1, futex_cmp_requeue(f1, 0, &f2, 0, 1, 0));
	EXPECT_EQ(1, futex_wake(&f2, 1, 0));
=======
	if (pthread_create(&waiter[0], NULL, waiterfn, NULL))
		ksft_exit_fail_msg("pthread_create failed\n");

	usleep(WAKE_WAIT_US);

	ksft_print_dbg_msg("Requeuing 1 futex from f1 to f2\n");
	res = futex_cmp_requeue(f1, 0, &f2, 0, 1, 0);
	if (res != 1)
		ksft_test_result_fail("futex_requeue simple returned: %d %s\n",
				      res ? errno : res,
				      res ? strerror(errno) : "");

	ksft_print_dbg_msg("Waking 1 futex at f2\n");
	res = futex_wake(&f2, 1, 0);
	if (res != 1) {
		ksft_test_result_fail("futex_requeue simple returned: %d %s\n",
				      res ? errno : res,
				      res ? strerror(errno) : "");
	} else {
		ksft_test_result_pass("futex_requeue simple succeeds\n");
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

TEST(requeue_multiple)
{
	volatile futex_t _f1 = 0;
	volatile futex_t f2 = 0;
	pthread_t waiter[10];
<<<<<<< HEAD
	int i;
=======
	int res, i;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	f1 = &_f1;

	/*
	 * Create 10 waiters at f1. At futex_requeue, wake 3 and requeue 7.
	 * At futex_wake, wake INT_MAX (should be exactly 7).
	 */
<<<<<<< HEAD
	for (i = 0; i < 10; i++)
		ASSERT_EQ(0, pthread_create(&waiter[i], NULL, waiterfn, NULL));

	usleep(WAKE_WAIT_US);

	EXPECT_EQ(10, futex_cmp_requeue(f1, 0, &f2, 3, 7, 0));
	EXPECT_EQ(7, futex_wake(&f2, INT_MAX, 0));
=======
	for (i = 0; i < 10; i++) {
		if (pthread_create(&waiter[i], NULL, waiterfn, NULL))
			ksft_exit_fail_msg("pthread_create failed\n");
	}

	usleep(WAKE_WAIT_US);

	ksft_print_dbg_msg("Waking 3 futexes at f1 and requeuing 7 futexes from f1 to f2\n");
	res = futex_cmp_requeue(f1, 0, &f2, 3, 7, 0);
	if (res != 10) {
		ksft_test_result_fail("futex_requeue many returned: %d %s\n",
				      res ? errno : res,
				      res ? strerror(errno) : "");
	}

	ksft_print_dbg_msg("Waking INT_MAX futexes at f2\n");
	res = futex_wake(&f2, INT_MAX, 0);
	if (res != 7) {
		ksft_test_result_fail("futex_requeue many returned: %d %s\n",
				      res ? errno : res,
				      res ? strerror(errno) : "");
	} else {
		ksft_test_result_pass("futex_requeue many succeeds\n");
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

TEST_HARNESS_MAIN
