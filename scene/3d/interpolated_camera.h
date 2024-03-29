#ifndef INTERPOLATED_CAMERA_H
#define INTERPOLATED_CAMERA_H

/*************************************************************************/
/*  interpolated_camera.h                                                */
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

#include "scene/3d/camera.h"

class InterpolatedCamera : public Camera {
	GDCLASS(InterpolatedCamera, Camera);

public:
	enum InterpolatedCameraProcessMode {
		INTERPOLATED_CAMERA_PROCESS_PHYSICS,
		INTERPOLATED_CAMERA_PROCESS_IDLE
	};

private:
	bool enabled;
	real_t speed;
	NodePath target;
	InterpolatedCameraProcessMode process_mode;

	void _update_process_mode();

protected:
	void _notification(int p_what);
	static void _bind_methods();
	void _set_target(const Object *p_target);

public:
	void set_target(const Spatial *p_target);
	void set_target_path(const NodePath &p_path);
	NodePath get_target_path() const;

	void set_speed(real_t p_speed);
	real_t get_speed() const;

	void set_interpolation_enabled(bool p_enable);
	bool is_interpolation_enabled() const;

	void set_process_mode(InterpolatedCameraProcessMode p_mode);
	InterpolatedCameraProcessMode get_process_mode() const;

	InterpolatedCamera();
};

VARIANT_ENUM_CAST(InterpolatedCamera::InterpolatedCameraProcessMode);

#endif // INTERPOLATED_CAMERA_H
