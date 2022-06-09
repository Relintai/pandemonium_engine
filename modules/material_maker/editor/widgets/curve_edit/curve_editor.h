#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H


class CurveEditor : public "res://addons/mat_maker_gd/widgets/curve_edit/curve_view.gd" {
 GDCLASS(CurveEditor, "res://addons/mat_maker_gd/widgets/curve_edit/curve_view.gd");

 public:

 void _ready();
 void set_curve(const Variant &c);
 void update_controls();
 void _on_ControlPoint_moved(const Variant &index);
 void _on_ControlPoint_removed(const Variant &index);
 void _on_CurveEditor_gui_input(const Variant &event);
 void _on_resize();

 CurveEditor();
 ~CurveEditor();

 protected:
 static void _bind_methods();

 //tool
 signal value_changed(value);
};


#endif
