
#include "mat_maker_file_dialog.h"

#include "core/variant.h"
#include "left_panel.h"
#include "scene/gui/option_button.h"
#include "scene/gui/split_container.h"

#include "core/os/os.h"

String MatMakerFileDialog::get_full_current_dir() {
	String prefix = "";

	if (_volume_option != nullptr && _volume_option->is_visible()) {
		prefix = _volume_option->get_item_text(_volume_option->get_selected());
	}

	return prefix + get_current_dir();
}

void MatMakerFileDialog::_on_FileDialog_file_selected(const String &path) {
	_left_panel->add_recent(get_full_current_dir());

	Array arr;
	arr.push_back(path);

	emit_signal("return_paths", arr);
}

void MatMakerFileDialog::_on_FileDialog_files_selected(const PoolStringArray &paths) {
	_left_panel->add_recent(get_full_current_dir());
	emit_signal("return_paths", paths);
}

void MatMakerFileDialog::_on_FileDialog_dir_selected(const String &dir) {
	Array arr;
	arr.push_back(dir);

	emit_signal("return_paths", arr);
}

void MatMakerFileDialog::_on_FileDialog_popup_hide() {
	emit_signal("return_paths", Array());
}

Array MatMakerFileDialog::select_files() {
	popup_centered();
	Variant = yield(self, "return_paths");
	queue_free();
	return result;
}

void MatMakerFileDialog::add_favorite() {
	_left_panel->add_favorite(get_full_current_dir());
}

MatMakerFileDialog::MatMakerFileDialog() {
	_left_panel = nullptr;
	_volume_option = nullptr;
}

MatMakerFileDialog::~MatMakerFileDialog() {
}

void MatMakerFileDialog::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		Control *vbox = Object::cast_to<Control>(get_child(3));
		HSplitContainer *hbox = memnew(HSplitContainer);
		add_child(hbox);
		remove_child(vbox);

		_left_panel = memnew(LeftPanel);
		hbox->add_child(_left_panel);
		_left_panel->connect("open_directory", this, "set_current_dir");
		hbox->add_child(vbox);
		vbox->set_h_size_flags(SIZE_EXPAND_FILL);

		Button *fav_button = memnew(Button);
		fav_button->set_text("F");
		fav_button->set_tooltip("Favorite");
		vbox->get_child(0)->add_child(fav_button);

		fav_button->connect("pressed", this, "add_favorite");

		if (OS::get_singleton()->get_name() == "Windows") {
			_volume_option = Object::cast_to<OptionButton>(vbox->get_child(0)->get_child(3));
		}

		connect("dir_selected", this, "_on_FileDialog_dir_selected");
		connect("file_selected", this, "_on_FileDialog_file_selected");
		connect("files_selected", this, "_on_FileDialog_files_selected");
		connect("popup_hide", this, "_on_FileDialog_popup_hide");
	}
}

void MatMakerFileDialog::_bind_methods() {
	ADD_SIGNAL(MethodInfo("return_paths", PropertyInfo(Variant::ARRAY, "path_list")));

	ClassDB::bind_method(D_METHOD("get_full_current_dir"), &MatMakerFileDialog::get_full_current_dir);

	ClassDB::bind_method(D_METHOD("_on_FileDialog_file_selected", "path"), &MatMakerFileDialog::_on_FileDialog_file_selected);
	ClassDB::bind_method(D_METHOD("_on_FileDialog_files_selected", "paths"), &MatMakerFileDialog::_on_FileDialog_files_selected);
	ClassDB::bind_method(D_METHOD("_on_FileDialog_dir_selected", "dir"), &MatMakerFileDialog::_on_FileDialog_dir_selected);
	ClassDB::bind_method(D_METHOD("_on_FileDialog_popup_hide"), &MatMakerFileDialog::_on_FileDialog_popup_hide);
	ClassDB::bind_method(D_METHOD("select_files"), &MatMakerFileDialog::select_files);
	ClassDB::bind_method(D_METHOD("add_favorite"), &MatMakerFileDialog::add_favorite);
}
