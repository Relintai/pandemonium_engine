#ifndef ENTITY_RESOURCE_SPEED_H
#define ENTITY_RESOURCE_SPEED_H


#include "entity_resource.h"

class Entity;

class EntityResourceSpeed : public EntityResource {
	GDCLASS(EntityResourceSpeed, EntityResource);

public:
	void _init();
	void _ons_added(Node *entity);
	void _notification_sstat_changed(int statid, float current);
	void refresh();

	void resolve_references();

	EntityResourceSpeed();
	~EntityResourceSpeed();

protected:
	static void _bind_methods();

private:
	int speed_stat_id;
	int base_value;
};

#endif
