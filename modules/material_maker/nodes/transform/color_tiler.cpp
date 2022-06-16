
#include "color_tiler.h"


Ref<Resource> ColorTiler::get_input() {
 return input;
}

void ColorTiler::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> ColorTiler::get_in_mask() {
 return in_mask;
}

void ColorTiler::set_in_mask(const Ref<Resource> &val) {
in_mask = val;
}


Ref<Resource> ColorTiler::get_output() {
 return output;
}

void ColorTiler::set_output(const Ref<Resource> &val) {
output = val;
}


Ref<Resource> ColorTiler::get_instance_map() {
 return instance_map;
}

void ColorTiler::set_instance_map(const Ref<Resource> &val) {
instance_map = val;
}


Vector2 ColorTiler::get_tile() {
 return tile;
}

void ColorTiler::set_tile(const Vector2 &val) {
tile = val;
}


float ColorTiler::get_overlap() const {
 return overlap;
}

void ColorTiler::set_overlap(const float val) {
overlap = val;
}


int ColorTiler::get_select_inputs() const {
 return select_inputs;
}

void ColorTiler::set_select_inputs(const int val) {
select_inputs = val;
}


Vector2 ColorTiler::get_scale() {
 return scale;
}

void ColorTiler::set_scale(const Vector2 &val) {
scale = val;
}


float ColorTiler::get_fixed_offset() const {
 return fixed_offset;
}

void ColorTiler::set_fixed_offset(const float val) {
fixed_offset = val;
}


float ColorTiler::get_rnd_offset() const {
 return rnd_offset;
}

void ColorTiler::set_rnd_offset(const float val) {
rnd_offset = val;
}


float ColorTiler::get_rnd_rotate() const {
 return rnd_rotate;
}

void ColorTiler::set_rnd_rotate(const float val) {
rnd_rotate = val;
}


float ColorTiler::get_rnd_scale() const {
 return rnd_scale;
}

void ColorTiler::set_rnd_scale(const float val) {
rnd_scale = val;
}


float ColorTiler::get_rnd_opacity() const {
 return rnd_opacity;
}

void ColorTiler::set_rnd_opacity(const float val) {
rnd_opacity = val;
}


bool ColorTiler::get_variations() const {
 return variations;
}

