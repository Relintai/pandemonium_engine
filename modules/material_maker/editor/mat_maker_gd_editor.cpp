
#include "mat_maker_gd_editor.h"

NodePath MatMakerGDEditor::get_graph_edit_path() {
	return graph_edit_path;
}

void MatMakerGDEditor::set_graph_edit_path(const NodePath &val) {
	graph_edit_path = val;
}

NodePath MatMakerGDEditor::get_add_popup_path() {
	return add_popup_path;
}

void MatMakerGDEditor::set_add_popup_path(const NodePath &val) {
	add_popup_path = val;
}

GraphEdit *MatMakerGDEditor::get_graph_edit() {
	return _graph_edit;
}

void MatMakerGDEditor::set_graph_edit(GraphEdit *val) {
	_graph_edit = val;
}

Ref<MMMaterial> MatMakerGDEditor::get_mm_material() {
	return _material;
}

void MatMakerGDEditor::set_mm_material(const Ref<MMMaterial> &val) {
	_material = val;
}

int MatMakerGDEditor::get_ignore_material_change_event() const {
	return _ignore_material_change_event;
}

void MatMakerGDEditor::set_ignore_material_change_event(const int val) {
	_ignore_material_change_event = val;
}

bool MatMakerGDEditor::get_recreation_in_progress() const {
	return _recreation_in_progress;
}

void MatMakerGDEditor::set_recreation_in_progress(const bool val) {
	_recreation_in_progress = val;
}

EditorPlugin *MatMakerGDEditor::get_plugin() {
	return _plugin;
}

void MatMakerGDEditor::set_plugin(EditorPlugin *plugin) {
	_plugin = plugin;
	_undo_redo = plugin->get_undo_redo();
}

UndoRedo *MatMakerGDEditor::get_undo_redo() {
	return _undo_redo;
}

void MatMakerGDEditor::set_undo_redo(UndoRedo *val) {
	_undo_redo = val;
}

void MatMakerGDEditor::_enter_tree() {
	ensure_objs();
}

void MatMakerGDEditor::ensure_objs() {
	if (!_graph_edit) {
		_graph_edit = get_node(graph_edit_path);

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
	ensure_objs();
	_graph_edit.clear_connections();

	for (int i = 0; i < _graph_edit->get_child_count(); ++i) {
		GraphNode *c = Object::cast_to<GraphNode>(_graph_edit->get_child(i));

		if (c) {
			_graph_edit->remove_child(c);
			c->queue_free();
		}
	}

	if (!_material) {
		return;
	}

	_material->cancel_render_and_wait();

	for (int i = 0; i < _material->nodes.size(); ++i) {
		Ref<MMNode> n = _material->nodes[i];

		MMGraphNode *gn = memnew(MMGraphNode);
		gn->set_editor(this);
		gn->set_node(_material, n);
		_graph_edit->add_child(gn);
	}

	//connect them;

	for (int i = 0; i < _material->nodes.size(); ++i) {
		Ref<MMNode> n = _material->nodes[i];

		if (n.is_valid()) {
			for (int j = 0; j < n->input_property.size(); ++j) {
				Ref<MMNodeUniversalProperty> ip = n->input_properties[j];

				if (ip.is_valid()) {
					Node *input_node = find_graph_node_for(n);
					Node *output_node = find_graph_node_for(ip.input_property.owner);
					int to_slot = input_node.get_input_property_graph_node_slot_index(ip);
					int from_slot = output_node.get_output_property_graph_node_slot_index(ip.input_property);
					_graph_edit.connect_node(output_node.name, from_slot, input_node.name, to_slot);
				}
			}
		}
	}

	_material.render();
	_recreation_in_progress = false;
	ignore_changes(false);
}

MMGraphNode *MatMakerGDEditor::find_graph_node_for(const Ref<MMnode> &nnode) {
	for (int i = 0; i < _graph_edit->get_child_count(); ++i) {
		GraphNode *c = Object::cast_to<GraphNode>(_graph_edit->get_child(i));

		if (c) {
			if (c->has_method("get_material_node")) {
				Ref<MMnode> n = c->get_material_node();

				if (n == nnode) {
					return c;
				}
			}
		}
	}

	return nullptr;
}

void MatMakerGDEditor::set_mmmaterial(const Ref<MMMaterial> &object) {
	if (_material) {
		_material->disconnect("changed", this, "on_material_changed");
	}

	_material = object;
	recreate();

	if (_material) {
		_material->connect("changed", this, "on_material_changed");
	}
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

void MatMakerGDEditor::ignore_changes(const bool val) {
	if (val) {
		_ignore_material_change_event += 1;
	}

	else {
		_ignore_material_change_event -= 1;
	}
}

void MatMakerGDEditor::on_graph_edit_connection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	GraphNode *from_node = _graph_edit.get_node(from);
	GraphNode *to_node = _graph_edit.get_node(to);
	ignore_changes(true);
	_material->cancel_render_and_wait();

	if (from_node->connect_slot(from_slot, to_node, to_slot)) {
		_graph_edit->connect_node(from, from_slot, to, to_slot);
	}

	ignore_changes(false);
}

void MatMakerGDEditor::on_graph_edit_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot) {
	GraphNode *from_node = _graph_edit.get_node(from);
	GraphNode *to_node = _graph_edit.get_node(to);
	ignore_changes(true);
	_material->cancel_render_and_wait();

	if (from_node->disconnect_slot(from_slot, to_node, to_slot)) {
		_graph_edit->disconnect_node(from, from_slot, to, to_slot);
	}

	ignore_changes(false);
}

