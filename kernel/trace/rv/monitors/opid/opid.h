/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Automatically generated C representation of opid automaton
 * For further information about this format, see kernel documentation:
 *   Documentation/trace/rv/deterministic_automata.rst
 */

#define MONITOR_NAME opid

enum states_opid {
<<<<<<< HEAD
	any_opid,
=======
	disabled_opid,
	enabled_opid,
	in_irq_opid,
	irq_disabled_opid,
	preempt_disabled_opid,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	state_max_opid,
};

#define INVALID_STATE state_max_opid

enum events_opid {
<<<<<<< HEAD
=======
	irq_disable_opid,
	irq_enable_opid,
	irq_entry_opid,
	preempt_disable_opid,
	preempt_enable_opid,
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	sched_need_resched_opid,
	sched_waking_opid,
	event_max_opid,
};

<<<<<<< HEAD
enum envs_opid {
	irq_off_opid,
	preempt_off_opid,
	env_max_opid,
	env_max_stored_opid = irq_off_opid,
};

_Static_assert(env_max_stored_opid <= MAX_HA_ENV_LEN, "Not enough slots");

struct automaton_opid {
	char *state_names[state_max_opid];
	char *event_names[event_max_opid];
	char *env_names[env_max_opid];
=======
struct automaton_opid {
	char *state_names[state_max_opid];
	char *event_names[event_max_opid];
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	unsigned char function[state_max_opid][event_max_opid];
	unsigned char initial_state;
	bool final_states[state_max_opid];
};

static const struct automaton_opid automaton_opid = {
	.state_names = {
<<<<<<< HEAD
		"any",
	},
	.event_names = {
		"sched_need_resched",
		"sched_waking",
	},
	.env_names = {
		"irq_off",
		"preempt_off",
	},
	.function = {
		{           any_opid,           any_opid },
	},
	.initial_state = any_opid,
	.final_states = { 1 },
=======
		"disabled",
		"enabled",
		"in_irq",
		"irq_disabled",
		"preempt_disabled",
	},
	.event_names = {
		"irq_disable",
		"irq_enable",
		"irq_entry",
		"preempt_disable",
		"preempt_enable",
		"sched_need_resched",
		"sched_waking",
	},
	.function = {
		{
			INVALID_STATE,
			preempt_disabled_opid,
			disabled_opid,
			INVALID_STATE,
			irq_disabled_opid,
			disabled_opid,
			disabled_opid,
		},
		{
			irq_disabled_opid,
			INVALID_STATE,
			INVALID_STATE,
			preempt_disabled_opid,
			enabled_opid,
			INVALID_STATE,
			INVALID_STATE,
		},
		{
			INVALID_STATE,
			enabled_opid,
			in_irq_opid,
			INVALID_STATE,
			INVALID_STATE,
			in_irq_opid,
			in_irq_opid,
		},
		{
			INVALID_STATE,
			enabled_opid,
			in_irq_opid,
			disabled_opid,
			INVALID_STATE,
			irq_disabled_opid,
			INVALID_STATE,
		},
		{
			disabled_opid,
			INVALID_STATE,
			INVALID_STATE,
			INVALID_STATE,
			enabled_opid,
			INVALID_STATE,
			INVALID_STATE,
		},
	},
	.initial_state = disabled_opid,
	.final_states = { 0, 1, 0, 0, 0 },
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
};
