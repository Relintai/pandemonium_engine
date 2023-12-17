#ifndef PROP_DATA_ENTITY_H
#define PROP_DATA_ENTITY_H


#include "modules/modules_enabled.gen.h"

#ifdef MODULE_PROPS_ENABLED

#include "../../props/props/prop_data_entry.h"

#include "../entities/data/entity_data.h"

class PropDataEntity : public PropDataEntry {
	GDCLASS(PropDataEntity, PropDataEntry);

public:
	Ref<EntityData> get_entity_data() const;
	void set_entity_data(const Ref<EntityData> &value);

	int get_level() const;
	void set_level(const int value);

	PropDataEntity();
	~PropDataEntity();

protected:
	static void _bind_methods();

private:
	int _level;
	Ref<EntityData> _entity_data;
};

#endif

#endif
