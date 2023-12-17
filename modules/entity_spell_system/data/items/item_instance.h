#ifndef ITEM_INSTANCE_H
#define ITEM_INSTANCE_H


#include "core/object/resource.h"
#include "core/containers/vector.h"

#include "../../item_enums.h"

class ItemTemplate;

class ItemInstance : public Resource {
	GDCLASS(ItemInstance, Resource);

public:
	Ref<ItemTemplate> get_item_template();
	void set_item_template(const Ref<ItemTemplate> &value);

	//Modifiers
	int stat_modifier_get_stat_id(const int index) const;
	void stat_modifier_set_stat_id(const int index, const int value);

	float stat_modifier_get_base_mod(const int index) const;
	void stat_modifier_set_base_mod(const int index, const float value);

	float stat_modifier_get_bonus_mod(const int index) const;
	void stat_modifier_set_bonus_mod(const int index, const float value);

	float stat_modifier_get_percent_mod(const int index) const;
	void stat_modifier_set_percent_mod(const int index, const float value);

	void add_item_stat_modifier(const int stat_id, const int base_mod, const int bonus_mod, const int percent_mod);
	void remove_item_stat_modifier(const int index);
	void clear_item_stat_modifiers();

	int stat_modifier_get_count() const;

	Vector<Variant> stat_modifiers_get();
	void stat_modifiers_set(const Vector<Variant> &mods);

	int get_stack_size() const;
	void set_stack_size(const int value);

	int get_charges() const;
	void set_charges(const int value);

	String get_description();

	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	virtual Dictionary _to_dict();
	virtual void _from_dict(const Dictionary &dict);

	ItemInstance();
	~ItemInstance();

protected:
	static void _bind_methods();

protected:
	struct ItemStatModifier {
		int stat_id;
		float base_mod;
		float bonus_mod;
		float percent_mod;

		ItemStatModifier() {
			stat_id = 0;
			base_mod = 0;
			bonus_mod = 0;
			percent_mod = 0;
		}
	};

private:
	Ref<ItemTemplate> _item_template;
	StringName _item_template_path;

	int _stack_size;
	int _charges;

	Vector<ItemStatModifier> _modifiers;
};

#endif
