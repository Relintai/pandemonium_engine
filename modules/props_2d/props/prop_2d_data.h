#ifndef PROP_2D_DATA_H
#define PROP_2D_DATA_H
/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "core/math/transform.h"
#include "core/reference.h"
#include "core/vector.h"

#include "core/math/rect2.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#include "prop_2d_data_entry.h"

#if TEXTURE_PACKER_PRESENT
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

#if TEXTURE_PACKER_PRESENT
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
