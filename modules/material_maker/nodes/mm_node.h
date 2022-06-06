#ifndef MM_NODE_H
#define MM_NODE_H

class MMNode : public Resource {
	GDCLASS(MMNode, Resource);

public:
	Vector2 get_graph_position();
	void set_graph_position(const Vector2 &val);

	Array get_input_properties();
	void set_input_properties(const Array &val);

	Array get_output_properties();
	void set_output_properties(const Array &val);

	bool get_properties_initialized() const;
	void set_properties_initialized(const bool val);

	bool get_dirty() const;
	void set_dirty(const bool val);

	bool render(const Variant &material);
	void _render(const Variant &material);
	Image render_image(const Variant &material);
	Color get_value_for(const Vector2 &uv, const int pseed);
	void init_properties();
	void _init_properties();
	void register_methods(const Variant &mm_graph_node);
	void _register_methods(const Variant &mm_graph_node);
	Vector2 get_graph_position();
	void set_graph_position(const Vector2 &pos);
	void register_input_property(const MMNodeUniversalProperty &prop);
	void unregister_input_property(const MMNodeUniversalProperty &prop);
	void register_output_property(const MMNodeUniversalProperty &prop);
	void unregister_output_property(const MMNodeUniversalProperty &prop);
	void set_dirty(const bool val);
	void on_input_property_changed();

	MMNode();
	~MMNode();

protected:
	static void _bind_methods();

	//tool
	//export(Vector2)
	Vector2 graph_position = Vector2();
	Array input_properties = ;
	Array output_properties = ;
	bool properties_initialized = false;
	bool dirty = true;
	//MMMateial
	//MMMateial
	// Add it to the MMAlgos bing class instead.
	// Not a perfect fit, but a better fit.
	//func editor_register_node_class(category : String, cls : String)
	// -> c++ method, adds node to the editor gui (add button)
	// in gdscript a plugin should instance an MMNode and call it to populate the add menu
	// with MMNodes
	// in c++ it should have a static counterpart.
	// register_types should populate c++ types with this
	//func editor_unregister_node_class(category : String, cls : String)
	//func editor_register_node_script(category : String, script_path : String)
	// same as the above, but for scripts
	//func editor_unregister_node_script(category : String, cls : String)
};

#endif
