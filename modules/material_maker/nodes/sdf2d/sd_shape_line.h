#ifndef SD_SHAPE_LINE_H
#define SD_SHAPE_LINE_H


class SdShapeLine : public CurveBase {
 GDCLASS(SdShapeLine, CurveBase);

 public:

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Vector2 get_A();
 void set_A(const Vector2 &val);

 Vector2 get_B();
 void set_B(const Vector2 &val);

 float get_width() const;
 void set_width(const float val);

 void _init();
 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 float _get_property_value(const Vector2 &uv);
 Vector2 get_a();
 void set_a(const Vector2 &val);
 Vector2 get_b();
 void set_b(const Vector2 &val);
 float get_width();
 void set_width(const float val);
 void _curve_changed();

 SdShapeLine();
 ~SdShapeLine();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> output;
 //export(Vector2) 
 Vector2 A = Vector2(-0.3, -0.3);
 //export(Vector2) 
 Vector2 B = Vector2(0.3, 0.3);
 //export(float) 
 float width = 0.1;
 //a
 //b
 //width
};


#endif
