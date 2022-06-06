
#include "createnamepopup.h"

NodePath CreateNamePopup::get_line_edit_path() {
	return line_edit_path;
}

void CreateNamePopup::set_line_edit_path(const NodePath &val) {
	line_edit_path = val;
}

NodePath CreateNamePopup::get_tree_path() {
	return tree_path;
}

void CreateNamePopup::set_tree_path(const NodePath &val) {
	tree_path = val;
}

PoolStringArray CreateNamePopup::get_type_folders() {
	return type_folders;
}

void CreateNamePopup::set_type_folders(const PoolStringArray &val) {
	type_folders = val;
}

String CreateNamePopup::get__resource_type() {
	return _resource_type;
}

void CreateNamePopup::set__resource_type(const String &val) {
	_resource_type = val;
}

LineEdit CreateNamePopup::get_ *_line_edit() {
	return *_line_edit;
}

void CreateNamePopup::set_ *_line_edit(const LineEdit &val) {
	*_line_edit = val;
}

Tree CreateNamePopup::get_ *_tree() {
	return *_tree;
}

void CreateNamePopup::set_ *_tree(const Tree &val) {
	*_tree = val;
}

//tool;
signal ok_pressed;
//export(NodePath) ;
NodePath line_edit_path = ;
//export(NodePath) ;
NodePath tree_path = ;
//export(PoolStringArray) ;
PoolStringArray type_folders = ;
String _resource_type = "MMNode";
LineEdit *_line_edit;
Tree *_tree;

void CreateNamePopup::_ready() {
	_line_edit = get_node(line_edit_path) as LineEdit;
	_tree = get_node(tree_path) as Tree;
	connect("confirmed", self, "_on_OK_pressed");
	connect("about_to_show", self, "about_to_show");
}

void CreateNamePopup::set_resource_type(const String &resource_type) {
	_resource_type = resource_type;
}

void CreateNamePopup::about_to_show() {
	_tree.clear();
	TreeItem *root = _tree.create_item();

	for (s in type_folders) {
		evaluate_folder(s, root);
	}
}

void CreateNamePopup::evaluate_folder(const String &folder, const TreeItem &root) {
	TreeItem *ti = _tree.create_item(root);
	ti.set_text(0, folder.substr(folder.find_last("/") + 1));
	Variant = Directory.new();

	if (dir.open(folder) == OK) {
		dir.list_dir_begin();
		Variant = dir.get_next();

		while (file_name != "") {
			if (!dir.current_is_dir()) {
				print("Found file: " + file_name);
				TreeItem *e = _tree.create_item(ti);
				e.set_text(0, file_name.get_file());
				e.set_meta("file", folder + "/" + file_name);
			}

			file_name = dir.get_next();
		}

	}

	else {
		print("An error occurred when trying to access the path.");
	}
}

void CreateNamePopup::_on_OK_pressed() {
	TreeItem *selected = _tree.get_selected();

	if (selected) {
		if (!selected.has_meta("file")) {
			hide();
			return;
		}

		String file_name = selected.get_meta("file");
		emit_signal("ok_pressed", file_name);
	}

	hide();
}
}

