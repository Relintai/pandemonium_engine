#ifndef SHADER_COMPILER_GLES3_H
#define SHADER_COMPILER_GLES3_H

/*************************************************************************/
/*  shader_compiler_gles3.h                                              */
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

#include "core/containers/pair.h"
#include "servers/rendering/shader_language.h"
#include "servers/rendering/shader_types.h"
#include "servers/rendering_server.h"

class ShaderCompilerGLES3 {
public:
	struct IdentifierActions {
		RBMap<StringName, Pair<int *, int>> render_mode_values;
		RBMap<StringName, bool *> render_mode_flags;
		RBMap<StringName, bool *> usage_flag_pointers;
		RBMap<StringName, bool *> write_flag_pointers;

		RBMap<StringName, ShaderLanguage::ShaderNode::Uniform> *uniforms;
	};

	struct GeneratedCode {
		Vector<CharString> defines;
		Vector<StringName> texture_uniforms;
		Vector<ShaderLanguage::DataType> texture_types;
		Vector<ShaderLanguage::ShaderNode::Uniform::Hint> texture_hints;

		Vector<uint32_t> uniform_offsets;
		uint32_t uniform_total_size;
		String uniforms;
		String vertex_global;
		String vertex;
		String fragment_global;
		String fragment;
		String light;

		bool uses_fragment_time;
		bool uses_vertex_time;
	};

private:
	ShaderLanguage parser;

	struct DefaultIdentifierActions {
		RBMap<StringName, String> renames;
		RBMap<StringName, String> render_mode_defines;
		RBMap<StringName, String> usage_defines;
	};

	void _dump_function_deps(const ShaderLanguage::ShaderNode *p_node, const StringName &p_for_func, const RBMap<StringName, String> &p_func_code, String &r_to_add, RBSet<StringName> &added);
	String _dump_node_code(const ShaderLanguage::Node *p_node, int p_level, GeneratedCode &r_gen_code, IdentifierActions &p_actions, const DefaultIdentifierActions &p_default_actions, bool p_assigning, bool p_use_scope = true);

	const ShaderLanguage::ShaderNode *shader;
	const ShaderLanguage::FunctionNode *function;
	StringName current_func_name;
	StringName vertex_name;
	StringName fragment_name;
	StringName light_name;
	StringName time_name;

	RBSet<StringName> used_name_defines;
	RBSet<StringName> used_flag_pointers;
	RBSet<StringName> used_rmode_defines;
	RBSet<StringName> internal_functions;
	RBSet<StringName> fragment_varyings;

	DefaultIdentifierActions actions[RS::SHADER_MAX];

public:
	Error compile(RS::ShaderMode p_mode, const String &p_code, IdentifierActions *p_actions, const String &p_path, GeneratedCode &r_gen_code);

	ShaderCompilerGLES3();
};

#endif // SHADER_COMPILER_GLES3_H
