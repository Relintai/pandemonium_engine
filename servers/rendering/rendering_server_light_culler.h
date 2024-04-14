#ifndef RENDERING_SERVER_LIGHT_CULLER_H
#define RENDERING_SERVER_LIGHT_CULLER_H

/*************************************************************************/
/*  rendering_server_light_culler.h                                      */
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

#include "core/math/plane.h"
#include "core/math/vector3.h"
#include "rendering_server_scene.h"

struct Projection;
struct Transform;

// For testing performance improvements from the LightCuller:
// Uncomment LIGHT_CULLER_DEBUG_FLASH and it will turn the culler
// on and off every LIGHT_CULLER_DEBUG_FLASH_FREQUENCY camera prepares.
// Uncomment LIGHT_CULLER_DEBUG_LOGGING to get periodic print of the number of casters culled before / after.

// #define LIGHT_CULLER_DEBUG_LOGGING
// #define LIGHT_CULLER_DEBUG_FLASH
#define LIGHT_CULLER_DEBUG_FLASH_FREQUENCY 1024
////////////////////////////////////////////////////////////////////////////////////////////////

// The code to generate the lookup table is included but commented out.
// This may be useful for debugging / regenerating the LUT in the future,
// especially if the order of planes changes.
// When this define is set, the generated lookup table will be printed to debug output.
// The generated lookup table can be copy pasted
// straight to LUT_entry_sizes and LUT_entries.
// See the referenced article for explanation.
// #define RENDERING_SERVER_LIGHT_CULLER_CALCULATE_LUT

////////////////////////////////////////////////////////////////////////////////////////////////
// This define will be set automatically depending on earlier defines, you can leave this as is.
#if defined(LIGHT_CULLER_DEBUG_LOGGING) || defined(RENDERING_SERVER_LIGHT_CULLER_CALCULATE_LUT)
#define RENDERING_SERVER_LIGHT_CULLER_DEBUG_STRINGS
#endif

// Culls shadow casters that can't cast shadows into the camera frustum.
class RenderingServerLightCuller {
public:
	RenderingServerLightCuller();

private:
	class LightSource {
	public:
		enum SourceType {
			ST_UNKNOWN,
			ST_DIRECTIONAL,
			ST_SPOTLIGHT,
			ST_OMNI,
		};

		LightSource() {
			type = ST_UNKNOWN;
			angle = 0.0f;
			range = FLT_MAX;
		}

		// All in world space, culling done in world space.
		Vector3 pos;
		Vector3 dir;
		SourceType type;

		float angle; // For spotlight.
		float range;
	};

	// Same order as godot.
	enum PlaneOrder {
		PLANE_NEAR,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_TOP,
		PLANE_RIGHT,
		PLANE_BOTTOM,
		PLANE_TOTAL,
	};

	// Same order as godot.
	enum PointOrder {
		PT_FAR_LEFT_TOP,
		PT_FAR_LEFT_BOTTOM,
		PT_FAR_RIGHT_TOP,
		PT_FAR_RIGHT_BOTTOM,
		PT_NEAR_LEFT_TOP,
		PT_NEAR_LEFT_BOTTOM,
		PT_NEAR_RIGHT_TOP,
		PT_NEAR_RIGHT_BOTTOM,
	};

	// 6 bits, 6 planes.
	enum {
		NUM_CAM_PLANES = 6,
		NUM_CAM_POINTS = 8,
		MAX_CULL_PLANES = 17,
		LUT_SIZE = 64,
	};

public:
	// Before each pass with a different camera, you must call this so the culler can pre-create
	// the camera frustum planes and corner points in world space which are used for the culling.
	bool prepare_camera(const Transform &p_cam_transform, const Projection &p_cam_matrix);

	// Returns false if the entire light is culled (i.e. there is no intersection between the light and the view frustum).
	bool prepare_light(const RenderingServerScene::Instance &p_instance);

	// Cull according to the planes that were setup in the previous call to prepare_light.
	int cull(int p_count, RenderingServerScene::Instance **p_result_array);

	// Can turn on and off from the engine if desired.
	void set_caster_culling_active(bool p_active) { data.caster_culling_active = p_active; }
	void set_light_culling_active(bool p_active) { data.light_culling_active = p_active; }

private:
	// Internal version uses LightSource.
	bool _add_light_camera_planes(const LightSource &p_light_source);

