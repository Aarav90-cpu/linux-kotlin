// SPDX-License-Identifier: GPL-2.0-only

#include <linux/bits.h>
#include <linux/bitfield.h>
#include <linux/bug.h>
#include <linux/container_of.h>
#include <linux/dev_printk.h>
#include <linux/dpll.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/math64.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/netlink.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/property.h>
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include <linux/slab.h>
#include <linux/sprintf.h>

#include "core.h"
#include "dpll.h"
#include "prop.h"
#include "regs.h"

#define ZL3073X_DPLL_REF_NONE		ZL3073X_NUM_REFS
#define ZL3073X_DPLL_REF_IS_VALID(_ref)	((_ref) != ZL3073X_DPLL_REF_NONE)

/**
 * struct zl3073x_dpll_pin - DPLL pin
 * @list: this DPLL pin list entry
 * @dpll: DPLL the pin is registered to
 * @dpll_pin: pointer to registered dpll_pin
 * @tracker: tracking object for the acquired reference
<<<<<<< HEAD
 * @fwnode: firmware node handle
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 * @label: package label
 * @dir: pin direction
 * @id: pin id
 * @prio: pin priority <0, 14>
<<<<<<< HEAD
=======
 * @selectable: pin is selectable in automatic mode
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 * @esync_control: embedded sync is controllable
 * @phase_gran: phase adjustment granularity
 * @pin_state: last saved pin state
 * @phase_offset: last saved pin phase offset
 * @freq_offset: last saved fractional frequency offset
<<<<<<< HEAD
 * @measured_freq: last saved measured frequency
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 */
struct zl3073x_dpll_pin {
	struct list_head	list;
	struct zl3073x_dpll	*dpll;
	struct dpll_pin		*dpll_pin;
	dpll_tracker		tracker;
<<<<<<< HEAD
	struct fwnode_handle	*fwnode;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	char			label[8];
	enum dpll_pin_direction	dir;
	u8			id;
	u8			prio;
<<<<<<< HEAD
=======
	bool			selectable;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	bool			esync_control;
	s32			phase_gran;
	enum dpll_pin_state	pin_state;
	s64			phase_offset;
	s64			freq_offset;
<<<<<<< HEAD
	u32			measured_freq;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};

/*
 * Supported esync ranges for input and for output per output pair type
 */
static const struct dpll_pin_frequency esync_freq_ranges[] = {
	DPLL_PIN_FREQUENCY_RANGE(0, 1),
};

/**
 * zl3073x_dpll_is_input_pin - check if the pin is input one
 * @pin: pin to check
 *
 * Return: true if pin is input, false if pin is output.
 */
static bool
zl3073x_dpll_is_input_pin(struct zl3073x_dpll_pin *pin)
{
	return pin->dir == DPLL_PIN_DIRECTION_INPUT;
}

/**
 * zl3073x_dpll_is_p_pin - check if the pin is P-pin
 * @pin: pin to check
 *
 * Return: true if the pin is P-pin, false if it is N-pin
 */
static bool
zl3073x_dpll_is_p_pin(struct zl3073x_dpll_pin *pin)
{
	return zl3073x_is_p_pin(pin->id);
}

static int
zl3073x_dpll_pin_direction_get(const struct dpll_pin *dpll_pin, void *pin_priv,
			       const struct dpll_device *dpll, void *dpll_priv,
			       enum dpll_pin_direction *direction,
			       struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *pin = pin_priv;

	*direction = pin->dir;

	return 0;
}

static struct zl3073x_dpll_pin *
zl3073x_dpll_pin_get_by_ref(struct zl3073x_dpll *zldpll, u8 ref_id)
{
	struct zl3073x_dpll_pin *pin;

	list_for_each_entry(pin, &zldpll->pins, list) {
		if (zl3073x_dpll_is_input_pin(pin) &&
		    zl3073x_input_pin_ref_get(pin->id) == ref_id)
			return pin;
	}

	return NULL;
}

static int
zl3073x_dpll_input_pin_esync_get(const struct dpll_pin *dpll_pin,
				 void *pin_priv,
				 const struct dpll_device *dpll,
				 void *dpll_priv,
				 struct dpll_pin_esync *esync,
				 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_ref *ref;
	u8 ref_id;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);

<<<<<<< HEAD
	if (!pin->esync_control || zl3073x_ref_freq_get(ref) <= 1)
		return -EOPNOTSUPP;

	esync->range = esync_freq_ranges;
	esync->range_num = ARRAY_SIZE(esync_freq_ranges);

	switch (zl3073x_ref_sync_mode_get(ref)) {
=======
	switch (FIELD_GET(ZL_REF_SYNC_CTRL_MODE, ref->sync_ctrl)) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	case ZL_REF_SYNC_CTRL_MODE_50_50_ESYNC_25_75:
		esync->freq = ref->esync_n_div == ZL_REF_ESYNC_DIV_1HZ ? 1 : 0;
		esync->pulse = 25;
		break;
	default:
		esync->freq = 0;
		esync->pulse = 0;
		break;
	}

<<<<<<< HEAD
=======
	/* If the pin supports esync control expose its range but only
	 * if the current reference frequency is > 1 Hz.
	 */
	if (pin->esync_control && zl3073x_ref_freq_get(ref) > 1) {
		esync->range = esync_freq_ranges;
		esync->range_num = ARRAY_SIZE(esync_freq_ranges);
	} else {
		esync->range = NULL;
		esync->range_num = 0;
	}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return 0;
}

static int
zl3073x_dpll_input_pin_esync_set(const struct dpll_pin *dpll_pin,
				 void *pin_priv,
				 const struct dpll_device *dpll,
				 void *dpll_priv, u64 freq,
				 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_ref ref;
	u8 ref_id, sync_mode;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = *zl3073x_ref_state_get(zldev, ref_id);

	/* Use freq == 0 to disable esync */
	if (!freq)
		sync_mode = ZL_REF_SYNC_CTRL_MODE_REFSYNC_PAIR_OFF;
	else
		sync_mode = ZL_REF_SYNC_CTRL_MODE_50_50_ESYNC_25_75;

<<<<<<< HEAD
	zl3073x_ref_sync_mode_set(&ref, sync_mode);
=======
	ref.sync_ctrl &= ~ZL_REF_SYNC_CTRL_MODE;
	ref.sync_ctrl |= FIELD_PREP(ZL_REF_SYNC_CTRL_MODE, sync_mode);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (freq) {
		/* 1 Hz is only supported frequency now */
		ref.esync_n_div = ZL_REF_ESYNC_DIV_1HZ;
	}

	/* Update reference configuration */
	return zl3073x_ref_state_set(zldev, ref_id, &ref);
}

static int
<<<<<<< HEAD
zl3073x_dpll_input_pin_ref_sync_get(const struct dpll_pin *dpll_pin,
				    void *pin_priv,
				    const struct dpll_pin *ref_sync_pin,
				    void *ref_sync_pin_priv,
				    enum dpll_pin_state *state,
				    struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *sync_pin = ref_sync_pin_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	const struct zl3073x_ref *ref;
	u8 ref_id, mode, pair;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);
	mode = zl3073x_ref_sync_mode_get(ref);
	pair = zl3073x_ref_sync_pair_get(ref);

	if (mode == ZL_REF_SYNC_CTRL_MODE_REFSYNC_PAIR &&
	    pair == zl3073x_input_pin_ref_get(sync_pin->id))
		*state = DPLL_PIN_STATE_CONNECTED;
	else
		*state = DPLL_PIN_STATE_DISCONNECTED;

	return 0;
}

static int
zl3073x_dpll_input_pin_ref_sync_set(const struct dpll_pin *dpll_pin,
				    void *pin_priv,
				    const struct dpll_pin *ref_sync_pin,
				    void *ref_sync_pin_priv,
				    const enum dpll_pin_state state,
				    struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *sync_pin = ref_sync_pin_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 mode, ref_id, sync_ref_id;
	struct zl3073x_chan chan;
	struct zl3073x_ref ref;
	int rc;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	sync_ref_id = zl3073x_input_pin_ref_get(sync_pin->id);
	ref = *zl3073x_ref_state_get(zldev, ref_id);

	if (state == DPLL_PIN_STATE_CONNECTED) {
		const struct zl3073x_ref *sync_ref;
		u32 ref_freq, sync_freq;

		sync_ref = zl3073x_ref_state_get(zldev, sync_ref_id);
		ref_freq = zl3073x_ref_freq_get(&ref);
		sync_freq = zl3073x_ref_freq_get(sync_ref);

		/* Sync signal must be 8 kHz or less and clock reference
		 * must be 1 kHz or more and higher than the sync signal.
		 */
		if (sync_freq > 8000) {
			NL_SET_ERR_MSG(extack,
				       "sync frequency must be 8 kHz or less");
			return -EINVAL;
		}
		if (ref_freq < 1000) {
			NL_SET_ERR_MSG(extack,
				       "clock frequency must be 1 kHz or more");
			return -EINVAL;
		}
		if (ref_freq <= sync_freq) {
			NL_SET_ERR_MSG(extack,
				       "clock frequency must be higher than sync frequency");
			return -EINVAL;
		}

		zl3073x_ref_sync_pair_set(&ref, sync_ref_id);
		mode = ZL_REF_SYNC_CTRL_MODE_REFSYNC_PAIR;
	} else {
		mode = ZL_REF_SYNC_CTRL_MODE_REFSYNC_PAIR_OFF;
	}

	zl3073x_ref_sync_mode_set(&ref, mode);

	rc = zl3073x_ref_state_set(zldev, ref_id, &ref);
	if (rc)
		return rc;

	/* Exclude sync source from automatic reference selection by setting
	 * its priority to NONE. On disconnect the priority is left as NONE
	 * and the user must explicitly make the pin selectable again.
	 */
	if (state == DPLL_PIN_STATE_CONNECTED) {
		chan = *zl3073x_chan_state_get(zldev, zldpll->id);
		zl3073x_chan_ref_prio_set(&chan, sync_ref_id,
					  ZL_DPLL_REF_PRIO_NONE);
		return zl3073x_chan_state_set(zldev, zldpll->id, &chan);
	}

	return 0;
}

static int
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
zl3073x_dpll_input_pin_ffo_get(const struct dpll_pin *dpll_pin, void *pin_priv,
			       const struct dpll_device *dpll, void *dpll_priv,
			       s64 *ffo, struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *pin = pin_priv;

	*ffo = pin->freq_offset;

	return 0;
}

