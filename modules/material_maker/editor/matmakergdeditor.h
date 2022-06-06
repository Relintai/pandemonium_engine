#ifndef MATMAKERGDEDITOR_H
#define MATMAKERGDEDITOR_H


class MatMakerGDEditor : public MarginContainer {
 GDCLASS(MatMakerGDEditor, MarginContainer);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 NodePath get_graph_edit_path();
 void set_graph_edit_path(const NodePath &val);

 NodePath get_add_popup_path();
 void set_add_popup_path(const NodePath &val);

 GraphEdit get_*_graph_edit();
 void set_*_graph_edit(const GraphEdit &val);

 MMMateial get_*_material();
 void set_*_material(const MMMateial &val);

 int get__ignore_material_change_event() const;
 void set__ignore_material_change_event(const int val);

 bool get__recreation_in_progress() const;
 void set__recreation_in_progress(const bool val);

 EditorPlugin get_*_plugin();
 void set_*_plugin(const EditorPlugin &val);

 UndoRedo get_*_undo_redo();
 void set_*_undo_redo(const UndoRedo &val);

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
 Variant  = preload("res://addons/mat_maker_gd/editor/mm_graph_node.gd");
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
