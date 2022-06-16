#ifndef ICHING_H
#define ICHING_H


class Iching : public MMNode {
 GDCLASS(Iching, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Vector2 get_size();
 void set_size(const Vector2 &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_size();
 void set_size(const Vector2 &val);

 Iching();
 ~Iching();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Vector2) 
 Vector2 size = Vector2(4, 4);
 //size
};


#endif
