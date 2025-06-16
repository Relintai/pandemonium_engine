/*************************************************************************/
/*  skeleton_editor_plugin.cpp                                           */
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

#include "skeleton_editor_plugin.h"

#include "../nodes/physical_bone.h"
#include "../resources/skin.h"
#include "core/io/resource_saver.h"
#include "core/os/keyboard.h"
#include "editor/animation_track_editor.h"
#include "editor/editor_file_dialog.h"
#include "editor/editor_inspector.h"
#include "editor/editor_properties.h"
#include "editor/editor_scale.h"
#include "editor/editor_settings.h"
#include "editor/editor_spin_slider.h"
#include "editor/plugins/animation_player_editor_plugin.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/physics_joint.h"
#include "scene/gui/check_box.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/label.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/separator.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"
#include "scene/main/viewport.h"
#include "scene/resources/mesh/surface_tool.h"
#include "scene/resources/shapes/capsule_shape.h"
#include "scene/resources/shapes/sphere_shape.h"

#include "physical_bone_editor_plugin.h"

#define DISTANCE_DEFAULT 4

#define GIZMO_ARROW_SIZE 0.35
#define GIZMO_RING_HALF_WIDTH 0.1
//#define GIZMO_SCALE_DEFAULT 0.15
#define GIZMO_PLANE_SIZE 0.2
#define GIZMO_PLANE_DST 0.3
#define GIZMO_CIRCLE_SIZE 1.1
#define GIZMO_SCALE_OFFSET (GIZMO_CIRCLE_SIZE + 0.3)
#define GIZMO_ARROW_OFFSET (GIZMO_CIRCLE_SIZE + 0.3)

#define ZOOM_FREELOOK_MIN 0.01
#define ZOOM_FREELOOK_MULTIPLIER 1.08
#define ZOOM_FREELOOK_INDICATOR_DELAY_S 1.5

#ifdef REAL_T_IS_DOUBLE
#define ZOOM_FREELOOK_MAX 1'000'000'000'000
#else
#define ZOOM_FREELOOK_MAX 10'000
#endif

#define MIN_Z 0.01
#define MAX_Z 1000000.0

#define MIN_FOV 0.01
#define MAX_FOV 179

void BoneTransformEditor::create_editors() {
	const Color section_color = get_theme_color("prop_subsection", "Editor");

	section = memnew(EditorInspectorSection);
	section->setup("trf_properties", label, this, section_color, true);
	section->unfold();
	add_child(section);

	enabled_checkbox = memnew(EditorPropertyCheck());
	enabled_checkbox->set_label("Pose Enabled");
	enabled_checkbox->set_selectable(false);
	enabled_checkbox->connect("property_changed", this, "_value_changed");
	section->get_vbox()->add_child(enabled_checkbox);

	// Position property.
	position_property = memnew(EditorPropertyVector3());
	position_property->setup(-10000, 10000, 0.001f, true);
	position_property->set_label("Position");
	position_property->set_selectable(false);
	position_property->connect("property_changed", this, "_value_changed");
	position_property->connect("property_keyed", this, "_property_keyed");
	section->get_vbox()->add_child(position_property);

	// Rotation property
	rotation_property = memnew(EditorPropertyQuaternion());
	rotation_property->setup(-10000, 10000, 0.001f, true);
	rotation_property->set_label("Rotation");
	rotation_property->set_selectable(false);
	rotation_property->connect("property_changed", this, "_value_changed");
	rotation_property->connect("property_keyed", this, "_property_keyed");
	section->get_vbox()->add_child(rotation_property);

	// Scale property
	scale_property = memnew(EditorPropertyVector3());
	scale_property->setup(-10000, 10000, 0.001f, true);
	scale_property->set_label("Scale");
	scale_property->set_selectable(false);
	scale_property->connect("property_changed", this, "_value_changed_vector3");
	scale_property->connect("property_keyed", this, "_property_keyed");
	section->get_vbox()->add_child(scale_property);

	// Transform/Matrix section
	rest_section = memnew(EditorInspectorSection);
	rest_section->setup("trf_properties_transform", "Rest", this, section_color, true);
	section->get_vbox()->add_child(rest_section);

	// Transform/Matrix property
	rest_matrix = memnew(EditorPropertyTransform());
	rest_matrix->setup(-10000, 10000, 0.001f, true);
	rest_matrix->set_label("Transform");
	rest_matrix->set_selectable(false);
	rest_section->get_vbox()->add_child(rest_matrix);
}

void BoneTransformEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			create_editors();
		}
	}
}

void BoneTransformEditor::_value_changed(const String &p_path, const Variant &p_value, const String &p_name, bool changing) {
	if (updating) {
		return;
	}

	if (skeleton) {
		undo_redo->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);
		undo_redo->add_undo_property(skeleton, p_path, skeleton->get(p_path));
		undo_redo->add_do_property(skeleton, p_path, p_value);
		undo_redo->commit_action();
	}
}

void BoneTransformEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_value_changed"), &BoneTransformEditor::_value_changed);
	ClassDB::bind_method(D_METHOD("_property_keyed"), &BoneTransformEditor::_property_keyed);
	//ClassDB::bind_method(D_METHOD("update_joint_tree"), &BoneTransformEditor::update_joint_tree);
}

BoneTransformEditor::BoneTransformEditor(Skeleton *p_skeleton) {
	skeleton = p_skeleton;
	enabled_checkbox = nullptr;
	toggle_enabled = false;
	updating = false;
	undo_redo = EditorNode::get_undo_redo();
}

void BoneTransformEditor::set_keyable(const bool p_keyable) {
	position_property->set_keying(p_keyable);
	rotation_property->set_keying(p_keyable);
	scale_property->set_keying(p_keyable);
}

void BoneTransformEditor::set_target(const String &p_prop) {
	enabled_checkbox->set_object_and_property(skeleton, p_prop + "enabled");
	enabled_checkbox->update_property();

	position_property->set_object_and_property(skeleton, p_prop + "position");
	position_property->update_property();

	rotation_property->set_object_and_property(skeleton, p_prop + "rotation");
	rotation_property->update_property();

	scale_property->set_object_and_property(skeleton, p_prop + "scale");
	scale_property->update_property();

	rest_matrix->set_object_and_property(skeleton, p_prop + "rest");
	rest_matrix->update_property();
}

void BoneTransformEditor::_property_keyed(const String &p_path, bool p_advance) {
	AnimationTrackEditor *te = AnimationPlayerEditor::get_singleton()->get_track_editor();
	Vector<String> split = p_path.split("/");
	if (split.size() == 3 && split[0] == "bones") {
		int bone_idx = split[1].to_int();
		if (split[2] == "position") {
			te->insert_transform_key(skeleton, skeleton->get_bone_name(bone_idx), Animation::TYPE_POSITION_3D, skeleton->get(p_path));
		}
		if (split[2] == "rotation") {
			te->insert_transform_key(skeleton, skeleton->get_bone_name(bone_idx), Animation::TYPE_ROTATION_3D, skeleton->get(p_path));
		}
		if (split[2] == "scale") {
			te->insert_transform_key(skeleton, skeleton->get_bone_name(bone_idx), Animation::TYPE_SCALE_3D, skeleton->get(p_path));
		}
	}
}

void BoneTransformEditor::_update_properties() {
	/*
	if (!skeleton) {
		return;
	}

	int selected = SkeletonEditor::get_singleton()->get_selected_bone();

	List<PropertyInfo> props;
	skeleton->get_property_list(&props);
	for (const PropertyInfo &E : props) {
		PackedStringArray spr = E.name.split("/");
		if (spr.size() == 3 && spr[0] == "bones") {
			if (spr[1].to_int() == selected) {
				if (spr[2] == "enabled") {
					enabled_checkbox->set_read_only(E.usage & PROPERTY_USAGE_READ_ONLY);
					enabled_checkbox->update_property();
					enabled_checkbox->update();
				}
				if (spr[2] == "position") {
					position_property->set_read_only(E.usage & PROPERTY_USAGE_READ_ONLY);
					position_property->update_property();
					position_property->update();
				}
				if (spr[2] == "rotation") {
					rotation_property->set_read_only(E.usage & PROPERTY_USAGE_READ_ONLY);
					rotation_property->update_property();
					rotation_property->update();
				}
				if (spr[2] == "scale") {
					scale_property->set_read_only(E.usage & PROPERTY_USAGE_READ_ONLY);
					scale_property->update_property();
					scale_property->update();
				}
				if (spr[2] == "rest") {
					rest_matrix->set_read_only(E.usage & PROPERTY_USAGE_READ_ONLY);
					rest_matrix->update_property();
					rest_matrix->update();
				}
			}
		}
	}
	*/
}

