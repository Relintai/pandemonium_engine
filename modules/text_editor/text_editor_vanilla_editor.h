#ifndef TEXT_EDITOR_VANILLA_EDITOR_H
#define TEXT_EDITOR_VANILLA_EDITOR_H

#include "core/os/os.h"
#include "core/reference.h"
#include "core/ustring.h"

#include "scene/gui/box_container.h"

class TextEditorSettings;

class TextEdit;
class FileDialog;
class HBoxContainer;
class LineEdit;
class CheckBox;
class Button;
class TextureRect;
class Label;
class FindReplaceBar;

class TextEditorVanillaEditor : public VBoxContainer {
	GDCLASS(TextEditorVanillaEditor, VBoxContainer);

public:
	String get_current_path();
	void set_current_path(const String &val);

	String get_current_filename();
	void set_current_filename(const String &val);

	int get_search_flag() const;
	void set_search_flag(const int val);

	void set_font(const String &font_path);
	void load_default_font();
	void set_wrap_enabled(const bool enabled);
	void draw_minimap(const bool value);
	void color_region(const String &filextension);
	void clean_editor();
	void new_file_open(const String &file_content, const OS::DateTime &last_modified, const String &current_file_path);
	void update_lastmodified(const OS::DateTime &last_modified, const String &icon);
	void new_file_create(const String &file_name);
	void _on_Readonly_toggled(const bool button_pressed);
	void _on_text_editor_text_changed();
	void count_characters();
	void open_search_box();
	void open_replace_box();

	TextEditorVanillaEditor();
	~TextEditorVanillaEditor();

	FileDialog *file_list;
	TextEdit *text_editor;

protected:
	void _notification(int p_what);

	static void _bind_methods();

	Ref<TextEditorSettings> last_opened_files;

	TextureRect *file_info_last_modified_icon;
	Label *file_info_last_modified;
	Label *file_info_c_counter;
	CheckBox *file_info_read_only;

	FindReplaceBar *_find_replace_bar;

	String current_path;
	String current_filename;

	int search_flag;
};

#endif
