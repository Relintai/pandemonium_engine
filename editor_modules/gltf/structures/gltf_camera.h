#ifndef GLTF_CAMERA_H
#define GLTF_CAMERA_H

/*************************************************************************/
/*  gltf_camera.h                                                        */
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

#include "core/object/resource.h"

class Camera;

// Reference and test file:
// https://github.com/KhronosGroup/glTF-Tutorials/blob/master/gltfTutorial/gltfTutorial_015_SimpleCameras.md

class GLTFCamera : public Resource {
	GDCLASS(GLTFCamera, Resource);

private:
	// GLTF has no default camera values, they should always be specified in
	// the GLTF file. Here we default to Godot's default camera settings.
	bool perspective = true;
	real_t fov = Math::deg2rad(75.0);
	real_t size_mag = 0.5;
	real_t zfar = 4000.0;
	real_t znear = 0.05;

protected:
	static void _bind_methods();

public:
	bool get_perspective() const { return perspective; }
	void set_perspective(bool p_val) { perspective = p_val; }
	real_t get_fov_size() const { return fov; }
	void set_fov_size(real_t p_val) { fov = p_val; }
	real_t get_size_mag() const { return size_mag; }
	void set_size_mag(real_t p_val) { size_mag = p_val; }
	real_t get_zfar() const { return zfar; }
	void set_zfar(real_t p_val) { zfar = p_val; }
	real_t get_znear() const { return znear; }
	void set_znear(real_t p_val) { znear = p_val; }

	static Ref<GLTFCamera> from_node(const Camera *p_camera);
	Camera *to_node() const;

	static Ref<GLTFCamera> from_dictionary(const Dictionary p_dictionary);
	Dictionary to_dictionary() const;
};

#endif // GLTF_CAMERA_H