CreateNamePopup::CreateNamePopup() {
	line_edit_path = ;
	tree_path = ;
	type_folders = ;
	_resource_type = "MMNode";
	*_line_edit;
	*_tree;

	//Script: res://addons/mat_maker_gd/editor/CreateNamePopup.gd
	ConfirmationDialog *createnamepopup = memnew(ConfirmationDialog);
	createnamepopup->set_name("CreateNamePopup");

	createnamepopup->set_name("CreateNamePopup");
	createnamepopup->set_filename("res://addons/mat_maker_gd/editor/CreateNamePopup.tscn");
	createnamepopup->set_anchor_left(0.5);
	createnamepopup->set_anchor_top(0.5);
	createnamepopup->set_anchor_right(0.5);
	createnamepopup->set_anchor_bottom(0.5);
	createnamepopup->set_margin_left(-245.5);
	createnamepopup->set_margin_top(-220);
	createnamepopup->set_margin_right(245.5);
	createnamepopup->set_margin_bottom(220);
	createnamepopup->set_rect_position(Vector2(-245.5, -220));
	createnamepopup->set_rect_global_position(Vector2(-245.5, -220));
	createnamepopup->set_rect_size(Vector2(491, 440));
	createnamepopup->set_window_title("Create New Resource");
	//createnamepopup property __meta__ TYPE_DICTIONARY value: {_edit_use_anchors_:False}
	//createnamepopup property line_edit_path TYPE_NODE_PATH value: VBoxContainer/LineEdit
	//createnamepopup property tree_path TYPE_NODE_PATH value: VBoxContainer/Tree

	TextureButton *texturebutton_createnamepopup = memnew(TextureButton);
	texturebutton_createnamepopup->set_name("TextureButton");
	createnamepopup->add_child(texturebutton_createnamepopup);
	texturebutton_createnamepopup->set_name("TextureButton");

	Label *label_createnamepopup = memnew(Label);
	label_createnamepopup->set_name("Label");
	createnamepopup->add_child(label_createnamepopup);

	label_createnamepopup->set_name("Label");
	label_createnamepopup->set_anchor_right(1);
	label_createnamepopup->set_anchor_bottom(1);
	label_createnamepopup->set_margin_left(8);
	label_createnamepopup->set_margin_top(8);
	label_createnamepopup->set_margin_right(-8);
	label_createnamepopup->set_margin_bottom(-42);
	label_createnamepopup->set_rect_position(Vector2(8, 8));
	label_createnamepopup->set_rect_global_position(Vector2(8, 8));
	label_createnamepopup->set_rect_size(Vector2(0, 14));

	HBoxContainer *hboxcontainer_createnamepopup = memnew(HBoxContainer);
	hboxcontainer_createnamepopup->set_name("HBoxContainer");
	createnamepopup->add_child(hboxcontainer_createnamepopup);
	hboxcontainer_createnamepopup->set_name("HBoxContainer");

	Control *control3_hboxcontainer_createnamepopup = memnew(Control);
	control3_hboxcontainer_createnamepopup->set_name("Control3");
	hboxcontainer_createnamepopup->add_child(control3_hboxcontainer_createnamepopup);
	control3_hboxcontainer_createnamepopup->set_name("Control3");
	control3_hboxcontainer_createnamepopup->set_mouse_filter(1);
	control3_hboxcontainer_createnamepopup->set_size_flags_horizontal(3);

	Button *button2_hboxcontainer_createnamepopup = memnew(Button);
	button2_hboxcontainer_createnamepopup->set_name("Button2");
	hboxcontainer_createnamepopup->add_child(button2_hboxcontainer_createnamepopup);
	button2_hboxcontainer_createnamepopup->set_name("Button2");
	button2_hboxcontainer_createnamepopup->set_text("Cancel");

	Control *control_hboxcontainer_createnamepopup = memnew(Control);
	control_hboxcontainer_createnamepopup->set_name("Control");
	hboxcontainer_createnamepopup->add_child(control_hboxcontainer_createnamepopup);
	control_hboxcontainer_createnamepopup->set_name("Control");
	control_hboxcontainer_createnamepopup->set_mouse_filter(1);
	control_hboxcontainer_createnamepopup->set_size_flags_horizontal(3);

	Button *button_hboxcontainer_createnamepopup = memnew(Button);
	button_hboxcontainer_createnamepopup->set_name("Button");
	hboxcontainer_createnamepopup->add_child(button_hboxcontainer_createnamepopup);
	button_hboxcontainer_createnamepopup->set_name("Button");
	button_hboxcontainer_createnamepopup->set_text("OK");

	Control *control2_hboxcontainer_createnamepopup = memnew(Control);
	control2_hboxcontainer_createnamepopup->set_name("Control2");
	hboxcontainer_createnamepopup->add_child(control2_hboxcontainer_createnamepopup);
	control2_hboxcontainer_createnamepopup->set_name("Control2");
	control2_hboxcontainer_createnamepopup->set_mouse_filter(1);
	control2_hboxcontainer_createnamepopup->set_size_flags_horizontal(3);

	VBoxContainer *vboxcontainer_createnamepopup = memnew(VBoxContainer);
	vboxcontainer_createnamepopup->set_name("VBoxContainer");
	createnamepopup->add_child(vboxcontainer_createnamepopup);
	vboxcontainer_createnamepopup->set_name("VBoxContainer");
	//vboxcontainer_createnamepopup property owner TYPE_OBJECT value: CreateNamePopup:[ConfirmationDialog:280461]
	vboxcontainer_createnamepopup->set_margin_left(8);
	vboxcontainer_createnamepopup->set_margin_top(8);
	vboxcontainer_createnamepopup->set_margin_right(483);
	vboxcontainer_createnamepopup->set_margin_bottom(404);
	vboxcontainer_createnamepopup->set_rect_position(Vector2(8, 8));
	vboxcontainer_createnamepopup->set_rect_global_position(Vector2(8, 8));
	vboxcontainer_createnamepopup->set_rect_size(Vector2(475, 396));
	vboxcontainer_createnamepopup->set_size_flags_horizontal(3);
	vboxcontainer_createnamepopup->set_size_flags_vertical(3);
	//vboxcontainer_createnamepopup property __meta__ TYPE_DICTIONARY value: {_edit_use_anchors_:False}

	Label *label2_vboxcontainer_createnamepopup = memnew(Label);
	label2_vboxcontainer_createnamepopup->set_name("Label2");
	vboxcontainer_createnamepopup->add_child(label2_vboxcontainer_createnamepopup);
	label2_vboxcontainer_createnamepopup->set_name("Label2");
	//label2_vboxcontainer_createnamepopup property owner TYPE_OBJECT value: CreateNamePopup:[ConfirmationDialog:280461]
	label2_vboxcontainer_createnamepopup->set_margin_right(475);
	label2_vboxcontainer_createnamepopup->set_margin_bottom(14);
	label2_vboxcontainer_createnamepopup->set_rect_size(Vector2(475, 14));
	label2_vboxcontainer_createnamepopup->set_size_flags_horizontal(3);
	label2_vboxcontainer_createnamepopup->set_text("Type");

	Tree *tree_vboxcontainer_createnamepopup = memnew(Tree);
	tree_vboxcontainer_createnamepopup->set_name("Tree");
	vboxcontainer_createnamepopup->add_child(tree_vboxcontainer_createnamepopup);
	tree_vboxcontainer_createnamepopup->set_name("Tree");
	//tree_vboxcontainer_createnamepopup property owner TYPE_OBJECT value: CreateNamePopup:[ConfirmationDialog:280461]
	tree_vboxcontainer_createnamepopup->set_margin_top(18);
	tree_vboxcontainer_createnamepopup->set_margin_right(475);
	tree_vboxcontainer_createnamepopup->set_margin_bottom(350);
	tree_vboxcontainer_createnamepopup->set_rect_position(Vector2(0, 18));
	tree_vboxcontainer_createnamepopup->set_rect_global_position(Vector2(0, 18));
	tree_vboxcontainer_createnamepopup->set_rect_size(Vector2(475, 332));
	tree_vboxcontainer_createnamepopup->set_size_flags_horizontal(3);
	tree_vboxcontainer_createnamepopup->set_size_flags_vertical(3);
	tree_vboxcontainer_createnamepopup->set_hide_root(True);

	PopupMenu *popupmenu_tree_vboxcontainer_createnamepopup = memnew(PopupMenu);
	popupmenu_tree_vboxcontainer_createnamepopup->set_name("PopupMenu");
	tree_vboxcontainer_createnamepopup->add_child(popupmenu_tree_vboxcontainer_createnamepopup);
	popupmenu_tree_vboxcontainer_createnamepopup->set_name("PopupMenu");

	Timer *timer_popupmenu_tree_vboxcontainer_createnamepopup = memnew(Timer);
	timer_popupmenu_tree_vboxcontainer_createnamepopup->set_name("Timer");
	popupmenu_tree_vboxcontainer_createnamepopup->add_child(timer_popupmenu_tree_vboxcontainer_createnamepopup);
	timer_popupmenu_tree_vboxcontainer_createnamepopup->set_name("Timer");
	timer_popupmenu_tree_vboxcontainer_createnamepopup->set_wait_time(0.3);
	timer_popupmenu_tree_vboxcontainer_createnamepopup->set_one_shot(True);

	LineEdit *lineedit_tree_vboxcontainer_createnamepopup = memnew(LineEdit);
	lineedit_tree_vboxcontainer_createnamepopup->set_name("LineEdit");
	tree_vboxcontainer_createnamepopup->add_child(lineedit_tree_vboxcontainer_createnamepopup);
	lineedit_tree_vboxcontainer_createnamepopup->set_name("LineEdit");
	lineedit_tree_vboxcontainer_createnamepopup->set_visible(False);

	Timer *timer_lineedit_tree_vboxcontainer_createnamepopup = memnew(Timer);
	timer_lineedit_tree_vboxcontainer_createnamepopup->set_name("Timer");
	lineedit_tree_vboxcontainer_createnamepopup->add_child(timer_lineedit_tree_vboxcontainer_createnamepopup);
	timer_lineedit_tree_vboxcontainer_createnamepopup->set_name("Timer");
	timer_lineedit_tree_vboxcontainer_createnamepopup->set_wait_time(0.65);

	PopupMenu *popupmenu_lineedit_tree_vboxcontainer_createnamepopup = memnew(PopupMenu);
	popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_name("PopupMenu");
	lineedit_tree_vboxcontainer_createnamepopup->add_child(popupmenu_lineedit_tree_vboxcontainer_createnamepopup);
	popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_name("PopupMenu");
	//popupmenu_lineedit_tree_vboxcontainer_createnamepopup property items TYPE_ARRAY value: [Cut, [Object:null], 0, False, False, 0, 268435544, Null, , False, Copy, [Object:null], 0, False, False, 1, 268435523, Null, , False, Paste, [Object:null], 0, False, False, 2, 268435542, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Select All, [Object:null], 0, False, False, 4, 268435521, Null, , False, Clear, [Object:null], 0, False, False, 3, 0, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Undo, [Object:null], 0, False, False, 5, 268435546, Null, , False, Redo, [Object:null], 0, False, False, 6, 301989978, Null, , False]

	Timer *timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup = memnew(Timer);
	timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_name("Timer");
	popupmenu_lineedit_tree_vboxcontainer_createnamepopup->add_child(timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup);
	timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_name("Timer");
	timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_wait_time(0.3);
	timer_popupmenu_lineedit_tree_vboxcontainer_createnamepopup->set_one_shot(True);

	HSlider *hslider_tree_vboxcontainer_createnamepopup = memnew(HSlider);
	hslider_tree_vboxcontainer_createnamepopup->set_name("HSlider");
	tree_vboxcontainer_createnamepopup->add_child(hslider_tree_vboxcontainer_createnamepopup);
	hslider_tree_vboxcontainer_createnamepopup->set_name("HSlider");
	hslider_tree_vboxcontainer_createnamepopup->set_visible(False);

	HScrollBar *hscrollbar_tree_vboxcontainer_createnamepopup = memnew(HScrollBar);
	hscrollbar_tree_vboxcontainer_createnamepopup->set_name("HScrollBar");
	tree_vboxcontainer_createnamepopup->add_child(hscrollbar_tree_vboxcontainer_createnamepopup);
	hscrollbar_tree_vboxcontainer_createnamepopup->set_name("HScrollBar");

	VScrollBar *vscrollbar_tree_vboxcontainer_createnamepopup = memnew(VScrollBar);
	vscrollbar_tree_vboxcontainer_createnamepopup->set_name("VScrollBar");
	tree_vboxcontainer_createnamepopup->add_child(vscrollbar_tree_vboxcontainer_createnamepopup);
	vscrollbar_tree_vboxcontainer_createnamepopup->set_name("VScrollBar");
	vscrollbar_tree_vboxcontainer_createnamepopup->set_custom_step(14);

	Timer *timer_tree_vboxcontainer_createnamepopup = memnew(Timer);
	timer_tree_vboxcontainer_createnamepopup->set_name("Timer");
	tree_vboxcontainer_createnamepopup->add_child(timer_tree_vboxcontainer_createnamepopup);
	timer_tree_vboxcontainer_createnamepopup->set_name("Timer");

	Label *label_vboxcontainer_createnamepopup = memnew(Label);
	label_vboxcontainer_createnamepopup->set_name("Label");
	vboxcontainer_createnamepopup->add_child(label_vboxcontainer_createnamepopup);
	label_vboxcontainer_createnamepopup->set_name("Label");
	//label_vboxcontainer_createnamepopup property owner TYPE_OBJECT value: CreateNamePopup:[ConfirmationDialog:280461]
	label_vboxcontainer_createnamepopup->set_visible(False);
	label_vboxcontainer_createnamepopup->set_margin_top(354);
	label_vboxcontainer_createnamepopup->set_margin_right(475);
	label_vboxcontainer_createnamepopup->set_margin_bottom(368);
	label_vboxcontainer_createnamepopup->set_rect_position(Vector2(0, 354));
	label_vboxcontainer_createnamepopup->set_rect_global_position(Vector2(0, 354));
	label_vboxcontainer_createnamepopup->set_rect_size(Vector2(475, 14));
	label_vboxcontainer_createnamepopup->set_size_flags_horizontal(3);
	label_vboxcontainer_createnamepopup->set_text("Name");

	LineEdit *lineedit_vboxcontainer_createnamepopup = memnew(LineEdit);
	lineedit_vboxcontainer_createnamepopup->set_name("LineEdit");
	vboxcontainer_createnamepopup->add_child(lineedit_vboxcontainer_createnamepopup);
	lineedit_vboxcontainer_createnamepopup->set_name("LineEdit");
	//lineedit_vboxcontainer_createnamepopup property owner TYPE_OBJECT value: CreateNamePopup:[ConfirmationDialog:280461]
	lineedit_vboxcontainer_createnamepopup->set_visible(False);
	lineedit_vboxcontainer_createnamepopup->set_margin_top(372);
	lineedit_vboxcontainer_createnamepopup->set_margin_right(475);
	lineedit_vboxcontainer_createnamepopup->set_margin_bottom(396);
	lineedit_vboxcontainer_createnamepopup->set_rect_position(Vector2(0, 372));
	lineedit_vboxcontainer_createnamepopup->set_rect_global_position(Vector2(0, 372));
	lineedit_vboxcontainer_createnamepopup->set_rect_size(Vector2(475, 24));
	lineedit_vboxcontainer_createnamepopup->set_size_flags_horizontal(3);
	lineedit_vboxcontainer_createnamepopup->set_caret_blink(True);

	Timer *timer_lineedit_vboxcontainer_createnamepopup = memnew(Timer);
	timer_lineedit_vboxcontainer_createnamepopup->set_name("Timer");
	lineedit_vboxcontainer_createnamepopup->add_child(timer_lineedit_vboxcontainer_createnamepopup);
	timer_lineedit_vboxcontainer_createnamepopup->set_name("Timer");
	timer_lineedit_vboxcontainer_createnamepopup->set_wait_time(0.65);

	PopupMenu *popupmenu_lineedit_vboxcontainer_createnamepopup = memnew(PopupMenu);
	popupmenu_lineedit_vboxcontainer_createnamepopup->set_name("PopupMenu");
	lineedit_vboxcontainer_createnamepopup->add_child(popupmenu_lineedit_vboxcontainer_createnamepopup);
	popupmenu_lineedit_vboxcontainer_createnamepopup->set_name("PopupMenu");
	//popupmenu_lineedit_vboxcontainer_createnamepopup property items TYPE_ARRAY value: [Cut, [Object:null], 0, False, False, 0, 268435544, Null, , False, Copy, [Object:null], 0, False, False, 1, 268435523, Null, , False, Paste, [Object:null], 0, False, False, 2, 268435542, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Select All, [Object:null], 0, False, False, 4, 268435521, Null, , False, Clear, [Object:null], 0, False, False, 3, 0, Null, , False, , [Object:null], 0, False, False, -1, 0, Null, , True, Undo, [Object:null], 0, False, False, 5, 268435546, Null, , False, Redo, [Object:null], 0, False, False, 6, 301989978, Null, , False]

	Timer *timer_popupmenu_lineedit_vboxcontainer_createnamepopup = memnew(Timer);
	timer_popupmenu_lineedit_vboxcontainer_createnamepopup->set_name("Timer");
	popupmenu_lineedit_vboxcontainer_createnamepopup->add_child(timer_popupmenu_lineedit_vboxcontainer_createnamepopup);
	timer_popupmenu_lineedit_vboxcontainer_createnamepopup->set_name("Timer");
	timer_popupmenu_lineedit_vboxcontainer_createnamepopup->set_wait_time(0.3);
	timer_popupmenu_lineedit_vboxcontainer_createnamepopup->set_one_shot(True);
}

