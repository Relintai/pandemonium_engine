#ifndef ACTION_BAR_ENTRY_H
#define ACTION_BAR_ENTRY_H

/*************************************************************************/
/*  action_bar_entry.h                                                   */
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
#include "core/variant/array.h"
#include "core/variant/dictionary.h"

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
