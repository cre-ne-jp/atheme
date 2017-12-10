/*
 * Copyright (c) 2011 William Pitcock <nenolod@dereferenced.org>
 *
 * Rights to this code are as documented in doc/LICENSE.
 */

#include "atheme.h"
#include "exttarget.h"

static mowgli_patricia_t **exttarget_tree = NULL;

static myentity_t dummy_entity;

static chanacs_t *dummy_match_user(chanacs_t *ca, user_t *u)
{
	if (is_ircop(u))
		return ca;

	return NULL;
}

static chanacs_t *dummy_match_entity(chanacs_t *ca, myentity_t *mt)
{
	if (ca->entity == mt)
		return ca;

	return NULL;
}

static bool dummy_can_register_channel(myentity_t *mt)
{
	return false;
}

static bool dummy_allow_foundership(myentity_t *mt)
{
	return false;
}

static entity_chanacs_validation_vtable_t dummy_validate = {
	.match_entity = dummy_match_entity,
	.match_user = dummy_match_user,
	.can_register_channel = dummy_can_register_channel,
	.allow_foundership = dummy_allow_foundership,
};

static myentity_t dummy_entity = {
	.name = "$oper",
	.type = ENT_EXTTARGET,
	.chanacs_validate = &dummy_validate,
};

static myentity_t *oper_validate_f(const char *param)
{
	return &dummy_entity;
}

static void
mod_init(module_t *const restrict m)
{
	MODULE_TRY_REQUEST_SYMBOL(m, exttarget_tree, "exttarget/main", "exttarget_tree");

	mowgli_patricia_add(*exttarget_tree, "oper", oper_validate_f);

	object_init(object(&dummy_entity), "$oper", NULL);
}

static void
mod_deinit(const module_unload_intent_t intent)
{
	mowgli_patricia_delete(*exttarget_tree, "oper");
}

SIMPLE_DECLARE_MODULE_V1("exttarget/oper", MODULE_UNLOAD_CAPABILITY_OK)
