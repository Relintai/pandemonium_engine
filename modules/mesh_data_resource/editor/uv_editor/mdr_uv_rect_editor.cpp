

#include "mdr_uv_rect_editor.h"

#include "../../mesh_data_resource.h"
#include "../../nodes/mesh_data_instance.h"
#include "../mdi_ed_plugin.h"
#include "editor/editor_plugin.h"
#include "editor/editor_zoom_widget.h"
#include "mdr_uv_rect_view.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/spin_box.h"

void MDRUVRectEditor::set_plugin(MDIEdPlugin *plugin) {
	uv_rect_view->set_plugin(plugin);
}

void MDRUVRectEditor::set_mesh_data_resource(const Ref<MeshDataResource> &a) {
	uv_rect_view->set_mesh_data_resource(a);
}
void MDRUVRectEditor::set_mesh_data_instance(MeshDataInstance *a) {
	uv_rect_view->set_mesh_data_instance(a);
}
void MDRUVRectEditor::ok_pressed() {
	uv_rect_view->ok_pressed();
}
void MDRUVRectEditor::cancel_pressed() {
	uv_rect_view->cancel_pressed();
}

MDRUVRectEditor::MDRUVRectEditor() {
	// -- Main ScrollContainer
	ScrollContainer *main_scroll_container = memnew(ScrollContainer);
	add_child(main_scroll_container);

	MarginContainer *main_margin_container = memnew(MarginContainer);
	main_margin_container->set_size(Size2(700, 700));
	main_scroll_container->add_child(main_margin_container);

	uv_rect_view = memnew(MDRUVRectView);
	main_margin_container->set_custom_minimum_size(Size2(600, 600));
	main_margin_container->add_child(uv_rect_view);

	// -- Main Buttons
	VBoxContainer *main_control_container = memnew(VBoxContainer);
	main_control_container->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	main_control_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_control_container->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(main_control_container);

	HBoxContainer *control_container = memnew(HBoxContainer);
	control_container->set_mouse_filter(Control::MOUSE_FILTER_IGNORE);
	control_container->set_h_size_flags(SIZE_EXPAND_FILL);
	main_control_container->add_child(control_container);

	EditorZoomWidget *ezw = memnew(EditorZoomWidget);
	ezw->connect("zoom_changed", uv_rect_view, "on_zoom_changed");
	control_container->add_child(ezw);

	control_container->add_child(memnew(VSeparator));

	horizontal_mirror_button = memnew(Button);
	horizontal_mirror_button->set_tooltip("Mirror the selected island horizontally.");
	horizontal_mirror_button->connect("pressed", uv_rect_view, "on_mirror_horizontal_button_pressed");
	control_container->add_child(horizontal_mirror_button);

	vertical_mirror_button = memnew(Button);
	vertical_mirror_button->set_tooltip("Mirror the selected island vertically.");
	vertical_mirror_button->connect("pressed", uv_rect_view, "on_mirror_vertical_button_pressed");
	control_container->add_child(vertical_mirror_button);

	control_container->add_child(memnew(VSeparator));

	rotate_left_button = memnew(Button);
	rotate_left_button->set_tooltip("Rotate left.");
	rotate_left_button->connect("pressed", uv_rect_view, "on_rotate_left_button_button_pressed");
	control_container->add_child(rotate_left_button);

	SpinBox *sb = memnew(SpinBox);
	sb->set_tooltip("Rotate amount in degrees.");
	sb->set_value(45);
	sb->set_max(360);
	sb->set_allow_lesser(true);
	sb->set_allow_greater(true);
	sb->connect("value_changed", uv_rect_view, "on_rotate_amount_spinbox_changed");
	control_container->add_child(sb);

	rotate_right_button = memnew(Button);
	rotate_right_button->set_tooltip("Rotate right.");
	rotate_right_button->connect("pressed", uv_rect_view, "on_rotate_right_button_button_pressed");
	control_container->add_child(rotate_right_button);
}

MDRUVRectEditor::~MDRUVRectEditor() {
}

void MDRUVRectEditor::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED: {
			horizontal_mirror_button->set_icon(get_theme_icon("MirrorX", "EditorIcons"));
			vertical_mirror_button->set_icon(get_theme_icon("MirrorY", "EditorIcons"));
			rotate_left_button->set_icon(get_theme_icon("RotateLeft", "EditorIcons"));
			rotate_right_button->set_icon(get_theme_icon("RotateRight", "EditorIcons"));
		} break;
	}
}

void MDRUVRectEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("ok_pressed"), &MDRUVRectEditor::ok_pressed);
	ClassDB::bind_method(D_METHOD("cancel_pressed"), &MDRUVRectEditor::cancel_pressed);
}
