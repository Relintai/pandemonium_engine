
#include "polygon_view.h"


Variant PolygonView::get_Variant() {
 return Variant;
}

void PolygonView::set_Variant(const Variant &val) {
Variant = val;
}


Variant PolygonView::get_Variant() {
 return Variant;
}

void PolygonView::set_Variant(const Variant &val) {
Variant = val;
}


Vector2 PolygonView::get_draw_size() {
 return draw_size;
}

void PolygonView::set_draw_size(const Vector2 &val) {
draw_size = val;
}


Vector2 PolygonView::get_draw_offset() {
 return draw_offset;
}

void PolygonView::set_draw_offset(const Vector2 &val) {
draw_offset = val;
}


bool PolygonView::get_closed() const {
 return closed;
}

void PolygonView::set_closed(const bool val) {
closed = val;
}



 //tool;
 //var MMPolygon = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
 //: MMPolygon;
  Variant ;
  Vector2 draw_size = Vector2(1, 1);
  Vector2 draw_offset = Vector2(0, 0);
  bool closed = true;

 void PolygonView::set_closed(const bool c) {
  closed = c;
  update();
}


 void PolygonView::_ready() {
  //	polygon = MMPolygon.new();
  connect("resized", self, "_on_resize");
  _on_resize();
}


 Vector2 PolygonView::transform_point(const Vector2 &p) {
  return draw_offset+p*draw_size;
}


 Vector2 PolygonView::reverse_transform_point(const Vector2 &p) {
  return (p-draw_offset)/draw_size;
}


 void PolygonView::set_polygon(const Variant &val) {
  polygon = val;
  update();
}


 void PolygonView::_draw() {

  if (!polygon) {
   return;
}

  //	var current_theme : Theme = get_node("/root/MainWindow").theme;
  //	var bg = current_theme.get_stylebox("panel", "Panel").bg_color;
  //	var fg = current_theme.get_color("font_color", "Label");
  //	var axes_color : Color = bg.linear_interpolate(fg, 0.25);
  //	var curve_color : Color = bg.linear_interpolate(fg, 0.75);
   Color axes_color = Color(0.9, 0.9, 0.9, 1);
   Color curve_color = Color(1, 1, 1, 1);
  draw_rect(Rect2(draw_offset, draw_size), axes_color, false);
   Vector2 tp = transform_point(polygon.points[polygon.points.size()-1 if closed else 0]);

  for (p in polygon.points) {
    Variant  = transform_point(p);
   draw_line(tp, tnp, curve_color);
   tp = tnp;
}

}


 void PolygonView::_on_resize() {
   float ds = min(rect_size.x, rect_size.y);
  draw_size = Vector2(ds, ds);
  draw_offset = 0.5*(rect_size-draw_size);
  update();
}

}

 PolygonView::PolygonView() {
 //var MMPolygon = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
  ;
  draw_size = Vector2(1, 1);
  draw_offset = Vector2(0, 0);
  closed = true;
 }

 PolygonView::~PolygonView() {
 }


 static void PolygonView::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &PolygonView::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &PolygonView::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_Variant"), &PolygonView::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &PolygonView::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_draw_size"), &PolygonView::get_draw_size);
   ClassDB::bind_method(D_METHOD("set_draw_size", "value"), &PolygonView::set_draw_size);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "draw_size"), "set_draw_size", "get_draw_size");


   ClassDB::bind_method(D_METHOD("get_draw_offset"), &PolygonView::get_draw_offset);
   ClassDB::bind_method(D_METHOD("set_draw_offset", "value"), &PolygonView::set_draw_offset);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "draw_offset"), "set_draw_offset", "get_draw_offset");


   ClassDB::bind_method(D_METHOD("get_closed"), &PolygonView::get_closed);
   ClassDB::bind_method(D_METHOD("set_closed", "value"), &PolygonView::set_closed);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "closed"), "set_closed", "get_closed");


  ClassDB::bind_method(D_METHOD("set_closed", "c"), &PolygonView::set_closed, true);
  ClassDB::bind_method(D_METHOD("_ready"), &PolygonView::_ready);
  ClassDB::bind_method(D_METHOD("transform_point", "p"), &PolygonView::transform_point);
  ClassDB::bind_method(D_METHOD("reverse_transform_point", "p"), &PolygonView::reverse_transform_point);
  ClassDB::bind_method(D_METHOD("set_polygon", "val"), &PolygonView::set_polygon);
  ClassDB::bind_method(D_METHOD("_draw"), &PolygonView::_draw);
  ClassDB::bind_method(D_METHOD("_on_resize"), &PolygonView::_on_resize);

 }



