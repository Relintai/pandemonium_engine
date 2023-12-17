#ifndef PROFILE_MANAGER_H
#define PROFILE_MANAGER_H


#include "core/object/object.h"
#include "core/containers/vector.h"

#include "../profiles/player_profile.h"

class ProfileManager : public Object {
	GDCLASS(ProfileManager, Object);

public:
	static ProfileManager *get_singleton();

	bool get_automatic_load() const;
	void set_automatic_load(const bool load);

	bool get_automatic_save() const;
	void set_automatic_save(const bool load);

	String get_save_file() const;
	void set_save_file(const String &file);

	int gets_player_profile_count() const;
	Ref<PlayerProfile> gets_player_profile_index(const int index);
	void adds_player_profile(const Ref<PlayerProfile> &profile);
	void clears_player_profiles();
	void removes_player_profile(const int index);

	Ref<PlayerProfile> getc_player_profile();

	void save();
	void load();

	void _save();
	void _load();

	void save_profile(const String &name);
	void load_profile(const String &name);

	void load_defaults();

	Dictionary to_dict() const;
	void from_dict(const Dictionary &dict);

	void on_keybinds_changed(const StringName &class_path);

	ProfileManager();
	~ProfileManager();

protected:
	void _on_player_profile_changed(Ref<PlayerProfile> profile);
	static void _bind_methods();

private:
	static ProfileManager *_instance;

	bool _automatic_load;
	bool _automatic_save;

	String _save_file;

	Vector<Ref<PlayerProfile>> _s_player_profiles;
	Ref<PlayerProfile> _c_player_profile;
};

#endif
