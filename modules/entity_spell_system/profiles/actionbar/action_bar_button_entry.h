#ifndef ACTION_BAR_BUTTON_ENTRY_H
#define ACTION_BAR_BUTTON_ENTRY_H


#include "core/variant/dictionary.h"
#include "core/object/reference.h"

class ActionBarEntry;

class ActionBarButtonEntry : public Reference {
	GDCLASS(ActionBarButtonEntry, Reference);

public:
	static const String BINDING_STRING_ACTIONBAR_BUTTON_ENTRY_TYPE;

	enum ActionBarButtonEntryType {
		ACTION_BAR_BUTTON_ENTRY_TYPE_NONE,
		ACTION_BAR_BUTTON_ENTRY_TYPE_SPELL,
		ACTION_BAR_BUTTON_ENTRY_TYPE_ITEM
	};

	Ref<ActionBarEntry> get_owner();
	void set_owner(ActionBarEntry *owner);

	int get_action_bar_id() const;
	void set_action_bar_id(const int value);

	int get_slot_id() const;
	void set_slot_id(int value);

	ActionBarButtonEntryType get_type() const;
	void set_type(const ActionBarButtonEntryType value);

	StringName get_item_path() const;
	void set_item_path(const StringName &value);

	void emit_change();

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	ActionBarButtonEntry();
	ActionBarButtonEntry(const int actionBarId, const int slotId);
	ActionBarButtonEntry(const int actionBarId, const int slotId, const ActionBarButtonEntryType type, const StringName &item_path);
	~ActionBarButtonEntry();

protected:
	static void _bind_methods();

private:
	int _action_bar_id;
	int _slot_id;
	ActionBarButtonEntryType _type;
	StringName _item_path;

	ActionBarEntry *_owner;
};

VARIANT_ENUM_CAST(ActionBarButtonEntry::ActionBarButtonEntryType);

#endif
