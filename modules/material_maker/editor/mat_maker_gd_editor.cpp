
#include "mat_maker_gd_editor.h"

#include "../algos/mm_algos.h"

#include "core/io/resource_loader.h"
#include "core/undo_redo.h"
#include "modules/material_maker/editor/mm_create_name_popup.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/control.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/panel_container.h"

#include "../nodes/mm_material.h"
#include "../nodes/mm_node.h"
#include "../nodes/mm_node_universal_property.h"
#include "mm_graph_node.h"

#include "editor/editor_plugin.h"

GraphEdit *MatMakerGDEditor::get_graph_edit() {
	return _graph_edit;
}

Ref<MMMaterial> MatMakerGDEditor::get_mm_material() {
	return _material;
}

void MatMakerGDEditor::set_mm_material(const Ref<MMMaterial> &object) {
	if (_material.is_valid()) {
		_material->disconnect("changed", this, "on_material_changed");
	}

	_material = object;
	recreate();

	if (_material.is_valid()) {
		_material->connect("changed", this, "on_material_changed");
	}
}

int MatMakerGDEditor::get_ignore_material_change_event() const {
	return _ignore_material_change_event;
}

void MatMakerGDEditor::set_ignore_material_change_event(const int val) {
	_ignore_material_change_event = val;
}

void MatMakerGDEditor::ignore_changes(const bool val) {
	if (val) {
		_ignore_material_change_event += 1;
	}

	else {
		_ignore_material_change_event -= 1;
	}
}

bool MatMakerGDEditor::get_recreation_in_progress() const {
	return _recreation_in_progress;
}

void MatMakerGDEditor::set_recreation_in_progress(const bool val) {
	_recreation_in_progress = val;
}

UndoRedo *MatMakerGDEditor::get_undo_redo() {
	if (!_undo_redo) {
		_undo_redo = memnew(UndoRedo);
	}

	return _undo_redo;
}

void MatMakerGDEditor::set_undo_redo(UndoRedo *ur) {
	_undo_redo = ur;
}

void MatMakerGDEditor::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_INT, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_VECTOR2, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_VECTOR3, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_COLOR, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_INT);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_VECTOR2);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_VECTOR3);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_COLOR);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL, MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE, MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_INT, MMNodeUniversalProperty::SLOT_TYPE_INT);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_FLOAT, MMNodeUniversalProperty::SLOT_TYPE_FLOAT);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_VECTOR2, MMNodeUniversalProperty::SLOT_TYPE_VECTOR2);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_VECTOR3, MMNodeUniversalProperty::SLOT_TYPE_VECTOR3);
		_graph_edit->add_valid_connection_type(MMNodeUniversalProperty::SLOT_TYPE_COLOR, MMNodeUniversalProperty::SLOT_TYPE_COLOR);

		_graph_edit->connect("connection_request", this, "on_graph_edit_connection_request");
		_graph_edit->connect("disconnection_request", this, "on_graph_edit_disconnection_request");
	}
}

void MatMakerGDEditor::recreate() {
	ignore_changes(true);

	if (_recreation_in_progress) {
		return;
	}

	_recreation_in_progress = true;
	_graph_edit->clear_connections();

	for (int i = 0; i < _graph_edit->get_child_count(); ++i) {
		MMGraphNode *c = Object::cast_to<MMGraphNode>(_graph_edit->get_child(i));

		if (c) {
			_graph_edit->remove_child(c);
			c->queue_delete();
		}
	}

	if (!_material.is_valid()) {
		return;
	}

	_material->cancel_render_and_wait();

	for (int i = 0; i < _material->nodes.size(); ++i) {
		Ref<MMNode> n = _material->nodes[i];

		MMGraphNode *gn = memnew(MMGraphNode);
		gn->set_editor_node(this);
		gn->set_node(_material, n);
		_graph_edit->add_child(gn);
	}

	//connect them;

	for (int i = 0; i < _material->nodes.size(); ++i) {
		Ref<MMNode> n = _material->nodes[i];

		if (n.is_valid()) {
			for (int j = 0; j < n->input_properties.size(); ++j) {
				Ref<MMNodeUniversalProperty> ip = n->input_properties[j];

				if (ip.is_valid()) {
					MMGraphNode *input_node = find_graph_node_for(n);
					MMGraphNode *output_node = find_graph_node_for(ip->get_input_property()->get_owner());

					ERR_CONTINUE(!input_node);
					ERR_CONTINUE(!output_node);

					int to_slot = input_node->get_input_property_graph_node_slot_index(ip);
					int from_slot = output_node->get_output_property_graph_node_slot_index(ip->get_input_property());
					_graph_edit->connect_node(output_node->get_name(), from_slot, input_node->get_name(), to_slot);
				}
			}
		}
	}

	_material->render();
	_recreation_in_progress = false;
	ignore_changes(false);
}

