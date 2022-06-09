
#include "curve_view.h"


Variant CurveView::get_Variant() {
 return Variant;
}

void CurveView::set_Variant(const Variant &val) {
Variant = val;
}


bool CurveView::get_show_axes() const {
 return show_axes;
}

void CurveView::set_show_axes(const bool val) {
show_axes = val;
}


Variant CurveView::get_Variant() {
 return Variant;
}

void CurveView::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 //export ;
  bool show_axes = false;
 //: MMCurve;
  Variant ;

 void CurveView::_ready() {
  //	curve = MMCurve.new();
  connect("resized", self, "_on_resize");
  update();
}


 void CurveView::set_curve(const Variant &val) {
  curve = val;
  update();
}


 Vector2 CurveView::transform_point(const Vector2 &p) {
  return (Vector2(0.0, 1.0)+Vector2(1.0, -1.0)*p)*rect_size;
}


 Vector2 CurveView::reverse_transform_point(const Vector2 &p) {
  return Vector2(0.0, 1.0)+Vector2(1.0, -1.0)*p/rect_size;
}


 void CurveView::_draw() {

  if (!curve) {
   return;
}

  //	var current_theme : Theme = get_node("/root/MainWindow").theme;
  //;
  //	var bg = current_theme.get_stylebox("panel", "Panel").bg_color;
  //	var fg = current_theme.get_color("font_color", "Label");
  //;
  //	var axes_color : Color = bg.linear_interpolate(fg, 0.25);
  //	var curve_color : Color = bg.linear_interpolate(fg, 0.75);
   Color axes_color = Color(0.9, 0.9, 0.9, 1);
   Color curve_color = Color(1, 1, 1, 1);

  if (show_axes) {

   for (int i = 0; i < 5; ++i) { //i in range(5)
     Variant  = transform_point(0.25*Vector2(i, i));
    draw_line(Vector2(p.x, 0), Vector2(p.x, rect_size.y-1), axes_color);
    draw_line(Vector2(0, p.y), Vector2(rect_size.x-1, p.y), axes_color);
}

}

   Variant  = curve.get_points();

  for (int i = 0; i < points.size() - 1; ++i) { //i in range(points.size() - 1)
    Variant  = points[i].p;
    Variant  = points[i+1].p;
    Variant  = (p2.x-p1.x)/3.0;
    Variant  = p1.y+d*points[i].rs;
    Variant  = p2.y-d*points[i+1].ls;
    Variant  = transform_point(p1);
    int count = max(1, int((transform_point(p2).x-p.x/5.0)));

   for (int tt = 0; tt < count; ++tt) { //tt in range(count)
     Variant  = (tt+1.0)/count;
     Variant  = (1.0 - t);
     Variant  = omt * omt;
     Variant  = omt2 * omt;
     Variant  = t * t;
     Variant  = t2 * t;
     Variant  = p1.x+(p2.x-p1.x)*t;
     Variant  = transform_point(Vector2(x, p1.y*omt3 + yac*omt2*t*3.0 + ybc*omt*t2*3.0 + p2.y*t3));
    draw_line(p, np, curve_color);
    p = np;
}

}

}


 void CurveView::_on_resize() {
  update();
}

}

 CurveView::CurveView() {
 //var MMCurve = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
  show_axes = false;
  ;
 }

 CurveView::~CurveView() {
 }


 static void CurveView::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveView::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveView::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


   ClassDB::bind_method(D_METHOD("get_show_axes"), &CurveView::get_show_axes);
   ClassDB::bind_method(D_METHOD("set_show_axes", "value"), &CurveView::set_show_axes);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_axes"), "set_show_axes", "get_show_axes");


   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveView::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveView::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("_ready"), &CurveView::_ready);
  ClassDB::bind_method(D_METHOD("set_curve", "val"), &CurveView::set_curve);
  ClassDB::bind_method(D_METHOD("transform_point", "p"), &CurveView::transform_point);
  ClassDB::bind_method(D_METHOD("reverse_transform_point", "p"), &CurveView::reverse_transform_point);
  ClassDB::bind_method(D_METHOD("_draw"), &CurveView::_draw);
  ClassDB::bind_method(D_METHOD("_on_resize"), &CurveView::_on_resize);

 }