void MatMakerGDEditor::on_graph_node_close_request(const GraphNode &node) {
	if (_material) {
		ignore_changes(true);
		_material->cancel_render_and_wait();
		//_material.remove_node(node._node);
		_undo_redo->create_action("MMGD: Remove Node");
		_undo_redo->add_do_method(_material, "remove_node", node._node);
		_undo_redo->add_undo_method(_material, "add_node", node._node);
		_undo_redo->commit_action();
		recreate();
		ignore_changes(false);
	}
}

void MatMakerGDEditor::_on_AddButton_pressed() {
	get_node(add_popup_path)->opup_centered();
}

void MatMakerGDEditor::_on_AddPopup_ok_pressed(const String &script_path) {
	if (!_material) {
		return;
	}

	ensure_objs();
	_material->cancel_render_and_wait();
	//Variant = load(script_path);
	MMGraphNode *nnode = memnew(MMGraphNode);

	if (!nnode) {
		print_error("_on_AddPopup_ok_pressed: Error !nnode! script: " + script_path);
		return;
	}

	ignore_changes(true);
	//_material.add_node(nnode);
	_undo_redo->create_action("MMGD: Add Node");
	_undo_redo->add_do_method(_material, "add_node", nnode);
	_undo_redo->add_undo_method(_material, "remove_node", nnode);
	_undo_redo->commit_action();
	MMGraphNode *gn = memnew(MMGraphNode);
	gn->set_editor(this);
	gn->set_node(_material, nnode);
	_graph_edit->add_child(gn);
	ignore_changes(false);
}

