

#include "spell_follow_projectile_3d.h"

#include "../../infos/spell_cast_info.h"

void SpellFollowProjectile3D::setup_projectile(Ref<SpellCastInfo> info) {
	_info = info;
}

SpellFollowProjectile3D::SpellFollowProjectile3D() {
}
SpellFollowProjectile3D::~SpellFollowProjectile3D() {
	_info.unref();
}

void SpellFollowProjectile3D::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("setup_projectile", "info"), &SpellFollowProjectile3D::setup_projectile);
}
