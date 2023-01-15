#ifndef GLTF_SKELETON_H
#define GLTF_SKELETON_H
/*************************************************************************/
/*  gltf_skeleton.h                                                      */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
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

#include "core/object/resource.h"

#include "../gltf_defines.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_SKELETON_3D_ENABLED
class Skeleton;
class BoneAttachment;
#endif

class GLTFSkeleton : public Resource {
	GDCLASS(GLTFSkeleton, Resource);
	friend class GLTFDocument;

private:
	// The *synthesized* skeletons joints
	PoolVector<GLTFNodeIndex> joints;

	// The roots of the skeleton. If there are multiple, each root must have the
	// same parent (ie roots are siblings)
	PoolVector<GLTFNodeIndex> roots;

#ifdef MODULE_SKELETON_3D_ENABLED
	// The created Skeleton for the scene
	Skeleton *pandemonium_skeleton = nullptr;
#endif

	// Set of unique bone names for the skeleton
	RBSet<String> unique_names;

	RBMap<int32_t, GLTFNodeIndex> pandemonium_bone_node;

#ifdef MODULE_SKELETON_3D_ENABLED
	PoolVector<BoneAttachment *> bone_attachments;
#endif

protected:
	static void _bind_methods();

public:
	PoolVector<GLTFNodeIndex> get_joints();
	void set_joints(PoolVector<GLTFNodeIndex> p_joints);

	PoolVector<GLTFNodeIndex> get_roots();
	void set_roots(PoolVector<GLTFNodeIndex> p_roots);

#ifdef MODULE_SKELETON_3D_ENABLED
	Skeleton *get_pandemonium_skeleton();
	BoneAttachment *get_bone_attachment(int idx);
#endif

	Array get_unique_names();
	void set_unique_names(Array p_unique_names);

	Dictionary get_pandemonium_bone_node();
	void set_pandemonium_bone_node(Dictionary p_indict);

	int32_t get_bone_attachment_count();
};

#endif // GLTF_SKELETON_H
