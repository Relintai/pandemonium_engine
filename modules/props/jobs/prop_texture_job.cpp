

#include "prop_texture_job.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#ifdef MODULE_TEXTURE_PACKER_ENABLED
Ref<TexturePacker> PropTextureJob::get_merger() {
	return _merger;
}

void PropTextureJob::set_merger(const Ref<TexturePacker> &merger) {
	_merger = merger;
}
#endif

void PropTextureJob::_execute() {
#ifdef MODULE_TEXTURE_PACKER_ENABLED
	if (!_merger.is_valid()) {
		set_complete(true);
		return;
	}

	_merger->merge();
#endif

	set_complete(true);
}

PropTextureJob::PropTextureJob() {
}

PropTextureJob::~PropTextureJob() {
}

void PropTextureJob::_bind_methods() {
#ifdef MODULE_TEXTURE_PACKER_ENABLED
	ClassDB::bind_method(D_METHOD("get_merger"), &PropTextureJob::get_merger);
	ClassDB::bind_method(D_METHOD("set_merger", "value"), &PropTextureJob::set_merger);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "merger", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker"), "set_merger", "get_merger");
#endif
}
