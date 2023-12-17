

#include "terrain_2d_world_default.h"

#include "terrain_2d_chunk_default.h"

#include "../../defines.h"

#include "../../meshers/default/terrain_2d_mesher_default.h"
#include "../jobs/terrain_2d_light_job.h"
#include "../jobs/terrain_2d_prop_job.h"
#include "../jobs/terrain_2d_terrain_job.h"

_FORCE_INLINE_ int Terrain2DWorldDefault::get_build_flags() const {
	return _build_flags;
}
_FORCE_INLINE_ void Terrain2DWorldDefault::set_build_flags(const int flags) {
	_build_flags = flags;

	for (int i = 0; i < chunk_get_count(); ++i) {
		Ref<Terrain2DChunkDefault> c = chunk_get_index(i);

		if (!c.is_valid())
			continue;

		c->set_build_flags(_build_flags);
	}
}

PoolColorArray Terrain2DWorldDefault::get_vertex_colors(const Transform2D &transform, const PoolVector2Array &vertices, const float base_light_value, const float ao_strength) {
	PoolColorArray arr;
	arr.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i) {
		Vector2 v = transform.xform(vertices[i]);

		Vector2 pos = v;
		v.x /= get_cell_size_x();
		v.y /= get_cell_size_y();

		//Note: floor is needed to handle negative numbers proiberly
		int x = static_cast<int>(Math::floor(pos.x / get_chunk_size_x()));
		int y = static_cast<int>(Math::floor(pos.y / get_chunk_size_y()));

		int bx = static_cast<int>(Math::floor(pos.x)) % get_chunk_size_x();
		int by = static_cast<int>(Math::floor(pos.y)) % get_chunk_size_y();

		if (bx < 0) {
			bx += get_chunk_size_x();
		}

		if (by < 0) {
			by += get_chunk_size_y();
		}

		Ref<Terrain2DChunk> chunk = chunk_get(x, y);

		if (chunk.is_valid()) {
			Color light = Color(
					chunk->get_voxel(bx, by, Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_R) / 255.0,
					chunk->get_voxel(bx, by, Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_G) / 255.0,
					chunk->get_voxel(bx, by, Terrain2DChunkDefault::DEFAULT_CHANNEL_LIGHT_COLOR_B) / 255.0);

			float ao = (chunk->get_voxel(bx, by, Terrain2DChunkDefault::DEFAULT_CHANNEL_AO) / 255.0) * ao_strength;
			float rao = chunk->get_voxel(bx, by, Terrain2DChunkDefault::DEFAULT_CHANNEL_RANDOM_AO) / 255.0;

			ao += rao;

			light.r += base_light_value;
			light.g += base_light_value;
			light.b += base_light_value;

			light.r -= ao;
			light.g -= ao;
			light.b -= ao;

			light.r = CLAMP(light.r, 0, 1.0);
			light.g = CLAMP(light.g, 0, 1.0);
			light.b = CLAMP(light.b, 0, 1.0);

			arr.set(i, light);
		} else {
			arr.set(i, Color(base_light_value, base_light_value, base_light_value));
		}
	}

	return arr;
}

Ref<Terrain2DChunk> Terrain2DWorldDefault::_create_chunk(int x, int y, Ref<Terrain2DChunk> chunk) {
	if (!chunk.is_valid()) {
		chunk = Ref<Terrain2DChunk>(memnew(Terrain2DChunkDefault));
	}

	if (chunk->job_get_count() == 0) {
		Ref<Terrain2DTerrain2DJob> tj;
		tj.instance();

		Ref<Terrain2DLightJob> lj;
		lj.instance();

		Ref<Terrain2DProp2DJob> pj;
		pj.instance();
		pj->set_prop_mesher(Ref<Terrain2DMesher>(memnew(Terrain2DMesherDefault)));

		chunk->job_add(lj);
		chunk->job_add(tj);
		chunk->job_add(pj);
	}

	Ref<Terrain2DChunkDefault> vcd = chunk;

	if (vcd.is_valid()) {
		vcd->set_build_flags(_build_flags);
	}

	return Terrain2DWorld::_create_chunk(x, y, chunk);
}

void Terrain2DWorldDefault::_chunk_added(Ref<Terrain2DChunk> chunk) {
	Ref<Terrain2DChunkDefault> c = chunk;

	if (c.is_valid()) {
		c->set_build_flags(_build_flags);
	}
}

int Terrain2DWorldDefault::_get_channel_index_info(const Terrain2DWorld::ChannelTypeInfo channel_type) {
	switch (channel_type) {
		case CHANNEL_TYPE_INFO_TYPE:
			return Terrain2DChunkDefault::DEFAULT_CHANNEL_TYPE;
		case CHANNEL_TYPE_INFO_ISOLEVEL:
			return -1;
		case CHANNEL_TYPE_INFO_LIQUID_FLOW:
			return -1;
		default:
			return -1;
	}
}

Terrain2DWorldDefault::Terrain2DWorldDefault() {
	_build_flags = Terrain2DChunkDefault::BUILD_FLAG_CREATE_COLLIDER;

	set_data_margin_start(1);
	set_data_margin_end(1);
}

Terrain2DWorldDefault ::~Terrain2DWorldDefault() {
}

void Terrain2DWorldDefault::_notification(int p_what) {
	Terrain2DWorld::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_ACTIVE_STATE_CHANGED: {
			if (!is_inside_tree()) {
				return;
			}

			bool active = get_active();

			for (int i = 0; i < chunk_get_count(); ++i) {
				Ref<Terrain2DChunk> chunk = chunk_get_index(i);

				if (chunk.is_valid()) {
					chunk->set_visible(active);
				}
			}

			break;
		}
	}
}

/*
void Terrain2DWorldDefault::_notification(int p_what) {
	Terrain2DWorld::_notification(p_what);

	switch (p_what) {
		case NOTIFICATION_INTERNAL_PROCESS: {
			if (!get_player()) {
				return;
			}

			if (!ObjectDB::instance_validate(get_player())) {
				set_player(NULL);
				return;
			}

			_lod_update_timer += get_process_delta_time();

			if (_lod_update_timer >= _lod_update_interval) {
				_lod_update_timer = 0;

				update_lods();
			}
		} break;
	}
}*/

void Terrain2DWorldDefault::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_chunk_added", "chunk"), &Terrain2DWorldDefault::_chunk_added);

	ClassDB::bind_method(D_METHOD("get_build_flags"), &Terrain2DWorldDefault::get_build_flags);
	ClassDB::bind_method(D_METHOD("set_build_flags", "value"), &Terrain2DWorldDefault::set_build_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "build_flags", PROPERTY_HINT_FLAGS, Terrain2DChunkDefault::BINDING_STRING_BUILD_FLAGS), "set_build_flags", "get_build_flags");

	ClassDB::bind_method(D_METHOD("get_vertex_colors", "transform", "vertices", "base_light_value", "ao_strength"), &Terrain2DWorldDefault::get_vertex_colors, DEFVAL(0.45), DEFVAL(0.2));
}
