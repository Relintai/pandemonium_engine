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

#ifndef MODULE_SKELETON_EDITOR_PLUGIN_H
#define MODULE_SKELETON_EDITOR_PLUGIN_H

#include "core/os/input_event.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/skeleton.h"

class ModuleEditorInspectorPluginSkeleton;
class Joint;
class PhysicalBone;
class ModuleSkeletonEditorPlugin;
class Button;
class CheckBox;

class ModuleBoneTransformEditor : public VBoxContainer {
	GDCLASS(ModuleBoneTransformEditor, VBoxContainer);

	static const int32_t TRANSLATION_COMPONENTS = 3;
	static const int32_t ROTATION_DEGREES_COMPONENTS = 3;
	static const int32_t SCALE_COMPONENTS = 3;
	static const int32_t BASIS_COMPONENTS = 9;
	static const int32_t BASIS_SPLIT_COMPONENTS = 3;
	static const int32_t TRANSFORM_COMPONENTS = 12;
	static const int32_t TRANSFORM_SPLIT_COMPONENTS = 3;
	static const int32_t TRANSFORM_CONTROL_COMPONENTS = 3;

	EditorInspectorSection *section;

	GridContainer *translation_grid;
	GridContainer *rotation_grid;
	GridContainer *scale_grid;
	GridContainer *transform_grid;

	EditorSpinSlider *translation_slider[TRANSLATION_COMPONENTS];
	EditorSpinSlider *rotation_slider[ROTATION_DEGREES_COMPONENTS];
	EditorSpinSlider *scale_slider[SCALE_COMPONENTS];
	EditorSpinSlider *transform_slider[TRANSFORM_COMPONENTS];

	Rect2 background_rects[5];

	Skeleton *skeleton;
	String property;

	UndoRedo *undo_redo;

	Button *key_button;
	CheckBox *enabled_checkbox;

	bool keyable;
	bool toggle_enabled;
	bool updating;

	String label;

	void create_editors();
	void setup_spinner(EditorSpinSlider *spinner, const bool is_transform_spinner);

	void _value_changed(const double p_value, const bool p_from_transform);

	Transform compute_transform(const bool p_from_transform) const;

	void update_enabled_checkbox();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	ModuleBoneTransformEditor(Skeleton *p_skeleton);

	// Which transform target to modify
	void set_target(const String &p_prop);
	void set_label(const String &p_label) { label = p_label; }

	void _update_properties();
	void _update_custom_pose_properties();
	void _update_transform_properties(Transform p_transform);

	// Can/cannot modify the spinner values for the Transform
	void set_read_only(const bool p_read_only);

	// Transform can be keyed, whether or not to show the button
	void set_keyable(const bool p_keyable);

	// Bone can be toggled enabled or disabled, whether or not to show the checkbox
	void set_toggle_enabled(const bool p_enabled);

	// Key Transform Button pressed
	void _key_button_pressed();

	// Bone Enabled Checkbox toggled
	void _checkbox_toggled(const bool p_toggled);
};

class ModuleSkeletonEditor : public VBoxContainer {

	GDCLASS(ModuleSkeletonEditor, VBoxContainer);

	friend class ModuleSkeletonEditorPlugin;

	enum Menu {
		MENU_OPTION_INIT_POSE,
		MENU_OPTION_INSERT_KEYS,
		MENU_OPTION_INSERT_KEYS_EXISTED,
		MENU_OPTION_POSE_TO_REST,
		MENU_OPTION_CREATE_PHYSICAL_SKELETON,
		MENU_OPTION_ADD_BONE,
		MENU_OPTION_REMOVE_BONE,
		MENU_OPTION_RENAME_BONE
	};

	enum ToolMode {
		TOOL_MODE_BONE_SELECT,
		TOOL_MODE_BONE_MOVE,
		TOOL_MODE_BONE_ROTATE,
		TOOL_MODE_BONE_SCALE,
		TOOL_MODE_BONE_NONE,
		TOOL_MODE_BONE_MAX
	};

	enum MenuToolOption {
		MENU_TOOL_BONE_SELECT,
		MENU_TOOL_BONE_MOVE,
		MENU_TOOL_BONE_ROTATE,
		MENU_TOOL_BONE_SCALE,
		MENU_TOOL_BONE_NONE,
		MENU_TOOL_BONE_MAX
	};

	struct BoneInfo {
		PhysicalBone *physical_bone;
		Transform relative_rest; // Relative to skeleton node
		BoneInfo() :
				physical_bone(NULL) {}
	};

	EditorNode *editor;
	ModuleEditorInspectorPluginSkeleton *editor_plugin;

