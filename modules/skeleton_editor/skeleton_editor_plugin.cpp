/*************************************************************************/
/*  skeleton_editor_plugin.cpp                                           */
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

#include "skeleton_editor_plugin.h"

#include "core/io/resource_saver.h"
#include "editor/editor_file_dialog.h"
#include "editor/editor_properties.h"
#include "editor/editor_scale.h"
#include "editor/plugins/animation_player_editor_plugin.h"
#include "editor/plugins/spatial_editor_plugin.h"
#include "scene/3d/collision_shape.h"
#include "scene/3d/mesh_instance.h"
#include "scene/3d/physics_body.h"
#include "scene/3d/physics_joint.h"
#include "scene/main/viewport.h"
#include "scene/resources/capsule_shape.h"
#include "scene/resources/sphere_shape.h"
#include "editor/editor_spin_slider.h"
#include "scene/gui/texture_rect.h"
#include "editor/editor_inspector.h"
#include "editor/editor_spin_slider.h"
#include "editor/animation_track_editor.h"

#define DISTANCE_DEFAULT 4

#define GIZMO_ARROW_SIZE 0.35
#define GIZMO_RING_HALF_WIDTH 0.1
#define GIZMO_SCALE_DEFAULT 0.15
#define GIZMO_PLANE_SIZE 0.2
#define GIZMO_PLANE_DST 0.3
#define GIZMO_CIRCLE_SIZE 1.1
#define GIZMO_SCALE_OFFSET (GIZMO_CIRCLE_SIZE + 0.3)
#define GIZMO_ARROW_OFFSET (GIZMO_CIRCLE_SIZE + 0.3)

#define ZOOM_MIN_DISTANCE 0.001
#define ZOOM_MULTIPLIER 1.08
#define ZOOM_INDICATOR_DELAY_S 1.5

#define FREELOOK_MIN_SPEED 0.01
#define FREELOOK_SPEED_MULTIPLIER 1.08

#define MIN_Z 0.01
#define MAX_Z 1000000.0

#define MIN_FOV 0.01
#define MAX_FOV 179

void ModuleBoneTransformEditor::create_editors() {
	const Color section_color = get_color("prop_subsection", "Editor");

	section = memnew(EditorInspectorSection);
	section->setup("trf_properties", label, this, section_color, true);
	add_child(section);

	key_button = memnew(Button);
	key_button->set_text(TTR("Key Transform"));
	key_button->set_visible(keyable);
	key_button->set_icon(get_icon("Key", "EditorIcons"));
	key_button->set_flat(true);
	section->get_vbox()->add_child(key_button);

	enabled_checkbox = memnew(CheckBox(TTR("Pose Enabled")));
	enabled_checkbox->set_flat(true);
	enabled_checkbox->set_visible(toggle_enabled);
	section->get_vbox()->add_child(enabled_checkbox);

	Label *l1 = memnew(Label(TTR("Translation")));
	section->get_vbox()->add_child(l1);

	translation_grid = memnew(GridContainer());
	translation_grid->set_columns(TRANSLATION_COMPONENTS);
	section->get_vbox()->add_child(translation_grid);

	Label *l2 = memnew(Label(TTR("Rotation Degrees")));
	section->get_vbox()->add_child(l2);

	rotation_grid = memnew(GridContainer());
	rotation_grid->set_columns(ROTATION_DEGREES_COMPONENTS);
	section->get_vbox()->add_child(rotation_grid);

	Label *l3 = memnew(Label(TTR("Scale")));
	section->get_vbox()->add_child(l3);

	scale_grid = memnew(GridContainer());
	scale_grid->set_columns(SCALE_COMPONENTS);
	section->get_vbox()->add_child(scale_grid);

	Label *l4 = memnew(Label(TTR("Transform")));
	section->get_vbox()->add_child(l4);

	transform_grid = memnew(GridContainer());
	transform_grid->set_columns(TRANSFORM_CONTROL_COMPONENTS);
	section->get_vbox()->add_child(transform_grid);

	static const char *desc[TRANSFORM_COMPONENTS] = { "x", "y", "z", "x", "y", "z", "x", "y", "z", "x", "y", "z" };
	float snap = EDITOR_GET("interface/inspector/default_float_step");

	for (int i = 0; i < TRANSFORM_CONTROL_COMPONENTS; ++i) {
		translation_slider[i] = memnew(EditorSpinSlider());
		translation_slider[i]->set_label(desc[i]);
		translation_slider[i]->set_step(snap);
		setup_spinner(translation_slider[i], false);
		translation_grid->add_child(translation_slider[i]);

		rotation_slider[i] = memnew(EditorSpinSlider());
		rotation_slider[i]->set_label(desc[i]);
		rotation_slider[i]->set_step(snap);
		setup_spinner(rotation_slider[i], false);
		rotation_grid->add_child(rotation_slider[i]);

		scale_slider[i] = memnew(EditorSpinSlider());
		scale_slider[i]->set_label(desc[i]);
		scale_slider[i]->set_step(snap);
		setup_spinner(scale_slider[i], false);
		scale_grid->add_child(scale_slider[i]);
	}

	for (int i = 0; i < TRANSFORM_COMPONENTS; ++i) {
		transform_slider[i] = memnew(EditorSpinSlider());
		transform_slider[i]->set_label(desc[i]);
		transform_slider[i]->set_step(snap);
		setup_spinner(transform_slider[i], true);
		transform_grid->add_child(transform_slider[i]);
	}
}

void ModuleBoneTransformEditor::setup_spinner(EditorSpinSlider *spinner, const bool is_transform_spinner) {
	spinner->set_flat(true);
	spinner->set_min(-10000);
	spinner->set_max(10000);
	spinner->set_hide_slider(true);
	spinner->set_allow_greater(true);
	spinner->set_allow_lesser(true);
	spinner->set_h_size_flags(SIZE_EXPAND_FILL);

	spinner->connect("value_changed", this, "_value_changed", varray(is_transform_spinner));
}

void ModuleBoneTransformEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			create_editors();
			key_button->connect("pressed", this, "_key_button_pressed");
			enabled_checkbox->connect("toggled", this, "_checkbox_toggled");
			FALLTHROUGH;
		}
		case NOTIFICATION_THEME_CHANGED: {
			const Color base = get_color("accent_color", "Editor");
			const Color bg_color = get_color("property_color", "Editor");
			const Color bg_lbl_color(bg_color.r, bg_color.g, bg_color.b, 0.5);

			for (int i = 0; i < TRANSLATION_COMPONENTS; i++) {
				Color c = base;
				c.set_hsv(float(i % TRANSLATION_COMPONENTS) / TRANSLATION_COMPONENTS + 0.05, c.get_s() * 0.75, c.get_v());
				if (!translation_slider[i]) {
					continue;
				}
				translation_slider[i]->set_custom_label_color(true, c);
			}

			for (int i = 0; i < ROTATION_DEGREES_COMPONENTS; i++) {
				Color c = base;
				c.set_hsv(float(i % ROTATION_DEGREES_COMPONENTS) / ROTATION_DEGREES_COMPONENTS + 0.05, c.get_s() * 0.75, c.get_v());
				if (!rotation_slider[i]) {
					continue;
				}
				rotation_slider[i]->set_custom_label_color(true, c);
			}

			for (int i = 0; i < SCALE_COMPONENTS; i++) {
				Color c = base;
				c.set_hsv(float(i % SCALE_COMPONENTS) / SCALE_COMPONENTS + 0.05, c.get_s() * 0.75, c.get_v());
				if (!scale_slider[i]) {
					continue;
				}
				scale_slider[i]->set_custom_label_color(true, c);
			}

			for (int i = 0; i < TRANSFORM_COMPONENTS; i++) {
				Color c = base;
				c.set_hsv(float(i % TRANSFORM_COMPONENTS) / TRANSFORM_COMPONENTS + 0.05, c.get_s() * 0.75, c.get_v());
				if (!transform_slider[i]) {
					continue;
				}
				transform_slider[i]->set_custom_label_color(true, c);
			}

			break;
		}
		case NOTIFICATION_SORT_CHILDREN: {
			const Ref<Font> font = get_font("font", "Tree");

			Point2 buffer;
			buffer.x += get_constant("inspector_margin", "Editor");
			buffer.y += font->get_height();
			buffer.y += get_constant("vseparation", "Tree");

			const float vector_height = translation_grid->get_size().y;
			const float transform_height = transform_grid->get_size().y;
			const float button_height = key_button->get_size().y;

			const float width = get_size().x - get_constant("inspector_margin", "Editor");
			Vector<Rect2> input_rects;
			if (keyable && section->get_vbox()->is_visible()) {
				input_rects.push_back(Rect2(key_button->get_position() + buffer, Size2(width, button_height)));
			} else {
				input_rects.push_back(Rect2(0, 0, 0, 0));
			}

			if (section->get_vbox()->is_visible()) {
				input_rects.push_back(Rect2(translation_grid->get_position() + buffer, Size2(width, vector_height)));
				input_rects.push_back(Rect2(rotation_grid->get_position() + buffer, Size2(width, vector_height)));
				input_rects.push_back(Rect2(scale_grid->get_position() + buffer, Size2(width, vector_height)));
				input_rects.push_back(Rect2(transform_grid->get_position() + buffer, Size2(width, transform_height)));
			} else {
				const int32_t start = input_rects.size();
				const int32_t empty_input_rect_elements = 4;
				const int32_t end = start + empty_input_rect_elements;
				for (int i = start; i < end; ++i) {
					input_rects.push_back(Rect2(0, 0, 0, 0));
				}
			}

			for (int32_t i = 0; i < input_rects.size(); i++) {
				background_rects[i] = input_rects[i];
			}

			update();
			break;
		}
		case NOTIFICATION_DRAW: {
			const Color dark_color = get_color("dark_color_2", "Editor");

			for (int i = 0; i < 5; ++i) {
				draw_rect(background_rects[i], dark_color);
			}

			break;
		}
	}
}

