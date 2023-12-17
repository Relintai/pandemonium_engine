#ifndef ACTION_BAR_PROFILE_H
#define ACTION_BAR_PROFILE_H

/*************************************************************************/
/*  action_bar_profile.h                                                 */
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
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"

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
