#ifndef TEXT_EDITOR_EDITOR_PLUGIN_H
#define TEXT_EDITOR_EDITOR_PLUGIN_H



#include "core/io/resource_importer.h"
#include "core/object/reference.h"
#include "editor/editor_plugin.h"

class TextFileEditor;
class Texture;

class TextEditorEditorPlugin : public EditorPlugin {
	GDCLASS(TextEditorEditorPlugin, EditorPlugin);

public:
	void make_visible(const bool visible);
	const Ref<Texture> get_icon() const;
	bool has_main_screen() const;
	String get_name() const;
	void edit(Object *p_object);
	bool handles(Object *p_object) const;

	TextEditorEditorPlugin(EditorNode *p_node);
	~TextEditorEditorPlugin();

	EditorNode *editor;

	TextFileEditor *window;

protected:
	void _on_filesystem_dock_entry_pressed(int id);

	static void _bind_methods();

	Ref<Texture> _icon;
	int _filesystem_dock_entry_id;
};

#endif