CreateNamePopup::~CreateNamePopup() {
}

static void CreateNamePopup::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_line_edit_path"), &CreateNamePopup::get_line_edit_path);
	ClassDB::bind_method(D_METHOD("set_line_edit_path", "value"), &CreateNamePopup::set_line_edit_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "line_edit_path"), "set_line_edit_path", "get_line_edit_path");

	ClassDB::bind_method(D_METHOD("get_tree_path"), &CreateNamePopup::get_tree_path);
	ClassDB::bind_method(D_METHOD("set_tree_path", "value"), &CreateNamePopup::set_tree_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "tree_path"), "set_tree_path", "get_tree_path");

	ClassDB::bind_method(D_METHOD("get_type_folders"), &CreateNamePopup::get_type_folders);
	ClassDB::bind_method(D_METHOD("set_type_folders", "value"), &CreateNamePopup::set_type_folders);
	ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "type_folders"), "set_type_folders", "get_type_folders");

	ClassDB::bind_method(D_METHOD("get__resource_type"), &CreateNamePopup::get__resource_type);
	ClassDB::bind_method(D_METHOD("set__resource_type", "value"), &CreateNamePopup::set__resource_type);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "_resource_type"), "set__resource_type", "get__resource_type");

	ClassDB::bind_method(D_METHOD("get_*_line_edit"), &CreateNamePopup::get_ * _line_edit);
	ClassDB::bind_method(D_METHOD("set_*_line_edit", "value"), &CreateNamePopup::set_ * _line_edit);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_line_edit", PROPERTY_HINT_RESOURCE_TYPE, "LineEdit"), "set_*_line_edit", "get_*_line_edit");

	ClassDB::bind_method(D_METHOD("get_*_tree"), &CreateNamePopup::get_ * _tree);
	ClassDB::bind_method(D_METHOD("set_*_tree", "value"), &CreateNamePopup::set_ * _tree);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "*_tree", PROPERTY_HINT_RESOURCE_TYPE, "Tree"), "set_*_tree", "get_*_tree");

	ClassDB::bind_method(D_METHOD("_ready"), &CreateNamePopup::_ready);
	ClassDB::bind_method(D_METHOD("set_resource_type", "resource_type"), &CreateNamePopup::set_resource_type);
	ClassDB::bind_method(D_METHOD("about_to_show"), &CreateNamePopup::about_to_show);
	ClassDB::bind_method(D_METHOD("evaluate_folder", "folder", "root"), &CreateNamePopup::evaluate_folder);
	ClassDB::bind_method(D_METHOD("_on_OK_pressed"), &CreateNamePopup::_on_OK_pressed);
}
