# SPDX-License-Identifier: GPL-2.0

gen := arch/$(ARCH)/include/generated
kapi := $(gen)/asm

<<<<<<< HEAD
kapisyshdr-y := cpucap-defs.h kernel-hwcap.h sysreg-defs.h
=======
kapisyshdr-y := cpucap-defs.h sysreg-defs.h
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

kapi-hdrs-y := $(addprefix $(kapi)/, $(kapisyshdr-y))

targets += $(addprefix ../../../, $(kapi-hdrs-y))

PHONY += kapi

all: $(syscall64) kapi

kapi:   $(kapi-hdrs-y)

quiet_cmd_gen_cpucaps = GEN     $@
      cmd_gen_cpucaps = mkdir -p $(dir $@); $(AWK) -f $(real-prereqs) > $@

<<<<<<< HEAD
quiet_cmd_gen_kernel_hwcap = GEN     $@
      cmd_gen_kernel_hwcap = mkdir -p $(dir $@); /bin/sh -e $(real-prereqs) > $@

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
quiet_cmd_gen_sysreg = GEN     $@
      cmd_gen_sysreg = mkdir -p $(dir $@); $(AWK) -f $(real-prereqs) > $@

$(kapi)/cpucap-defs.h: $(src)/gen-cpucaps.awk $(src)/cpucaps FORCE
	$(call if_changed,gen_cpucaps)

<<<<<<< HEAD
$(kapi)/kernel-hwcap.h: $(src)/gen-kernel-hwcaps.sh $(srctree)/arch/arm64/include/uapi/asm/hwcap.h FORCE
	$(call if_changed,gen_kernel_hwcap)

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
$(kapi)/sysreg-defs.h: $(src)/gen-sysreg.awk $(src)/sysreg FORCE
	$(call if_changed,gen_sysreg)
