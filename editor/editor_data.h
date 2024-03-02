#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

/*************************************************************************/
/*  editor_data.h                                                        */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "core/object/object.h"

#include "scene/resources/texture.h"

#include "core/containers/hash_map.h"
#include "core/containers/list.h"
#include "core/containers/pair.h"
#include "core/containers/rb_map.h"
#include "core/containers/rb_set.h"
#include "core/containers/vector.h"
#include "core/object/func_ref.h"
#include "core/object/object_id.h"
#include "core/object/reference.h"
#include "core/object/script_language.h"
#include "core/object/undo_redo.h"
#include "core/string/node_path.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/typedefs.h"
#include "core/variant/array.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_EDITOR_CODE_EDITOR_ENABLED
#include "editor_code_editor/script_editor_plugin.h"
#endif

class ConfigFile;
class EditorPlugin;
class Node;
class Resource;

class EditorHistory {
	enum {

		HISTORY_MAX = 64
	};

	struct Obj {
		REF ref;
		ObjectID object;
		String property;
		bool inspector_only;
	};

	struct History {
		Vector<Obj> path;
		int level;
	};
	friend class EditorData;

	Vector<History> history;
	int current;

	//Vector<EditorPlugin*> editor_plugins;

	struct PropertyData {
		String name;
		Variant value;
	};

	void _add_object(ObjectID p_object, const String &p_property, int p_level_change, bool p_inspector_only = false);

public:
	void cleanup_history();

	bool is_at_beginning() const;
	bool is_at_end() const;

	void add_object_inspector_only(ObjectID p_object);
	void add_object(ObjectID p_object);
	void add_object(ObjectID p_object, const String &p_subprop);
	void add_object(ObjectID p_object, int p_relevel);

	int get_history_len();
	int get_history_pos();
	ObjectID get_history_obj(int p_obj) const;
	bool is_history_obj_inspector_only(int p_obj) const;

	bool next();
	bool previous();
	ObjectID get_current();
	bool is_current_inspector_only() const;

	int get_path_size() const;
	ObjectID get_path_object(int p_index) const;
	String get_path_property(int p_index) const;

	void clear();

	EditorHistory();
};

class EditorSelection;

class EditorData {
public:
	struct CustomType {
		String name;
		Ref<Script> script;
		Ref<Texture> icon;
	};

	struct EditedScene {
		Node *root;
		String path;
		uint64_t file_modified_time = 0;
		Dictionary editor_states;
		List<Node *> selection;
		Vector<EditorHistory::History> history_stored;
		int history_current;
		Dictionary custom_state;
		uint64_t version;
		NodePath live_edit_root;
	};

private:
	Vector<EditorPlugin *> editor_plugins;

	struct PropertyData {
		String name;
		Variant value;
	};
	RBMap<String, Vector<CustomType>> custom_types;

	List<PropertyData> clipboard;
	UndoRedo undo_redo;

	RBMap<StringName, Ref<FuncRef>> move_element_functions;

	Vector<EditedScene> edited_scene;
	int current_edited_scene;

	bool _find_updated_instances(Node *p_root, Node *p_node, RBSet<String> &checked_paths);

	HashMap<StringName, String> _script_class_icon_paths;

public:
	EditorPlugin *get_editor(Object *p_object);
	EditorPlugin *get_subeditor(Object *p_object);
	Vector<EditorPlugin *> get_subeditors(Object *p_object);
	EditorPlugin *get_editor(String p_name);

	void copy_object_params(Object *p_object);
	void paste_object_params(Object *p_object);

	Dictionary get_editor_states() const;
	Dictionary get_scene_editor_states(int p_idx) const;
	void set_editor_states(const Dictionary &p_states);
	void get_editor_breakpoints(List<String> *p_breakpoints);
	void clear_editor_states();
	void save_editor_external_data();
	void apply_changes_in_editors();

	void add_editor_plugin(EditorPlugin *p_plugin);
	void remove_editor_plugin(EditorPlugin *p_plugin);

	int get_editor_plugin_count() const;
	EditorPlugin *get_editor_plugin(int p_idx);

	// Function should have this signature: void (Object* undo_redo, Object *modified_object, String array_prefix, int element_index, int new_position)
	void add_move_array_element_function(const StringName &p_class, const Ref<FuncRef> &p_funcref);
	void remove_move_array_element_function(const StringName &p_class);
	Ref<FuncRef> get_move_array_element_function(const StringName &p_class) const;

