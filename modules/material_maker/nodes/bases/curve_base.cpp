
#include "curve_base.h"


Variant CurveBase::get_Variant() {
 return Variant;
}

void CurveBase::set_Variant(const Variant &val) {
Variant = val;
}



 //tool;

  Vector2 Point::get_p() {
  return p;
 }

 void Point::set_p(const Vector2 &val) {
 p = val;
 }


 float Point::get_ls() const {
  return ls;
 }

 void Point::set_ls(const float val) {
 ls = val;
 }


 float Point::get_rs() const {
  return rs;
 }

 void Point::set_rs(const float val) {
 rs = val;
 }



   Vector2 p = ;
   float ls = ;
   float rs = ;

  void Point::_init(const float x, const float y, const float nls, const float nrs) {
   p = Vector2(x, y);
   ls = nls;
   rs = nrs;
}

  //export(PoolRealArray) ;
}

  Point::Point() {
   p = ;
   ls = ;
   rs = ;
  }

  Point::~Point() {
  }


  static void Point::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_p"), &Point::get_p);
    ClassDB::bind_method(D_METHOD("set_p", "value"), &Point::set_p);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "p"), "set_p", "get_p");


    ClassDB::bind_method(D_METHOD("get_ls"), &Point::get_ls);
    ClassDB::bind_method(D_METHOD("set_ls", "value"), &Point::set_ls);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "ls"), "set_ls", "get_ls");


    ClassDB::bind_method(D_METHOD("get_rs"), &Point::get_rs);
    ClassDB::bind_method(D_METHOD("set_rs", "value"), &Point::set_rs);
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "rs"), "set_rs", "get_rs");


   ClassDB::bind_method(D_METHOD("_init", "x", "y", "nls", "nrs"), &Point::_init);

  }


  Variant ;

 void CurveBase::init_points_01() {

  if (points.size() == 0) {
   points = [ 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 ];
}

}


 void CurveBase::init_points_11() {

  if (points.size() == 0) {
   points = [ 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0 ];
}

}


 String CurveBase::to_string() {
   Variant  = PoolStringArray();

  for (p in points) {
   rv.append("("+str(p.x)+","+str(p.y)+","+str(p.ls)+","+str(p.rs)+")");
}

  return rv.join(",");
}


 void CurveBase::clear() {
  points.clear();
  curve_changed();
}


 void CurveBase::add_point(const float x, const float y, const float ls, const float rs) {
   int indx = points.size() / 4;

  for (i in indx) {
    int ii = i * 4;

   if (x < points[ii]) {

    if (ls == INF) {
     ls == 0;
}


    if (rs == INF) {
     rs == 0;
}

    points.insert(ii, x);
    points.insert(ii + 1, y);
    points.insert(ii + 2, ls);
    points.insert(ii + 3, rs);
    curve_changed();
    return;
}

}

  points.append(x);
  points.append(y);
  points.append(ls);
  points.append(rs);
  curve_changed();
}


 bool CurveBase::remove_point(const int i) {
   int index = i * 4;

  if (index <= 0 || index >= points.size() - 1) {
   return false;
}


  else {
   points.remove(index);
   points.remove(index);
   points.remove(index);
   points.remove(index);
   curve_changed();
}

  return true;
}


 int CurveBase::get_point_count() {
  return points.size() / 4;
}


 void CurveBase::set_point(const int i, const Point &v) {
   int indx = i * 4;
  points[indx + 0] = v.p.x;
  points[indx + 1] = v.p.y;
  points[indx + 2] = v.ls;
  points[indx + 3] = v.rs;
  curve_changed();
}


 Point CurveBase::get_point(const int i) {
   int indx = i * 4;
  return Point.new(points[indx + 0], points[indx + 1], points[indx + 2], points[indx + 3]);
}


 Array CurveBase::get_points() {
   Array arr = Array();
   int c = get_point_count();

  for (int i = 0; i < c; ++i) { //i in range(c)
   arr.append(get_point(i));
}

  return arr;
}


 void CurveBase::set_points(const Array &arr, const bool notify) {
  points.resize(0);

  for (p in arr) {
   points.append(p.p.x);
   points.append(p.p.y);
   points.append(p.ls);
   points.append(p.rs);
}


  if (notify) {
   curve_changed();
}

}


 void CurveBase::curve_changed() {
  _curve_changed();
}


 void CurveBase::_curve_changed() {
  emit_changed();
}

}

 CurveBase::CurveBase() {
  ;
 }

 CurveBase::~CurveBase() {
 }


 static void CurveBase::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_Variant"), &CurveBase::get_Variant);
   ClassDB::bind_method(D_METHOD("set_Variant", "value"), &CurveBase::set_Variant);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "Variant", PROPERTY_HINT_RESOURCE_TYPE, "Variant"), "set_Variant", "get_Variant");


  ClassDB::bind_method(D_METHOD("init_points_01"), &CurveBase::init_points_01);
  ClassDB::bind_method(D_METHOD("init_points_11"), &CurveBase::init_points_11);
  ClassDB::bind_method(D_METHOD("to_string"), &CurveBase::to_string);
  ClassDB::bind_method(D_METHOD("clear"), &CurveBase::clear);
  ClassDB::bind_method(D_METHOD("add_point", "x", "y", "ls", "rs"), &CurveBase::add_point, INF, INF);
  ClassDB::bind_method(D_METHOD("remove_point", "i"), &CurveBase::remove_point);
  ClassDB::bind_method(D_METHOD("get_point_count"), &CurveBase::get_point_count);
  ClassDB::bind_method(D_METHOD("set_point", "i", "v"), &CurveBase::set_point);
  ClassDB::bind_method(D_METHOD("get_point", "i"), &CurveBase::get_point);
  ClassDB::bind_method(D_METHOD("get_points"), &CurveBase::get_points);
  ClassDB::bind_method(D_METHOD("set_points", "arr", "notify"), &CurveBase::set_points, true);
  ClassDB::bind_method(D_METHOD("curve_changed"), &CurveBase::curve_changed);
  ClassDB::bind_method(D_METHOD("_curve_changed"), &CurveBase::_curve_changed);

 }



