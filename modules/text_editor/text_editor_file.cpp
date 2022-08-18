#include "text_editor_file.h"

String TextEditorFile::get_file_path() {
	return _file_path;
}

void TextEditorFile::set_file_path(const String &file_path) {
	_file_path = file_path;
}

void TextEditorFile::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_file_path"), &TextEditorFile::get_file_path);
	ClassDB::bind_method(D_METHOD("set_file_path", "file_path"), &TextEditorFile::set_file_path);
	//ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path"), "set_file_path", "get_file_path");
}
