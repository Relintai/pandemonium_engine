#ifndef ENTITY_RESOURCE_COST_DATA_RESOURCE_H
#define ENTITY_RESOURCE_COST_DATA_RESOURCE_H


#include "core/object/resource.h"

#include "entity_resource.h"
#include "entity_resource_cost_data.h"

class EntityResourceCostDataResource : public EntityResourceCostData {
	GDCLASS(EntityResourceCostDataResource, EntityResourceCostData);

public:
	Ref<EntityResource> get_entity_resource_data();
	void set_entity_resource_data(Ref<EntityResource> data);

	EntityResourceCostDataResource();

protected:
	static void _bind_methods();

private:
	Ref<EntityResource> _entity_resource_data;
};

#endif
