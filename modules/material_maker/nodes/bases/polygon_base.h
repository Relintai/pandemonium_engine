#ifndef POLYGON_BASE_H
#define POLYGON_BASE_H


class PolygonBase : public MMNode {
 GDCLASS(PolygonBase, MMNode);

 public:

 PoolVector2Array get_points();
 void set_points(const PoolVector2Array &val);

 void clear();
 void add_point(const float x, const float y, const bool closed = true);
 bool remove_point(const int index);
 int get_point_count();
 Vector2 get_point(const int i);
 void set_point(const int i, const Vector2 &v);
 void set_points(const PoolVector2Array &v);
 void polygon_changed();
 void _polygon_changed();
 String to_string();

 PolygonBase();
 ~PolygonBase();

 protected:
 static void _bind_methods();

 //tool
 //export(PoolVector2Array) 
 PoolVector2Array points = [Vector2(0.2, 0.2), Vector2(0.7, 0.4), Vector2(0.4, 0.7)];
};


#endif
