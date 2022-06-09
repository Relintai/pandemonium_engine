#ifndef POLYGON_EDITOR_H
#define POLYGON_EDITOR_H


class PolygonEditor : public "res://addons/mat_maker_gd/widgets/polygon_edit/polygon_view.gd" {
 GDCLASS(PolygonEditor, "res://addons/mat_maker_gd/widgets/polygon_edit/polygon_view.gd");

 public:

 void _ready();
 void set_polygon(const Variant &p);
 void update_controls();
 void _on_ControlPoint_moved(const Variant &index);
 void _on_ControlPoint_removed(const Variant &index);
 void _on_PolygonEditor_gui_input(const Variant &event);
 void _on_resize();

 PolygonEditor();
 ~PolygonEditor();

 protected:
 static void _bind_methods();

 //tool
 signal value_changed(value);
};


#endif
