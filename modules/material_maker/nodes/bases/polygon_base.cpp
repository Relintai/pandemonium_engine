
#include "polygon_base.h"


PoolVector2Array PolygonBase::get_points() {
 return points;
}

void PolygonBase::set_points(const PoolVector2Array &val) {
points = val;
}



 //tool;
 //export(PoolVector2Array) ;
  PoolVector2Array points = [Vector2(0.2, 0.2), Vector2(0.7, 0.4), Vector2(0.4, 0.7)];

 void PolygonBase::clear() {
  points.resize(0);
  _polygon_changed();
}


 void PolygonBase::add_point(const float x, const float y, const bool closed) {
   Vector2 p = Vector2(x, y);
   Variant  = points.size();

  if (points_count < 3) {
   points.append(p);
   _polygon_changed();
   return;
}

   float min_length = (p-Geometry.get_closest_point_to_segment_2d(p, points[0], points[points_count-1])).length();
   Variant  = 0;

  for (i in points_count-1) {
    Variant  = (p - Geometry.get_closest_point_to_segment_2d(p, points[i], points[i+1])).length();

   if (length < min_length) {
    min_length = length;
    insert_point = i+1;
}

}


  if (!closed && insert_point == 0 && (points[0]-p).length() > (points[points_count-1]-p).length()) {
   insert_point = points_count;
}

  points.insert(insert_point, p);
  _polygon_changed();
}


 bool PolygonBase::remove_point(const int index) {
   Variant  = points.size();

  if (s < 4 || index < 0 || index >= s) {
   return false;
}


  else {
   points.remove(index);
   _polygon_changed();
}

  return true;
}


 int PolygonBase::get_point_count() {
  return points.size();
}


 Vector2 PolygonBase::get_point(const int i) {
  return points[i];
}


 void PolygonBase::set_point(const int i, const Vector2 &v) {
  points[i] = v;
  _polygon_changed();
}


 void PolygonBase::set_points(const PoolVector2Array &v) {
  points = v;
  _polygon_changed();
}


 void PolygonBase::polygon_changed() {
  _polygon_changed();
}


 void PolygonBase::_polygon_changed() {
  emit_changed();
}


 String PolygonBase::to_string() {
   Variant  = PoolStringArray();

  for (p in points) {
   rv.append("("+str(p.x)+","+str(p.y)+")");
}

  return rv.join(",");
}

}

 PolygonBase::PolygonBase() {
  points = [Vector2(0.2, 0.2), Vector2(0.7, 0.4), Vector2(0.4, 0.7)];
 }

 PolygonBase::~PolygonBase() {
 }


 static void PolygonBase::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_points"), &PolygonBase::get_points);
   ClassDB::bind_method(D_METHOD("set_points", "value"), &PolygonBase::set_points);
   ADD_PROPERTY(PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "points"), "set_points", "get_points");


  ClassDB::bind_method(D_METHOD("clear"), &PolygonBase::clear);
  ClassDB::bind_method(D_METHOD("add_point", "x", "y", "closed"), &PolygonBase::add_point, true);
  ClassDB::bind_method(D_METHOD("remove_point", "index"), &PolygonBase::remove_point);
  ClassDB::bind_method(D_METHOD("get_point_count"), &PolygonBase::get_point_count);
  ClassDB::bind_method(D_METHOD("get_point", "i"), &PolygonBase::get_point);
  ClassDB::bind_method(D_METHOD("set_point", "i", "v"), &PolygonBase::set_point);
  ClassDB::bind_method(D_METHOD("set_points", "v"), &PolygonBase::set_points);
  ClassDB::bind_method(D_METHOD("polygon_changed"), &PolygonBase::polygon_changed);
  ClassDB::bind_method(D_METHOD("_polygon_changed"), &PolygonBase::_polygon_changed);
  ClassDB::bind_method(D_METHOD("to_string"), &PolygonBase::to_string);

 }



