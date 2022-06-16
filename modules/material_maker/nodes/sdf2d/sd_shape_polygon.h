#ifndef SD_SHAPE_POLYGON_H
#define SD_SHAPE_POLYGON_H


class SdShapePolygon : public PolygonBase {
 GDCLASS(SdShapePolygon, PolygonBase);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 void _polygon_changed();

 SdShapePolygon();
 ~SdShapePolygon();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
};


#endif
