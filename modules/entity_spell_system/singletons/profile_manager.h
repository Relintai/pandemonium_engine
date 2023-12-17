#ifndef PROFILE_MANAGER_H
#define PROFILE_MANAGER_H

/*************************************************************************/
/*  profile_manager.h                                                    */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/containers/vector.h"
#include "core/object/object.h"

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