SkeletonEditor *SkeletonEditor::singleton = nullptr;

void SkeletonEditor::set_keyable(const bool p_keyable) {
	keyable = p_keyable;

	if (p_keyable) {
		animation_hb->show();
	} else {
		animation_hb->hide();
	}
};

void SkeletonEditor::set_bone_options_enabled(const bool p_bone_options_enabled) {
	skeleton_options->get_popup()->set_item_disabled(SKELETON_OPTION_INIT_SELECTED_POSES, !p_bone_options_enabled);
	skeleton_options->get_popup()->set_item_disabled(SKELETON_OPTION_SELECTED_POSES_TO_RESTS, !p_bone_options_enabled);
};

void SkeletonEditor::_on_click_skeleton_option(int p_skeleton_option) {
	if (!skeleton) {
		return;
	}

	switch (p_skeleton_option) {
		case SKELETON_OPTION_INIT_ALL_POSES: {
			init_pose(true);
			break;
		}
		case SKELETON_OPTION_INIT_SELECTED_POSES: {
			init_pose(false);
			break;
		}
		case SKELETON_OPTION_ALL_POSES_TO_RESTS: {
			pose_to_rest(true);
			break;
		}
		case SKELETON_OPTION_SELECTED_POSES_TO_RESTS: {
			pose_to_rest(false);
			break;
		}
		case SKELETON_OPTION_CREATE_PHYSICAL_SKELETON: {
			create_physical_skeleton();
			break;
		}
		case SKELETON_OPTION_ADD_BONE: {
			add_bone();
		} break;
		case SKELETON_OPTION_REMOVE_BONE: {
			remove_bone();
		} break;
		case SKELETON_OPTION_RENAME_BONE: {
			rename_bone();
		} break;
		case SKELETON_OPTION_COPY_POSE: {
			copy_pose();
		} break;
		case SKELETON_OPTION_PASTE_POSE: {
			paste_pose();
		} break;
	}
}

void SkeletonEditor::init_pose(const bool p_all_bones) {
	if (!skeleton) {
		return;
	}

	const int bone_len = skeleton->get_bone_count();
	if (!bone_len) {
		return;
	}
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ur->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);

	if (p_all_bones) {
		for (int i = 0; i < bone_len; i++) {
			Transform rest = skeleton->get_bone_rest(i);
			ur->add_do_method(skeleton, "set_bone_pose_position", i, rest.origin);
			ur->add_do_method(skeleton, "set_bone_pose_rotation", i, rest.basis.get_rotation_quaternion());
			ur->add_do_method(skeleton, "set_bone_pose_scale", i, rest.basis.get_scale());
			ur->add_undo_method(skeleton, "set_bone_pose_position", i, skeleton->get_bone_pose_position(i));
			ur->add_undo_method(skeleton, "set_bone_pose_rotation", i, skeleton->get_bone_pose_rotation(i));
			ur->add_undo_method(skeleton, "set_bone_pose_scale", i, skeleton->get_bone_pose_scale(i));
		}
	} else {
		// Todo: Do method with multiple bone selection.
		if (selected_bone == -1) {
			ur->commit_action();
			return;
		}
		Transform rest = skeleton->get_bone_rest(selected_bone);
		ur->add_do_method(skeleton, "set_bone_pose_position", selected_bone, rest.origin);
		ur->add_do_method(skeleton, "set_bone_pose_rotation", selected_bone, rest.basis.get_rotation_quaternion());
		ur->add_do_method(skeleton, "set_bone_pose_scale", selected_bone, rest.basis.get_scale());
		ur->add_undo_method(skeleton, "set_bone_pose_position", selected_bone, skeleton->get_bone_pose_position(selected_bone));
		ur->add_undo_method(skeleton, "set_bone_pose_rotation", selected_bone, skeleton->get_bone_pose_rotation(selected_bone));
		ur->add_undo_method(skeleton, "set_bone_pose_scale", selected_bone, skeleton->get_bone_pose_scale(selected_bone));
	}

	ur->commit_action();
}

void SkeletonEditor::insert_keys(const bool p_all_bones) {
	if (!skeleton) {
		return;
	}

	bool pos_enabled = key_loc_button->is_pressed();
	bool rot_enabled = key_rot_button->is_pressed();
	bool scl_enabled = key_scale_button->is_pressed();

	int bone_len = skeleton->get_bone_count();
	Node *root = EditorNode::get_singleton()->get_tree()->get_root();
	String path = root->get_path_to(skeleton);

	AnimationTrackEditor *te = AnimationPlayerEditor::get_singleton()->get_track_editor();
	for (int i = 0; i < bone_len; i++) {
		const String name = skeleton->get_bone_name(i);

		if (name.empty()) {
			continue;
		}

		if (pos_enabled && (p_all_bones || te->has_track(skeleton, name, Animation::TYPE_POSITION_3D))) {
			te->insert_transform_key(skeleton, name, Animation::TYPE_POSITION_3D, skeleton->get_bone_pose_position(i));
		}
		if (rot_enabled && (p_all_bones || te->has_track(skeleton, name, Animation::TYPE_ROTATION_3D))) {
			te->insert_transform_key(skeleton, name, Animation::TYPE_ROTATION_3D, skeleton->get_bone_pose_rotation(i));
		}
		if (scl_enabled && (p_all_bones || te->has_track(skeleton, name, Animation::TYPE_SCALE_3D))) {
			te->insert_transform_key(skeleton, name, Animation::TYPE_SCALE_3D, skeleton->get_bone_pose_scale(i));
		}
	}

	//te->commit_insert_queue();
}

void SkeletonEditor::pose_to_rest(const bool p_all_bones) {
	if (!skeleton) {
		return;
	}

	const int bone_len = skeleton->get_bone_count();
	if (!bone_len) {
		return;
	}

	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();

	ur->create_action(TTR("Set Bone Rest"), UndoRedo::MERGE_ENDS);

	if (p_all_bones) {
		for (int i = 0; i < bone_len; i++) {
			ur->add_do_method(skeleton, "set_bone_rest", i, skeleton->get_bone_pose(i));
			ur->add_undo_method(skeleton, "set_bone_rest", i, skeleton->get_bone_rest(i));
		}
	} else {
		// Todo: Do method with multiple bone selection.
		if (selected_bone == -1) {
			ur->commit_action();
			return;
		}
		ur->add_do_method(skeleton, "set_bone_rest", selected_bone, skeleton->get_bone_pose(selected_bone));
		ur->add_undo_method(skeleton, "set_bone_rest", selected_bone, skeleton->get_bone_rest(selected_bone));
	}
	ur->commit_action();
}

void SkeletonEditor::create_physical_skeleton() {
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ERR_FAIL_COND(!get_tree());
	Node *owner = get_tree()->get_edited_scene_root();

	const int bc = skeleton->get_bone_count();

	if (!bc) {
		return;
	}

	Vector<BoneInfo> bones_infos;
	bones_infos.resize(bc);

	for (int bone_id = 0; bc > bone_id; ++bone_id) {
		const int parent = skeleton->get_bone_parent(bone_id);

		if (parent < 0) {
			bones_infos.write[bone_id].relative_rest = skeleton->get_bone_rest(bone_id);

		} else {
			const int parent_parent = skeleton->get_bone_parent(parent);

			bones_infos.write[bone_id].relative_rest = bones_infos[parent].relative_rest * skeleton->get_bone_rest(bone_id);

			/// create physical bone on parent
			if (!bones_infos[parent].physical_bone) {
				bones_infos.write[parent].physical_bone = create_physical_bone(parent, bone_id, bones_infos);

				ur->create_action(TTR("Create physical bones"));
				ur->add_do_method(skeleton, "add_child", bones_infos[parent].physical_bone);
				ur->add_do_reference(bones_infos[parent].physical_bone);
				ur->add_undo_method(skeleton, "remove_child", bones_infos[parent].physical_bone);
				ur->commit_action();

				bones_infos[parent].physical_bone->set_bone_name(skeleton->get_bone_name(parent));
				bones_infos[parent].physical_bone->set_owner(owner);
				bones_infos[parent].physical_bone->get_child(0)->set_owner(owner); // set shape owner

				/// Create joint between parent of parent
				if (-1 != parent_parent) {
					bones_infos[parent].physical_bone->set_joint_type(PhysicalBone::JOINT_TYPE_PIN);
				}
			}
		}
	}
}

