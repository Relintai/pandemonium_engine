#ifndef LOOT_DATA_BASE_H
#define LOOT_DATA_BASE_H


#include "core/variant/array.h"
#include "core/containers/vector.h"

#include "../items/item_template.h"

class LootDataBase : public Resource {
	GDCLASS(LootDataBase, Resource);

public:
	int get_loot_db_size() const;
	void set_loot_db_size(const int value);

	int get_items_size() const;
	void set_items_size(const int value);

	float get_loot_db_chance(const int index) const;
	void set_loot_db_chance(const int index, const float value);

	Ref<LootDataBase> get_loot_db(const int index) const;
	void set_loot_db(const int index, const Ref<LootDataBase> &value);

	float get_item_chance(const int index) const;
	void set_item_chance(const int index, const float value);

	Ref<ItemTemplate> get_item(const int index) const;
	void set_item(const int index, const Ref<ItemTemplate> &value);

	Array get_loot();
	Array _get_loot();

	LootDataBase();
	~LootDataBase();

protected:
	struct LootDBLDF {
		float chance;
		Ref<LootDataBase> loot_db;

		LootDBLDF() {
			chance = 0;
		}
	};

	struct LootDBItem {
		float chance;
		Ref<ItemTemplate> item;

		LootDBItem() {
			chance = 0;
		}
	};

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	Vector<LootDBLDF> _loot_dbs;
	Vector<LootDBItem> _items;
};

#endif
