
#include "blur_gaussian.h"


Ref<Resource> BlurGaussian::get_image() {
 return image;
}

void BlurGaussian::set_image(const Ref<Resource> &val) {
image = val;
}


Ref<Resource> BlurGaussian::get_input() {
 return input;
}

void BlurGaussian::set_input(const Ref<Resource> &val) {
input = val;
}


Ref<Resource> BlurGaussian::get_sigma() {
 return sigma;
}

void BlurGaussian::set_sigma(const Ref<Resource> &val) {
sigma = val;
}


int BlurGaussian::get_direction() const {
 return direction;
}

void BlurGaussian::set_direction(const int val) {
direction = val;
}


int BlurGaussian::get_size() const {
 return size;
}

void BlurGaussian::set_size(const int val) {
size = val;
}



 //tool;
 //export(Resource) ;
  Ref<Resource> image;
 //export(Resource) ;
  Ref<Resource> input;
 //export(Resource) ;
  Ref<Resource> sigma;
 //export(int, "Both,X,Y") ;
  int direction = 0;
  int size = 0;

 void BlurGaussian::_init_properties() {

  if (!image) {
   image = MMNodeUniversalProperty.new();
   image.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_IMAGE;
}

  image.output_slot_type = MMNodeUniversalProperty.SLOT_TYPE_IMAGE;

  if (!input) {
   input = MMNodeUniversalProperty.new();
   input.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_COLOR;
   input.set_default_value(Color());
}

  input.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  input.slot_name = ">>>    Input1    ";

  if (!sigma) {
   sigma = MMNodeUniversalProperty.new();
   sigma.default_type = MMNodeUniversalProperty.DEFAULT_TYPE_FLOAT;
   sigma.set_default_value(50);
}

  sigma.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_UNIVERSAL;
  sigma.slot_name = "Sigma";
  register_input_property(input);
  register_output_property(image);
  register_input_property(sigma);
}


 void BlurGaussian::_register_methods(const Variant &mm_graph_node) {
  mm_graph_node.add_slot_texture_universal(image);
  mm_graph_node.add_slot_label_universal(input);
  mm_graph_node.add_slot_int_universal(sigma);
  mm_graph_node.add_slot_enum("get_direction", "set_direction", "Direction", [ "Both", "X", "Y" ]);
}


 void BlurGaussian::_render(const Variant &material) {
  size = max(material.image_size.x, material.image_size.y);
   Ref<Image> img = render_image(material);
  image.set_value(img);
}


 Image BlurGaussian::_render_image(const Variant &material) {
   Ref<Image> img = Image.new();
  img.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
  img.lock();
   float w = img.get_width();
   float h = img.get_width();
   float pseed = randf() + randi();

  if (direction == 0) {

   for (int x = 0; x < img.get_width(); ++x) { //x in range(img.get_width())

    for (int y = 0; y < img.get_height(); ++y) { //y in range(img.get_height())
      Vector2 v = Vector2(x / w, y / h);
      Color col = get_value_x(v, pseed);
     img.set_pixel(x, y, col);
}

}

   img.unlock();
   image.set_value(img);
    Ref<Image> image2 = Image.new();
   image2.create(material.image_size.x, material.image_size.y, false, Image.FORMAT_RGBA8);
   image2.lock();

   for (int x = 0; x < img.get_width(); ++x) { //x in range(img.get_width())

    for (int y = 0; y < img.get_height(); ++y) { //y in range(img.get_height())
      Vector2 v = Vector2(x / w, y / h);
      Color col = get_value_y_img(v, pseed);
     image2.set_pixel(x, y, col);
}

}

   image2.unlock();
   return image2;
}


  if (direction == 1) {

   for (int x = 0; x < img.get_width(); ++x) { //x in range(img.get_width())

    for (int y = 0; y < img.get_height(); ++y) { //y in range(img.get_height())
      Vector2 v = Vector2(x / w, y / h);
      Color col = get_value_x(v, pseed);
     img.set_pixel(x, y, col);
}

}

}


  if (direction == 2) {

   for (int x = 0; x < img.get_width(); ++x) { //x in range(img.get_width())

    for (int y = 0; y < img.get_height(); ++y) { //y in range(img.get_height())
      Vector2 v = Vector2(x / w, y / h);
      Color col = get_value_y(v, pseed);
     img.set_pixel(x, y, col);
}

}

}

  img.unlock();
  return img;
}


 Color BlurGaussian::get_value_x(const Vector2 &uv, const int pseed) {
   float sig_def = sigma.get_default_value(uv);
   float sig = sigma.get_value(uv);
  return gaussian_blur_x(uv, size, sig_def, sig);
}


 Color BlurGaussian::get_value_y(const Vector2 &uv, const int pseed) {
   float sig_def = sigma.get_default_value(uv);
   float sig = sigma.get_value(uv);
  return gaussian_blur_y(uv, size, sig_def, sig);
}


 Color BlurGaussian::get_value_y_img(const Vector2 &uv, const int pseed) {
   float sig_def = sigma.get_default_value(uv);
   float sig = sigma.get_value(uv);
  return gaussian_blur_y_img(uv, size, sig_def, sig);
}


 int BlurGaussian::get_direction() {
  return direction;
}


 void BlurGaussian::set_direction(const int val) {
  direction = val;
  set_dirty(true);
}

 //----------------------;
 //gaussian_blur_x.mmg;
 //vec4 $(name)_fct(vec2 uv) {;
 //	float e = 1.0 / $size;
 //	vec4 rv = vec4(0.0);
 //	float sum = 0.0;
 //	float sigma = max(0.000001, $sigma * $amount(uv));
 //;
 //	for (float i = -50.0; i <= 50.0; i += 1.0) {;
 //		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
 //		rv += $in(uv+vec2(i*e, 0.0))*coef;
 //		sum += coef;
 //	};
 //;
 //	return rv/sum;
 //};

 Color BlurGaussian::gaussian_blur_x(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
   float e = 1.0 / psize;
   Color rv = Color();
   float sum = 0.0;
  //pamount(uv));
   float sigma = max(0.000001, psigma * pamount);
   float i = -50;
  //for (float i = -50.0; i <= 50.0; i += 1.0) {;

  while (i <= 50) {
    float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
   rv += input.get_value(uv + Vector2(i*e, 0.0)) * coef;
   sum += coef;
   i += 1;
}

  return rv / sum;
}

 //----------------------;
 //gaussian_blur_y.mmg;
 //vec4 $(name)_fct(vec2 uv) {;
 //	float e = 1.0/$size;
 //	vec4 rv = vec4(0.0);
 //	float sum = 0.0;
 //	float sigma = max(0.000001, $sigma*$amount(uv));
 //	for (float i = -50.0; i <= 50.0; i += 1.0) {;
 //		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718*sigma*sigma);
 //		rv += $in(uv+vec2(0.0, i*e))*coef;
 //		sum += coef;
 //	};
 //;
 //	return rv/sum;
 //};

 Color BlurGaussian::gaussian_blur_y(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
   float e = 1.0 / psize;
   Color rv = Color();
   float sum = 0.0;
  //pamount(uv));
   float sigma = max(0.000001, psigma * pamount);
   float i = -50;
  //for (float i = -50.0; i <= 50.0; i += 1.0) {;

  while (i <= 50) {
    float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
   rv += input.get_value(uv + Vector2(0.0, i * e)) * coef;
   sum += coef;
   i += 1;
}

  return rv / sum;
}


 Color BlurGaussian::gaussian_blur_y_img(const Vector2 &uv, const float psize, const float psigma, const float pamount) {
   float e = 1.0 / psize;
   Color rv = Color();
   float sum = 0.0;
  //pamount(uv));
   float sigma = max(0.000001, psigma * pamount);
   float i = -50;
  //for (float i = -50.0; i <= 50.0; i += 1.0) {;

  while (i <= 50) {
    float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
   rv += image.get_value(uv + Vector2(0.0, i * e)) * coef;
   sum += coef;
   i += 1;
}

  return rv / sum;
}

}

 BlurGaussian::BlurGaussian() {
  image;
  input;
  sigma;
  direction = 0;
  size = 0;
 }

 BlurGaussian::~BlurGaussian() {
 }


 static void BlurGaussian::_bind_methods() {
   ClassDB::bind_method(D_METHOD("get_image"), &BlurGaussian::get_image);
   ClassDB::bind_method(D_METHOD("set_image", "value"), &BlurGaussian::set_image);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_image", "get_image");


   ClassDB::bind_method(D_METHOD("get_input"), &BlurGaussian::get_input);
   ClassDB::bind_method(D_METHOD("set_input", "value"), &BlurGaussian::set_input);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "input", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_input", "get_input");


   ClassDB::bind_method(D_METHOD("get_sigma"), &BlurGaussian::get_sigma);
   ClassDB::bind_method(D_METHOD("set_sigma", "value"), &BlurGaussian::set_sigma);
   ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sigma", PROPERTY_HINT_RESOURCE_TYPE, "Ref<Resource>"), "set_sigma", "get_sigma");


   ClassDB::bind_method(D_METHOD("get_direction"), &BlurGaussian::get_direction);
   ClassDB::bind_method(D_METHOD("set_direction", "value"), &BlurGaussian::set_direction);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "direction"), "set_direction", "get_direction");


   ClassDB::bind_method(D_METHOD("get_size"), &BlurGaussian::get_size);
   ClassDB::bind_method(D_METHOD("set_size", "value"), &BlurGaussian::set_size);
   ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");


  ClassDB::bind_method(D_METHOD("_init_properties"), &BlurGaussian::_init_properties);
  ClassDB::bind_method(D_METHOD("_register_methods", "mm_graph_node"), &BlurGaussian::_register_methods);
  ClassDB::bind_method(D_METHOD("_render", "material"), &BlurGaussian::_render);
  ClassDB::bind_method(D_METHOD("_render_image", "material"), &BlurGaussian::_render_image);
  ClassDB::bind_method(D_METHOD("get_value_x", "uv", "pseed"), &BlurGaussian::get_value_x);
  ClassDB::bind_method(D_METHOD("get_value_y", "uv", "pseed"), &BlurGaussian::get_value_y);
  ClassDB::bind_method(D_METHOD("get_value_y_img", "uv", "pseed"), &BlurGaussian::get_value_y_img);
  ClassDB::bind_method(D_METHOD("get_direction"), &BlurGaussian::get_direction);
  ClassDB::bind_method(D_METHOD("set_direction", "val"), &BlurGaussian::set_direction);
  ClassDB::bind_method(D_METHOD("gaussian_blur_x", "uv", "psize", "psigma", "pamount"), &BlurGaussian::gaussian_blur_x);
  ClassDB::bind_method(D_METHOD("gaussian_blur_y", "uv", "psize", "psigma", "pamount"), &BlurGaussian::gaussian_blur_y);
  ClassDB::bind_method(D_METHOD("gaussian_blur_y_img", "uv", "psize", "psigma", "pamount"), &BlurGaussian::gaussian_blur_y_img);

 }