PhysicalBone *SkeletonEditor::create_physical_bone(int bone_id, int bone_child_id, const Vector<BoneInfo> &bones_infos) {
	const Transform child_rest = skeleton->get_bone_rest(bone_child_id);

	real_t half_height(child_rest.origin.length() * 0.5);
	real_t radius(half_height * 0.2);

	CapsuleShape *bone_shape_capsule = memnew(CapsuleShape);
	//bone_shape_capsule->set_height((half_height - radius) * 2);
	bone_shape_capsule->set_height(half_height * 2);
	bone_shape_capsule->set_radius(radius);

	CollisionShape *bone_shape = memnew(CollisionShape);
	bone_shape->set_shape(bone_shape_capsule);

	Transform capsule_transform;
	bone_shape->set_transform(capsule_transform);

	Vector3 up = Vector3(0, 1, 0);
	if (up.cross(child_rest.origin).is_equal_approx(Vector3())) {
		up = Vector3(0, 0, 1);
	}

	Transform body_transform;
	body_transform.set_look_at(Vector3(0, 0, 0), child_rest.origin, up);
	body_transform.origin = body_transform.basis.xform(Vector3(0, 0, -half_height));

	Transform joint_transform;
	joint_transform.origin = Vector3(0, 0, half_height);

	PhysicalBone *physical_bone = memnew(PhysicalBone);
	physical_bone->add_child(bone_shape);
	physical_bone->set_name("Physical Bone " + skeleton->get_bone_name(bone_id));
	physical_bone->set_body_offset(body_transform);
	physical_bone->set_joint_offset(joint_transform);
	return physical_bone;
}

Variant SkeletonEditor::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	TreeItem *selected = joint_tree->get_selected();

	if (!selected) {
		return Variant();
	}

	Ref<Texture> icon = selected->get_icon(0);

	VBoxContainer *vb = memnew(VBoxContainer);
	HBoxContainer *hb = memnew(HBoxContainer);
	TextureRect *tf = memnew(TextureRect);
	tf->set_texture(icon);
	tf->set_stretch_mode(TextureRect::STRETCH_KEEP_CENTERED);
	hb->add_child(tf);
	Label *label = memnew(Label(selected->get_text(0)));
	hb->add_child(label);
	vb->add_child(hb);
	hb->set_modulate(Color(1, 1, 1, 1));

	set_drag_preview(vb);
	Dictionary drag_data;
	drag_data["type"] = "nodes";
	drag_data["node"] = selected;

	return drag_data;
}

bool SkeletonEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	TreeItem *target = joint_tree->get_item_at_position(p_point);
	if (!target) {
		return false;
	}

	const String path = target->get_metadata(0);
	if (!path.begins_with("bones/")) {
		return false;
	}

	TreeItem *selected = Object::cast_to<TreeItem>(Dictionary(p_data)["node"]);
	if (target == selected)
		return false;

	const String path2 = target->get_metadata(0);
	if (!path2.begins_with("bones/")) {
		return false;
	}

	return true;
}

void SkeletonEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	if (!can_drop_data_fw(p_point, p_data, p_from)) {
		return;
	}

	TreeItem *target = joint_tree->get_item_at_position(p_point);
	TreeItem *selected = Object::cast_to<TreeItem>(Dictionary(p_data)["node"]);

	const BoneId target_boneidx = String(target->get_metadata(0)).get_slicec('/', 1).to_int();
	const BoneId selected_boneidx = String(selected->get_metadata(0)).get_slicec('/', 1).to_int();

	move_skeleton_bone(skeleton->get_path(), selected_boneidx, target_boneidx);
}

void SkeletonEditor::move_skeleton_bone(NodePath p_skeleton_path, int32_t p_selected_boneidx, int32_t p_target_boneidx) {
	Node *node = get_node_or_null(p_skeleton_path);
	Skeleton *skeleton = Object::cast_to<Skeleton>(node);
	ERR_FAIL_NULL(skeleton);
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ur->create_action(TTR("Set Bone Parentage"));

	// If the target is a child of ourselves, we move only *us* and not our children
	if (skeleton->is_bone_parent_of(p_target_boneidx, p_selected_boneidx)) {
		const BoneId parent_idx = skeleton->get_bone_parent(p_selected_boneidx);
		const int bone_count = skeleton->get_bone_count();
		for (BoneId i = 0; i < bone_count; ++i) {
			if (skeleton->get_bone_parent(i) == p_selected_boneidx) {
				ur->add_undo_method(skeleton, "set_bone_parent", i, skeleton->get_bone_parent(i));
				ur->add_do_method(skeleton, "set_bone_parent", i, parent_idx);
				skeleton->set_bone_parent(i, parent_idx);
			}
		}
	}

	ur->add_undo_method(skeleton, "set_bone_parent", p_selected_boneidx, skeleton->get_bone_parent(p_selected_boneidx));
	ur->add_do_method(skeleton, "set_bone_parent", p_selected_boneidx, p_target_boneidx);
	skeleton->set_bone_parent(p_selected_boneidx, p_target_boneidx);

	ur->commit_action();

	update_joint_tree();
}

void SkeletonEditor::_joint_tree_selection_changed() {
	TreeItem *selected = joint_tree->get_selected();

	if (selected) {
		const String path = selected->get_metadata(0);

		if (path.begins_with("bones/")) {
			const int b_idx = path.get_slicec('/', 1).to_int();
			const String bone_path = "bones/" + itos(b_idx) + "/";

			pose_editor->set_target(bone_path);
			pose_editor->set_keyable(keyable);

			selected_bone = b_idx;
		}
	}

	pose_editor->set_visible(selected);

	set_bone_options_enabled(selected);
	_update_properties();
	_update_gizmo_visible();
}

void SkeletonEditor::_joint_tree_rmb_select(const Vector2 &p_pos) {
	//skeleton->set_selected_bone(-1);
	//_update_spatial_transform_gizmo();
}

void SkeletonEditor::_update_properties() {
	if (pose_editor) {
		pose_editor->_update_properties();
	}

	SpatialEditor::get_singleton()->update_transform_gizmo();
}

void SkeletonEditor::update_joint_tree() {
	skeleton->force_update_all_bone_transforms();

	joint_tree->clear();

	if (!skeleton) {
		return;
	}

	TreeItem *root = joint_tree->create_item();

	RBMap<int, TreeItem *> items;

	items.insert(-1, root);

	Ref<Texture> bone_icon = get_theme_icon("Bone", "EditorIcons");

	Vector<int> bones_to_process = skeleton->get_parentless_bones();
	while (bones_to_process.size() > 0) {
		int current_bone_idx = bones_to_process[0];
		bones_to_process.erase(current_bone_idx);

		const int parent_idx = skeleton->get_bone_parent(current_bone_idx);
		TreeItem *parent_item = items.find(parent_idx)->get();

		TreeItem *joint_item = joint_tree->create_item(parent_item);
		items.insert(current_bone_idx, joint_item);

		joint_item->set_text(0, skeleton->get_bone_name(current_bone_idx));
		joint_item->set_icon(0, bone_icon);
		joint_item->set_selectable(0, true);
		joint_item->set_metadata(0, "bones/" + itos(current_bone_idx));

		// Add the bone's children to the list of bones to be processed.
		Vector<int> current_bone_child_bones = skeleton->get_bone_children(current_bone_idx);
		int child_bone_size = current_bone_child_bones.size();
		for (int i = 0; i < child_bone_size; i++) {
			bones_to_process.push_back(current_bone_child_bones[i]);
		}
	}
}

void SkeletonEditor::update_editors() {
}

