

#include "text_editor_format_loader.h"

#include "text_editor_file.h"

RES TextEditorTextLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_no_subresource_cache) {
	if (r_error) {
		*r_error = ERR_FILE_CANT_OPEN;
	}

	Ref<TextEditorFile> file;
	file.instance();
	file->set_file_path(p_original_path);

	if (r_error) {
		*r_error = OK;
	}

	return file;
}

void TextEditorTextLoader::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("txt");
	p_extensions->push_back("md");
	p_extensions->push_back("xml");
	p_extensions->push_back("sql");
	p_extensions->push_back("csv");
	p_extensions->push_back("cfg");
	p_extensions->push_back("ini");
	p_extensions->push_back("log");
	p_extensions->push_back("json");
	p_extensions->push_back("yml");
	p_extensions->push_back("yaml");
	p_extensions->push_back("toml");
	p_extensions->push_back("html");
	p_extensions->push_back("js");
	p_extensions->push_back("css");
	p_extensions->push_back("htmpl");
}

bool TextEditorTextLoader::handles_type(const String &p_type) const {
	return p_type == "TextEditorFile";
}

String TextEditorTextLoader::get_resource_type(const String &p_path) const {
	return "TextEditorFile";
}

void TextEditorTextLoader::get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types) {
}
