
#include "text_file_editor.h"

#include "core/array.h"
#include "core/engine.h"
#include "text_editor_settings.h"
#include "text_editor_vanilla_editor.h"

#include "scene/gui/box_container.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/item_list.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/option_button.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/split_container.h"
#include "scene/resources/dynamic_font.h"

void TextFileEditor::_ready() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	clean_editor();
	connect_signals();

	Array opened_files = last_opened_files->load_opened_files();

	for (int i = 0; i < opened_files.size(); ++i) {
		Array opened_file = opened_files[i];

		open_file(opened_file[1], opened_file[2]);
	}

	file_list->set_filters(EXTENSIONS);
}

void TextFileEditor::connect_signals() {
	file_list->connect("confirmed", this, "update_list");
	file_btn_popup->connect("id_pressed", this, "_on_file_btn_pressed");
	preview_btn_popup->connect("id_pressed", this, "_on_preview_btn_pressed");
	settings_btn_popup->connect("id_pressed", this, "_on_settings_btn_pressed");
	_open_file_list->connect("item_selected", this, "_on_fileitem_pressed");
	wrap_btn->connect("item_selected", this, "on_wrap_button");
	map_btn->connect("item_selected", this, "on_minimap_button");
	select_font_dialog->connect("file_selected", this, "_on_font_selected");
}

void TextFileEditor::create_selected_file() {
	update_list();

	file_list->set_mode(FileDialog::MODE_SAVE_FILE);
	file_list->set_title("Create a new File");

	if (file_list->is_connected("file_selected", this, "delete_file")) {
		file_list->disconnect("file_selected", this, "delete_file");
	}

	if (file_list->is_connected("file_selected", this, "open_file")) {
		file_list->disconnect("file_selected", this, "open_file");
	}

	if (!file_list->is_connected("file_selected", this, "create_new_file")) {
		file_list->connect("file_selected", this, "create_new_file");
	}

	open_file_list();
}

void TextFileEditor::open_selected_file() {
	update_list();

	file_list->set_mode(FileDialog::MODE_OPEN_FILE);
	file_list->set_title("Select a File you want to edit");

	if (file_list->is_connected("file_selected", this, "delete_file")) {
		file_list->disconnect("file_selected", this, "delete_file");
	}

	if (file_list->is_connected("file_selected", this, "create_new_file")) {
		file_list->disconnect("file_selected", this, "create_new_file");
	}

	if (!file_list->is_connected("file_selected", this, "open_file")) {
		file_list->connect("file_selected", this, "open_file");
	}

	open_file_list();
}

void TextFileEditor::delete_selected_file() {
	update_list();

	file_list->set_mode(FileDialog::MODE_OPEN_FILES);
	file_list->set_title("Select one or more Files you want to delete");

	if (file_list->is_connected("file_selected", this, "open_file")) {
		file_list->disconnect("file_selected", this, "open_file");
	}

	if (file_list->is_connected("file_selected", this, "create_new_file")) {
		file_list->disconnect("file_selected", this, "create_new_file");
	}

	if (!file_list->is_connected("files_selected", this, "delete_file")) {
		file_list->connect("files_selected", this, "delete_file");
	}

	open_file_list();
}

void TextFileEditor::save_current_file_as() {
	update_list();

	file_list->set_mode(FileDialog::MODE_SAVE_FILE);
	file_list->set_title("Save this File as...");

	if (file_list->is_connected("file_selected", this, "delete_file")) {
		file_list->disconnect("file_selected", this, "delete_file");
	}

	if (file_list->is_connected("file_selected", this, "open_file")) {
		file_list->disconnect("file_selected", this, "open_file");
	}

	if (!file_list->is_connected("file_selected", this, "create_new_file")) {
		file_list->connect("file_selected", this, "create_new_file");
	}

	open_file_list();
}

