#ifndef MODEL_VISUAL_H
#define MODEL_VISUAL_H

/*************************************************************************/
/*  model_visual.h                                                       */
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

#include "core/object/resource.h"

#include "../../item_enums.h"
#include "model_visual_entry.h"

class ModelVisual : public Resource {
	GDCLASS(ModelVisual, Resource);

public:
	int get_layer();
	void set_layer(int layer);

	//ModelVisualEntry
	Ref<ModelVisualEntry> get_visual_entry(const int index) const;
	void set_visual_entry(const int index, const Ref<ModelVisualEntry> visual_entry);
	void add_visual_entry(const Ref<ModelVisualEntry> visual_entry);
	void remove_visual_entry(const int index);

	int get_visual_entry_count() const;

	Vector<Variant> get_visual_entries();
	void set_visual_entries(const Vector<Variant> &visual_entries);

	ModelVisual();
	~ModelVisual();

protected:
#ifndef DISABLE_DEPRECATED
	// TODO REMOVE AFTER NEXT RELEASE
	bool _set(const StringName &p_name, const Variant &p_value);
#endif
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	int _layer;

	Vector<Ref<ModelVisualEntry>> _visual_entries;
};

#endif
