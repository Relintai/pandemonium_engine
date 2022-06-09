#ifndef POLYGON_EDIT_H
#define POLYGON_EDIT_H


class PolygonEdit : public Control {
 GDCLASS(PolygonEdit, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 bool get_closed() const;
 void set_closed(const bool val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 void set_closed(const bool c = true);
 void set_value(const Variant &v);
 void _on_PolygonEdit_pressed();
 void on_value_changed(const Variant &v);

 PolygonEdit();
 ~PolygonEdit();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
 //export 
 // setget set_closed
 bool closed = true;
 // setget set_value
 Variant  = null;
 signal updated(polygon);
};


#endif
