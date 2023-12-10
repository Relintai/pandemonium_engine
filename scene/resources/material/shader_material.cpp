/*************************************************************************/
/*  material.cpp                                                         */
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

#include "shader_material.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/version.h"
#include "scene/resources/texture.h"
#include "servers/rendering/shader_language.h"

#include "scene/main/scene_string_names.h"

bool ShaderMaterial::_set(const StringName &p_name, const Variant &p_value) {
	if (shader.is_valid()) {
		StringName pr = shader->remap_param(p_name);
		if (!pr) {
			String n = p_name;
			if (n.find("param/") == 0) { //backwards compatibility
				pr = n.substr(6, n.length());
			}
			if (n.find("shader_param/") == 0) { //backwards compatibility
				pr = n.replace_first("shader_param/", "");
			}
		}
		if (pr) {
			RenderingServer::get_singleton()->material_set_param(_get_material(), pr, p_value);
			return true;
		}
	}

	return false;
}

bool ShaderMaterial::_get(const StringName &p_name, Variant &r_ret) const {
	if (shader.is_valid()) {
		StringName pr = shader->remap_param(p_name);
		if (!pr) {
			String n = p_name;
			if (n.find("param/") == 0) { //backwards compatibility
				pr = n.substr(6, n.length());
			}
			if (n.find("shader_param/") == 0) { //backwards compatibility
				pr = n.replace_first("shader_param/", "");
			}
		}

		if (pr) {
			r_ret = RenderingServer::get_singleton()->material_get_param(_get_material(), pr);
			return true;
		}
	}

	return false;
}

void ShaderMaterial::_get_property_list(List<PropertyInfo> *p_list) const {
	if (!shader.is_null()) {
		shader->get_param_list(p_list);
	}
}

bool ShaderMaterial::property_can_revert(const String &p_name) {
	if (shader.is_valid()) {
		StringName pr = shader->remap_param(p_name);
		if (pr) {
			Variant default_value = RenderingServer::get_singleton()->material_get_param_default(_get_material(), pr);
			Variant current_value;
			_get(p_name, current_value);
			return default_value.get_type() != Variant::NIL && default_value != current_value;
		}
	}
	return false;
}

Variant ShaderMaterial::property_get_revert(const String &p_name) {
	Variant r_ret;
	if (shader.is_valid()) {
		StringName pr = shader->remap_param(p_name);
		if (pr) {
			r_ret = RenderingServer::get_singleton()->material_get_param_default(_get_material(), pr);
		}
	}
	return r_ret;
}

void ShaderMaterial::set_shader(const Ref<Shader> &p_shader) {
	// Only connect/disconnect the signal when running in the editor.
	// This can be a slow operation, and `_change_notify()` (which is called by `_shader_changed()`)
	// does nothing in non-editor builds anyway. See GH-34741 for details.
	if (shader.is_valid() && Engine::get_singleton()->is_editor_hint()) {
		shader->disconnect("changed", this, "_shader_changed");
	}

	shader = p_shader;

	RID rid;
	if (shader.is_valid()) {
		rid = shader->get_rid();

		if (Engine::get_singleton()->is_editor_hint()) {
			shader->connect("changed", this, "_shader_changed");
		}
	}

	RS::get_singleton()->material_set_shader(_get_material(), rid);
	_change_notify(); //properties for shader exposed
	emit_changed();
}

Ref<Shader> ShaderMaterial::get_shader() const {
	return shader;
}

void ShaderMaterial::set_shader_param(const StringName &p_param, const Variant &p_value) {
	RS::get_singleton()->material_set_param(_get_material(), p_param, p_value);
}

Variant ShaderMaterial::get_shader_param(const StringName &p_param) const {
	return RS::get_singleton()->material_get_param(_get_material(), p_param);
}

void ShaderMaterial::_shader_changed() {
	_change_notify(); //update all properties
}

void ShaderMaterial::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_shader", "shader"), &ShaderMaterial::set_shader);
	ClassDB::bind_method(D_METHOD("get_shader"), &ShaderMaterial::get_shader);
	ClassDB::bind_method(D_METHOD("set_shader_param", "param", "value"), &ShaderMaterial::set_shader_param);
	ClassDB::bind_method(D_METHOD("get_shader_param", "param"), &ShaderMaterial::get_shader_param);
	ClassDB::bind_method(D_METHOD("_shader_changed"), &ShaderMaterial::_shader_changed);
	ClassDB::bind_method(D_METHOD("property_can_revert", "name"), &ShaderMaterial::property_can_revert);
	ClassDB::bind_method(D_METHOD("property_get_revert", "name"), &ShaderMaterial::property_get_revert);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shader", PROPERTY_HINT_RESOURCE_TYPE, "Shader"), "set_shader", "get_shader");
}

void ShaderMaterial::get_argument_options(const StringName &p_function, int p_idx, List<String> *r_options, const String &quote_style) const {
	String f = p_function.operator String();
	if ((f == "get_shader_param" || f == "set_shader_param") && p_idx == 0) {
		if (shader.is_valid()) {
			List<PropertyInfo> pl;
			shader->get_param_list(&pl);
			for (List<PropertyInfo>::Element *E = pl.front(); E; E = E->next()) {
				r_options->push_back(quote_style + E->get().name.replace_first("shader_param/", "") + quote_style);
			}
		}
	}
	Resource::get_argument_options(p_function, p_idx, r_options, quote_style);
}

bool ShaderMaterial::_can_do_next_pass() const {
	return shader.is_valid() && shader->get_mode() == Shader::MODE_SPATIAL;
}

Shader::Mode ShaderMaterial::get_shader_mode() const {
	if (shader.is_valid()) {
		return shader->get_mode();
	} else {
		return Shader::MODE_SPATIAL;
	}
}

ShaderMaterial::ShaderMaterial() {
}

ShaderMaterial::~ShaderMaterial() {
}