	// Directional light gives parallel culling planes (as opposed to point lights).
	bool add_light_camera_planes_directional(const LightSource &p_light_source);

	// Avoid adding extra culling planes derived from near colinear triangles.
	// The normals derived from these will be inaccurate, and can lead to false
	// culling of objects that should be within the light volume.
	bool _is_colinear_tri(const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_c) const {
		// Lengths of sides a, b and c.
		float la = (p_b - p_a).length();
		float lb = (p_c - p_b).length();
		float lc = (p_c - p_a).length();

		// Get longest side into lc.
		if (lb < la) {
			SWAP(la, lb);
		}
		if (lc < lb) {
			SWAP(lb, lc);
		}

		// Prevent divide by zero.
		if (lc > 0.00001f) {
			// If the summed length of the smaller two
			// sides is close to the length of the longest side,
			// the points are colinear, and the triangle is near degenerate.
			float ld = ((la + lb) - lc) / lc;

			// ld will be close to zero for colinear tris.
			return ld < 0.00001f;
		}

		// Don't create planes from tiny triangles,
		// they won't be accurate.
		return true;
	}

	// Is the light culler active? maybe not in the editor...
	bool is_caster_culling_active() const { return data.caster_culling_active; }
	bool is_light_culling_active() const { return data.light_culling_active; }

	// Do we want to log some debug output?
	bool is_logging() const { return data.debug_count == 0; }

	// Culling planes.
	void add_cull_plane(const Plane &p);

	struct Data {
		// Camera frustum planes (world space) - order ePlane.
		Vector<Plane> frustum_planes;

		// Camera frustum corners (world space) - order ePoint.
		Vector3 frustum_points[NUM_CAM_POINTS];

		// We are storing cull planes in a ye olde style array to prevent needless allocations.
		Plane cull_planes[MAX_CULL_PLANES];
		int num_cull_planes = 0;

		// The whole light can be out of range of the view frustum, in which case all casters should be culled.
		bool out_of_range = false;

#ifdef RENDERING_SERVER_LIGHT_CULLER_DEBUG_STRINGS
		static String plane_bitfield_to_string(unsigned int BF);
		// Names of the plane and point enums, useful for debugging.
		static const char *string_planes[];
		static const char *string_points[];
#endif

		// Precalculated look up table.
		static uint8_t LUT_entry_sizes[LUT_SIZE];
		static uint8_t LUT_entries[LUT_SIZE][8];

		bool caster_culling_active = true;
		bool light_culling_active = true;

		// Light culling is a basic on / off switch.
		// Caster culling only works if light culling is also on.
		bool is_active() const { return light_culling_active; }

		// Ideally a frame counter, but for ease of implementation
		// this is just incremented on each prepare_camera.
		// used to turn on and off debugging features.
		int debug_count = -1;
	} data;

	// This functionality is not required in general use (and is compiled out),
	// as the lookup table can normally be hard coded
	// (provided order of planes etc does not change).
	// It is provided for debugging / future maintenance.
#ifdef RENDERING_SERVER_LIGHT_CULLER_CALCULATE_LUT
	void get_neighbouring_planes(PlaneOrder p_plane, PlaneOrder r_neigh_planes[4]) const;
	void get_corners_of_planes(PlaneOrder p_plane_a, PlaneOrder p_plane_b, PointOrder r_points[2]) const;
	void create_LUT();
	void compact_LUT_entry(uint32_t p_entry_id);
	void debug_print_LUT();
	void debug_print_LUT_as_table();
	void add_LUT(int p_plane_0, int p_plane_1, PointOrder p_pts[2]);
	void add_LUT_entry(uint32_t p_entry_id, PointOrder p_pts[2]);
	String debug_string_LUT_entry(const LocalVector<uint8_t> &p_entry, bool p_pair = false);
	String string_LUT_entry(const LocalVector<uint8_t> &p_entry);

	// Contains a list of points for each combination of plane facing directions.
	LocalVector<uint8_t> _calculated_LUT[LUT_SIZE];
#endif
};

#endif // RENDERING_SERVER_LIGHT_CULLER_H
