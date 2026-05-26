/*
 * Copyright (C) 2004 Jeff Dike (jdike@addtoit.com)
 * Copyright 2003 PathScale, Inc.
 * Licensed under the GPL
 */

#ifndef __VM_FLAGS_X86_H
#define __VM_FLAGS_X86_H

#ifdef CONFIG_X86_32

<<<<<<< HEAD
#define VMA_DATA_DEFAULT_FLAGS	VMA_DATA_FLAGS_TSK_EXEC

#else

#define VMA_STACK_DEFAULT_FLAGS append_vma_flags(VMA_DATA_FLAGS_EXEC, VMA_GROWSDOWN_BIT)
=======
#define VM_DATA_DEFAULT_FLAGS	VM_DATA_FLAGS_TSK_EXEC

#else

#define VM_STACK_DEFAULT_FLAGS (VM_GROWSDOWN | VM_DATA_FLAGS_EXEC)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#endif
#endif