void ColorTiler::set_variations(const bool val) {
variations = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> in_mask;
 //export(Resource) ;
  Ref<Resource> output;
 //export(Resource) ;
  Ref<Resource> instance_map;
 //export(Vector2) ;
  Vector2 tile = Vector2(4, 4);
 //export(float) ;
  float overlap = 1;
 //export(int, "1,4,16") ;
  int select_inputs = 0;
 //export(Vector2) ;
  Vector2 scale = Vector2(0.5, 0.5);
 //export(float) ;
  float fixed_offset = 0;
 //export(float) ;
  float rnd_offset = 0.25;
 //export(float) ;
  float rnd_rotate = 45;
 //export(float) ;
  float rnd_scale = 0.2;
 //export(float) ;
  float rnd_opacity = 0;
 //export(bool) ;
  bool variations = false;

 void ColorTiler::_init_properties() {

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color(0, 0, 0, 1));
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input    ";

  if (!in_mask) {
   in_mask = MMNodeUniversalProperty.new();
   in_mask.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   in_mask.set_default_value(1);
}

  in_mask.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  in_mask.slot_name = ">>>    Mask    ";

  if (!output) {
   output = MMNodeUniversalProperty.new();
   output.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  output.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!instance_map) {
   instance_map = MMNodeUniversalProperty.new();
   instance_map.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  instance_map.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;
  register_input_property(input);
  register_input_property(in_mask);
  register_output_property(output);
  register_output_property(instance_map);
}


 void ColorTiler::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_label_universal(in_mask);
  mm_graph_node.add_slot_texture_universal(output);
  mm_graph_node.add_slot_texture_universal(instance_map);
  mm_graph_node.add_slot_vector2("get_tile", "set_tile", "Tile", 1);
  mm_graph_node.add_slot_float("get_overlap", "set_overlap", "Overlap", 1);
  mm_graph_node.add_slot_enum("get_select_inputs", "set_select_inputs", "Select inputs", [ "1", "4", "16" ]);
  mm_graph_node.add_slot_vector2("get_scale", "set_scale", "Scale", 0.01);
  mm_graph_node.add_slot_float("get_fixed_offset", "set_fixed_offset", "Fixed Offset", 0.01);
  mm_graph_node.add_slot_float("get_rnd_offset", "set_rnd_offset", "Rnd Offset", 0.01);
  mm_graph_node.add_slot_float("get_rnd_rotate", "set_rnd_rotate", "Rnd Rotate", 0.1);
  mm_graph_node.add_slot_float("get_rnd_scale", "set_rnd_scale", "Rnd Scale", 0.01);
  mm_graph_node.add_slot_float("get_rnd_opacity", "set_rnd_opacity", "Rnd Opacity", 0.01);
}

 //mm_graph_node.add_slot_bool("get_variations", "set_variations", "Variations");

 void ColorTiler::_render(const Variant &material) {
   Ref<Image> output_img = Image.new();
   Ref<Image> instance_map_img = Image.new();
  output_img.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  instance_map_img.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  output_img.lock();
  instance_map_img.lock();
   float w = material.image_size.x;
   float h = material.image_size.y;
   float pseed = randf() + randi();
   float ps = 1.0 / float(pseed);
   int ix = int(material.image_size.x);
   int iy = int(material.image_size.y);

  for (int x = 0; x < ix; ++x) { //x in range(ix)

   for (int y = 0; y < iy; ++y) { //y in range(iy)
     Vector2 uv = Vector2(x / w, y / h);
    //vec3 $(name_uv)_random_color;
    //vec4 $(name_uv)_tiled_output = tiler_$(name)($uv, vec2($tx, $ty), int($overlap), vec2(float($seed)), $(name_uv)_random_color);
     PoolColorArray rch = tiler_calc(uv, tile, overlap, Vector2(ps, ps));
    output_img.set_pixel(x, y, rch[1]);
    instance_map_img.set_pixel(x, y, rch[0]);
}

}

  output_img.unlock();
  instance_map_img.unlock();
  output.set_value(output_img);
  instance_map.set_value(instance_map_img);
}


 Color ColorTiler::_get_value_for(const Vector2 &uv, const int pseed) {
  return Color();
}

 //tile;

 Vector2 ColorTiler::get_tile() {
  return tile;
}


 void ColorTiler::set_tile(const Vector2 &val) {
  tile = val;
  set_dirty(true);
}

 //overlap;

 float ColorTiler::get_overlap() {
  return overlap;
}


 void ColorTiler::set_overlap(const float val) {
  overlap = val;
  set_dirty(true);
}

 //select_inputs;

 int ColorTiler::get_select_inputs() {
  return select_inputs;
}


 void ColorTiler::set_select_inputs(const int val) {
  select_inputs = val;
  set_dirty(true);
}

 //scale;

 Vector2 ColorTiler::get_scale() {
  return scale;
}


 void ColorTiler::set_scale(const Vector2 &val) {
  scale = val;
  set_dirty(true);
}

 //fixed_offset;

 float ColorTiler::get_fixed_offset() {
  return fixed_offset;
}


 void ColorTiler::set_fixed_offset(const float val) {
  fixed_offset = val;
  set_dirty(true);
}

 //rnd_offset;

 float ColorTiler::get_rnd_offset() {
  return rnd_offset;
}


 void ColorTiler::set_rnd_offset(const float val) {
  rnd_offset = val;
  set_dirty(true);
}

 //rnd_rotate;

 float ColorTiler::get_rnd_rotate() {
  return rnd_rotate;
}


 void ColorTiler::set_rnd_rotate(const float val) {
  rnd_rotate = val;
  set_dirty(true);
}

 //rnd_scale;

 float ColorTiler::get_rnd_scale() {
  return rnd_scale;
}


 void ColorTiler::set_rnd_scale(const float val) {
  rnd_scale = val;
  set_dirty(true);
}

 //rnd_opacity;

 float ColorTiler::get_rnd_opacity() {
  return rnd_opacity;
}


 void ColorTiler::set_rnd_opacity(const float val) {
  rnd_opacity = val;
  set_dirty(true);
}

 //variations;

 bool ColorTiler::get_variations() {
  return variations;
}


 void ColorTiler::set_variations(const bool val) {
  variations = val;
  set_dirty(true);
}

 //----------------------;
 //color_tiler.mmg;
 //Tiles several occurences of an input image while adding randomness.;
 //vec4 tiler_$(name)(vec2 uv, vec2 tile, int overlap, vec2 _seed, out vec3 random_color) {;
 //	vec4 c = vec4(0.0);
 //	vec3 rc = vec3(0.0);
 //	vec3 rc1;
 //;
 //	for (int dx = -overlap; dx <= overlap; ++dx) {;
 //		for (int dy = -overlap; dy <= overlap; ++dy) {;
 //			vec2 pos = fract((floor(uv*tile)+vec2(float(dx), float(dy))+vec2(0.5))/tile-vec2(0.5));
 //			vec2 seed = rand2(pos+_seed);
 //			rc1 = rand3(seed);
 //			pos = fract(pos+vec2($fixed_offset/tile.x, 0.0)*floor(mod(pos.y*tile.y, 2.0))+$offset*seed/tile);
 //			float mask = $mask(fract(pos+vec2(0.5)));
 //			if (mask > 0.01) {;
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
 //;
 //				$select_inputs;
 //;
 //				vec4 n = $in.variation(pv, $variations ? seed.x : 0.0);
 //;
 //				seed = rand2(seed);
 //				float na = n.a*mask*(1.0-$opacity*seed.x);
 //				float a = (1.0-c.a)*(1.0*na);
 //;
 //				c = mix(c, n, na);
 //				rc = mix(rc, rc1, n.a);
 //			};
 //		};
 //	};
 //;
 //	random_color = rc;
 //	return c;
 //};
 //select_inputs enum;
 //1,  " ";
 //4, "pv = clamp(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
 //16, "pv = clamp(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";

 PoolColorArray ColorTiler::tiler_calc(const Vector2 &uv, const Vector2 &tile, const int overlap, const Vector2 &_seed) {
   Color c = Color();
   Vector3 rc = Vector3();
   Vector3 rc1 = Vector3();
  //for (int dx = -overlap; dx <= overlap; ++dx) {;

  for (int dx = -overlap; dx <  overlap; ++dx) { //dx in range(-overlap, overlap)
   //for (int dy = -overlap; dy <= overlap; ++dy) {;

   for (int dy = -overlap; dy <  overlap; ++dy) { //dy in range(-overlap, overlap)
     Vector2 pos = MMAlgos.fractv2((MMAlgos.floorv2(uv * tile) + Vector2(dx, dy) + Vector2(0.5, 0.5)) / tile - Vector2(0.5, 0.5));
     Vector2 vseed = MMAlgos.rand2(pos + _seed);
    rc1 = MMAlgos.rand3(vseed);
    pos = MMAlgos.fractv2(pos + Vector2(fixed_offset / tile.x, 0.0) * floor(MMAlgos.modf(pos.y * tile.y, 2.0)) + rnd_offset * vseed / tile);
     float mask = in_mask.get_value(MMAlgos.fractv2(pos + Vector2(0.5, 0.5)));

    if ((mask > 0.01)) {
      Vector2 pv = MMAlgos.fractv2(uv - pos) - Vector2(0.5, 0.5);
     vseed = MMAlgos.rand2(vseed);
      float angle = (vseed.x * 2.0 - 1.0) * rnd_rotate * 0.01745329251;
      float ca = cos(angle);
      float sa = sin(angle);
     pv = Vector2(ca * pv.x + sa * pv.y, -sa * pv.x + ca * pv.y);
     pv *= (vseed.y-0.5) * 2.0 * rnd_scale + 1.0;
     pv /= scale;
     pv += Vector2(0.5, 0.5);
     pv = MMAlgos.clampv2(pv, Vector2(), Vector2(1, 1));
     //1,  " ";
     //4, "pv = clamp(0.5*(pv+floor(rand2(seed)*2.0)), vec2(0.0), vec2(1.0));";
     //16, "pv = clamp(0.25*(pv+floor(rand2(seed)*4.0)), vec2(0.0), vec2(1.0));";

     if (select_inputs == 1) {
      pv = MMAlgos.clampv2(0.5*(pv + MMAlgos.floorv2(MMAlgos.rand2(vseed)*2.0)), Vector2(), Vector2(1, 1));
}


     else if (select_inputs == 2) {
      pv = MMAlgos.clampv2(0.25*(pv + MMAlgos.floorv2(MMAlgos.rand2(vseed)*4.0)), Vector2(), Vector2(1, 1));
}

     //				vec4 n = $in.variation(pv, $variations ? seed.x : 0.0);
      Color n = input.get_value(pv) * mask * (1.0 - rnd_opacity * vseed.x);
     vseed = MMAlgos.rand2(vseed);
      float na = n.a * mask * (1.0 - rnd_opacity * vseed.x);
      float a = (1.0 - c.a) * (1.0 * na);
     c = lerp(c, n, na);
     rc = lerp(rc, rc1, n.a);
}

}

}

   PoolColorArray pc = PoolColorArray();
  pc.append(Color(rc.x, rc.y, rc.z, 1));
  pc.append(c);
  return pc;
}

}

 ColorTiler::ColorTiler() {
  input;
  in_mask;
  output;
  instance_map;
  tile = Vector2(4, 4);
  overlap = 1;
  select_inputs = 0;
  scale = Vector2(0.5, 0.5);
  fixed_offset = 0;
  rnd_offset = 0.25;
  rnd_rotate = 45;
  rnd_scale = 0.2;
  rnd_opacity = 0;
  variations = false;
 }

 ColorTiler::~ColorTiler() {
 }


 static void ColorTiler::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_input"), &ColorTiler::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &ColorTiler::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_in_mask"), &ColorTiler::get_in_mask);
   ClassDB::bind_method(D_METHOD("set_in_mask", "value"), &ColorTiler::set_in_mask);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "in_mask", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_in_mask", "get_in_mask");


   ClassDB::bind_method(D_METHOD("get_output"), &ColorTiler::get_output);
   ClassDB::bind_method(D_METHOD("set_output", "value"), &ColorTiler::set_output);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_output", "get_output");


   ClassDB::bind_method(D_METHOD("get_instance_map"), &ColorTiler::get_instance_map);
   ClassDB::bind_method(D_METHOD("set_instance_map", "value"), &ColorTiler::set_instance_map);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "instance_map", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_instance_map", "get_instance_map");


   ClassDB::bind_method(D_METHOD("get_tile"), &ColorTiler::get_tile);
   ClassDB::bind_method(D_METHOD("set_tile", "value"), &ColorTiler::set_tile);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "tile"), "set_tile", "get_tile");


   ClassDB::bind_method(D_METHOD("get_overlap"), &ColorTiler::get_overlap);
   ClassDB::bind_method(D_METHOD("set_overlap", "value"), &ColorTiler::set_overlap);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "overlap"), "set_overlap", "get_overlap");


   ClassDB::bind_method(D_METHOD("get_select_inputs"), &ColorTiler::get_select_inputs);
   ClassDB::bind_method(D_METHOD("set_select_inputs", "value"), &ColorTiler::set_select_inputs);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "select_inputs"), "set_select_inputs", "get_select_inputs");


   ClassDB::bind_method(D_METHOD("get_scale"), &ColorTiler::get_scale);
   ClassDB::bind_method(D_METHOD("set_scale", "value"), &ColorTiler::set_scale);
   ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "scale"), "set_scale", "get_scale");


   ClassDB::bind_method(D_METHOD("get_fixed_offset"), &ColorTiler::get_fixed_offset);
   ClassDB::bind_method(D_METHOD("set_fixed_offset", "value"), &ColorTiler::set_fixed_offset);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "fixed_offset"), "set_fixed_offset", "get_fixed_offset");


   ClassDB::bind_method(D_METHOD("get_rnd_offset"), &ColorTiler::get_rnd_offset);
   ClassDB::bind_method(D_METHOD("set_rnd_offset", "value"), &ColorTiler::set_rnd_offset);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_offset"), "set_rnd_offset", "get_rnd_offset");


   ClassDB::bind_method(D_METHOD("get_rnd_rotate"), &ColorTiler::get_rnd_rotate);
   ClassDB::bind_method(D_METHOD("set_rnd_rotate", "value"), &ColorTiler::set_rnd_rotate);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_rotate"), "set_rnd_rotate", "get_rnd_rotate");


   ClassDB::bind_method(D_METHOD("get_rnd_scale"), &ColorTiler::get_rnd_scale);
   ClassDB::bind_method(D_METHOD("set_rnd_scale", "value"), &ColorTiler::set_rnd_scale);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_scale"), "set_rnd_scale", "get_rnd_scale");


   ClassDB::bind_method(D_METHOD("get_rnd_opacity"), &ColorTiler::get_rnd_opacity);
   ClassDB::bind_method(D_METHOD("set_rnd_opacity", "value"), &ColorTiler::set_rnd_opacity);
   ADD_PROPERTY(PropertyInfo(Variant::REAL, "rnd_opacity"), "set_rnd_opacity", "get_rnd_opacity");


   ClassDB::bind_method(D_METHOD("get_variations"), &ColorTiler::get_variations);
   ClassDB::bind_method(D_METHOD("set_variations", "value"), &ColorTiler::set_variations);
   ADD_PROPERTY(PropertyInfo(Variant::BOOL, "variations"), "set_variations", "get_variations");


  ClassDB::bind_method(D_METHOD("_init_properties"), &ColorTiler::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &ColorTiler::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &ColorTiler::_render);
  ClassDB::bind_method(D_METHOD("_get_value_for", "uv", "pseed"), &ColorTiler::_get_value_for);
  ClassDB::bind_method(D_METHOD("get_tile"), &ColorTiler::get_tile);
  ClassDB::bind_method(D_METHOD("set_tile", "val"), &ColorTiler::set_tile);
  ClassDB::bind_method(D_METHOD("get_overlap"), &ColorTiler::get_overlap);
  ClassDB::bind_method(D_METHOD("set_overlap", "val"), &ColorTiler::set_overlap);
  ClassDB::bind_method(D_METHOD("get_select_inputs"), &ColorTiler::get_select_inputs);
  ClassDB::bind_method(D_METHOD("set_select_inputs", "val"), &ColorTiler::set_select_inputs);
  ClassDB::bind_method(D_METHOD("get_scale"), &ColorTiler::get_scale);
  ClassDB::bind_method(D_METHOD("set_scale", "val"), &ColorTiler::set_scale);
  ClassDB::bind_method(D_METHOD("get_fixed_offset"), &ColorTiler::get_fixed_offset);
  ClassDB::bind_method(D_METHOD("set_fixed_offset", "val"), &ColorTiler::set_fixed_offset);
  ClassDB::bind_method(D_METHOD("get_rnd_offset"), &ColorTiler::get_rnd_offset);
  ClassDB::bind_method(D_METHOD("set_rnd_offset", "val"), &ColorTiler::set_rnd_offset);
  ClassDB::bind_method(D_METHOD("get_rnd_rotate"), &ColorTiler::get_rnd_rotate);
  ClassDB::bind_method(D_METHOD("set_rnd_rotate", "val"), &ColorTiler::set_rnd_rotate);
  ClassDB::bind_method(D_METHOD("get_rnd_scale"), &ColorTiler::get_rnd_scale);
  ClassDB::bind_method(D_METHOD("set_rnd_scale", "val"), &ColorTiler::set_rnd_scale);
  ClassDB::bind_method(D_METHOD("get_rnd_opacity"), &ColorTiler::get_rnd_opacity);
  ClassDB::bind_method(D_METHOD("set_rnd_opacity", "val"), &ColorTiler::set_rnd_opacity);
  ClassDB::bind_method(D_METHOD("get_variations"), &ColorTiler::get_variations);
  ClassDB::bind_method(D_METHOD("set_variations", "val"), &ColorTiler::set_variations);
  ClassDB::bind_method(D_METHOD("tiler_calc", "uv", "tile", "overlap", "_seed"), &ColorTiler::tiler_calc);

 }