void ModuleBoneTransformEditor::_value_changed(const double p_value, const bool p_from_transform) {
	if (updating)
		return;

	if (property.get_slicec('/', 0) == "bones" && property.get_slicec('/', 2) == "custom_pose") {
		const Transform tform = compute_transform(p_from_transform);

		undo_redo->create_action(TTR("Set Custom Bone Pose Transform"), UndoRedo::MERGE_ENDS);
		undo_redo->add_undo_method(skeleton, "set_bone_custom_pose", property.get_slicec('/', 1).to_int(), skeleton->get_bone_custom_pose(property.get_slicec('/', 1).to_int()));
		undo_redo->add_do_method(skeleton, "set_bone_custom_pose", property.get_slicec('/', 1).to_int(), tform);
		undo_redo->commit_action();
	} else if (property.get_slicec('/', 0) == "bones") {
		const Transform tform = compute_transform(p_from_transform);

		undo_redo->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);
		undo_redo->add_undo_property(skeleton, property, skeleton->get(property));
		undo_redo->add_do_property(skeleton, property, tform);
		undo_redo->commit_action();
	}
}

Transform ModuleBoneTransformEditor::compute_transform(const bool p_from_transform) const {
	// Last modified was a raw transform column...
	if (p_from_transform) {
		Transform tform;

		for (int i = 0; i < BASIS_COMPONENTS; ++i) {
			tform.basis[i / BASIS_SPLIT_COMPONENTS][i % BASIS_SPLIT_COMPONENTS] = transform_slider[i]->get_value();
		}

		for (int i = 0; i < TRANSLATION_COMPONENTS; ++i) {
			tform.origin[i] = transform_slider[i + BASIS_COMPONENTS]->get_value();
		}

		return tform;
	}

	return Transform(
			Basis(Vector3(Math::deg2rad(rotation_slider[0]->get_value()), Math::deg2rad(rotation_slider[1]->get_value()), Math::deg2rad(rotation_slider[2]->get_value())),
					Vector3(scale_slider[0]->get_value(), scale_slider[1]->get_value(), scale_slider[2]->get_value())),
			Vector3(translation_slider[0]->get_value(), translation_slider[1]->get_value(), translation_slider[2]->get_value()));
}

void ModuleBoneTransformEditor::update_enabled_checkbox() {
	if (property == "") {
		return;
	}

	if (enabled_checkbox) {
		const String path = "bones/" + property.get_slicec('/', 1) + "/enabled";
		const bool is_enabled = skeleton->get(path);
		enabled_checkbox->set_pressed(is_enabled);
	}
}

void ModuleBoneTransformEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_value_changed", "value"), &ModuleBoneTransformEditor::_value_changed);
	ClassDB::bind_method(D_METHOD("_key_button_pressed"), &ModuleBoneTransformEditor::_key_button_pressed);
	ClassDB::bind_method(D_METHOD("_checkbox_toggled", "toggled"), &ModuleBoneTransformEditor::_checkbox_toggled);
}

void ModuleBoneTransformEditor::_update_properties() {
	if (updating)
		return;

	if (skeleton == nullptr)
		return;

	updating = true;

	Transform tform = skeleton->get(property);
	_update_transform_properties(tform);
}

void ModuleBoneTransformEditor::_update_custom_pose_properties() {
	if (updating)
		return;

	if (skeleton == nullptr)
		return;

	updating = true;

	int idx = property.to_int();

	if (idx == 0) {
		return;
	}

	Transform tform = skeleton->get_bone_custom_pose(idx);
	_update_transform_properties(tform);
}

void ModuleBoneTransformEditor::_update_transform_properties(Transform tform) {
	Quat rot = tform.get_basis().orthonormalized();
	Vector3 rot_rad = rot.get_euler();
	Vector3 rot_degrees = Vector3(Math::rad2deg(rot_rad.x), Math::rad2deg(rot_rad.y), Math::rad2deg(rot_rad.z));
	Vector3 tr = tform.get_origin();
	Vector3 scale = tform.basis.get_scale();

	for (int i = 0; i < TRANSLATION_COMPONENTS; i++) {
		translation_slider[i]->set_value(tr[i]);
	}

	for (int i = 0; i < ROTATION_DEGREES_COMPONENTS; i++) {
		rotation_slider[i]->set_value(rot_degrees[i]);
	}

	for (int i = 0; i < SCALE_COMPONENTS; i++) {
		scale_slider[i]->set_value(scale[i]);
	}

	transform_slider[0]->set_value(tform.get_basis()[Vector3::AXIS_X].x);
	transform_slider[1]->set_value(tform.get_basis()[Vector3::AXIS_Y].x);
	transform_slider[2]->set_value(tform.get_basis()[Vector3::AXIS_Z].x);
	transform_slider[3]->set_value(tform.get_basis()[Vector3::AXIS_X].y);
	transform_slider[4]->set_value(tform.get_basis()[Vector3::AXIS_Y].y);
	transform_slider[5]->set_value(tform.get_basis()[Vector3::AXIS_Z].y);
	transform_slider[6]->set_value(tform.get_basis()[Vector3::AXIS_X].z);
	transform_slider[7]->set_value(tform.get_basis()[Vector3::AXIS_Y].z);
	transform_slider[8]->set_value(tform.get_basis()[Vector3::AXIS_Z].z);

	for (int i = 0; i < TRANSLATION_COMPONENTS; i++) {
		transform_slider[BASIS_COMPONENTS + i]->set_value(tform.get_origin()[i]);
	}

	update_enabled_checkbox();
	updating = false;
}

ModuleBoneTransformEditor::ModuleBoneTransformEditor(Skeleton *p_skeleton) :
		translation_slider(),
		rotation_slider(),
		scale_slider(),
		transform_slider(),
		skeleton(p_skeleton),
		key_button(nullptr),
		enabled_checkbox(nullptr),
		keyable(false),
		toggle_enabled(false),
		updating(false) {
	undo_redo = EditorNode::get_undo_redo();
}

void ModuleBoneTransformEditor::set_target(const String &p_prop) {
	property = p_prop;
}

void ModuleBoneTransformEditor::set_keyable(const bool p_keyable) {
	keyable = p_keyable;
	if (key_button) {
		key_button->set_visible(p_keyable);
	}
}

void ModuleBoneTransformEditor::set_toggle_enabled(const bool p_enabled) {
	toggle_enabled = p_enabled;
	if (enabled_checkbox) {
		enabled_checkbox->set_visible(p_enabled);
	}
}

void ModuleBoneTransformEditor::_key_button_pressed() {
	if (skeleton == nullptr)
		return;

	const BoneId bone_id = property.get_slicec('/', 1).to_int();
	const String name = skeleton->get_bone_name(bone_id);

	if (name.empty())
		return;

	// Need to normalize the basis before you key it
	Transform tform = compute_transform(true);
	tform.orthonormalize();
	AnimationPlayerEditor::singleton->get_track_editor()->insert_transform_key(skeleton, name, tform);
}

void ModuleBoneTransformEditor::_checkbox_toggled(const bool p_toggled) {
	if (enabled_checkbox) {
		const String path = "bones/" + property.get_slicec('/', 1) + "/enabled";
		skeleton->set(path, p_toggled);
	}
}

