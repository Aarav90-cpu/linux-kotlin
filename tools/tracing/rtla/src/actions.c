// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "actions.h"
#include "trace.h"
#include "utils.h"

/*
 * actions_init - initialize struct actions
 */
void
actions_init(struct actions *self)
{
	self->size = action_default_size;
<<<<<<< HEAD
	self->list = calloc_fatal(self->size, sizeof(struct action));
=======
	self->list = calloc(self->size, sizeof(struct action));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	self->len = 0;
	self->continue_flag = false;

	/* This has to be set by the user */
	self->trace_output_inst = NULL;
}

/*
 * actions_destroy - destroy struct actions
 */
void
actions_destroy(struct actions *self)
{
	/* Free any action-specific data */
	struct action *action;

	for_each_action(self, action) {
		if (action->type == ACTION_SHELL)
			free(action->command);
		if (action->type == ACTION_TRACE_OUTPUT)
			free(action->trace_output);
	}

	/* Free action list */
	free(self->list);
}

/*
 * actions_new - Get pointer to new action
 */
static struct action *
actions_new(struct actions *self)
{
	if (self->len >= self->size) {
<<<<<<< HEAD
		const size_t new_size = self->size * 2;

		self->list = reallocarray_fatal(self->list, new_size, sizeof(struct action));
		self->size = new_size;
=======
		self->size *= 2;
		self->list = realloc(self->list, self->size * sizeof(struct action));
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	}

	return &self->list[self->len++];
}

/*
 * actions_add_trace_output - add an action to output trace
 */
