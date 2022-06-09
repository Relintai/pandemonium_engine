#ifndef CURVE_VIEW_H
#define CURVE_VIEW_H


class CurveView : public Control {
 GDCLASS(CurveView, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 bool get_show_axes() const;
 void set_show_axes(const bool val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _ready();
 void set_curve(const Variant &val);
 Vector2 transform_point(const Vector2 &p);
 Vector2 reverse_transform_point(const Vector2 &p);
 void _draw();
 void _on_resize();

 CurveView();
 ~CurveView();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 //export 
 bool show_axes = false;
 //: MMCurve
 Variant ;
};


#endif
