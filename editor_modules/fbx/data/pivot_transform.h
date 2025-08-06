/**************************************************************************/
/*  pivot_transform.h                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef PIVOT_TRANSFORM_H
#define PIVOT_TRANSFORM_H

#include "core/math/transform.h"
#include "core/reference.h"

#include "model_abstraction.h"

#include "fbx_parser/FBXDocument.h"
#include "tools/import_utils.h"

enum TransformationComp {
	TransformationComp_Translation,
	TransformationComp_Scaling,
	TransformationComp_Rotation,
	TransformationComp_RotationOffset,
	TransformationComp_RotationPivot,
	TransformationComp_PreRotation,
	TransformationComp_PostRotation,
	TransformationComp_ScalingOffset,
	TransformationComp_ScalingPivot,
	TransformationComp_GeometricTranslation,
	TransformationComp_GeometricRotation,
	TransformationComp_GeometricScaling,
	TransformationComp_MAXIMUM
};
// Abstract away pivot data so its simpler to handle
struct PivotTransform : Reference, ModelAbstraction {
	// at the end we want to keep geometric_ everything, post and pre rotation
	// these are used during animation data processing / keyframe ingestion the rest can be simplified down / out.
	Quat pre_rotation = Quat();
	Quat post_rotation = Quat();
	Quat rotation = Quat();
	Quat geometric_rotation = Quat();
	Vector3 rotation_pivot = Vector3();
	Vector3 rotation_offset = Vector3();
	Vector3 scaling_offset = Vector3(1.0, 1.0, 1.0);
	Vector3 scaling_pivot = Vector3(1.0, 1.0, 1.0);
	Vector3 translation = Vector3();
	Vector3 scaling = Vector3(1.0, 1.0, 1.0);
	Vector3 geometric_scaling = Vector3(1.0, 1.0, 1.0);
	Vector3 geometric_translation = Vector3();

	Vector3 raw_rotation = Vector3();
	Vector3 raw_post_rotation = Vector3();
	Vector3 raw_pre_rotation = Vector3();

	/* Read pivots from the document */
	void ReadTransformChain();

	void debug_pivot_xform(String p_name) {
		print_verbose("debugging node name: " + p_name);
		print_verbose("raw rotation: " + raw_rotation * (180 / Math_PI));
		print_verbose("raw pre_rotation " + raw_pre_rotation * (180 / Math_PI));
		print_verbose("raw post_rotation " + raw_post_rotation * (180 / Math_PI));
	}

	Transform ComputeGlobalTransform(Transform t) const;
	Transform ComputeLocalTransform(Transform t) const;
	Transform ComputeGlobalTransform(Vector3 p_translation, Quat p_rotation, Vector3 p_scaling) const;
	Transform ComputeLocalTransform(Vector3 p_translation, Quat p_rotation, Vector3 p_scaling) const;

	/* Extract into xforms and calculate once */
	void ComputePivotTransform();

	/* Execute the command for the pivot generation */
	void Execute();

	void set_parent(Ref<PivotTransform> p_parent) {
		parent_transform = p_parent;
	}

	bool computed_global_xform = false;
	Ref<PivotTransform> parent_transform = Ref<PivotTransform>();
	//Transform chain[TransformationComp_MAXIMUM];

	// cached for later use
	Transform GlobalTransform = Transform();
	Transform LocalTransform = Transform();
	Transform Local_Scaling_Matrix = Transform(); // used for inherit type.
	Transform GeometricTransform = Transform(); // 3DS max only
	FBXDocParser::TransformInheritance inherit_type = FBXDocParser::TransformInheritance_MAX; // maya fbx requires this - sorry <3
};

#endif // PIVOT_TRANSFORM_H
