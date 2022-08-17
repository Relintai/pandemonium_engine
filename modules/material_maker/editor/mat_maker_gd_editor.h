#ifndef MAT_MAKER_GD_EDITOR_H
#define MAT_MAKER_GD_EDITOR_H

#include "core/math/vector2.h"
#include "core/node_path.h"
#include "core/reference.h"
#include "core/undo_redo.h"
#include "core/ustring.h"
#include "core/containers/vector.h"

#include "modules/material_maker/editor/mm_create_name_popup.h"
#include "scene/gui/margin_container.h"

class GraphEdit;
class EditorPlugin;
class UndoRedo;
class MMMaterial;
class GraphNode;
class MMGraphNode;
class MMNode;

class MatMakerGDEditor : public MarginContainer {
	GDCLASS(MatMakerGDEditor, MarginContainer);

public:
	GraphEdit *get_graph_edit();

	Ref<MMMaterial> get_mm_material();
	void set_mm_material(const Ref<MMMaterial> &val);

	int get_ignore_material_change_event() const;
	void set_ignore_material_change_event(const int val);

	void ignore_changes(const bool val);

	bool get_recreation_in_progress() const;
	void set_recreation_in_progress(const bool val);

	UndoRedo *get_undo_redo();
	void set_undo_redo(UndoRedo *ur);

	void recreate();
	MMGraphNode *find_graph_node_for(const Ref<MMNode> &nnode);

	void on_material_changed();

	void on_graph_edit_connection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_edit_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_node_close_request(Node *node);
	void _on_AddButton_pressed();
	void _on_AddPopup_ok_pressed(const int type, const String &data);

	MatMakerGDEditor();
	~MatMakerGDEditor();

protected:
	void _notification(int p_what);
	static void _bind_methods();

	GraphEdit *_graph_edit;
	MMCreateNamePopup *_create_popup;
	Ref<MMMaterial> _material;
	int _ignore_material_change_event;
	bool _recreation_in_progress;
	EditorPlugin *_plugin;

private:
	UndoRedo *_undo_redo;
};

#endif
