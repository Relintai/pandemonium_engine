#ifndef ENTITY_RESOURCE_HEALTH_H
#define ENTITY_RESOURCE_HEALTH_H


#include "entity_resource.h"

class Entity;

class EntityResourceHealth : public EntityResource {
	GDCLASS(EntityResourceHealth, EntityResource);

public:
	void _init();
	void _ons_added(Node *entity);
	void _notification_sstat_changed(int statid, float current);
	void refresh();

	void resolve_references();

	EntityResourceHealth();
	~EntityResourceHealth();

protected:
	static void _bind_methods();

private:
	int stamina_stat_id;
	int health_stat_id;
};

#endif
