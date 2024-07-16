#ifndef VOXEL_LIGHT_BAKER_H
#define VOXEL_LIGHT_BAKER_H

/*************************************************************************/
/*  voxel_light_baker.h                                                  */
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

#include "scene/3d/mesh_instance.h"
#include "scene/resources/mesh/multimesh.h"

class Material;

class VoxelLightBaker {
public:
	enum DebugMode {
		DEBUG_ALBEDO,
		DEBUG_LIGHT
	};

	enum BakeQuality {
		BAKE_QUALITY_LOW,
		BAKE_QUALITY_MEDIUM,
		BAKE_QUALITY_HIGH
	};

	enum BakeMode {
		BAKE_MODE_CONE_TRACE,
		BAKE_MODE_RAY_TRACE,
	};

private:
	enum {
		CHILD_EMPTY = 0xFFFFFFFF

	};

	struct Cell {
		uint32_t children[8];
		float albedo[3]; //albedo in RGB24
		float emission[3]; //accumulated light in 16:16 fixed point (needs to be integer for moving lights fast)
		float normal[3];
		uint32_t used_sides;
		float alpha; //used for upsampling
		int level;

		Cell() {
			for (int i = 0; i < 8; i++) {
				children[i] = CHILD_EMPTY;
			}

			for (int i = 0; i < 3; i++) {
				emission[i] = 0;
				albedo[i] = 0;
				normal[i] = 0;
			}
			alpha = 0;
			used_sides = 0;
			level = 0;
		}
	};

	Vector<Cell> bake_cells;
	int cell_subdiv;

	struct Light {
		int x, y, z;
		float accum[6][3]; //rgb anisotropic
		float direct_accum[6][3]; //for direct bake
		int next_leaf;
		Light() {
			x = y = z = 0;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 3; j++) {
					accum[i][j] = 0;
					direct_accum[i][j] = 0;
				}
			}
			next_leaf = 0;
		}
	};

	int first_leaf;

	Vector<Light> bake_light;

	struct MaterialCache {
		//128x128 textures
		Vector<Color> albedo;
		Vector<Color> emission;
	};

	RBMap<Ref<Material>, MaterialCache> material_cache;
	int leaf_voxel_count;
	bool direct_lights_baked;

	AABB original_bounds;
	AABB po2_bounds;
	int axis_cell_size[3];

	Transform to_cell_space;

	int color_scan_cell_width;
	int bake_texture_size;
	float cell_size;
	float propagation;

	BakeQuality bake_quality;

	int max_original_cells;

	void _init_light_plot(int p_idx, int p_level, int p_x, int p_y, int p_z, uint32_t p_parent);

	Vector<Color> _get_bake_texture(Ref<Image> p_image, const Color &p_color_mul, const Color &p_color_add);
	MaterialCache _get_material_cache(Ref<Material> p_material);

	void _plot_face(int p_idx, int p_level, int p_x, int p_y, int p_z, const Vector3 *p_vtx, const Vector3 *p_normal, const Vector2 *p_uv, const MaterialCache &p_material, const AABB &p_aabb);
	void _fixup_plot(int p_idx, int p_level);
	void _debug_mesh(int p_idx, int p_level, const AABB &p_aabb, Ref<MultiMesh> &p_multimesh, int &idx, DebugMode p_mode);
	void _check_init_light();

	uint32_t _find_cell_at_pos(const Cell *cells, int x, int y, int z);

public:
	void begin_bake(int p_subdiv, const AABB &p_bounds);
	void plot_mesh(const Transform &p_xform, Ref<Mesh> &p_mesh, const Vector<Ref<Material>> &p_materials, const Ref<Material> &p_override_material);
	void begin_bake_light(BakeQuality p_quality = BAKE_QUALITY_MEDIUM, float p_propagation = 0.85);
	void plot_light_directional(const Vector3 &p_direction, const Color &p_color, float p_energy, float p_indirect_energy, bool p_direct);
	void plot_light_omni(const Vector3 &p_pos, const Color &p_color, float p_energy, float p_indirect_energy, float p_radius, float p_attenutation, bool p_direct);
	void plot_light_spot(const Vector3 &p_pos, const Vector3 &p_axis, const Color &p_color, float p_energy, float p_indirect_energy, float p_radius, float p_attenutation, float p_spot_angle, float p_spot_attenuation, bool p_direct);
	void end_bake();

	struct LightMapData {
		int width;
		int height;
		PoolVector<float> light;
	};

	PoolVector<int> create_gi_probe_data();
	Ref<MultiMesh> create_debug_multimesh(DebugMode p_mode = DEBUG_ALBEDO);
	PoolVector<uint8_t> create_capture_octree(int p_subdiv);

	float get_cell_size() const;
	Transform get_to_cell_space_xform() const;
	VoxelLightBaker();
};

#endif // VOXEL_LIGHT_BAKER_H
