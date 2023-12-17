/*************************************************************************/
/*  gradient_popup.cpp                                                   */
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

#include "gradient_popup.h"

#include "core/object/object.h"
#include "gradient_editor.h"

#include "../../../nodes/bases/gradient_base.h"
#include "../../../nodes/mm_node_universal_property.h"
#include "scene/gui/panel.h"

void GradientPopup::init(const Ref<GradientBase> &value, MMGraphNode *graph_node, UndoRedo *undo_redo) {
	_gradient_editor->set_undo_redo(undo_redo);
	_gradient_editor->set_graph_node(graph_node);
	_gradient_editor->set_value(value);
}

void GradientPopup::_on_Control_updated(const Ref<GradientBase> &value) {
	emit_signal("updated", value);
}

void GradientPopup::_on_GradientPopup_popup_hide() {
	queue_delete();
}

GradientPopup::GradientPopup() {
	set_h_size_flags(0);
	set_v_size_flags(0);

	Panel *panel_gradientpopup = memnew(Panel);
	add_child(panel_gradientpopup);

	Ref<StyleBoxFlat> panel_gradientpopup_prop_custom_styles;
	panel_gradientpopup_prop_custom_styles.instance();
	panel_gradientpopup->set("custom_styles/panel", panel_gradientpopup_prop_custom_styles);

	_gradient_editor = memnew(MMGradientEditor);
	panel_gradientpopup->add_child(_gradient_editor);
}

GradientPopup::~GradientPopup() {
}

void GradientPopup::_bind_methods() {
	ADD_SIGNAL(MethodInfo("updated", PropertyInfo(Variant::OBJECT, "value", PROPERTY_HINT_RESOURCE_TYPE, "GradientBase")));

	//ClassDB::bind_method(D_METHOD("init", "value", " graph_node", " undo_redo"), &GradientPopup::init);
	ClassDB::bind_method(D_METHOD("_on_Control_updated", "value"), &GradientPopup::_on_Control_updated);
	ClassDB::bind_method(D_METHOD("_on_GradientPopup_popup_hide"), &GradientPopup::_on_GradientPopup_popup_hide);
}
