

#include "prop_2d_texture_job.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#ifdef MODULE_TEXTURE_PACKER_ENABLED
Ref<TexturePacker> Prop2DTextureJob::get_merger() {
	return _merger;
}

void Prop2DTextureJob::set_merger(const Ref<TexturePacker> &merger) {
	_merger = merger;
}
#endif

void Prop2DTextureJob::_execute() {
#ifdef MODULE_TEXTURE_PACKER_ENABLED
	if (!_merger.is_valid()) {
		set_complete(true);
		return;
	}

	_merger->merge();
#endif

	set_complete(true);
}

Prop2DTextureJob::Prop2DTextureJob() {
}

Prop2DTextureJob::~Prop2DTextureJob() {
}

void Prop2DTextureJob::_bind_methods() {
#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("get_merger"), &Prop2DTextureJob::get_merger);
	ClassDB::bind_method(D_METHOD("set_merger", "value"), &Prop2DTextureJob::set_merger);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "merger", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker"), "set_merger", "get_merger");
#endif
}
