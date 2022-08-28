
#include "register_types.h"

#include "nodes/physical_bone_2d.h"
#include "nodes/skeleton_2d.h"

#include "resources/skeleton_modification_2d.h"
#include "resources/skeleton_modification_2d_ccdik.h"
#include "resources/skeleton_modification_2d_fabrik.h"
#include "resources/skeleton_modification_2d_jiggle.h"
#include "resources/skeleton_modification_2d_lookat.h"
#include "resources/skeleton_modification_2d_physicalbones.h"
#include "resources/skeleton_modification_2d_stackholder.h"
#include "resources/skeleton_modification_2d_twoboneik.h"
#include "resources/skeleton_modification_stack_2d.h"

#ifdef TOOLS_ENABLED
#include "editor/skeleton_2d_editor_plugin.h"
#endif

void register_skeleton_2d_types() {
	ClassDB::register_class<Skeleton2D>();
	ClassDB::register_class<Bone2D>();
	ClassDB::register_class<PhysicalBone2D>();

	ClassDB::register_class<SkeletonModificationStack2D>();
	ClassDB::register_class<SkeletonModification2D>();
	ClassDB::register_class<SkeletonModification2DLookAt>();
	ClassDB::register_class<SkeletonModification2DCCDIK>();
	ClassDB::register_class<SkeletonModification2DFABRIK>();
	ClassDB::register_class<SkeletonModification2DJiggle>();
	ClassDB::register_class<SkeletonModification2DTwoBoneIK>();
	ClassDB::register_class<SkeletonModification2DStackHolder>();

	ClassDB::register_class<PhysicalBone2D>();
	ClassDB::register_class<SkeletonModification2DPhysicalBones>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<Skeleton2DEditorPlugin>();
#endif
}

void unregister_skeleton_2d_types() {
}
