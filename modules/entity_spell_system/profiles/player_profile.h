#ifndef PLAYER_PROFILE_H
#define PLAYER_PROFILE_H


#include "core/variant/dictionary.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"

#include "class_profile.h"

class PlayerProfile : public Reference {
	GDCLASS(PlayerProfile, Reference);

public:
	static const String DEFAULT_PROFILE_FILE_NAME;

	int get_id();
	void set_id(int value);

	String get_profile_name();
	void set_profile_name(String value);

	int get_last_used_character() const;
	void set_last_used_character(const int value);

	int get_class_profile_count() const;
	Ref<ClassProfile> get_class_profile_index(const int index);
	void add_class_profile(Ref<ClassProfile> profile);
	void clear_class_profiles();
	void remove_class_profile(const int index);

	Vector<Ref<ClassProfile>> &get_class_profiles();
	Ref<ClassProfile> get_class_profile(const StringName &class_path);

	Dictionary get_custom_data();
	void set_custom_data(const Dictionary &dict);

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	void emit_change();

	PlayerProfile();
	~PlayerProfile();

	void load_defaults();

protected:
	void _on_class_profile_changed();

	static void _bind_methods();

private:
	int _id;
	int _last_used_character;
	String _profile_name;

	Dictionary _custom_data;

	Vector<Ref<ClassProfile>> _class_profiles;
};

#endif
