#ifndef MAKE_TILEABLE_H
#define MAKE_TILEABLE_H


class MakeTileable : public MMNode {
 GDCLASS(MakeTileable, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_width() const;
 void set_width(const float val);

 int get_size() const;
 void set_size(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_width();
 void set_width(const float val);
 Color make_tileable(const Vector2 &uv, const float w);

 MakeTileable();
 ~MakeTileable();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float width = 0.1;
 int size = 0;
 //----------------------
 //make_tileable.mmg
 //vec4 make_tileable_$(name)(vec2 uv, float w) {
 //	vec4 a = $in(uv);
 //	vec4 b = $in(fract(uv+vec2(0.5)));
 //	float coef_ab = sin(1.57079632679*clamp((length(uv-vec2(0.5))-0.5+w)/w, 0.0, 1.0));
 //	vec4 c = $in(fract(uv+vec2(0.25)));
 //	float coef_abc = sin(1.57079632679*clamp((min(min(length(uv-vec2(0.0, 0.5)), length(uv-vec2(0.5, 0.0))), min(length(uv-vec2(1.0, 0.5)), length(uv-vec2(0.5, 1.0))))-w)/w, 0.0, 1.0));
 //	return mix(c, mix(a, b, coef_ab), coef_abc);
 //}
};


#endif
