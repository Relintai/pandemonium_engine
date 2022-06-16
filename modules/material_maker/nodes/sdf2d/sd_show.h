#ifndef SD_SHOW_H
#define SD_SHOW_H


class SdShow : public MMNode {
 GDCLASS(SdShow, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_bevel() const;
 void set_bevel(const float val);

 float get_base() const;
 void set_base(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_bevel();
 void set_bevel(const float val);
 float get_base();
 void set_base(const float val);

 SdShow();
 ~SdShow();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float bevel = 0;
 //export(float) 
 float base = 0;
 //bevel
 //base
};


#endif
