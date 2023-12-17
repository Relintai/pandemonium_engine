#ifndef ENTITY_RESOURCE_COST_DATA_HEALTH_H
#define ENTITY_RESOURCE_COST_DATA_HEALTH_H


#include "core/object/resource.h"

#include "entity_resource.h"
#include "entity_resource_cost_data.h"

class EntityResourceCostDataHealth : public EntityResourceCostData {
	GDCLASS(EntityResourceCostDataHealth, EntityResourceCostData);

public:
	EntityResourceCostDataHealth();

protected:
	static void _bind_methods();
};

#endif
