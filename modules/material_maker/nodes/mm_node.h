#ifndef MAT_MAKER_NODE_H
#define MAT_MAKER_NODE_H

#include "core/image.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/variant/variant.h"

#include "core/object/resource.h"

class Node;
class MMMaterial;
class MMNodeUniversalProperty;
class MMGraphNode;

class MMNode : public Resource {
	GDCLASS(MMNode, Resource);

public:
	Vector2 get_graph_position();
	void set_graph_position(const Vector2 &val);

	Vector<Variant> get_input_properties();
	void set_input_properties(const Vector<Variant> &val);

	Vector<Variant> get_output_properties();
	void set_output_properties(const Vector<Variant> &val);

	bool get_properties_initialized() const;
	void set_properties_initialized(const bool val);

	bool get_dirty() const;
	void set_dirty(const bool val);

	bool render(const Ref<MMMaterial> &material);
	virtual void _render(const Ref<MMMaterial> &material);

	Ref<Image> render_image(const Ref<MMMaterial> &material);
	virtual Ref<Image> _render_image(const Ref<MMMaterial> &material);

	Color get_value_for(const Vector2 &uv, const int pseed);
	virtual Color _get_value_for(const Vector2 &uv, const int pseed);

	void init_properties();
	virtual void _init_properties();

	void register_methods(Node *mm_graph_node);
	virtual void _register_methods_bind(Node *mm_graph_node);
	virtual void _register_methods(MMGraphNode *mm_graph_node);

	void register_input_property(const Ref<MMNodeUniversalProperty> &prop);
	void unregister_input_property(const Ref<MMNodeUniversalProperty> &prop);

	void register_output_property(const Ref<MMNodeUniversalProperty> &prop);
	void unregister_output_property(const Ref<MMNodeUniversalProperty> &prop);

	void on_input_property_changed();

	Variant get_property_value(const Vector2 &uv);
	virtual Variant _get_property_value(const Vector2 &uv);

	Vector2 get_property_value_sdf3d(const Vector3 &uv3);
	virtual Vector2 _get_property_value_sdf3d(const Vector3 &uv3);

	MMNode();
	~MMNode();

	Vector2 graph_position;
	Vector<Ref<MMNodeUniversalProperty>> input_properties;
	Vector<Ref<MMNodeUniversalProperty>> output_properties;
	bool properties_initialized;
	bool dirty;

protected:
	static void _bind_methods();
};

#endif
