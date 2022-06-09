
#include "color_picker_button.h"



 //tool;

 void ColorPickerButton::get_drag_data(const Variant &_position) {
   Variant  = ColorRect.new();
  preview.color = color;
  preview.rect_min_size = Vector2(32, 32);
  set_drag_preview(preview);
  return color;
}


 bool ColorPickerButton::can_drop_data(const Variant &_position, const Variant & data) {
  return typeof(data) == TYPE_COLOR;
}


 void ColorPickerButton::drop_data(const Variant &_position, const Variant & data) {
  color = data;
  emit_signal("color_changed", color);
}

}

 ColorPickerButton::ColorPickerButton() {
 }

 ColorPickerButton::~ColorPickerButton() {
 }


 static void ColorPickerButton::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_drag_data", "_position"), &ColorPickerButton::get_drag_data);
  ClassDB::bind_method(D_METHOD("can_drop_data", "_position", " data"), &ColorPickerButton::can_drop_data);
  ClassDB::bind_method(D_METHOD("drop_data", "_position", " data"), &ColorPickerButton::drop_data);

 }



