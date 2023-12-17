#ifndef VENDOR_ITEM_DATA_ENTRY_H
#define VENDOR_ITEM_DATA_ENTRY_H


#include "core/object/resource.h"

class ItemTemplate;
class Spell;

class VendorItemDataEntry : public Resource {
	GDCLASS(VendorItemDataEntry, Resource);

public:
	Ref<ItemTemplate> get_item();
	void set_item(const Ref<ItemTemplate> &item);

	Ref<Spell> get_spell();
	void set_spell(const Ref<Spell> &spell);

	Ref<ItemTemplate> get_cost_item();
	void set_cost_item(const Ref<ItemTemplate> &item);

	int get_price() const;
	void set_price(const int value);

	int get_count() const;
	void set_count(const int value);

	int get_spawn_time() const;
	void set_spawn_time(const int time);

	VendorItemDataEntry();
	~VendorItemDataEntry();

protected:
	static void _bind_methods();

private:
	Ref<ItemTemplate> _item;
	Ref<Spell> _spell;
	Ref<ItemTemplate> _cost_item;

	int _price;
	int _count;
	int _spawn_time;
};

#endif
