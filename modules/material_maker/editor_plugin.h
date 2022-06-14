#ifndef MATERIAL_MAKER_EDITOR_PLUGIN_H
#define MATERIAL_MAKER_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

class MatMakerGDEditor;
class ToolButton;

class MaterialMakerEditorPlugin : public EditorPlugin {
	GDCLASS(MaterialMakerEditorPlugin, EditorPlugin);

public:
	bool has_main_screen() const { return false; }
	String get_name() const { return "MaterialMakerEditorPlugin"; }

	void make_visible(bool p_visible);
	void edit(Object *p_object);
	bool handles(Object *p_object) const;

	MaterialMakerEditorPlugin(EditorNode *p_node);
	~MaterialMakerEditorPlugin();

protected:
	void _notification(int p_what);

	MatMakerGDEditor *editor_scene;
	ToolButton *tool_button;

	EditorNode *_editor_node;
};

#endif
