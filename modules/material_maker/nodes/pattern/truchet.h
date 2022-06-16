#ifndef TRUCHET_H
#define TRUCHET_H


class Truchet : public MMNode {
 GDCLASS(Truchet, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 int get_shape() const;
 void set_shape(const int val);

 float get_size() const;
 void set_size(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_shape();
 void set_shape(const int val);
 float get_size();
 void set_size(const float val);

 Truchet();
 ~Truchet();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(int, "Line,Circle") 
 int shape = 0;
 //export(float) 
 float size = 4;
 //shape
 //size
};


#endif
