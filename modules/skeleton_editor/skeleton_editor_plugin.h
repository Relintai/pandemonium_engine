#ifndef MODULE_SKELETON_EDITOR_PLUGIN_H
#define MODULE_SKELETON_EDITOR_PLUGIN_H
/*************************************************************************/
/*  skeleton_editor_plugin.h                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "core/os/input_event.h"
#include "editor/editor_inspector.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/spatial_editor_gizmos.h"
#include "scene/3d/camera.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/skeleton.h"
#include "scene/resources/immediate_mesh.h"

class EditorInspectorPluginSkeleton;
class Joint;
class PhysicalBone;
class SkeletonEditorPlugin;
class Button;
class CheckBox;
class EditorSpinSlider;
class EditorInspectorSection;
class GridContainer;
class Tree;
class Label;
class PopupMenu;
class CheckBox;
class VSeparator;
class EditorPropertyTransform;
class EditorPropertyVector3;
class EditorPropertyCheck;
class EditorPropertyQuaternion;

class BoneTransformEditor : public VBoxContainer {
	GDCLASS(BoneTransformEditor, VBoxContainer);

	EditorInspectorSection *section;

	EditorPropertyCheck *enabled_checkbox = nullptr;
	EditorPropertyVector3 *position_property = nullptr;
	EditorPropertyQuaternion *rotation_property = nullptr;
	EditorPropertyVector3 *scale_property;

	EditorInspectorSection *rest_section = nullptr;
	EditorPropertyTransform *rest_matrix = nullptr;

	Rect2 background_rects[5];

	Skeleton *skeleton;
	//String property;

	UndoRedo *undo_redo;

	bool toggle_enabled;
	bool updating;

	String label;

	void create_editors();

	void _value_changed(const String &p_path, const Variant &p_value, const String &p_name = "", bool changing = false);

	void _property_keyed(const String &p_path, bool p_advance);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	BoneTransformEditor(Skeleton *p_skeleton);

	// Which transform target to modify
	void set_target(const String &p_prop);
	void set_label(const String &p_label) { label = p_label; }
	void set_keyable(const bool p_keyable);

	void _update_properties();
};

class SkeletonEditor : public VBoxContainer {
	GDCLASS(SkeletonEditor, VBoxContainer);

	friend class SkeletonEditorPlugin;

	enum SkeletonOption {
		SKELETON_OPTION_INIT_ALL_POSES,
		SKELETON_OPTION_INIT_SELECTED_POSES,
		SKELETON_OPTION_ALL_POSES_TO_RESTS,
		SKELETON_OPTION_SELECTED_POSES_TO_RESTS,
		SKELETON_OPTION_CREATE_PHYSICAL_SKELETON,
		SKELETON_OPTION_ADD_BONE,
		SKELETON_OPTION_REMOVE_BONE,
		SKELETON_OPTION_RENAME_BONE
	};

	struct BoneInfo {
		PhysicalBone *physical_bone;
		Transform relative_rest; // Relative to skeleton node

		BoneInfo() {
			physical_bone = NULL;
		}
	};

	EditorNode *editor;
	EditorInspectorPluginSkeleton *editor_plugin;

	Skeleton *skeleton;

	Tree *joint_tree;
	BoneTransformEditor *rest_editor;
	BoneTransformEditor *pose_editor;

	VSeparator *separator;
	MenuButton *skeleton_options;
	Button *edit_mode_button;

	bool edit_mode;

	HBoxContainer *animation_hb;
	Button *key_loc_button;
	Button *key_rot_button;
	Button *key_scale_button;
	Button *key_insert_button;
	Button *key_insert_all_button;

	EditorFileDialog *file_dialog;

	bool keyable;

	static SkeletonEditor *singleton;

	void _on_click_skeleton_option(int p_skeleton_option);
	void _file_selected(const String &p_file);
	TreeItem *_find(TreeItem *p_node, const NodePath &p_path);
	void edit_mode_toggled(const bool pressed);

	EditorFileDialog *file_export_lib;

	void update_joint_tree();
	void update_editors();

	void create_editors();

	void init_pose(const bool p_all_bones);
	void pose_to_rest(const bool p_all_bones);

	void insert_keys(const bool p_all_bones);

	void create_physical_skeleton();
	PhysicalBone *create_physical_bone(int bone_id, int bone_child_id, const Vector<BoneInfo> &bones_infos);

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	void set_keyable(const bool p_keyable);
	void set_bone_options_enabled(const bool p_bone_options_enabled);

	MeshInstance *handles_mesh_instance;
	Ref<ImmediateMesh> handles_mesh;
	Ref<ShaderMaterial> handle_material;
	Ref<Shader> handle_shader;

	Vector3 bone_original_position;
	Quaternion bone_original_rotation;
	Vector3 bone_original_scale;

	void _update_gizmo_visible();
	void _bone_enabled_changed(const int p_bone_id);

	void _hide_handles();

	void _draw_gizmo();
	void _draw_handles();

	void _joint_tree_selection_changed();
	void _joint_tree_rmb_select(const Vector2 &p_pos);
	void _update_properties();

	void _subgizmo_selection_change();

	int selected_bone;

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	static SkeletonEditor *get_singleton() { return singleton; }

	void select_bone(int p_idx);

	int get_selected_bone() const;

	void move_skeleton_bone(NodePath p_skeleton_path, int32_t p_selected_boneidx, int32_t p_target_boneidx);

	Skeleton *get_skeleton() const { return skeleton; };

	bool is_edit_mode() const { return edit_mode; }

	void update_bone_original();
	Vector3 get_bone_original_position() const { return bone_original_position; };
	Quaternion get_bone_original_rotation() const { return bone_original_rotation; };
	Vector3 get_bone_original_scale() const { return bone_original_scale; };

	SkeletonEditor(EditorInspectorPluginSkeleton *e_plugin, EditorNode *p_editor, Skeleton *skeleton);
	~SkeletonEditor();

	void add_bone();
	void remove_bone();
	void rename_bone();

	void _add_bone_callback();
	void _remove_bone_callback();
	void _rename_bone_callback();

	void create_bone_tool_popups();
	static void _bind_tool_popup_methods();

	ConfirmationDialog *_bone_add_dialog;
	LineEdit *_bone_add_line_edit;
	ConfirmationDialog *_bone_rename_dialog;
	LineEdit *_bone_rename_line_edit;
	ConfirmationDialog *_bone_remove_dialog;
};

class EditorInspectorPluginSkeleton : public EditorInspectorPlugin {
	GDCLASS(EditorInspectorPluginSkeleton, EditorInspectorPlugin);

	friend class SkeletonEditorPlugin;

	SkeletonEditor *skel_editor;
	EditorNode *editor;

protected:
	static void _bind_methods();

public:
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
};

class SkeletonEditorPlugin : public EditorPlugin {
	GDCLASS(SkeletonEditorPlugin, EditorPlugin);

	EditorInspectorPluginSkeleton *skeleton_plugin;
	EditorNode *editor;

public:
	virtual EditorPlugin::AfterGUIInput forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event);

	bool has_main_screen() const { return false; }
	virtual bool handles(Object *p_object) const;

	virtual String get_name() const { return "Skeleton"; }

	SkeletonEditorPlugin(EditorNode *p_node);

protected:
	void _notification(int p_what);
};

class SkeletonGizmoPlugin : public EditorSpatialGizmoPlugin {
	GDCLASS(SkeletonGizmoPlugin, EditorSpatialGizmoPlugin);

	Ref<SpatialMaterial> unselected_mat;
	Ref<ShaderMaterial> selected_mat;
	Ref<Shader> selected_sh;

public:
	bool has_gizmo(Spatial *p_spatial);
	String get_gizmo_name() const;
	int get_priority() const;

	int subgizmos_intersect_ray(const EditorSpatialGizmo *p_gizmo, Camera *p_camera, const Vector2 &p_point) const;
	Transform get_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id) const;
	void set_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id, Transform p_transform);
	void commit_subgizmos(const EditorSpatialGizmo *p_gizmo, const Vector<int> &p_ids, const Vector<Transform> &p_restore, bool p_cancel);

	void redraw(EditorSpatialGizmo *p_gizmo);

	SkeletonGizmoPlugin();
};

#endif // SKELETON_EDITOR_PLUGIN_H
