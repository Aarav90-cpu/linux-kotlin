/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_ADDR2LINE_H
#define __PERF_ADDR2LINE_H

#include <linux/types.h>

struct dso;
struct inline_node;
struct symbol;

<<<<<<< HEAD
=======
extern int addr2line_timeout_ms;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
int cmd__addr2line(const char *dso_name, u64 addr,
		   char **file, unsigned int *line_nr,
		   struct dso *dso,
		   bool unwind_inlines,
		   struct inline_node *node,
		   struct symbol *sym);

#endif /* __PERF_ADDR2LINE_H */
