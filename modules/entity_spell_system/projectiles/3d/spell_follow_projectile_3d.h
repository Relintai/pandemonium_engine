#ifndef SPELL_PROJECTILE_3D
#define SPELL_PROJECTILE_3D


#include "../../defines.h"

#include "scene/main/spatial.h"

class SpellCastInfo;

class SpellFollowProjectile3D : public Spatial {
	GDCLASS(SpellFollowProjectile3D, Spatial);

public:
	void setup_projectile(Ref<SpellCastInfo> info);

	SpellFollowProjectile3D();
	~SpellFollowProjectile3D();

protected:
	static void _bind_methods();

private:
	Ref<SpellCastInfo> _info;
};

#endif
