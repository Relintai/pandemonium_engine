

#include "register_types.h"

#include "texture_merger.h"
#include "texture_packer.h"

#include "texture_resource/packer_image_resource.h"

#include "layers/texture_layer_merger.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"

#include "texture_resource/editor_plugin_packer_image_resource.h"
#endif

void register_texture_packer_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<TexturePacker>();
		ClassDB::register_class<TextureMerger>();

		ClassDB::register_class<PackerImageResource>();

		ClassDB::register_class<TextureLayerMerger>();
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<EditorPluginPackerImageResource>();
	}
#endif
}

void unregister_texture_packer_types(ModuleRegistrationLevel p_level) {
}
