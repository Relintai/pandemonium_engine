#ifndef SKELETON_MODIFICATION_3D_H
#define SKELETON_MODIFICATION_3D_H

/*************************************************************************/
/*  skeleton_modification_3d.h                                           */
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

#include "../nodes/skeleton.h"
#include "core/object/reference.h"

class SkeletonModificationStack3D;

class SkeletonModification3D : public Resource {
	GDCLASS(SkeletonModification3D, Resource);
	friend class Skeleton;
	friend class SkeletonModificationStack3D;

protected:
	static void _bind_methods();

	SkeletonModificationStack3D *stack;
	int execution_mode; // 0 = process

	bool enabled;
	bool is_setup;
	bool execution_error_found;

	bool _print_execution_error(bool p_condition, String p_message);

public:
	real_t clamp_angle(real_t p_angle, real_t p_min_bound, real_t p_max_bound, bool p_invert);

	void set_enabled(bool p_enabled);
	bool get_enabled();

	void set_execution_mode(int p_mode);
	int get_execution_mode() const;

	Ref<SkeletonModificationStack3D> get_modification_stack();

	void set_is_setup(bool p_setup);
	bool get_is_setup() const;

	void execute(real_t p_delta);
	void setup_modification(Ref<SkeletonModificationStack3D> p_stack);

	virtual void _execute(real_t p_delta);
	virtual void _setup_modification(Ref<SkeletonModificationStack3D> p_stack);

	SkeletonModification3D();
};

#endif // SKELETON_MODIFICATION_3D_H
