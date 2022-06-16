#ifndef BRIGHTNESS_CONTRAST_H
#define BRIGHTNESS_CONTRAST_H


class BrightnessContrast : public MMNode {
 GDCLASS(BrightnessContrast, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_brightness() const;
 void set_brightness(const float val);

 float get_contrast() const;
 void set_contrast(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_brightness();
 void set_brightness(const float val);
 float get_contrast();
 void set_contrast(const float val);

 BrightnessContrast();
 ~BrightnessContrast();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float brightness = 0;
 //export(float) 
 float contrast = 1;
 //brightness
 //contrast
};


#endif
