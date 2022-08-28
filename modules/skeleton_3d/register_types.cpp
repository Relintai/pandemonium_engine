
#include "register_types.h"

#include "nodes/skeleton.h"
#include "resources/skin.h"

#include "nodes/bone_attachment.h"
#include "nodes/skeleton_ik.h"

#include "nodes/physical_bone.h"

#include "resources/skeleton_modification_3d.h"
#include "resources/skeleton_modification_3d_ccdik.h"
#include "resources/skeleton_modification_3d_fabrik.h"
#include "resources/skeleton_modification_3d_jiggle.h"
#include "resources/skeleton_modification_3d_lookat.h"
#include "resources/skeleton_modification_3d_stackholder.h"
#include "resources/skeleton_modification_3d_twoboneik.h"
#include "resources/skeleton_modification_stack_3d.h"

#ifdef TOOLS_ENABLED
#include "editor/skeleton_editor_plugin.h"
#include "editor/skeleton_ik_editor_plugin.h"
#include "editor/physical_bone_plugin.h"
#endif

void register_skeleton_3d_types() {
	ClassDB::register_class<Skeleton>();
	ClassDB::register_class<Skin>();
	ClassDB::register_class<SkinReference>();

	ClassDB::register_class<BoneAttachment>();

	ClassDB::register_class<SkeletonIK>();

	ClassDB::register_class<PhysicalBone>();

	ClassDB::register_class<SkeletonModificationStack3D>();
	ClassDB::register_class<SkeletonModification3D>();
	ClassDB::register_class<SkeletonModification3DLookAt>();
	ClassDB::register_class<SkeletonModification3DCCDIK>();
	ClassDB::register_class<SkeletonModification3DFABRIK>();
	ClassDB::register_class<SkeletonModification3DJiggle>();
	ClassDB::register_class<SkeletonModification3DTwoBoneIK>();
	ClassDB::register_class<SkeletonModification3DStackHolder>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<SkeletonEditorPlugin>();
	EditorPlugins::add_by_type<SkeletonIKEditorPlugin>();
	EditorPlugins::add_by_type<PhysicalBonePlugin>();
#endif
}

void unregister_skeleton_3d_types() {
}