	UndoRedo &get_undo_redo();

	void save_editor_global_states();
	void restore_editor_global_states();

	void add_custom_type(const String &p_type, const String &p_inherits, const Ref<Script> &p_script, const Ref<Texture> &p_icon);
	Variant instance_custom_type(const String &p_type, const String &p_inherits);
	void remove_custom_type(const String &p_type);
	const RBMap<String, Vector<CustomType>> &get_custom_types() const { return custom_types; }

	int add_edited_scene(int p_at_pos);
	void move_edited_scene_index(int p_idx, int p_to_idx);
	void remove_scene(int p_idx);
	void set_edited_scene(int p_idx);
	void set_edited_scene_root(Node *p_root);
	int get_edited_scene() const;
	Node *get_edited_scene_root(int p_idx = -1);
	int get_edited_scene_count() const;
	Vector<EditedScene> get_edited_scenes() const;
	String get_scene_title(int p_idx, bool p_always_strip_extension = false) const;
	String get_scene_path(int p_idx) const;
	String get_scene_type(int p_idx) const;
	void set_scene_path(int p_idx, const String &p_path);
	Ref<Script> get_scene_root_script(int p_idx) const;
	void set_edited_scene_version(uint64_t version, int p_scene_idx = -1);
	uint64_t get_edited_scene_version() const;
	uint64_t get_scene_version(int p_idx) const;
	void set_scene_modified_time(int p_idx, uint64_t p_time);
	uint64_t get_scene_modified_time(int p_idx) const;
	void clear_edited_scenes();
	void set_edited_scene_live_edit_root(const NodePath &p_root);
	NodePath get_edited_scene_live_edit_root();
	bool check_and_update_scene(int p_idx);
	void move_edited_scene_to_index(int p_idx);
	bool call_build();

	void set_plugin_window_layout(Ref<ConfigFile> p_layout);
	void get_plugin_window_layout(Ref<ConfigFile> p_layout);

	void save_edited_scene_state(EditorSelection *p_selection, EditorHistory *p_history, const Dictionary &p_custom);
	Dictionary restore_edited_scene_state(EditorSelection *p_selection, EditorHistory *p_history);
	void notify_edited_scene_changed();
	void notify_resource_saved(const Ref<Resource> &p_resource);

	bool class_equals_or_inherits(const StringName &p_class, const StringName &p_inherits) const;
	bool script_class_is_parent(const StringName &p_class, const StringName &p_inherits) const;
	StringName script_class_get_base(const StringName &p_class) const;
	Variant script_class_instance(const StringName &p_class) const;

	Ref<Script> script_class_get_base_from_anonymous_path(const String &p_path) const;
	StringName type_get_from_anonymous_path(const String &p_path) const;

	String script_class_get_icon_path(const StringName &p_class) const;
	void script_class_set_icon_path(const StringName &p_class, const String &p_icon_path);
	void script_class_clear_icon_paths() { _script_class_icon_paths.clear(); }
	void script_class_save_icon_paths();
	void script_class_load_icon_paths();

	EditorData();
};

class EditorSelection : public Object {
	GDCLASS(EditorSelection, Object);

private:
	RBMap<Node *, Object *> selection;

	bool emitted;
	bool changed;
	bool nl_changed;

	void _node_removed(Node *p_node);

	List<Object *> editor_plugins;
	List<Node *> selected_node_list;

	void _update_nl();
	Array _get_transformable_selected_nodes();
	void _emit_change();

protected:
	static void _bind_methods();

public:
	Array get_selected_nodes();
	void add_node(Node *p_node);
	void remove_node(Node *p_node);
	bool is_selected(Node *) const;

	template <class T>
	T *get_node_editor_data(Node *p_node) {
		if (!selection.has(p_node)) {
			return nullptr;
		}
		return Object::cast_to<T>(selection[p_node]);
	}

	void add_editor_plugin(Object *p_object);

	void update();
	void clear();

	List<Node *> &get_selected_node_list();
	List<Node *> get_full_selected_node_list();
	RBMap<Node *, Object *> &get_selection() { return selection; }

	EditorSelection();
	~EditorSelection();
};

#endif // EDITOR_DATA_H
