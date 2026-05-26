// SPDX-License-Identifier: GPL-2.0
/*
 * Renesas RZ/G2L IRQC Driver
 *
 * Copyright (C) 2022 Renesas Electronics Corporation.
 *
 * Author: Lad Prabhakar <prabhakar.mahadev-lad.rj@bp.renesas.com>
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/irqchip.h>
#include <linux/irqdomain.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/reset.h>
#include <linux/spinlock.h>
#include <linux/syscore_ops.h>

<<<<<<< HEAD
#define IRQC_NMI			0
#define IRQC_IRQ_START			1
#define IRQC_TINT_COUNT			32
#define IRQC_SHARED_IRQ_COUNT		8
#define IRQC_IRQ_SHARED_START		(IRQC_IRQ_START + IRQC_SHARED_IRQ_COUNT)

#define NSCR				0x0
#define NITSR				0x4
=======
#define IRQC_IRQ_START			1
#define IRQC_IRQ_COUNT			8
#define IRQC_TINT_START			(IRQC_IRQ_START + IRQC_IRQ_COUNT)
#define IRQC_TINT_COUNT			32
#define IRQC_NUM_IRQ			(IRQC_TINT_START + IRQC_TINT_COUNT)

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#define ISCR				0x10
#define IITSR				0x14
#define TSCR				0x20
#define TITSR(n)			(0x24 + (n) * 4)
#define TITSR0_MAX_INT			16
#define TITSEL_WIDTH			0x2
<<<<<<< HEAD
#define INTTSEL				0x2c
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#define TSSR(n)				(0x30 + ((n) * 4))
#define TIEN				BIT(7)
#define TSSEL_SHIFT(n)			(8 * (n))
#define TSSEL_MASK			GENMASK(7, 0)
#define IRQ_MASK			0x3
#define IMSK				0x10010
#define TMSK				0x10020

#define TSSR_OFFSET(n)			((n) % 4)
#define TSSR_INDEX(n)			((n) / 4)

<<<<<<< HEAD
#define NSCR_NSTAT			0
#define NITSR_NTSEL_EDGE_FALLING	0
#define NITSR_NTSEL_EDGE_RISING		1

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#define TITSR_TITSEL_EDGE_RISING	0
#define TITSR_TITSEL_EDGE_FALLING	1
#define TITSR_TITSEL_LEVEL_HIGH		2
#define TITSR_TITSEL_LEVEL_LOW		3

#define IITSR_IITSEL(n, sense)		((sense) << ((n) * 2))
#define IITSR_IITSEL_LEVEL_LOW		0
#define IITSR_IITSEL_EDGE_FALLING	1
#define IITSR_IITSEL_EDGE_RISING	2
#define IITSR_IITSEL_EDGE_BOTH		3
#define IITSR_IITSEL_MASK(n)		IITSR_IITSEL((n), 3)

<<<<<<< HEAD
#define INTTSEL_TINTSEL(n)		BIT(n)
#define INTTSEL_TINTSEL_START		24

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#define TINT_EXTRACT_HWIRQ(x)		FIELD_GET(GENMASK(15, 0), (x))
#define TINT_EXTRACT_GPIOINT(x)		FIELD_GET(GENMASK(31, 16), (x))

/**
 * struct rzg2l_irqc_reg_cache - registers cache (necessary for suspend/resume)
<<<<<<< HEAD
 * @nitsr:	NITSR register
 * @iitsr:	IITSR register
 * @inttsel:	INTTSEL register
 * @titsr:	TITSR registers
 */
