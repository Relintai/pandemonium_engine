/*************************************************************************/
/*  gltf_skeleton.cpp                                                    */
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

#include "gltf_skeleton.h"

#include "../gltf_template_convert.h"

#ifdef MODULE_SKELETON_3D_ENABLED
#include "modules/skeleton_3d/nodes/bone_attachment.h"
#include "modules/skeleton_3d/nodes/skeleton.h"
#endif

void GLTFSkeleton::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_joints"), &GLTFSkeleton::get_joints);
	ClassDB::bind_method(D_METHOD("set_joints", "joints"), &GLTFSkeleton::set_joints);
	ClassDB::bind_method(D_METHOD("get_roots"), &GLTFSkeleton::get_roots);
	ClassDB::bind_method(D_METHOD("set_roots", "roots"), &GLTFSkeleton::set_roots);

#ifdef MODULE_SKELETON_3D_ENABLED
	ClassDB::bind_method(D_METHOD("get_pandemonium_skeleton"), &GLTFSkeleton::get_pandemonium_skeleton);
	ClassDB::bind_method(D_METHOD("get_bone_attachment", "idx"), &GLTFSkeleton::get_bone_attachment);
#endif

	ClassDB::bind_method(D_METHOD("get_unique_names"), &GLTFSkeleton::get_unique_names);
	ClassDB::bind_method(D_METHOD("set_unique_names", "unique_names"), &GLTFSkeleton::set_unique_names);
	ClassDB::bind_method(D_METHOD("get_pandemonium_bone_node"), &GLTFSkeleton::get_pandemonium_bone_node);
	ClassDB::bind_method(D_METHOD("set_pandemonium_bone_node", "pandemonium_bone_node"), &GLTFSkeleton::set_pandemonium_bone_node);
	ClassDB::bind_method(D_METHOD("get_bone_attachment_count"), &GLTFSkeleton::get_bone_attachment_count);

	ADD_PROPERTY(PropertyInfo(Variant::POOL_INT_ARRAY, "joints"), "set_joints", "get_joints"); // PoolVector<GLTFNodeIndex>
	ADD_PROPERTY(PropertyInfo(Variant::POOL_INT_ARRAY, "roots"), "set_roots", "get_roots"); // PoolVector<GLTFNodeIndex>
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "unique_names", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_EDITOR), "set_unique_names", "get_unique_names"); // RBSet<String>
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "pandemonium_bone_node", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL | PROPERTY_USAGE_EDITOR), "set_pandemonium_bone_node", "get_pandemonium_bone_node"); // RBMap<int32_t,
}

PoolVector<GLTFNodeIndex> GLTFSkeleton::get_joints() {
	return joints;
}

void GLTFSkeleton::set_joints(PoolVector<GLTFNodeIndex> p_joints) {
	joints = p_joints;
}

PoolVector<GLTFNodeIndex> GLTFSkeleton::get_roots() {
	return roots;
}

void GLTFSkeleton::set_roots(PoolVector<GLTFNodeIndex> p_roots) {
	roots = p_roots;
}

#ifdef MODULE_SKELETON_3D_ENABLED
Skeleton *GLTFSkeleton::get_pandemonium_skeleton() {
	return pandemonium_skeleton;
}
BoneAttachment *GLTFSkeleton::get_bone_attachment(int idx) {
	ERR_FAIL_INDEX_V(idx, bone_attachments.size(), nullptr);
	return bone_attachments[idx];
}
#endif

Array GLTFSkeleton::get_unique_names() {
	return GLTFTemplateConvert::to_array(unique_names);
}

void GLTFSkeleton::set_unique_names(Array p_unique_names) {
	GLTFTemplateConvert::set_from_array(unique_names, p_unique_names);
}

Dictionary GLTFSkeleton::get_pandemonium_bone_node() {
	return GLTFTemplateConvert::to_dict(pandemonium_bone_node);
}

void GLTFSkeleton::set_pandemonium_bone_node(Dictionary p_indict) {
	GLTFTemplateConvert::set_from_dict(pandemonium_bone_node, p_indict);
}

int32_t GLTFSkeleton::get_bone_attachment_count() {
#ifdef MODULE_SKELETON_3D_ENABLED
	return bone_attachments.size();
#else
	return 0;
#endif
}
