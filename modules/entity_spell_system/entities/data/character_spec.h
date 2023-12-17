#ifndef CHARACTER_SPEC_H
#define CHARACTER_SPEC_H


#include "core/object/resource.h"
#include "core/string/ustring.h"
#include "core/containers/vector.h"

class Spell;

class CharacterSpec : public Resource {
	GDCLASS(CharacterSpec, Resource);

public:
	int get_id() const;
	void set_id(const int value);

	int get_num_rows() const;
	void set_num_rows(const int value);

	int get_num_columns(const int row) const;
	void set_num_columns(const int row, const int value);

	int get_num_ranks(const int row, const int column) const;
	void set_num_ranks(const int row, const int column, const int value);

	Vector<Variant> get_talents();

	Ref<Spell> get_talent(const int row, const int column, const int rank);
	void set_talent(const int row, const int column, const int rank, const Ref<Spell> &talent);

	bool has_talent_with_id(const int id);
	Ref<Spell> get_talent_with_id(const int id);

	CharacterSpec();
	~CharacterSpec();

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;
	static void _bind_methods();

private:
	int _id;
	Vector<Vector<Vector<Ref<Spell>>>> _rows;
};

#endif
