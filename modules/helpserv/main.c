/*
 * Copyright (c) 2005 Atheme Development Group
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains the main() routine.
 */

#include "atheme.h"

service_t *helpserv;

static void helpserv_cmd_help(sourceinfo_t *si, const int parc, char *parv[]);

command_t helpserv_help = { "HELP", N_(N_("Displays contextual help information.")), AC_NONE, 2, helpserv_cmd_help, { .path = "help" } };

/* HELP <command> [params] */
void helpserv_cmd_help(sourceinfo_t *si, int parc, char *parv[])
{
	char *command = parv[0];

	if (!command)
	{
		command_success_nodata(si, _("***** \2%s Help\2 *****"), si->service->nick);
		command_success_nodata(si, _("\2%s\2 allows users to request help from network staff."), si->service->nick);
		command_success_nodata(si, " ");
		command_success_nodata(si, _("For more information on a command, type:"));
		command_success_nodata(si, "\2/%s%s help <command>\2", (ircd->uses_rcommand == false) ? "msg " : "", helpserv->disp);
		command_success_nodata(si, " ");

		command_help(si, si->service->commands);

		command_success_nodata(si, _("***** \2End of Help\2 *****"));
		return;
	}

	/* take the command through the hash table */
	help_display(si, si->service, command, si->service->commands);
}

static void
mod_init(module_t *const restrict m)
{
	helpserv = service_add("helpserv", NULL);

	service_bind_command(helpserv, &helpserv_help);
}

static void
mod_deinit(const module_unload_intent_t intent)
{
	service_unbind_command(helpserv, &helpserv_help);

	if (helpserv)
	{
		service_delete(helpserv);
		helpserv = NULL;
	}
}

SIMPLE_DECLARE_MODULE_V1("helpserv/main", MODULE_UNLOAD_CAPABILITY_OK)
