#ifndef BLEND_H
#define BLEND_H


class Blend : public MMNode {
 GDCLASS(Blend, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input1();
 void set_input1(const Ref<Resource> &val);

 Ref<Resource> get_input2();
 void set_input2(const Ref<Resource> &val);

 int get_blend_type() const;
 void set_blend_type(const int val);

 Ref<Resource> get_opacity();
 void set_opacity(const Ref<Resource> &val);

 enum BlendType  {

  NORMAL = 0,
  DISSOLVE,
  MULTIPLY,
  SCREEN,
  OVERLAY,
  HARD_LIGHT,
  SOFT_LIGHT,
  BURN,
  DODGE,
  LIGHTEN,
  DARKEN,
  DIFFRENCE
};

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 int get_blend_type();
 void set_blend_type(const int val);

 Blend();
 ~Blend();

 protected:
 static void _bind_methods();

 //tool
 };
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input1;
 //export(Resource) 
 Ref<Resource> input2;
 //export(int, "Normal,Dissolve,Multiply,Screen,Overlay,Hard Light,Soft Light,Burn,Dodge,Lighten,Darken,Difference") 
 int blend_type = 0;
 //export(Resource) 
 Ref<Resource> opacity;
};


#endif
