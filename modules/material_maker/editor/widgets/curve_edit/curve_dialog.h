#ifndef CURVE_DIALOG_H
#define CURVE_DIALOG_H


class CurveDialog : public WindowDialog {
 GDCLASS(CurveDialog, WindowDialog);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Array get_previous_points();
 void set_previous_points(const Array &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void _ready();
 void _on_CurveDialog_popup_hide();
 void _on_OK_pressed();
 void _on_Cancel_pressed();
 void edit_curve(const Variant &c);
 void _on_CurveEditor_value_changed(const Variant &value);

 CurveDialog();
 ~CurveDialog();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/curve_base.gd");
 Array previous_points = ;
 Variant ;
 signal curve_changed(curve);
 // Called when the node enters the scene tree for the first time.
};


#endif