static int
<<<<<<< HEAD
zl3073x_dpll_input_pin_measured_freq_get(const struct dpll_pin *dpll_pin,
					 void *pin_priv,
					 const struct dpll_device *dpll,
					 void *dpll_priv, u64 *measured_freq,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *pin = pin_priv;

	*measured_freq = pin->measured_freq;
	*measured_freq *= DPLL_PIN_MEASURED_FREQUENCY_DIVIDER;

	return 0;
}

static int
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
zl3073x_dpll_input_pin_frequency_get(const struct dpll_pin *dpll_pin,
				     void *pin_priv,
				     const struct dpll_device *dpll,
				     void *dpll_priv, u64 *frequency,
				     struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;
	u8 ref_id;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	*frequency = zl3073x_dev_ref_freq_get(zldpll->dev, ref_id);

	return 0;
}

static int
zl3073x_dpll_input_pin_frequency_set(const struct dpll_pin *dpll_pin,
				     void *pin_priv,
				     const struct dpll_device *dpll,
				     void *dpll_priv, u64 frequency,
				     struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_ref ref;
	u8 ref_id;

	/* Get reference state */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = *zl3073x_ref_state_get(zldev, ref_id);

	/* Update frequency */
	zl3073x_ref_freq_set(&ref, frequency);

	/* Commit reference state */
	return zl3073x_ref_state_set(zldev, ref_id, &ref);
}

/**
<<<<<<< HEAD
 * zl3073x_dpll_connected_ref_get - get currently connected reference
 * @zldpll: pointer to zl3073x_dpll
 *
 * Looks for currently connected reference the DPLL is locked to.
 *
 * Return: reference index if locked, ZL3073X_DPLL_REF_NONE otherwise
 */
static u8
zl3073x_dpll_connected_ref_get(struct zl3073x_dpll *zldpll)
{
	const struct zl3073x_chan *chan = zl3073x_chan_state_get(zldpll->dev,
								 zldpll->id);
	u8 state;

	/* A reference is connected only when the DPLL is locked to it */
	state = zl3073x_chan_refsel_state_get(chan);
	if (state == ZL_DPLL_REFSEL_STATUS_STATE_LOCK)
		return zl3073x_chan_refsel_ref_get(chan);

	return ZL3073X_DPLL_REF_NONE;
=======
 * zl3073x_dpll_selected_ref_get - get currently selected reference
 * @zldpll: pointer to zl3073x_dpll
 * @ref: place to store selected reference
 *
 * Check for currently selected reference the DPLL should be locked to
 * and stores its index to given @ref.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_selected_ref_get(struct zl3073x_dpll *zldpll, u8 *ref)
{
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 state, value;
	int rc;

	switch (zldpll->refsel_mode) {
	case ZL_DPLL_MODE_REFSEL_MODE_AUTO:
		/* For automatic mode read refsel_status register */
		rc = zl3073x_read_u8(zldev,
				     ZL_REG_DPLL_REFSEL_STATUS(zldpll->id),
				     &value);
		if (rc)
			return rc;

		/* Extract reference state */
		state = FIELD_GET(ZL_DPLL_REFSEL_STATUS_STATE, value);

		/* Return the reference only if the DPLL is locked to it */
		if (state == ZL_DPLL_REFSEL_STATUS_STATE_LOCK)
			*ref = FIELD_GET(ZL_DPLL_REFSEL_STATUS_REFSEL, value);
		else
			*ref = ZL3073X_DPLL_REF_NONE;
		break;
	case ZL_DPLL_MODE_REFSEL_MODE_REFLOCK:
		/* For manual mode return stored value */
		*ref = zldpll->forced_ref;
		break;
	default:
		/* For other modes like NCO, freerun... there is no input ref */
		*ref = ZL3073X_DPLL_REF_NONE;
		break;
	}

	return 0;
}

/**
 * zl3073x_dpll_selected_ref_set - select reference in manual mode
 * @zldpll: pointer to zl3073x_dpll
 * @ref: input reference to be selected
 *
 * Selects the given reference for the DPLL channel it should be
 * locked to.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_selected_ref_set(struct zl3073x_dpll *zldpll, u8 ref)
{
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 mode, mode_refsel;
	int rc;

	mode = zldpll->refsel_mode;

	switch (mode) {
	case ZL_DPLL_MODE_REFSEL_MODE_REFLOCK:
		/* Manual mode with ref selected */
		if (ref == ZL3073X_DPLL_REF_NONE) {
			switch (zldpll->lock_status) {
			case DPLL_LOCK_STATUS_LOCKED_HO_ACQ:
			case DPLL_LOCK_STATUS_HOLDOVER:
				/* Switch to forced holdover */
				mode = ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER;
				break;
			default:
				/* Switch to freerun */
				mode = ZL_DPLL_MODE_REFSEL_MODE_FREERUN;
				break;
			}
			/* Keep selected reference */
			ref = zldpll->forced_ref;
		} else if (ref == zldpll->forced_ref) {
			/* No register update - same mode and same ref */
			return 0;
		}
		break;
	case ZL_DPLL_MODE_REFSEL_MODE_FREERUN:
	case ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER:
		/* Manual mode without no ref */
		if (ref == ZL3073X_DPLL_REF_NONE)
			/* No register update - keep current mode */
			return 0;

		/* Switch to reflock mode and update ref selection */
		mode = ZL_DPLL_MODE_REFSEL_MODE_REFLOCK;
		break;
	default:
		/* For other modes like automatic or NCO ref cannot be selected
		 * manually
		 */
		return -EOPNOTSUPP;
	}

	/* Build mode_refsel value */
	mode_refsel = FIELD_PREP(ZL_DPLL_MODE_REFSEL_MODE, mode) |
		      FIELD_PREP(ZL_DPLL_MODE_REFSEL_REF, ref);

	/* Update dpll_mode_refsel register */
	rc = zl3073x_write_u8(zldev, ZL_REG_DPLL_MODE_REFSEL(zldpll->id),
			      mode_refsel);
	if (rc)
		return rc;

	/* Store new mode and forced reference */
	zldpll->refsel_mode = mode;
	zldpll->forced_ref = ref;

	return rc;
}

/**
 * zl3073x_dpll_connected_ref_get - get currently connected reference
 * @zldpll: pointer to zl3073x_dpll
 * @ref: place to store selected reference
 *
 * Looks for currently connected the DPLL is locked to and stores its index
 * to given @ref.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_connected_ref_get(struct zl3073x_dpll *zldpll, u8 *ref)
{
	struct zl3073x_dev *zldev = zldpll->dev;
	int rc;

	/* Get currently selected input reference */
	rc = zl3073x_dpll_selected_ref_get(zldpll, ref);
	if (rc)
		return rc;

	/* If the monitor indicates an error nothing is connected */
	if (ZL3073X_DPLL_REF_IS_VALID(*ref) &&
	    !zl3073x_dev_ref_is_status_ok(zldev, *ref))
		*ref = ZL3073X_DPLL_REF_NONE;

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int
zl3073x_dpll_input_pin_phase_offset_get(const struct dpll_pin *dpll_pin,
					void *pin_priv,
					const struct dpll_device *dpll,
					void *dpll_priv, s64 *phase_offset,
					struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_ref *ref;
	u8 conn_id, ref_id;
	s64 ref_phase;
<<<<<<< HEAD

	/* Get currently connected reference */
	conn_id = zl3073x_dpll_connected_ref_get(zldpll);
=======
	int rc;

	/* Get currently connected reference */
	rc = zl3073x_dpll_connected_ref_get(zldpll, &conn_id);
	if (rc)
		return rc;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* Report phase offset only for currently connected pin if the phase
	 * monitor feature is disabled and only if the input pin signal is
	 * present.
	 */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);
	if ((!zldpll->phase_monitor && ref_id != conn_id) ||
	    !zl3073x_ref_is_status_ok(ref)) {
		*phase_offset = 0;
		return 0;
	}

	ref_phase = pin->phase_offset;

	/* The DPLL being locked to a higher freq than the current ref
	 * the phase offset is modded to the period of the signal
	 * the dpll is locked to.
	 */
	if (ZL3073X_DPLL_REF_IS_VALID(conn_id) && conn_id != ref_id) {
		u32 conn_freq, ref_freq;

		/* Get frequency of connected and given ref */
		conn_freq = zl3073x_dev_ref_freq_get(zldev, conn_id);
		ref_freq = zl3073x_ref_freq_get(ref);

		if (conn_freq > ref_freq) {
			s64 conn_period, div_factor;

			conn_period = div_s64(PSEC_PER_SEC, conn_freq);
			div_factor = div64_s64(ref_phase, conn_period);
			ref_phase -= conn_period * div_factor;
		}
	}

	*phase_offset = ref_phase * DPLL_PHASE_OFFSET_DIVIDER;

<<<<<<< HEAD
	return 0;
=======
	return rc;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int
zl3073x_dpll_input_pin_phase_adjust_get(const struct dpll_pin *dpll_pin,
					void *pin_priv,
					const struct dpll_device *dpll,
					void *dpll_priv,
					s32 *phase_adjust,
					struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_ref *ref;
	s64 phase_comp;
	u8 ref_id;

	/* Read reference configuration */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);

	/* Perform sign extension based on register width */
	if (zl3073x_dev_is_ref_phase_comp_32bit(zldev))
		phase_comp = sign_extend64(ref->phase_comp, 31);
	else
		phase_comp = sign_extend64(ref->phase_comp, 47);

	/* Reverse two's complement negation applied during set and convert
	 * to 32bit signed int
	 */
	*phase_adjust = (s32)-phase_comp;

	return 0;
}

static int
zl3073x_dpll_input_pin_phase_adjust_set(const struct dpll_pin *dpll_pin,
					void *pin_priv,
					const struct dpll_device *dpll,
					void *dpll_priv,
					s32 phase_adjust,
					struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_ref ref;
	u8 ref_id;

	/* Read reference configuration */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = *zl3073x_ref_state_get(zldev, ref_id);

	/* The value in the register is stored as two's complement negation
	 * of requested value.
	 */
	ref.phase_comp = -phase_adjust;

	/* Update reference configuration */
	return zl3073x_ref_state_set(zldev, ref_id, &ref);
}

