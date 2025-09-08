#ifndef MULTIMESH_H
#define MULTIMESH_H

/*************************************************************************/
/*  multimesh.h                                                          */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "scene/resources/mesh/mesh.h"
#include "servers/rendering_server.h"

class MultiMesh : public Resource {
	GDCLASS(MultiMesh, Resource);
	RES_BASE_EXTENSION("multimesh");

public:
	enum TransformFormat {
		TRANSFORM_2D = RS::MULTIMESH_TRANSFORM_2D,
		TRANSFORM_3D = RS::MULTIMESH_TRANSFORM_3D
	};

	enum ColorFormat {
		COLOR_NONE = RS::MULTIMESH_COLOR_NONE,
		COLOR_8BIT = RS::MULTIMESH_COLOR_8BIT,
		COLOR_FLOAT = RS::MULTIMESH_COLOR_FLOAT,
	};

	enum CustomDataFormat {
		CUSTOM_DATA_NONE,
		CUSTOM_DATA_8BIT,
		CUSTOM_DATA_FLOAT,
	};

	enum PhysicsInterpolationQuality {
		INTERP_QUALITY_FAST,
		INTERP_QUALITY_HIGH,
	};

private:
	Ref<Mesh> mesh;
	RID multimesh;
	TransformFormat transform_format;
	ColorFormat color_format;
	CustomDataFormat custom_data_format;
	int instance_count;
	int visible_instance_count;
	PhysicsInterpolationQuality _physics_interpolation_quality;

protected:
	static void _bind_methods();

	void _set_transform_array(const PoolVector<Vector3> &p_array);
	PoolVector<Vector3> _get_transform_array() const;

	void _set_transform_2d_array(const PoolVector<Vector2> &p_array);
	PoolVector<Vector2> _get_transform_2d_array() const;

	void _set_color_array(const PoolVector<Color> &p_array);
	PoolVector<Color> _get_color_array() const;

	void _set_custom_data_array(const PoolVector<Color> &p_array);
	PoolVector<Color> _get_custom_data_array() const;

public:
	void set_mesh(const Ref<Mesh> &p_mesh);
	Ref<Mesh> get_mesh() const;

	void set_color_format(ColorFormat p_color_format);
	ColorFormat get_color_format() const;

	void set_custom_data_format(CustomDataFormat p_custom_data_format);
	CustomDataFormat get_custom_data_format() const;

	void set_transform_format(TransformFormat p_transform_format);
	TransformFormat get_transform_format() const;

	void set_instance_count(int p_count);
	int get_instance_count() const;

	void set_visible_instance_count(int p_count);
	int get_visible_instance_count() const;

	void set_physics_interpolation_quality(PhysicsInterpolationQuality p_quality);
	PhysicsInterpolationQuality get_physics_interpolation_quality() const { return _physics_interpolation_quality; }

	void set_instance_transform(int p_instance, const Transform &p_transform);
	void set_instance_transform_2d(int p_instance, const Transform2D &p_transform);
	Transform get_instance_transform(int p_instance) const;
	Transform2D get_instance_transform_2d(int p_instance) const;

	void set_instance_color(int p_instance, const Color &p_color);
	Color get_instance_color(int p_instance) const;

	void set_instance_custom_data(int p_instance, const Color &p_custom_data);
	Color get_instance_custom_data(int p_instance) const;

	void reset_instance_physics_interpolation(int p_instance);
	void reset_instances_physics_interpolation();

	void set_as_bulk_array(const PoolVector<float> &p_array);
	void set_as_bulk_array_interpolated(const PoolVector<float> &p_array_curr, const PoolVector<float> &p_array_prev);

	void set_physics_interpolated(bool p_interpolated);

	virtual AABB get_aabb() const;

	virtual RID get_rid() const;

	MultiMesh();
	~MultiMesh();
};

VARIANT_ENUM_CAST(MultiMesh::TransformFormat);
VARIANT_ENUM_CAST(MultiMesh::ColorFormat);
VARIANT_ENUM_CAST(MultiMesh::CustomDataFormat);
VARIANT_ENUM_CAST(MultiMesh::PhysicsInterpolationQuality);

#endif // MULTI_MESH_H
