
#include "sine_wave.h"


Ref<Resource> SineWave::get_image() {
 return image;
}

void SineWave::set_image(const Ref<Resource> &val) {
image = val;
}


float SineWave::get_amplitude() const {
 return amplitude;
}

void SineWave::set_amplitude(const float val) {
amplitude = val;
}


float SineWave::get_frequency() const {
 return frequency;
}

void SineWave::set_frequency(const float val) {
frequency = val;
}


float SineWave::get_phase() const {
 return phase;
}

void SineWave::set_phase(const float val) {
phase = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(float) ;
  float amplitude = 0.5;
 //export(float) ;
  float frequency = 2;
 //export(float) ;
  float phase = 0;

 void SineWave::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_output_property(image);
}


 void SineWave::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_float("get_amplitude", "set_amplitude", "Amplitude", 0.01);
  mm_graph_node.add_slot_float("get_frequency", "set_frequency", "Frequency", 0.1);
  mm_graph_node.add_slot_float("get_phase", "set_phase", "Phase", 0.01);
}


 void SineWave::_render(const Variant &material) {
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Color SineWave::_get_value_for(const Vector2 &uv, const int pseed) {
   float f = 1.0 - abs(2.0 * (uv.y - 0.5) - amplitude *sin((frequency * uv.x + phase)*6.28318530718));
  return Color(f, f, f, 1);
}

 //amplitude;

 float SineWave::get_amplitude() {
  return amplitude;
}


 void SineWave::set_amplitude(const float val) {
  amplitude = val;
  set_dirty(true);
}

 //frequency;

 float SineWave::get_frequency() {
  return frequency;
}


 void SineWave::set_frequency(const float val) {
  frequency = val;
  set_dirty(true);
}

 //phase;

 float SineWave::get_phase() {
  return phase;
}


 void SineWave::set_phase(const float val) {
  phase = val;
  set_dirty(true);
}

}

 SineWave::SineWave() {
  image;
  amplitude = 0.5;
  frequency = 2;
  phase = 0;
 }

 SineWave::~SineWave() {
 }


 static void SineWave::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &SineWave::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &SineWave::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_amplitude"), &SineWave::get_amplitude);
   ClassDB::bind_method(D_METHOD("set_amplitude", "value"), &SineWave::set_amplitude);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "amplitude"), "set_amplitude", "get_amplitude");


   ClassDB::bind_method(D_METHOD("get_frequency"), &SineWave::get_frequency);
   ClassDB::bind_method(D_METHOD("set_frequency", "value"), &SineWave::set_frequency);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "frequency"), "set_frequency", "get_frequency");


   ClassDB::bind_method(D_METHOD("get_phase"), &SineWave::get_phase);
   ClassDB::bind_method(D_METHOD("set_phase", "value"), &SineWave::set_phase);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "phase"), "set_phase", "get_phase");


  ClassDB::bind_method(D_METHOD("_init_properties"), &SineWave::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &SineWave::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &SineWave::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &SineWave::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_amplitude"), &SineWave::get_amplitude);
  ClassDB::bind_method(D_METHOD("set_amplitude", "val"), &SineWave::set_amplitude);
  ClassDB::bind_method(D_METHOD("get_frequency"), &SineWave::get_frequency);
  ClassDB::bind_method(D_METHOD("set_frequency", "val"), &SineWave::set_frequency);
  ClassDB::bind_method(D_METHOD("get_phase"), &SineWave::get_phase);
  ClassDB::bind_method(D_METHOD("set_phase", "val"), &SineWave::set_phase);

 }