void ModuleBoneTransformEditor::set_read_only(const bool p_read_only) {
	for (int i = 0; i < TRANSLATION_COMPONENTS; i++) {
		translation_slider[i]->set_read_only(p_read_only);
	}
	for (int i = 0; i < ROTATION_DEGREES_COMPONENTS; i++) {
		rotation_slider[i]->set_read_only(p_read_only);
	}
	for (int i = 0; i < SCALE_COMPONENTS; i++) {
		scale_slider[i]->set_read_only(p_read_only);
	}
	for (int i = 0; i < TRANSFORM_COMPONENTS; i++) {
		transform_slider[i]->set_read_only(p_read_only);
	}
}

void ModuleSkeletonEditor::set_keyable(const bool p_keyable) {
	keyable = p_keyable;
	options->get_popup()->set_item_disabled(MENU_OPTION_INSERT_KEYS, !p_keyable);
	options->get_popup()->set_item_disabled(MENU_OPTION_INSERT_KEYS_EXISTED, !p_keyable);
};

void ModuleSkeletonEditor::_on_click_option(int p_option) {
	if (!skeleton) {
		return;
	}

	switch (p_option) {
		case MENU_OPTION_INIT_POSE: {
			init_pose();
		} break;
		case MENU_OPTION_INSERT_KEYS: {
			insert_keys(true);
		} break;
		case MENU_OPTION_INSERT_KEYS_EXISTED: {
			insert_keys(false);
		} break;
		case MENU_OPTION_POSE_TO_REST: {
			pose_to_rest();
		} break;
		case MENU_OPTION_CREATE_PHYSICAL_SKELETON: {
			create_physical_skeleton();
		} break;
		case MENU_OPTION_ADD_BONE: {
			add_bone();
		} break;
		case MENU_OPTION_REMOVE_BONE: {
			remove_bone();
		} break;
		case MENU_OPTION_RENAME_BONE: {
			rename_bone();
		} break;
	}
}

void ModuleSkeletonEditor::init_pose() {
	const int bone_len = skeleton->get_bone_count();
	if (!bone_len) {
		return;
	}
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ur->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);
	for (int i = 0; i < bone_len; i++) {
		ur->add_do_method(skeleton, "set_bone_pose", i, Transform());
		ur->add_undo_method(skeleton, "set_bone_pose", i, skeleton->get_bone_pose(i));
	}
	ur->commit_action();
}

void ModuleSkeletonEditor::insert_keys(bool p_all_bones) {
	if (skeleton == nullptr)
		return;

	int bone_len = skeleton->get_bone_count();
	Node *root = EditorNode::get_singleton()->get_tree()->get_root();
	String path = root->get_path_to(skeleton);

	for (int i = 0; i < bone_len; i++) {
		const String name = skeleton->get_bone_name(i);

		if (name.empty())
			continue;

		if (!p_all_bones && !AnimationPlayerEditor::singleton->get_track_editor()->has_transform_key(skeleton, name)) {
			continue;
		}

		// Need to normalize the basis before you key it
		Transform tform = skeleton->get_bone_pose(i);
		tform.orthonormalize();
		AnimationPlayerEditor::singleton->get_track_editor()->insert_transform_key(skeleton, name, tform);
	}
}

void ModuleSkeletonEditor::pose_to_rest() {
	const int bone_len = skeleton->get_bone_count();
	if (!bone_len) {
		return;
	}
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ur->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);
	for (int i = 0; i < bone_len; i++) {
		ur->add_do_method(skeleton, "set_bone_pose", i, Transform());
		ur->add_undo_method(skeleton, "set_bone_pose", i, skeleton->get_bone_pose(i));
		ur->add_do_method(skeleton, "set_bone_custom_pose", i, Transform());
		ur->add_undo_method(skeleton, "set_bone_custom_pose", i, skeleton->get_bone_custom_pose(i));
		ur->add_do_method(skeleton, "set_bone_rest", i, skeleton->get_bone_rest(i) * skeleton->get_bone_custom_pose(i) * skeleton->get_bone_pose(i));
		ur->add_undo_method(skeleton, "set_bone_rest", i, skeleton->get_bone_rest(i));
	}
	ur->commit_action();
}

