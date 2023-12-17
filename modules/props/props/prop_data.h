#ifndef PROP_DATA_H
#define PROP_DATA_H


#include "core/math/transform.h"
#include "core/object/reference.h"
#include "core/containers/vector.h"

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "prop_data_entry.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

class PropData : public Resource {
	GDCLASS(PropData, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	bool get_snap_to_mesh() const;
	void set_snap_to_mesh(const bool value);

	Vector3 get_snap_axis() const;
	void set_snap_axis(const Vector3 &value);

	Ref<PropDataEntry> get_prop(const int index) const;
	void set_prop(const int index, const Ref<PropDataEntry> prop);
	void add_prop(const Ref<PropDataEntry> prop);
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

	void copy_from(const Ref<PropData> &prop_data);

	PropData();
	~PropData();

protected:
	static void _bind_methods();

private:
	int _id;
	bool _snap_to_mesh;
	Vector3 _snap_axis;

	Vector<Ref<PropDataEntry>> _props;

	bool _is_room;
	PoolVector3Array _room_bounds;
};

#endif