struct rzg2l_irqc_reg_cache {
	u32	nitsr;
	u32	iitsr;
	u32	inttsel;
=======
 * @iitsr: IITSR register
 * @titsr: TITSR registers
 */
struct rzg2l_irqc_reg_cache {
	u32	iitsr;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	u32	titsr[2];
};

/**
<<<<<<< HEAD
 * struct rzg2l_hw_info - Interrupt Control Unit controller hardware info structure.
 * @tssel_lut:		TINT lookup table
 * @irq_count:		Number of IRQC interrupts
 * @tint_start:		Start of TINT interrupts
 * @num_irq:		Total Number of interrupts
 * @shared_irq_cnt:	Number of shared interrupts
 */
struct rzg2l_hw_info {
	const u8	*tssel_lut;
	unsigned int	irq_count;
	unsigned int	tint_start;
	unsigned int	num_irq;
	unsigned int	shared_irq_cnt;
};

/**
 * struct rzg2l_irqc_priv - IRQ controller private data structure
 * @base:	Controller's base address
 * @irq_chip:	Pointer to struct irq_chip for irq
 * @tint_chip:	Pointer to struct irq_chip for tint
 * @fwspec:	IRQ firmware specific data
 * @lock:	Lock to serialize access to hardware registers
 * @info:	Hardware specific data
 * @cache:	Registers cache for suspend/resume
 * @used_irqs:	Bitmap to manage the shared interrupts
 */
static struct rzg2l_irqc_priv {
	void __iomem			*base;
	const struct irq_chip		*irq_chip;
	const struct irq_chip		*tint_chip;
	struct irq_fwspec		*fwspec;
	raw_spinlock_t			lock;
	struct rzg2l_hw_info		info;
	struct rzg2l_irqc_reg_cache	cache;
	DECLARE_BITMAP(used_irqs, IRQC_SHARED_IRQ_COUNT);
=======
 * struct rzg2l_irqc_priv - IRQ controller private data structure
 * @base:	Controller's base address
 * @irqchip:	Pointer to struct irq_chip
 * @fwspec:	IRQ firmware specific data
 * @lock:	Lock to serialize access to hardware registers
 * @cache:	Registers cache for suspend/resume
 */
static struct rzg2l_irqc_priv {
	void __iomem			*base;
	const struct irq_chip		*irqchip;
	struct irq_fwspec		fwspec[IRQC_NUM_IRQ];
	raw_spinlock_t			lock;
	struct rzg2l_irqc_reg_cache	cache;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
} *rzg2l_irqc_data;

static struct rzg2l_irqc_priv *irq_data_to_priv(struct irq_data *data)
{
	return data->domain->host_data;
}

<<<<<<< HEAD
static void rzg2l_clear_nmi_int(struct rzg2l_irqc_priv *priv)
{
	u32 bit = BIT(NSCR_NSTAT);
	u32 reg;

	/*
	 * No locking required as the register is not shared
	 * with other interrupts.
	 *
	 * Writing is allowed only when NSTAT is 1
	 */
	reg = readl_relaxed(priv->base + NSCR);
	if (reg & bit) {
		writel_relaxed(reg & ~bit, priv->base + NSCR);
		/*
		 * Enforce that the posted write is flushed to prevent that the
		 * just handled interrupt is raised again.
		 */
		readl_relaxed(priv->base + NSCR);
	}
}

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static void rzg2l_clear_irq_int(struct rzg2l_irqc_priv *priv, unsigned int hwirq)
{
	unsigned int hw_irq = hwirq - IRQC_IRQ_START;
	u32 bit = BIT(hw_irq);
	u32 iitsr, iscr;

	iscr = readl_relaxed(priv->base + ISCR);
	iitsr = readl_relaxed(priv->base + IITSR);

	/*
	 * ISCR can only be cleared if the type is falling-edge, rising-edge or
	 * falling/rising-edge.
	 */
	if ((iscr & bit) && (iitsr & IITSR_IITSEL_MASK(hw_irq))) {
		writel_relaxed(iscr & ~bit, priv->base + ISCR);
		/*
		 * Enforce that the posted write is flushed to prevent that the
		 * just handled interrupt is raised again.
		 */
		readl_relaxed(priv->base + ISCR);
	}
}

static void rzg2l_clear_tint_int(struct rzg2l_irqc_priv *priv, unsigned int hwirq)
{
<<<<<<< HEAD
	u32 bit = BIT(hwirq - priv->info.tint_start);
=======
	u32 bit = BIT(hwirq - IRQC_TINT_START);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	u32 reg;

	reg = readl_relaxed(priv->base + TSCR);
	if (reg & bit) {
		writel_relaxed(reg & ~bit, priv->base + TSCR);
		/*
		 * Enforce that the posted write is flushed to prevent that the
		 * just handled interrupt is raised again.
		 */
		readl_relaxed(priv->base + TSCR);
	}
}

<<<<<<< HEAD
static void rzg2l_irqc_nmi_eoi(struct irq_data *d)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);

	rzg2l_clear_nmi_int(priv);
	irq_chip_eoi_parent(d);
}

static void rzg2l_irqc_irq_eoi(struct irq_data *d)
=======
static void rzg2l_irqc_eoi(struct irq_data *d)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hw_irq = irqd_to_hwirq(d);

<<<<<<< HEAD
	scoped_guard(raw_spinlock, &priv->lock)
		rzg2l_clear_irq_int(priv, hw_irq);

	irq_chip_eoi_parent(d);
}

static void rzg2l_irqc_tint_eoi(struct irq_data *d)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hw_irq = irqd_to_hwirq(d);

	scoped_guard(raw_spinlock, &priv->lock)
		rzg2l_clear_tint_int(priv, hw_irq);

=======
	raw_spin_lock(&priv->lock);
	if (hw_irq >= IRQC_IRQ_START && hw_irq <= IRQC_IRQ_COUNT)
		rzg2l_clear_irq_int(priv, hw_irq);
	else if (hw_irq >= IRQC_TINT_START && hw_irq < IRQC_NUM_IRQ)
		rzg2l_clear_tint_int(priv, hw_irq);
	raw_spin_unlock(&priv->lock);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	irq_chip_eoi_parent(d);
}

static void rzfive_irqc_mask_irq_interrupt(struct rzg2l_irqc_priv *priv,
					   unsigned int hwirq)
{
	u32 bit = BIT(hwirq - IRQC_IRQ_START);

	writel_relaxed(readl_relaxed(priv->base + IMSK) | bit, priv->base + IMSK);
}

static void rzfive_irqc_unmask_irq_interrupt(struct rzg2l_irqc_priv *priv,
					     unsigned int hwirq)
{
	u32 bit = BIT(hwirq - IRQC_IRQ_START);

	writel_relaxed(readl_relaxed(priv->base + IMSK) & ~bit, priv->base + IMSK);
}

static void rzfive_irqc_mask_tint_interrupt(struct rzg2l_irqc_priv *priv,
					    unsigned int hwirq)
{
<<<<<<< HEAD
	u32 bit = BIT(hwirq - priv->info.tint_start);
=======
	u32 bit = BIT(hwirq - IRQC_TINT_START);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	writel_relaxed(readl_relaxed(priv->base + TMSK) | bit, priv->base + TMSK);
}

static void rzfive_irqc_unmask_tint_interrupt(struct rzg2l_irqc_priv *priv,
					      unsigned int hwirq)
{
<<<<<<< HEAD
	u32 bit = BIT(hwirq - priv->info.tint_start);
=======
	u32 bit = BIT(hwirq - IRQC_TINT_START);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	writel_relaxed(readl_relaxed(priv->base + TMSK) & ~bit, priv->base + TMSK);
}

<<<<<<< HEAD
static void rzfive_irqc_irq_mask(struct irq_data *d)
=======
static void rzfive_irqc_mask(struct irq_data *d)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);

<<<<<<< HEAD
	scoped_guard(raw_spinlock, &priv->lock)
		rzfive_irqc_mask_irq_interrupt(priv, hwirq);

	irq_chip_mask_parent(d);
}

