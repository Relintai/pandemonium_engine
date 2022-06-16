#ifndef SINE_WAVE_H
#define SINE_WAVE_H


class SineWave : public MMNode {
 GDCLASS(SineWave, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 float get_amplitude() const;
 void set_amplitude(const float val);

 float get_frequency() const;
 void set_frequency(const float val);

 float get_phase() const;
 void set_phase(const float val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_amplitude();
 void set_amplitude(const float val);
 float get_frequency();
 void set_frequency(const float val);
 float get_phase();
 void set_phase(const float val);

 SineWave();
 ~SineWave();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(float) 
 float amplitude = 0.5;
 //export(float) 
 float frequency = 2;
 //export(float) 
 float phase = 0;
 //amplitude
 //frequency
 //phase
};


#endif