void TextFileEditor::_on_file_btn_pressed(const int index) {
	switch (index) {
		case FILE_MENU_OPTION_NEW: {
			create_selected_file();
		} break;
		case FILE_MENU_OPTION_OPEN: {
			open_selected_file();
		} break;
		case FILE_MENU_OPTION_CLOSE: {
			if (current_file_index != -1 && current_file_path != "") {
				close_file(current_file_index);
			}
		} break;
		case FILE_MENU_OPTION_SAVE: {
			if (current_file_index != -1 && current_file_path != "") {
				save_as = false;
				save_file(current_file_path);
			}
		} break;
		case FILE_MENU_OPTION_SAVE_AS: {
			if (current_file_index != -1 && current_file_path != "") {
				save_as = true;
				save_file(current_file_path);
				save_current_file_as();
			}
		} break;
		case FILE_MENU_OPTION_DELETE: {
			delete_selected_file();
		} break;
		case FILE_MENU_OPTION_SEARCH: {
			current_editor->open_search_box();
		} break;
		case FILE_MENU_OPTION_REPLACE: {
			current_editor->open_replace_box();
		} break;
		default:
			break;
	}
}

void TextFileEditor::_on_preview_btn_pressed(const int id) {
	if (id == 0) {
		bbcode_preview();
	} else if (id == 1) {
		markdown_preview();
	} else if (id == 2) {
		html_preview();
	} else if (id == 3) {
		csv_preview();
	}
}

void TextFileEditor::_on_settings_btn_pressed(const int index) {
	if (index == 0) {
		select_font_dialog->popup();
	}
}

void TextFileEditor::_on_font_selected(const String &font_path) {
	current_editor->set_font(font_path);
	last_opened_files->store_editor_fonts(current_file_path.get_file(), font_path);
}

void TextFileEditor::_on_fileitem_pressed(const int index) {
	current_file_index = index;
	Array selected_item_metadata = open_file_list->get_item_metadata(current_file_index);
	String extension = selected_item_metadata[0].current_path.get_file().get_extension();

	if (_open_file_list->get_item_text(current_file_index).begins_with("(*)")) {
		editing_file = true;
	} else {
		editing_file = false;
	}

	current_file_path = selected_item_metadata[0].current_path;

	if (current_editor->get_visible() || current_editor == nullptr) {
		if (current_editor != nullptr) {
			current_editor.hide();
		}

		current_editor = selected_item_metadata[0];
		current_editor->show();
		open_file_name->set_text(current_editor->current_path);

		if (wrap_btn->get_selected_id() == 1) {
			current_editor->set_wrap_enabled(true);
		} else {
			current_editor->set_wrap_enabled(false);
		}

		if (map_btn->get_selected_id() == 1) {
			current_editor->draw_minimap(true);
		} else {
			current_editor->draw_minimap(false);
		}
	}
}

void TextFileEditor::open_file(const String &path, const String &font) {
	if (current_file_path != path) {
		current_file_path = path;
		TextEditorVanillaEditor *vanilla_editor = open_in_vanillaeditor(path);

		if (font != "null" && vanilla_editor.get("custom_fonts/font") != nullptr) {
			vanilla_editor->set_font(font);
		}

		generate_file_item(path, vanilla_editor);
		last_opened_files->store_opened_files(_open_file_list);
	}

	current_editor->show();
}

void TextFileEditor::generate_file_item(const String &path, const Control &veditor) {
	open_file_name->set_text(path);
	_open_file_list->add_item(path.get_file(), nullptr, true);
	current_file_index = _open_file_list.get_item_count() - 1;
	open_file_list->set_item_metadata(current_file_index, [veditor]);
	_open_file_list->select(_open_file_list->get_item_count() - 1);
}

Control TextFileEditor::open_in_vanillaeditor(const String &path) {
	Control *editor = TextEditorVanillaEditor.new();
	editor.last_opened_files = TextEditorSettings;
	editor.file_list = file_list;
	split_editor_container.add_child(editor, true);

	if (current_editor && current_editor != editor) {
		editor.show();
		current_editor.hide();
	}

	current_editor = editor;
	editor.connect("text_changed", this, "_on_vanillaeditor_text_changed");

	File *current_file = memnew(File);
	current_file.open(path, File.READ);

	String current_content = "";
	current_content = current_file.get_as_text();
	Variant = OS.get_datetime_from_unix_time(current_file.get_modified_time(path));
	current_file.close();
	editor.new_file_open(current_content, last_modified, current_file_path);
	update_list();

	if (wrap_btn.get_selected_id() == 1) {
		current_editor.set_wrap_enabled(true);
	}

	return editor;
}

