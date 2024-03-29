/*************************************************************************/
/*  mm_create_name_popup.cpp                                             */
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

#include "mm_create_name_popup.h"

#include "../algos/mm_algos.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/tree.h"

void MMCreateNamePopup::about_to_show() {
	if (_initialized) {
		return;
	}

	_initialized = true;

	_tree->clear();
	TreeItem *root = _tree->create_item();

	for (int i = 0; i < MMAlgos::mm_node_registry.size(); ++i) {
		const MMAlgos::MMNodeRegistryCategory &category = MMAlgos::mm_node_registry[i];

		TreeItem *ti = _tree->create_item(root);
		ti->set_text(0, category.category_name);

		for (int j = 0; j < category.entries.size(); ++j) {
			const MMAlgos::MMNodeRegistryEntry &e = category.entries[j];

			TreeItem *ce = _tree->create_item(ti);

			if (e.type == MMAlgos::MMNODE_REGISTRY_TYPE_CLASS) {
				ce->set_text(0, e.data);
				ce->set_meta("node_type", static_cast<int>(MMAlgos::MMNODE_REGISTRY_TYPE_CLASS));
			} else if (e.type == MMAlgos::MMNODE_REGISTRY_TYPE_SCRIPT) {
				ce->set_text(0, e.data.get_file());
				ce->set_meta("node_type", static_cast<int>(MMAlgos::MMNODE_REGISTRY_TYPE_SCRIPT));
			}

			ce->set_meta("data", e.data);
		}
	}
}

void MMCreateNamePopup::_on_OK_pressed() {
	TreeItem *selected = _tree->get_selected();

	if (selected) {
		if (!selected->has_meta("node_type")) {
			hide();
			return;
		}

		int type = selected->get_meta("node_type");
		String data = selected->get_meta("data");

		emit_signal("ok_pressed", type, data);
	}

	hide();
}

MMCreateNamePopup::MMCreateNamePopup() {
	_initialized = false;

	set_anchors_and_margins_preset(LayoutPreset::PRESET_CENTER);
	set_size(Vector2(491, 440));
	set_title("Create New Resource");

	VBoxContainer *vboxcontainer = memnew(VBoxContainer);
	vboxcontainer->set_name("VBoxContainer");
	vboxcontainer->set_h_size_flags(SIZE_EXPAND_FILL);
	vboxcontainer->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(vboxcontainer);

	Label *label2_vboxcontainer = memnew(Label);
	label2_vboxcontainer->set_name("Label2");
	label2_vboxcontainer->set_h_size_flags(SIZE_EXPAND_FILL);
	label2_vboxcontainer->set_text("Type");
	vboxcontainer->add_child(label2_vboxcontainer);

	_tree = memnew(Tree);
	_tree->set_name("Tree");
	_tree->set_h_size_flags(SIZE_EXPAND_FILL);
	_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	_tree->set_hide_root(true);
	vboxcontainer->add_child(_tree);
}

MMCreateNamePopup::~MMCreateNamePopup() {
}

void MMCreateNamePopup::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		connect("confirmed", this, "_on_OK_pressed");
		connect("about_to_show", this, "about_to_show");
	}
}

void MMCreateNamePopup::_bind_methods() {
	ADD_SIGNAL(MethodInfo("ok_pressed", PropertyInfo(Variant::INT, "type"), PropertyInfo(Variant::STRING, "data")));

	ClassDB::bind_method(D_METHOD("about_to_show"), &MMCreateNamePopup::about_to_show);
	ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &MMCreateNamePopup::_on_OK_pressed);
}
