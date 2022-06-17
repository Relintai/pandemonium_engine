#ifndef MM_SDF3D_RENDER_H
#define MM_SDF3D_RENDER_H

#include "../mm_node.h"
#include "../mm_node_universal_property.h"

class MMSdf3dRender : public MMNode {
	GDCLASS(MMSdf3dRender, MMNode);

public:
	Ref<MMNodeUniversalProperty> get_input();
	void set_input(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_height_map();
	void set_out_height_map(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_normal_map();
	void set_out_normal_map(const Ref<MMNodeUniversalProperty> &val);

	Ref<MMNodeUniversalProperty> get_out_color_map();
	void set_out_color_map(const Ref<MMNodeUniversalProperty> &val);

	void _init_properties();
	void _register_methods(MMGraphNode *mm_graph_node);
	void _render(const Ref<MMMaterial> &material);
	Color _get_value_for(const Vector2 &uv, const int pseed);
	Vector2 sdf3d_raymarch(const Vector2 &uv);
	Vector3 sdf3d_normal(const Vector3 &p);

	MMSdf3dRender();
	~MMSdf3dRender();

protected:
	static void _bind_methods();

	Ref<MMNodeUniversalProperty> input;
	Ref<MMNodeUniversalProperty> out_height_map;
	Ref<MMNodeUniversalProperty> out_normal_map;
	Ref<MMNodeUniversalProperty> out_color_map;
};

#endif
