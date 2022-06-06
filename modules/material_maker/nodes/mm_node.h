#ifndef MM_NODE_H
#define MM_NODE_H

#include "core/image.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/variant.h"

#include "core/resource.h"

class MMMaterial;
class MMNodeUniversalProperty;

class MMNode : public Resource {
	GDCLASS(MMNode, Resource);

public:
	Vector2 get_graph_position();
	void set_graph_position(const Vector2 &val);

	bool get_properties_initialized() const;
	void set_properties_initialized(const bool val);

	bool get_dirty() const;
	void set_dirty(const bool val);

	bool render(const Ref<MMMaterial> &material);
	void _render(const Ref<MMMaterial> &material);
	Ref<Image> render_image(const Ref<MMMaterial> &material);
	Color get_value_for(const Vector2 &uv, const int pseed);

	void init_properties();
	void _init_properties();

	void register_methods(const Variant &mm_graph_node);
	void _register_methods(const Variant &mm_graph_node);

	void register_input_property(const Ref<MMNodeUniversalProperty> &prop);
	void unregister_input_property(const Ref<MMNodeUniversalProperty> &prop);

	void register_output_property(const Ref<MMNodeUniversalProperty> &prop);
	void unregister_output_property(const Ref<MMNodeUniversalProperty> &prop);

	void on_input_property_changed();

	MMNode();
	~MMNode();

protected:
	static void _bind_methods();

	Vector2 graph_position;
	Vector<Ref<MMNodeUniversalProperty>> input_properties;
	Vector<Ref<MMNodeUniversalProperty>> output_properties;
	bool properties_initialized;
	bool dirty;
};

#endif
