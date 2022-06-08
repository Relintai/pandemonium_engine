
#include "mm_create_name_popup.h"

#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/tree.h"

void MMCreateNamePopup::about_to_show() {
	_tree->clear();
	TreeItem *root = _tree->create_item();

	//for (s in type_folders) {
	//	evaluate_folder(s, root);
	//}
}

/*
void MMCreateNamePopup::evaluate_folder(const String &folder, const TreeItem &root) {
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
*/

void MMCreateNamePopup::_on_OK_pressed() {
	TreeItem *selected = _tree->get_selected();

	if (selected) {
		if (!selected->has_meta("file")) {
			hide();
			return;
		}

		String file_name = selected->get_meta("file");
		emit_signal("ok_pressed", file_name);
	}

	hide();
}

MMCreateNamePopup::MMCreateNamePopup() {
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
