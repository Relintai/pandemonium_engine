#ifndef COLOR_TILER_H
#define COLOR_TILER_H


class ColorTiler : public MMNode {
 GDCLASS(ColorTiler, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_in_mask();
 void set_in_mask(const Ref<Resource> &val);

 Ref<Resource> get_output();
 void set_output(const Ref<Resource> &val);

 Ref<Resource> get_instance_map();
 void set_instance_map(const Ref<Resource> &val);

 Vector2 get_tile();
 void set_tile(const Vector2 &val);

 float get_overlap() const;
 void set_overlap(const float val);

 int get_select_inputs() const;
 void set_select_inputs(const int val);

 Vector2 get_scale();
 void set_scale(const Vector2 &val);

 float get_fixed_offset() const;
 void set_fixed_offset(const float val);

 float get_rnd_offset() const;
 void set_rnd_offset(const float val);

 float get_rnd_rotate() const;
 void set_rnd_rotate(const float val);

 float get_rnd_scale() const;
 void set_rnd_scale(const float val);

 float get_rnd_opacity() const;
 void set_rnd_opacity(const float val);

 bool get_variations() const;
 void set_variations(const bool val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 get_tile();
 void set_tile(const Vector2 &val);
 float get_overlap();
 void set_overlap(const float val);
 int get_select_inputs();
 void set_select_inputs(const int val);
 Vector2 get_scale();
 void set_scale(const Vector2 &val);
 float get_fixed_offset();
 void set_fixed_offset(const float val);
 float get_rnd_offset();
 void set_rnd_offset(const float val);
 float get_rnd_rotate();
 void set_rnd_rotate(const float val);
 float get_rnd_scale();
 void set_rnd_scale(const float val);
 float get_rnd_opacity();
 void set_rnd_opacity(const float val);
 bool get_variations();
 void set_variations(const bool val);
 PoolColorArray tiler_calc(const Vector2 &uv, const Vector2 &tile, const int overlap, const Vector2 &_seed);

 ColorTiler();
 ~ColorTiler();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> in_mask;
 //export(Resource) 
 Ref<Resource> output;
 //export(Resource) 
 Ref<Resource> instance_map;
 //export(Vector2) 
 Vector2 tile = Vector2(4, 4);
 //export(float) 
 float overlap = 1;
 //export(int, "1,4,16") 
 int select_inputs = 0;
 //export(Vector2) 
 Vector2 scale = Vector2(0.5, 0.5);
 //export(float) 
 float fixed_offset = 0;
 //export(float) 
 float rnd_offset = 0.25;
 //export(float) 
 float rnd_rotate = 45;
 //export(float) 
 float rnd_scale = 0.2;
 //export(float) 
 float rnd_opacity = 0;
 //export(bool) 
 bool variations = false;
 //mm_graph_node.add_slot_bool("get_variations", "set_variations", "Variations")
 //tile
 //overlap
 //select_inputs
 //scale
 //fixed_offset
 //rnd_offset
 //rnd_rotate
 //rnd_scale
 //rnd_opacity
 //variations
 //----------------------
 //color_tiler.mmg
 //Tiles several occurences of an input image while adding randomness.
 //vec4 tiler_$(name)(vec2 uv, vec2 tile, int overlap, vec2 _seed, out vec3 random_color) {
 //	vec4 c = vec4(0.0);
 //	vec3 rc = vec3(0.0);
 //	vec3 rc1;
 //
 //	for (int dx = -overlap; dx <= overlap; ++dx) {
 //		for (int dy = -overlap; dy <= overlap; ++dy) {
 //			vec2 pos = fract((floor(uv*tile)+vec2(float(dx), float(dy))+vec2(0.5))/tile-vec2(0.5));
 //			vec2 seed = rand2(pos+_seed);
 //			rc1 = rand3(seed);
 //			pos = fract(pos+vec2($fixed_offset/tile.x, 0.0)*floor(mod(pos.y*tile.y, 2.0))+$offset*seed/tile);
 //			float mask = $mask(fract(pos+vec2(0.5)));
 //			if (mask > 0.01) {
 //				vec2 pv = fract(uv - pos)-vec2(0.5);
 //				seed = rand2(seed);
 //				float angle = (seed.x * 2.0 - 1.0) * $rotate * 0.01745329251;
 //				float ca = cos(angle);
 //				float sa = sin(angle);
 //				pv = vec2(ca*pv.x+sa*pv.y, -sa*pv.x+ca*pv.y);
 //				pv *= (seed.y-0.5)*2.0*$scale+1.0;
 //				pv /= vec2($scale_x, $scale_y);
 //				pv += vec2(0.5);
 //				pv = clamp(pv, vec2(0.0), vec2(1.0));
 //
 //				$select_inputs
 //
 //				vec4 n = $in.variation(pv, $variations ? seed.x : 0.0);
 //
 //				seed = rand2(seed);
 //				float na = n.a*mask*(1.0-$opacity*seed.x);
 //				float a = (1.0-c.a)*(1.0*na);
 //
 //				c = mix(c, n, na);
 //				rc = mix(rc, rc1, n.a);
 //			}
 //		}
 //	}
 //
 //	random_color = rc;
 //	return c;
 //}
 //select_inputs enum
 //1,  " "
 //4, "pv = clamp(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));"
 //16, "pv = clamp(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));"
};


#endif
