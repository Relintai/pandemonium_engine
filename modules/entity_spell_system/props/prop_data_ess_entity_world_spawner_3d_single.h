#ifndef PROP_DATA_ESS_ENTITY_WORLD_SPAWNER_3D_SINGLE_H
#define PROP_DATA_ESS_ENTITY_WORLD_SPAWNER_3D_SINGLE_H

/*************************************************************************/
/*  prop_data_ess_entity_world_spawner_3d_single.h                       */
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

#include "prop_data_ess_entity_world_spawner_3d.h"

class ESSEntityWorldSpawner3DSingle;
class EntityData;
class PropData;

class PropDataESSEntityWorldSpawner3DSingle : public PropDataESSEntityWorldSpawner3D {
	GDCLASS(PropDataESSEntityWorldSpawner3DSingle, PropDataESSEntityWorldSpawner3D);

public:
	String get_entity_name() const;
	void set_entity_name(const String &p_name);

	Ref<EntityData> get_entity_data() const;
	void set_entity_data(const Ref<EntityData> &p_data);

	int get_entity_level() const;
	void set_entity_level(const int p_level);

	float get_respawn_time_min() const;
	void set_respawn_time_min(const float p_respawn_time);

	float get_respawn_time_max() const;
	void set_respawn_time_max(const float p_respawn_time);

	bool _processor_handles(Node *node);
	void _processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform);
	Node *_processor_get_node_for(const Transform &transform);

	PropDataESSEntityWorldSpawner3DSingle();
	~PropDataESSEntityWorldSpawner3DSingle();

protected:
	static void _bind_methods();

private:
	String _entity_name;
	Ref<EntityData> _entity_data;
	int _entity_level;
	float _respawn_time_min;
	float _respawn_time_max;
	real_t _respawn_timer;
};

#endif