void ModuleSkeletonEditor::create_physical_skeleton() {
	UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
	ERR_FAIL_COND(!get_tree());
	Node *owner = skeleton == get_tree()->get_edited_scene_root() ? skeleton : skeleton->get_owner();

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

PhysicalBone *ModuleSkeletonEditor::create_physical_bone(int bone_id, int bone_child_id, const Vector<BoneInfo> &bones_infos) {
	real_t half_height(skeleton->get_bone_rest(bone_child_id).origin.length() * 0.5);
	real_t radius(half_height * 0.2);

	CapsuleShape *bone_shape_capsule = memnew(CapsuleShape);
	bone_shape_capsule->set_height((half_height - radius) * 2);
	bone_shape_capsule->set_radius(radius);

	CollisionShape *bone_shape = memnew(CollisionShape);
	bone_shape->set_shape(bone_shape_capsule);

	Transform body_transform;
	body_transform.origin = Vector3(0, 0, -half_height);

	Transform joint_transform;
	joint_transform.origin = Vector3(0, 0, half_height);

	PhysicalBone *physical_bone = memnew(PhysicalBone);
	physical_bone->add_child(bone_shape);
	physical_bone->set_name("Physical Bone " + skeleton->get_bone_name(bone_id));
	physical_bone->set_body_offset(body_transform);
	physical_bone->set_joint_offset(joint_transform);
	return physical_bone;
}

Variant ModuleSkeletonEditor::get_drag_data_fw(const Point2 &p_point, Control *p_from) {
	TreeItem *selected = joint_tree->get_selected();

	if (!selected)
		return Variant();

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

bool ModuleSkeletonEditor::can_drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) const {
	TreeItem *target = joint_tree->get_item_at_position(p_point);
	if (!target)
		return false;

	const String path = target->get_metadata(0);
	if (!path.begins_with("bones/"))
		return false;

	TreeItem *selected = Object::cast_to<TreeItem>(Dictionary(p_data)["node"]);
	if (target == selected)
		return false;

	const String path2 = target->get_metadata(0);
	if (!path2.begins_with("bones/"))
		return false;

	return true;
}

void ModuleSkeletonEditor::drop_data_fw(const Point2 &p_point, const Variant &p_data, Control *p_from) {
	if (!can_drop_data_fw(p_point, p_data, p_from))
		return;

	TreeItem *target = joint_tree->get_item_at_position(p_point);
	TreeItem *selected = Object::cast_to<TreeItem>(Dictionary(p_data)["node"]);

	const BoneId target_boneidx = String(target->get_metadata(0)).get_slicec('/', 1).to_int();
	const BoneId selected_boneidx = String(selected->get_metadata(0)).get_slicec('/', 1).to_int();

	move_skeleton_bone(skeleton->get_path(), selected_boneidx, target_boneidx);
}

void ModuleSkeletonEditor::move_skeleton_bone(NodePath p_skeleton_path, int32_t p_selected_boneidx, int32_t p_target_boneidx) {
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

	update_joint_tree();
	ur->commit_action();
}

void ModuleSkeletonEditor::_update_spatial_transform_gizmo() {
	SpatialEditor::get_singleton()->clear_externals();
	if (skeleton->get_selected_bone() >= 0) {
		SpatialEditor::get_singleton()->append_to_externals(skeleton->get_global_transform() * skeleton->get_bone_global_pose(skeleton->get_selected_bone()));
	}
	SpatialEditor::get_singleton()->update_transform_gizmo();
};

void ModuleSkeletonEditor::_joint_tree_selection_changed() {
	TreeItem *selected = joint_tree->get_selected();
	const String path = selected->get_metadata(0);

	if (path.begins_with("bones/")) {
		const int b_idx = path.get_slicec('/', 1).to_int();
		const String bone_path = "bones/" + itos(b_idx) + "/";

		pose_editor->set_target(bone_path + "pose");
		rest_editor->set_target(bone_path + "rest");
		custom_pose_editor->set_target(bone_path + "custom_pose");

		pose_editor->set_visible(true);
		rest_editor->set_visible(true);
		custom_pose_editor->set_visible(true);

		skeleton->set_selected_bone(b_idx);
	}

	_update_properties();
}

void ModuleSkeletonEditor::_joint_tree_rmb_select(const Vector2 &p_pos) {
	skeleton->set_selected_bone(-1);
	_update_spatial_transform_gizmo();
}

void ModuleSkeletonEditor::_update_properties() {
	if (rest_editor)
		rest_editor->_update_properties();
	if (pose_editor)
		pose_editor->_update_properties();
	if (custom_pose_editor)
		custom_pose_editor->_update_custom_pose_properties();
	_update_spatial_transform_gizmo();
}

void ModuleSkeletonEditor::update_joint_tree() {
	joint_tree->clear();

	if (skeleton == nullptr)
		return;

	TreeItem *root = joint_tree->create_item();

	Map<int, TreeItem *> items;

	items.insert(-1, root);

	const Vector<int> &joint_porder = skeleton->get_bone_process_order();

	Ref<Texture> bone_icon = get_icon("Bone", "EditorIcons");

	for (int i = 0; i < joint_porder.size(); ++i) {
		const int b_idx = joint_porder[i];

		const int p_idx = skeleton->get_bone_parent(b_idx);
		TreeItem *p_item = items.find(p_idx)->get();

		TreeItem *joint_item = joint_tree->create_item(p_item);
		items.insert(b_idx, joint_item);

		joint_item->set_text(0, skeleton->get_bone_name(b_idx));
		joint_item->set_icon(0, bone_icon);
		joint_item->set_selectable(0, true);
		joint_item->set_metadata(0, "bones/" + itos(b_idx));
	}
}

void ModuleSkeletonEditor::update_editors() {
}

void ModuleSkeletonEditor::create_editors() {
	set_h_size_flags(SIZE_EXPAND_FILL);
	add_constant_override("separation", 0);

	set_focus_mode(FOCUS_ALL);

	// Create Top Menu Bar
	separators[0] = memnew(VSeparator);
	separators[1] = memnew(VSeparator);

	SpatialEditor::get_singleton()->add_control_to_menu_panel(separators[0]);

	options = memnew(MenuButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(options);
	options->set_text(TTR("Skeleton"));
	options->set_icon(EditorNode::get_singleton()->get_gui_base()->get_icon("Skeleton", "EditorIcons"));
	options->get_popup()->add_item(TTR("Init pose"), MENU_OPTION_INIT_POSE);
	options->get_popup()->add_item(TTR("Insert key of all bone poses"), MENU_OPTION_INSERT_KEYS);
	options->get_popup()->add_item(TTR("Insert key of bone poses already exist track"), MENU_OPTION_INSERT_KEYS_EXISTED);
	options->get_popup()->add_item(TTR("Apply current pose to rest"), MENU_OPTION_POSE_TO_REST);
	options->get_popup()->add_item(TTR("Create physical skeleton"), MENU_OPTION_CREATE_PHYSICAL_SKELETON);
	options->get_popup()->add_item(TTR("Add bone"), MENU_OPTION_ADD_BONE);
	options->get_popup()->add_item(TTR("Rename bone"), MENU_OPTION_RENAME_BONE);
	options->get_popup()->add_item(TTR("Remove bone"), MENU_OPTION_REMOVE_BONE);
	options->get_popup()->connect("id_pressed", this, "_on_click_option");

	Vector<Variant> button_binds;
	button_binds.resize(1);

	tool_button[TOOL_MODE_BONE_SELECT] = memnew(ToolButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(tool_button[TOOL_MODE_BONE_SELECT]);
	tool_button[TOOL_MODE_BONE_SELECT]->set_tooltip(TTR("Transform Bone Mode"));
	tool_button[TOOL_MODE_BONE_SELECT]->set_toggle_mode(true);
	tool_button[TOOL_MODE_BONE_SELECT]->set_flat(true);
	button_binds.write[0] = MENU_TOOL_BONE_SELECT;
	tool_button[TOOL_MODE_BONE_SELECT]->connect("pressed", this, "_menu_tool_item_pressed", button_binds);

	tool_button[TOOL_MODE_BONE_MOVE] = memnew(ToolButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(tool_button[TOOL_MODE_BONE_MOVE]);
	tool_button[TOOL_MODE_BONE_MOVE]->set_tooltip(TTR("Move Bone Mode"));
	tool_button[TOOL_MODE_BONE_MOVE]->set_toggle_mode(true);
	tool_button[TOOL_MODE_BONE_MOVE]->set_flat(true);
	button_binds.write[0] = MENU_TOOL_BONE_MOVE;
	tool_button[TOOL_MODE_BONE_MOVE]->connect("pressed", this, "_menu_tool_item_pressed", button_binds);

	tool_button[TOOL_MODE_BONE_ROTATE] = memnew(ToolButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(tool_button[TOOL_MODE_BONE_ROTATE]);
	tool_button[TOOL_MODE_BONE_ROTATE]->set_tooltip(TTR("Rotate Bone Mode"));
	tool_button[TOOL_MODE_BONE_ROTATE]->set_toggle_mode(true);
	tool_button[TOOL_MODE_BONE_ROTATE]->set_flat(true);
	button_binds.write[0] = MENU_TOOL_BONE_ROTATE;
	tool_button[TOOL_MODE_BONE_ROTATE]->connect("pressed", this, "_menu_tool_item_pressed", button_binds);

	tool_button[TOOL_MODE_BONE_SCALE] = memnew(ToolButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(tool_button[TOOL_MODE_BONE_SCALE]);
	tool_button[TOOL_MODE_BONE_SCALE]->set_tooltip(TTR("Scale Bone Mode"));
	tool_button[TOOL_MODE_BONE_SCALE]->set_toggle_mode(true);
	tool_button[TOOL_MODE_BONE_SCALE]->set_flat(true);
	button_binds.write[0] = MENU_TOOL_BONE_SCALE;
	tool_button[TOOL_MODE_BONE_SCALE]->connect("pressed", this, "_menu_tool_item_pressed", button_binds);

	tool_button[TOOL_MODE_BONE_NONE] = memnew(ToolButton);
	button_binds.write[0] = MENU_TOOL_BONE_NONE;
	tool_button[TOOL_MODE_BONE_NONE]->connect("pressed", this, "_menu_tool_item_pressed", button_binds);
	SpatialEditor::get_singleton()->connect("change_tool_mode", this, "_menu_tool_item_pressed", button_binds);

	tool_mode = TOOL_MODE_BONE_NONE;

	SpatialEditor::get_singleton()->add_control_to_menu_panel(separators[1]);

	rest_mode_button = memnew(ToolButton);
	SpatialEditor::get_singleton()->add_control_to_menu_panel(rest_mode_button);
	rest_mode_button->set_tooltip(TTR("Rest Mode\nNote: Bone poses are disabled during Rest Mode."));
	rest_mode_button->set_toggle_mode(true);
	rest_mode_button->set_flat(true);
	rest_mode_button->connect("toggled", this, "rest_mode_toggled");

	rest_mode = false;

	set_keyable(AnimationPlayerEditor::singleton->get_track_editor()->has_keying());

	if (skeleton) {
		skeleton->add_child(pointsm);
		pointsm->set_skeleton_path(NodePath(""));
		skeleton->connect("skeleton_updated", this, "_draw_handles");
	}

	const Color section_color = get_color("prop_subsection", "Editor");

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
	joint_tree->set_focus_mode(Control::FocusMode::FOCUS_NONE);
	joint_tree->set_select_mode(Tree::SELECT_SINGLE);
	joint_tree->set_hide_root(true);
	joint_tree->set_v_size_flags(SIZE_EXPAND_FILL);
	joint_tree->set_h_size_flags(SIZE_EXPAND_FILL);
	joint_tree->set_allow_rmb_select(true);
	joint_tree->set_drag_forwarding(this);
	s_con->add_child(joint_tree);

	pose_editor = memnew(ModuleBoneTransformEditor(skeleton));
	pose_editor->set_label(TTR("Bone Pose"));
	pose_editor->set_keyable(AnimationPlayerEditor::singleton->get_track_editor()->has_keying());
	// pose_editor->set_toggle_enabled(true);
	pose_editor->set_visible(false);
	add_child(pose_editor);

	rest_editor = memnew(ModuleBoneTransformEditor(skeleton));
	rest_editor->set_label(TTR("Bone Rest"));
	rest_editor->set_visible(false);
	add_child(rest_editor);

	custom_pose_editor = memnew(ModuleBoneTransformEditor(skeleton));
	custom_pose_editor->set_label(TTR("Bone Custom Pose"));
	custom_pose_editor->set_visible(false);
	add_child(custom_pose_editor);

	skeleton->set_selected_bone(-1);
}

void ModuleSkeletonEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			tool_button[TOOL_MODE_BONE_SELECT]->set_icon(get_icon("ToolBoneSelect", "EditorIcons"));
			tool_button[TOOL_MODE_BONE_MOVE]->set_icon(get_icon("ToolBoneMove", "EditorIcons"));
			tool_button[TOOL_MODE_BONE_ROTATE]->set_icon(get_icon("ToolBoneRotate", "EditorIcons"));
			tool_button[TOOL_MODE_BONE_SCALE]->set_icon(get_icon("ToolBoneScale", "EditorIcons"));
			rest_mode_button->set_icon(get_icon("ToolBoneRest", "EditorIcons"));
		} break;
		case NOTIFICATION_ENTER_TREE: {
			create_editors();
			update_joint_tree();
			update_editors();

			get_tree()->connect("node_removed", this, "_node_removed", Vector<Variant>(), Object::CONNECT_ONESHOT);
			joint_tree->connect("item_selected", this, "_joint_tree_selection_changed");
			joint_tree->connect("item_rmb_selected", this, "_joint_tree_rmb_select");

#ifdef TOOLS_ENABLED
			skeleton->connect("skeleton_updated", this, "_update_properties");
#endif // TOOLS_ENABLED

			break;
		}
	}
}

void ModuleSkeletonEditor::_node_removed(Node *p_node) {
	if (skeleton && p_node == skeleton) {
		skeleton = nullptr;
	}
}

void ModuleSkeletonEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_node_removed"), &ModuleSkeletonEditor::_node_removed);
	ClassDB::bind_method(D_METHOD("_joint_tree_selection_changed"), &ModuleSkeletonEditor::_joint_tree_selection_changed);
	ClassDB::bind_method(D_METHOD("_joint_tree_rmb_select"), &ModuleSkeletonEditor::_joint_tree_rmb_select);
	ClassDB::bind_method(D_METHOD("_update_properties"), &ModuleSkeletonEditor::_update_properties);
	ClassDB::bind_method(D_METHOD("_on_click_option"), &ModuleSkeletonEditor::_on_click_option);
	ClassDB::bind_method(D_METHOD("_menu_tool_item_pressed"), &ModuleSkeletonEditor::_menu_tool_item_pressed);
	ClassDB::bind_method(D_METHOD("rest_mode_toggled"), &ModuleSkeletonEditor::rest_mode_toggled);
	ClassDB::bind_method(D_METHOD("set_rest_mode_toggled"), &ModuleSkeletonEditor::set_rest_mode_toggled);

	ClassDB::bind_method(D_METHOD("get_drag_data_fw"), &ModuleSkeletonEditor::get_drag_data_fw);
	ClassDB::bind_method(D_METHOD("can_drop_data_fw"), &ModuleSkeletonEditor::can_drop_data_fw);
	ClassDB::bind_method(D_METHOD("drop_data_fw"), &ModuleSkeletonEditor::drop_data_fw);
	ClassDB::bind_method(D_METHOD("move_skeleton_bone"), &ModuleSkeletonEditor::move_skeleton_bone);

	ClassDB::bind_method(D_METHOD("_draw_handles"), &ModuleSkeletonEditor::_draw_handles);

	_bind_tool_popup_methods();
}

void ModuleSkeletonEditor::_menu_tool_item_pressed(int p_option) {
	if (p_option != TOOL_MODE_BONE_NONE && !SpatialEditor::get_singleton()->is_tool_external()) {
		SpatialEditor::get_singleton()->set_tool_mode(SpatialEditor::TOOL_MODE_EXTERNAL);
	}
	for (int i = 0; i < TOOL_MODE_BONE_MAX; i++)
		tool_button[i]->set_pressed(i == p_option);
	tool_mode = (ToolMode)p_option;
	if (skeleton) {
		if (p_option == TOOL_MODE_BONE_NONE) {
			_hide_handles();
		} else {
			_draw_handles();
			if (skeleton->get_selected_bone() >= 0) {
				SpatialEditor::get_singleton()->clear_externals();
				SpatialEditor::get_singleton()->append_to_externals(skeleton->get_global_transform() * skeleton->get_bone_global_pose(skeleton->get_selected_bone()));
			}
		}
	}

	switch (p_option) {
		case TOOL_MODE_BONE_SELECT: {
			SpatialEditor::get_singleton()->set_external_tool_mode(SpatialEditor::EX_TOOL_MODE_SELECT);
		} break;
		case TOOL_MODE_BONE_MOVE: {
			SpatialEditor::get_singleton()->set_external_tool_mode(SpatialEditor::EX_TOOL_MODE_MOVE);
		} break;
		case TOOL_MODE_BONE_ROTATE: {
			SpatialEditor::get_singleton()->set_external_tool_mode(SpatialEditor::EX_TOOL_MODE_ROTATE);
		} break;
		case TOOL_MODE_BONE_SCALE: {
			SpatialEditor::get_singleton()->set_external_tool_mode(SpatialEditor::EX_TOOL_MODE_SCALE);
		} break;
		case TOOL_MODE_BONE_NONE:
			break;
	}

	_update_spatial_transform_gizmo();
}

void ModuleSkeletonEditor::rest_mode_toggled(const bool pressed) {
	bool before_val = rest_mode;

	// Prevent that bone pose will be undo during rest mode.
	// However ModuleSkeletonEditor will be memdeleted,
	// so it need to record in SpatialEditor with calling method in
	// ModuleEditorInspectorPluginSkeleton and it will not be memdeleted.
	UndoRedo *ur = SpatialEditor::get_singleton()->get_undo_redo();
	ur->create_action(TTR("Toggled Rest Mode"));
	set_rest_mode_toggled(pressed);
	ur->add_undo_method(editor_plugin, "set_rest_mode_toggled", before_val);
	ur->add_do_method(editor_plugin, "set_rest_mode_toggled", pressed);
	ur->commit_action();
}

void ModuleSkeletonEditor::set_rest_mode_toggled(const bool pressed) {
	rest_mode_button->disconnect("toggled", this, "rest_mode_toggled");
	rest_mode_button->set_pressed(pressed);
	rest_mode_button->connect("toggled", this, "rest_mode_toggled");

	rest_mode = pressed;
	const int bone_len = skeleton->get_bone_count();
	for (int i = 0; i < bone_len; i++) {
		skeleton->set_bone_enabled(i, !rest_mode);
	}
	if (pose_editor) {
		pose_editor->set_read_only(rest_mode);
	}
	if (custom_pose_editor) {
		custom_pose_editor->set_read_only(rest_mode);
	}
	set_keyable(AnimationPlayerEditor::singleton->get_track_editor()->has_keying() && !rest_mode);
}

ModuleSkeletonEditor::ModuleSkeletonEditor(ModuleEditorInspectorPluginSkeleton *e_plugin, EditorNode *p_editor, Skeleton *p_skeleton) :
		editor(p_editor),
		editor_plugin(e_plugin),
		skeleton(p_skeleton) {
	handle_material = Ref<ShaderMaterial>(memnew(ShaderMaterial));
	handle_shader = Ref<Shader>(memnew(Shader));
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
	Ref<Texture> handle = editor->get_gui_base()->get_icon("EditorBoneHandle", "EditorIcons");
	handle_material->set_shader_param("point_size", handle->get_width());
	handle_material->set_shader_param("texture_albedo", handle);
	//handle_material->set_texture(SpatialMaterial::TEXTURE_ALBEDO, handle);

	pointsm = memnew(MeshInstance);
	am.instance();
	pointsm->set_mesh(am);
	pointsm->set_transform(Transform(Basis(), Vector3(0, 0, 0.00001)));

	create_bone_tool_popups();
}

ModuleSkeletonEditor::~ModuleSkeletonEditor() {
	set_rest_mode_toggled(false);
	SpatialEditor::get_singleton()->disconnect("change_tool_mode", this, "_menu_tool_item_pressed");
	if (skeleton) {
		pointsm->get_parent()->remove_child(pointsm);
		skeleton->set_selected_bone(-1);
		skeleton->disconnect("skeleton_updated", this, "_draw_handles");
		memdelete(pointsm);
	}
	for (int i = 0; i < 2; i++) {
		if (separators[i]) {
			SpatialEditor::get_singleton()->remove_control_from_menu_panel(separators[i]);
			memdelete(separators[i]);
		}
	}
	if (options) {
		SpatialEditor::get_singleton()->remove_control_from_menu_panel(options);
		memdelete(options);
	}
	SpatialEditor::get_singleton()->remove_control_from_menu_panel(tool_button[TOOL_MODE_BONE_SELECT]);
	SpatialEditor::get_singleton()->remove_control_from_menu_panel(tool_button[TOOL_MODE_BONE_MOVE]);
	SpatialEditor::get_singleton()->remove_control_from_menu_panel(tool_button[TOOL_MODE_BONE_ROTATE]);
	SpatialEditor::get_singleton()->remove_control_from_menu_panel(tool_button[TOOL_MODE_BONE_SCALE]);
	for (int i = 0; i < TOOL_MODE_BONE_MAX; i++) {
		if (tool_button[i]) {
			memdelete(tool_button[i]);
		}
	}
	SpatialEditor::get_singleton()->remove_control_from_menu_panel(rest_mode_button);
	if (rest_mode_button) {
		memdelete(rest_mode_button);
	}
	if (SpatialEditor::get_singleton()->is_tool_external()) {
		SpatialEditor::get_singleton()->set_tool_mode(SpatialEditor::TOOL_MODE_SELECT);
		SpatialEditor::get_singleton()->set_external_tool_mode(SpatialEditor::EX_TOOL_MODE_SELECT);
	}
}

void ModuleSkeletonEditor::_hide_handles() {
	if (!skeleton)
		return;

	pointsm->hide();
}

void ModuleSkeletonEditor::_draw_handles() {
	if (!skeleton || tool_mode == TOOL_MODE_BONE_NONE)
		return;

	while (am->get_surface_count()) {
		am->surface_remove(0);
	}

	pointsm->show();

	Array a;
	a.resize(Mesh::ARRAY_MAX);
	PoolVector<Vector3> va;
	PoolVector<Color> ca;
	{
		const int bone_len = skeleton->get_bone_count();
		va.resize(bone_len);
		ca.resize(bone_len);
		PoolVector<Vector3>::Write vaw = va.write();
		PoolVector<Color>::Write caw = ca.write();

		for (int i = 0; i < bone_len; i++) {
			Vector3 point = skeleton->get_bone_global_pose(i).origin;
			vaw[i] = point;
			Color c;
			if (i == skeleton->get_selected_bone()) {
				c = Color(1, 1, 0);
			} else {
				c = Color(0, 0, 1);
			}
			caw[i] = c;
		}
	}
	a[Mesh::ARRAY_VERTEX] = va;
	a[Mesh::ARRAY_COLOR] = ca;
	am->add_surface_from_arrays(Mesh::PRIMITIVE_POINTS, a);
	am->surface_set_material(0, handle_material);
}

bool ModuleSkeletonEditor::forward_spatial_gui_input(int p_index, Camera *p_camera, const Ref<InputEvent> &p_event) {
	if (!skeleton || tool_mode == TOOL_MODE_BONE_NONE)
		return false;

	SpatialEditor *se = SpatialEditor::get_singleton();
	SpatialEditorViewport *sev = se->get_editor_viewport(p_index);

	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid()) {
		Transform gt = skeleton->get_global_transform();
		Vector3 ray_from = p_camera->get_global_transform().origin;
		Vector2 gpoint = mb->get_position();
		real_t grab_threshold = 4 * EDSCALE;

		switch (mb->get_button_index()) {
			case BUTTON_LEFT: {
				if (mb->is_pressed()) {
					_edit.mouse_pos = mb->get_position();
					_edit.snap = se->is_snap_enabled();
					_edit.mode = SpatialEditorViewport::TRANSFORM_NONE;

					// check gizmo
					if (_gizmo_select(p_index, _edit.mouse_pos)) {
						return true;
					}

					// select bone
					int closest_idx = -1;
					real_t closest_dist = 1e10;
					const int bone_len = skeleton->get_bone_count();
					for (int i = 0; i < bone_len; i++) {
						Vector3 joint_pos_3d = gt.xform(skeleton->get_bone_global_pose(i).origin);
						Vector2 joint_pos_2d = p_camera->unproject_position(joint_pos_3d);
						real_t dist_3d = ray_from.distance_to(joint_pos_3d);
						real_t dist_2d = gpoint.distance_to(joint_pos_2d);
						if (dist_2d < grab_threshold && dist_3d < closest_dist) {
							closest_dist = dist_3d;
							closest_idx = i;
						}
					}
					if (closest_idx >= 0) {
						TreeItem *ti = joint_tree->get_item_with_text(skeleton->get_bone_name(closest_idx));
						ti->select(0);
						joint_tree->scroll_to_item(ti);
					} else {
						skeleton->set_selected_bone(-1);
						joint_tree->deselect_all();
					}

				} else {
					if (_edit.mode != SpatialEditorViewport::TRANSFORM_NONE) {
						if (skeleton && (skeleton->get_selected_bone() >= 0)) {
							UndoRedo *ur = EditorNode::get_singleton()->get_undo_redo();
							ur->create_action(TTR("Set Bone Transform"), UndoRedo::MERGE_ENDS);
							if (rest_mode) {
								ur->add_do_method(skeleton, "set_bone_rest", skeleton->get_selected_bone(), skeleton->get_bone_rest(skeleton->get_selected_bone()));
								ur->add_undo_method(skeleton, "set_bone_rest", skeleton->get_selected_bone(), original_local);
							} else {
								ur->add_do_method(skeleton, "set_bone_pose", skeleton->get_selected_bone(), skeleton->get_bone_pose(skeleton->get_selected_bone()));
								ur->add_undo_method(skeleton, "set_bone_pose", skeleton->get_selected_bone(), original_local);
							}
							ur->commit_action();
							_edit.mode = SpatialEditorViewport::TRANSFORM_NONE;
						}
					}
				}
				return true;
			} break;
			default:
				break;
		}
	}

	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid()) {
		_edit.mouse_pos = mm->get_position();
		if (!(mm->get_button_mask() & 1)) {
			_gizmo_select(p_index, _edit.mouse_pos, true);
		}
		if (mm->get_button_mask() & BUTTON_MASK_LEFT) {
			if (_edit.mode == SpatialEditorViewport::TRANSFORM_NONE)
				return true;

			Vector3 ray_pos = sev->get_ray_pos(mm->get_position());
			Vector3 ray = sev->get_ray(mm->get_position());
			float snap = EDITOR_GET("interface/inspector/default_float_step");

			switch (_edit.mode) {
				case SpatialEditorViewport::TRANSFORM_SCALE: {
					Vector3 motion_mask;
					Plane plane;
					//bool plane_mv = false;

					switch (_edit.plane) {
						case SpatialEditorViewport::TRANSFORM_VIEW:
							motion_mask = Vector3(0, 0, 0);
							plane = Plane(_edit.center, sev->get_camera_normal());
							break;
						case SpatialEditorViewport::TRANSFORM_X_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(0);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_Y_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(1);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_Z_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(2);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_YZ:
							motion_mask = se->get_gizmo_transform().basis.get_axis(2) + se->get_gizmo_transform().basis.get_axis(1);
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(0));
							//plane_mv = true;
							break;
						case SpatialEditorViewport::TRANSFORM_XZ:
							motion_mask = se->get_gizmo_transform().basis.get_axis(2) + se->get_gizmo_transform().basis.get_axis(0);
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(1));
							//plane_mv = true;
							break;
						case SpatialEditorViewport::TRANSFORM_XY:
							motion_mask = se->get_gizmo_transform().basis.get_axis(0) + se->get_gizmo_transform().basis.get_axis(1);
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(2));
							//plane_mv = true;
							break;
					}

					Vector3 intersection;
					if (!plane.intersects_ray(ray_pos, ray, &intersection))
						break;

					Vector3 click;
					if (!plane.intersects_ray(_edit.click_ray_pos, _edit.click_ray, &click))
						break;

					Vector3 motion = intersection - click;
					if (_edit.plane != SpatialEditorViewport::TRANSFORM_VIEW) {
						motion = motion_mask.dot(motion) * motion_mask;
					} else {
						float center_click_dist = click.distance_to(_edit.center);
						float center_inters_dist = intersection.distance_to(_edit.center);
						if (center_click_dist == 0)
							break;

						float scale = center_inters_dist - center_click_dist;
						motion = Vector3(scale, scale, scale);
					}

					bool local_coords = (se->are_local_coords_enabled() && _edit.plane != SpatialEditorViewport::TRANSFORM_VIEW);

					if (_edit.snap || se->is_snap_enabled()) {
						snap = se->get_scale_snap() / 100;
					}

					Transform t;

					if (local_coords) {
						Basis g = original_global.basis;
						motion = g.inverse().xform(motion);
						if (_edit.snap || se->is_snap_enabled()) {
							motion.snap(Vector3(snap, snap, snap));
						}
						Vector3 local_scale = original_local.basis.get_scale() * (motion + Vector3(1, 1, 1));
						// Prevent scaling to 0 it would break the gizmo
						Basis check = original_local.basis;
						check.scale(local_scale);
						if (check.determinant() != 0) {
							t = original_local;
							t.basis = t.basis.scaled_local(motion + Vector3(1, 1, 1));
						}
					} else {
						if (_edit.snap || se->is_snap_enabled()) {
							motion.snap(Vector3(snap, snap, snap));
						}
						t = original_local;
						Transform r;
						r.basis.scale(motion + Vector3(1, 1, 1));
						Basis base = original_to_local.get_basis().orthonormalized().inverse();
						t.basis = base * (r.get_basis() * (base.inverse() * original_local.get_basis()));
					}

					// Apply scale
					if (rest_mode) {
						skeleton->set_bone_rest(skeleton->get_selected_bone(), t);
					} else {
						skeleton->set_bone_pose(skeleton->get_selected_bone(), t);
					}

					sev->update_surface();

				} break;

				case SpatialEditorViewport::TRANSFORM_TRANSLATE: {
					Vector3 motion_mask;
					Plane plane;
					bool plane_mv = false;

					switch (_edit.plane) {
						case SpatialEditorViewport::TRANSFORM_VIEW:
							plane = Plane(_edit.center, sev->get_camera_normal());
							break;
						case SpatialEditorViewport::TRANSFORM_X_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(0);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_Y_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(1);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_Z_AXIS:
							motion_mask = se->get_gizmo_transform().basis.get_axis(2);
							plane = Plane(_edit.center, motion_mask.cross(motion_mask.cross(sev->get_camera_normal())).normalized());
							break;
						case SpatialEditorViewport::TRANSFORM_YZ:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(0));
							plane_mv = true;
							break;
						case SpatialEditorViewport::TRANSFORM_XZ:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(1));
							plane_mv = true;
							break;
						case SpatialEditorViewport::TRANSFORM_XY:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(2));
							plane_mv = true;
							break;
					}

					Vector3 intersection;
					if (!plane.intersects_ray(ray_pos, ray, &intersection))
						break;

					Vector3 click;
					if (!plane.intersects_ray(_edit.click_ray_pos, _edit.click_ray, &click))
						break;

					Vector3 motion = intersection - click;
					if (_edit.plane != SpatialEditorViewport::TRANSFORM_VIEW) {
						if (!plane_mv) {
							motion = motion_mask.dot(motion) * motion_mask;
						}
					}

					if (_edit.snap || se->is_snap_enabled()) {
						snap = se->get_translate_snap();
					}

					motion = original_to_local.basis.inverse().xform(motion);
					if (_edit.snap || se->is_snap_enabled()) {
						motion.snap(Vector3(snap, snap, snap));
					}

					Transform t;
					// Apply translation
					t = original_local;
					t.origin += motion;

					if (rest_mode) {
						skeleton->set_bone_rest(skeleton->get_selected_bone(), t);
					} else {
						skeleton->set_bone_pose(skeleton->get_selected_bone(), t);
					}

					sev->update_surface();

				} break;

				case SpatialEditorViewport::TRANSFORM_ROTATE: {
					Plane plane;
					Vector3 axis;

					switch (_edit.plane) {
						case SpatialEditorViewport::TRANSFORM_VIEW:
							plane = Plane(_edit.center, sev->get_camera_normal());
							break;
						case SpatialEditorViewport::TRANSFORM_X_AXIS:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(0));
							axis = Vector3(1, 0, 0);
							break;
						case SpatialEditorViewport::TRANSFORM_Y_AXIS:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(1));
							axis = Vector3(0, 1, 0);
							break;
						case SpatialEditorViewport::TRANSFORM_Z_AXIS:
							plane = Plane(_edit.center, se->get_gizmo_transform().basis.get_axis(2));
							axis = Vector3(0, 0, 1);
							break;
						case SpatialEditorViewport::TRANSFORM_YZ:
						case SpatialEditorViewport::TRANSFORM_XZ:
						case SpatialEditorViewport::TRANSFORM_XY:
							break;
					}

					Vector3 intersection;
					if (!plane.intersects_ray(ray_pos, ray, &intersection))
						break;

					Vector3 click;
					if (!plane.intersects_ray(_edit.click_ray_pos, _edit.click_ray, &click))
						break;

					Vector3 y_axis = (click - _edit.center).normalized();
					Vector3 x_axis = plane.normal.cross(y_axis).normalized();

					float angle = Math::atan2(x_axis.dot(intersection - _edit.center), y_axis.dot(intersection - _edit.center));

					if (_edit.snap || se->is_snap_enabled()) {
						snap = se->get_rotate_snap();
					}
					angle = Math::rad2deg(angle) + snap * 0.5; //else it won't reach +180
					angle -= Math::fmod(angle, snap);
					// set_message(vformat(TTR("Rotating %s degrees."), String::num(angle, snap_step_decimals)));
					angle = Math::deg2rad(angle);

					bool local_coords = (se->are_local_coords_enabled() && _edit.plane != SpatialEditorViewport::TRANSFORM_VIEW); // Disable local transformation for TRANSFORM_VIEW

					Transform t;

					if (local_coords) {
						Basis rot = Basis(axis, angle);
						t.basis = original_local.get_basis().orthonormalized() * rot;
						t.basis = t.basis.scaled_local(original_local.basis.get_scale());
						t.origin = original_local.origin;
					} else {
						Transform r;
						Basis base = original_to_local.get_basis().orthonormalized().inverse();
						r.basis.rotate(plane.normal, angle);
						t.basis = base * r.get_basis() * base.inverse() * original_local.get_basis();
						// t.basis = t.basis.scaled(original_local.basis.get_scale());
						t.origin = original_local.origin;
					}

					// Apply rotation
					if (rest_mode) {
						skeleton->set_bone_rest(skeleton->get_selected_bone(), t);
					} else {
						skeleton->set_bone_pose(skeleton->get_selected_bone(), t);
					}

					sev->update_surface();

				} break;
				default: {
				}
			}

			return true;
		}
	}

	return false;
}

