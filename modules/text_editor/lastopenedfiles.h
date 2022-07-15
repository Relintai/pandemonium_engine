#ifndef LASTOPENEDFILES_H
#define LASTOPENEDFILES_H

#include "core/reference.h"

class ItemList;

class LastOpenedFiles : public Reference {
	GDCLASS(LastOpenedFiles, Reference);

public:
	void store_opened_files(ItemList *filecontainer);
	void remove_opened_file(const int index, ItemList *filecontainer);
	Array load_opened_files();
	void store_editor_fonts(const String &file_name, const String &font_path);
	String get_editor_font();

	LastOpenedFiles();
	~LastOpenedFiles();

protected:
	static void _bind_methods();
};

#endif