MMGraphNode *MatMakerGDEditor::find_graph_node_for(const Ref<MMNode> &nnode) {
	for (int i = 0; i < _graph_edit->get_child_count(); ++i) {
		MMGraphNode *c = Object::cast_to<MMGraphNode>(_graph_edit->get_child(i));

		if (c) {
			if (c->has_method("get_material_node")) {
				Ref<MMNode> n = c->get_material_node();

				if (n == nnode) {
					return c;
				}
			}
		}
	}

	return nullptr;
}

void MatMakerGDEditor::on_material_changed() {
	if (_ignore_material_change_event > 0) {
		return;
	}

	if (_recreation_in_progress) {
		return;
	}

	call_deferred("recreate");
}

void MatMakerGDEditor::on_graph_edit_connection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	MMGraphNode *from_node = Object::cast_to<MMGraphNode>(_graph_edit->get_node(from));
	MMGraphNode *to_node = Object::cast_to<MMGraphNode>(_graph_edit->get_node(to));

	ignore_changes(true);
	_material->cancel_render_and_wait();

	if (from_node->connect_slot(from_slot, to_node, to_slot)) {
		_graph_edit->connect_node(from, from_slot, to, to_slot);
	}

	ignore_changes(false);
}

void MatMakerGDEditor::on_graph_edit_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	MMGraphNode *from_node = Object::cast_to<MMGraphNode>(_graph_edit->get_node(from));
	MMGraphNode *to_node = Object::cast_to<MMGraphNode>(_graph_edit->get_node(to));

	ignore_changes(true);
	_material->cancel_render_and_wait();

	if (from_node->disconnect_slot(from_slot, to_node, to_slot)) {
		_graph_edit->disconnect_node(from, from_slot, to, to_slot);
	}

	ignore_changes(false);
}

void MatMakerGDEditor::on_graph_node_close_request(Node *p_node) {
	MMGraphNode *node = Object::cast_to<MMGraphNode>(p_node);

	if (_material.is_valid()) {
		ignore_changes(true);

		_material->cancel_render_and_wait();
		//_material.remove_node(node._node);
		get_undo_redo()->create_action("MMGD: Remove Node");
		get_undo_redo()->add_do_method(*_material, "remove_node", node->get_mm_node());
		get_undo_redo()->add_undo_method(*_material, "add_node", node->get_mm_node());
		get_undo_redo()->commit_action();

		recreate();
		ignore_changes(false);
	}
}

void MatMakerGDEditor::_on_AddButton_pressed() {
	_create_popup->popup_centered();
}

void MatMakerGDEditor::_on_AddPopup_ok_pressed(const int type, const String &data) {
	if (!_material.is_valid()) {
		return;
	}

	_material->cancel_render_and_wait();
	Ref<MMNode> nnode;

	if (type == MMAlgos::MMNODE_REGISTRY_TYPE_CLASS) {
		nnode = Ref<MMNode>(ClassDB::instance(data));
	} else if (type == MMAlgos::MMNODE_REGISTRY_TYPE_SCRIPT) {
		Ref<Script> script = ResourceLoader::load(data);

		if (script.is_valid() && script->can_instance()) {
			nnode.instance();
			nnode->set_script(script.get_ref_ptr());
		}
	}

	if (!nnode.is_valid()) {
		print_error("_on_AddPopup_ok_pressed: Error !nnode! script/class: " + data);
		return;
	}

	ignore_changes(true);
	//_material.add_node(nnode);

	get_undo_redo()->create_action("MMGD: Add Node");
	get_undo_redo()->add_do_method(*_material, "add_node", nnode);
	get_undo_redo()->add_undo_method(*_material, "remove_node", nnode);
	get_undo_redo()->commit_action();

	MMGraphNode *gn = memnew(MMGraphNode);
	gn->set_editor_node(this);
	gn->set_node(_material, nnode);
	_graph_edit->add_child(gn);
	ignore_changes(false);
}

