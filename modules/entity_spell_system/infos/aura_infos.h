#ifndef AURA_INFOS_H
#define AURA_INFOS_H


#include "core/object/reference.h"

#include "../entities/entity.h"

class Entity;
class Spell;

class AuraApplyInfo : public Reference {
	GDCLASS(AuraApplyInfo, Reference);

public:
	Entity *caster_get() const;
	void caster_set(Entity *caster);
	void caster_set_bind(Node *caster);

	Entity *target_get() const;
	void target_set(Entity *caster);
	void target_set_bind(Node *caster);

	float spell_scale_get() const;
	void spell_scale_set(float value);

	Ref<Spell> get_aura() const;
	void set_aura(Ref<Spell> aura);

	AuraApplyInfo();
	AuraApplyInfo(Entity *_caster, Entity *_target, float _spell_scale, Spell *_aura);
	AuraApplyInfo(Entity *_caster, Entity *_target, float _spell_scale);

protected:
	static void _bind_methods();

private:
	Entity *_caster;
	Entity *_target;
	float _spell_scale;
	Spell *_aura;
};

#endif