void TextFileEditor::close_file(const int index) {
	if (editing_file) {
		confirmation_close.popup();
	} else {
		confirm_close(index);
	}
}

void TextFileEditor::confirm_close(const int index) {
	last_opened_files->remove_opened_file(index, open_file_list);
	open_file_list->remove_item(index);
	open_file_name->clear();
	current_editor->queue_free();

	if (index > 0) {
		open_file_list->select(index - 1);
		_on_fileitem_pressed(index - 1);
	}
}

void TextFileEditor::_on_update_file() {
	File *current_file = memnew(File);
	current_file.open(current_file_path, File.READ);

	String current_content = current_file.get_as_text();
	Dictionary last_modified = OS::get_singleton()->get_datetime_from_unix_time(current_file->get_modified_time(current_file_path));
	current_file->close();
	current_editor->new_file_open(current_content, last_modified, current_file_path);
}

void TextFileEditor::delete_file(const PoolStringArray &files_selected) {
	Directory dir = Directory.new();

	for (file in files_selected) {
		dir.remove(file);
	}

	update_list();
}

void TextFileEditor::open_new_file_dialogue() {
	new_file_dialogue.popup();
	new_file_dialogue.set_position(OS::get_singleton()->get_screen_size() / 2 - new_file_dialogue->get_size() / 2);
}

void TextFileEditor::open_file_list() {
	update_list();
	file_list.popup();
	file_list.set_position(OS::get_singleton()->get_screen_size() / 2 - file_list->get_size() / 2);
}

void TextFileEditor::create_new_file(const String &given_path) {
	File *current_file = File.new();
	current_file.open(given_path, File.WRITE);

	if (save_as) {
		current_file.store_line(current_editor.text_editor.get_text());
	}

	current_file.close();
	open_file(given_path);
}

void TextFileEditor::save_file(const String &current_path) {
	//print("Saving file: ", current_path);

	File *current_file = File.new();
	current_file.open(current_path, File.WRITE);
	String current_content = "";
	int lines = current_editor.text_editor.get_line_count();

	for (int line = 0; line < lines; ++line) { //line in range(lines)
		//if current_editor.text_editor.get_line(line) == "":;
		//	continue;
		current_content = current_editor.text_editor.get_text();
		current_file.store_line(current_editor.text_editor.get_line(line));
	}

	current_file.close();
	current_file_path = current_path;
	Dictionary last_modified = OS::get_singleton()->get_datetime_from_unix_time(current_file->get_modified_time(current_file_path));
	current_editor->update_lastmodified(last_modified, "save");
	open_file_list->set_item_metadata(current_file_index, [current_editor]);

	if (open_file_list->get_item_text(current_file_index).begins_with("(*)")) {
		open_file_list->set_item_text(current_file_index, open_file_list->get_item_text(current_file_index).lstrip("(*)"));
		editing_file = false;
	}

	update_list();
}

void TextFileEditor::clean_editor() {
	Array nodes = get_tree()->get_nodes_in_group("vanilla_editor");

	for (int i = 0; i < nodes.size(); ++i) { //i in range(nodes.size())
		Node *vanillaeditor = nodes[i];
		vanillaeditor.queue_free();
	}

	open_file_name.clear();
	open_file_list.clear();
}

void TextFileEditor::csv_preview() {
	TextEditorPreview *preview = memnew(TextEditorPreview);
	get_parent().get_parent().get_parent().add_child(preview);
	preview->popup();
	preview->set_window_title(" (" + current_file_path.get_file() + ")");
	int lines = current_editor->text_editor->get_line_count();
	Array rows = Array();

	for (int i = 0; i < lines - 1; ++i) { //i in range(0, lines-1)
		rows.append(current_editor->text_editor->get_line(i).rsplit(",", false));
	}

	preview.print_csv(rows);
}