/**
<<<<<<< HEAD
=======
 * zl3073x_dpll_ref_prio_get - get priority for given input pin
 * @pin: pointer to pin
 * @prio: place to store priority
 *
 * Reads current priority for the given input pin and stores the value
 * to @prio.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_ref_prio_get(struct zl3073x_dpll_pin *pin, u8 *prio)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 ref, ref_prio;
	int rc;

	guard(mutex)(&zldev->multiop_lock);

	/* Read DPLL configuration */
	rc = zl3073x_mb_op(zldev, ZL_REG_DPLL_MB_SEM, ZL_DPLL_MB_SEM_RD,
			   ZL_REG_DPLL_MB_MASK, BIT(zldpll->id));
	if (rc)
		return rc;

	/* Read reference priority - one value for P&N pins (4 bits/pin) */
	ref = zl3073x_input_pin_ref_get(pin->id);
	rc = zl3073x_read_u8(zldev, ZL_REG_DPLL_REF_PRIO(ref / 2),
			     &ref_prio);
	if (rc)
		return rc;

	/* Select nibble according pin type */
	if (zl3073x_dpll_is_p_pin(pin))
		*prio = FIELD_GET(ZL_DPLL_REF_PRIO_REF_P, ref_prio);
	else
		*prio = FIELD_GET(ZL_DPLL_REF_PRIO_REF_N, ref_prio);

	return rc;
}

/**
 * zl3073x_dpll_ref_prio_set - set priority for given input pin
 * @pin: pointer to pin
 * @prio: place to store priority
 *
 * Sets priority for the given input pin.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_ref_prio_set(struct zl3073x_dpll_pin *pin, u8 prio)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 ref, ref_prio;
	int rc;

	guard(mutex)(&zldev->multiop_lock);

	/* Read DPLL configuration into mailbox */
	rc = zl3073x_mb_op(zldev, ZL_REG_DPLL_MB_SEM, ZL_DPLL_MB_SEM_RD,
			   ZL_REG_DPLL_MB_MASK, BIT(zldpll->id));
	if (rc)
		return rc;

	/* Read reference priority - one value shared between P&N pins */
	ref = zl3073x_input_pin_ref_get(pin->id);
	rc = zl3073x_read_u8(zldev, ZL_REG_DPLL_REF_PRIO(ref / 2), &ref_prio);
	if (rc)
		return rc;

	/* Update nibble according pin type */
	if (zl3073x_dpll_is_p_pin(pin)) {
		ref_prio &= ~ZL_DPLL_REF_PRIO_REF_P;
		ref_prio |= FIELD_PREP(ZL_DPLL_REF_PRIO_REF_P, prio);
	} else {
		ref_prio &= ~ZL_DPLL_REF_PRIO_REF_N;
		ref_prio |= FIELD_PREP(ZL_DPLL_REF_PRIO_REF_N, prio);
	}

	/* Update reference priority */
	rc = zl3073x_write_u8(zldev, ZL_REG_DPLL_REF_PRIO(ref / 2), ref_prio);
	if (rc)
		return rc;

	/* Commit configuration */
	return zl3073x_mb_op(zldev, ZL_REG_DPLL_MB_SEM, ZL_DPLL_MB_SEM_WR,
			     ZL_REG_DPLL_MB_MASK, BIT(zldpll->id));
}

/**
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 * zl3073x_dpll_ref_state_get - get status for given input pin
 * @pin: pointer to pin
 * @state: place to store status
 *
 * Checks current status for the given input pin and stores the value
 * to @state.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_ref_state_get(struct zl3073x_dpll_pin *pin,
			   enum dpll_pin_state *state)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;
	u8 ref;

	chan = zl3073x_chan_state_get(zldev, zldpll->id);
	ref = zl3073x_input_pin_ref_get(pin->id);

	/* Check if the pin reference is connected */
	if (ref == zl3073x_dpll_connected_ref_get(zldpll)) {
=======
	u8 ref, ref_conn;
	int rc;

	ref = zl3073x_input_pin_ref_get(pin->id);

	/* Get currently connected reference */
	rc = zl3073x_dpll_connected_ref_get(zldpll, &ref_conn);
	if (rc)
		return rc;

	if (ref == ref_conn) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		*state = DPLL_PIN_STATE_CONNECTED;
		return 0;
	}

	/* If the DPLL is running in automatic mode and the reference is
	 * selectable and its monitor does not report any error then report
	 * pin as selectable.
	 */
<<<<<<< HEAD
	if (zl3073x_chan_mode_get(chan) == ZL_DPLL_MODE_REFSEL_MODE_AUTO &&
	    zl3073x_dev_ref_is_status_ok(zldev, ref) &&
	    zl3073x_chan_ref_is_selectable(chan, ref)) {
=======
	if (zldpll->refsel_mode == ZL_DPLL_MODE_REFSEL_MODE_AUTO &&
	    zl3073x_dev_ref_is_status_ok(zldev, ref) && pin->selectable) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		*state = DPLL_PIN_STATE_SELECTABLE;
		return 0;
	}

	/* Otherwise report the pin as disconnected */
	*state = DPLL_PIN_STATE_DISCONNECTED;

	return 0;
}

static int
zl3073x_dpll_input_pin_state_on_dpll_get(const struct dpll_pin *dpll_pin,
					 void *pin_priv,
					 const struct dpll_device *dpll,
					 void *dpll_priv,
					 enum dpll_pin_state *state,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *pin = pin_priv;

	return zl3073x_dpll_ref_state_get(pin, state);
}

static int
zl3073x_dpll_input_pin_state_on_dpll_set(const struct dpll_pin *dpll_pin,
					 void *pin_priv,
					 const struct dpll_device *dpll,
					 void *dpll_priv,
					 enum dpll_pin_state state,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;
<<<<<<< HEAD
	struct zl3073x_chan chan;
	u8 mode, ref;
	int rc;

	chan = *zl3073x_chan_state_get(zldpll->dev, zldpll->id);
	ref = zl3073x_input_pin_ref_get(pin->id);
	mode = zl3073x_chan_mode_get(&chan);

	switch (mode) {
	case ZL_DPLL_MODE_REFSEL_MODE_REFLOCK:
		if (state == DPLL_PIN_STATE_CONNECTED) {
			/* Choose the pin as new selected reference */
			zl3073x_chan_ref_set(&chan, ref);
		} else if (state == DPLL_PIN_STATE_DISCONNECTED) {
			/* Choose new mode based on lock status */
			switch (zldpll->lock_status) {
			case DPLL_LOCK_STATUS_LOCKED_HO_ACQ:
			case DPLL_LOCK_STATUS_HOLDOVER:
				mode = ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER;
				break;
			default:
				mode = ZL_DPLL_MODE_REFSEL_MODE_FREERUN;
				break;
			}
			zl3073x_chan_mode_set(&chan, mode);
		} else {
			goto invalid_state;
		}
		break;
=======
	u8 new_ref;
	int rc;

	switch (zldpll->refsel_mode) {
	case ZL_DPLL_MODE_REFSEL_MODE_REFLOCK:
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	case ZL_DPLL_MODE_REFSEL_MODE_FREERUN:
	case ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER:
		if (state == DPLL_PIN_STATE_CONNECTED) {
			/* Choose the pin as new selected reference */
<<<<<<< HEAD
			zl3073x_chan_ref_set(&chan, ref);
			/* Switch to reflock mode */
			zl3073x_chan_mode_set(&chan,
					      ZL_DPLL_MODE_REFSEL_MODE_REFLOCK);
		} else if (state != DPLL_PIN_STATE_DISCONNECTED) {
			goto invalid_state;
		}
		break;
	case ZL_DPLL_MODE_REFSEL_MODE_AUTO:
		if (state == DPLL_PIN_STATE_SELECTABLE) {
			if (zl3073x_chan_ref_is_selectable(&chan, ref))
				return 0; /* Pin is already selectable */

			/* Restore pin priority in HW */
			zl3073x_chan_ref_prio_set(&chan, ref, pin->prio);
		} else if (state == DPLL_PIN_STATE_DISCONNECTED) {
			if (!zl3073x_chan_ref_is_selectable(&chan, ref))
				return 0; /* Pin is already disconnected */

			/* Set pin priority to none in HW */
			zl3073x_chan_ref_prio_set(&chan, ref,
						  ZL_DPLL_REF_PRIO_NONE);
		} else {
			goto invalid_state;
		}
		break;
=======
			new_ref = zl3073x_input_pin_ref_get(pin->id);
		} else if (state == DPLL_PIN_STATE_DISCONNECTED) {
			/* No reference */
			new_ref = ZL3073X_DPLL_REF_NONE;
		} else {
			NL_SET_ERR_MSG_MOD(extack,
					   "Invalid pin state for manual mode");
			return -EINVAL;
		}

		rc = zl3073x_dpll_selected_ref_set(zldpll, new_ref);
		break;

	case ZL_DPLL_MODE_REFSEL_MODE_AUTO:
		if (state == DPLL_PIN_STATE_SELECTABLE) {
			if (pin->selectable)
				return 0; /* Pin is already selectable */

			/* Restore pin priority in HW */
			rc = zl3073x_dpll_ref_prio_set(pin, pin->prio);
			if (rc)
				return rc;

			/* Mark pin as selectable */
			pin->selectable = true;
		} else if (state == DPLL_PIN_STATE_DISCONNECTED) {
			if (!pin->selectable)
				return 0; /* Pin is already disconnected */

			/* Set pin priority to none in HW */
			rc = zl3073x_dpll_ref_prio_set(pin,
						       ZL_DPLL_REF_PRIO_NONE);
			if (rc)
				return rc;

			/* Mark pin as non-selectable */
			pin->selectable = false;
		} else {
			NL_SET_ERR_MSG(extack,
				       "Invalid pin state for automatic mode");
			return -EINVAL;
		}
		break;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	default:
		/* In other modes we cannot change input reference */
		NL_SET_ERR_MSG(extack,
			       "Pin state cannot be changed in current mode");
<<<<<<< HEAD
		return -EOPNOTSUPP;
	}

	/* Commit DPLL channel changes */
	rc = zl3073x_chan_state_set(zldpll->dev, zldpll->id, &chan);
	if (rc)
		return rc;

	return 0;
invalid_state:
	NL_SET_ERR_MSG_MOD(extack, "Invalid pin state for this device mode");
	return -EINVAL;
=======
		rc = -EOPNOTSUPP;
		break;
	}

	return rc;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

static int
zl3073x_dpll_input_pin_prio_get(const struct dpll_pin *dpll_pin, void *pin_priv,
				const struct dpll_device *dpll, void *dpll_priv,
				u32 *prio, struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll_pin *pin = pin_priv;

	*prio = pin->prio;

	return 0;
}

