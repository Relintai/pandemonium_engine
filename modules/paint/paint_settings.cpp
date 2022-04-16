/*
Copyright (c) 2019 Flairieve
Copyright (c) 2020-2022 cobrapitz
Copyright (c) 2022 Péter Magyar

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

#include "paint_settings.h"

void PaintTextInfo::_enter_tree() {
	/*
	canvas_outline = get_parent().find_node("CanvasOutline")
	editor = get_parent()
	*/
}
void PaintTextInfo::_on_ColorPickerButton_color_changed(const Color &color) {
	/*
	canvas_outline.color = color
	*/
}
void PaintTextInfo::_on_CheckButton_toggled(const bool button_pressed) {
	/*
	canvas_outline.visible = button_pressed
	*/
}
void PaintTextInfo::_on_Ok_pressed() {
	/*
	hide()
	*/
}

PaintTextInfo::PaintTextInfo() {
	/*

[gd_scene load_steps=2 format=2]

[ext_resource path="res://addons/Godoxel/Settings.gd" type="Script" id=1]


[node name="Settings" type="WindowDialog"]
visible = true
margin_top = 20.0
margin_right = 300.0
margin_bottom = 120.0
window_title = "Settings"
script = ExtResource( 1 )

[node name="Ok" type="Button" parent="."]
margin_left = 210.0
margin_top = 70.0
margin_right = 290.0
margin_bottom = 90.0
text = "Ok"

[node name="CanvasOutlineToggle" type="Control" parent="."]
margin_left = 10.0
margin_top = 10.0
margin_right = 290.0
margin_bottom = 30.0
__meta__ = {
"_edit_group_": true
}

[node name="Label" type="Label" parent="CanvasOutlineToggle"]
margin_right = 130.0
margin_bottom = 20.0
text = "Canvas Outline:"
valign = 1

[node name="CheckButton" type="CheckButton" parent="CanvasOutlineToggle"]
margin_left = 210.0
margin_top = -10.0
margin_right = 286.0
margin_bottom = 30.0
pressed = true

[node name="CanvasOutlineColor" type="Control" parent="."]
margin_left = 10.0
margin_top = 40.0
margin_right = 290.0
margin_bottom = 60.0
__meta__ = {
"_edit_group_": true
}

[node name="Label" type="Label" parent="CanvasOutlineColor"]
margin_right = 130.0
margin_bottom = 20.0
text = "Canvas Outline Color:"
valign = 1

[node name="ColorPickerButton" type="ColorPickerButton" parent="CanvasOutlineColor"]
margin_left = 170.0
margin_right = 280.0
margin_bottom = 20.0
[connection signal="pressed" from="Ok" to="." method="_on_Ok_pressed"]
[connection signal="toggled" from="CanvasOutlineToggle/CheckButton" to="." method="_on_CheckButton_toggled"]
[connection signal="color_changed" from="CanvasOutlineColor/ColorPickerButton" to="." method="_on_ColorPickerButton_color_changed"]


	*/
}

PaintTextInfo::~PaintTextInfo() {
}

void PaintTextInfo::_bind_methods() {
}
