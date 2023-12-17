#ifndef ENTITY_SPECIES_DATA_H
#define ENTITY_SPECIES_DATA_H


#include "core/object/resource.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

#include "../../entity_enums.h"

#include "species_model_data.h"

class Spell;

class EntitySpeciesData : public Resource {
	GDCLASS(EntitySpeciesData, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	int get_type() const;
	void set_type(const int value);

	String get_text_description() const;
	void set_text_description(const String &value);

	//ModelData
	Ref<SpeciesModelData> get_model_data(const int index) const;
	void set_model_data(const int index, const Ref<SpeciesModelData> &data);
	void add_model_data(const Ref<SpeciesModelData> &data);
	void remove_model_data(const int index);

	int get_model_data_count() const;

	Vector<Variant> get_model_datas();
	void set_model_datas(const Vector<Variant> &data);

	//Spells
	Ref<Spell> get_spell(const int index) const;
	void set_spell(const int index, const Ref<Spell> &spell);
	void add_spell(const Ref<Spell> &spell);
	void remove_spell(const int index);

	int get_spell_count() const;

	Vector<Variant> get_spells();
	void set_spells(const Vector<Variant> &spells);

	//Auras
	Ref<Spell> get_aura(const int index) const;
	void set_aura(const int index, const Ref<Spell> &aura);
	void add_aura(const Ref<Spell> &aura);
	void remove_aura(const int index);

	int get_aura_count() const;

	Vector<Variant> get_auras();
	void set_auras(const Vector<Variant> &auras);

	String generate_name(int seed);

	EntitySpeciesData();
	~EntitySpeciesData();

protected:
	void _validate_property(PropertyInfo &property) const;
	static void _bind_methods();

private:
	int _id;
	int _type;
	String _text_description;

	Vector<Ref<SpeciesModelData>> _model_datas;
	Vector<Ref<Spell>> _spells;
	Vector<Ref<Spell>> _auras;
};

#endif
