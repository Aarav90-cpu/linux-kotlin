/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _XOR_H
#define _XOR_H

<<<<<<< HEAD
void xor_gen(void *dest, void **srcs, unsigned int src_cnt, unsigned int bytes);

#endif /* _XOR_H */
=======
#define MAX_XOR_BLOCKS 4

extern void xor_blocks(unsigned int count, unsigned int bytes,
	void *dest, void **srcs);

struct xor_block_template {
        struct xor_block_template *next;
        const char *name;
        int speed;
	void (*do_2)(unsigned long, unsigned long * __restrict,
		     const unsigned long * __restrict);
	void (*do_3)(unsigned long, unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict);
	void (*do_4)(unsigned long, unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict);
	void (*do_5)(unsigned long, unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict,
		     const unsigned long * __restrict);
};

#endif
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
