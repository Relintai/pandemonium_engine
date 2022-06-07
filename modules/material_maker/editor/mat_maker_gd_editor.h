#ifndef MAT_MAKER_GD_EDITOR_H
#define MAT_MAKER_GD_EDITOR_H

class MatMakerGDEditor : public MarginContainer {
	GDCLASS(MatMakerGDEditor, MarginContainer);

public:
	Variant get_Variant();
	void set_Variant(const Variant &val);

	NodePath get_graph_edit_path();
	void set_graph_edit_path(const NodePath &val);

	NodePath get_add_popup_path();
	void set_add_popup_path(const NodePath &val);

	GraphEdit get_graph_edit();
	void set_graph_edit(const GraphEdit &val);

	MMMateial get_material();
	void set_material(const MMMateial &val);

	int get__ignore_material_change_event() const;
	void set__ignore_material_change_event(const int val);

	bool get__recreation_in_progress() const;
	void set__recreation_in_progress(const bool val);

	EditorPlugin get_plugin();
	void set_plugin(const EditorPlugin &val);

	UndoRedo get_undo_redo();
	void set_undo_redo(const UndoRedo &val);

	void _enter_tree();
	void set_plugin(const EditorPlugin &plugin);
	UndoRedo get_undo_redo();
	void ensure_objs();
	void recreate();
	Node find_graph_node_for(const Variant &nnode);
	void set_mmmaterial(const MMMateial &object);
	void on_material_changed();
	void ignore_changes(const bool val);
	void on_graph_edit_connection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_edit_disconnection_request(const String &from, const int from_slot, const String &to, const int to_slot);
	void on_graph_node_close_request(const GraphNode &node);
	void _on_AddButton_pressed();
	void _on_AddPopup_ok_pressed(const String &script_path);

	MatMakerGDEditor();
	~MatMakerGDEditor();

protected:
	static void _bind_methods();

	//tool
	Variant = preload("res://addons/mat_maker_gd/editor/mm_graph_node.gd");
	//export(NodePath)
	NodePath graph_edit_path = "VBoxContainer/GraphEdit";
	//export(NodePath)
	NodePath add_popup_path = "Popups/AddPopup";
	GraphEdit *_graph_edit = null;
	MMMateial *_material;
	int _ignore_material_change_event = 0;
	bool _recreation_in_progress = false;
	EditorPlugin *_plugin = null;
	UndoRedo *_undo_redo = null;
};

#endif
