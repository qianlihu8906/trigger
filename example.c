#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trigger.h"

static void trigger_bool_htol(void *d)
{
	struct trigger_bool *ab = d;
	const char *name = trigger_bool_get_name(ab);
	printf("%s:active low\n",name);
}

static void trigger_bool_ltoh(void *d)
{
	struct trigger_bool *ab = d;
	const char *name = trigger_bool_get_name(ab);
	printf("%s:active high\n",name);
}

static void trigger_threshold_over_max(void *d)
{
	struct trigger_threshold *at = d;
	const char *name = trigger_threshold_get_name(at);
	float current = trigger_threshold_get_current(at);
	float max = trigger_threshold_get_max(at);
	float min = trigger_threshold_get_min(at);
	printf("%s: over max (current=%4.2f,max=%4.2f,min=%4.2f)\n",name,current,max,min);
}

static void trigger_threshold_over_min(void *d)
{
	struct trigger_threshold *at = d;
	const char *name = trigger_threshold_get_name(at);
	float current = trigger_threshold_get_current(at);
	float max = trigger_threshold_get_max(at);
	float min = trigger_threshold_get_min(at);
	printf("%s: over min (current=%4.2f,max=%4.2f,min=%4.2f)\n",name,current,max,min);
}

static void trigger_threshold_to_normal(void *d)
{
	struct trigger_threshold *at = d;
	const char *name = trigger_threshold_get_name(at);
	float current = trigger_threshold_get_current(at);
	float max = trigger_threshold_get_max(at);
	float min = trigger_threshold_get_min(at);
	printf("%s: normal (current=%4.2f,max=%4.2f,min=%4.2f)\n",name,current,max,min);
}

int main()
{
	struct trigger_bool *ab = trigger_bool_create("fan",0);
	trigger_bool_set_action(ab,trigger_bool_htol,trigger_bool_ltoh,NULL,NULL);
	trigger_bool_check(ab,1);
	trigger_bool_check(ab,0);
	trigger_bool_check(ab,1);
	trigger_bool_check(ab,0);

	struct trigger_threshold *at = trigger_threshold_create("temperature",45,60,30);
	trigger_threshold_set_action(at,trigger_threshold_over_max,trigger_threshold_over_min,trigger_threshold_to_normal,NULL,NULL,NULL);

	trigger_threshold_check(at,45);
	trigger_threshold_check(at,61);
	trigger_threshold_check(at,45);
	trigger_threshold_check(at,29);

	return 0;
}