void ModuleSkeletonEditor::add_bone() {
	_bone_add_dialog->popup_centered();
	_bone_add_line_edit->set_text("");
	_bone_add_line_edit->grab_focus();
}
void ModuleSkeletonEditor::_add_bone_callback() {
	skeleton->add_bone(_bone_add_line_edit->get_text());

	update_joint_tree();
}
void ModuleSkeletonEditor::remove_bone() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	_bone_remove_dialog->popup_centered();
}
void ModuleSkeletonEditor::_remove_bone_callback() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	skeleton->remove_bone(skeleton->get_selected_bone());

	update_joint_tree();
}
void ModuleSkeletonEditor::rename_bone() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	_bone_rename_line_edit->set_text(skeleton->get_bone_name(skeleton->get_selected_bone()));

	_bone_rename_dialog->popup_centered();
	_bone_rename_line_edit->grab_focus();
}

void ModuleSkeletonEditor::_rename_bone_callback() {
	if (skeleton->get_selected_bone() == -1) {
		return;
	}

	skeleton->set_bone_name(skeleton->get_selected_bone(), _bone_rename_line_edit->get_text());

	update_joint_tree();
}

void ModuleSkeletonEditor::create_bone_tool_popups() {
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

void ModuleSkeletonEditor::_bind_tool_popup_methods() {
	ClassDB::bind_method(D_METHOD("_add_bone_callback"), &ModuleSkeletonEditor::_add_bone_callback);
	ClassDB::bind_method(D_METHOD("_remove_bone_callback"), &ModuleSkeletonEditor::_remove_bone_callback);
	ClassDB::bind_method(D_METHOD("_rename_bone_callback"), &ModuleSkeletonEditor::_rename_bone_callback);
}

void ModuleEditorInspectorPluginSkeleton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_rest_mode_toggled"), &ModuleEditorInspectorPluginSkeleton::set_rest_mode_toggled);
}

