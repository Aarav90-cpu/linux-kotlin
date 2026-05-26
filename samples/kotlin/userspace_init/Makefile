# SPDX-License-Identifier: GPL-2.0
#
# Makefile for Kotlin Userspace Init

# We build init.kexe using kotlinc-native and package it into an initramfs

quiet_cmd_kotlinc_userspace = KOTLINC $@
      cmd_kotlinc_userspace = \
	$(KOTLINC) -opt -target linux_x64 -o $@ $<

$(obj)/init.kexe: $(src)/init.kt FORCE
	$(call if_changed,kotlinc_userspace)

# Create an initramfs specification file
quiet_cmd_gen_initramfs = GEN     $@
      cmd_gen_initramfs = \
	echo "file /init $(obj)/init.kexe 0755 0 0" > $@

$(obj)/initramfs_list: $(obj)/init.kexe FORCE
	$(call if_changed,gen_initramfs)

always-$(CONFIG_SAMPLE_KOTLIN_USERSPACE) += init.kexe initramfs_list
clean-files += init.kexe initramfs_list
