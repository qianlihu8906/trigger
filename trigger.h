#ifndef TRIGGER_H__
#define TRIGGER_H__

enum{
	TRIGGER_BOOL_HTOL,
	TRIGGER_BOOL_LTOH,
	TRIGGER_BOOL_STILL_HIGH,
	TRIGGER_BOOL_STILL_LOW,
};

enum{
	TRIGGER_THRESHOLD_OVER_MAX,
	TRIGGER_THRESHOLD_OVER_MIN,
	TRIGGER_THRESHOLD_TO_NORMAL,
	TRIGGER_THRESHOLD_STILL_OVER_MAX,
	TRIGGER_THRESHOLD_STILL_OVER_MIN,
	TRIGGER_THRESHOLD_STILL_NORMAL,
};

struct trigger_bool;
struct trigger_threshold;
typedef void (*trigger_action)(void *d);

struct trigger_bool* trigger_bool_create(const char *name,char init);
void trigger_bool_destroy(struct trigger_bool *ab);
void trigger_bool_set_action(struct trigger_bool *ab,trigger_action htol,trigger_action ltoh,
		trigger_action still_high,trigger_action still_low);
const char* trigger_bool_get_name(struct trigger_bool *ab);
int trigger_bool_check(struct trigger_bool *ab,char v);

struct trigger_threshold* trigger_threshold_create(const char *name,float init,
		              float max,float min);
void trigger_threshold_destroy(struct trigger_threshold *at);
void trigger_threshold_set_action(struct trigger_threshold *at,trigger_action over_max,
	trigger_action over_min,trigger_action to_normal,
	trigger_action still_max,trigger_action still_min,trigger_action still_normal);
const char* trigger_threshold_get_name(struct trigger_threshold *at);
float trigger_threshold_get_max(struct trigger_threshold *at);
float trigger_threshold_get_min(struct trigger_threshold *at);
float trigger_threshold_get_current(struct trigger_threshold *at);
int trigger_threshold_check(struct trigger_threshold *at,float v);

#endif