static void rzfive_irqc_tint_mask(struct irq_data *d)
=======
	raw_spin_lock(&priv->lock);
	if (hwirq >= IRQC_IRQ_START && hwirq <= IRQC_IRQ_COUNT)
		rzfive_irqc_mask_irq_interrupt(priv, hwirq);
	else if (hwirq >= IRQC_TINT_START && hwirq < IRQC_NUM_IRQ)
		rzfive_irqc_mask_tint_interrupt(priv, hwirq);
	raw_spin_unlock(&priv->lock);
	irq_chip_mask_parent(d);
}

static void rzfive_irqc_unmask(struct irq_data *d)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);

<<<<<<< HEAD
	scoped_guard(raw_spinlock, &priv->lock)
		rzfive_irqc_mask_tint_interrupt(priv, hwirq);

	irq_chip_mask_parent(d);
}

static void rzfive_irqc_irq_unmask(struct irq_data *d)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);

	scoped_guard(raw_spinlock, &priv->lock)
		rzfive_irqc_unmask_irq_interrupt(priv, hwirq);

	irq_chip_unmask_parent(d);
}

static void rzfive_irqc_tint_unmask(struct irq_data *d)
=======
	raw_spin_lock(&priv->lock);
	if (hwirq >= IRQC_IRQ_START && hwirq <= IRQC_IRQ_COUNT)
		rzfive_irqc_unmask_irq_interrupt(priv, hwirq);
	else if (hwirq >= IRQC_TINT_START && hwirq < IRQC_NUM_IRQ)
		rzfive_irqc_unmask_tint_interrupt(priv, hwirq);
	raw_spin_unlock(&priv->lock);
	irq_chip_unmask_parent(d);
}

static void rzfive_tint_irq_endisable(struct irq_data *d, bool enable)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);

<<<<<<< HEAD
	scoped_guard(raw_spinlock, &priv->lock)
		rzfive_irqc_unmask_tint_interrupt(priv, hwirq);

	irq_chip_unmask_parent(d);
}

static void rzfive_irq_endisable(struct irq_data *d, bool enable)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);

	guard(raw_spinlock)(&priv->lock);
	if (enable)
		rzfive_irqc_unmask_irq_interrupt(priv, hwirq);
	else
		rzfive_irqc_mask_irq_interrupt(priv, hwirq);
}

static void rzfive_tint_endisable(struct irq_data *d, bool enable)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);
	unsigned int offset = hwirq - priv->info.tint_start;
	unsigned int tssr_offset = TSSR_OFFSET(offset);
	unsigned int tssr_index = TSSR_INDEX(offset);
	u32 reg;

	guard(raw_spinlock)(&priv->lock);
	if (enable)
		rzfive_irqc_unmask_tint_interrupt(priv, hwirq);
	else
		rzfive_irqc_mask_tint_interrupt(priv, hwirq);
	reg = readl_relaxed(priv->base + TSSR(tssr_index));
	if (enable)
		reg |= TIEN << TSSEL_SHIFT(tssr_offset);
	else
		reg &= ~(TIEN << TSSEL_SHIFT(tssr_offset));
	writel_relaxed(reg, priv->base + TSSR(tssr_index));
=======
	if (hwirq >= IRQC_TINT_START && hwirq < IRQC_NUM_IRQ) {
		u32 offset = hwirq - IRQC_TINT_START;
		u32 tssr_offset = TSSR_OFFSET(offset);
		u8 tssr_index = TSSR_INDEX(offset);
		u32 reg;

		raw_spin_lock(&priv->lock);
		if (enable)
			rzfive_irqc_unmask_tint_interrupt(priv, hwirq);
		else
			rzfive_irqc_mask_tint_interrupt(priv, hwirq);
		reg = readl_relaxed(priv->base + TSSR(tssr_index));
		if (enable)
			reg |= TIEN << TSSEL_SHIFT(tssr_offset);
		else
			reg &= ~(TIEN << TSSEL_SHIFT(tssr_offset));
		writel_relaxed(reg, priv->base + TSSR(tssr_index));
		raw_spin_unlock(&priv->lock);
	} else {
		raw_spin_lock(&priv->lock);
		if (enable)
			rzfive_irqc_unmask_irq_interrupt(priv, hwirq);
		else
			rzfive_irqc_mask_irq_interrupt(priv, hwirq);
		raw_spin_unlock(&priv->lock);
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void rzfive_irqc_irq_disable(struct irq_data *d)
{
	irq_chip_disable_parent(d);
<<<<<<< HEAD
	rzfive_irq_endisable(d, false);
=======
	rzfive_tint_irq_endisable(d, false);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static void rzfive_irqc_irq_enable(struct irq_data *d)
{
<<<<<<< HEAD
	rzfive_irq_endisable(d, true);
	irq_chip_enable_parent(d);
}

static void rzfive_irqc_tint_disable(struct irq_data *d)
{
	irq_chip_disable_parent(d);
	rzfive_tint_endisable(d, false);
}

static void rzfive_irqc_tint_enable(struct irq_data *d)
{
	rzfive_tint_endisable(d, true);
=======
	rzfive_tint_irq_endisable(d, true);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	irq_chip_enable_parent(d);
}

static void rzg2l_tint_irq_endisable(struct irq_data *d, bool enable)
{
<<<<<<< HEAD
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hw_irq = irqd_to_hwirq(d);
	unsigned int offset = hw_irq - priv->info.tint_start;
	unsigned int tssr_offset = TSSR_OFFSET(offset);
	unsigned int tssr_index = TSSR_INDEX(offset);
	u32 reg;

	guard(raw_spinlock)(&priv->lock);
	reg = readl_relaxed(priv->base + TSSR(tssr_index));
	if (enable)
		reg |= TIEN << TSSEL_SHIFT(tssr_offset);
	else
		reg &= ~(TIEN << TSSEL_SHIFT(tssr_offset));
	writel_relaxed(reg, priv->base + TSSR(tssr_index));
}

static void rzg2l_irqc_tint_disable(struct irq_data *d)
=======
	unsigned int hw_irq = irqd_to_hwirq(d);

	if (hw_irq >= IRQC_TINT_START && hw_irq < IRQC_NUM_IRQ) {
		struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
		u32 offset = hw_irq - IRQC_TINT_START;
		u32 tssr_offset = TSSR_OFFSET(offset);
		u8 tssr_index = TSSR_INDEX(offset);
		u32 reg;

		raw_spin_lock(&priv->lock);
		reg = readl_relaxed(priv->base + TSSR(tssr_index));
		if (enable)
			reg |= TIEN << TSSEL_SHIFT(tssr_offset);
		else
			reg &= ~(TIEN << TSSEL_SHIFT(tssr_offset));
		writel_relaxed(reg, priv->base + TSSR(tssr_index));
		raw_spin_unlock(&priv->lock);
	}
}

static void rzg2l_irqc_irq_disable(struct irq_data *d)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	irq_chip_disable_parent(d);
	rzg2l_tint_irq_endisable(d, false);
}

