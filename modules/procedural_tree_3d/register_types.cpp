
#include "register_types.h"

#include "procedural_tree_3d_mesh.h"

void register_procedural_tree_3d_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
		ClassDB::register_class<ProceduralTreeMesh >();
	}
}

void unregister_procedural_tree_3d_types(ModuleRegistrationLevel p_level) {
}
