#ifndef ENTITY_DATA_H
#define ENTITY_DATA_H

/*************************************************************************/
/*  entity_data.h                                                        */
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

#include "core/containers/vector.h"
#include "core/object/resource.h"
#include "core/string/ustring.h"

#include "scene/resources/texture.h"

#include "../../entity_enums.h"

#include "../../entities/auras/aura_data.h"
#include "../../pipelines/spell_damage_info.h"
#include "../../pipelines/spell_heal_info.h"

#include "../../data/loot/loot_data_base.h"

#include "entity_class_data.h"

#include "../../data/items/equipment_data.h"

#include "../ai/entity_ai.h"

class Spell;
class Entity;
class CharacterSpec;
class Entity;
class SpellCastInfo;
class AIAction;
class CraftRecipe;
class EntitySpeciesData;
class SpeciesInstance;

class EntityData : public Resource {
	GDCLASS(EntityData, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	String get_text_description() const;
	void set_text_description(const String value);

	EntityEnums::EntityInteractionType get_entity_interaction_type() const;
	void set_entity_interaction_type(const EntityEnums::EntityInteractionType value);

	bool get_is_playable() const;
	void set_is_playable(const bool value);

	int get_immunity_flags() const;
	void set_immunity_flags(const int value);

	int get_entity_flags() const;
	void set_entity_flags(const int value);

	int get_money() const;
	void set_money(const int value);

	int get_bag_size() const;
	void set_bag_size(const int value);

	Ref<EntitySpeciesData> get_entity_species_data() const;
	void set_entity_species_data(const Ref<EntitySpeciesData> &value);

	Ref<EntityClassData> get_entity_class_data() const;
	void set_entity_class_data(const Ref<EntityClassData> &data);

	Ref<EquipmentData> get_equipment_data() const;
	void set_equipment_data(const Ref<EquipmentData> &data);

	Ref<SpeciesInstance> get_species_instance() const;
	void set_species_instance(const Ref<SpeciesInstance> &value);

	Ref<EntityAI> get_ai() const;
	void set_ai(const Ref<EntityAI> &ai);
	Ref<EntityAI> get_ai_instance();
	Ref<EntityAI> _get_ai_instance();

	Ref<LootDataBase> get_loot_db() const;
	void set_loot_db(const Ref<LootDataBase> &data);

	String generate_name();

	//Setup
	void setup_resources(Entity *entity);

	////    Interactions    ////
	bool cans_interact(Entity *entity);
	bool cans_interact_bind(Node *entity);

	void sinteract(Entity *entity);
	void sinteract_bind(Node *entity);

	EntityData();
	~EntityData();

protected:
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	int _id;

	EntityEnums::EntityInteractionType _interaction_type;

	bool _is_playable;

	int _immunity_flags;
	int _entity_flags;

	String _text_description;

	int _money;
	int _bag_size;

	Ref<EntityClassData> _entity_class_data;
	Ref<EntitySpeciesData> _entity_species_data;
	Ref<EquipmentData> _equipment_data;
	Ref<SpeciesInstance> _species_instance;

	Ref<EntityAI> _ai;

	Ref<LootDataBase> _lootdb;
};

#endif
