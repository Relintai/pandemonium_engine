#ifndef CURVE_EDIT_H
#define CURVE_EDIT_H


class CurveEdit : public Control {
 GDCLASS(CurveEdit, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void set_value(const Variant &v);
 void _on_CurveEdit_pressed();
 void on_value_changed(const Variant &v);

 CurveEdit();
 ~CurveEdit();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 // setget set_value
 Variant  = null;
 signal updated(curve);
};


#endif
