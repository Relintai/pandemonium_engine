#ifndef TEXT_EDITOR_SETTINGS_H
#define TEXT_EDITOR_SETTINGS_H

#include "core/reference.h"

class ItemList;

class TextEditorSettings : public Reference {
	GDCLASS(TextEditorSettings, Reference);

public:
	void store_opened_files(ItemList *filecontainer);
	void remove_opened_file(const int index, ItemList *filecontainer);
	Array load_opened_files();
	void store_editor_fonts(const String &file_name, const String &font_path);
	String get_editor_font();

	TextEditorSettings();
	~TextEditorSettings();

protected:
	static void _bind_methods();
};

#endif