MatMakerGDEditor::MatMakerGDEditor() {
	//var MMGraphNode = preload("res://addons/mat_maker_gd/editor/mm_graph_node.gd");
	graph_edit_path = "VBoxContainer/GraphEdit";
	add_popup_path = "Popups/AddPopup";
	_graph_edit = nullptr;
	_material;
	_ignore_material_change_event = 0;
	_recreation_in_progress = false;
	_plugin = nullptr;
	_undo_redo = nullptr;

	//Script: res://addons/mat_maker_gd/editor/MatMakerGDEditor.gd
	MarginContainer *matmakergdeditor = memnew(MarginContainer);
	matmakergdeditor->set_name("MatMakerGDEditor");
	matmakergdeditor->set_filename("res://addons/mat_maker_gd/editor/MatMakerGDEditor.tscn");
	matmakergdeditor->set_anchor_right(1);
	matmakergdeditor->set_anchor_bottom(1);
	matmakergdeditor->set_rect_min_size(Vector2(0, 200));
	matmakergdeditor->set_size_flags_horizontal(3);
	matmakergdeditor->set_size_flags_vertical(3);

	VBoxContainer *vboxcontainer_matmakergdeditor = memnew(VBoxContainer);
	vboxcontainer_matmakergdeditor->set_name("VBoxContainer");
	matmakergdeditor->add_child(vboxcontainer_matmakergdeditor);
	vboxcontainer_matmakergdeditor->set_name("VBoxContainer");
	//vboxcontainer_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	vboxcontainer_matmakergdeditor->set_margin_right(1024);
	vboxcontainer_matmakergdeditor->set_margin_bottom(600);
	vboxcontainer_matmakergdeditor->set_rect_size(Vector2(1024, 600));

	PanelContainer *panelcontainer_vboxcontainer_matmakergdeditor = memnew(PanelContainer);
	panelcontainer_vboxcontainer_matmakergdeditor->set_name("PanelContainer");
	vboxcontainer_matmakergdeditor->add_child(panelcontainer_vboxcontainer_matmakergdeditor);
	panelcontainer_vboxcontainer_matmakergdeditor->set_name("PanelContainer");
	//panelcontainer_vboxcontainer_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	panelcontainer_vboxcontainer_matmakergdeditor->set_margin_right(1024);
	panelcontainer_vboxcontainer_matmakergdeditor->set_margin_bottom(34);
	panelcontainer_vboxcontainer_matmakergdeditor->set_rect_size(Vector2(1024, 34));

	HBoxContainer *hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor = memnew(HBoxContainer);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_name("HBoxContainer");
	panelcontainer_vboxcontainer_matmakergdeditor->add_child(hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_name("HBoxContainer");
	//hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_left(7);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_top(7);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_right(1017);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_bottom(27);
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_rect_position(Vector2(7, 7));
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_rect_global_position(Vector2(7, 7));
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_rect_size(Vector2(1010, 20));

	Button *addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor = memnew(Button);
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_name("AddButton");
	hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->add_child(addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor);
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_name("AddButton");
	//addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_right(37);
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_margin_bottom(20);
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_rect_size(Vector2(37, 20));
	addbutton_hboxcontainer_panelcontainer_vboxcontainer_matmakergdeditor->set_text("Add");

	GraphEdit *graphedit_vboxcontainer_matmakergdeditor = memnew(GraphEdit);
	graphedit_vboxcontainer_matmakergdeditor->set_name("GraphEdit");
	vboxcontainer_matmakergdeditor->add_child(graphedit_vboxcontainer_matmakergdeditor);
	graphedit_vboxcontainer_matmakergdeditor->set_name("GraphEdit");
	//graphedit_vboxcontainer_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	graphedit_vboxcontainer_matmakergdeditor->set_margin_top(38);
	graphedit_vboxcontainer_matmakergdeditor->set_margin_right(1024);
	graphedit_vboxcontainer_matmakergdeditor->set_margin_bottom(600);
	graphedit_vboxcontainer_matmakergdeditor->set_rect_position(Vector2(0, 38));
	graphedit_vboxcontainer_matmakergdeditor->set_rect_global_position(Vector2(0, 38));
	graphedit_vboxcontainer_matmakergdeditor->set_rect_size(Vector2(1024, 562));
	graphedit_vboxcontainer_matmakergdeditor->set_size_flags_horizontal(3);
	graphedit_vboxcontainer_matmakergdeditor->set_size_flags_vertical(3);
	graphedit_vboxcontainer_matmakergdeditor->set_right_disconnects(True);
	graphedit_vboxcontainer_matmakergdeditor->set_scroll_offset(Vector2(0, -20));

	GraphEditFilter *grapheditfilter_graphedit_vboxcontainer_matmakergdeditor = memnew(GraphEditFilter);
	grapheditfilter_graphedit_vboxcontainer_matmakergdeditor->set_name("GraphEditFilter");
	graphedit_vboxcontainer_matmakergdeditor->add_child(grapheditfilter_graphedit_vboxcontainer_matmakergdeditor);
	Control *clayer_graphedit_vboxcontainer_matmakergdeditor = memnew(Control);
	clayer_graphedit_vboxcontainer_matmakergdeditor->set_name("CLAYER");
	graphedit_vboxcontainer_matmakergdeditor->add_child(clayer_graphedit_vboxcontainer_matmakergdeditor);
	clayer_graphedit_vboxcontainer_matmakergdeditor->set_name("CLAYER");
	clayer_graphedit_vboxcontainer_matmakergdeditor->set_mouse_filter(2);

	Control *popups_matmakergdeditor = memnew(Control);
	popups_matmakergdeditor->set_name("Popups");
	matmakergdeditor->add_child(popups_matmakergdeditor);
	popups_matmakergdeditor->set_name("Popups");
	//popups_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	popups_matmakergdeditor->set_margin_right(1024);
	popups_matmakergdeditor->set_margin_bottom(600);
	popups_matmakergdeditor->set_rect_size(Vector2(1024, 600));
	popups_matmakergdeditor->set_mouse_filter(2);

	//Script: res://addons/mat_maker_gd/editor/CreateNamePopup.gd
	ConfirmationDialog *addpopup_popups_matmakergdeditor = memnew(ConfirmationDialog);
	addpopup_popups_matmakergdeditor->set_name("AddPopup");
	popups_matmakergdeditor->add_child(addpopup_popups_matmakergdeditor);
	addpopup_popups_matmakergdeditor->set_name("AddPopup");
	addpopup_popups_matmakergdeditor->set_filename("res://addons/mat_maker_gd/editor/CreateNamePopup.tscn");
	//addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: MatMakerGDEditor:[MarginContainer:280327]
	addpopup_popups_matmakergdeditor->set_anchor_left(0.5);
	addpopup_popups_matmakergdeditor->set_anchor_top(0.5);
	addpopup_popups_matmakergdeditor->set_anchor_right(0.5);
	addpopup_popups_matmakergdeditor->set_anchor_bottom(0.5);
	addpopup_popups_matmakergdeditor->set_margin_left(-245.5);
	addpopup_popups_matmakergdeditor->set_margin_top(-220);
	addpopup_popups_matmakergdeditor->set_margin_right(245.5);
	addpopup_popups_matmakergdeditor->set_margin_bottom(220);
	addpopup_popups_matmakergdeditor->set_rect_position(Vector2(-245.5, -220));
	addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(-245.5, -220));
	addpopup_popups_matmakergdeditor->set_rect_size(Vector2(491, 440));
	addpopup_popups_matmakergdeditor->set_window_title("Create New Resource");
	//addpopup_popups_matmakergdeditor property _meta_ TYPE_DICTIONARY value: {_edit_use_anchors_:False}
	//addpopup_popups_matmakergdeditor property line_edit_path TYPE_NODE_PATH value: VBoxContainer/LineEdit
	//addpopup_popups_matmakergdeditor property tree_path TYPE_NODE_PATH value: VBoxContainer/Tree
	//addpopup_popups_matmakergdeditor property type_folders TYPE_STRING_ARRAY value: [res://addons/mat_maker_gd/nodes/uniform, res://addons/mat_maker_gd/nodes/noise, res://addons/mat_maker_gd/nodes/filter, res://addons/mat_maker_gd/nodes/gradient, res://addons/mat_maker_gd/nodes/pattern, res://addons/mat_maker_gd/nodes/sdf2d, res://addons/mat_maker_gd/nodes/sdf3d, res://addons/mat_maker_gd/nodes/transform, res://addons/mat_maker_gd/nodes/simple, res://addons/mat_maker_gd/nodes/other]

	TextureButton *texturebutton_addpopup_popups_matmakergdeditor = memnew(TextureButton);
	texturebutton_addpopup_popups_matmakergdeditor->set_name("TextureButton");
	addpopup_popups_matmakergdeditor->add_child(texturebutton_addpopup_popups_matmakergdeditor);
	texturebutton_addpopup_popups_matmakergdeditor->set_name("TextureButton");

	Label *label_addpopup_popups_matmakergdeditor = memnew(Label);
	label_addpopup_popups_matmakergdeditor->set_name("Label");
	addpopup_popups_matmakergdeditor->add_child(label_addpopup_popups_matmakergdeditor);
	label_addpopup_popups_matmakergdeditor->set_name("Label");
	label_addpopup_popups_matmakergdeditor->set_anchor_right(1);
	label_addpopup_popups_matmakergdeditor->set_anchor_bottom(1);
	label_addpopup_popups_matmakergdeditor->set_margin_left(8);
	label_addpopup_popups_matmakergdeditor->set_margin_top(8);
	label_addpopup_popups_matmakergdeditor->set_margin_right(-8);
	label_addpopup_popups_matmakergdeditor->set_margin_bottom(-42);
	label_addpopup_popups_matmakergdeditor->set_rect_position(Vector2(8, 8));
	label_addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(8, 8));
	label_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(0, 14));

	HBoxContainer *hboxcontainer_addpopup_popups_matmakergdeditor = memnew(HBoxContainer);
	hboxcontainer_addpopup_popups_matmakergdeditor->set_name("HBoxContainer");
	addpopup_popups_matmakergdeditor->add_child(hboxcontainer_addpopup_popups_matmakergdeditor);
	hboxcontainer_addpopup_popups_matmakergdeditor->set_name("HBoxContainer");

	Control *control3_hboxcontainer_addpopup_popups_matmakergdeditor = memnew(Control);
	control3_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control3");
	hboxcontainer_addpopup_popups_matmakergdeditor->add_child(control3_hboxcontainer_addpopup_popups_matmakergdeditor);
	control3_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control3");
	control3_hboxcontainer_addpopup_popups_matmakergdeditor->set_mouse_filter(1);
	control3_hboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);

	Button *button2_hboxcontainer_addpopup_popups_matmakergdeditor = memnew(Button);
	button2_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Button2");
	hboxcontainer_addpopup_popups_matmakergdeditor->add_child(button2_hboxcontainer_addpopup_popups_matmakergdeditor);
	button2_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Button2");
	button2_hboxcontainer_addpopup_popups_matmakergdeditor->set_text("Cancel");

	Control *control_hboxcontainer_addpopup_popups_matmakergdeditor = memnew(Control);
	control_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control");
	hboxcontainer_addpopup_popups_matmakergdeditor->add_child(control_hboxcontainer_addpopup_popups_matmakergdeditor);
	control_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control");
	control_hboxcontainer_addpopup_popups_matmakergdeditor->set_mouse_filter(1);
	control_hboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);

	Button *button_hboxcontainer_addpopup_popups_matmakergdeditor = memnew(Button);
	button_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Button");
	hboxcontainer_addpopup_popups_matmakergdeditor->add_child(button_hboxcontainer_addpopup_popups_matmakergdeditor);
	button_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Button");
	button_hboxcontainer_addpopup_popups_matmakergdeditor->set_text("OK");

	Control *control2_hboxcontainer_addpopup_popups_matmakergdeditor = memnew(Control);
	control2_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control2");
	hboxcontainer_addpopup_popups_matmakergdeditor->add_child(control2_hboxcontainer_addpopup_popups_matmakergdeditor);
	control2_hboxcontainer_addpopup_popups_matmakergdeditor->set_name("Control2");
	control2_hboxcontainer_addpopup_popups_matmakergdeditor->set_mouse_filter(1);
	control2_hboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);

	VBoxContainer *vboxcontainer_addpopup_popups_matmakergdeditor = memnew(VBoxContainer);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_name("VBoxContainer");
	addpopup_popups_matmakergdeditor->add_child(vboxcontainer_addpopup_popups_matmakergdeditor);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_name("VBoxContainer");
	//vboxcontainer_addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: AddPopup:[ConfirmationDialog:280352]
	vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_left(8);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_top(8);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_right(483);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_bottom(404);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_position(Vector2(8, 8));
	vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(8, 8));
	vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(475, 396));
	vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);
	vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_vertical(3);
	//vboxcontainer_addpopup_popups_matmakergdeditor property _meta_ TYPE_DICTIONARY value: {_edit_use_anchors_:False}

	Label *label2_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(Label);
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Label2");
	vboxcontainer_addpopup_popups_matmakergdeditor->add_child(label2_vboxcontainer_addpopup_popups_matmakergdeditor);
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Label2");
	//label2_vboxcontainer_addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: AddPopup:[ConfirmationDialog:280352]
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_right(475);
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_bottom(14);
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(475, 14));
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);
	label2_vboxcontainer_addpopup_popups_matmakergdeditor->set_text("Type");

	Tree *tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(Tree);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Tree");
	vboxcontainer_addpopup_popups_matmakergdeditor->add_child(tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Tree");
	//tree_vboxcontainer_addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: AddPopup:[ConfirmationDialog:280352]
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_top(18);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_right(475);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_bottom(350);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_position(Vector2(0, 18));
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(0, 18));
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(475, 332));
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_vertical(3);
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_hide_root(True);

	PopupMenu *popupmenu_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(PopupMenu);
	popupmenu_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(popupmenu_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	popupmenu_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");

	LineEdit *lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(LineEdit);
	lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("LineEdit");
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("LineEdit");
	lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_visible(False);

	PopupMenu *popupmenu_lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(PopupMenu);
	popupmenu_lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");
	lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(popupmenu_lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	popupmenu_lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");
	//popupmenu_lineedit_tree_vboxcontainer_addpopup_popups_matmakergdeditor property items TYPE_ARRAY value: [Cut, [Object:null], 0, False, False, 0, 268435544, Null, , False, Copy, [Object:null], 0, False, False, 1, 268435523, Null, , False, Paste, [Object:null], 0, False, False, 2, 268435542, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Select All, [Object:null], 0, False, False, 4, 268435521, Null, , False, Clear, [Object:null], 0, False, False, 3, 0, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Undo, [Object:null], 0, False, False, 5, 268435546, Null, , False, Redo, [Object:null], 0, False, False, 6, 301989978, Null, , False]

	HSlider *hslider_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(HSlider);
	hslider_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("HSlider");
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(hslider_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	hslider_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("HSlider");
	hslider_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_visible(False);

	HScrollBar *hscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(HScrollBar);
	hscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("HScrollBar");
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(hscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	hscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("HScrollBar");

	VScrollBar *vscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(VScrollBar);
	vscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("VScrollBar");
	tree_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(vscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor);
	vscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("VScrollBar");
	vscrollbar_tree_vboxcontainer_addpopup_popups_matmakergdeditor->set_custom_step(14);

	Label *label_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(Label);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Label");
	vboxcontainer_addpopup_popups_matmakergdeditor->add_child(label_vboxcontainer_addpopup_popups_matmakergdeditor);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("Label");
	//label_vboxcontainer_addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: AddPopup:[ConfirmationDialog:280352]
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_visible(False);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_top(354);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_right(475);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_bottom(368);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_position(Vector2(0, 354));
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(0, 354));
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(475, 14));
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);
	label_vboxcontainer_addpopup_popups_matmakergdeditor->set_text("Name");

	LineEdit *lineedit_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(LineEdit);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("LineEdit");
	vboxcontainer_addpopup_popups_matmakergdeditor->add_child(lineedit_vboxcontainer_addpopup_popups_matmakergdeditor);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("LineEdit");
	//lineedit_vboxcontainer_addpopup_popups_matmakergdeditor property owner TYPE_OBJECT value: AddPopup:[ConfirmationDialog:280352]
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_visible(False);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_top(372);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_right(475);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_margin_bottom(396);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_position(Vector2(0, 372));
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_global_position(Vector2(0, 372));
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_rect_size(Vector2(475, 24));
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_size_flags_horizontal(3);
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_caret_blink(True);

	PopupMenu *popupmenu_lineedit_vboxcontainer_addpopup_popups_matmakergdeditor = memnew(PopupMenu);
	popupmenu_lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");
	lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->add_child(popupmenu_lineedit_vboxcontainer_addpopup_popups_matmakergdeditor);
	popupmenu_lineedit_vboxcontainer_addpopup_popups_matmakergdeditor->set_name("PopupMenu");
	//popupmenu_lineedit_vboxcontainer_addpopup_popups_matmakergdeditor property items TYPE_ARRAY value: [Cut, [Object:null], 0, False, False, 0, 268435544, Null, , False, Copy, [Object:null], 0, False, False, 1, 268435523, Null, , False, Paste, [Object:null], 0, False, False, 2, 268435542, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Select All, [Object:null], 0, False, False, 4, 268435521, Null, , False, Clear, [Object:null], 0, False, False, 3, 0, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Undo, [Object:null], 0, False, False, 5, 268435546, Null, , False, Redo, [Object:null], 0, False, False, 6, 301989978, Null, , False]
}

