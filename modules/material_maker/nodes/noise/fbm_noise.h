#ifndef FBM_NOISE_H
#define FBM_NOISE_H


class FbmNoise : public MMNode {
 GDCLASS(FbmNoise, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_type() const;
 void set_type(const int val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 int get_folds() const;
 void set_folds(const int val);

 int get_iterations() const;
 void set_iterations(const int val);

 float get_persistence() const;
 void set_persistence(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 void _render(const Variant &material);
 int get_type();
 void set_type(const int val);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);
 int get_folds();
 void set_folds(const int val);
 int get_iterations();
 void set_iterations(const int val);
 float get_persistence();
 void set_persistence(const float val);

 FbmNoise();
 ~FbmNoise();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(int, "Value,Perlin,Simplex,Cellular1,Cellular2,Cellular3,Cellular4,Cellular5,Cellular6") 
 int type = 0;
 //export(Vector2) 
 Vector2 scale = Vector2(2, 2);
 //export(int) 
 int folds = 0;
 //export(int) 
 int iterations = 5;
 //export(float) 
 float persistence = 0.5;
};


#endif