void SkeletonEditor::create_editors() {
	set_h_size_flags(SIZE_EXPAND_FILL);
	add_theme_constant_override("separation", 0);

	set_focus_mode(FOCUS_ALL);

	SpatialEditor *ne = SpatialEditor::get_singleton();
	AnimationTrackEditor *te = AnimationPlayerEditor::get_singleton()->get_track_editor();

	// Create Top Menu Bar
	separator = memnew(VSeparator);
	ne->add_control_to_menu_panel(separator);

	// Create Skeleton Option in Top Menu Bar.
	skeleton_options = memnew(MenuButton);
	ne->add_control_to_menu_panel(skeleton_options);

	skeleton_options->set_text(TTR("Skeleton"));
	skeleton_options->set_icon(EditorNode::get_singleton()->get_gui_base()->get_theme_icon("Skeleton", "EditorIcons"));

	PopupMenu *p = skeleton_options->get_popup();
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/init_all_poses", TTR("Init all Poses")), SKELETON_OPTION_INIT_ALL_POSES);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/init_selected_poses", TTR("Init selected Poses")), SKELETON_OPTION_INIT_SELECTED_POSES);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/all_poses_to_rests", TTR("Apply all poses to rests")), SKELETON_OPTION_ALL_POSES_TO_RESTS);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/selected_poses_to_rests", TTR("Apply selected poses to rests")), SKELETON_OPTION_SELECTED_POSES_TO_RESTS);
	p->add_separator();
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/create_physical_skeleton", TTR("Create physical skeleton")), SKELETON_OPTION_CREATE_PHYSICAL_SKELETON);
	p->add_separator();
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/add_bone", TTR("Add bone")), SKELETON_OPTION_ADD_BONE);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/rename_bone", TTR("Rename bone")), SKELETON_OPTION_RENAME_BONE);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/remove_bone", TTR("Remove bone")), SKELETON_OPTION_REMOVE_BONE);
	p->add_separator();
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/copy_pose", TTR("Copy Pose")), SKELETON_OPTION_COPY_POSE);
	p->add_shortcut(ED_SHORTCUT("skeleton_3d_editor/paste_pose", TTR("Paste Pose")), SKELETON_OPTION_PASTE_POSE);

	skeleton_options->get_popup()->connect("id_pressed", this, "_on_click_skeleton_option");
	set_bone_options_enabled(false);

	Vector<Variant> button_binds;
	button_binds.resize(1);

	edit_mode_button = memnew(Button);
	ne->add_control_to_menu_panel(edit_mode_button);
	edit_mode_button->set_flat(true);
	edit_mode_button->set_toggle_mode(true);
	edit_mode_button->set_focus_mode(FOCUS_NONE);
	edit_mode_button->set_tooltip(TTR("Edit Mode\nShow buttons on joints."));
	edit_mode_button->connect("toggled", this, "edit_mode_toggled");

	edit_mode = false;

	if (skeleton) {
		skeleton->add_child(handles_mesh_instance);
		handles_mesh_instance->set_skeleton_path(NodePath(""));
	}

	// Keying buttons.
	animation_hb = memnew(HBoxContainer);
	ne->add_control_to_menu_panel(animation_hb);
	animation_hb->add_child(memnew(VSeparator));
	animation_hb->hide();

	key_loc_button = memnew(Button);
	key_loc_button->set_flat(true);
	key_loc_button->set_toggle_mode(true);
	key_loc_button->set_pressed(false);
	key_loc_button->set_focus_mode(FOCUS_NONE);
	key_loc_button->set_tooltip(TTR("Translation mask for inserting keys."));
	animation_hb->add_child(key_loc_button);

	key_rot_button = memnew(Button);
	key_rot_button->set_flat(true);
	key_rot_button->set_toggle_mode(true);
	key_rot_button->set_pressed(true);
	key_rot_button->set_focus_mode(FOCUS_NONE);
	key_rot_button->set_tooltip(TTR("Rotation mask for inserting keys."));
	animation_hb->add_child(key_rot_button);

	key_scale_button = memnew(Button);
	key_scale_button->set_flat(true);
	key_scale_button->set_toggle_mode(true);
	key_scale_button->set_pressed(false);
	key_scale_button->set_focus_mode(FOCUS_NONE);
	key_scale_button->set_tooltip(TTR("Scale mask for inserting keys."));
	animation_hb->add_child(key_scale_button);

	key_insert_button = memnew(Button);
	key_insert_button->set_flat(true);
	key_insert_button->set_focus_mode(FOCUS_NONE);
	key_insert_button->connect("pressed", this, "insert_keys", varray(false));
	key_insert_button->set_tooltip(TTR("Insert key of bone poses already exist track."));
	key_insert_button->set_shortcut(ED_SHORTCUT("skeleton_3d_editor/insert_key_to_existing_tracks", TTR("Insert Key (Existing Tracks)"), KEY_INSERT));
	animation_hb->add_child(key_insert_button);

	key_insert_all_button = memnew(Button);
	key_insert_all_button->set_flat(true);
	key_insert_all_button->set_focus_mode(FOCUS_NONE);
	key_insert_all_button->connect("pressed", this, "insert_keys", varray(true));
	key_insert_all_button->set_tooltip(TTR("Insert key of all bone poses."));
	key_insert_all_button->set_shortcut(ED_SHORTCUT("skeleton_3d_editor/insert_key_of_all_bones", TTR("Insert Key (All Bones)"), KEY_MASK_CMD + KEY_INSERT));
	animation_hb->add_child(key_insert_all_button);

	// Bone tree.

	const Color section_color = get_theme_color("prop_subsection", "Editor");

	EditorInspectorSection *bones_section = memnew(EditorInspectorSection);
	bones_section->setup("bones", "Bones", skeleton, section_color, true);
	add_child(bones_section);
	bones_section->unfold();

	ScrollContainer *s_con = memnew(ScrollContainer);
	s_con->set_h_size_flags(SIZE_EXPAND_FILL);
	s_con->set_custom_minimum_size(Size2(1, 350) * EDSCALE);
	bones_section->get_vbox()->add_child(s_con);

	joint_tree = memnew(Tree);
	joint_tree->set_columns(1);
	joint_tree->set_focus_mode(Control::FOCUS_NONE);
	joint_tree->set_select_mode(Tree::SELECT_SINGLE);
	joint_tree->set_hide_root(true);
	joint_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	joint_tree->set_h_size_flags(SIZE_EXPAND_FILL);
	joint_tree->set_allow_rmb_select(true);
	joint_tree->set_drag_forwarding(this);
	s_con->add_child(joint_tree);

	pose_editor = memnew(BoneTransformEditor(skeleton));
	pose_editor->set_label(TTR("Bone Transform"));
	pose_editor->set_visible(false);
	add_child(pose_editor);

	set_keyable(te->has_keying());
}

void SkeletonEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			edit_mode_button->set_icon(get_theme_icon("ToolBoneSelect", "EditorIcons"));
			key_loc_button->set_icon(get_theme_icon("KeyPosition", "EditorIcons"));
			key_rot_button->set_icon(get_theme_icon("KeyRotation", "EditorIcons"));
			key_scale_button->set_icon(get_theme_icon("KeyScale", "EditorIcons"));
			key_insert_button->set_icon(get_theme_icon("Key", "EditorIcons"));
			key_insert_all_button->set_icon(get_theme_icon("NewKey", "EditorIcons"));

			get_tree()->connect("node_removed", this, "_node_removed", Vector<Variant>(), Object::CONNECT_ONESHOT);
			break;
		} break;
		case NOTIFICATION_ENTER_TREE: {
			create_editors();
			update_joint_tree();
			update_editors();
			joint_tree->connect("item_selected", this, "_joint_tree_selection_changed");
			joint_tree->connect("item_rmb_selected", this, "_joint_tree_rmb_select");

#ifdef TOOLS_ENABLED
			skeleton->connect("pose_updated", this, "_draw_gizmo");
			skeleton->connect("pose_updated", this, "_update_properties");
			skeleton->connect("bone_enabled_changed", this, "_bone_enabled_changed");
			skeleton->connect("show_rest_only_changed", this, "_update_gizmo_visible");
#endif
			break;
		}
	}
}

void SkeletonEditor::_node_removed(Node *p_node) {
	if (skeleton && p_node == skeleton) {
		skeleton = nullptr;
		skeleton_options->hide();
	}

	_update_properties();
}

void SkeletonEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_node_removed"), &SkeletonEditor::_node_removed);
	ClassDB::bind_method(D_METHOD("_joint_tree_selection_changed"), &SkeletonEditor::_joint_tree_selection_changed);
	ClassDB::bind_method(D_METHOD("_joint_tree_rmb_select"), &SkeletonEditor::_joint_tree_rmb_select);
	ClassDB::bind_method(D_METHOD("_update_properties"), &SkeletonEditor::_update_properties);
	ClassDB::bind_method(D_METHOD("_on_click_skeleton_option"), &SkeletonEditor::_on_click_skeleton_option);
	ClassDB::bind_method(D_METHOD("insert_keys"), &SkeletonEditor::insert_keys);
	ClassDB::bind_method(D_METHOD("edit_mode_toggled"), &SkeletonEditor::edit_mode_toggled);

	ClassDB::bind_method(D_METHOD("get_drag_data_fw"), &SkeletonEditor::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &SkeletonEditor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw"), &SkeletonEditor::drop_data_fw);
	ClassDB::bind_method(D_METHOD("move_skeleton_bone"), &SkeletonEditor::move_skeleton_bone);

	ClassDB::bind_method(D_METHOD("_draw_handles"), &SkeletonEditor::_draw_handles);
	ClassDB::bind_method(D_METHOD("_draw_gizmo"), &SkeletonEditor::_draw_gizmo);
	ClassDB::bind_method(D_METHOD("_update_gizmo_visible"), &SkeletonEditor::_update_gizmo_visible);

	_bind_tool_popup_methods();
}

void SkeletonEditor::edit_mode_toggled(const bool pressed) {
	edit_mode = pressed;
	_update_gizmo_visible();
}

SkeletonEditor::SkeletonEditor(EditorInspectorPluginSkeleton *e_plugin, EditorNode *p_editor, Skeleton *p_skeleton) {
	editor = p_editor;
	editor_plugin = e_plugin;

	skeleton = p_skeleton;

	singleton = this;

	joint_tree = nullptr;
	rest_editor = nullptr;
	pose_editor = nullptr;

	skeleton_options = nullptr;

	edit_mode = false;

	file_dialog = nullptr;

	keyable = false;

	file_export_lib = nullptr;

	selected_bone = -1;

	handle_material = Ref<ShaderMaterial>(memnew(ShaderMaterial));
	handle_shader = Ref<Shader>(memnew(Shader));

	/*
	handle_shader->set_code(" \
					shader_type spatial; \
					render_mode unshaded; \
					uniform vec4 albedo : hint_color = vec4(1,1,1,1); \
					uniform sampler2D texture_albedo : hint_albedo; \
					uniform float point_size : hint_range(0,128) = 32; \
					void vertex() { \
						if (!OUTPUT_IS_SRGB) { \
							COLOR.rgb = mix( pow((COLOR.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), COLOR.rgb* (1.0 / 12.92), lessThan(COLOR.rgb,vec3(0.04045)) ); \
						} \
						POINT_SIZE=point_size; \
						VERTEX = VERTEX; \
						POSITION=PROJECTION_MATRIX*INV_CAMERA_MATRIX*WORLD_MATRIX*vec4(VERTEX.xyz,1.0); \
						POSITION.z = mix(POSITION.z, -POSITION.w, 0.999); \
					} \
					void fragment() { \
						vec4 albedo_tex = texture(texture_albedo,POINT_COORD); \
						if (albedo.a * albedo_tex.a < 0.5) { discard; } \
						vec3 col = albedo_tex.rgb + COLOR.rgb; \
						col = vec3(min(col.r,1.0),min(col.g,1.0),min(col.b,1.0)); \
						ALBEDO = albedo.rgb * col; \
					} \
				");

	handle_material->set_shader(handle_shader);
	// handle_material->set_flag(SpatialMaterial::FLAG_DISABLE_DEPTH_TEST, true);
	handle_material->set_render_priority(SpatialMaterial::RENDER_PRIORITY_MIN);
	// handle_material->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	// handle_material->set_flag(SpatialMaterial::FLAG_USE_POINT_SIZE, true);
	// handle_material->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	Ref<Texture> handle = editor->get_gui_base()->get_theme_icon("EditorBoneHandle", "EditorIcons");
	handle_material->set_shader_param("point_size", handle->get_width());
	handle_material->set_shader_param("texture_albedo", handle);
	//handle_material->set_texture(SpatialMaterial::TEXTURE_ALBEDO, handle);
	*/

	handle_shader->set_code(R"(
				// Skeleton 3D gizmo handle shader.
				shader_type spatial;
				render_mode unshaded, shadows_disabled, depth_draw_always;
				uniform sampler2D texture_albedo : hint_albedo;
				uniform float point_size : hint_range(0,128) = 32;
				void vertex() {
					if (!OUTPUT_IS_SRGB) {
						COLOR.rgb = mix( pow((COLOR.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), COLOR.rgb* (1.0 / 12.92), lessThan(COLOR.rgb,vec3(0.04045)) );
					}
					VERTEX = VERTEX;
					POSITION=PROJECTION_MATRIX*INV_CAMERA_MATRIX*WORLD_MATRIX*vec4(VERTEX.xyz,1.0);
					POSITION.z = mix(POSITION.z, 0, 0.999);
					POINT_SIZE = point_size;
				}
				void fragment() {
					vec4 albedo_tex = texture(texture_albedo,POINT_COORD);
					vec3 col = albedo_tex.rgb + COLOR.rgb;
					col = vec3(min(col.r,1.0),min(col.g,1.0),min(col.b,1.0));
					ALBEDO = col;
					if (albedo_tex.a < 0.5) { discard; }
					ALPHA = albedo_tex.a;
				}
				)");

	handle_material->set_shader(handle_shader);
	Ref<Texture> handle = editor->get_gui_base()->get_theme_icon("EditorBoneHandle", "EditorIcons");
	handle_material->set_shader_param("point_size", handle->get_width());
	handle_material->set_shader_param("texture_albedo", handle);

	handles_mesh_instance = memnew(MeshInstance);
	handles_mesh_instance->set_cast_shadows_setting(GeometryInstance::SHADOW_CASTING_SETTING_OFF);
	handles_mesh.instance();
	handles_mesh_instance->set_mesh(handles_mesh);

	create_bone_tool_popups();
}

SkeletonEditor::~SkeletonEditor() {
	if (skeleton) {
#ifdef TOOLS_ENABLED
		skeleton->disconnect("show_rest_only_changed", this, "_update_gizmo_visible");
		skeleton->disconnect("bone_enabled_changed", this, "_bone_enabled_changed");
		skeleton->disconnect("pose_updated", this, "_draw_gizmo");
		skeleton->disconnect("pose_updated", this, "_update_properties");
		skeleton->set_transform_gizmo_visible(true);
#endif
		handles_mesh_instance->get_parent()->remove_child(handles_mesh_instance);
	}

	handles_mesh_instance->queue_delete();

	SpatialEditor *ne = SpatialEditor::get_singleton();

	if (animation_hb) {
		ne->remove_control_from_menu_panel(animation_hb);
		memdelete(animation_hb);
	}

	if (separator) {
		ne->remove_control_from_menu_panel(separator);
		memdelete(separator);
	}

	if (skeleton_options) {
		ne->remove_control_from_menu_panel(skeleton_options);
		memdelete(skeleton_options);
	}

	if (edit_mode_button) {
		ne->remove_control_from_menu_panel(edit_mode_button);
		memdelete(edit_mode_button);
	}
}

void SkeletonEditor::update_bone_original() {
	if (!skeleton) {
		return;
	}

	if (skeleton->get_bone_count() == 0 || selected_bone == -1) {
		return;
	}

	bone_original_position = skeleton->get_bone_pose_position(selected_bone);
	bone_original_rotation = skeleton->get_bone_pose_rotation(selected_bone);
	bone_original_scale = skeleton->get_bone_pose_scale(selected_bone);
}

void SkeletonEditor::_hide_handles() {
	handles_mesh_instance->hide();
}

void SkeletonEditor::_draw_gizmo() {
	if (!skeleton) {
		return;
	}

	// If you call get_bone_global_pose() while drawing the surface, such as toggle rest mode,
	// the skeleton update will be done first and
	// the drawing surface will be interrupted once and an error will occur.
	skeleton->force_update_all_dirty_bones();

	// Handles.
	if (edit_mode) {
		_draw_handles();
	} else {
		_hide_handles();
	}
}

void SkeletonEditor::_draw_handles() {
	handles_mesh_instance->show();

	const int bone_len = skeleton->get_bone_count();
	handles_mesh->clear_surfaces();
	handles_mesh->surface_begin(Mesh::PRIMITIVE_POINTS);

	for (int i = 0; i < bone_len; i++) {
		Color c;
		if (i == selected_bone) {
			c = Color(1, 1, 0);
		} else {
			c = Color(0.1, 0.25, 0.8);
		}
		Vector3 point = skeleton->get_bone_global_pose(i).origin;
		handles_mesh->surface_set_color(c);
		handles_mesh->surface_add_vertex(point);
	}
	handles_mesh->surface_end();
	handles_mesh->surface_set_material(0, handle_material);
}

TreeItem *SkeletonEditor::_find(TreeItem *p_node, const NodePath &p_path) {
	if (!p_node) {
		return nullptr;
	}

	NodePath np = p_node->get_metadata(0);
	if (np == p_path) {
		return p_node;
	}

	TreeItem *children = p_node->get_children();
	while (children) {
		TreeItem *n = _find(children, p_path);
		if (n) {
			return n;
		}
		children = children->get_next();
	}

	return nullptr;
}

void SkeletonEditor::_subgizmo_selection_change() {
	if (!skeleton) {
		return;
	}

	// Once validated by subgizmos_intersect_ray, but required if through inspector's bones tree.
	if (!edit_mode) {
		skeleton->clear_subgizmo_selection();
		return;
	}

	int selected = -1;
	SkeletonEditor *se = SkeletonEditor::get_singleton();
	if (se) {
		selected = se->get_selected_bone();
	}

	if (selected >= 0) {
		Vector<Ref<SpatialGizmo>> gizmos = skeleton->get_gizmos();
		for (int i = 0; i < gizmos.size(); i++) {
			Ref<EditorSpatialGizmo> gizmo = gizmos[i];
			if (!gizmo.is_valid()) {
				continue;
			}
			Ref<SkeletonGizmoPlugin> plugin = gizmo->get_plugin();
			if (!plugin.is_valid()) {
				continue;
			}
			skeleton->set_subgizmo_selection(gizmo, selected, skeleton->get_bone_global_pose(selected));
			break;
		}
	} else {
		skeleton->clear_subgizmo_selection();
	}
}

void SkeletonEditor::select_bone(int p_idx) {
	if (p_idx >= 0) {
		TreeItem *ti = _find(joint_tree->get_root(), "bones/" + itos(p_idx));
		if (ti) {
			// Make visible when it's collapsed.
			TreeItem *node = ti->get_parent();
			while (node && node != joint_tree->get_root()) {
				node->set_collapsed(false);
				node = node->get_parent();
			}
			ti->select(0);
			joint_tree->scroll_to_item(ti);
		}
	} else {
		selected_bone = -1;
		joint_tree->deselect_all();
		_joint_tree_selection_changed();
	}
}

void SkeletonEditor::add_bone() {
	_bone_add_dialog->popup_centered();
	_bone_add_line_edit->set_text("");
	_bone_add_line_edit->grab_focus();
}
void SkeletonEditor::_add_bone_callback() {
	skeleton->add_bone(_bone_add_line_edit->get_text());

	update_joint_tree();
}
void SkeletonEditor::remove_bone() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	_bone_remove_dialog->popup_centered();
}
void SkeletonEditor::_remove_bone_callback() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	skeleton->remove_bone(skeleton->get_selected_bone());

	update_joint_tree();
}
void SkeletonEditor::rename_bone() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	_bone_rename_line_edit->set_text(skeleton->get_bone_name(skeleton->get_selected_bone()));

	_bone_rename_dialog->popup_centered();
	_bone_rename_line_edit->grab_focus();
}

