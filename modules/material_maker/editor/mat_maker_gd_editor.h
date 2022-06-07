#ifndef MAT_MAKER_GD_EDITOR_H
#define MAT_MAKER_GD_EDITOR_H

#include "core/math/vector2.h"
#include "core/node_path.h"
#include "core/reference.h"
#include "core/undo_redo.h"
#include "core/ustring.h"
#include "core/vector.h"

#include "scene/gui/margin_container.h"

class GraphEdit;
class EditorPlugin;
class UndoRedo;
class MMMaterial;
class GraphNode;
class MMGraphNode;

class MatMakerGDEditor : public MarginContainer {
	GDCLASS(MatMakerGDEditor, MarginContainer);

public:
	Variant get_Variant();
	void set_Variant(const Variant &val);

	NodePath get_graph_edit_path();
	void set_graph_edit_path(const NodePath &val);

	NodePath get_add_popup_path();
	void set_add_popup_path(const NodePath &val);

	GraphEdit *get_graph_edit();
	void set_graph_edit(const GraphEdit *val);

	Ref<MMMaterial> get_mm_material();
	void set_mm_material(const Ref<MMMaterial> &val);

	int get_ignore_material_change_event() const;
	void set_ignore_material_change_event(const int val);

	bool get_recreation_in_progress() const;
	void set_recreation_in_progress(const bool val);

	EditorPlugin *get_plugin();
	void set_plugin(const EditorPlugin *val);

	UndoRedo *get_undo_redo();
	void set_undo_redo(const UndoRedo *val);

	void _enter_tree();

	void ensure_objs();
	void recreate();
	MMGraphNode *find_graph_node_for(const Ref<MMnode> &nnode);
	void set_mmmaterial(const Ref<MMMaterial> &object);
	void on_material_changed();
	void ignore_changes(const bool val);

	void on_graph_edit_connection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_edit_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_node_close_request(GraphNode *node);
	void _on_AddButton_pressed();
	void _on_AddPopup_ok_pressed(const String &script_path);

	MatMakerGDEditor();
	~MatMakerGDEditor();

protected:
	static void _bind_methods();

	NodePath graph_edit_path;
	NodePath add_popup_path;
	GraphEdit *_graph_edit;
	Ref<MMMaterial> *_material;
	int _ignore_material_change_event;
	bool _recreation_in_progress;
	EditorPlugin *_plugin;
	UndoRedo *_undo_redo;
};

#endif