void TextFileEditor::bbcode_preview() {
	TextEditorPreview *preview = memnew(TextEditorPreview);
	get_parent().get_parent().get_parent().add_child(preview);
	preview->popup();
	preview->set_window_title(" (" + current_file_path.get_file() + ")");
	preview->print_bb(current_editor.text_editor.get_text());
}

void TextFileEditor::markdown_preview() {
	TextEditorPreview *preview = memnew(TextEditorPreview);
	get_parent().get_parent().get_parent().add_child(preview);
	preview->popup();
	preview->set_window_title(" (" + current_file_path.get_file() + ")");
	preview->print_markdown(current_editor.text_editor.get_text());
}

void TextFileEditor::html_preview() {
	TextEditorPreview *preview = memnew(TextEditorPreview);
	get_parent().get_parent().get_parent().add_child(preview);
	preview->popup();
	preview->set_window_title(" (" + current_file_path.get_file() + ")");
	preview->print_html(current_editor.text_editor.get_text());
}

void TextFileEditor::_on_vanillaeditor_text_changed() {
	if (!open_file_list->get_item_text(current_file_index).begins_with("(*)")) {
		open_file_list->set_item_text(current_file_index, "(*)" + open_file_list.get_item_text(current_file_index));
		editing_file = true;
	}
}

void TextFileEditor::update_list() {
	file_list->invalidate();
}

void TextFileEditor::on_wrap_button(const int index) {
	switch (index) {
		case 0: {
			current_editor.set_wrap_enabled(false);
		} break;
		case 1: {
			current_editor.set_wrap_enabled(true);
		} break;
		default:
			break;
	}
}

void TextFileEditor::on_minimap_button(const int index) {
	switch (index) {
		case 0: {
			current_editor.draw_minimap(false);
		} break;
		case 1: {
			current_editor.draw_minimap(true);
		} break;
		default:
			break;
	}
}

void TextFileEditor::check_file_preview(const String &file) {
	// check whether the opened file has a corresponding preview session for its extension;
}

void TextFileEditor::_on_ConfirmationDialog_confirmed() {
	confirm_close(current_file_index);
}