	Skeleton *skeleton;

	Tree *joint_tree;
	ModuleBoneTransformEditor *rest_editor;
	ModuleBoneTransformEditor *pose_editor;
	ModuleBoneTransformEditor *custom_pose_editor;

	VSeparator *separators[2];
	MenuButton *options;
	ToolButton *tool_button[TOOL_MODE_BONE_MAX];
	ToolButton *rest_mode_button;

	ToolMode tool_mode = TOOL_MODE_BONE_NONE;
	bool rest_mode = false;

	EditorFileDialog *file_dialog;

	UndoRedo *undo_redo;

	bool keyable;

	void _on_click_option(int p_option);
	void _file_selected(const String &p_file);
	void _menu_tool_item_pressed(int p_option);
	void rest_mode_toggled(const bool pressed);

	EditorFileDialog *file_export_lib;

	void update_joint_tree();
	void update_editors();

	void create_editors();

	void init_pose();
	void insert_keys(bool p_all_bones);
	void pose_to_rest();
	void create_physical_skeleton();
	PhysicalBone *create_physical_bone(int bone_id, int bone_child_id, const Vector<BoneInfo> &bones_infos);

	Variant get_drag_data_fw(const Point2 &p_point, Control *p_from);
	bool can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const;
	void drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from);

	Ref<ShaderMaterial> handle_material;
	Ref<Shader> handle_shader;
	MeshInstance *pointsm;
	Ref<ArrayMesh> am;
	void _hide_handles();
	void _draw_handles();

	SpatialEditorViewport::EditData _edit;
	void _compute_edit(int p_index, const Point2 &p_point);
	bool _gizmo_select(int p_index, const Vector2 &p_screenpos, bool p_highlight_only = false);

	Transform original_local;
	Transform original_global;
	Transform original_to_local;

	void _update_spatial_transform_gizmo();

protected:
	void _notification(int p_what);
	void _node_removed(Node *p_node);
	static void _bind_methods();

public:
	virtual bool forward_spatial_gui_input(int p_index, Camera *p_camera, const Ref<InputEvent> &p_event);
	void move_skeleton_bone(NodePath p_skeleton_path, int32_t p_selected_boneidx, int32_t p_target_boneidx);

	// Transform can be keyed, whether or not to show the button
	void set_keyable(const bool p_keyable);

	Skeleton *get_skeleton() const { return skeleton; };

	void set_rest_mode_toggled(const bool pressed);

	void _joint_tree_selection_changed();
	void _joint_tree_rmb_select(const Vector2 &p_pos);

	void _update_properties();

	ModuleSkeletonEditor(ModuleEditorInspectorPluginSkeleton *e_plugin, EditorNode *p_editor, Skeleton *skeleton);
	~ModuleSkeletonEditor();

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

class ModuleEditorInspectorPluginSkeleton : public EditorInspectorPlugin {
	GDCLASS(ModuleEditorInspectorPluginSkeleton, EditorInspectorPlugin);

	friend class ModuleSkeletonEditorPlugin;

	ModuleSkeletonEditor *skel_editor;
	EditorNode *editor;
	UndoRedo *undo_redo;

	void set_rest_mode_toggled (const bool p_pressed);

protected:
	static void _bind_methods();
public:
	virtual bool forward_spatial_gui_input(int p_index, Camera *p_camera, const Ref<InputEvent> &p_event) { return skel_editor->forward_spatial_gui_input(p_index, p_camera, p_event); }
	virtual bool can_handle(Object *p_object);
	virtual void parse_begin(Object *p_object);
	UndoRedo *get_undo_redo() { return undo_redo; }
};

class ModuleSkeletonEditorPlugin : public EditorPlugin {
	GDCLASS(ModuleSkeletonEditorPlugin, EditorPlugin);

	ModuleEditorInspectorPluginSkeleton *skeleton_plugin;
	EditorNode *editor;

public:
	virtual bool forward_spatial_gui_input(int p_index, Camera *p_camera, const Ref<InputEvent> &p_event) {
		if (SpatialEditor::get_singleton()->get_tool_mode() != SpatialEditor::TOOL_MODE_EXTERNAL) {
			return false;
		}
		return skeleton_plugin->forward_spatial_gui_input(p_index, p_camera, p_event);
	}
	bool has_main_screen() const { return false; }
	virtual bool handles(Object *p_object) const;

	virtual String get_name() const { return "Skeleton"; }

	ModuleSkeletonEditorPlugin(EditorNode *p_node);
};

#endif // SKELETON_EDITOR_PLUGIN_H
