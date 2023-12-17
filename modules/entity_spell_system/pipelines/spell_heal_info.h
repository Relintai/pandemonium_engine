#ifndef SPELL_HEAL_INFO_H
#define SPELL_HEAL_INFO_H


#include "core/object/reference.h"

#include "../spell_enums.h"
#include "scene/main/node.h"

class Entity;
class Spell;

class SpellHealInfo : public Reference {
	GDCLASS(SpellHealInfo, Reference);

public:
	enum HealSourceType {
		HEAL_SOURCE_UNKNOWN = 0,
		HEAL_SOURCE_SPELL,
		HEAL_SOURCE_AURA,
	};

protected:
	static void _bind_methods();

public:
	int heal_get();
	void heal_set(int value);

	bool get_immune() const;
	void set_immune(const bool value);

	int damage_get() const;
	void damage_set(const int value);

	bool crit_get() const;
	void crit_set(const bool value);

	int amount_absorbed_get() const;
	void amount_absorbed_set(const int value);

	int heal_type_get() const;
	void heal_type_set(const int value);

	Entity *dealer_get();
	void dealer_set(Entity *value);
	void dealer_set_bind(Node *value);

	Entity *receiver_get();
	void receiver_set(Entity *value);
	void receiver_set_bind(Node *value);

	Ref<Reference> source_get();
	void source_set(Ref<Reference> value);

	Ref<Spell> spell_source_get();
	void spell_source_set(const Ref<Spell> &value);

	Ref<Spell> aura_source_get();
	void aura_source_set(const Ref<Spell> &value);

	int source_get_id() const;
	void source_set_id(const int value);

	int source_get_type() const;
	void source_set_type(const int value);

	void reset();

	void resolve_references(Node *owner);
	Dictionary to_dict();
	void from_dict(const Dictionary &dict);

	SpellHealInfo();
	~SpellHealInfo();

private:
	bool _immune;
	int _heal;
	int _original_heal;
	int _amount_absorbed;
	bool _crit;
	int _heal_type;

	int _heal_source_type;
	Ref<Reference> _heal_source;
	int _heal_source_id;

	Entity *_dealer;
	Entity *_receiver;

	NodePath _dealer_path;
	NodePath _receiver_path;
};

#endif
