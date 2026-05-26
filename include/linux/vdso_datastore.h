/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_VDSO_DATASTORE_H
#define _LINUX_VDSO_DATASTORE_H

<<<<<<< HEAD
#ifdef CONFIG_HAVE_GENERIC_VDSO
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/mm_types.h>

extern const struct vm_special_mapping vdso_vvar_mapping;
struct vm_area_struct *vdso_install_vvar_mapping(struct mm_struct *mm, unsigned long addr);

<<<<<<< HEAD
void __init vdso_setup_data_pages(void);
#else /* !CONFIG_HAVE_GENERIC_VDSO */
static inline void vdso_setup_data_pages(void) { }
#endif /* CONFIG_HAVE_GENERIC_VDSO */

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#endif /* _LINUX_VDSO_DATASTORE_H */
