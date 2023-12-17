#ifndef ACTION_BAR_PROFILE_H
#define ACTION_BAR_PROFILE_H


#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "action_bar_entry.h"

class ClassProfile;

class ActionBarProfile : public Reference {
	GDCLASS(ActionBarProfile, Reference);

public:
	Ref<ClassProfile> get_owner();
	void set_owner(ClassProfile *owner);

	String get_action_bar_profile_name();
	void set_action_bar_profile_name(const String &value);
	Vector<Ref<ActionBarEntry>> &get_action_bars();
	void load_defaults();

	int get_action_bar_count() const;
	void add_action_bar(Ref<ActionBarEntry> actionbar);
	Ref<ActionBarEntry> get_action_bar(const int index);
	void remove_action_bar(const int index);
	void clear_action_bars();

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	void from_actionbar_profile(Ref<ActionBarProfile> other);

	void emit_change();

	ActionBarProfile();
	~ActionBarProfile();

protected:
	static void _bind_methods();

private:
	String _name;
	Vector<Ref<ActionBarEntry>> _action_bars;

	ClassProfile *_owner;
};

#endif
