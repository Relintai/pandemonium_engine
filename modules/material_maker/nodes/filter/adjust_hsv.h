#ifndef ADJUST_HSV_H
#define ADJUST_HSV_H


class AdjustHsv : public MMNode {
 GDCLASS(AdjustHsv, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_hue() const;
 void set_hue(const float val);

 float get_saturation() const;
 void set_saturation(const float val);

 float get_value() const;
 void set_value(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_hue();
 void set_hue(const float val);
 float get_saturation();
 void set_saturation(const float val);
 float get_value();
 void set_value(const float val);

 AdjustHsv();
 ~AdjustHsv();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float hue = 0;
 //export(float) 
 float saturation = 1;
 //export(float) 
 float value = 1;
 //hue
 //saturation
 //value
};


#endif
