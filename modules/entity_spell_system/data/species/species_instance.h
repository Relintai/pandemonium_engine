#ifndef SPECIES_INSTANCE_H
#define SPECIES_INSTANCE_H


#include "core/object/resource.h"

#include "../items/model_visual_entry.h"

class EntitySpeciesData;

class SpeciesInstance : public Resource {
	GDCLASS(SpeciesInstance, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	int get_species_id() const;
	void set_species_id(const int value);

	StringName get_species_path() const;
	void set_species_path(const StringName &value);

	Ref<EntitySpeciesData> get_species();
	void set_species(const Ref<EntitySpeciesData> &value);

	int get_skin_color_index() const;
	void set_skin_color_index(const int value);

	int get_hair_style_index() const;
	void set_hair_style_index(const int value);

	int get_hair_color_index() const;
	void set_hair_color_index(const int value);

	int get_head_index() const;
	void set_head_index(const int value);

	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	Dictionary _to_dict();
	void _from_dict(const Dictionary &dict);

	SpeciesInstance();
	~SpeciesInstance();

protected:
	static void _bind_methods();

private:
	int _id;
	StringName _path;
	int _species_id;
	Ref<EntitySpeciesData> _species;
	int _skin_color_index;
	int _hair_style_index;
	int _hair_color_index;
	int _head_index;
};

#endif
