#ifndef ENTITY_DATA_CONTAINER_H
#define ENTITY_DATA_CONTAINER_H


#include "core/variant/dictionary.h"
#include "core/object/resource.h"

class EntityDataContainer : public Resource {
	GDCLASS(EntityDataContainer, Resource);

public:
	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	Dictionary _to_dict();
	void _from_dict(const Dictionary &dict);

	EntityDataContainer();
	~EntityDataContainer();

protected:
	static void _bind_methods();
};

#endif
