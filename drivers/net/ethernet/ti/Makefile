# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the TI network device drivers.
#

obj-$(CONFIG_TI_PRUETH) += icssm-prueth.o
icssm-prueth-y := icssm/icssm_prueth.o icssm/icssm_prueth_switch.o icssm/icssm_switchdev.o

<<<<<<< HEAD
ti-cpsw-common-y += cpsw-common.o davinci_cpdma.o
ti-cpsw-priv-y += cpsw_priv.o cpsw_ethtool.o
ti-cpsw-ale-y += cpsw_ale.o
ti-cpsw-sl-y += cpsw_sl.o

obj-$(CONFIG_TLAN) += tlan.o
obj-$(CONFIG_TI_DAVINCI_EMAC) += davinci_emac.o ti-cpsw-common.o
obj-$(CONFIG_TI_DAVINCI_MDIO) += davinci_mdio.o
obj-$(CONFIG_TI_CPSW_PHY_SEL) += cpsw-phy-sel.o
obj-$(CONFIG_TI_CPTS) += cpts.o
obj-$(CONFIG_TI_CPSW) += ti_cpsw.o ti-cpsw-common.o ti-cpsw-priv.o ti-cpsw-ale.o ti-cpsw-sl.o
ti_cpsw-y := cpsw.o
obj-$(CONFIG_TI_CPSW_SWITCHDEV) += ti_cpsw_new.o ti-cpsw-common.o ti-cpsw-priv.o ti-cpsw-ale.o ti-cpsw-sl.o
ti_cpsw_new-y := cpsw_switchdev.o cpsw_new.o

obj-$(CONFIG_TI_KEYSTONE_NETCP) += keystone_netcp.o ti-cpsw-ale.o
keystone_netcp-y := netcp_core.o
obj-$(CONFIG_TI_KEYSTONE_NETCP_ETHSS) += keystone_netcp_ethss.o ti-cpsw-ale.o
keystone_netcp_ethss-y := netcp_ethss.o netcp_sgmii.o netcp_xgbepcsr.o

obj-$(CONFIG_TI_K3_CPPI_DESC_POOL) += k3-cppi-desc-pool.o

obj-$(CONFIG_TI_K3_AM65_CPSW_NUSS) += ti-am65-cpsw-nuss.o ti-cpsw-sl.o ti-cpsw-ale.o
ti-am65-cpsw-nuss-y := am65-cpsw-nuss.o am65-cpsw-ethtool.o
=======
obj-$(CONFIG_TI_CPSW) += cpsw-common.o
obj-$(CONFIG_TI_DAVINCI_EMAC) += cpsw-common.o
obj-$(CONFIG_TI_CPSW_SWITCHDEV) += cpsw-common.o

obj-$(CONFIG_TLAN) += tlan.o
obj-$(CONFIG_TI_DAVINCI_EMAC) += ti_davinci_emac.o
ti_davinci_emac-y := davinci_emac.o davinci_cpdma.o
obj-$(CONFIG_TI_DAVINCI_MDIO) += davinci_mdio.o
obj-$(CONFIG_TI_CPSW_PHY_SEL) += cpsw-phy-sel.o
obj-$(CONFIG_TI_CPTS) += cpts.o
obj-$(CONFIG_TI_CPSW) += ti_cpsw.o
ti_cpsw-y := cpsw.o davinci_cpdma.o cpsw_ale.o cpsw_priv.o cpsw_sl.o cpsw_ethtool.o
obj-$(CONFIG_TI_CPSW_SWITCHDEV) += ti_cpsw_new.o
ti_cpsw_new-y := cpsw_switchdev.o cpsw_new.o davinci_cpdma.o cpsw_ale.o cpsw_sl.o cpsw_priv.o cpsw_ethtool.o

obj-$(CONFIG_TI_KEYSTONE_NETCP) += keystone_netcp.o
keystone_netcp-y := netcp_core.o cpsw_ale.o
obj-$(CONFIG_TI_KEYSTONE_NETCP_ETHSS) += keystone_netcp_ethss.o
keystone_netcp_ethss-y := netcp_ethss.o netcp_sgmii.o netcp_xgbepcsr.o cpsw_ale.o

obj-$(CONFIG_TI_K3_CPPI_DESC_POOL) += k3-cppi-desc-pool.o

obj-$(CONFIG_TI_K3_AM65_CPSW_NUSS) += ti-am65-cpsw-nuss.o
ti-am65-cpsw-nuss-y := am65-cpsw-nuss.o cpsw_sl.o am65-cpsw-ethtool.o cpsw_ale.o
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
ti-am65-cpsw-nuss-$(CONFIG_TI_AM65_CPSW_QOS) += am65-cpsw-qos.o
ti-am65-cpsw-nuss-$(CONFIG_TI_K3_AM65_CPSW_SWITCHDEV) += am65-cpsw-switchdev.o
obj-$(CONFIG_TI_K3_AM65_CPTS) += am65-cpts.o

obj-$(CONFIG_TI_ICSSG_PRUETH) += icssg-prueth.o icssg.o
icssg-prueth-y := icssg/icssg_prueth.o icssg/icssg_switchdev.o

obj-$(CONFIG_TI_ICSSG_PRUETH_SR1) += icssg-prueth-sr1.o icssg.o
icssg-prueth-sr1-y := icssg/icssg_prueth_sr1.o

icssg-y := icssg/icssg_common.o \
	   icssg/icssg_classifier.o \
	   icssg/icssg_queues.o \
	   icssg/icssg_config.o \
	   icssg/icssg_mii_cfg.o \
	   icssg/icssg_stats.o \
	   icssg/icssg_ethtool.o

obj-$(CONFIG_TI_ICSS_IEP) += icssg/icss_iep.o