TextFileEditor::TextFileEditor() {
	last_opened_files.instance();

	DIRECTORY = "res://";
	EXCEPTIONS = "addons";

	EXTENSIONS.push_back("*.txt ; Plain Text File");
	EXTENSIONS.push_back("*.rtf ; Rich Text Format File");
	EXTENSIONS.push_back("*.log ; Log File");
	EXTENSIONS.push_back("*.md ; MD File", );
	EXTENSIONS.push_back("*.doc ; WordPad Document", );
	EXTENSIONS.push_back("*.doc ; Microsoft Word Document");
	EXTENSIONS.push_back("*.docm ; Word Open XML Macro-Enabled Document");
	EXTENSIONS.push_back("*.docx ; Microsoft Word Open XML Document");
	EXTENSIONS.push_back("*.bbs ; Bulletin Board System Text", );
	EXTENSIONS.push_back("*.dat ; Data File", ; "*.xml ; XML File");
	EXTENSIONS.push_back("*.sql ; SQL database file");
	EXTENSIONS.push_back("*.json ; JavaScript Object Notation File");
	EXTENSIONS.push_back("*.html ; HyperText Markup Language");
	EXTENSIONS.push_back("*.csv ; Comma-separated values");
	EXTENSIONS.push_back("*.cfg ; Configuration File", );
	EXTENSIONS.push_back("*.ini ; Initialization File (same as .cfg Configuration File)");
	EXTENSIONS.push_back("*.csv ; Comma-separated values File");
	EXTENSIONS.push_back("*.res ; Resource File");

	current_file_index = -1;
	save_as = false;
	current_editor = nullptr;
	editing_file = false;

	set_anchors_and_margins_preset(PRESET_WIDE);
	set_size_flags_vertical(SIZE_EXPAND_FILL);
	set_size_flags_horizontal(SIZE_EXPAND_FILL);

	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	vbc->set_anchors_and_margins_preset(PRESET_WIDE);

	HBoxContainer *tob_bar = memnew(HBoxContainer);
	vbc->add_child(tob_bar);

	file_btn = memnew(MenuButton);
	tob_bar->add_child(file_btn);
	file_btn->set_text("File");
	file_btn_popup = file_btn->get_popup();

	Ref<InputEventKey> hotkey;
	hotkey.instance();
	hotkey->set_scancode(KEY_N);
	hotkey->set_control(true);
	file_btn_popup->add_item("New File", FILE_MENU_OPTION_NEW, hotkey->get_scancode_with_modifiers());

	hotkey.instance();
	hotkey->set_scancode(KEY_O);
	hotkey->set_control(true);
	file_btn_popup->add_item("Open File", FILE_MENU_OPTION_OPEN, hotkey->get_scancode_with_modifiers());

	hotkey.instance();
	hotkey->set_scancode(KEY_C);
	hotkey->set_control(true);
	hotkey->set_alt(true);
	file_btn_popup->add_item("Close File", FILE_MENU_OPTION_CLOSE, hotkey->get_scancode_with_modifiers());

	file_btn_popup->add_separator();

	hotkey.instance();
	hotkey->set_scancode(KEY_S);
	hotkey->set_control(true);
	file_btn_popup->add_item("Save File", FILE_MENU_OPTION_SAVE, hotkey->get_scancode_with_modifiers());

	hotkey.instance();
	hotkey->set_scancode(KEY_S);
	hotkey->set_control(true);
	hotkey->set_alt(true);
	file_btn_popup->add_item("Save File as...", FILE_MENU_OPTION_SAVE_AS, hotkey->get_scancode_with_modifiers());

	//hotkey = InputEventKey.new();
	//hotkey.scancode = KEY_D;
	//hotkey.control = true;
	//file_btn_popup.add_item("Delete File", FileMenuOptions.FILE_MENU_OPTION_DELETE, hotkey.get_scancode_with_modifiers());
	file_btn_popup->add_item("Delete File", FILE_MENU_OPTION_DELETE);

	file_btn_popup->add_separator();

	hotkey.instance();
	hotkey->set_scancode(KEY_F);
	hotkey->set_control(true);
	file_btn_popup->add_item("Search in file...", FILE_MENU_OPTION_SEARCH, hotkey->get_scancode_with_modifiers());

	hotkey.instance();
	hotkey->set_scancode(KEY_R);
	hotkey->set_control(true);
	file_btn_popup->add_item("Replace occurencies", FILE_MENU_OPTION_REPLACE, hotkey->get_scancode_with_modifiers());

	//TextEditorPreview;
	preview_btn = memnew(MenuButton);
	tob_bar->add_child(preview_btn);
	preview_btn->set_text("TextEditorPreview");

	preview_btn_popup = preview_btn->get_popup();
	preview_btn_popup->add_item("BBCode TextEditorPreview");
	preview_btn_popup->add_item("Markdown TextEditorPreview");
	preview_btn_popup->add_item("HTML TextEditorPreview");
	preview_btn_popup->add_item("CSV TextEditorPreview");

	//Settings;
	settings_btn = memnew(MenuButton);
	tob_bar->add_child(settings_btn);
	settings_btn->set_text("Settings");

	settings_btn_popup = settings_btn->get_popup();
	settings_btn_popup->add_item("Change Font");

	//SplitContainer;
	editor_container = memnew(HSplitContainer);
	vbc->add_child(editor_container);
	editor_container->set_split_offset(150);
	editor_container->set_size_flags_horizontal(SIZE_EXPAND_FILL);
	editor_container->set_size_flags_vertical(SIZE_EXPAND_FILL);

	//Files;
	file_container = memnew(VBoxContainer);
	editor_container->add_child(file_container);

	open_file_list = memnew(ItemList);
	file_container->add_child(open_file_list);
	open_file_list->set_allow_reselect(true);
	open_file_list->set_size_flags_vertical(SIZE_EXPAND_FILL);
	file_container->add_child(memnew(HSeparator));

	//Editor;
	split_editor_container = memnew(VBoxContainer);
	editor_container->add_child(split_editor_container);

	HBoxContainer *editor_top_bar = memnew(HBoxContainer);
	split_editor_container->add_child(editor_top_bar);

	Label *edtopbar_label = memnew(Label);
	editor_top_bar->add_child(edtopbar_label);
	edtopbar_label->set_text("Editing file:");

	open_file_name = memnew(LineEdit);
	editor_top_bar->add_child(open_file_name);
	open_file_name->set_editable(false);
	open_file_name->set_mouse_filter(Control.MOUSE_FILTER_PASS);
	open_file_name->set_size_flags_horizontal(SIZE_EXPAND_FILL);

	wrap_btn = memnew(OptionButton);
	editor_top_bar->add_child(wrap_btn);
	wrap_btn->add_item("No Wrap");
	wrap_btn->add_item("Soft Wrap");

	map_btn = memnew(OptionButton);
	editor_top_bar->add_child(map_btn);
	map_btn->add_item("Hide Map");
	map_btn->add_item("Show Map");
	map_btn->set_selected(1);

	//dialogs;
	file_list = memnew(FileDialog);
	add_child(file_list);
	file_list->set_show_hidden_files(true);
	file_list->set_dialog_hide_on_ok(true);
	file_list->set_window_title("Save file");
	file_list->set_popup_exclusive(true);
	file_list->set_anchors_and_margins_preset(Control.PRESET_WIDE);
	file_list->set_margin_left(222);
	file_list->set_margin_top(132);
	file_list->set_margin_right(-221);
	file_list->set_margin_bottom(-131);
	file_list->set_rect_min_size(Vector2(200, 70));

	new_file_dialogue = memnew(AcceptDialog);
	add_child(new_file_dialogue);
	new_file_dialogue->set_window_title("Create new File");

	VBoxContainer *nfd_vbc = memnew(VBoxContainer);
	new_file_dialogue->add_child(nfd_vbc);

	Label *nfd_name = memnew(Label);
	nfd_vbc->add_child(nfd_name);
	nfd_name->set_text("Insert file name (no extension needed)");
	nfd_name->set_align(Label::ALIGN_CENTER);
	nfd_name->set_valign(Label::VALIGN_CENTER);
	nfd_name->set_size_flags_vertical(SIZE_EXPAND_FILL);

	new_file_dialogue_name = memnew(LineEdit);
	nfd_vbc->add_child(new_file_dialogue_name);
	new_file_dialogue_name->set_clear_button_enabled(true);
	new_file_dialogue_name->set_text("example");
	new_file_dialogue_name->set_rect_min_size(Vector2(200, 0));
	new_file_dialogue_name->set_size_flags_horizontal(SIZE_EXPAND | SIZE_SHRINK_CENTER);
	new_file_dialogue_name->set_size_flags_vertical(SIZE_EXPAND_FILL);

	confirmation_close = memnew(ConfirmationDialog);
	add_child(confirmation_close);
	confirmation_close->set_dialog_text("There are some unsaved changes.\nPress \"OK\" if you want to close this tab anyway, or \"cancel\" if you want to keep on editing your file.");
	confirmation_close->set_window_title("Unsaved changes");
	confirmation_close->set_anchors_and_margins_preset(PRESET_CENTER);

	select_font_dialog = memnew(FileDialog);
	add_child(select_font_dialog);
	select_font_dialog->set_mode(FileDialog::MODE_OPEN_FILE);
	select_font_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
	select_font_dialog->set_show_hidden_files(true);
	select_font_dialog->set_window_title("Open a File");
	select_font_dialog->set_resizable(true);
	select_font_dialog->set_anchors_and_margins_preset(PRESET_WIDE);
	select_font_dialog->set_margin_left(222);
	select_font_dialog->set_margin_top(132);
	select_font_dialog->set_margin_right(-221);
	select_font_dialog->set_margin_bottom(-131);
	select_font_dialog->set_rect_min_size(Vector2(200, 70));

	PoolStringArray farr = PoolStringArray();
	farr.push_back("*.TTF");
	farr.push_back("*.ttf");

	select_font_dialog->set_filters(farr);
}

