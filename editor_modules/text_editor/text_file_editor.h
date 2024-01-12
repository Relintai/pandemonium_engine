#ifndef TEXT_FILE_EDITOR_H
#define TEXT_FILE_EDITOR_H

/*************************************************************************/
/*  text_file_editor.h                                                   */
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

#include "core/containers/vector.h"
#include "core/object/reference.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

#include "scene/main/control.h"

class MenuButton;
class PopupMenu;
class HSplitContainer;
class VBoxContainer;
class ItemList;
class LineEdit;
class OptionButton;
class FileDialog;
class AcceptDialog;
class ConfirmationDialog;
class TextEditorSettings;
class DynamicFont;
class TextEditorVanillaEditor;

class TextFileEditor : public Control {
	GDCLASS(TextFileEditor, Control);

public:
	enum FileMenuOptions {
		FILE_MENU_OPTION_NEW = 0,
		FILE_MENU_OPTION_OPEN = 1,
		FILE_MENU_OPTION_CLOSE = 2,
		FILE_MENU_OPTION_SAVE = 3,
		FILE_MENU_OPTION_SAVE_AS = 4,
		FILE_MENU_OPTION_SEARCH = 5,
		FILE_MENU_OPTION_REPLACE = 6,
	};

	void connect_signals();

	void create_selected_file();
	void open_selected_file();
	void save_current_file_as();

	void _on_file_btn_pressed(const int index);
	void _on_preview_btn_pressed(const int id);
	void _on_settings_btn_pressed(const int index);
	void _on_font_selected(const String &font_path);
	void _on_fileitem_pressed(const int index);

	void open_file(const String &path, const String &font = "");
	void generate_file_item(const String &path, Control *veditor);
	TextEditorVanillaEditor *open_in_vanillaeditor(const String &path);
	void close_file(const int index);
	void confirm_close(const int index);

	void _on_update_file();

	void open_new_file_dialogue();
	void open_file_list();
	void create_new_file(const String &given_path);
	void save_file(const String &current_path);

	void clean_editor();
	void csv_preview();
	void bbcode_preview();
	void markdown_preview();
	void html_preview();

	void _on_vanillaeditor_text_changed();
	void update_list();

	void on_wrap_button(const int index);
	void on_minimap_button(const int index);
	void check_file_preview(const String &file);

	void _on_ConfirmationDialog_confirmed();

	TextFileEditor();
	~TextFileEditor();

protected:
	void _notification(int p_what);

	static void _bind_methods();

	String DIRECTORY;
	String EXCEPTIONS;
	Vector<String> EXTENSIONS;

	MenuButton *file_btn;
	MenuButton *preview_btn;
	MenuButton *settings_btn;

	PopupMenu *file_btn_popup;
	PopupMenu *preview_btn_popup;
	PopupMenu *settings_btn_popup;

	HSplitContainer *editor_container;
	VBoxContainer *file_container;
	ItemList *_open_file_list;
	VBoxContainer *split_editor_container;
	LineEdit *open_file_name;
	OptionButton *wrap_btn;
	OptionButton *map_btn;

	FileDialog *file_list;
	AcceptDialog *new_file_dialogue;
	LineEdit *new_file_dialogue_name;
	ConfirmationDialog *confirmation_close;
	FileDialog *select_font_dialog;

	Ref<TextEditorSettings> _text_editor_settings;

	Array directories;
	Array files;

	int current_file_index;
	String current_file_path;
	bool save_as;

	TextEditorVanillaEditor *current_editor;

	Ref<DynamicFont> current_font;
	bool editing_file;
};

#endif