bool ModuleEditorInspectorPluginSkeleton::can_handle(Object *p_object) {
	return Object::cast_to<Skeleton>(p_object) != nullptr;
}

void ModuleEditorInspectorPluginSkeleton::parse_begin(Object *p_object) {
	Skeleton *skeleton = Object::cast_to<Skeleton>(p_object);
	ERR_FAIL_COND(!skeleton);

	skel_editor = memnew(ModuleSkeletonEditor(this, editor, skeleton));
	add_custom_control(skel_editor);
}

void ModuleEditorInspectorPluginSkeleton::set_rest_mode_toggled(const bool p_pressed) {
	if (SpatialEditor::get_singleton()->get_selected()->get_class() == "Skeleton" && skel_editor) {
		skel_editor->set_rest_mode_toggled(p_pressed);
	}
}

ModuleSkeletonEditorPlugin::ModuleSkeletonEditorPlugin(EditorNode *p_node) {
	editor = p_node;

	skeleton_plugin = memnew(ModuleEditorInspectorPluginSkeleton);
	skeleton_plugin->editor = editor;

	EditorInspector::add_inspector_plugin(skeleton_plugin);
}

bool ModuleSkeletonEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("Skeleton");
}

void ModuleSkeletonEditor::_compute_edit(int p_index, const Point2 &p_point) {
	SpatialEditor *se = SpatialEditor::get_singleton();
	SpatialEditorViewport *sev = se->get_editor_viewport(p_index);

	_edit.click_ray = sev->get_ray(Vector2(p_point.x, p_point.y));
	_edit.click_ray_pos = sev->get_ray_pos(Vector2(p_point.x, p_point.y));
	_edit.plane = SpatialEditorViewport::TRANSFORM_VIEW;
	_update_spatial_transform_gizmo();
	_edit.center = se->get_gizmo_transform().origin;

	if (skeleton->get_selected_bone() != -1) {
		original_global = skeleton->get_global_transform() * skeleton->get_bone_global_pose(skeleton->get_selected_bone());
		if (rest_mode) {
			original_local = skeleton->get_bone_rest(skeleton->get_selected_bone());
		} else {
			original_local = skeleton->get_bone_pose(skeleton->get_selected_bone());
		}
		original_to_local = skeleton->get_global_transform();
		int parent_idx = skeleton->get_bone_parent(skeleton->get_selected_bone());
		if (parent_idx >= 0) {
			original_to_local = original_to_local * skeleton->get_bone_global_pose(parent_idx);
		}
		if (!rest_mode) {
			original_to_local = original_to_local * skeleton->get_bone_rest(skeleton->get_selected_bone()) * skeleton->get_bone_custom_pose(skeleton->get_selected_bone());
		}
	}
}

