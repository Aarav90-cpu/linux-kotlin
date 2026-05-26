/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ___ASM_SPARC_PROCESSOR_H
#define ___ASM_SPARC_PROCESSOR_H
<<<<<<< HEAD

#include <asm/vdso/processor.h>

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#if defined(__sparc__) && defined(__arch64__)
#include <asm/processor_64.h>
#else
#include <asm/processor_32.h>
#endif
#endif