static int
zl3073x_dpll_input_pin_prio_set(const struct dpll_pin *dpll_pin, void *pin_priv,
				const struct dpll_device *dpll, void *dpll_priv,
				u32 prio, struct netlink_ext_ack *extack)
{
<<<<<<< HEAD
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_chan chan;
	u8 ref;
=======
	struct zl3073x_dpll_pin *pin = pin_priv;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	int rc;

	if (prio > ZL_DPLL_REF_PRIO_MAX)
		return -EINVAL;

	/* If the pin is selectable then update HW registers */
<<<<<<< HEAD
	chan = *zl3073x_chan_state_get(zldpll->dev, zldpll->id);
	ref = zl3073x_input_pin_ref_get(pin->id);
	if (zl3073x_chan_ref_is_selectable(&chan, ref)) {
		zl3073x_chan_ref_prio_set(&chan, ref, prio);
		rc = zl3073x_chan_state_set(zldpll->dev, zldpll->id, &chan);
=======
	if (pin->selectable) {
		rc = zl3073x_dpll_ref_prio_set(pin, prio);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		if (rc)
			return rc;
	}

	/* Save priority */
	pin->prio = prio;

	return 0;
}

static int
zl3073x_dpll_output_pin_esync_get(const struct dpll_pin *dpll_pin,
				  void *pin_priv,
				  const struct dpll_device *dpll,
				  void *dpll_priv,
				  struct dpll_pin_esync *esync,
				  struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_synth *synth;
	const struct zl3073x_out *out;
<<<<<<< HEAD
	u32 synth_freq, out_freq;
	u8 out_id;
=======
	u8 clock_type, out_id;
	u32 synth_freq;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	out_id = zl3073x_output_pin_out_get(pin->id);
	out = zl3073x_out_state_get(zldev, out_id);

	/* If N-division is enabled, esync is not supported. The register used
	 * for N-division is also used for the esync divider so both cannot
	 * be used.
	 */
<<<<<<< HEAD
	if (zl3073x_out_is_ndiv(out))
		return -EOPNOTSUPP;
=======
	switch (zl3073x_out_signal_format_get(out)) {
	case ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV:
	case ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV_INV:
		return -EOPNOTSUPP;
	default:
		break;
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* Get attached synth frequency */
	synth = zl3073x_synth_state_get(zldev, zl3073x_out_synth_get(out));
	synth_freq = zl3073x_synth_freq_get(synth);
<<<<<<< HEAD
	out_freq = synth_freq / out->div;

	if (!pin->esync_control || out_freq <= 1)
		return -EOPNOTSUPP;

	esync->range = esync_freq_ranges;
	esync->range_num = ARRAY_SIZE(esync_freq_ranges);

	if (zl3073x_out_clock_type_get(out) != ZL_OUTPUT_MODE_CLOCK_TYPE_ESYNC) {
=======

	clock_type = FIELD_GET(ZL_OUTPUT_MODE_CLOCK_TYPE, out->mode);
	if (clock_type != ZL_OUTPUT_MODE_CLOCK_TYPE_ESYNC) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		/* No need to read esync data if it is not enabled */
		esync->freq = 0;
		esync->pulse = 0;

<<<<<<< HEAD
		return 0;
	}

	/* Compute esync frequency */
	esync->freq = out_freq / out->esync_n_period;
=======
		goto finish;
	}

	/* Compute esync frequency */
	esync->freq = synth_freq / out->div / out->esync_n_period;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* By comparing the esync_pulse_width to the half of the pulse width
	 * the esync pulse percentage can be determined.
	 * Note that half pulse width is in units of half synth cycles, which
	 * is why it reduces down to be output_div.
	 */
	esync->pulse = (50 * out->esync_n_width) / out->div;

<<<<<<< HEAD
=======
finish:
	/* Set supported esync ranges if the pin supports esync control and
	 * if the output frequency is > 1 Hz.
	 */
	if (pin->esync_control && (synth_freq / out->div) > 1) {
		esync->range = esync_freq_ranges;
		esync->range_num = ARRAY_SIZE(esync_freq_ranges);
	} else {
		esync->range = NULL;
		esync->range_num = 0;
	}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return 0;
}

static int
zl3073x_dpll_output_pin_esync_set(const struct dpll_pin *dpll_pin,
				  void *pin_priv,
				  const struct dpll_device *dpll,
				  void *dpll_priv, u64 freq,
				  struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_synth *synth;
	struct zl3073x_out out;
<<<<<<< HEAD
	u32 synth_freq;
	u8 out_id;
=======
	u8 clock_type, out_id;
	u32 synth_freq;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	out_id = zl3073x_output_pin_out_get(pin->id);
	out = *zl3073x_out_state_get(zldev, out_id);

	/* If N-division is enabled, esync is not supported. The register used
	 * for N-division is also used for the esync divider so both cannot
	 * be used.
	 */
<<<<<<< HEAD
	if (zl3073x_out_is_ndiv(&out))
		return -EOPNOTSUPP;

	/* Update clock type in output mode */
	if (freq)
		zl3073x_out_clock_type_set(&out,
					   ZL_OUTPUT_MODE_CLOCK_TYPE_ESYNC);
	else
		zl3073x_out_clock_type_set(&out,
					   ZL_OUTPUT_MODE_CLOCK_TYPE_NORMAL);
=======
	switch (zl3073x_out_signal_format_get(&out)) {
	case ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV:
	case ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV_INV:
		return -EOPNOTSUPP;
	default:
		break;
	}

	/* Select clock type */
	if (freq)
		clock_type = ZL_OUTPUT_MODE_CLOCK_TYPE_ESYNC;
	else
		clock_type = ZL_OUTPUT_MODE_CLOCK_TYPE_NORMAL;

	/* Update clock type in output mode */
	out.mode &= ~ZL_OUTPUT_MODE_CLOCK_TYPE;
	out.mode |= FIELD_PREP(ZL_OUTPUT_MODE_CLOCK_TYPE, clock_type);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* If esync is being disabled just write mailbox and finish */
	if (!freq)
		goto write_mailbox;

	/* Get attached synth frequency */
	synth = zl3073x_synth_state_get(zldev, zl3073x_out_synth_get(&out));
	synth_freq = zl3073x_synth_freq_get(synth);

	/* Compute and update esync period */
	out.esync_n_period = synth_freq / (u32)freq / out.div;

	/* Half of the period in units of 1/2 synth cycle can be represented by
	 * the output_div. To get the supported esync pulse width of 25% of the
	 * period the output_div can just be divided by 2. Note that this
	 * assumes that output_div is even, otherwise some resolution will be
	 * lost.
	 */
	out.esync_n_width = out.div / 2;

write_mailbox:
	/* Commit output configuration */
	return zl3073x_out_state_set(zldev, out_id, &out);
}

static int
zl3073x_dpll_output_pin_frequency_get(const struct dpll_pin *dpll_pin,
				      void *pin_priv,
				      const struct dpll_device *dpll,
				      void *dpll_priv, u64 *frequency,
				      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dpll_pin *pin = pin_priv;

	*frequency = zl3073x_dev_output_pin_freq_get(zldpll->dev, pin->id);

	return 0;
}

static int
zl3073x_dpll_output_pin_frequency_set(const struct dpll_pin *dpll_pin,
				      void *pin_priv,
				      const struct dpll_device *dpll,
				      void *dpll_priv, u64 frequency,
				      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_synth *synth;
<<<<<<< HEAD
	u32 new_div, synth_freq;
	struct zl3073x_out out;
	u8 out_id;
=======
	u8 out_id, signal_format;
	u32 new_div, synth_freq;
	struct zl3073x_out out;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	out_id = zl3073x_output_pin_out_get(pin->id);
	out = *zl3073x_out_state_get(zldev, out_id);

	/* Get attached synth frequency and compute new divisor */
	synth = zl3073x_synth_state_get(zldev, zl3073x_out_synth_get(&out));
	synth_freq = zl3073x_synth_freq_get(synth);
	new_div = synth_freq / (u32)frequency;

<<<<<<< HEAD
	/* Check signal format */
	if (!zl3073x_out_is_ndiv(&out)) {
=======
	/* Get used signal format for the given output */
	signal_format = zl3073x_out_signal_format_get(&out);

	/* Check signal format */
	if (signal_format != ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV &&
	    signal_format != ZL_OUTPUT_MODE_SIGNAL_FORMAT_2_NDIV_INV) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		/* For non N-divided signal formats the frequency is computed
		 * as division of synth frequency and output divisor.
		 */
		out.div = new_div;

		/* For 50/50 duty cycle the divisor is equal to width */
		out.width = new_div;

		/* Commit output configuration */
		return zl3073x_out_state_set(zldev, out_id, &out);
	}

	if (zl3073x_dpll_is_p_pin(pin)) {
		/* We are going to change output frequency for P-pin but
		 * if the requested frequency is less than current N-pin
		 * frequency then indicate a failure as we are not able
		 * to compute N-pin divisor to keep its frequency unchanged.
		 *
		 * Update divisor for N-pin to keep N-pin frequency.
		 */
		out.esync_n_period = (out.esync_n_period * out.div) / new_div;
		if (!out.esync_n_period)
			return -EINVAL;

		/* Update the output divisor */
		out.div = new_div;

		/* For 50/50 duty cycle the divisor is equal to width */
		out.width = out.div;
	} else {
		/* We are going to change frequency of N-pin but if
		 * the requested freq is greater or equal than freq of P-pin
		 * in the output pair we cannot compute divisor for the N-pin.
		 * In this case indicate a failure.
		 *
		 * Update divisor for N-pin
		 */
		out.esync_n_period = div64_u64(synth_freq, frequency * out.div);
		if (!out.esync_n_period)
			return -EINVAL;
	}

	/* For 50/50 duty cycle the divisor is equal to width */
	out.esync_n_width = out.esync_n_period;

	/* Commit output configuration */
	return zl3073x_out_state_set(zldev, out_id, &out);
}

static int
zl3073x_dpll_output_pin_phase_adjust_get(const struct dpll_pin *dpll_pin,
					 void *pin_priv,
					 const struct dpll_device *dpll,
					 void *dpll_priv,
					 s32 *phase_adjust,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	const struct zl3073x_out *out;
	u8 out_id;

	out_id = zl3073x_output_pin_out_get(pin->id);
	out = zl3073x_out_state_get(zldev, out_id);

	/* The value in the register is expressed in half synth clock cycles. */
	*phase_adjust = out->phase_comp * pin->phase_gran;

	return 0;
}

static int
zl3073x_dpll_output_pin_phase_adjust_set(const struct dpll_pin *dpll_pin,
					 void *pin_priv,
					 const struct dpll_device *dpll,
					 void *dpll_priv,
					 s32 phase_adjust,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	struct zl3073x_dpll_pin *pin = pin_priv;
	struct zl3073x_out out;
	u8 out_id;

	out_id = zl3073x_output_pin_out_get(pin->id);
	out = *zl3073x_out_state_get(zldev, out_id);

	/* The value in the register is expressed in half synth clock cycles. */
	out.phase_comp = phase_adjust / pin->phase_gran;

	/* Update output configuration from mailbox */
	return zl3073x_out_state_set(zldev, out_id, &out);
}

static int
zl3073x_dpll_output_pin_state_on_dpll_get(const struct dpll_pin *dpll_pin,
					  void *pin_priv,
					  const struct dpll_device *dpll,
					  void *dpll_priv,
					  enum dpll_pin_state *state,
					  struct netlink_ext_ack *extack)
{
	/* If the output pin is registered then it is always connected */
	*state = DPLL_PIN_STATE_CONNECTED;

	return 0;
}

static int
<<<<<<< HEAD
zl3073x_dpll_temp_get(const struct dpll_device *dpll, void *dpll_priv,
		      s32 *temp, struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
	struct zl3073x_dev *zldev = zldpll->dev;
	u16 val;
	int rc;

	rc = zl3073x_read_u16(zldev, ZL_REG_DIE_TEMP_STATUS, &val);
	if (rc)
		return rc;

	/* Register value is in units of 0.1 C, convert to millidegrees */
	*temp = (s16)val * 100;

	return 0;
}

static int
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
zl3073x_dpll_lock_status_get(const struct dpll_device *dpll, void *dpll_priv,
			     enum dpll_lock_status *status,
			     enum dpll_lock_status_error *status_error,
			     struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;

	chan = zl3073x_chan_state_get(zldpll->dev, zldpll->id);

	switch (zl3073x_chan_mode_get(chan)) {
=======
	struct zl3073x_dev *zldev = zldpll->dev;
	u8 mon_status, state;
	int rc;

	switch (zldpll->refsel_mode) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	case ZL_DPLL_MODE_REFSEL_MODE_FREERUN:
	case ZL_DPLL_MODE_REFSEL_MODE_NCO:
		/* In FREERUN and NCO modes the DPLL is always unlocked */
		*status = DPLL_LOCK_STATUS_UNLOCKED;

		return 0;
	default:
		break;
	}

<<<<<<< HEAD
	switch (zl3073x_chan_lock_state_get(chan)) {
	case ZL_DPLL_MON_STATUS_STATE_LOCK:
		if (zl3073x_chan_is_ho_ready(chan))
=======
	/* Read DPLL monitor status */
	rc = zl3073x_read_u8(zldev, ZL_REG_DPLL_MON_STATUS(zldpll->id),
			     &mon_status);
	if (rc)
		return rc;
	state = FIELD_GET(ZL_DPLL_MON_STATUS_STATE, mon_status);

	switch (state) {
	case ZL_DPLL_MON_STATUS_STATE_LOCK:
		if (FIELD_GET(ZL_DPLL_MON_STATUS_HO_READY, mon_status))
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
			*status = DPLL_LOCK_STATUS_LOCKED_HO_ACQ;
		else
			*status = DPLL_LOCK_STATUS_LOCKED;
		break;
	case ZL_DPLL_MON_STATUS_STATE_HOLDOVER:
	case ZL_DPLL_MON_STATUS_STATE_ACQUIRING:
		*status = DPLL_LOCK_STATUS_HOLDOVER;
		break;
	default:
<<<<<<< HEAD
		dev_warn(zldpll->dev->dev,
			 "Unknown DPLL monitor status: 0x%02x\n",
			 chan->mon_status);
=======
		dev_warn(zldev->dev, "Unknown DPLL monitor status: 0x%02x\n",
			 mon_status);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		*status = DPLL_LOCK_STATUS_UNLOCKED;
		break;
	}

	return 0;
}

static int
zl3073x_dpll_supported_modes_get(const struct dpll_device *dpll,
				 void *dpll_priv, unsigned long *modes,
				 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;

	chan = zl3073x_chan_state_get(zldpll->dev, zldpll->id);
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* We support switching between automatic and manual mode, except in
	 * a case where the DPLL channel is configured to run in NCO mode.
	 * In this case, report only the manual mode to which the NCO is mapped
	 * as the only supported one.
	 */
<<<<<<< HEAD
	if (zl3073x_chan_mode_get(chan) != ZL_DPLL_MODE_REFSEL_MODE_NCO)
=======
	if (zldpll->refsel_mode != ZL_DPLL_MODE_REFSEL_MODE_NCO)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		__set_bit(DPLL_MODE_AUTOMATIC, modes);

	__set_bit(DPLL_MODE_MANUAL, modes);

	return 0;
}

static int
zl3073x_dpll_mode_get(const struct dpll_device *dpll, void *dpll_priv,
		      enum dpll_mode *mode, struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;

	chan = zl3073x_chan_state_get(zldpll->dev, zldpll->id);

	switch (zl3073x_chan_mode_get(chan)) {
=======

	switch (zldpll->refsel_mode) {
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	case ZL_DPLL_MODE_REFSEL_MODE_FREERUN:
	case ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER:
	case ZL_DPLL_MODE_REFSEL_MODE_NCO:
	case ZL_DPLL_MODE_REFSEL_MODE_REFLOCK:
		/* Use MANUAL for device FREERUN, HOLDOVER, NCO and
		 * REFLOCK modes
		 */
		*mode = DPLL_MODE_MANUAL;
		break;
	case ZL_DPLL_MODE_REFSEL_MODE_AUTO:
		/* Use AUTO for device AUTO mode */
		*mode = DPLL_MODE_AUTOMATIC;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int
zl3073x_dpll_phase_offset_avg_factor_get(const struct dpll_device *dpll,
					 void *dpll_priv, u32 *factor,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;

	*factor = zl3073x_dev_phase_avg_factor_get(zldpll->dev);

	return 0;
}

static void
zl3073x_dpll_change_work(struct work_struct *work)
{
	struct zl3073x_dpll *zldpll;

	zldpll = container_of(work, struct zl3073x_dpll, change_work);
	dpll_device_change_ntf(zldpll->dpll_dev);
}

static int
zl3073x_dpll_phase_offset_avg_factor_set(const struct dpll_device *dpll,
					 void *dpll_priv, u32 factor,
					 struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *item, *zldpll = dpll_priv;
	int rc;

	if (factor > 15) {
		NL_SET_ERR_MSG_FMT(extack,
				   "Phase offset average factor has to be from range <0,15>");
		return -EINVAL;
	}

	rc = zl3073x_dev_phase_avg_factor_set(zldpll->dev, factor);
	if (rc) {
		NL_SET_ERR_MSG_FMT(extack,
				   "Failed to set phase offset averaging factor");
		return rc;
	}

	/* The averaging factor is common for all DPLL channels so after change
	 * we have to send a notification for other DPLL devices.
	 */
	list_for_each_entry(item, &zldpll->dev->dplls, list) {
		if (item != zldpll)
			schedule_work(&item->change_work);
	}

	return 0;
}

static int
zl3073x_dpll_mode_set(const struct dpll_device *dpll, void *dpll_priv,
		      enum dpll_mode mode, struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;
<<<<<<< HEAD
	struct zl3073x_chan chan;
	u8 hw_mode, ref;
	int rc;

	chan = *zl3073x_chan_state_get(zldpll->dev, zldpll->id);
	ref = zl3073x_chan_refsel_ref_get(&chan);
=======
	u8 hw_mode, mode_refsel, ref;
	int rc;

	rc = zl3073x_dpll_selected_ref_get(zldpll, &ref);
	if (rc) {
		NL_SET_ERR_MSG_MOD(extack, "failed to get selected reference");
		return rc;
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (mode == DPLL_MODE_MANUAL) {
		/* We are switching from automatic to manual mode:
		 * - if we have a valid reference selected during auto mode then
		 *   we will switch to forced reference lock mode and use this
		 *   reference for selection
		 * - if NO valid reference is selected, we will switch to forced
		 *   holdover mode or freerun mode, depending on the current
		 *   lock status
		 */
		if (ZL3073X_DPLL_REF_IS_VALID(ref))
			hw_mode = ZL_DPLL_MODE_REFSEL_MODE_REFLOCK;
		else if (zldpll->lock_status == DPLL_LOCK_STATUS_UNLOCKED)
			hw_mode = ZL_DPLL_MODE_REFSEL_MODE_FREERUN;
		else
			hw_mode = ZL_DPLL_MODE_REFSEL_MODE_HOLDOVER;
	} else {
		/* We are switching from manual to automatic mode:
		 * - if there is a valid reference selected then ensure that
		 *   it is selectable after switch to automatic mode
		 * - switch to automatic mode
		 */
<<<<<<< HEAD
		if (ZL3073X_DPLL_REF_IS_VALID(ref) &&
		    !zl3073x_chan_ref_is_selectable(&chan, ref)) {
			struct zl3073x_dpll_pin *pin;

			pin = zl3073x_dpll_pin_get_by_ref(zldpll, ref);
			if (pin) {
				/* Restore pin priority in HW */
				zl3073x_chan_ref_prio_set(&chan, ref,
							  pin->prio);
			}
=======
		struct zl3073x_dpll_pin *pin;

		pin = zl3073x_dpll_pin_get_by_ref(zldpll, ref);
		if (pin && !pin->selectable) {
			/* Restore pin priority in HW */
			rc = zl3073x_dpll_ref_prio_set(pin, pin->prio);
			if (rc) {
				NL_SET_ERR_MSG_MOD(extack,
						   "failed to restore pin priority");
				return rc;
			}

			pin->selectable = true;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		}

		hw_mode = ZL_DPLL_MODE_REFSEL_MODE_AUTO;
	}

<<<<<<< HEAD
	zl3073x_chan_mode_set(&chan, hw_mode);
	if (ZL3073X_DPLL_REF_IS_VALID(ref))
		zl3073x_chan_ref_set(&chan, ref);

	rc = zl3073x_chan_state_set(zldpll->dev, zldpll->id, &chan);
=======
	/* Build mode_refsel value */
	mode_refsel = FIELD_PREP(ZL_DPLL_MODE_REFSEL_MODE, hw_mode);

	if (ZL3073X_DPLL_REF_IS_VALID(ref))
		mode_refsel |= FIELD_PREP(ZL_DPLL_MODE_REFSEL_REF, ref);

	/* Update dpll_mode_refsel register */
	rc = zl3073x_write_u8(zldpll->dev, ZL_REG_DPLL_MODE_REFSEL(zldpll->id),
			      mode_refsel);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (rc) {
		NL_SET_ERR_MSG_MOD(extack,
				   "failed to set reference selection mode");
		return rc;
	}

<<<<<<< HEAD
=======
	zldpll->refsel_mode = hw_mode;

	if (ZL3073X_DPLL_REF_IS_VALID(ref))
		zldpll->forced_ref = ref;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return 0;
}

static int
zl3073x_dpll_phase_offset_monitor_get(const struct dpll_device *dpll,
				      void *dpll_priv,
				      enum dpll_feature_state *state,
				      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;

	if (zldpll->phase_monitor)
		*state = DPLL_FEATURE_STATE_ENABLE;
	else
		*state = DPLL_FEATURE_STATE_DISABLE;

	return 0;
}

static int
zl3073x_dpll_phase_offset_monitor_set(const struct dpll_device *dpll,
				      void *dpll_priv,
				      enum dpll_feature_state state,
				      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;

	zldpll->phase_monitor = (state == DPLL_FEATURE_STATE_ENABLE);

	return 0;
}

<<<<<<< HEAD
static int
zl3073x_dpll_freq_monitor_get(const struct dpll_device *dpll,
			      void *dpll_priv,
			      enum dpll_feature_state *state,
			      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;

	if (zldpll->freq_monitor)
		*state = DPLL_FEATURE_STATE_ENABLE;
	else
		*state = DPLL_FEATURE_STATE_DISABLE;

	return 0;
}

static int
zl3073x_dpll_freq_monitor_set(const struct dpll_device *dpll,
			      void *dpll_priv,
			      enum dpll_feature_state state,
			      struct netlink_ext_ack *extack)
{
	struct zl3073x_dpll *zldpll = dpll_priv;

	zldpll->freq_monitor = (state == DPLL_FEATURE_STATE_ENABLE);

	return 0;
}

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
static const struct dpll_pin_ops zl3073x_dpll_input_pin_ops = {
	.direction_get = zl3073x_dpll_pin_direction_get,
	.esync_get = zl3073x_dpll_input_pin_esync_get,
	.esync_set = zl3073x_dpll_input_pin_esync_set,
	.ffo_get = zl3073x_dpll_input_pin_ffo_get,
	.frequency_get = zl3073x_dpll_input_pin_frequency_get,
	.frequency_set = zl3073x_dpll_input_pin_frequency_set,
<<<<<<< HEAD
	.measured_freq_get = zl3073x_dpll_input_pin_measured_freq_get,
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.phase_offset_get = zl3073x_dpll_input_pin_phase_offset_get,
	.phase_adjust_get = zl3073x_dpll_input_pin_phase_adjust_get,
	.phase_adjust_set = zl3073x_dpll_input_pin_phase_adjust_set,
	.prio_get = zl3073x_dpll_input_pin_prio_get,
	.prio_set = zl3073x_dpll_input_pin_prio_set,
<<<<<<< HEAD
	.ref_sync_get = zl3073x_dpll_input_pin_ref_sync_get,
	.ref_sync_set = zl3073x_dpll_input_pin_ref_sync_set,
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.state_on_dpll_get = zl3073x_dpll_input_pin_state_on_dpll_get,
	.state_on_dpll_set = zl3073x_dpll_input_pin_state_on_dpll_set,
};

static const struct dpll_pin_ops zl3073x_dpll_output_pin_ops = {
	.direction_get = zl3073x_dpll_pin_direction_get,
	.esync_get = zl3073x_dpll_output_pin_esync_get,
	.esync_set = zl3073x_dpll_output_pin_esync_set,
	.frequency_get = zl3073x_dpll_output_pin_frequency_get,
	.frequency_set = zl3073x_dpll_output_pin_frequency_set,
	.phase_adjust_get = zl3073x_dpll_output_pin_phase_adjust_get,
	.phase_adjust_set = zl3073x_dpll_output_pin_phase_adjust_set,
	.state_on_dpll_get = zl3073x_dpll_output_pin_state_on_dpll_get,
};

static const struct dpll_device_ops zl3073x_dpll_device_ops = {
	.lock_status_get = zl3073x_dpll_lock_status_get,
	.mode_get = zl3073x_dpll_mode_get,
	.mode_set = zl3073x_dpll_mode_set,
	.phase_offset_avg_factor_get = zl3073x_dpll_phase_offset_avg_factor_get,
	.phase_offset_avg_factor_set = zl3073x_dpll_phase_offset_avg_factor_set,
	.phase_offset_monitor_get = zl3073x_dpll_phase_offset_monitor_get,
	.phase_offset_monitor_set = zl3073x_dpll_phase_offset_monitor_set,
<<<<<<< HEAD
	.freq_monitor_get = zl3073x_dpll_freq_monitor_get,
	.freq_monitor_set = zl3073x_dpll_freq_monitor_set,
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	.supported_modes_get = zl3073x_dpll_supported_modes_get,
};

/**
 * zl3073x_dpll_pin_alloc - allocate DPLL pin
 * @zldpll: pointer to zl3073x_dpll
 * @dir: pin direction
 * @id: pin id
 *
 * Allocates and initializes zl3073x_dpll_pin structure for given
 * pin id and direction.
 *
 * Return: pointer to allocated structure on success, error pointer on error
 */
static struct zl3073x_dpll_pin *
zl3073x_dpll_pin_alloc(struct zl3073x_dpll *zldpll, enum dpll_pin_direction dir,
		       u8 id)
{
	struct zl3073x_dpll_pin *pin;

	pin = kzalloc_obj(*pin);
	if (!pin)
		return ERR_PTR(-ENOMEM);

	pin->dpll = zldpll;
	pin->dir = dir;
	pin->id = id;

	return pin;
}

/**
 * zl3073x_dpll_pin_free - deallocate DPLL pin
 * @pin: pin to free
 *
 * Deallocates DPLL pin previously allocated by @zl3073x_dpll_pin_alloc.
 */
static void
zl3073x_dpll_pin_free(struct zl3073x_dpll_pin *pin)
{
	WARN(pin->dpll_pin, "DPLL pin is still registered\n");

	kfree(pin);
}

/**
 * zl3073x_dpll_pin_register - register DPLL pin
 * @pin: pointer to DPLL pin
 * @index: absolute pin index for registration
 *
 * Registers given DPLL pin into DPLL sub-system.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_pin_register(struct zl3073x_dpll_pin *pin, u32 index)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_pin_props *props;
	const struct dpll_pin_ops *ops;
	int rc;

	/* Get pin properties */
	props = zl3073x_pin_props_get(zldpll->dev, pin->dir, pin->id);
	if (IS_ERR(props))
		return PTR_ERR(props);

<<<<<<< HEAD
	/* Save package label, fwnode, esync capability and phase adjust
	 * granularity.
	 */
	strscpy(pin->label, props->package_label);
	pin->fwnode = fwnode_handle_get(props->fwnode);
=======
	/* Save package label, esync capability and phase adjust granularity */
	strscpy(pin->label, props->package_label);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	pin->esync_control = props->esync_control;
	pin->phase_gran = props->dpll_props.phase_gran;

	if (zl3073x_dpll_is_input_pin(pin)) {
<<<<<<< HEAD
		const struct zl3073x_chan *chan;
		u8 ref;

		chan = zl3073x_chan_state_get(zldpll->dev, zldpll->id);
		ref = zl3073x_input_pin_ref_get(pin->id);
		pin->prio = zl3073x_chan_ref_prio_get(chan, ref);

		if (pin->prio == ZL_DPLL_REF_PRIO_NONE)
			/* Clamp prio to max value */
			pin->prio = ZL_DPLL_REF_PRIO_MAX;
=======
		rc = zl3073x_dpll_ref_prio_get(pin, &pin->prio);
		if (rc)
			goto err_prio_get;

		if (pin->prio == ZL_DPLL_REF_PRIO_NONE) {
			/* Clamp prio to max value & mark pin non-selectable */
			pin->prio = ZL_DPLL_REF_PRIO_MAX;
			pin->selectable = false;
		} else {
			/* Mark pin as selectable */
			pin->selectable = true;
		}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	}

	/* Create or get existing DPLL pin */
	pin->dpll_pin = dpll_pin_get(zldpll->dev->clock_id, index, THIS_MODULE,
				     &props->dpll_props, &pin->tracker);
	if (IS_ERR(pin->dpll_pin)) {
		rc = PTR_ERR(pin->dpll_pin);
		goto err_pin_get;
	}
	dpll_pin_fwnode_set(pin->dpll_pin, props->fwnode);

	if (zl3073x_dpll_is_input_pin(pin))
		ops = &zl3073x_dpll_input_pin_ops;
	else
		ops = &zl3073x_dpll_output_pin_ops;

	/* Register the pin */
	rc = dpll_pin_register(zldpll->dpll_dev, pin->dpll_pin, ops, pin);
	if (rc)
		goto err_register;

	/* Free pin properties */
	zl3073x_pin_props_put(props);

	return 0;

err_register:
	dpll_pin_put(pin->dpll_pin, &pin->tracker);
<<<<<<< HEAD
=======
<<<<<<< HEAD
	pin->dpll_pin = NULL;
>>>>>>> 7fb39c93c52e (Sync)
err_pin_get:
	pin->dpll_pin = NULL;
	fwnode_handle_put(pin->fwnode);
	pin->fwnode = NULL;
=======
err_prio_get:
	pin->dpll_pin = NULL;
err_pin_get:
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	zl3073x_pin_props_put(props);

	return rc;
}

/**
 * zl3073x_dpll_pin_unregister - unregister DPLL pin
 * @pin: pointer to DPLL pin
 *
 * Unregisters pin previously registered by @zl3073x_dpll_pin_register.
 */
static void
zl3073x_dpll_pin_unregister(struct zl3073x_dpll_pin *pin)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	const struct dpll_pin_ops *ops;

	WARN(!pin->dpll_pin, "DPLL pin is not registered\n");

	if (zl3073x_dpll_is_input_pin(pin))
		ops = &zl3073x_dpll_input_pin_ops;
	else
		ops = &zl3073x_dpll_output_pin_ops;

	/* Unregister the pin */
	dpll_pin_unregister(zldpll->dpll_dev, pin->dpll_pin, ops, pin);

	dpll_pin_put(pin->dpll_pin, &pin->tracker);
	pin->dpll_pin = NULL;
<<<<<<< HEAD

	fwnode_handle_put(pin->fwnode);
	pin->fwnode = NULL;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/**
 * zl3073x_dpll_pins_unregister - unregister all registered DPLL pins
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Enumerates all DPLL pins registered to given DPLL device and
 * unregisters them.
 */
static void
zl3073x_dpll_pins_unregister(struct zl3073x_dpll *zldpll)
{
	struct zl3073x_dpll_pin *pin, *next;

	list_for_each_entry_safe(pin, next, &zldpll->pins, list) {
		zl3073x_dpll_pin_unregister(pin);
		list_del(&pin->list);
		zl3073x_dpll_pin_free(pin);
	}
}

/**
 * zl3073x_dpll_pin_is_registrable - check if the pin is registrable
 * @zldpll: pointer to zl3073x_dpll structure
 * @dir: pin direction
 * @index: pin index
 *
 * Checks if the given pin can be registered to given DPLL. For both
 * directions the pin can be registered if it is enabled. In case of
 * differential signal type only P-pin is reported as registrable.
 * And additionally for the output pin, the pin can be registered only
 * if it is connected to synthesizer that is driven by given DPLL.
 *
 * Return: true if the pin is registrable, false if not
 */
static bool
zl3073x_dpll_pin_is_registrable(struct zl3073x_dpll *zldpll,
				enum dpll_pin_direction dir, u8 index)
{
	struct zl3073x_dev *zldev = zldpll->dev;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;
	bool is_diff, is_enabled;
	const char *name;

	chan = zl3073x_chan_state_get(zldev, zldpll->id);

=======
	bool is_diff, is_enabled;
	const char *name;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (dir == DPLL_PIN_DIRECTION_INPUT) {
		u8 ref_id = zl3073x_input_pin_ref_get(index);
		const struct zl3073x_ref *ref;

		/* Skip the pin if the DPLL is running in NCO mode */
<<<<<<< HEAD
		if (zl3073x_chan_mode_get(chan) == ZL_DPLL_MODE_REFSEL_MODE_NCO)
=======
		if (zldpll->refsel_mode == ZL_DPLL_MODE_REFSEL_MODE_NCO)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
			return false;

		name = "REF";
		ref = zl3073x_ref_state_get(zldev, ref_id);
		is_diff = zl3073x_ref_is_diff(ref);
		is_enabled = zl3073x_ref_is_enabled(ref);
	} else {
		/* Output P&N pair shares single HW output */
		u8 out = zl3073x_output_pin_out_get(index);

		/* Skip the pin if it is connected to different DPLL channel */
		if (zl3073x_dev_out_dpll_get(zldev, out) != zldpll->id) {
			dev_dbg(zldev->dev,
				"OUT%u is driven by different DPLL\n", out);

			return false;
		}

		name = "OUT";
		is_diff = zl3073x_dev_out_is_diff(zldev, out);
		is_enabled = zl3073x_dev_output_pin_is_enabled(zldev, index);
	}

	/* Skip N-pin if the corresponding input/output is differential */
	if (is_diff && zl3073x_is_n_pin(index)) {
		dev_dbg(zldev->dev, "%s%u is differential, skipping N-pin\n",
			name, index / 2);

		return false;
	}

	/* Skip the pin if it is disabled */
	if (!is_enabled) {
		dev_dbg(zldev->dev, "%s%u%c is disabled\n", name, index / 2,
			zl3073x_is_p_pin(index) ? 'P' : 'N');

		return false;
	}

	return true;
}

/**
 * zl3073x_dpll_pins_register - register all registerable DPLL pins
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Enumerates all possible input/output pins and registers all of them
 * that are registrable.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_pins_register(struct zl3073x_dpll *zldpll)
{
	struct zl3073x_dpll_pin *pin;
	enum dpll_pin_direction dir;
	u8 id, index;
	int rc;

	/* Process input pins */
	for (index = 0; index < ZL3073X_NUM_PINS; index++) {
		/* First input pins and then output pins */
		if (index < ZL3073X_NUM_INPUT_PINS) {
			id = index;
			dir = DPLL_PIN_DIRECTION_INPUT;
		} else {
			id = index - ZL3073X_NUM_INPUT_PINS;
			dir = DPLL_PIN_DIRECTION_OUTPUT;
		}

		/* Check if the pin registrable to this DPLL */
		if (!zl3073x_dpll_pin_is_registrable(zldpll, dir, id))
			continue;

		pin = zl3073x_dpll_pin_alloc(zldpll, dir, id);
		if (IS_ERR(pin)) {
			rc = PTR_ERR(pin);
			goto error;
		}

		rc = zl3073x_dpll_pin_register(pin, index);
		if (rc) {
			zl3073x_dpll_pin_free(pin);
			goto error;
		}

		list_add(&pin->list, &zldpll->pins);
	}

	return 0;

error:
	zl3073x_dpll_pins_unregister(zldpll);

	return rc;
}

/**
 * zl3073x_dpll_device_register - register DPLL device
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Registers given DPLL device into DPLL sub-system.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_device_register(struct zl3073x_dpll *zldpll)
{
	struct zl3073x_dev *zldev = zldpll->dev;
<<<<<<< HEAD
	int rc;

	zldpll->ops = zl3073x_dpll_device_ops;
	if (zldev->info->flags & ZL3073X_FLAG_DIE_TEMP)
		zldpll->ops.temp_get = zl3073x_dpll_temp_get;
=======
	u8 dpll_mode_refsel;
	int rc;

	/* Read DPLL mode and forcibly selected reference */
	rc = zl3073x_read_u8(zldev, ZL_REG_DPLL_MODE_REFSEL(zldpll->id),
			     &dpll_mode_refsel);
	if (rc)
		return rc;

	/* Extract mode and selected input reference */
	zldpll->refsel_mode = FIELD_GET(ZL_DPLL_MODE_REFSEL_MODE,
					dpll_mode_refsel);
	zldpll->forced_ref = FIELD_GET(ZL_DPLL_MODE_REFSEL_REF,
				       dpll_mode_refsel);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	zldpll->dpll_dev = dpll_device_get(zldev->clock_id, zldpll->id,
					   THIS_MODULE, &zldpll->tracker);
	if (IS_ERR(zldpll->dpll_dev)) {
		rc = PTR_ERR(zldpll->dpll_dev);
		zldpll->dpll_dev = NULL;

		return rc;
	}

	rc = dpll_device_register(zldpll->dpll_dev,
				  zl3073x_prop_dpll_type_get(zldev, zldpll->id),
<<<<<<< HEAD
				  &zldpll->ops, zldpll);
=======
				  &zl3073x_dpll_device_ops, zldpll);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	if (rc) {
		dpll_device_put(zldpll->dpll_dev, &zldpll->tracker);
		zldpll->dpll_dev = NULL;
	}

	return rc;
}

/**
 * zl3073x_dpll_device_unregister - unregister DPLL device
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Unregisters given DPLL device from DPLL sub-system previously registered
 * by @zl3073x_dpll_device_register.
 */
static void
zl3073x_dpll_device_unregister(struct zl3073x_dpll *zldpll)
{
	WARN(!zldpll->dpll_dev, "DPLL device is not registered\n");

	cancel_work_sync(&zldpll->change_work);

<<<<<<< HEAD
	dpll_device_unregister(zldpll->dpll_dev, &zldpll->ops, zldpll);
=======
	dpll_device_unregister(zldpll->dpll_dev, &zl3073x_dpll_device_ops,
			       zldpll);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	dpll_device_put(zldpll->dpll_dev, &zldpll->tracker);
	zldpll->dpll_dev = NULL;
}

/**
 * zl3073x_dpll_pin_phase_offset_check - check for pin phase offset change
 * @pin: pin to check
 *
 * Check for the change of DPLL to connected pin phase offset change.
 *
 * Return: true on phase offset change, false otherwise
 */
static bool
zl3073x_dpll_pin_phase_offset_check(struct zl3073x_dpll_pin *pin)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	unsigned int reg;
	s64 phase_offset;
	u8 ref_id;
	int rc;

	/* No phase offset if the ref monitor reports signal errors */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	if (!zl3073x_dev_ref_is_status_ok(zldev, ref_id))
		return false;

	/* Select register to read phase offset value depending on pin and
	 * phase monitor state:
	 * 1) For connected pin use dpll_phase_err_data register
	 * 2) For other pins use appropriate ref_phase register if the phase
	 *    monitor feature is enabled.
	 */
	if (pin->pin_state == DPLL_PIN_STATE_CONNECTED)
		reg = ZL_REG_DPLL_PHASE_ERR_DATA(zldpll->id);
	else if (zldpll->phase_monitor)
		reg = ZL_REG_REF_PHASE(ref_id);
	else
		return false;

	/* Read measured phase offset value */
	rc = zl3073x_read_u48(zldev, reg, &phase_offset);
	if (rc) {
		dev_err(zldev->dev, "Failed to read ref phase offset: %pe\n",
			ERR_PTR(rc));

		return false;
	}

	/* Convert to ps */
	phase_offset = div_s64(sign_extend64(phase_offset, 47), 100);

	/* Compare with previous value */
	if (phase_offset != pin->phase_offset) {
		dev_dbg(zldev->dev, "%s phase offset changed: %lld -> %lld\n",
			pin->label, pin->phase_offset, phase_offset);
		pin->phase_offset = phase_offset;

		return true;
	}

	return false;
}

/**
 * zl3073x_dpll_pin_ffo_check - check for pin fractional frequency offset change
 * @pin: pin to check
 *
 * Check for the given pin's fractional frequency change.
 *
 * Return: true on fractional frequency offset change, false otherwise
 */
static bool
zl3073x_dpll_pin_ffo_check(struct zl3073x_dpll_pin *pin)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	const struct zl3073x_ref *ref;
	u8 ref_id;
<<<<<<< HEAD
	s64 ffo;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* Get reference monitor status */
	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);

	/* Do not report ffo changes if the reference monitor report errors */
	if (!zl3073x_ref_is_status_ok(ref))
		return false;

	/* Compare with previous value */
<<<<<<< HEAD
	ffo = zl3073x_ref_ffo_get(ref);
	if (pin->freq_offset != ffo) {
		dev_dbg(zldev->dev, "%s freq offset changed: %lld -> %lld\n",
			pin->label, pin->freq_offset, ffo);
		pin->freq_offset = ffo;

		return true;
	}

	return false;
}