bool ModuleSkeletonEditor::_gizmo_select(int p_index, const Vector2 &p_screenpos, bool p_highlight_only) {
	SpatialEditor *se = SpatialEditor::get_singleton();
	SpatialEditorViewport *sev = se->get_editor_viewport(p_index);

	if (!se->is_gizmo_visible())
		return false;
	if (skeleton->get_selected_bone() == -1) {
		if (p_highlight_only)
			se->select_gizmo_highlight_axis(-1);
		return false;
	}

	Vector3 ray_pos = sev->get_ray_pos(Vector2(p_screenpos.x, p_screenpos.y));
	Vector3 ray = sev->get_ray(Vector2(p_screenpos.x, p_screenpos.y));

	Transform gt = se->get_gizmo_transform();
	float gs = sev->get_gizmo_scale();

	if (se->get_external_tool_mode() == SpatialEditor::EX_TOOL_MODE_SELECT || se->get_external_tool_mode() == SpatialEditor::EX_TOOL_MODE_MOVE) {
		int col_axis = -1;
		float col_d = 1e20;

		for (int i = 0; i < 3; i++) {
			Vector3 grabber_pos = gt.origin + gt.basis.get_axis(i) * gs * (GIZMO_ARROW_OFFSET + (GIZMO_ARROW_SIZE * 0.5));
			float grabber_radius = gs * GIZMO_ARROW_SIZE;

			Vector3 r;

			if (Geometry::segment_intersects_sphere(ray_pos, ray_pos + ray * MAX_Z, grabber_pos, grabber_radius, &r)) {
				float d = r.distance_to(ray_pos);
				if (d < col_d) {
					col_d = d;
					col_axis = i;
				}
			}
		}

		bool is_plane_translate = false;
		// plane select
		if (col_axis == -1) {
			col_d = 1e20;

			for (int i = 0; i < 3; i++) {
				Vector3 ivec2 = gt.basis.get_axis((i + 1) % 3).normalized();
				Vector3 ivec3 = gt.basis.get_axis((i + 2) % 3).normalized();

				Vector3 grabber_pos = gt.origin + (ivec2 + ivec3) * gs * (GIZMO_PLANE_SIZE + GIZMO_PLANE_DST);

				Vector3 r;
				Plane plane(gt.origin, gt.basis.get_axis(i).normalized());

				if (plane.intersects_ray(ray_pos, ray, &r)) {
					float dist = r.distance_to(grabber_pos);
					if (dist < (gs * GIZMO_PLANE_SIZE)) {
						float d = ray_pos.distance_to(r);
						if (d < col_d) {
							col_d = d;
							col_axis = i;

							is_plane_translate = true;
						}
					}
				}
			}
		}

		if (col_axis != -1) {
			if (p_highlight_only) {
				se->select_gizmo_highlight_axis(col_axis + (is_plane_translate ? 6 : 0));

			} else {
				//handle plane translate
				_edit.mode = SpatialEditorViewport::TRANSFORM_TRANSLATE;
				_compute_edit(p_index, Point2(p_screenpos.x, p_screenpos.y));
				_edit.plane = SpatialEditorViewport::TransformPlane(SpatialEditorViewport::TRANSFORM_X_AXIS + col_axis + (is_plane_translate ? 3 : 0));
			}
			return true;
		}
	}

	if (se->get_external_tool_mode() == SpatialEditor::EX_TOOL_MODE_SELECT || se->get_external_tool_mode() == SpatialEditor::EX_TOOL_MODE_ROTATE) {
		int col_axis = -1;
		float col_d = 1e20;

		for (int i = 0; i < 3; i++) {
			Plane plane(gt.origin, gt.basis.get_axis(i).normalized());
			Vector3 r;
			if (!plane.intersects_ray(ray_pos, ray, &r))
				continue;

			float dist = r.distance_to(gt.origin);

			if (dist > gs * (GIZMO_CIRCLE_SIZE - GIZMO_RING_HALF_WIDTH) && dist < gs * (GIZMO_CIRCLE_SIZE + GIZMO_RING_HALF_WIDTH)) {
				float d = ray_pos.distance_to(r);
				if (d < col_d) {
					col_d = d;
					col_axis = i;
				}
			}
		}

		if (col_axis != -1) {
			if (p_highlight_only) {
				se->select_gizmo_highlight_axis(col_axis + 3);
			} else {
				//handle rotate
				_edit.mode = SpatialEditorViewport::TRANSFORM_ROTATE;
				_compute_edit(p_index, Point2(p_screenpos.x, p_screenpos.y));
				_edit.plane = SpatialEditorViewport::TransformPlane(SpatialEditorViewport::TRANSFORM_X_AXIS + col_axis);
			}
			return true;
		}
	}

	if (se->get_external_tool_mode() == SpatialEditor::EX_TOOL_MODE_SCALE) {
		int col_axis = -1;
		float col_d = 1e20;

		for (int i = 0; i < 3; i++) {
			Vector3 grabber_pos = gt.origin + gt.basis.get_axis(i) * gs * GIZMO_SCALE_OFFSET;
			float grabber_radius = gs * GIZMO_ARROW_SIZE;

			Vector3 r;

			if (Geometry::segment_intersects_sphere(ray_pos, ray_pos + ray * MAX_Z, grabber_pos, grabber_radius, &r)) {
				float d = r.distance_to(ray_pos);
				if (d < col_d) {
					col_d = d;
					col_axis = i;
				}
			}
		}

		bool is_plane_scale = false;
		// plane select
		if (col_axis == -1) {
			col_d = 1e20;

			for (int i = 0; i < 3; i++) {
				Vector3 ivec2 = gt.basis.get_axis((i + 1) % 3).normalized();
				Vector3 ivec3 = gt.basis.get_axis((i + 2) % 3).normalized();

				Vector3 grabber_pos = gt.origin + (ivec2 + ivec3) * gs * (GIZMO_PLANE_SIZE + GIZMO_PLANE_DST);

				Vector3 r;
				Plane plane(gt.origin, gt.basis.get_axis(i).normalized());

				if (plane.intersects_ray(ray_pos, ray, &r)) {
					float dist = r.distance_to(grabber_pos);
					if (dist < (gs * GIZMO_PLANE_SIZE)) {
						float d = ray_pos.distance_to(r);
						if (d < col_d) {
							col_d = d;
							col_axis = i;

							is_plane_scale = true;
						}
					}
				}
			}
		}

		if (col_axis != -1) {
			if (p_highlight_only) {
				se->select_gizmo_highlight_axis(col_axis + (is_plane_scale ? 12 : 9));

			} else {
				//handle scale
				_edit.mode = SpatialEditorViewport::TRANSFORM_SCALE;
				_compute_edit(p_index, Point2(p_screenpos.x, p_screenpos.y));
				_edit.plane = SpatialEditorViewport::TransformPlane(SpatialEditorViewport::TRANSFORM_X_AXIS + col_axis + (is_plane_scale ? 3 : 0));
			}
			return true;
		}
	}

	if (p_highlight_only)
		se->select_gizmo_highlight_axis(-1);

	return false;
}
