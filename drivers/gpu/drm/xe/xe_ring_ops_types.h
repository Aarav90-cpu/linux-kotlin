/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef _XE_RING_OPS_TYPES_H_
#define _XE_RING_OPS_TYPES_H_

<<<<<<< HEAD
#include <linux/types.h>

struct xe_gt;
struct xe_sched_job;

#define MAX_JOB_SIZE_DW 74
=======
struct xe_sched_job;

#define MAX_JOB_SIZE_DW 58
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#define MAX_JOB_SIZE_BYTES (MAX_JOB_SIZE_DW * 4)

/**
 * struct xe_ring_ops - Ring operations
 */
struct xe_ring_ops {
	/** @emit_job: Write job to ring */
	void (*emit_job)(struct xe_sched_job *job);
<<<<<<< HEAD

	/** @emit_aux_table_inv: Emit aux table invalidation to the ring */
	u32 *(*emit_aux_table_inv)(struct xe_gt *gt, u32 *cmd);
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

#endif
