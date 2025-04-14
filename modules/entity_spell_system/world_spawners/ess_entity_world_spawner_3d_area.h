#ifndef ESS_ENTITY_WORLD_SPAWNER_3D_AREA_H
#define ESS_ENTITY_WORLD_SPAWNER_3D_AREA_H

/*************************************************************************/
/*  ess_entity_world_spawner_3d_area.h                                   */
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

#include "ess_entity_world_spawner_3d.h"

class EntityData;

class ESSEntityWorldSpawner3DArea : public ESSEntityWorldSpawner3D {
	GDCLASS(ESSEntityWorldSpawner3DArea, ESSEntityWorldSpawner3D);

public:
	Vector3 get_spawn_area_extents() const;
	void set_spawn_area_extents(const Vector3 &p_extents);

	int get_spawn_slot_count() const;
	void set_spawn_slot_count(const int p_spawn_slot_count);

	float get_respawn_time_min() const;
	void set_respawn_time_min(const float p_respawn_time);

	float get_respawn_time_max() const;
	void set_respawn_time_max(const float p_respawn_time);

	int get_spawn_entry_count() const;
	void set_spawn_entry_count(const int p_spawn_entry_count);

	String get_entity_name(const int p_index) const;
	void set_entity_name(const int p_index, const String &p_name);

	Ref<EntityData> get_entity_data(const int p_index) const;
	void set_entity_data(const int p_index, const Ref<EntityData> &p_data);

	Vector2i get_entity_level_range(const int p_index) const;
	void set_entity_level_range(const int p_index, const Vector2i &p_level_range);

	float get_entity_spawn_chance(const int p_index) const;
	void set_entity_spawn_chance(const int p_index, const float p_spawn_chance);

	void clear_spawn_entries();
	void add_spawn_entry(const String &p_name, const Ref<EntityData> &p_data, const Vector2i &p_level_range, const float p_spawn_chance);
	void remove_spawn_entry(const int p_index);

	virtual void _spawn();

	ESSEntityWorldSpawner3DArea();
	~ESSEntityWorldSpawner3DArea();

	// For the gizmo
	Vector<Vector2> get_spawn_positions();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	void _notification(int p_what);
	static void _bind_methods();

	void _on_entity_tree_exited(const ObjectID p_entity_object_id);
	void _recalculate_slots();
	void _recalculate_valid_spawn_entries();
	void _spawn_slot(const int p_index);
	int generate_spawn_index();

	Vector3 _spawn_area_extents;
	int _spawn_slot_count;

	bool _spawn_slots_dirty;

	float _respawn_time_min;
	float _respawn_time_max;

	struct SpawnEntry {
		String entity_name;
		Ref<EntityData> entity_data;
		Vector2i level_range;
		float spawn_chance;

		SpawnEntry();
		~SpawnEntry();
	};

	Vector<SpawnEntry> _spawn_entries;
	Vector<SpawnEntry> _valid_spawn_entries;
	float _spawn_entries_max_chance;
	bool _spawn_entries_dirty;

	struct SpawnSlot {
		Vector2 position;
		float respawn_timer;
		ObjectID entity;

		SpawnSlot() {
			respawn_timer = 0;
			entity = ObjectID();
		}
	};

	Vector<SpawnSlot> _spawn_slots;
};

#endif
