#ifndef TEXT_EDITOR_FORMAT_LOADER_H
#define TEXT_EDITOR_FORMAT_LOADER_H



#include "core/io/resource_loader.h"

class TextFileEditor;
class Texture;

class TextEditorTextLoader : public ResourceFormatLoader {
	GDCLASS(TextEditorTextLoader, ResourceFormatLoader);

public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_no_subresource_cache = false);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
	virtual void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false);
};

#endif