<<<<<<< HEAD
void
=======
int
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
actions_add_trace_output(struct actions *self, const char *trace_output)
{
	struct action *action = actions_new(self);

	self->present[ACTION_TRACE_OUTPUT] = true;
	action->type = ACTION_TRACE_OUTPUT;
<<<<<<< HEAD
	action->trace_output = strdup_fatal(trace_output);
=======
	action->trace_output = calloc(strlen(trace_output) + 1, sizeof(char));
	if (!action->trace_output)
		return -1;
	strcpy(action->trace_output, trace_output);

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * actions_add_trace_output - add an action to send signal to a process
 */
<<<<<<< HEAD
void
=======
int
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
actions_add_signal(struct actions *self, int signal, int pid)
{
	struct action *action = actions_new(self);

	self->present[ACTION_SIGNAL] = true;
	action->type = ACTION_SIGNAL;
	action->signal = signal;
	action->pid = pid;
<<<<<<< HEAD
=======

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * actions_add_shell - add an action to execute a shell command
 */
<<<<<<< HEAD
void
=======
int
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
actions_add_shell(struct actions *self, const char *command)
{
	struct action *action = actions_new(self);

	self->present[ACTION_SHELL] = true;
	action->type = ACTION_SHELL;
<<<<<<< HEAD
	action->command = strdup_fatal(command);
=======
	action->command = calloc(strlen(command) + 1, sizeof(char));
	if (!action->command)
		return -1;
	strcpy(action->command, command);

	return 0;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * actions_add_continue - add an action to resume measurement
 */
<<<<<<< HEAD
void
=======
int
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
actions_add_continue(struct actions *self)
{
	struct action *action = actions_new(self);

	self->present[ACTION_CONTINUE] = true;
	action->type = ACTION_CONTINUE;
<<<<<<< HEAD
}

static inline const char *__extract_arg(const char *token, const char *opt, size_t opt_len)
{
	const size_t tok_len = strlen(token);

	if (tok_len <= opt_len)
		return NULL;

	if (strncmp(token, opt, opt_len))
		return NULL;

	return token + opt_len;
}

/*
 * extract_arg - extract argument value from option token
 * @token: option token (e.g., "file=trace.txt")
 * @opt: option name to match (e.g., "file")
 *
 * Returns pointer to argument value after "=" if token matches "opt=",
 * otherwise returns NULL.
 */
#define extract_arg(token, opt) __extract_arg(token, opt "=", STRING_LENGTH(opt "="))

=======

	return 0;
}

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
/*
 * actions_parse - add an action based on text specification
 */
int
actions_parse(struct actions *self, const char *trigger, const char *tracefn)
{
	enum action_type type = ACTION_NONE;
	const char *token;
	char trigger_c[strlen(trigger) + 1];
<<<<<<< HEAD
	const char *arg_value;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	/* For ACTION_SIGNAL */
	int signal = 0, pid = 0;

	/* For ACTION_TRACE_OUTPUT */
	const char *trace_output;

	strcpy(trigger_c, trigger);
	token = strtok(trigger_c, ",");
	if (!token)
		return -1;

	if (strcmp(token, "trace") == 0)
		type = ACTION_TRACE_OUTPUT;
	else if (strcmp(token, "signal") == 0)
		type = ACTION_SIGNAL;
	else if (strcmp(token, "shell") == 0)
		type = ACTION_SHELL;
	else if (strcmp(token, "continue") == 0)
		type = ACTION_CONTINUE;
	else
		/* Invalid trigger type */
		return -1;

	token = strtok(NULL, ",");

	switch (type) {
	case ACTION_TRACE_OUTPUT:
		/* Takes no argument */
		if (token == NULL)
			trace_output = tracefn;
		else {
<<<<<<< HEAD
			trace_output = extract_arg(token, "file");
			if (!trace_output)
				/* Invalid argument */
				return -1;
=======
			if (strlen(token) > 5 && strncmp(token, "file=", 5) == 0) {
				trace_output = token + 5;
			} else {
				/* Invalid argument */
				return -1;
			}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

			token = strtok(NULL, ",");
			if (token != NULL)
				/* Only one argument allowed */
				return -1;
		}
<<<<<<< HEAD
		actions_add_trace_output(self, trace_output);
		break;
	case ACTION_SIGNAL:
		/* Takes two arguments, num (signal) and pid */
		while (token != NULL) {
			arg_value = extract_arg(token, "num");
			if (arg_value) {
				if (strtoi(arg_value, &signal))
					return -1;
			} else {
				arg_value = extract_arg(token, "pid");
				if (arg_value) {
					if (strncmp_static(arg_value, "parent") == 0)
						pid = -1;
					else if (strtoi(arg_value, &pid))
						return -1;
				} else {
					/* Invalid argument */
					return -1;
				}
=======
		return actions_add_trace_output(self, trace_output);
	case ACTION_SIGNAL:
		/* Takes two arguments, num (signal) and pid */
		while (token != NULL) {
			if (strlen(token) > 4 && strncmp(token, "num=", 4) == 0) {
				if (strtoi(token + 4, &signal))
					return -1;
			} else if (strlen(token) > 4 && strncmp(token, "pid=", 4) == 0) {
				if (strncmp(token + 4, "parent", 7) == 0)
					pid = -1;
				else if (strtoi(token + 4, &pid))
					return -1;
			} else {
				/* Invalid argument */
				return -1;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
			}

			token = strtok(NULL, ",");
		}

		if (!signal || !pid)
			/* Missing argument */
			return -1;

<<<<<<< HEAD
		actions_add_signal(self, signal, pid);
		break;
	case ACTION_SHELL:
		if (token == NULL)
			return -1;
		arg_value = extract_arg(token, "command");
		if (!arg_value)
			return -1;
		actions_add_shell(self, arg_value);
		break;
=======
		return actions_add_signal(self, signal, pid);
	case ACTION_SHELL:
		if (token == NULL)
			return -1;
		if (strlen(token) > 8 && strncmp(token, "command=", 8) == 0)
			return actions_add_shell(self, token + 8);
		return -1;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
	case ACTION_CONTINUE:
		/* Takes no argument */
		if (token != NULL)
			return -1;
<<<<<<< HEAD
		actions_add_continue(self);
		break;
	default:
		return -1;
	}

	return 0;
=======
		return actions_add_continue(self);
	default:
		return -1;
	}
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}

/*
 * actions_perform - perform all actions
 */
int
actions_perform(struct actions *self)
{
	int pid, retval;
	const struct action *action;

	for_each_action(self, action) {
		switch (action->type) {
		case ACTION_TRACE_OUTPUT:
			retval = save_trace_to_file(self->trace_output_inst, action->trace_output);
			if (retval) {
				err_msg("Error saving trace\n");
				return retval;
			}
			break;
		case ACTION_SIGNAL:
			if (action->pid == -1)
				pid = getppid();
			else
				pid = action->pid;
			retval = kill(pid, action->signal);
			if (retval) {
				err_msg("Error sending signal\n");
				return retval;
			}
			break;
		case ACTION_SHELL:
			retval = system(action->command);
			if (retval)
				return retval;
			break;
		case ACTION_CONTINUE:
			self->continue_flag = true;
			return 0;
		default:
			break;
		}
	}

	return 0;
}
