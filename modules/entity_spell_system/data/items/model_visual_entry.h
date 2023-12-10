#ifndef MODEL_VISUAL_ENTRY_H
#define MODEL_VISUAL_ENTRY_H
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

#include "core/math/color.h"
#include "core/object/resource.h"
#include "core/string/ustring.h"

#include "scene/resources/texture.h"

#include "../../entity_enums.h"
#include "../../item_enums.h"

#include "scene/resources/mesh/mesh.h"
#include "scene/resources/packed_scene.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
#include "../../../mesh_data_resource/mesh_data_resource.h"
#endif

class ModelVisualEntry : public Resource {
	GDCLASS(ModelVisualEntry, Resource);

public:
	enum ModenVisualEntryType {
		MODEL_VISUAL_ENTRY_TYPE_BONE = 0,
		MODEL_VISUAL_ENTRY_TYPE_ATTACHMENT
	};

	static const String BINDING_STRING_MODEL_VISUAL_ENTRY_TYPES;

public:
	ModelVisualEntry::ModenVisualEntryType get_type() const;
	void set_type(const ModelVisualEntry::ModenVisualEntryType type);

	int get_override_layer() const;
	void set_override_layer(const int layer);

	int get_entity_type() const;
	void set_entity_type(const int value);

	int get_bone() const;
	void set_bone(const int value);

	int get_group() const;
	void set_group(const int value);

#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
	Ref<MeshDataResource> get_mesh(const int index);
	void set_mesh(const int index, const Ref<MeshDataResource> &mesh);
#endif

	Ref<Texture> get_texture(const int index);
	void set_texture(const int index, const Ref<Texture> &texture);

	Color get_color(const int index) const;
	void set_color(const int index, const Color &color);

	Ref<PackedScene> get_attachment(const int index);
	void set_attachment(const int index, const Ref<PackedScene> &attachment);

	Transform get_transform(const int index) const;
	void set_transform(const int index, const Transform &transform);

	int get_size() const;
	void set_size(const int value);

	ModelVisualEntry();
	~ModelVisualEntry();

protected:
	struct MVEE {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
		Ref<MeshDataResource> mesh;
#endif
		Ref<Texture> texture;
		Color color;

		Ref<PackedScene> attachment;
		Transform transform;

		MVEE() {
			color = Color(1, 1, 1, 1);
		}

		~MVEE() {
#ifdef MODULE_MESH_DATA_RESOURCE_ENABLED
			mesh.unref();
#endif
			texture.unref();
			attachment.unref();
		}
	};

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	ModelVisualEntry::ModenVisualEntryType _type;
	int _override_layer;

	int _entity_type;
	int _bone;
	int _group;

	Vector<MVEE> _entries;
};

VARIANT_ENUM_CAST(ModelVisualEntry::ModenVisualEntryType);

#endif
