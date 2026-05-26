# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the ATM Protocol Families.
#

atm-y		:= addr.o pvc.o signaling.o svc.o ioctl.o common.o atm_misc.o raw.o resources.o atm_sysfs.o
<<<<<<< HEAD

obj-$(CONFIG_ATM) += atm.o
obj-$(CONFIG_ATM_BR2684) += br2684.o
atm-$(CONFIG_PROC_FS) += proc.o

=======
mpoa-objs	:= mpc.o mpoa_caches.o mpoa_proc.o

obj-$(CONFIG_ATM) += atm.o
obj-$(CONFIG_ATM_CLIP) += clip.o
obj-$(CONFIG_ATM_BR2684) += br2684.o
atm-$(CONFIG_PROC_FS) += proc.o

obj-$(CONFIG_ATM_LANE) += lec.o
obj-$(CONFIG_ATM_MPOA) += mpoa.o
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
obj-$(CONFIG_PPPOATM) += pppoatm.o
