#ifndef PROP_MESHER_JOB_STEP_H
#define PROP_MESHER_JOB_STEP_H

/*************************************************************************/
/*  prop_mesher_job_step.h                                               */
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

#include "core/object/reference.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_MESH_UTILS_ENABLED
#include "../../mesh_utils/fast_quadratic_mesh_simplifier.h"
#endif

class PropMesherJobStep : public Reference {
	GDCLASS(PropMesherJobStep, Reference);

public:
	//todo add:
	//type generate lighting,
	//type skip (this would leave the mesh empty)
	//type previous mesh (this would set the previous mesh's rid to the current lod level)
	enum PropMesherJobStepType {
		TYPE_NORMAL = 0,
		TYPE_NORMAL_LOD,
		TYPE_DROP_UV2,
		TYPE_MERGE_VERTS,
		TYPE_BAKE_TEXTURE,
		TYPE_SIMPLIFY_MESH,
		TYPE_OTHER,
	};

	static const String BINDING_STRING_PROP_MESHER_JOB_STEP_TYPE;

	PropMesherJobStepType get_job_type() const;
	void set_job_type(const PropMesherJobStepType value);

	int get_lod_index() const;
	void set_lod_index(const int value);

#ifdef MODULE_MESH_UTILS_ENABLED
	Ref<FastQuadraticMeshSimplifier> get_fqms();
	void set_fqms(const Ref<FastQuadraticMeshSimplifier> &val);

	float get_simplification_step_ratio() const;
	void set_simplification_step_ratio(const float value);

	int get_simplification_steps() const;
	void set_simplification_steps(const int value);

	float get_simplification_agressiveness() const;
	void set_simplification_agressiveness(const float value);
#endif

	PropMesherJobStep();
	~PropMesherJobStep();

protected:
	static void _bind_methods();

	PropMesherJobStepType _job_type;
	int _lod_index;

#ifdef MODULE_MESH_UTILS_ENABLED
	Ref<FastQuadraticMeshSimplifier> _fqms;
	float _simplification_step_ratio;
	int _simplification_steps;
	float _simplification_agressiveness;
#endif
};

VARIANT_ENUM_CAST(PropMesherJobStep::PropMesherJobStepType);

#endif
