#ifndef EMBOSS_H
#define EMBOSS_H


class Emboss : public MMNode {
 GDCLASS(Emboss, MMNode);

 public:

 Ref<Resource> get_image();
 void set_image(const Ref<Resource> &val);

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 float get_angle() const;
 void set_angle(const float val);

 float get_amount() const;
 void set_amount(const float val);

 float get_width() const;
 void set_width(const float val);

 int get_size() const;
 void set_size(const int val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 float get_angle();
 void set_angle(const float val);
 float get_amount();
 void set_amount(const float val);
 float get_width();
 void set_width(const float val);
 float emboss(const Vector2 &uv, const float psize, const float pangle, const float pamount, const float pwidth);

 Emboss();
 ~Emboss();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> image;
 //export(Resource) 
 Ref<Resource> input;
 //export(float) 
 float angle = 0;
 //export(float) 
 float amount = 5;
 //export(float) 
 float width = 1;
 int size = 0;
 //float $(name)_fct(vec2 uv) {
 //	float pixels = max(1.0, $width);
 //	float e = 1.0/$size;
 //	float rv = 0.0;
 //
 //	for (float dx = -pixels; dx <= pixels; dx += 1.0) {
 //		for (float dy = -pixels; dy <= pixels; dy += 1.0) {
 //			if (abs(dx) > 0.5 || abs(dy) > 0.5) {
 //				rv += $in(uv+e*vec2(dx, dy))*cos(atan(dy, dx)-$angle*3.14159265359/180.0)/length(vec2(dx, dy));
 //			}
 //		}
 //	}
 //
 //	return $amount*rv/pixels+0.5;
 //}
};


#endif
