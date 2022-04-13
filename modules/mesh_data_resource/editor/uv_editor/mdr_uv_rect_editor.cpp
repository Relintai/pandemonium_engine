/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "mdr_uv_rect_editor.h"

#include "editor/editor_plugin.h"
#include "../../mesh_data_resource.h"
#include "../../nodes/mesh_data_instance.h"
#include "../mdi_ed_plugin.h"

void MDRUVRectEditor::set_plugin(MDIEdPlugin *plugin) {
	//$ScrollContainer/MarginContainer/RectView.set_plugin(plugin)
}

void MDRUVRectEditor::set_mesh_data_resource(Ref<MeshDataResource> a) {
	//$ScrollContainer/MarginContainer/RectView.set_mesh_data_resource(a)
}
void MDRUVRectEditor::set_mesh_data_instance(MeshDataInstance *a) {
	//$ScrollContainer/MarginContainer/RectView.set_mesh_data_instance(a)
}
void MDRUVRectEditor::ok_pressed() {
	//$ScrollContainer/MarginContainer/RectView.ok_pressed()
}
void MDRUVRectEditor::cancel_pressed() {
	//$ScrollContainer/MarginContainer/RectView.cancel_pressed()
}

MDRUVRectEditor::MDRUVRectEditor() {
	/*

[gd_scene load_steps=8 format=2]

[ext_resource path="res://addons/mesh_data_resource_editor/uv_editor/RectEditor.gd" type="Script" id=1]
[ext_resource path="res://addons/mesh_data_resource_editor/widgets/EditorZoomWidget.tscn" type="PackedScene" id=2]
[ext_resource path="res://addons/mesh_data_resource_editor/uv_editor/RectView.gd" type="Script" id=3]
[ext_resource path="res://addons/mesh_data_resource_editor/icons/icon_v_mirror.png" type="Texture" id=4]
[ext_resource path="res://addons/mesh_data_resource_editor/icons/icon_h_mirror.png" type="Texture" id=5]
[ext_resource path="res://addons/mesh_data_resource_editor/icons/icon_rot_right.png" type="Texture" id=6]
[ext_resource path="res://addons/mesh_data_resource_editor/icons/icon_rot_left.png" type="Texture" id=7]

[node name="UVEditor" type="PanelContainer"]
anchor_right = 1.0
anchor_bottom = 1.0
script = ExtResource( 1 )
__meta__ = {
"_edit_use_anchors_": false
}

[node name="ScrollContainer" type="ScrollContainer" parent="."]
margin_left = 7.0
margin_top = 7.0
margin_right = 1017.0
margin_bottom = 593.0

[node name="MarginContainer" type="MarginContainer" parent="ScrollContainer"]
margin_right = 700.0
margin_bottom = 700.0
custom_constants/margin_right = 50
custom_constants/margin_top = 50
custom_constants/margin_left = 50
custom_constants/margin_bottom = 50

[node name="RectView" type="Control" parent="ScrollContainer/MarginContainer"]
margin_left = 50.0
margin_top = 50.0
margin_right = 650.0
margin_bottom = 650.0
rect_min_size = Vector2( 600, 600 )
script = ExtResource( 3 )
zoom_widget_path = NodePath("../../../Control/HBoxContainer/EditorZoomWidget")
mirror_horizontal_button_path = NodePath("../../../Control/HBoxContainer/HorizontalMirror")
mirror_vertical_button_path = NodePath("../../../Control/HBoxContainer/VerticalMirror")
rotate_left_button_path = NodePath("../../../Control/HBoxContainer/RotLeft")
rotate_amount_spinbox_path = NodePath("../../../Control/HBoxContainer/SpinBox")
rotate_right_button_path = NodePath("../../../Control/HBoxContainer/RotRight")

[node name="Control" type="VBoxContainer" parent="."]
margin_left = 7.0
margin_top = 7.0
margin_right = 1017.0
margin_bottom = 593.0
mouse_filter = 2
size_flags_horizontal = 3
size_flags_vertical = 3

[node name="HBoxContainer" type="HBoxContainer" parent="Control"]
margin_right = 1010.0
margin_bottom = 24.0
mouse_filter = 2
size_flags_horizontal = 3
__meta__ = {
"_edit_use_anchors_": false
}

[node name="EditorZoomWidget" parent="Control/HBoxContainer" instance=ExtResource( 2 )]
anchor_right = 0.0
anchor_bottom = 0.0
margin_right = 115.0
margin_bottom = 24.0
custom_constants/separation = -8
__meta__ = {
"_edit_use_anchors_": false
}

[node name="VSeparator2" type="VSeparator" parent="Control/HBoxContainer"]
margin_left = 119.0
margin_right = 123.0
margin_bottom = 24.0

[node name="HorizontalMirror" type="Button" parent="Control/HBoxContainer"]
margin_left = 127.0
margin_right = 155.0
margin_bottom = 24.0
hint_tooltip = "Mirror the selected island horizontally.."
icon = ExtResource( 5 )

[node name="VerticalMirror" type="Button" parent="Control/HBoxContainer"]
margin_left = 159.0
margin_right = 187.0
margin_bottom = 24.0
hint_tooltip = "Mirror the selected island vertically."
icon = ExtResource( 4 )

[node name="VSeparator" type="VSeparator" parent="Control/HBoxContainer"]
margin_left = 191.0
margin_right = 195.0
margin_bottom = 24.0

[node name="RotLeft" type="Button" parent="Control/HBoxContainer"]
margin_left = 199.0
margin_right = 227.0
margin_bottom = 24.0
hint_tooltip = "Rotate left."
icon = ExtResource( 7 )

[node name="SpinBox" type="SpinBox" parent="Control/HBoxContainer"]
margin_left = 231.0
margin_right = 305.0
margin_bottom = 24.0
hint_tooltip = "Rotate amount in degrees."
max_value = 360.0
value = 45.0
allow_greater = true
allow_lesser = true

[node name="RotRight" type="Button" parent="Control/HBoxContainer"]
margin_left = 309.0
margin_right = 337.0
margin_bottom = 24.0
hint_tooltip = "Rotate right."
icon = ExtResource( 6 )


	*/
}

MDRUVRectEditor::~MDRUVRectEditor() {
}

void MDRUVRectEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("ok_pressed"), &MDRUVRectEditor::ok_pressed);
	ClassDB::bind_method(D_METHOD("cancel_pressed"), &MDRUVRectEditor::cancel_pressed);
}