MatMakerGDEditor::MatMakerGDEditor() {
	_graph_edit = memnew(GraphEdit);
	_ignore_material_change_event = 0;
	_recreation_in_progress = false;
	_plugin = nullptr;
	_undo_redo = nullptr;

	set_name("MatMakerGDEditor");
	set_anchors_and_margins_preset(LayoutPreset::PRESET_WIDE);
	set_h_size_flags(SIZE_EXPAND_FILL);
	set_v_size_flags(SIZE_EXPAND_FILL);

	VBoxContainer *vboxcontainer = memnew(VBoxContainer);
	vboxcontainer->set_name("VBoxContainer");
	add_child(vboxcontainer);

	PanelContainer *panelcontainer_vboxcontainer = memnew(PanelContainer);
	panelcontainer_vboxcontainer->set_name("PanelContainer");
	vboxcontainer->add_child(panelcontainer_vboxcontainer);

	HBoxContainer *hboxcontainer_panelcontainer_vboxcontainer = memnew(HBoxContainer);
	hboxcontainer_panelcontainer_vboxcontainer->set_name("HBoxContainer");
	panelcontainer_vboxcontainer->add_child(hboxcontainer_panelcontainer_vboxcontainer);

	Button *addbutton_hboxcontainer_panelcontainer_vboxcontainer = memnew(Button);
	addbutton_hboxcontainer_panelcontainer_vboxcontainer->set_name("AddButton");
	addbutton_hboxcontainer_panelcontainer_vboxcontainer->set_text("Add");
	addbutton_hboxcontainer_panelcontainer_vboxcontainer->connect("pressed", this, "_on_AddButton_pressed");
	hboxcontainer_panelcontainer_vboxcontainer->add_child(addbutton_hboxcontainer_panelcontainer_vboxcontainer);

	_graph_edit = memnew(GraphEdit);
	_graph_edit->set_name("GraphEdit");
	_graph_edit->set_right_disconnects(true);
	_graph_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	_graph_edit->set_v_size_flags(SIZE_EXPAND_FILL);
	vboxcontainer->add_child(_graph_edit);

	Control *popups = memnew(Control);
	popups->set_name("Popups");
	popups->set_mouse_filter(MOUSE_FILTER_IGNORE);
	add_child(popups);

	_create_popup = memnew(MMCreateNamePopup);
	_create_popup->set_name("AddPopup");
	_create_popup->set_title("Create New Resource");
	popups->add_child(_create_popup);
}

MatMakerGDEditor::~MatMakerGDEditor() {
}

void MatMakerGDEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_graph_edit"), &MatMakerGDEditor::get_graph_edit);

	ClassDB::bind_method(D_METHOD("get_mm_material"), &MatMakerGDEditor::get_mm_material);
	ClassDB::bind_method(D_METHOD("set_mm_material", "value"), &MatMakerGDEditor::set_mm_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mm_material", PROPERTY_HINT_RESOURCE_TYPE, "MMMaterial"), "set_mm_material", "get_mm_material");

	ClassDB::bind_method(D_METHOD("get_ignore_material_change_event"), &MatMakerGDEditor::get_ignore_material_change_event);
	ClassDB::bind_method(D_METHOD("set_ignore_material_change_event", "value"), &MatMakerGDEditor::set_ignore_material_change_event);

	ClassDB::bind_method(D_METHOD("get_recreation_in_progress"), &MatMakerGDEditor::get_recreation_in_progress);
	ClassDB::bind_method(D_METHOD("set_recreation_in_progress", "value"), &MatMakerGDEditor::set_recreation_in_progress);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "recreation_in_progress"), "set_recreation_in_progress", "get_recreation_in_progress");

	//ClassDB::bind_method(D_METHOD("get_undo_redo"), &MatMakerGDEditor::get_undo_redo);
	//ClassDB::bind_method(D_METHOD("set_undo_redo", "value"), &MatMakerGDEditor::set_undo_redo);

	ClassDB::bind_method(D_METHOD("recreate"), &MatMakerGDEditor::recreate);
	//ClassDB::bind_method(D_METHOD("find_graph_node_for", "nnode"), &MatMakerGDEditor::find_graph_node_for);
	//ClassDB::bind_method(D_METHOD("set_mm_material", "object"), &MatMakerGDEditor::set_mm_material);
	ClassDB::bind_method(D_METHOD("on_material_changed"), &MatMakerGDEditor::on_material_changed);
	ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &MatMakerGDEditor::ignore_changes);

	ClassDB::bind_method(D_METHOD("on_graph_edit_connection_request", "from", "from_slot", "to", "to_slot"), &MatMakerGDEditor::on_graph_edit_connection_request);
	ClassDB::bind_method(D_METHOD("on_graph_edit_disconnection_request", "from", "from_slot", "to", "to_slot"), &MatMakerGDEditor::on_graph_edit_disconnection_request);
	ClassDB::bind_method(D_METHOD("on_graph_node_close_request", "node"), &MatMakerGDEditor::on_graph_node_close_request);
	ClassDB::bind_method(D_METHOD("_on_AddButton_pressed"), &MatMakerGDEditor::_on_AddButton_pressed);
	ClassDB::bind_method(D_METHOD("_on_AddPopup_ok_pressed", "script_path"), &MatMakerGDEditor::_on_AddPopup_ok_pressed);
}
