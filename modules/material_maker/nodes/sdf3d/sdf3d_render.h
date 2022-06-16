#ifndef SDF3D_RENDER_H
#define SDF3D_RENDER_H


class Sdf3dRender : public MMNode {
 GDCLASS(Sdf3dRender, MMNode);

 public:

 Ref<Resource> get_input();
 void set_input(const Ref<Resource> &val);

 Ref<Resource> get_out_height_map();
 void set_out_height_map(const Ref<Resource> &val);

 Ref<Resource> get_out_normal_map();
 void set_out_normal_map(const Ref<Resource> &val);

 Ref<Resource> get_out_color_map();
 void set_out_color_map(const Ref<Resource> &val);

 void _init_properties();
 void _register_methods(const Variant &mm_graph_node);
 void _render(const Variant &material);
 Color _get_value_for(const Vector2 &uv, const int pseed);
 Vector2 sdf3d_raymarch(const Vector2 &uv);
 Vector3 sdf3d_normal(const Vector3 &p);

 Sdf3dRender();
 ~Sdf3dRender();

 protected:
 static void _bind_methods();

 //tool
 //export(Resource) 
 Ref<Resource> input;
 //export(Resource) 
 Ref<Resource> out_height_map;
 //export(Resource) 
 Ref<Resource> out_normal_map;
 //export(Resource) 
 Ref<Resource> out_color_map;
 //vec2 raymarch_$name(vec2 uv) {
 //	vec3 ro = vec3(uv-vec2(0.5), 1.0);
 //	vec3 rd = vec3(0.0, 0.0, -1.0);
 //	float dO = 0.0;
 //	float c = 0.0;
 //
 //	for (int i=0; i < 100; i++) {
 //		vec3 p = ro + rd*dO;
 //		vec2 dS = $sdf(p);
 //		dO += dS.x;
 //
 //		if (dO >= 1.0) {
 //			break;
 //		} else if (dS.x < 0.0001) {
 //			c = dS.y;
 //			break;
 //		}
 //	}
 //
 //	return vec2(dO, c);
 //}
 //vec3 normal_$name(vec3 p) {
 //	if (p.z <= 0.0) {
 //		return vec3(0.0, 0.0, 1.0);
 //	}
 //
 //	float d = $sdf(p).x;
 //	float e = .001;
 //	vec3 n = d - vec3(
 //		$sdf(p-vec3(e, 0.0, 0.0)).x,
 //		$sdf(p-vec3(0.0, e, 0.0)).x,
 //		$sdf(p-vec3(0.0, 0.0, e)).x);
 //
 //	return vec3(-1.0, -1.0, -1.0)*normalize(n);
 //}
};


#endif