/**
 * zl3073x_dpll_pin_measured_freq_check - check for pin measured frequency
 * change
 * @pin: pin to check
 *
 * Check for the given pin's measured frequency change.
 *
 * Return: true on measured frequency change, false otherwise
 */
static bool
zl3073x_dpll_pin_measured_freq_check(struct zl3073x_dpll_pin *pin)
{
	struct zl3073x_dpll *zldpll = pin->dpll;
	struct zl3073x_dev *zldev = zldpll->dev;
	const struct zl3073x_ref *ref;
	u8 ref_id;
	u32 freq;

	if (!zldpll->freq_monitor)
		return false;

	ref_id = zl3073x_input_pin_ref_get(pin->id);
	ref = zl3073x_ref_state_get(zldev, ref_id);

	freq = zl3073x_ref_meas_freq_get(ref);
	if (pin->measured_freq != freq) {
		dev_dbg(zldev->dev, "%s measured freq changed: %u -> %u\n",
			pin->label, pin->measured_freq, freq);
		pin->measured_freq = freq;
=======
	if (pin->freq_offset != ref->ffo) {
		dev_dbg(zldev->dev, "%s freq offset changed: %lld -> %lld\n",
			pin->label, pin->freq_offset, ref->ffo);
		pin->freq_offset = ref->ffo;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

		return true;
	}

	return false;
}

/**
 * zl3073x_dpll_changes_check - check for changes and send notifications
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Checks for changes on given DPLL device and its registered DPLL pins
 * and sends notifications about them.
 *
 * This function is periodically called from @zl3073x_dev_periodic_work.
 */
void
zl3073x_dpll_changes_check(struct zl3073x_dpll *zldpll)
{
	struct zl3073x_dev *zldev = zldpll->dev;
	enum dpll_lock_status lock_status;
	struct device *dev = zldev->dev;
<<<<<<< HEAD
	const struct zl3073x_chan *chan;
	struct zl3073x_dpll_pin *pin;
	int rc;
	u8 mode;
=======
	struct zl3073x_dpll_pin *pin;
	int rc;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	zldpll->check_count++;

	/* Get current lock status for the DPLL */
	rc = zl3073x_dpll_lock_status_get(zldpll->dpll_dev, zldpll,
					  &lock_status, NULL, NULL);
	if (rc) {
		dev_err(dev, "Failed to get DPLL%u lock status: %pe\n",
			zldpll->id, ERR_PTR(rc));
		return;
	}

	/* If lock status was changed then notify DPLL core */
	if (zldpll->lock_status != lock_status) {
		zldpll->lock_status = lock_status;
		dpll_device_change_ntf(zldpll->dpll_dev);
	}

	/* Input pin monitoring does make sense only in automatic
	 * or forced reference modes.
	 */
<<<<<<< HEAD
	chan = zl3073x_chan_state_get(zldev, zldpll->id);
	mode = zl3073x_chan_mode_get(chan);
	if (mode != ZL_DPLL_MODE_REFSEL_MODE_AUTO &&
	    mode != ZL_DPLL_MODE_REFSEL_MODE_REFLOCK)
=======
	if (zldpll->refsel_mode != ZL_DPLL_MODE_REFSEL_MODE_AUTO &&
	    zldpll->refsel_mode != ZL_DPLL_MODE_REFSEL_MODE_REFLOCK)
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		return;

	/* Update phase offset latch registers for this DPLL if the phase
	 * offset monitor feature is enabled.
	 */
	if (zldpll->phase_monitor) {
		rc = zl3073x_ref_phase_offsets_update(zldev, zldpll->id);
		if (rc) {
			dev_err(zldev->dev,
				"Failed to update phase offsets: %pe\n",
				ERR_PTR(rc));
			return;
		}
	}

	list_for_each_entry(pin, &zldpll->pins, list) {
		enum dpll_pin_state state;
		bool pin_changed = false;

		/* Output pins change checks are not necessary because output
		 * states are constant.
		 */
		if (!zl3073x_dpll_is_input_pin(pin))
			continue;

		rc = zl3073x_dpll_ref_state_get(pin, &state);
		if (rc) {
			dev_err(dev,
				"Failed to get %s on DPLL%u state: %pe\n",
				pin->label, zldpll->id, ERR_PTR(rc));
			return;
		}

		if (state != pin->pin_state) {
			dev_dbg(dev, "%s state changed: %u->%u\n", pin->label,
				pin->pin_state, state);
			pin->pin_state = state;
			pin_changed = true;
		}

<<<<<<< HEAD
		/* Check for phase offset, ffo, and measured freq change
		 * once per second.
		 */
=======
		/* Check for phase offset and ffo change once per second */
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		if (zldpll->check_count % 2 == 0) {
			if (zl3073x_dpll_pin_phase_offset_check(pin))
				pin_changed = true;

			if (zl3073x_dpll_pin_ffo_check(pin))
				pin_changed = true;
<<<<<<< HEAD

			if (zl3073x_dpll_pin_measured_freq_check(pin))
				pin_changed = true;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
		}

		if (pin_changed)
			dpll_pin_change_ntf(pin->dpll_pin);
	}
}

/**
 * zl3073x_dpll_init_fine_phase_adjust - do initial fine phase adjustments
 * @zldev: pointer to zl3073x device
 *
 * Performs initial fine phase adjustments needed per datasheet.
 *
 * Return: 0 on success, <0 on error
 */
int
zl3073x_dpll_init_fine_phase_adjust(struct zl3073x_dev *zldev)
{
	int rc;

	rc = zl3073x_write_u8(zldev, ZL_REG_SYNTH_PHASE_SHIFT_MASK, 0x1f);
	if (rc)
		return rc;

	rc = zl3073x_write_u8(zldev, ZL_REG_SYNTH_PHASE_SHIFT_INTVL, 0x01);
	if (rc)
		return rc;

	rc = zl3073x_write_u16(zldev, ZL_REG_SYNTH_PHASE_SHIFT_DATA, 0xffff);
	if (rc)
		return rc;

	rc = zl3073x_write_u8(zldev, ZL_REG_SYNTH_PHASE_SHIFT_CTRL, 0x01);
	if (rc)
		return rc;

	return rc;
}

/**
 * zl3073x_dpll_alloc - allocate DPLL device
 * @zldev: pointer to zl3073x device
 * @ch: DPLL channel number
 *
 * Allocates DPLL device structure for given DPLL channel.
 *
 * Return: pointer to DPLL device on success, error pointer on error
 */
struct zl3073x_dpll *
zl3073x_dpll_alloc(struct zl3073x_dev *zldev, u8 ch)
{
	struct zl3073x_dpll *zldpll;

	zldpll = kzalloc_obj(*zldpll);
	if (!zldpll)
		return ERR_PTR(-ENOMEM);

	zldpll->dev = zldev;
	zldpll->id = ch;
	INIT_LIST_HEAD(&zldpll->pins);
	INIT_WORK(&zldpll->change_work, zl3073x_dpll_change_work);

	return zldpll;
}

/**
 * zl3073x_dpll_free - free DPLL device
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Deallocates given DPLL device previously allocated by @zl3073x_dpll_alloc.
 */
void
zl3073x_dpll_free(struct zl3073x_dpll *zldpll)
{
	WARN(zldpll->dpll_dev, "DPLL device is still registered\n");

	kfree(zldpll);
}

/**
<<<<<<< HEAD
 * zl3073x_dpll_ref_sync_pair_register - register ref_sync pairs for a pin
 * @pin: pointer to zl3073x_dpll_pin structure
 *
 * Iterates 'ref-sync-sources' phandles in the pin's firmware node and
 * registers each declared pairing.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_ref_sync_pair_register(struct zl3073x_dpll_pin *pin)
{
	struct zl3073x_dev *zldev = pin->dpll->dev;
	struct fwnode_handle *fwnode;
	struct dpll_pin *sync_pin;
	dpll_tracker tracker;
	int n, rc;

	for (n = 0; ; n++) {
		/* Get n'th ref-sync source */
		fwnode = fwnode_find_reference(pin->fwnode, "ref-sync-sources",
					       n);
		if (IS_ERR(fwnode)) {
			rc = PTR_ERR(fwnode);
			break;
		}

		/* Find associated dpll pin */
		sync_pin = fwnode_dpll_pin_find(fwnode, &tracker);
		fwnode_handle_put(fwnode);
		if (!sync_pin) {
			dev_warn(zldev->dev, "%s: ref-sync source %d not found",
				 pin->label, n);
			continue;
		}

		/* Register new ref-sync pair */
		rc = dpll_pin_ref_sync_pair_add(pin->dpll_pin, sync_pin);
		dpll_pin_put(sync_pin, &tracker);

		/* -EBUSY means pairing already exists from another DPLL's
		 * registration.
		 */
		if (rc && rc != -EBUSY) {
			dev_err(zldev->dev,
				"%s: failed to add ref-sync source %d: %pe",
				pin->label, n, ERR_PTR(rc));
			break;
		}
	}

	return rc != -ENOENT ? rc : 0;
}

