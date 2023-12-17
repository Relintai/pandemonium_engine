#ifndef PROP_2D_DATA_H
#define PROP_2D_DATA_H


#include "core/math/transform.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "prop_2d_data_entry.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

class Prop2DData : public Resource {
	GDCLASS(Prop2DData, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	Ref<Prop2DDataEntry> get_prop(const int index) const;
	void set_prop(const int index, const Ref<Prop2DDataEntry> prop);
	void add_prop(const Ref<Prop2DDataEntry> prop);
	void remove_prop(const int index);

	int get_prop_count() const;

	Vector<Variant> get_props();
	void set_props(const Vector<Variant> &props);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool get_is_room() const;
	void set_is_room(const bool value);

	PoolVector3Array get_room_bounds();
	void set_room_bounds(const PoolVector3Array &bounds);

	void copy_from(const Ref<Prop2DData> &prop_data);

	Prop2DData();
	~Prop2DData();

protected:
	static void _bind_methods();

private:
	int _id;
	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Vector<Ref<Prop2DDataEntry>> _props;

	bool _is_room;
	PoolVector3Array _room_bounds;
};

#endif
