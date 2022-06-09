#ifndef POLYGON_VIEW_H
#define POLYGON_VIEW_H


class PolygonView : public Control {
 GDCLASS(PolygonView, Control);

 public:

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Variant get_Variant();
 void set_Variant(const Variant &val);

 Vector2 get_draw_size();
 void set_draw_size(const Vector2 &val);

 Vector2 get_draw_offset();
 void set_draw_offset(const Vector2 &val);

 bool get_closed() const;
 void set_closed(const bool val);

 void set_closed(const bool c = true);
 void _ready();
 Vector2 transform_point(const Vector2 &p);
 Vector2 reverse_transform_point(const Vector2 &p);
 void set_polygon(const Variant &val);
 void _draw();
 void _on_resize();

 PolygonView();
 ~PolygonView();

 protected:
 static void _bind_methods();

 //tool
 Variant  = preload("res://addons/mat_maker_gd/nodes/bases/polygon_base.gd");
 //: MMPolygon
 Variant ;
 Vector2 draw_size = Vector2(1, 1);
 Vector2 draw_offset = Vector2(0, 0);
 bool closed = true;
};


#endif