/**
 * zl3073x_dpll_ref_sync_pairs_register - register ref_sync pairs for a DPLL
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Iterates all registered input pins of the given DPLL and establishes
 * ref_sync pairings declared by 'ref-sync-sources' phandles in the
 * device tree.
 *
 * Return: 0 on success, <0 on error
 */
static int
zl3073x_dpll_ref_sync_pairs_register(struct zl3073x_dpll *zldpll)
{
	struct zl3073x_dpll_pin *pin;
	int rc;

	list_for_each_entry(pin, &zldpll->pins, list) {
		if (!zl3073x_dpll_is_input_pin(pin) || !pin->fwnode)
			continue;

		rc = zl3073x_dpll_ref_sync_pair_register(pin);
		if (rc)
			return rc;
	}

	return 0;
}

/**
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
 * zl3073x_dpll_register - register DPLL device and all its pins
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Registers given DPLL device and all its pins into DPLL sub-system.
 *
 * Return: 0 on success, <0 on error
 */
int
zl3073x_dpll_register(struct zl3073x_dpll *zldpll)
{
	int rc;

	rc = zl3073x_dpll_device_register(zldpll);
	if (rc)
		return rc;

	rc = zl3073x_dpll_pins_register(zldpll);
	if (rc) {
		zl3073x_dpll_device_unregister(zldpll);
		return rc;
	}

<<<<<<< HEAD
	rc = zl3073x_dpll_ref_sync_pairs_register(zldpll);
	if (rc) {
		zl3073x_dpll_pins_unregister(zldpll);
		zl3073x_dpll_device_unregister(zldpll);
		return rc;
	}

=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	return 0;
}

/**
 * zl3073x_dpll_unregister - unregister DPLL device and its pins
 * @zldpll: pointer to zl3073x_dpll structure
 *
 * Unregisters given DPLL device and all its pins from DPLL sub-system
 * previously registered by @zl3073x_dpll_register.
 */
void
zl3073x_dpll_unregister(struct zl3073x_dpll *zldpll)
{
	/* Unregister all pins and dpll */
	zl3073x_dpll_pins_unregister(zldpll);
	zl3073x_dpll_device_unregister(zldpll);
}
