#ifndef TEXT_EDITOR_FILE_H
#define TEXT_EDITOR_FILE_H

#include "core/string/ustring.h"

#include "core/object/resource.h"

class TextEditorFile : public Resource {
	GDCLASS(TextEditorFile, Resource);

public:
	String get_file_path();
	void set_file_path(const String &file_path);

protected:
	static void _bind_methods();

private:
	String _file_path;
};

#endif