<<<<<<< HEAD
static void rzg2l_irqc_tint_enable(struct irq_data *d)
=======
static void rzg2l_irqc_irq_enable(struct irq_data *d)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	rzg2l_tint_irq_endisable(d, true);
	irq_chip_enable_parent(d);
}

<<<<<<< HEAD
static int rzg2l_nmi_set_type(struct irq_data *d, unsigned int type)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	u32 sense;

	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_EDGE_FALLING:
		sense = NITSR_NTSEL_EDGE_FALLING;
		break;
	case IRQ_TYPE_EDGE_RISING:
		sense = NITSR_NTSEL_EDGE_RISING;
		break;
	default:
		return -EINVAL;
	}

	writel_relaxed(sense, priv->base + NITSR);
	return 0;
}

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static int rzg2l_irq_set_type(struct irq_data *d, unsigned int type)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);
<<<<<<< HEAD
	unsigned int iitseln = hwirq - IRQC_IRQ_START;
	bool clear_irq_int = false;
	unsigned int sense, tmp;
=======
	u32 iitseln = hwirq - IRQC_IRQ_START;
	bool clear_irq_int = false;
	u16 sense, tmp;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_LEVEL_LOW:
		sense = IITSR_IITSEL_LEVEL_LOW;
		break;

	case IRQ_TYPE_EDGE_FALLING:
		sense = IITSR_IITSEL_EDGE_FALLING;
		clear_irq_int = true;
		break;

	case IRQ_TYPE_EDGE_RISING:
		sense = IITSR_IITSEL_EDGE_RISING;
		clear_irq_int = true;
		break;

	case IRQ_TYPE_EDGE_BOTH:
		sense = IITSR_IITSEL_EDGE_BOTH;
		clear_irq_int = true;
		break;

	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	guard(raw_spinlock)(&priv->lock);
=======
	raw_spin_lock(&priv->lock);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	tmp = readl_relaxed(priv->base + IITSR);
	tmp &= ~IITSR_IITSEL_MASK(iitseln);
	tmp |= IITSR_IITSEL(iitseln, sense);
	if (clear_irq_int)
		rzg2l_clear_irq_int(priv, hwirq);
	writel_relaxed(tmp, priv->base + IITSR);
<<<<<<< HEAD
=======
	raw_spin_unlock(&priv->lock);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	return 0;
}

static u32 rzg2l_disable_tint_and_set_tint_source(struct irq_data *d, struct rzg2l_irqc_priv *priv,
						  u32 reg, u32 tssr_offset, u8 tssr_index)
{
	u32 tint = (u32)(uintptr_t)irq_data_get_irq_chip_data(d);
	u32 tien = reg & (TIEN << TSSEL_SHIFT(tssr_offset));

<<<<<<< HEAD
	if (priv->info.tssel_lut)
		tint = priv->info.tssel_lut[tint];
	else
		tint = (u32)(uintptr_t)irq_data_get_irq_chip_data(d);

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	/* Clear the relevant byte in reg */
	reg &= ~(TSSEL_MASK << TSSEL_SHIFT(tssr_offset));
	/* Set TINT and leave TIEN clear */
	reg |= tint << TSSEL_SHIFT(tssr_offset);
	writel_relaxed(reg, priv->base + TSSR(tssr_index));

	return reg | tien;
}

static int rzg2l_tint_set_edge(struct irq_data *d, unsigned int type)
{
	struct rzg2l_irqc_priv *priv = irq_data_to_priv(d);
	unsigned int hwirq = irqd_to_hwirq(d);
<<<<<<< HEAD
	unsigned int titseln = hwirq - priv->info.tint_start;
	unsigned int tssr_offset = TSSR_OFFSET(titseln);
	unsigned int tssr_index = TSSR_INDEX(titseln);
	unsigned int index, sense;
=======
	u32 titseln = hwirq - IRQC_TINT_START;
	u32 tssr_offset = TSSR_OFFSET(titseln);
	u8 tssr_index = TSSR_INDEX(titseln);
	u8 index, sense;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	u32 reg, tssr;

	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_EDGE_RISING:
		sense = TITSR_TITSEL_EDGE_RISING;
		break;

	case IRQ_TYPE_EDGE_FALLING:
		sense = TITSR_TITSEL_EDGE_FALLING;
		break;

	default:
		return -EINVAL;
	}

	index = 0;
	if (titseln >= TITSR0_MAX_INT) {
		titseln -= TITSR0_MAX_INT;
		index = 1;
	}

	raw_spin_lock(&priv->lock);
	tssr = readl_relaxed(priv->base + TSSR(tssr_index));
	tssr = rzg2l_disable_tint_and_set_tint_source(d, priv, tssr, tssr_offset, tssr_index);
	reg = readl_relaxed(priv->base + TITSR(index));
	reg &= ~(IRQ_MASK << (titseln * TITSEL_WIDTH));
	reg |= sense << (titseln * TITSEL_WIDTH);
	writel_relaxed(reg, priv->base + TITSR(index));
	rzg2l_clear_tint_int(priv, hwirq);
	writel_relaxed(tssr, priv->base + TSSR(tssr_index));
	raw_spin_unlock(&priv->lock);

	return 0;
}