void SkeletonEditor::_rename_bone_callback() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	skeleton->set_bone_name(skeleton->get_selected_bone(), _bone_rename_line_edit->get_text());

	update_joint_tree();
}

void SkeletonEditor::copy_pose() {
	if (!skeleton) {
		return;
	}

	_pose_clipboard.clear();

	for (int i = 0; i < skeleton->get_bone_count(); ++i) {
		Transform pose = skeleton->get_bone_pose(i);
		_pose_clipboard.push_back(pose);
	}
}
void SkeletonEditor::paste_pose() {
	if (!skeleton) {
		return;
	}

	ERR_FAIL_COND(skeleton->get_bone_count() != _pose_clipboard.size());

	for (int i = 0; i < skeleton->get_bone_count(); ++i) {
		skeleton->set_bone_pose(i, _pose_clipboard[i]);
	}
}

void SkeletonEditor::create_bone_tool_popups() {
	_bone_add_dialog = memnew(ConfirmationDialog);
	_bone_add_dialog->set_title("Add bone");
	_bone_add_dialog->connect("confirmed", this, "_add_bone_callback");

	_bone_add_line_edit = memnew(LineEdit);
	_bone_add_line_edit->set_placeholder("Name");
	_bone_add_dialog->add_child(_bone_add_line_edit);

	add_child(_bone_add_dialog);

	_bone_rename_dialog = memnew(ConfirmationDialog);
	_bone_rename_dialog->set_title("Rename bone");
	_bone_rename_dialog->connect("confirmed", this, "_rename_bone_callback");

	_bone_rename_line_edit = memnew(LineEdit);
	_bone_rename_dialog->add_child(_bone_rename_line_edit);

	add_child(_bone_rename_dialog);

	_bone_remove_dialog = memnew(ConfirmationDialog);
	_bone_remove_dialog->set_title("Remove");
	_bone_remove_dialog->connect("confirmed", this, "_remove_bone_callback");

	Label *rl = memnew(Label);
	rl->set_text("Remove Bone?");
	_bone_remove_dialog->add_child(rl);

	add_child(_bone_remove_dialog);
}

void SkeletonEditor::_bind_tool_popup_methods() {
	ClassDB::bind_method(D_METHOD("_add_bone_callback"), &SkeletonEditor::_add_bone_callback);
	ClassDB::bind_method(D_METHOD("_remove_bone_callback"), &SkeletonEditor::_remove_bone_callback);
	ClassDB::bind_method(D_METHOD("_rename_bone_callback"), &SkeletonEditor::_rename_bone_callback);
}

void EditorInspectorPluginSkeleton::_bind_methods() {
}

bool EditorInspectorPluginSkeleton::can_handle(Object *p_object) {
	return Object::cast_to<Skeleton>(p_object) != nullptr;
}

void EditorInspectorPluginSkeleton::parse_begin(Object *p_object) {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_object);
	ERR_FAIL_COND(!skeleton);

	skel_editor = memnew(SkeletonEditor(this, editor, skeleton));
	add_custom_control(skel_editor);
}

SkeletonEditorPlugin::SkeletonEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	skeleton_plugin = memnew(EditorInspectorPluginSkeleton);
	skeleton_plugin->editor = editor;

	EditorInspector::add_inspector_plugin(skeleton_plugin);

	Ref<SkeletonGizmoPlugin> gizmo_plugin = Ref<SkeletonGizmoPlugin>(memnew(SkeletonGizmoPlugin));
	SpatialEditor::get_singleton()->add_gizmo_plugin(gizmo_plugin);
}

EditorPlugin::AfterGUIInput SkeletonEditorPlugin::forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) {
	SkeletonEditor *se = SkeletonEditor::get_singleton();
	SpatialEditor *ne = SpatialEditor::get_singleton();
	if (se->is_edit_mode()) {
		const Ref<InputEventMouseButton> mb = p_event;
		if (mb.is_valid() && mb->get_button_index() == BUTTON_LEFT) {
			if (ne->get_tool_mode() != SpatialEditor::TOOL_MODE_SELECT) {
				if (!ne->is_gizmo_visible()) {
					return EditorPlugin::AFTER_GUI_INPUT_STOP;
				}
			}
			if (mb->is_pressed()) {
				se->update_bone_original();
			}
		}
		return EditorPlugin::AFTER_GUI_INPUT_NO_DESELECT;
	}
	return EditorPlugin::AFTER_GUI_INPUT_PASS;
}

