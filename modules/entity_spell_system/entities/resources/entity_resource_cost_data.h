#ifndef ENTITY_RESOURCE_COST_DATA_H
#define ENTITY_RESOURCE_COST_DATA_H


#include "core/object/resource.h"

#include "entity_resource.h"
#include "entity_resource_cost_data.h"

class EntityResourceCostData : public Resource {
	GDCLASS(EntityResourceCostData, Resource);

public:
	int get_cost();
	void set_cost(int value);

	EntityResourceCostData();

protected:
	static void _bind_methods();

private:
	int _cost;
};

#endif