<<<<<<< HEAD
static int rzg2l_irqc_irq_set_type(struct irq_data *d, unsigned int type)
{
	int ret = rzg2l_irq_set_type(d, type);

	if (ret)
		return ret;

	return irq_chip_set_type_parent(d, IRQ_TYPE_LEVEL_HIGH);
}

static int rzg2l_irqc_tint_set_type(struct irq_data *d, unsigned int type)
{
	int ret = rzg2l_tint_set_edge(d, type);

	if (ret)
		return ret;

	return irq_chip_set_type_parent(d, IRQ_TYPE_LEVEL_HIGH);
}

static int rzg2l_irqc_nmi_set_type(struct irq_data *d, unsigned int type)
{
	int ret;

	ret = rzg2l_nmi_set_type(d, type);
=======
static int rzg2l_irqc_set_type(struct irq_data *d, unsigned int type)
{
	unsigned int hw_irq = irqd_to_hwirq(d);
	int ret = -EINVAL;

	if (hw_irq >= IRQC_IRQ_START && hw_irq <= IRQC_IRQ_COUNT)
		ret = rzg2l_irq_set_type(d, type);
	else if (hw_irq >= IRQC_TINT_START && hw_irq < IRQC_NUM_IRQ)
		ret = rzg2l_tint_set_edge(d, type);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (ret)
		return ret;

	return irq_chip_set_type_parent(d, IRQ_TYPE_LEVEL_HIGH);
}

static int rzg2l_irqc_irq_suspend(void *data)
{
	struct rzg2l_irqc_reg_cache *cache = &rzg2l_irqc_data->cache;
	void __iomem *base = rzg2l_irqc_data->base;

<<<<<<< HEAD
	cache->nitsr = readl_relaxed(base + NITSR);
	cache->iitsr = readl_relaxed(base + IITSR);
	if (rzg2l_irqc_data->info.shared_irq_cnt)
		cache->inttsel = readl_relaxed(base + INTTSEL);
=======
	cache->iitsr = readl_relaxed(base + IITSR);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	for (u8 i = 0; i < 2; i++)
		cache->titsr[i] = readl_relaxed(base + TITSR(i));

	return 0;
}

static void rzg2l_irqc_irq_resume(void *data)
{
	struct rzg2l_irqc_reg_cache *cache = &rzg2l_irqc_data->cache;
	void __iomem *base = rzg2l_irqc_data->base;

	/*
	 * Restore only interrupt type. TSSRx will be restored at the
	 * request of pin controller to avoid spurious interrupts due
	 * to invalid PIN states.
	 */
	for (u8 i = 0; i < 2; i++)
		writel_relaxed(cache->titsr[i], base + TITSR(i));
<<<<<<< HEAD
	if (rzg2l_irqc_data->info.shared_irq_cnt)
		writel_relaxed(cache->inttsel, base + INTTSEL);
	writel_relaxed(cache->iitsr, base + IITSR);
	writel_relaxed(cache->nitsr, base + NITSR);
=======
	writel_relaxed(cache->iitsr, base + IITSR);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static const struct syscore_ops rzg2l_irqc_syscore_ops = {
	.suspend	= rzg2l_irqc_irq_suspend,
	.resume		= rzg2l_irqc_irq_resume,
};

static struct syscore rzg2l_irqc_syscore = {
	.ops = &rzg2l_irqc_syscore_ops,
};

<<<<<<< HEAD
static const struct irq_chip rzg2l_irqc_nmi_chip = {
	.name			= "rzg2l-irqc",
	.irq_eoi		= rzg2l_irqc_nmi_eoi,
	.irq_mask		= irq_chip_mask_parent,
	.irq_unmask		= irq_chip_unmask_parent,
	.irq_disable		= irq_chip_disable_parent,
	.irq_enable		= irq_chip_enable_parent,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= rzg2l_irqc_nmi_set_type,
=======
static const struct irq_chip rzg2l_irqc_chip = {
	.name			= "rzg2l-irqc",
	.irq_eoi		= rzg2l_irqc_eoi,
	.irq_mask		= irq_chip_mask_parent,
	.irq_unmask		= irq_chip_unmask_parent,
	.irq_disable		= rzg2l_irqc_irq_disable,
	.irq_enable		= rzg2l_irqc_irq_enable,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= rzg2l_irqc_set_type,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.irq_set_affinity	= irq_chip_set_affinity_parent,
	.flags			= IRQCHIP_MASK_ON_SUSPEND |
				  IRQCHIP_SET_TYPE_MASKED |
				  IRQCHIP_SKIP_SET_WAKE,
};

<<<<<<< HEAD
static const struct irq_chip rzg2l_irqc_irq_chip = {
	.name			= "rzg2l-irqc",
	.irq_eoi		= rzg2l_irqc_irq_eoi,
	.irq_mask		= irq_chip_mask_parent,
	.irq_unmask		= irq_chip_unmask_parent,
	.irq_disable		= irq_chip_disable_parent,
	.irq_enable		= irq_chip_enable_parent,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= rzg2l_irqc_irq_set_type,
	.irq_set_affinity	= irq_chip_set_affinity_parent,
	.flags			= IRQCHIP_MASK_ON_SUSPEND |
				  IRQCHIP_SET_TYPE_MASKED |
				  IRQCHIP_SKIP_SET_WAKE,
};

static const struct irq_chip rzg2l_irqc_tint_chip = {
	.name			= "rzg2l-irqc",
	.irq_eoi		= rzg2l_irqc_tint_eoi,
	.irq_mask		= irq_chip_mask_parent,
	.irq_unmask		= irq_chip_unmask_parent,
	.irq_disable		= rzg2l_irqc_tint_disable,
	.irq_enable		= rzg2l_irqc_tint_enable,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= rzg2l_irqc_tint_set_type,
	.irq_set_affinity	= irq_chip_set_affinity_parent,
	.flags			= IRQCHIP_MASK_ON_SUSPEND |
				  IRQCHIP_SET_TYPE_MASKED |
				  IRQCHIP_SKIP_SET_WAKE,
};

static const struct irq_chip rzfive_irqc_irq_chip = {
	.name			= "rzfive-irqc",
	.irq_eoi		= rzg2l_irqc_irq_eoi,
	.irq_mask		= rzfive_irqc_irq_mask,
	.irq_unmask		= rzfive_irqc_irq_unmask,
=======
static const struct irq_chip rzfive_irqc_chip = {
	.name			= "rzfive-irqc",
	.irq_eoi		= rzg2l_irqc_eoi,
	.irq_mask		= rzfive_irqc_mask,
	.irq_unmask		= rzfive_irqc_unmask,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.irq_disable		= rzfive_irqc_irq_disable,
	.irq_enable		= rzfive_irqc_irq_enable,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
<<<<<<< HEAD
	.irq_set_type		= rzg2l_irqc_irq_set_type,
=======
	.irq_set_type		= rzg2l_irqc_set_type,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.irq_set_affinity	= irq_chip_set_affinity_parent,
	.flags			= IRQCHIP_MASK_ON_SUSPEND |
				  IRQCHIP_SET_TYPE_MASKED |
				  IRQCHIP_SKIP_SET_WAKE,
};

<<<<<<< HEAD
static const struct irq_chip rzfive_irqc_tint_chip = {
	.name			= "rzfive-irqc",
	.irq_eoi		= rzg2l_irqc_tint_eoi,
	.irq_mask		= rzfive_irqc_tint_mask,
	.irq_unmask		= rzfive_irqc_tint_unmask,
	.irq_disable		= rzfive_irqc_tint_disable,
	.irq_enable		= rzfive_irqc_tint_enable,
	.irq_get_irqchip_state	= irq_chip_get_parent_state,
	.irq_set_irqchip_state	= irq_chip_set_parent_state,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= rzg2l_irqc_tint_set_type,
	.irq_set_affinity	= irq_chip_set_affinity_parent,
	.flags			= IRQCHIP_MASK_ON_SUSPEND |
				  IRQCHIP_SET_TYPE_MASKED |
				  IRQCHIP_SKIP_SET_WAKE,
};

static bool rzg2l_irqc_is_shared_irqc(const struct rzg2l_hw_info info, unsigned int hw_irq)
{
	return ((hw_irq >= (info.tint_start - info.shared_irq_cnt)) && hw_irq < info.tint_start);
}

static bool rzg2l_irqc_is_shared_tint(const struct rzg2l_hw_info info, unsigned int hw_irq)
{
	return ((hw_irq >= (info.num_irq - info.shared_irq_cnt)) && hw_irq < info.num_irq);
}

static bool rzg2l_irqc_is_shared_and_get_irq_num(struct rzg2l_irqc_priv *priv,
						 irq_hw_number_t hwirq, unsigned int *irq_num)
{
	bool is_shared = false;

	if (rzg2l_irqc_is_shared_irqc(priv->info, hwirq)) {
		*irq_num = hwirq - IRQC_IRQ_SHARED_START;
		is_shared = true;
	} else if (rzg2l_irqc_is_shared_tint(priv->info, hwirq)) {
		*irq_num = hwirq - IRQC_TINT_COUNT - IRQC_IRQ_SHARED_START;
		is_shared = true;
	}

	return is_shared;
}

static void rzg2l_irqc_set_inttsel(struct rzg2l_irqc_priv *priv, unsigned int offset,
				   unsigned int select_irq)
{
	u32 reg;

	guard(raw_spinlock_irqsave)(&priv->lock);
	reg = readl_relaxed(priv->base + INTTSEL);
	if (select_irq)
		reg |= INTTSEL_TINTSEL(offset);
	else
		reg &= ~INTTSEL_TINTSEL(offset);
	writel_relaxed(reg, priv->base + INTTSEL);
}

static int rzg2l_irqc_shared_irq_alloc(struct rzg2l_irqc_priv *priv, irq_hw_number_t hwirq)
{
	unsigned int irq_num;

	if (rzg2l_irqc_is_shared_and_get_irq_num(priv, hwirq, &irq_num)) {
		if (test_and_set_bit(irq_num, priv->used_irqs))
			return -EBUSY;

		if (hwirq < priv->info.tint_start)
			rzg2l_irqc_set_inttsel(priv, INTTSEL_TINTSEL_START + irq_num, 1);
		else
			rzg2l_irqc_set_inttsel(priv, INTTSEL_TINTSEL_START + irq_num, 0);
	}

	return 0;
}

static void rzg2l_irqc_shared_irq_free(struct rzg2l_irqc_priv *priv, irq_hw_number_t hwirq)
{
	unsigned int irq_num;

	if (rzg2l_irqc_is_shared_and_get_irq_num(priv, hwirq, &irq_num) &&
	    test_and_clear_bit(irq_num, priv->used_irqs))
		rzg2l_irqc_set_inttsel(priv, INTTSEL_TINTSEL_START + irq_num, 0);
}

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static int rzg2l_irqc_alloc(struct irq_domain *domain, unsigned int virq,
			    unsigned int nr_irqs, void *arg)
{
	struct rzg2l_irqc_priv *priv = domain->host_data;
<<<<<<< HEAD
	const struct irq_chip *chip;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	unsigned long tint = 0;
	irq_hw_number_t hwirq;
	unsigned int type;
	int ret;

	ret = irq_domain_translate_twocell(domain, arg, &hwirq, &type);
	if (ret)
		return ret;

	/*
	 * For TINT interrupts ie where pinctrl driver is child of irqc domain
	 * the hwirq and TINT are encoded in fwspec->param[0].
	 * hwirq for TINT range from 9-40, hwirq is embedded 0-15 bits and TINT
	 * from 16-31 bits. TINT from the pinctrl driver needs to be programmed
	 * in IRQC registers to enable a given gpio pin as interrupt.
	 */
<<<<<<< HEAD
	if (hwirq == IRQC_NMI) {
		chip = &rzg2l_irqc_nmi_chip;
	} else if (hwirq > priv->info.irq_count) {
		tint = TINT_EXTRACT_GPIOINT(hwirq);
		hwirq = TINT_EXTRACT_HWIRQ(hwirq);
		chip = priv->tint_chip;
	} else {
		chip = priv->irq_chip;
	}

	if (hwirq >= priv->info.num_irq)
		return -EINVAL;

	if (priv->info.shared_irq_cnt) {
		ret = rzg2l_irqc_shared_irq_alloc(priv, hwirq);
		if (ret)
			return ret;
	}

	ret = irq_domain_set_hwirq_and_chip(domain, virq, hwirq, chip, (void *)(uintptr_t)tint);
	if (ret)
		goto shared_irq_free;

	ret = irq_domain_alloc_irqs_parent(domain, virq, nr_irqs, &priv->fwspec[hwirq]);
	if (ret)
		goto shared_irq_free;

	return 0;

shared_irq_free:
	if (priv->info.shared_irq_cnt)
		rzg2l_irqc_shared_irq_free(priv, hwirq);

	return ret;
}

static void rzg2l_irqc_free(struct irq_domain *domain, unsigned int virq, unsigned int nr_irqs)
{
	struct irq_data *d = irq_domain_get_irq_data(domain, virq);
	struct rzg2l_irqc_priv *priv = domain->host_data;
	irq_hw_number_t hwirq = irqd_to_hwirq(d);

	irq_domain_free_irqs_common(domain, virq, nr_irqs);

	if (priv->info.shared_irq_cnt)
		rzg2l_irqc_shared_irq_free(priv, hwirq);
=======
	if (hwirq > IRQC_IRQ_COUNT) {
		tint = TINT_EXTRACT_GPIOINT(hwirq);
		hwirq = TINT_EXTRACT_HWIRQ(hwirq);

		if (hwirq < IRQC_TINT_START)
			return -EINVAL;
	}

	if (hwirq > (IRQC_NUM_IRQ - 1))
		return -EINVAL;

	ret = irq_domain_set_hwirq_and_chip(domain, virq, hwirq, priv->irqchip,
					    (void *)(uintptr_t)tint);
	if (ret)
		return ret;

	return irq_domain_alloc_irqs_parent(domain, virq, nr_irqs, &priv->fwspec[hwirq]);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static const struct irq_domain_ops rzg2l_irqc_domain_ops = {
	.alloc = rzg2l_irqc_alloc,
<<<<<<< HEAD
	.free = rzg2l_irqc_free,
=======
	.free = irq_domain_free_irqs_common,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.translate = irq_domain_translate_twocell,
};

static int rzg2l_irqc_parse_interrupts(struct rzg2l_irqc_priv *priv,
				       struct device_node *np)
{
	struct of_phandle_args map;
	unsigned int i;
	int ret;

<<<<<<< HEAD
	for (i = 0; i < priv->info.num_irq; i++) {
=======
	for (i = 0; i < IRQC_NUM_IRQ; i++) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		ret = of_irq_parse_one(np, i, &map);
		if (ret)
			return ret;
		of_phandle_args_to_fwspec(np, map.args, map.args_count,
					  &priv->fwspec[i]);
	}

	return 0;
}

static int rzg2l_irqc_common_probe(struct platform_device *pdev, struct device_node *parent,
<<<<<<< HEAD
				   const struct irq_chip *irq_chip,
				   const struct irq_chip *tint_chip,
				   const struct rzg2l_hw_info info)
=======
				   const struct irq_chip *irq_chip)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
{
	struct irq_domain *irq_domain, *parent_domain;
	struct device_node *node = pdev->dev.of_node;
	struct device *dev = &pdev->dev;
	struct reset_control *resetn;
	int ret;

	parent_domain = irq_find_host(parent);
	if (!parent_domain)
		return dev_err_probe(dev, -ENODEV, "cannot find parent domain\n");

	rzg2l_irqc_data = devm_kzalloc(dev, sizeof(*rzg2l_irqc_data), GFP_KERNEL);
	if (!rzg2l_irqc_data)
		return -ENOMEM;

<<<<<<< HEAD
	rzg2l_irqc_data->irq_chip = irq_chip;
	rzg2l_irqc_data->tint_chip = tint_chip;
=======
	rzg2l_irqc_data->irqchip = irq_chip;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	rzg2l_irqc_data->base = devm_of_iomap(dev, dev->of_node, 0, NULL);
	if (IS_ERR(rzg2l_irqc_data->base))
		return PTR_ERR(rzg2l_irqc_data->base);

<<<<<<< HEAD
	rzg2l_irqc_data->info = info;

	rzg2l_irqc_data->fwspec = devm_kcalloc(&pdev->dev, info.num_irq,
					       sizeof(*rzg2l_irqc_data->fwspec), GFP_KERNEL);
	if (!rzg2l_irqc_data->fwspec)
		return -ENOMEM;

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	ret = rzg2l_irqc_parse_interrupts(rzg2l_irqc_data, node);
	if (ret)
		return dev_err_probe(dev, ret, "cannot parse interrupts: %d\n", ret);

	resetn = devm_reset_control_get_exclusive_deasserted(dev, NULL);
	if (IS_ERR(resetn)) {
		return dev_err_probe(dev, PTR_ERR(resetn),
				     "failed to acquire deasserted reset: %d\n", ret);
	}

	ret = devm_pm_runtime_enable(dev);
	if (ret)
		return dev_err_probe(dev, ret, "devm_pm_runtime_enable failed: %d\n", ret);

	ret = pm_runtime_resume_and_get(dev);
	if (ret)
		return dev_err_probe(dev, ret, "pm_runtime_resume_and_get failed: %d\n", ret);

	raw_spin_lock_init(&rzg2l_irqc_data->lock);

<<<<<<< HEAD
	irq_domain = irq_domain_create_hierarchy(parent_domain, 0, info.num_irq, dev_fwnode(dev),
						 &rzg2l_irqc_domain_ops, rzg2l_irqc_data);
	if (!irq_domain) {
		pm_runtime_put_sync(dev);
=======
	irq_domain = irq_domain_create_hierarchy(parent_domain, 0, IRQC_NUM_IRQ, dev_fwnode(dev),
						 &rzg2l_irqc_domain_ops, rzg2l_irqc_data);
	if (!irq_domain) {
		pm_runtime_put(dev);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		return -ENOMEM;
	}

	register_syscore(&rzg2l_irqc_syscore);

	return 0;
}

<<<<<<< HEAD
/* Mapping based on port index on Table 4.2-1 and GPIOINT on Table 4.6-7 */
static const u8 rzg3l_tssel_lut[] = {
	 83,  84,					/* P20-P21 */
	  7,   8,   9,  10,  11,  12,  13,		/* P30-P36 */
	 85,  86,  87,  88,  89,  90,  91,		/* P50-P56 */
	 92,  93,  94,  95,  96,  97,  98,		/* P60-P66 */
	 99, 100, 101, 102, 103, 104, 105, 106,		/* P70-P77 */
	107, 108, 109, 110, 111, 112,			/* P80-P85 */
	 45,  46,  47,  48,  49,  50,  51,  52,		/* PA0-PA7 */
	 53,  54,  55,  56,  57,  58,  59,  60,		/* PB0-PB7 */
	 61,  62,  63,					/* PC0-PC2 */
	 64,  65,  66,  67,  68,  69,  70,  71,		/* PD0-PD7 */
	 72,  73,  74,  75,  76,  77,  78,  79,		/* PE0-PE7 */
	 80,  81,  82,					/* PF0-PF2 */
	 27,  28,  29,  30,  31,  32,  33,  34,		/* PG0-PG7 */
	 35,  36,  37,  38,  39,  40,			/* PH0-PH5 */
	  2,   3,   4,   5,   6,			/* PJ0-PJ4 */
	 41,  42,  43,  44,				/* PK0-PK3 */
	 14,  15,  16,  17,  26,			/* PL0-PL4 */
	 18,  19,  20,  21,  22,  23,  24,  25,		/* PM0-PM7 */
	  0,   1					/* PS0-PS1 */
};

static const struct rzg2l_hw_info rzg3l_hw_params = {
	.tssel_lut	= rzg3l_tssel_lut,
	.irq_count	= 16,
	.tint_start	= IRQC_IRQ_START + 16,
	.num_irq	= IRQC_IRQ_START + 16 + IRQC_TINT_COUNT,
	.shared_irq_cnt	= IRQC_SHARED_IRQ_COUNT,
};

static const struct rzg2l_hw_info rzg2l_hw_params = {
	.irq_count	= 8,
	.tint_start	= IRQC_IRQ_START + 8,
	.num_irq	= IRQC_IRQ_START + 8 + IRQC_TINT_COUNT,
};

static int rzg2l_irqc_probe(struct platform_device *pdev, struct device_node *parent)
{
	return rzg2l_irqc_common_probe(pdev, parent, &rzg2l_irqc_irq_chip, &rzg2l_irqc_tint_chip,
				       rzg2l_hw_params);
}

static int rzg3l_irqc_probe(struct platform_device *pdev, struct device_node *parent)
{
	return rzg2l_irqc_common_probe(pdev, parent, &rzg2l_irqc_irq_chip, &rzg2l_irqc_tint_chip,
				       rzg3l_hw_params);
=======
static int rzg2l_irqc_probe(struct platform_device *pdev, struct device_node *parent)
{
	return rzg2l_irqc_common_probe(pdev, parent, &rzg2l_irqc_chip);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int rzfive_irqc_probe(struct platform_device *pdev, struct device_node *parent)
{
<<<<<<< HEAD
	return rzg2l_irqc_common_probe(pdev, parent, &rzfive_irqc_irq_chip, &rzfive_irqc_tint_chip,
				       rzg2l_hw_params);
=======
	return rzg2l_irqc_common_probe(pdev, parent, &rzfive_irqc_chip);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

IRQCHIP_PLATFORM_DRIVER_BEGIN(rzg2l_irqc)
IRQCHIP_MATCH("renesas,rzg2l-irqc", rzg2l_irqc_probe)
<<<<<<< HEAD
IRQCHIP_MATCH("renesas,r9a08g046-irqc", rzg3l_irqc_probe)
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
IRQCHIP_MATCH("renesas,r9a07g043f-irqc", rzfive_irqc_probe)
IRQCHIP_PLATFORM_DRIVER_END(rzg2l_irqc)
MODULE_AUTHOR("Lad Prabhakar <prabhakar.mahadev-lad.rj@bp.renesas.com>");
MODULE_DESCRIPTION("Renesas RZ/G2L IRQC Driver");
