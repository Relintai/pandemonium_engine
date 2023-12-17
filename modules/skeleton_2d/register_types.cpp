/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/
/*                         This file is part of:                         */
/*                          PANDEMONIUM ENGINE                           */
/*             https://github.com/Relintai/pandemonium_engine            */
/*************************************************************************/
/* Copyright (c) 2022-present Péter Magyar.                              */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

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

void register_skeleton_2d_types(ModuleRegistrationLevel p_level) {
	if (p_level == MODULE_REGISTRATION_LEVEL_SCENE) {
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
	}

#ifdef TOOLS_ENABLED
	if (p_level == MODULE_REGISTRATION_LEVEL_EDITOR) {
		EditorPlugins::add_by_type<Skeleton2DEditorPlugin>();
	}
#endif
}

void unregister_skeleton_2d_types(ModuleRegistrationLevel p_level) {
}
