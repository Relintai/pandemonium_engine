

#include "entity_resource_speed.h"

#include "../../database/ess_resource_db.h"
#include "../../singletons/ess.h"
#include "../entity.h"
#include "entity_resource.h"

void EntityResourceSpeed::_init() {
	set_current_value(base_value);

	speed_stat_id = 0;

	ERR_FAIL_COND(!ESS::get_singleton());

	if (ESS::get_singleton()->stat_is_property("Speed"))
		speed_stat_id = ESS::get_singleton()->stat_get_id("Speed");
}
void EntityResourceSpeed::_ons_added(Node *entity) {
	refresh();
}
void EntityResourceSpeed::_notification_sstat_changed(int statid, float current) {
	if (statid == speed_stat_id)
		refresh();
}
void EntityResourceSpeed::refresh() {
	ERR_FAIL_COND(get_owner() == NULL);

	float speed_stat = get_owner()->stat_gets_current(speed_stat_id);

	set_max_value(base_value + speed_stat * 0.01);
	set_current_value(base_value + speed_stat * 0.01);
}

void EntityResourceSpeed::resolve_references() {
}

EntityResourceSpeed::EntityResourceSpeed() {
	speed_stat_id = 0;
	base_value = 100;
}

EntityResourceSpeed::~EntityResourceSpeed() {
}

void EntityResourceSpeed::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_init"), &EntityResourceSpeed::_init);
	ClassDB::bind_method(D_METHOD("_ons_added", "entity"), &EntityResourceSpeed::_ons_added);
	ClassDB::bind_method(D_METHOD("_notification_sstat_changed", "statid", "current"), &EntityResourceSpeed::_notification_sstat_changed);
	ClassDB::bind_method(D_METHOD("refresh"), &EntityResourceSpeed::refresh);
}
