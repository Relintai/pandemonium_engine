#ifndef ACTION_BAR_ENTRY_H
#define ACTION_BAR_ENTRY_H


#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

#include "action_bar_button_entry.h"

class ActionBarProfile;

class ActionBarEntry : public Reference {
	GDCLASS(ActionBarEntry, Reference);

public:
	Ref<ActionBarProfile> get_owner();
	void set_owner(ActionBarProfile *owner);

	float get_size() const;
	void set_size(const float value);

	int get_action_bar_id() const;
	void set_action_bar_id(const int value);

	int get_slot_num() const;
	void set_slot_num(const int value);

	int get_action_bar_entry_count() const;

	void emit_change();

	Ref<ActionBarButtonEntry> get_button_for_slotid(const int slotId);
	Ref<ActionBarButtonEntry> get_button(const int index);

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	ActionBarEntry();
	~ActionBarEntry();

protected:
	static void _bind_methods();

private:
	int _action_bar_id;
	int _slot_num;
	Vector<Ref<ActionBarButtonEntry>> _button_entries;

	float _size;

	ActionBarProfile *_owner;
};

#endif