bool SkeletonEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("Skeleton");
}

void SkeletonEditor::_bone_enabled_changed(const int p_bone_id) {
	_update_gizmo_visible();
}

void SkeletonEditorPlugin::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		add_spatial_gizmo_plugin(Ref<SkeletonGizmoPlugin>(memnew(SkeletonGizmoPlugin)));
		add_spatial_gizmo_plugin(Ref<PhysicalBoneSpatialGizmoPlugin>(memnew(PhysicalBoneSpatialGizmoPlugin)));
	}
}

void SkeletonEditor::_update_gizmo_visible() {
	_subgizmo_selection_change();
	if (edit_mode) {
		if (selected_bone == -1) {
#ifdef TOOLS_ENABLED
			skeleton->set_transform_gizmo_visible(false);
#endif
		} else {
#ifdef TOOLS_ENABLED
			if (skeleton->is_bone_enabled(selected_bone) && !skeleton->is_show_rest_only()) {
				skeleton->set_transform_gizmo_visible(true);
			} else {
				skeleton->set_transform_gizmo_visible(false);
			}
#endif
		}
	} else {
#ifdef TOOLS_ENABLED
		skeleton->set_transform_gizmo_visible(true);
#endif
	}
	_draw_gizmo();
}

int SkeletonEditor::get_selected_bone() const {
	return selected_bone;
}

SkeletonGizmoPlugin::SkeletonGizmoPlugin() {
	unselected_mat.instance();
	unselected_mat->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
	unselected_mat->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
	unselected_mat->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
	unselected_mat->set_flag(SpatialMaterial::FLAG_SRGB_VERTEX_COLOR, true);

	selected_mat = Ref<ShaderMaterial>(memnew(ShaderMaterial));
	selected_sh = Ref<Shader>(memnew(Shader));
	selected_sh->set_code(R"(
		// Skeleton 3D gizmo bones shader.
		shader_type spatial;
		render_mode unshaded, shadows_disabled;
		void vertex() {
			if (!OUTPUT_IS_SRGB) {
				COLOR.rgb = mix( pow((COLOR.rgb + vec3(0.055)) * (1.0 / (1.0 + 0.055)), vec3(2.4)), COLOR.rgb* (1.0 / 12.92), lessThan(COLOR.rgb,vec3(0.04045)) );
			}
			VERTEX = VERTEX;
			POSITION=PROJECTION_MATRIX*INV_CAMERA_MATRIX*WORLD_MATRIX*vec4(VERTEX.xyz,1.0);
			POSITION.z = mix(POSITION.z, 0, 0.998);
		}
		void fragment() {
			ALBEDO = COLOR.rgb;
			ALPHA = COLOR.a;
		}
		)");
	selected_mat->set_shader(selected_sh);

	// Register properties in editor settings.
	EDITOR_DEF("editors/3d_gizmos/gizmo_colors/skeleton", Color(1, 0.8, 0.4));
	EDITOR_DEF("editors/3d_gizmos/gizmo_colors/selected_bone", Color(0.8, 0.3, 0.0));
	EDITOR_DEF("editors/3d_gizmos/gizmo_settings/bone_axis_length", (float)0.1);
	EDITOR_DEF("editors/3d_gizmos/gizmo_settings/bone_shape", 1);
	EditorSettings::get_singleton()->add_property_hint(PropertyInfo(Variant::INT, "editors/3d_gizmos/gizmo_settings/bone_shape", PROPERTY_HINT_ENUM, "Wire,Octahedron"));
}

bool SkeletonGizmoPlugin::has_gizmo(Spatial *p_spatial) {
	return Object::cast_to<Skeleton>(p_spatial) != nullptr;
}

String SkeletonGizmoPlugin::get_gizmo_name() const {
	return "Skeleton";
}

int SkeletonGizmoPlugin::get_priority() const {
	return -1;
}

int SkeletonGizmoPlugin::subgizmos_intersect_ray(const EditorSpatialGizmo *p_gizmo, Camera *p_camera, const Vector2 &p_point) const {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());
	ERR_FAIL_COND_V(!skeleton, -1);

	SkeletonEditor *se = SkeletonEditor::get_singleton();

	if (!se->is_edit_mode()) {
		return -1;
	}

	if (SpatialEditor::get_singleton()->get_tool_mode() != SpatialEditor::TOOL_MODE_SELECT) {
		return -1;
	}

	// Select bone.
	real_t grab_threshold = 4 * EDSCALE;
	Vector3 ray_from = p_camera->get_global_transform().origin;
	Transform gt = skeleton->get_global_transform();
	int closest_idx = -1;
	real_t closest_dist = 1e10;
	const int bone_len = skeleton->get_bone_count();
	for (int i = 0; i < bone_len; i++) {
		Vector3 joint_pos_3d = gt.xform(skeleton->get_bone_global_pose(i).origin);
		Vector2 joint_pos_2d = p_camera->unproject_position(joint_pos_3d);
		real_t dist_3d = ray_from.distance_to(joint_pos_3d);
		real_t dist_2d = p_point.distance_to(joint_pos_2d);
		if (dist_2d < grab_threshold && dist_3d < closest_dist) {
			closest_dist = dist_3d;
			closest_idx = i;
		}
	}

	if (closest_idx >= 0) {
		se->select_bone(closest_idx);
		return closest_idx;
	}

	se->select_bone(-1);
	return -1;
}

Transform SkeletonGizmoPlugin::get_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id) const {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());
	ERR_FAIL_COND_V(!skeleton, Transform());

	return skeleton->get_bone_global_pose(p_id);
}

void SkeletonGizmoPlugin::set_subgizmo_transform(const EditorSpatialGizmo *p_gizmo, int p_id, Transform p_transform) {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());
	ERR_FAIL_COND(!skeleton);

	// Prepare for global to local.
	Transform original_to_local = Transform();
	int parent_idx = skeleton->get_bone_parent(p_id);
	if (parent_idx >= 0) {
		original_to_local = original_to_local * skeleton->get_bone_global_pose(parent_idx);
	}

	Basis to_local = original_to_local.get_basis().inverse();

	// Prepare transform.
	Transform t = Transform();

	// Basis.
	t.basis = to_local * p_transform.get_basis();

	// Origin.
	Vector3 orig = Vector3();
	orig = skeleton->get_bone_pose(p_id).origin;
	Vector3 sub = p_transform.origin - skeleton->get_bone_global_pose(p_id).origin;
	t.origin = orig + to_local.xform(sub);

	// Apply transform.
	skeleton->set_bone_pose_position(p_id, t.origin);
	skeleton->set_bone_pose_rotation(p_id, t.basis.get_rotation_quaternion());
	skeleton->set_bone_pose_scale(p_id, t.basis.get_scale());
}

void SkeletonGizmoPlugin::commit_subgizmos(const EditorSpatialGizmo *p_gizmo, const Vector<int> &p_ids, const Vector<Transform> &p_restore, bool p_cancel) {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());
	ERR_FAIL_COND(!skeleton);

	SkeletonEditor *se = SkeletonEditor::get_singleton();
	SpatialEditor *ne = SpatialEditor::get_singleton();

	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();

	ur->create_action(TTR("Set Bone Transform"));
	if (ne->get_tool_mode() == SpatialEditor::TOOL_MODE_SELECT || ne->get_tool_mode() == SpatialEditor::TOOL_MODE_MOVE) {
		for (int i = 0; i < p_ids.size(); i++) {
			ur->add_do_method(skeleton, "set_bone_pose_position", p_ids[i], skeleton->get_bone_pose_position(p_ids[i]));
			ur->add_undo_method(skeleton, "set_bone_pose_position", p_ids[i], se->get_bone_original_position());
		}
	}
	if (ne->get_tool_mode() == SpatialEditor::TOOL_MODE_SELECT || ne->get_tool_mode() == SpatialEditor::TOOL_MODE_ROTATE) {
		for (int i = 0; i < p_ids.size(); i++) {
			ur->add_do_method(skeleton, "set_bone_pose_rotation", p_ids[i], skeleton->get_bone_pose_rotation(p_ids[i]));
			ur->add_undo_method(skeleton, "set_bone_pose_rotation", p_ids[i], se->get_bone_original_rotation());
		}
	}
	if (ne->get_tool_mode() == SpatialEditor::TOOL_MODE_SCALE) {
		for (int i = 0; i < p_ids.size(); i++) {
			// If the axis is swapped by scaling, the rotation can be changed.
			ur->add_do_method(skeleton, "set_bone_pose_rotation", p_ids[i], skeleton->get_bone_pose_rotation(p_ids[i]));
			ur->add_undo_method(skeleton, "set_bone_pose_rotation", p_ids[i], se->get_bone_original_rotation());
			ur->add_do_method(skeleton, "set_bone_pose_scale", p_ids[i], skeleton->get_bone_pose_scale(p_ids[i]));
			ur->add_undo_method(skeleton, "set_bone_pose_scale", p_ids[i], se->get_bone_original_scale());
		}
	}
	ur->commit_action();
}

