#ifndef MODEL_VISUAL_H
#define MODEL_VISUAL_H


#include "core/object/resource.h"

#include "../../item_enums.h"
#include "model_visual_entry.h"

class ModelVisual : public Resource {
	GDCLASS(ModelVisual, Resource);

public:
	int get_layer();
	void set_layer(int layer);

	//ModelVisualEntry
	Ref<ModelVisualEntry> get_visual_entry(const int index) const;
	void set_visual_entry(const int index, const Ref<ModelVisualEntry> visual_entry);
	void add_visual_entry(const Ref<ModelVisualEntry> visual_entry);
	void remove_visual_entry(const int index);

	int get_visual_entry_count() const;

	Vector<Variant> get_visual_entries();
	void set_visual_entries(const Vector<Variant> &visual_entries);

	ModelVisual();
	~ModelVisual();

protected:
#ifndef DISABLE_DEPRECATED
	// TODO REMOVE AFTER NEXT RELEASE
	bool _set(const StringName &p_name, const Variant &p_value);
#endif
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	int _layer;

	Vector<Ref<ModelVisualEntry>> _visual_entries;
};

#endif
