
#include "polygon_editor.h"



 //tool;
 signal value_changed(value);

 void PolygonEditor::_ready() {
  update_controls();
}


 void PolygonEditor::set_polygon(const Variant &p) {
  polygon = p;
  update();
  update_controls();
}


 void PolygonEditor::update_controls() {

  for (c in get_children()) {
   c.queue_free();
}


  if (!polygon) {
   return;
}


  for (i in polygon.points.size()) {
    Variant  = polygon.points[i];
   //var control_point = preload("res://addons/mat_maker_gd/widgets/polygon_edit/control_point.tscn").instance();
   add_child(control_point);
   control_point.initialize(p);
   control_point.rect_position = transform_point(p)-control_point.OFFSET;
   control_point.connect("moved", self, "_on_ControlPoint_moved");
   control_point.connect("removed", self, "_on_ControlPoint_removed");
}

  emit_signal("value_changed", polygon);
}


 void PolygonEditor::_on_ControlPoint_moved(const Variant &index) {
   Variant  = get_child(index);
  polygon.points[index] = reverse_transform_point(control_point.rect_position+control_point.OFFSET);
  update();
  emit_signal("value_changed", polygon);
}


 void PolygonEditor::_on_ControlPoint_removed(const Variant &index) {

  if (polygon.remove_point(index)) {
   update();
   update_controls();
}

}


 void PolygonEditor::_on_PolygonEditor_gui_input(const Variant &event) {

  if (!polygon) {
   return;
}


  if (event is InputEventMouseButton) {

   if (event.button_index == BUTTON_LEFT && event.doubleclick) {
     Variant  = reverse_transform_point(get_local_mouse_position());
    polygon.add_point(new_point_position.x, new_point_position.y, closed);
    update_controls();
}

}

}


 void PolygonEditor::_on_resize() {
  ._on_resize();
  update_controls();
}

}

 PolygonEditor::PolygonEditor() {
 }

 PolygonEditor::~PolygonEditor() {
 }


 static void PolygonEditor::_bind_methods() {
  ClassDB::bind_method(D_METHOD("_ready"), &PolygonEditor::_ready);
  ClassDB::bind_method(D_METHOD("set_polygon", "p"), &PolygonEditor::set_polygon);
  ClassDB::bind_method(D_METHOD("update_controls"), &PolygonEditor::update_controls);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_moved", "index"), &PolygonEditor::_on_ControlPoint_moved);
  ClassDB::bind_method(D_METHOD("_on_ControlPoint_removed", "index"), &PolygonEditor::_on_ControlPoint_removed);
  ClassDB::bind_method(D_METHOD("_on_PolygonEditor_gui_input", "event"), &PolygonEditor::_on_PolygonEditor_gui_input);
  ClassDB::bind_method(D_METHOD("_on_resize"), &PolygonEditor::_on_resize);

 }



