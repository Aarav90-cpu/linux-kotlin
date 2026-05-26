// SPDX-License-Identifier: GPL-2.0
#ifndef IORING_REGISTER_H
#define IORING_REGISTER_H

int io_eventfd_unregister(struct io_ring_ctx *ctx);
int io_unregister_personality(struct io_ring_ctx *ctx, unsigned id);
<<<<<<< HEAD
=======
struct file *io_uring_register_get_file(unsigned int fd, bool registered);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#endif
