#ifndef EQUIPMENT_DATA_H
#define EQUIPMENT_DATA_H


#include "core/object/resource.h"

class ItemInstance;
class ItemTemplate;

class EquipmentData : public Resource {
	GDCLASS(EquipmentData, Resource);

public:
	Ref<ItemTemplate> get_slot(int index);
	void set_slot(int index, Ref<ItemTemplate> entry);

	Ref<ItemInstance> get_item(int index);

	EquipmentData();
	~EquipmentData();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	Vector<Ref<ItemTemplate>> _entries;
};

#endif