void SkeletonGizmoPlugin::redraw(EditorSpatialGizmo *p_gizmo) {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_gizmo->get_spatial_node());
	p_gizmo->clear();

	int selected = -1;
	SkeletonEditor *se = SkeletonEditor::get_singleton();
	if (se) {
		selected = se->get_selected_bone();
	}

	Color bone_color = EditorSettings::get_singleton()->get("editors/3d_gizmos/gizmo_colors/skeleton");
	Color selected_bone_color = EditorSettings::get_singleton()->get("editors/3d_gizmos/gizmo_colors/selected_bone");
	real_t bone_axis_length = EditorSettings::get_singleton()->get("editors/3d_gizmos/gizmo_settings/bone_axis_length");
	int bone_shape = EditorSettings::get_singleton()->get("editors/3d_gizmos/gizmo_settings/bone_shape");

	LocalVector<Color> axis_colors;
	axis_colors.push_back(SpatialEditor::get_singleton()->get_theme_color("axis_x_color", "Editor"));
	axis_colors.push_back(SpatialEditor::get_singleton()->get_theme_color("axis_y_color", "Editor"));
	axis_colors.push_back(SpatialEditor::get_singleton()->get_theme_color("axis_z_color", "Editor"));

	Ref<SurfaceTool> surface_tool(memnew(SurfaceTool));
	surface_tool->begin(Mesh::PRIMITIVE_LINES);

	if (p_gizmo->is_selected()) {
		surface_tool->set_material(selected_mat);
	} else {
		unselected_mat->set_albedo(bone_color);
		surface_tool->set_material(unselected_mat);
	}

	LocalVector<int> bones;
	LocalVector<float> weights;
	bones.resize(4);
	weights.resize(4);
	for (int i = 0; i < 4; i++) {
		bones[i] = 0;
		weights[i] = 0;
	}
	weights[0] = 1;

	int current_bone_index = 0;
	Vector<int> bones_to_process = skeleton->get_parentless_bones();

	while (bones_to_process.size() > current_bone_index) {
		int current_bone_idx = bones_to_process[current_bone_index];
		current_bone_index++;

		Color current_bone_color = (current_bone_idx == selected) ? selected_bone_color : bone_color;

		Vector<int> child_bones_vector;
		child_bones_vector = skeleton->get_bone_children(current_bone_idx);
		int child_bones_size = child_bones_vector.size();

		for (int i = 0; i < child_bones_size; i++) {
			// Something wrong.
			if (child_bones_vector[i] < 0) {
				continue;
			}

			int child_bone_idx = child_bones_vector[i];

			Vector3 v0 = skeleton->get_bone_global_rest(current_bone_idx).origin;
			Vector3 v1 = skeleton->get_bone_global_rest(child_bone_idx).origin;
			Vector3 d = (v1 - v0).normalized();
			real_t dist = v0.distance_to(v1);

			// Find closest axis.
			int closest = -1;
			real_t closest_d = 0.0;
			for (int j = 0; j < 3; j++) {
				real_t dp = Math::abs(skeleton->get_bone_global_rest(current_bone_idx).basis[j].normalized().dot(d));
				if (j == 0 || dp > closest_d) {
					closest = j;
				}
			}

			// Draw bone.
			switch (bone_shape) {
				case 0: { // Wire shape.
					surface_tool->add_color(current_bone_color);
					bones[0] = current_bone_idx;
					surface_tool->add_bones(Vector<int>(bones));
					surface_tool->add_weights(Vector<float>(weights));
					surface_tool->add_vertex(v0);
					bones[0] = child_bone_idx;
					surface_tool->add_bones(Vector<int>(bones));
					surface_tool->add_weights(Vector<float>(weights));
					surface_tool->add_vertex(v1);
				} break;

				case 1: { // Octahedron shape.
					Vector3 first;
					Vector3 points[6];
					int point_idx = 0;
					for (int j = 0; j < 3; j++) {
						Vector3 axis;
						if (first == Vector3()) {
							axis = d.cross(d.cross(skeleton->get_bone_global_rest(current_bone_idx).basis[j])).normalized();
							first = axis;
						} else {
							axis = d.cross(first).normalized();
						}

						surface_tool->add_color(current_bone_color);
						for (int k = 0; k < 2; k++) {
							if (k == 1) {
								axis = -axis;
							}
							Vector3 point = v0 + d * dist * 0.2;
							point += axis * dist * 0.1;

							bones[0] = current_bone_idx;
							surface_tool->add_bones(Vector<int>(bones));
							surface_tool->add_weights(Vector<float>(weights));
							surface_tool->add_vertex(v0);
							surface_tool->add_bones(Vector<int>(bones));
							surface_tool->add_vertex(point);

							surface_tool->add_bones(Vector<int>(bones));
							surface_tool->add_weights(Vector<float>(weights));
							surface_tool->add_vertex(point);
							bones[0] = child_bone_idx;
							surface_tool->add_bones(Vector<int>(bones));
							surface_tool->add_weights(Vector<float>(weights));
							surface_tool->add_vertex(v1);
							points[point_idx++] = point;
						}
					}
					surface_tool->add_color(current_bone_color);
					SWAP(points[1], points[2]);
					bones[0] = current_bone_idx;
					for (int j = 0; j < 6; j++) {
						surface_tool->add_bones(Vector<int>(bones));
						surface_tool->add_weights(Vector<float>(weights));
						surface_tool->add_vertex(points[j]);
						surface_tool->add_bones(Vector<int>(bones));
						surface_tool->add_weights(Vector<float>(weights));
						surface_tool->add_vertex(points[(j + 1) % 6]);
					}
				} break;
			}

			// Axis as root of the bone.
			for (int j = 0; j < 3; j++) {
				bones[0] = current_bone_idx;
				surface_tool->add_color(axis_colors[j]);
				surface_tool->add_bones(Vector<int>(bones));
				surface_tool->add_weights(Vector<float>(weights));
				surface_tool->add_vertex(v0);
				surface_tool->add_bones(Vector<int>(bones));
				surface_tool->add_weights(Vector<float>(weights));
				surface_tool->add_vertex(v0 + (skeleton->get_bone_global_rest(current_bone_idx).basis.inverse())[j].normalized() * dist * bone_axis_length);

				if (j == closest) {
					continue;
				}
			}

			// Axis at the end of the bone children.
			if (i == child_bones_size - 1) {
				for (int j = 0; j < 3; j++) {
					bones[0] = child_bone_idx;
					surface_tool->add_color(axis_colors[j]);
					surface_tool->add_bones(Vector<int>(bones));
					surface_tool->add_weights(Vector<float>(weights));
					surface_tool->add_vertex(v1);
					surface_tool->add_bones(Vector<int>(bones));
					surface_tool->add_weights(Vector<float>(weights));
					surface_tool->add_vertex(v1 + (skeleton->get_bone_global_rest(child_bone_idx).basis.inverse())[j].normalized() * dist * bone_axis_length);

					if (j == closest) {
						continue;
					}
				}
			}

			// Add the bone's children to the list of bones to be processed.
			bones_to_process.push_back(child_bones_vector[i]);
		}
	}

	Ref<ArrayMesh> m = surface_tool->commit();
	p_gizmo->add_mesh(m, Ref<Material>(), Transform(), skeleton->register_skin(skeleton->create_skin_from_rest_transforms()));
}