MatMakerGDEditor::~MatMakerGDEditor() {
}

static void MatMakerGDEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_Variant"), &MatMakerGDEditor::get_Variant);
	ClassDB::bind_method(D_METHOD("set_Variant", "value"), &MatMakerGDEditor::set_Variant);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");

	ClassDB::bind_method(D_METHOD("get_graph_edit_path"), &MatMakerGDEditor::get_graph_edit_path);
	ClassDB::bind_method(D_METHOD("set_graph_edit_path", "value"), &MatMakerGDEditor::set_graph_edit_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "graph_edit_path"), "set_graph_edit_path", "get_graph_edit_path");

	ClassDB::bind_method(D_METHOD("get_add_popup_path"), &MatMakerGDEditor::get_add_popup_path);
	ClassDB::bind_method(D_METHOD("set_add_popup_path", "value"), &MatMakerGDEditor::set_add_popup_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "add_popup_path"), "set_add_popup_path", "get_add_popup_path");

	ClassDB::bind_method(D_METHOD("get_graph_edit"), &MatMakerGDEditor::get_graph_edit);
	ClassDB::bind_method(D_METHOD("set_graph_edit", "value"), &MatMakerGDEditor::set_graph_edit);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_graph_edit", PROPERTY_HINT_RESOURCE_TYPE, "GraphEdit"), "set_graph_edit", "get_graph_edit");

	ClassDB::bind_method(D_METHOD("get_material"), &MatMakerGDEditor::get_material);
	ClassDB::bind_method(D_METHOD("set_material", "value"), &MatMakerGDEditor::set_material);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_material", PROPERTY_HINT_RESOURCE_TYPE, "Ref<MMMaterial>"), "set_material", "get_material");

	ClassDB::bind_method(D_METHOD("get_ignore_material_change_event"), &MatMakerGDEditor::get_ignore_material_change_event);
	ClassDB::bind_method(D_METHOD("set_ignore_material_change_event", "value"), &MatMakerGDEditor::set_ignore_material_change_event);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "_ignore_material_change_event"), "set_ignore_material_change_event", "get_ignore_material_change_event");

	ClassDB::bind_method(D_METHOD("get_recreation_in_progress"), &MatMakerGDEditor::get_recreation_in_progress);
	ClassDB::bind_method(D_METHOD("set_recreation_in_progress", "value"), &MatMakerGDEditor::set_recreation_in_progress);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "_recreation_in_progress"), "set_recreation_in_progress", "get_recreation_in_progress");

	ClassDB::bind_method(D_METHOD("get_plugin"), &MatMakerGDEditor::get_plugin);
	ClassDB::bind_method(D_METHOD("set_plugin", "value"), &MatMakerGDEditor::set_plugin);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_plugin", PROPERTY_HINT_RESOURCE_TYPE, "EditorPlugin"), "set_plugin", "get_plugin");

	ClassDB::bind_method(D_METHOD("get_undo_redo"), &MatMakerGDEditor::get_undo_redo);
	ClassDB::bind_method(D_METHOD("set_undo_redo", "value"), &MatMakerGDEditor::set_undo_redo);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_undo_redo", PROPERTY_HINT_RESOURCE_TYPE, "UndoRedo"), "set_undo_redo", "get_undo_redo");

	ClassDB::bind_method(D_METHOD("_enter_tree"), &MatMakerGDEditor::_enter_tree);

	ClassDB::bind_method(D_METHOD("set_plugin", "plugin"), &MatMakerGDEditor::set_plugin);
	ClassDB::bind_method(D_METHOD("get_undo_redo"), &MatMakerGDEditor::get_undo_redo);
	ClassDB::bind_method(D_METHOD("ensure_objs"), &MatMakerGDEditor::ensure_objs);
	ClassDB::bind_method(D_METHOD("recreate"), &MatMakerGDEditor::recreate);
	ClassDB::bind_method(D_METHOD("find_graph_node_for", "nnode"), &MatMakerGDEditor::find_graph_node_for);
	ClassDB::bind_method(D_METHOD("set_mmmaterial", "object"), &MatMakerGDEditor::set_mmmaterial);
	ClassDB::bind_method(D_METHOD("on_material_changed"), &MatMakerGDEditor::on_material_changed);
	ClassDB::bind_method(D_METHOD("ignore_changes", "val"), &MatMakerGDEditor::ignore_changes);

	ClassDB::bind_method(D_METHOD("on_graph_edit_connection_request", "from", "from_slot", "to", "to_slot"), &MatMakerGDEditor::on_graph_edit_connection_request);
	ClassDB::bind_method(D_METHOD("on_graph_edit_disconnection_request", "from", "from_slot", "to", "to_slot"), &MatMakerGDEditor::on_graph_edit_disconnection_request);
	ClassDB::bind_method(D_METHOD("on_graph_node_close_request", "node"), &MatMakerGDEditor::on_graph_node_close_request);
	ClassDB::bind_method(D_METHOD("_on_AddButton_pressed"), &MatMakerGDEditor::_on_AddButton_pressed);
	ClassDB::bind_method(D_METHOD("_on_AddPopup_ok_pressed", "script_path"), &MatMakerGDEditor::_on_AddPopup_ok_pressed);
}
