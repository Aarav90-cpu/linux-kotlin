/* SPDX-License-Identifier: GPL-2.0 */
<<<<<<< HEAD
#ifndef __PERF_CLOCKID_H
#define __PERF_CLOCKID_H

#include <time.h>

=======

#ifndef __PERF_CLOCKID_H
#define __PERF_CLOCKID_H

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
struct option;
int parse_clockid(const struct option *opt, const char *str, int unset);

const char *clockid_name(clockid_t clk_id);

#endif