TextFileEditor::~TextFileEditor() {
}

void TextFileEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_ready"), &TextFileEditor::_ready);

	ClassDB::bind_method(D_METHOD("connect_signals"), &TextFileEditor::connect_signals);
	ClassDB::bind_method(D_METHOD("create_selected_file"), &TextFileEditor::create_selected_file);
	ClassDB::bind_method(D_METHOD("open_selected_file"), &TextFileEditor::open_selected_file);
	ClassDB::bind_method(D_METHOD("delete_selected_file"), &TextFileEditor::delete_selected_file);
	ClassDB::bind_method(D_METHOD("save_current_file_as"), &TextFileEditor::save_current_file_as);

	ClassDB::bind_method(D_METHOD("_on_file_btn_pressed", "index"), &TextFileEditor::_on_file_btn_pressed);
	ClassDB::bind_method(D_METHOD("_on_preview_btn_pressed", "id"), &TextFileEditor::_on_preview_btn_pressed);
	ClassDB::bind_method(D_METHOD("_on_settings_btn_pressed", "index"), &TextFileEditor::_on_settings_btn_pressed);
	ClassDB::bind_method(D_METHOD("_on_font_selected", "font_path"), &TextFileEditor::_on_font_selected);
	ClassDB::bind_method(D_METHOD("_on_fileitem_pressed", "index"), &TextFileEditor::_on_fileitem_pressed);

	ClassDB::bind_method(D_METHOD("open_file", "path", "font"), &TextFileEditor::open_file, "null");
	ClassDB::bind_method(D_METHOD("generate_file_item", "path", "veditor"), &TextFileEditor::generate_file_item);
	ClassDB::bind_method(D_METHOD("open_in_vanillaeditor", "path"), &TextFileEditor::open_in_vanillaeditor);

	ClassDB::bind_method(D_METHOD("close_file", "index"), &TextFileEditor::close_file);
	ClassDB::bind_method(D_METHOD("confirm_close", "index"), &TextFileEditor::confirm_close);
	ClassDB::bind_method(D_METHOD("_on_update_file"), &TextFileEditor::_on_update_file);

	ClassDB::bind_method(D_METHOD("delete_file", "files_selected"), &TextFileEditor::delete_file);
	ClassDB::bind_method(D_METHOD("open_new_file_dialogue"), &TextFileEditor::open_new_file_dialogue);
	ClassDB::bind_method(D_METHOD("open_file_list"), &TextFileEditor::open_file_list);
	ClassDB::bind_method(D_METHOD("create_new_file", "given_path"), &TextFileEditor::create_new_file);
	ClassDB::bind_method(D_METHOD("save_file", "current_path"), &TextFileEditor::save_file);

	ClassDB::bind_method(D_METHOD("clean_editor"), &TextFileEditor::clean_editor);
	ClassDB::bind_method(D_METHOD("csv_preview"), &TextFileEditor::csv_preview);
	ClassDB::bind_method(D_METHOD("bbcode_preview"), &TextFileEditor::bbcode_preview);
	ClassDB::bind_method(D_METHOD("markdown_preview"), &TextFileEditor::markdown_preview);
	ClassDB::bind_method(D_METHOD("html_preview"), &TextFileEditor::html_preview);

	ClassDB::bind_method(D_METHOD("_on_vanillaeditor_text_changed"), &TextFileEditor::_on_vanillaeditor_text_changed);
	ClassDB::bind_method(D_METHOD("update_list"), &TextFileEditor::update_list);

	ClassDB::bind_method(D_METHOD("on_wrap_button", "index"), &TextFileEditor::on_wrap_button);
	ClassDB::bind_method(D_METHOD("on_minimap_button", "index"), &TextFileEditor::on_minimap_button);
	ClassDB::bind_method(D_METHOD("check_file_preview", "file"), &TextFileEditor::check_file_preview);

	ClassDB::bind_method(D_METHOD("_on_ConfirmationDialog_confirmed"), &TextFileEditor::_on_ConfirmationDialog_confirmed);
}
