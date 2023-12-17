#ifndef MATERIAL_H
#define MATERIAL_H

/*************************************************************************/
/*  material.h                                                           */
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

#include "core/containers/self_list.h"
#include "core/object/resource.h"
#include "scene/resources/shader.h"
#include "servers/rendering_server.h"

class Material : public Resource {
	GDCLASS(Material, Resource);
	RES_BASE_EXTENSION("material")
	OBJ_SAVE_TYPE(Material);

	RID material;
	Ref<Material> next_pass;
	int render_priority;

protected:
	_FORCE_INLINE_ RID _get_material() const { return material; }
	static void _bind_methods();
	virtual bool _can_do_next_pass() const { return false; }

	void _validate_property(PropertyInfo &property) const;

public:
	enum {
		RENDER_PRIORITY_MAX = RS::MATERIAL_RENDER_PRIORITY_MAX,
		RENDER_PRIORITY_MIN = RS::MATERIAL_RENDER_PRIORITY_MIN,
	};
	void set_next_pass(const Ref<Material> &p_pass);
	Ref<Material> get_next_pass() const;

	void set_render_priority(int p_priority);
	int get_render_priority() const;

	virtual RID get_rid() const;

	virtual Shader::Mode get_shader_mode() const = 0;
	Material();
	virtual ~Material();
};

#endif
