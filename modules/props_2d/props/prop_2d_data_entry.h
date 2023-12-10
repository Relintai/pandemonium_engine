#ifndef PROP_2D_DATA_DATA_H
#define PROP_2D_DATA_DATA_H
/*
Copyright (c) 2019-2023 Péter Magyar

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
#include "core/math/transform_2d.h"
#include "core/object/resource.h"

#include "scene/resources/material/material.h"

#include "modules/modules_enabled.gen.h"

class Prop2DData;
class Prop2DMesher;
class TexturePacker;

class Prop2DDataEntry : public Resource {
	GDCLASS(Prop2DDataEntry, Resource);

public:
	Transform get_transform() const;
	void set_transform(const Transform &value);

	Transform2D get_transform_2d() const;
	void set_transform_2d(const Transform2D &value);

	int get_z_index() const;
	void set_z_index(const int value);

	bool get_z_as_relative() const;
	void set_z_as_relative(const bool value);

	bool get_visible() const;
	void set_visible(const bool value);

	Color get_modulate() const;
	void set_modulate(const Color &value);

	Color get_self_modulate() const;
	void set_self_modulate(const Color &value);

	bool get_show_behind_parent() const;
	void set_show_behind_parent(const bool value);

	int get_light_mask() const;
	void set_light_mask(const int value);

	Ref<Material> get_material() const;
	void set_material(const Ref<Material> &value);

	bool get_use_parent_material() const;
	void set_use_parent_material(const bool value);

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	void add_textures_into(Ref<TexturePacker> texture_packer);
#endif

	bool processor_handles(Node *node);
	void processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform);
	Node *processor_get_node_for(const Transform2D &transform);
	bool processor_evaluate_children();

	virtual bool _processor_handles(Node *node);
	virtual void _processor_process(Ref<Prop2DData> prop_data, Node *node, const Transform2D &transform, Ref<Prop2DDataEntry> entry = Ref<Prop2DDataEntry>());
	virtual Node *_processor_get_node_for(const Transform2D &transform, Node *node = nullptr);
	virtual bool _processor_evaluate_children();

	Prop2DDataEntry();
	~Prop2DDataEntry();

protected:
	static void _bind_methods();

private:
	Transform2D _transform;
	int _z_index;
	bool _z_as_relative;

	bool _visible;
	Color _modulate;
	Color _self_modulate;
	bool _show_behind_parent;

	int _light_mask;

	Ref<Material> _material;
	bool _use_parent_material;
};

#endif
