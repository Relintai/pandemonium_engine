#ifndef PROP_INSTANCE_H
#define PROP_INSTANCE_H


#include "scene/main/spatial.h"

#include "scene/resources/material/material.h"

#include "core/math/vector3.h"

#include "props/prop_data.h"

class PropInstance : public Spatial {
	GDCLASS(PropInstance, Spatial);

public:
	Ref<PropData> get_prop_data();
	void set_prop_data(const Ref<PropData> &data);

	Ref<Material> get_material();
	void set_material(const Ref<Material> &material);

	uint32_t get_collision_layer() const;
	void set_collision_layer(uint32_t p_layer);

	uint32_t get_collision_mask() const;
	void set_collision_mask(uint32_t p_mask);

	virtual void collision_layer_changed();
	virtual void collision_mask_changed();

	void init_materials();
	virtual void _init_materials();

	void build();
	void queue_build();
	void build_finished();

	virtual void _build();
	virtual void _build_finished();

	void prop_preprocess(Transform tarnsform, const Ref<PropData> &prop);
	virtual void _prop_preprocess(Transform tarnsform, const Ref<PropData> &prop);

	PropInstance();
	~PropInstance();

protected:
	void _notification(int p_what);
	static void _bind_methods();

protected:
	Ref<PropData> _prop_data;
	Ref<Material> _material;

	uint32_t _collision_layer;
	uint32_t _collision_mask;

	bool _build_queued;
	bool _building;
};

#endif
