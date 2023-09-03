/**************************************************************************/
/*  navigation_polygon_editor_plugin.cpp                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "navigation_polygon_editor_plugin.h"

#include "core/io/marshalls.h"
#include "core/io/resource_saver.h"
#include "core/object/undo_redo.h"
#include "editor/editor_node.h"
#include "scene/2d/mesh_instance_2d.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/label.h"
#include "scene/gui/option_button.h"
#include "scene/resources/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/navigation_polygon.h"
#include "servers/navigation/navigation_mesh_generator.h"
#include "scene/gui/separator.h"

Ref<NavigationPolygon> NavigationPolygonEditor::_ensure_navpoly() const {
	Ref<NavigationPolygon> navpoly = node->get_navigation_polygon();
	if (!navpoly.is_valid()) {
		navpoly = Ref<NavigationPolygon>(memnew(NavigationPolygon));
		node->set_navigation_polygon(navpoly);
	}
	return navpoly;
}

Node2D *NavigationPolygonEditor::_get_node() const {
	return node;
}

void NavigationPolygonEditor::_set_node(Node *p_polygon) {
	node = Object::cast_to<NavigationPolygonInstance>(p_polygon);
}

int NavigationPolygonEditor::_get_polygon_count() const {
	Ref<NavigationPolygon> navpoly = node->get_navigation_polygon();
	if (navpoly.is_valid()) {
		return navpoly->get_outline_count();
	} else {
		return 0;
	}
}

Variant NavigationPolygonEditor::_get_polygon(int p_idx) const {
	Ref<NavigationPolygon> navpoly = node->get_navigation_polygon();
	if (navpoly.is_valid()) {
		return navpoly->get_outline(p_idx);
	} else {
		return Variant(Vector<Vector2>());
	}
}

void NavigationPolygonEditor::_set_polygon(int p_idx, const Variant &p_polygon) const {
	Ref<NavigationPolygon> navpoly = _ensure_navpoly();
	navpoly->set_outline(p_idx, p_polygon);
}

void NavigationPolygonEditor::_action_add_polygon(const Variant &p_polygon) {
	Ref<NavigationPolygon> navpoly = _ensure_navpoly();
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action("Add Navigation Polygon");
	undo_redo->add_do_method(navpoly.ptr(), "add_outline", p_polygon);
	undo_redo->add_undo_method(navpoly.ptr(), "remove_outline", navpoly->get_outline_count());
	undo_redo->commit_action();
}

void NavigationPolygonEditor::_action_remove_polygon(int p_idx) {
	Ref<NavigationPolygon> navpoly = _ensure_navpoly();
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action("Remove Navigation Polygon");
	undo_redo->add_do_method(navpoly.ptr(), "remove_outline", p_idx);
	undo_redo->add_undo_method(navpoly.ptr(), "add_outline_at_index", navpoly->get_outline(p_idx), p_idx);
	undo_redo->commit_action();
}

void NavigationPolygonEditor::_action_set_polygon(int p_idx, const Variant &p_previous, const Variant &p_polygon) {
	Ref<NavigationPolygon> navpoly = _ensure_navpoly();
	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();
	undo_redo->create_action("Set Navigation Polygon");
	undo_redo->add_do_method(navpoly.ptr(), "set_outline", p_idx, p_polygon);
	undo_redo->add_undo_method(navpoly.ptr(), "set_outline", p_idx, p_previous);
	undo_redo->commit_action();
}

bool NavigationPolygonEditor::_has_resource() const {
	return node && node->get_navigation_polygon().is_valid();
}

void NavigationPolygonEditor::_create_resource() {
	if (!node) {
		return;
	}

	UndoRedo *undo_redo = EditorNode::get_singleton()->get_undo_redo();

	undo_redo->create_action(TTR("Create Navigation Polygon"));
	undo_redo->add_do_method(node, "set_navigation_polygon", Ref<NavigationPolygon>(memnew(NavigationPolygon)));
	undo_redo->add_undo_method(node, "set_navigation_polygon", Variant(Ref<NavigationPolygon>()));
	undo_redo->commit_action();

	_menu_option(MODE_CREATE);
}

NavigationPolygonEditor::NavigationPolygonEditor(EditorNode *p_editor, bool p_wip_destructive) :
		AbstractPolygon2DEditor(p_editor, p_wip_destructive) {
			
	bake_hbox = memnew(HBoxContainer);
	add_child(bake_hbox);

	bake_hbox->add_child(memnew(VSeparator));

	button_bake = memnew(Button);
	button_bake->set_flat(true);
	bake_hbox->add_child(button_bake);
	button_bake->set_toggle_mode(true);
	//button_bake->set_text(TTR("Bake NavigationPolygon"));
	button_bake->set_tooltip(TTR("Bake NavigationPolygon") + "\n" + TTR("Bakes the NavigationPolygon by first parsing the scene for source geometry and then creating the navigation polygon vertices and polygons."));
	button_bake->connect("pressed", this, "_bake_pressed");

	button_reset = memnew(Button);
	button_reset->set_flat(true);
	bake_hbox->add_child(button_reset);
	//button_reset->set_text(TTR("Clear NavigationPolygon"));
	button_reset->set_tooltip(TTR("Clear NavigationPolygon") + "\n" + TTR("Clears the internal NavigationPolygon outlines, vertices and polygons."));
	button_reset->connect("pressed", this, "_clear_pressed");

	//bake_info = memnew(Label);
	//bake_hbox->add_child(bake_info);

	err_dialog = memnew(AcceptDialog);
	add_child(err_dialog);
	node = nullptr;
}

NavigationPolygonEditor::~NavigationPolygonEditor() {
}

void NavigationPolygonEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			button_bake->set_icon(get_theme_icon("Bake", "EditorIcons"));
			button_reset->set_icon(get_theme_icon("Reload", "EditorIcons"));
		} break;
	}
}

void NavigationPolygonEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_bake_pressed"), &NavigationPolygonEditor::_bake_pressed);
	ClassDB::bind_method(D_METHOD("_clear_pressed"), &NavigationPolygonEditor::_clear_pressed);
}

void NavigationPolygonEditor::_bake_pressed() {
	button_bake->set_pressed(false);

	ERR_FAIL_COND(!node);
	Ref<NavigationPolygon> navigation_polygon = node->get_navigation_polygon();
	if (!navigation_polygon.is_valid()) {
		err_dialog->set_text(TTR("A NavigationPolygon resource must be set or created for this node to work."));
		err_dialog->popup_centered();
		return;
	}

	navigation_polygon->clear_polygons();
	navigation_polygon->set_vertices(PoolVector<Vector2>());

	Ref<NavigationMeshSourceGeometryData2D> source_geometry_data = NavigationMeshGenerator::get_singleton()->parse_2d_source_geometry_data(navigation_polygon, node);
	NavigationMeshGenerator::get_singleton()->bake_2d_from_source_geometry_data(navigation_polygon, source_geometry_data);

	node->update();
}

void NavigationPolygonEditor::_clear_pressed() {
	ERR_FAIL_COND(!node);
	if (!node->get_navigation_polygon().is_valid()) {
		return;
	}

	node->get_navigation_polygon()->clear();

	button_bake->set_pressed(false);
	//bake_info->set_text("");

	if (node) {
		node->update();
	}
}

void NavigationPolygonEditor::_update_polygon_editing_state() {
	if (!_get_node()) {
		return;
	}

	if (node != nullptr && node->get_navigation_polygon().is_valid()) {
		bake_hbox->show();
	} else {
		bake_hbox->hide();
	}
}

NavigationPolygonEditorPlugin::NavigationPolygonEditorPlugin(EditorNode *p_node) :
		AbstractPolygon2DEditorPlugin(p_node, memnew(NavigationPolygonEditor(p_node)), "NavigationPolygonInstance") {
}

NavigationPolygonEditorPlugin::~NavigationPolygonEditorPlugin() {
}

void NavigationPolygonEditorPlugin::_bind_methods() {
}