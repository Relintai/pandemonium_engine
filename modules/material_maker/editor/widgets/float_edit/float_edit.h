#ifndef FLOAT_EDIT_H
#define FLOAT_EDIT_H


class FloatEdit : public LineEdit {
 GDCLASS(FloatEdit, LineEdit);

 public:

 float get_value() const;
 void set_value(const float val);

 float get_min_value() const;
 void set_min_value(const float val);

 float get_max_value() const;
 void set_max_value(const float val);

 float get_step() const;
 void set_step(const float val);

 bool get_float_only() const;
 void set_float_only(const bool val);

 bool get_sliding() const;
 void set_sliding(const bool val);

 float get_start_position() const;
 void set_start_position(const float val);

 float get_last_position() const;
 void set_last_position(const float val);

 float get_start_value() const;
 void set_start_value(const float val);

 int get_modifiers() const;
 void set_modifiers(const int val);

 bool get_from_lower_bound() const;
 void set_from_lower_bound(const bool val);

 bool get_from_upper_bound() const;
 void set_from_upper_bound(const bool val);

 void _ready();
 void set_value(const Variant &v);
 void set_min_value(const float v);
 void set_max_value(const float v);
 void set_step(const float v);
 void do_update(const bool update_text = true);
 void get_modifiers(const Variant &event);
 void _on_LineEdit_gui_input(const InputEvent &event);
 void _on_LineEdit_text_changed(const String &new_text);
 void _on_LineEdit_text_entered(const String &new_text, const Variant &release = true);
 void _on_FloatEdit_focus_entered();
 void _on_LineEdit_focus_exited();

 FloatEdit();
 ~FloatEdit();

 protected:
 static void _bind_methods();

 //tool
 //export 
 // setget set_value
 float value = 0.5;
 //export 
 // setget set_min_value
 float min_value = 0.0;
 //export 
 // setget set_max_value
 float max_value = 1.0;
 //export 
 // setget set_step
 float step = 0.0;
 //export 
 bool float_only = false;
 bool sliding = false;
 float start_position = ;
 float last_position = ;
 float start_value = ;
 int modifiers = ;
 bool from_lower_bound = false;
 bool from_upper_bound = false;
 onready var slider = $Slider;
 onready var cursor = $Slider/Cursor;
 signal value_changed(value);
};


#endif
