#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "trigger.h"

struct trigger_bool{
	char name[64];
	char previous;

	trigger_action htol;
	trigger_action ltoh;
	trigger_action still_high;
	trigger_action still_low;

};

struct trigger_bool* trigger_bool_create(const char *name,char init)
{
	struct trigger_bool *ab = malloc(sizeof(*ab));
	assert(ab != NULL);
	memset(ab,0,sizeof(*ab));

	strncpy(ab->name,name,sizeof(ab->name)-1);
	ab->previous = init;

	return ab;
}

void trigger_bool_destroy(struct trigger_bool *ab)
{
	free(ab);
}

const char* trigger_bool_get_name(struct trigger_bool *ab)
{
	return ab->name;
}

void trigger_bool_set_action(struct trigger_bool *ab,trigger_action htol,
	trigger_action ltoh,trigger_action still_high,trigger_action still_low)
{
	ab->htol = htol;
	ab->ltoh = ltoh;
	ab->still_high = still_high;
	ab->still_low = still_low;
}

int trigger_bool_check(struct trigger_bool *ab,char v)
{
	if(ab->previous == v){
	       	if(v == 0){ 
			if(ab->still_low != NULL)
				ab->still_low(ab);
			return TRIGGER_BOOL_STILL_LOW;
		}
		if(ab->still_high != NULL)
			ab->still_high(ab);
		return TRIGGER_BOOL_STILL_HIGH;
	}

	ab->previous = v;
	if(v == 0){
		if(ab->htol != NULL)
			ab->htol(ab);
		return TRIGGER_BOOL_HTOL;
	}

	if(ab->ltoh != NULL)
		ab->ltoh(ab);

	return TRIGGER_BOOL_LTOH;
}

struct trigger_threshold {
	char name[64];
	float current;
	float max;
	float min;

	trigger_action over_max;
	trigger_action over_min;
	trigger_action to_normal;
	trigger_action still_over_max;
	trigger_action still_over_min;
	trigger_action still_normal;

	char already_over_max;
	char already_over_min;
};

struct trigger_threshold *trigger_threshold_create(const char *name,float init,float max,float min)
{
	struct trigger_threshold *at = malloc(sizeof(*at));
	assert(at != NULL);
	memset(at,0,sizeof(*at));

	strncpy(at->name,name,sizeof(at->name)-1);
	at->current = init;
	at->max = max;
	at->min = min;

	return at;
}

void trigger_threshold_destroy(struct trigger_threshold *at)
{
	free(at);
}

const char *trigger_threshold_get_name(struct trigger_threshold *at)
{
	return at->name;
}

float trigger_threshold_get_current(struct trigger_threshold *at)
{
	return at->current;
}

float trigger_threshold_get_max(struct trigger_threshold *at)
{
	return at->max;
}

float trigger_threshold_get_min(struct trigger_threshold *at)
{
	return at->min;
}

void trigger_threshold_set_action(struct trigger_threshold *at,trigger_action over_max,
	trigger_action over_min,trigger_action to_normal,
	trigger_action still_max,trigger_action still_min,trigger_action still_normal)
{
	at->over_max = over_max;
	at->over_min = over_min;
	at->to_normal = to_normal;

	at->still_normal = still_normal;
	at->still_over_max = still_max;
	at->still_over_min = still_min;
}

int trigger_threshold_check(struct trigger_threshold *at,float value)
{
	if(value > at->max){
		at->already_over_min = 0;
		if(at->already_over_max){
			if(at->still_over_max != NULL)
				at->still_over_max(at);
			return TRIGGER_THRESHOLD_STILL_OVER_MAX;
		}
		if(at->over_max != NULL)
			at->over_max(at);
		at->already_over_max = 1;
		return TRIGGER_THRESHOLD_OVER_MAX;
	}

	if(value < at->min){
		at->already_over_max = 0;
		if(at->already_over_min){
			if(at->still_over_min != NULL)
				at->still_over_min(at);
			return TRIGGER_THRESHOLD_STILL_OVER_MIN;
		}
		if(at->over_min != NULL)
			at->over_min(at);
		at->already_over_min = 1;
		return TRIGGER_THRESHOLD_OVER_MIN;
		
	}

	if(at->already_over_min || at->already_over_max){
		if(at->to_normal != NULL)
			at->to_normal(at);
		at->already_over_max = 0;
		at->already_over_min = 0;
		return TRIGGER_THRESHOLD_TO_NORMAL;
	}

	at->already_over_max = 0;
	at->already_over_min = 0;
	if(at->still_normal != NULL)
		at->still_normal(at);
	return TRIGGER_THRESHOLD_STILL_NORMAL;
}
