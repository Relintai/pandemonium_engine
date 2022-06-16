#ifndef BLUR_GAUSSIAN_H
#define BLUR_GAUSSIAN_H


class BlurGaussian : public MMNode {
 GDCLASS(BlurGaussian, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_sigma();
 void set_sigma(const Ref<Resource> &val);

 int get_direction() const;
 void set_direction(const int val);

 int get_size() const;
 void set_size(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Image _render_image(const Variant &material);
 Color get_value_x(const Vector2 &uv, const int pseed);
 Color get_value_y(const Vector2 &uv, const int pseed);
 Color get_value_y_img(const Vector2 &uv, const int pseed);
 int get_direction();
 void set_direction(const int val);
 Color gaussian_blur_x(const Vector2 &uv, const float psize, const float psigma, const float pamount);
 Color gaussian_blur_y(const Vector2 &uv, const float psize, const float psigma, const float pamount);
 Color gaussian_blur_y_img(const Vector2 &uv, const float psize, const float psigma, const float pamount);

 BlurGaussian();
 ~BlurGaussian();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> sigma;
 //export(int, "Both,X,Y") 
 int direction = 0;
 int size = 0;
 //----------------------
 //gaussian_blur_x.mmg
 //vec4 $(name)_fct(vec2 uv) {
 //	float e = 1.0 / $size;
 //	vec4 rv = vec4(0.0);
 //	float sum = 0.0;
 //	float sigma = max(0.000001, $sigma * $amount(uv));
 //
 //	for (float i = -50.0; i <= 50.0; i += 1.0) {
 //		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718 * sigma * sigma);
 //		rv += $in(uv+vec2(i*e, 0.0))*coef;
 //		sum += coef;
 //	}
 //
 //	return rv/sum;
 //}
 //----------------------
 //gaussian_blur_y.mmg
 //vec4 $(name)_fct(vec2 uv) {
 //	float e = 1.0/$size;
 //	vec4 rv = vec4(0.0);
 //	float sum = 0.0;
 //	float sigma = max(0.000001, $sigma*$amount(uv));
 //	for (float i = -50.0; i <= 50.0; i += 1.0) {
 //		float coef = exp(-0.5 * (pow(i / sigma, 2.0))) / (6.28318530718*sigma*sigma);
 //		rv += $in(uv+vec2(0.0, i*e))*coef;
 //		sum += coef;
 //	}
 //
 //	return rv/sum;
 //}
};


#endif
