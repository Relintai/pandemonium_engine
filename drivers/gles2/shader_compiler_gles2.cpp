/*************************************************************************/
/*  shader_compiler_gles2.cpp                                            */
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

#include "shader_compiler_gles2.h"

#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "core/string/string_buffer.h"
#include "core/string/string_builder.h"

#define SL ShaderLanguage

static String _mktab(int p_level) {
	String tb;
	for (int i = 0; i < p_level; i++) {
		tb += "\t";
	}

	return tb;
}

static String _typestr(SL::DataType p_type) {
	return ShaderLanguage::get_datatype_name(p_type);
}

static String _prestr(SL::DataPrecision p_pres) {
	switch (p_pres) {
		case SL::PRECISION_LOWP:
			return "lowp ";
		case SL::PRECISION_MEDIUMP:
			return "mediump ";
		case SL::PRECISION_HIGHP:
			return "highp ";
		case SL::PRECISION_DEFAULT:
			return "";
	}
	return "";
}

static String _constr(bool p_is_const) {
	if (p_is_const) {
		return "const ";
	}
	return "";
}

static String _qualstr(SL::ArgumentQualifier p_qual) {
	switch (p_qual) {
		case SL::ARGUMENT_QUALIFIER_IN:
			return "in ";
		case SL::ARGUMENT_QUALIFIER_OUT:
			return "out ";
		case SL::ARGUMENT_QUALIFIER_INOUT:
			return "inout ";
	}
	return "";
}

static String _opstr(SL::Operator p_op) {
	return SL::get_operator_text(p_op);
}

static String _mkid(const String &p_id) {
	String id = "m_" + p_id.replace("__", "_dus_");
	return id.replace("__", "_dus_"); //doubleunderscore is reserved in glsl
}

static String f2sp0(float p_float) {
	String num = rtoss(p_float);
	if (num.find(".") == -1 && num.find("e") == -1) {
		num += ".0";
	}
	return num;
}

static String get_constant_text(SL::DataType p_type, const Vector<SL::ConstantNode::Value> &p_values) {
	switch (p_type) {
		case SL::TYPE_BOOL:
			return p_values[0].boolean ? "true" : "false";
		case SL::TYPE_BVEC2:
		case SL::TYPE_BVEC3:
		case SL::TYPE_BVEC4: {
			StringBuffer<> text;

			text += "bvec";
			text += itos(p_type - SL::TYPE_BOOL + 1);
			text += "(";

			for (int i = 0; i < p_values.size(); i++) {
				if (i > 0) {
					text += ",";
				}

				text += p_values[i].boolean ? "true" : "false";
			}
			text += ")";
			return text.as_string();
		}

		// GLSL ES 2 doesn't support uints, so we just use signed ints instead...
		case SL::TYPE_UINT:
			return itos(p_values[0].uint);
		case SL::TYPE_UVEC2:
		case SL::TYPE_UVEC3:
		case SL::TYPE_UVEC4: {
			StringBuffer<> text;

			text += "ivec";
			text += itos(p_type - SL::TYPE_UINT + 1);
			text += "(";

			for (int i = 0; i < p_values.size(); i++) {
				if (i > 0) {
					text += ",";
				}

				text += itos(p_values[i].uint);
			}
			text += ")";
			return text.as_string();

		} break;

		case SL::TYPE_INT:
			return itos(p_values[0].sint);
		case SL::TYPE_IVEC2:
		case SL::TYPE_IVEC3:
		case SL::TYPE_IVEC4: {
			StringBuffer<> text;

			text += "ivec";
			text += itos(p_type - SL::TYPE_INT + 1);
			text += "(";

			for (int i = 0; i < p_values.size(); i++) {
				if (i > 0) {
					text += ",";
				}

				text += itos(p_values[i].sint);
			}
			text += ")";
			return text.as_string();

		} break;
		case SL::TYPE_FLOAT:
			return f2sp0(p_values[0].real);
		case SL::TYPE_VEC2:
		case SL::TYPE_VEC3:
		case SL::TYPE_VEC4: {
			StringBuffer<> text;

			text += "vec";
			text += itos(p_type - SL::TYPE_FLOAT + 1);
			text += "(";

			for (int i = 0; i < p_values.size(); i++) {
				if (i > 0) {
					text += ",";
				}

				text += f2sp0(p_values[i].real);
			}
			text += ")";
			return text.as_string();

		} break;
		case SL::TYPE_MAT2:
		case SL::TYPE_MAT3:
		case SL::TYPE_MAT4: {
			StringBuffer<> text;

			text += "mat";
			text += itos(p_type - SL::TYPE_MAT2 + 2);
			text += "(";

			for (int i = 0; i < p_values.size(); i++) {
				if (i > 0) {
					text += ",";
				}

				text += f2sp0(p_values[i].real);
			}
			text += ")";
			return text.as_string();

		} break;
		default:
			ERR_FAIL_V(String());
	}
}

void ShaderCompilerGLES2::_dump_function_deps(const SL::ShaderNode *p_node, const StringName &p_for_func, const RBMap<StringName, String> &p_func_code, StringBuilder &r_to_add, RBSet<StringName> &r_added) {
	int fidx = -1;

	for (int i = 0; i < p_node->functions.size(); i++) {
		if (p_node->functions[i].name == p_for_func) {
			fidx = i;
			break;
		}
	}

	ERR_FAIL_COND(fidx == -1);

	for (int ufidx = 0; ufidx < p_node->functions[fidx].uses_function.size(); ufidx++) {
		StringName function_name = p_node->functions[fidx].uses_function[ufidx];
		if (r_added.has(function_name)) {
			continue;
		}

		_dump_function_deps(p_node, function_name, p_func_code, r_to_add, r_added);

		SL::FunctionNode *fnode = nullptr;

		for (int i = 0; i < p_node->functions.size(); i++) {
			if (p_node->functions[i].name == function_name) {
				fnode = p_node->functions[i].function;
				break;
			}
		}

		ERR_FAIL_COND(!fnode);

		r_to_add += "\n";

		StringBuffer<128> header;
		if (fnode->return_type == SL::TYPE_STRUCT) {
			header += _mkid(fnode->return_struct_name) + " " + _mkid(fnode->name) + "(";
		} else {
			header += _typestr(fnode->return_type) + " " + _mkid(fnode->name) + "(";
		}

		for (int i = 0; i < fnode->arguments.size(); i++) {
			if (i > 0) {
				header += ", ";
			}

			header += _constr(fnode->arguments[i].is_const);

			if (fnode->arguments[i].type == SL::TYPE_STRUCT) {
				header += _qualstr(fnode->arguments[i].qualifier) + _mkid(fnode->arguments[i].type_str) + " " + _mkid(fnode->arguments[i].name);
			} else {
				header += _qualstr(fnode->arguments[i].qualifier) + _prestr(fnode->arguments[i].precision) + _typestr(fnode->arguments[i].type) + " " + _mkid(fnode->arguments[i].name);
			}
		}

		header += ")\n";
		r_to_add += header.as_string();
		r_to_add += p_func_code[function_name];

		r_added.insert(function_name);
	}
}

String ShaderCompilerGLES2::_dump_node_code(const SL::Node *p_node, int p_level, GeneratedCode &r_gen_code, IdentifierActions &p_actions, const DefaultIdentifierActions &p_default_actions, bool p_assigning, bool p_use_scope) {
	StringBuilder code;

	switch (p_node->type) {
		case SL::Node::TYPE_SHADER: {
			SL::ShaderNode *snode = (SL::ShaderNode *)p_node;

			for (int i = 0; i < snode->render_modes.size(); i++) {
				if (p_default_actions.render_mode_defines.has(snode->render_modes[i]) && !used_rmode_defines.has(snode->render_modes[i])) {
					r_gen_code.custom_defines.push_back(p_default_actions.render_mode_defines[snode->render_modes[i]].utf8());
					used_rmode_defines.insert(snode->render_modes[i]);
				}

				if (p_actions.render_mode_flags.has(snode->render_modes[i])) {
					*p_actions.render_mode_flags[snode->render_modes[i]] = true;
				}

				if (p_actions.render_mode_values.has(snode->render_modes[i])) {
					Pair<int *, int> &p = p_actions.render_mode_values[snode->render_modes[i]];
					*p.first = p.second;
				}
			}

			int max_texture_uniforms = 0;
			int max_uniforms = 0;

			for (OrderedHashMap<StringName, SL::ShaderNode::Uniform>::Element E = snode->uniforms.front(); E; E = E.next()) {
				if (SL::is_sampler_type(E.get().type)) {
					max_texture_uniforms++;
				} else {
					max_uniforms++;
				}
			}

			r_gen_code.texture_uniforms.resize(max_texture_uniforms);
			r_gen_code.texture_hints.resize(max_texture_uniforms);

			r_gen_code.uniforms.resize(max_uniforms + max_texture_uniforms);

			StringBuilder vertex_global;
			StringBuilder fragment_global;

			// structs

			for (int i = 0; i < snode->vstructs.size(); i++) {
				SL::StructNode *st = snode->vstructs[i].shader_struct;
				String struct_code;

				struct_code += "struct ";
				struct_code += _mkid(snode->vstructs[i].name);
				struct_code += " ";
				struct_code += "{\n";
				for (int j = 0; j < st->members.size(); j++) {
					SL::MemberNode *m = st->members[j];
					if (m->datatype == SL::TYPE_STRUCT) {
						struct_code += _mkid(m->struct_name);
					} else {
						struct_code += _prestr(m->precision);
						struct_code += _typestr(m->datatype);
					}
					struct_code += " ";
					struct_code += m->name;
					if (m->array_size > 0) {
						struct_code += "[";
						struct_code += itos(m->array_size);
						struct_code += "]";
					}
					struct_code += ";\n";
				}
				struct_code += "}";
				struct_code += ";\n";

				vertex_global += struct_code;
				fragment_global += struct_code;
			}

			// uniforms

			for (OrderedHashMap<StringName, SL::ShaderNode::Uniform>::Element E = snode->uniforms.front(); E; E = E.next()) {
				StringBuffer<> uniform_code;

				// use highp if no precision is specified to prevent different default values in fragment and vertex shader
				SL::DataPrecision precision = E.get().precision;
				if (precision == SL::PRECISION_DEFAULT && E.get().type != SL::TYPE_BOOL) {
					precision = SL::PRECISION_HIGHP;
				}

				uniform_code += "uniform ";
				uniform_code += _prestr(precision);
				uniform_code += _typestr(E.get().type);
				uniform_code += " ";
				uniform_code += _mkid(E.key());
				uniform_code += ";\n";

				if (SL::is_sampler_type(E.get().type)) {
					r_gen_code.texture_uniforms.write[E.get().texture_order] = E.key();
					r_gen_code.texture_hints.write[E.get().texture_order] = E.get().hint;
				} else {
					r_gen_code.uniforms.write[E.get().order] = E.key();
				}

				vertex_global += uniform_code.as_string();
				fragment_global += uniform_code.as_string();

				p_actions.uniforms->insert(E.key(), E.get());
			}

			// varyings

			List<Pair<StringName, SL::ShaderNode::Varying>> var_frag_to_light;

			for (OrderedHashMap<StringName, SL::ShaderNode::Varying>::Element E = snode->varyings.front(); E; E = E.next()) {
				if (E.get().stage == SL::ShaderNode::Varying::STAGE_FRAGMENT_TO_LIGHT || E.get().stage == SL::ShaderNode::Varying::STAGE_FRAGMENT) {
					var_frag_to_light.push_back(Pair<StringName, SL::ShaderNode::Varying>(E.key(), E.get()));
					fragment_varyings.insert(E.key());
					continue;
				}
				StringBuffer<> varying_code;

				varying_code += "varying ";
				varying_code += _prestr(E.get().precision);
				varying_code += _typestr(E.get().type);
				varying_code += " ";
				varying_code += _mkid(E.key());
				if (E.get().array_size > 0) {
					varying_code += "[";
					varying_code += itos(E.get().array_size);
					varying_code += "]";
				}
				varying_code += ";\n";

				String final_code = varying_code.as_string();

				vertex_global += final_code;
				fragment_global += final_code;
			}

			if (var_frag_to_light.size() > 0) {
				String gcode = "\n\nstruct {\n";
				for (List<Pair<StringName, SL::ShaderNode::Varying>>::Element *E = var_frag_to_light.front(); E; E = E->next()) {
					gcode += "\t" + _prestr(E->get().second.precision) + _typestr(E->get().second.type) + " " + _mkid(E->get().first);
					if (E->get().second.array_size > 0) {
						gcode += "[";
						gcode += itos(E->get().second.array_size);
						gcode += "]";
					}
					gcode += ";\n";
				}
				gcode += "} frag_to_light;\n";
				r_gen_code.fragment_global += gcode;
			}

			// constants

			for (int i = 0; i < snode->vconstants.size(); i++) {
				String gcode;
				gcode += _constr(true);
				if (snode->vconstants[i].type == SL::TYPE_STRUCT) {
					gcode += _mkid(snode->vconstants[i].type_str);
				} else {
					gcode += _prestr(snode->vconstants[i].precision);
					gcode += _typestr(snode->vconstants[i].type);
				}
				gcode += " " + _mkid(String(snode->vconstants[i].name));
				gcode += "=";
				gcode += _dump_node_code(snode->vconstants[i].initializer, p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				gcode += ";\n";
				vertex_global += gcode;
				fragment_global += gcode;
			}

			// functions

			RBMap<StringName, String> function_code;

			for (int i = 0; i < snode->functions.size(); i++) {
				SL::FunctionNode *fnode = snode->functions[i].function;
				function = fnode;
				current_func_name = fnode->name;
				function_code[fnode->name] = _dump_node_code(fnode->body, 1, r_gen_code, p_actions, p_default_actions, p_assigning);
				function = nullptr;
			}

			RBSet<StringName> added_vertex;
			RBSet<StringName> added_fragment;

			for (int i = 0; i < snode->functions.size(); i++) {
				SL::FunctionNode *fnode = snode->functions[i].function;

				function = fnode;

				current_func_name = fnode->name;

				if (fnode->name == vertex_name) {
					_dump_function_deps(snode, fnode->name, function_code, vertex_global, added_vertex);
					r_gen_code.vertex = function_code[vertex_name];

				} else if (fnode->name == fragment_name) {
					_dump_function_deps(snode, fnode->name, function_code, fragment_global, added_fragment);
					r_gen_code.fragment = function_code[fragment_name];

				} else if (fnode->name == light_name) {
					_dump_function_deps(snode, fnode->name, function_code, fragment_global, added_fragment);
					r_gen_code.light = function_code[light_name];
				}

				function = nullptr;
			}

			r_gen_code.vertex_global = vertex_global.as_string();
			r_gen_code.fragment_global = fragment_global.as_string();

		} break;
		case SL::Node::TYPE_STRUCT: {
		} break;
		case SL::Node::TYPE_FUNCTION: {
		} break;

		case SL::Node::TYPE_BLOCK: {
			SL::BlockNode *bnode = (SL::BlockNode *)p_node;

			if (!bnode->single_statement) {
				code += _mktab(p_level - 1);
				code += "{\n";
			}

			for (int i = 0; i < bnode->statements.size(); i++) {
				String statement_code = _dump_node_code(bnode->statements[i], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);

				if (bnode->statements[i]->type == SL::Node::TYPE_CONTROL_FLOW || bnode->single_statement) {
					code += statement_code;
				} else {
					code += _mktab(p_level);
					code += statement_code;
					code += ";\n";
				}
			}

			if (!bnode->single_statement) {
				code += _mktab(p_level - 1);
				code += "}\n";
			}
		} break;

		case SL::Node::TYPE_VARIABLE_DECLARATION: {
			SL::VariableDeclarationNode *var_dec_node = (SL::VariableDeclarationNode *)p_node;

			StringBuffer<> declaration;
			declaration += _constr(var_dec_node->is_const);
			if (var_dec_node->datatype == SL::TYPE_STRUCT) {
				declaration += _mkid(var_dec_node->struct_name);
			} else {
				declaration += _prestr(var_dec_node->precision);
				declaration += _typestr(var_dec_node->datatype);
			}

			for (int i = 0; i < var_dec_node->declarations.size(); i++) {
				if (i > 0) {
					declaration += ",";
				}

				declaration += " ";

				declaration += _mkid(var_dec_node->declarations[i].name);

				if (var_dec_node->declarations[i].initializer) {
					declaration += " = ";
					declaration += _dump_node_code(var_dec_node->declarations[i].initializer, p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				}
			}

			code += declaration.as_string();
		} break;

		case SL::Node::TYPE_VARIABLE: {
			SL::VariableNode *var_node = (SL::VariableNode *)p_node;
			bool use_fragment_varying = false;

			if (!var_node->is_local && current_func_name != vertex_name) {
				if (p_assigning) {
					if (shader->varyings.has(var_node->name)) {
						use_fragment_varying = true;
					}
				} else {
					if (fragment_varyings.has(var_node->name)) {
						use_fragment_varying = true;
					}
				}
			}

			if (p_assigning && p_actions.write_flag_pointers.has(var_node->name)) {
				*p_actions.write_flag_pointers[var_node->name] = true;
			}

			if (p_default_actions.usage_defines.has(var_node->name) && !used_name_defines.has(var_node->name)) {
				String define = p_default_actions.usage_defines[var_node->name];
				String node_name = define.substr(1, define.length());

				if (define.begins_with("@")) {
					define = p_default_actions.usage_defines[node_name];
				}

				if (!used_name_defines.has(node_name)) {
					r_gen_code.custom_defines.push_back(define.utf8());
				}
				used_name_defines.insert(var_node->name);
			}

			if (p_actions.usage_flag_pointers.has(var_node->name) && !used_flag_pointers.has(var_node->name)) {
				*p_actions.usage_flag_pointers[var_node->name] = true;
				used_flag_pointers.insert(var_node->name);
			}

			if (p_default_actions.renames.has(var_node->name)) {
				code += p_default_actions.renames[var_node->name];
			} else if (use_fragment_varying) {
				code += "frag_to_light." + _mkid(var_node->name);
			} else {
				code += _mkid(var_node->name);
			}

			if (var_node->name == time_name) {
				if (current_func_name == vertex_name) {
					r_gen_code.uses_vertex_time = true;
				}
				if (current_func_name == fragment_name || current_func_name == light_name) {
					r_gen_code.uses_fragment_time = true;
				}
			}
		} break;
		case SL::Node::TYPE_ARRAY_CONSTRUCT: {
			SL::ArrayConstructNode *arr_con_node = (SL::ArrayConstructNode *)p_node;
			int sz = arr_con_node->initializer.size();
			if (arr_con_node->datatype == SL::TYPE_STRUCT) {
				code += _mkid(arr_con_node->struct_name);
			} else {
				code += _typestr(arr_con_node->datatype);
			}
			code += "[";
			code += itos(arr_con_node->initializer.size());
			code += "]";
			code += "(";
			for (int i = 0; i < sz; i++) {
				code += _dump_node_code(arr_con_node->initializer[i], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				if (i != sz - 1) {
					code += ", ";
				}
			}
			code += ")";
		} break;
		case SL::Node::TYPE_ARRAY_DECLARATION: {
			SL::ArrayDeclarationNode *arr_dec_node = (SL::ArrayDeclarationNode *)p_node;

			StringBuffer<> declaration;
			if (arr_dec_node->datatype == SL::TYPE_STRUCT) {
				declaration += _mkid(arr_dec_node->struct_name);
			} else {
				declaration += _prestr(arr_dec_node->precision);
				declaration += _typestr(arr_dec_node->datatype);
			}
			for (int i = 0; i < arr_dec_node->declarations.size(); i++) {
				if (i > 0) {
					declaration += ",";
				}

				declaration += " ";

				declaration += _mkid(arr_dec_node->declarations[i].name);
				declaration += "[";
				declaration += itos(arr_dec_node->declarations[i].size);
				declaration += "]";
			}

			code += declaration.as_string();
		} break;
		case SL::Node::TYPE_ARRAY: {
			SL::ArrayNode *arr_node = (SL::ArrayNode *)p_node;
			bool use_fragment_varying = false;

			if (!arr_node->is_local && current_func_name != vertex_name) {
				if (arr_node->assign_expression != nullptr) {
					use_fragment_varying = true;
				} else {
					if (p_assigning) {
						if (shader->varyings.has(arr_node->name)) {
							use_fragment_varying = true;
						}
					} else {
						if (fragment_varyings.has(arr_node->name)) {
							use_fragment_varying = true;
						}
					}
				}
			}

			if (p_assigning && p_actions.write_flag_pointers.has(arr_node->name)) {
				*p_actions.write_flag_pointers[arr_node->name] = true;
			}

			if (p_default_actions.usage_defines.has(arr_node->name) && !used_name_defines.has(arr_node->name)) {
				String define = p_default_actions.usage_defines[arr_node->name];
				String node_name = define.substr(1, define.length());

				if (define.begins_with("@")) {
					define = p_default_actions.usage_defines[node_name];
				}

				if (!used_name_defines.has(node_name)) {
					r_gen_code.custom_defines.push_back(define.utf8());
				}
				used_name_defines.insert(arr_node->name);
			}

			if (p_actions.usage_flag_pointers.has(arr_node->name) && !used_flag_pointers.has(arr_node->name)) {
				*p_actions.usage_flag_pointers[arr_node->name] = true;
				used_flag_pointers.insert(arr_node->name);
			}

			if (p_default_actions.renames.has(arr_node->name)) {
				code += p_default_actions.renames[arr_node->name];
			} else if (use_fragment_varying) {
				code += "frag_to_light." + _mkid(arr_node->name);
			} else {
				code += _mkid(arr_node->name);
			}

			if (arr_node->call_expression != nullptr) {
				code += ".";
				code += _dump_node_code(arr_node->call_expression, p_level, r_gen_code, p_actions, p_default_actions, p_assigning, false);
			}

			if (arr_node->index_expression != nullptr) {
				code += "[";
				code += _dump_node_code(arr_node->index_expression, p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += "]";
			}

			if (arr_node->name == time_name) {
				if (current_func_name == vertex_name) {
					r_gen_code.uses_vertex_time = true;
				}
				if (current_func_name == fragment_name || current_func_name == light_name) {
					r_gen_code.uses_fragment_time = true;
				}
			}

		} break;
		case SL::Node::TYPE_CONSTANT: {
			SL::ConstantNode *const_node = (SL::ConstantNode *)p_node;

			return get_constant_text(const_node->datatype, const_node->values);
		} break;

		case SL::Node::TYPE_OPERATOR: {
			SL::OperatorNode *op_node = (SL::OperatorNode *)p_node;

			switch (op_node->op) {
				case SL::OP_ASSIGN:
				case SL::OP_ASSIGN_ADD:
				case SL::OP_ASSIGN_SUB:
				case SL::OP_ASSIGN_MUL:
				case SL::OP_ASSIGN_DIV:
				case SL::OP_ASSIGN_SHIFT_LEFT:
				case SL::OP_ASSIGN_SHIFT_RIGHT:
				case SL::OP_ASSIGN_BIT_AND:
				case SL::OP_ASSIGN_BIT_OR:
				case SL::OP_ASSIGN_BIT_XOR: {
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, true);
					code += " ";
					code += _opstr(op_node->op);
					code += " ";
					code += _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				} break;

				case SL::OP_ASSIGN_MOD: {
					String a = _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					String n = _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += a + " = " + n + " == 0 ? 0 : ";
					code += a + " - " + n + " * (" + a + " / " + n + ")";
				} break;

				case SL::OP_BIT_INVERT:
				case SL::OP_NEGATE:
				case SL::OP_NOT:
				case SL::OP_DECREMENT:
				case SL::OP_INCREMENT: {
					code += _opstr(op_node->op);
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				} break;

				case SL::OP_POST_DECREMENT:
				case SL::OP_POST_INCREMENT: {
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += _opstr(op_node->op);
				} break;

				case SL::OP_CALL:
				case SL::OP_STRUCT:
				case SL::OP_CONSTRUCT: {
					ERR_FAIL_COND_V(op_node->arguments[0]->type != SL::Node::TYPE_VARIABLE, String());

					SL::VariableNode *var_node = (SL::VariableNode *)op_node->arguments[0];
					if (op_node->op == SL::OP_STRUCT) {
						code += _mkid(var_node->name);
					} else if (op_node->op == SL::OP_CONSTRUCT) {
						code += var_node->name;
					} else {
						if (var_node->name == "texture") {
							// emit texture call

							if (op_node->arguments[1]->get_datatype() == SL::TYPE_SAMPLER2D ||
									op_node->arguments[1]->get_datatype() == SL::TYPE_SAMPLEREXT) {
								code += "texture2D";
							} else if (op_node->arguments[1]->get_datatype() == SL::TYPE_SAMPLERCUBE) {
								code += "textureCube";
							}

						} else if (var_node->name == "textureLod") {
							// emit texture call

							if (op_node->arguments[1]->get_datatype() == SL::TYPE_SAMPLER2D) {
								code += "texture2DLod";
							} else if (op_node->arguments[1]->get_datatype() == SL::TYPE_SAMPLERCUBE) {
								code += "textureCubeLod";
							}

						} else if (var_node->name == "mix") {
							switch (op_node->arguments[3]->get_datatype()) {
								case SL::TYPE_BVEC2: {
									code += "select2";
								} break;

								case SL::TYPE_BVEC3: {
									code += "select3";
								} break;

								case SL::TYPE_BVEC4: {
									code += "select4";
								} break;

								case SL::TYPE_VEC2:
								case SL::TYPE_VEC3:
								case SL::TYPE_VEC4:
								case SL::TYPE_FLOAT: {
									code += "mix";
								} break;

								default: {
									SL::DataType type = op_node->arguments[3]->get_datatype();
									// FIXME: Proper error print or graceful handling
									print_line(String("uhhhh invalid mix with type: ") + itos(type));
								} break;
							}

						} else if (p_default_actions.renames.has(var_node->name)) {
							code += p_default_actions.renames[var_node->name];
						} else if (internal_functions.has(var_node->name)) {
							code += var_node->name;
						} else {
							code += _mkid(var_node->name);
						}
					}

					code += "(";

					for (int i = 1; i < op_node->arguments.size(); i++) {
						if (i > 1) {
							code += ", ";
						}

						code += _dump_node_code(op_node->arguments[i], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					}

					code += ")";

					if (p_default_actions.usage_defines.has(var_node->name) && !used_name_defines.has(var_node->name)) {
						String define = p_default_actions.usage_defines[var_node->name];
						String node_name = define.substr(1, define.length());

						if (define.begins_with("@")) {
							define = p_default_actions.usage_defines[node_name];
						}

						if (!used_name_defines.has(node_name)) {
							r_gen_code.custom_defines.push_back(define.utf8());
						}
						used_name_defines.insert(var_node->name);
					}

				} break;

				case SL::OP_INDEX: {
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += "[";
					code += _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += "]";
				} break;

				case SL::OP_SELECT_IF: {
					code += "(";
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += " ? ";
					code += _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += " : ";
					code += _dump_node_code(op_node->arguments[2], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += ")";
				} break;

				case SL::OP_MOD: {
					String a = _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					String n = _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += "(" + n + " == 0 ? 0 : ";
					code += a + " - " + n + " * (" + a + " / " + n + "))";
				} break;

				default: {
					if (p_use_scope) {
						code += "(";
					}
					code += _dump_node_code(op_node->arguments[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					code += " ";
					code += _opstr(op_node->op);
					code += " ";
					code += _dump_node_code(op_node->arguments[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
					if (p_use_scope) {
						code += ")";
					}
				} break;
			}
		} break;

		case SL::Node::TYPE_CONTROL_FLOW: {
			SL::ControlFlowNode *cf_node = (SL::ControlFlowNode *)p_node;

			if (cf_node->flow_op == SL::FLOW_OP_IF) {
				code += _mktab(p_level);
				code += "if (";
				code += _dump_node_code(cf_node->expressions[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += ")\n";
				code += _dump_node_code(cf_node->blocks[0], p_level + 1, r_gen_code, p_actions, p_default_actions, p_assigning);

				if (cf_node->blocks.size() == 2) {
					code += _mktab(p_level);
					code += "else\n";
					code += _dump_node_code(cf_node->blocks[1], p_level + 1, r_gen_code, p_actions, p_default_actions, p_assigning);
				}
			} else if (cf_node->flow_op == SL::FLOW_OP_DO) {
				code += _mktab(p_level);
				code += "do";
				code += _dump_node_code(cf_node->blocks[0], p_level + 1, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += _mktab(p_level);
				code += "while (";
				code += _dump_node_code(cf_node->expressions[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += ");";
			} else if (cf_node->flow_op == SL::FLOW_OP_WHILE) {
				code += _mktab(p_level);
				code += "while (";
				code += _dump_node_code(cf_node->expressions[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += ")\n";
				code += _dump_node_code(cf_node->blocks[0], p_level + 1, r_gen_code, p_actions, p_default_actions, p_assigning);
			} else if (cf_node->flow_op == SL::FLOW_OP_FOR) {
				code += _mktab(p_level);
				code += "for (";
				code += _dump_node_code(cf_node->blocks[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += "; ";
				code += _dump_node_code(cf_node->expressions[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += "; ";
				code += _dump_node_code(cf_node->expressions[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += ")\n";

				code += _dump_node_code(cf_node->blocks[1], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);

			} else if (cf_node->flow_op == SL::FLOW_OP_RETURN) {
				code += _mktab(p_level);
				code += "return";

				if (cf_node->expressions.size()) {
					code += " ";
					code += _dump_node_code(cf_node->expressions[0], p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				}
				code += ";\n";
			} else if (cf_node->flow_op == SL::FLOW_OP_DISCARD) {
				if (p_actions.usage_flag_pointers.has("DISCARD") && !used_flag_pointers.has("DISCARD")) {
					*p_actions.usage_flag_pointers["DISCARD"] = true;
					used_flag_pointers.insert("DISCARD");
				}
				code += "discard;";
			} else if (cf_node->flow_op == SL::FLOW_OP_CONTINUE) {
				code += "continue;";
			} else if (cf_node->flow_op == SL::FLOW_OP_BREAK) {
				code += "break;";
			}
		} break;

		case SL::Node::TYPE_MEMBER: {
			SL::MemberNode *member_node = (SL::MemberNode *)p_node;
			code += _dump_node_code(member_node->owner, p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
			code += ".";
			code += member_node->name;
			if (member_node->index_expression != nullptr) {
				code += "[";
				code += _dump_node_code(member_node->index_expression, p_level, r_gen_code, p_actions, p_default_actions, p_assigning);
				code += "]";
			}
		} break;
	}

	return code.as_string();
}

Error ShaderCompilerGLES2::compile(RS::ShaderMode p_mode, const String &p_code, IdentifierActions *p_actions, const String &p_path, GeneratedCode &r_gen_code) {
	Error err = parser.compile(p_code, ShaderTypes::get_singleton()->get_functions(p_mode), ShaderTypes::get_singleton()->get_modes(p_mode), ShaderTypes::get_singleton()->get_types());

	if (err != OK) {
		Vector<String> shader = p_code.split("\n");
		for (int i = 0; i < shader.size(); i++) {
			if (i + 1 == parser.get_error_line()) {
				// Mark the error line to be visible without having to look at
				// the trace at the end.
				print_line(vformat("E%4d-> %s", i + 1, shader[i]));
			} else {
				print_line(vformat("%5d | %s", i + 1, shader[i]));
			}
		}

		_err_print_error(nullptr, p_path.utf8().get_data(), parser.get_error_line(), parser.get_error_text().utf8().get_data(), ERR_HANDLER_SHADER);
		return err;
	}

	r_gen_code.custom_defines.clear();
	r_gen_code.uniforms.clear();
	r_gen_code.texture_uniforms.clear();
	r_gen_code.texture_hints.clear();
	r_gen_code.vertex = String();
	r_gen_code.vertex_global = String();
	r_gen_code.fragment = String();
	r_gen_code.fragment_global = String();
	r_gen_code.light = String();
	r_gen_code.uses_fragment_time = false;
	r_gen_code.uses_vertex_time = false;

	used_name_defines.clear();
	used_rmode_defines.clear();
	used_flag_pointers.clear();
	fragment_varyings.clear();

	shader = parser.get_shader();
	function = nullptr;
	_dump_node_code(shader, 1, r_gen_code, *p_actions, actions[p_mode], false);

	return OK;
}

ShaderCompilerGLES2::ShaderCompilerGLES2() {
	/** CANVAS ITEM SHADER **/

	actions[RS::SHADER_CANVAS_ITEM].renames["VERTEX"] = "outvec.xy";
	actions[RS::SHADER_CANVAS_ITEM].renames["UV"] = "uv";
	actions[RS::SHADER_CANVAS_ITEM].renames["POINT_SIZE"] = "point_size";

	actions[RS::SHADER_CANVAS_ITEM].renames["WORLD_MATRIX"] = "modelview_matrix";
	actions[RS::SHADER_CANVAS_ITEM].renames["PROJECTION_MATRIX"] = "projection_matrix";
	actions[RS::SHADER_CANVAS_ITEM].renames["EXTRA_MATRIX"] = "extra_matrix_instance";
	actions[RS::SHADER_CANVAS_ITEM].renames["TIME"] = "time";
	actions[RS::SHADER_CANVAS_ITEM].renames["AT_LIGHT_PASS"] = "at_light_pass";
	actions[RS::SHADER_CANVAS_ITEM].renames["INSTANCE_CUSTOM"] = "instance_custom";

	actions[RS::SHADER_CANVAS_ITEM].renames["COLOR"] = "color";
	actions[RS::SHADER_CANVAS_ITEM].renames["MODULATE"] = "final_modulate_alias";
	actions[RS::SHADER_CANVAS_ITEM].renames["NORMAL"] = "normal";
	actions[RS::SHADER_CANVAS_ITEM].renames["NORMALMAP"] = "normal_map";
	actions[RS::SHADER_CANVAS_ITEM].renames["NORMALMAP_DEPTH"] = "normal_depth";
	actions[RS::SHADER_CANVAS_ITEM].renames["TEXTURE"] = "color_texture";
	actions[RS::SHADER_CANVAS_ITEM].renames["TEXTURE_PIXEL_SIZE"] = "color_texpixel_size";
	actions[RS::SHADER_CANVAS_ITEM].renames["NORMAL_TEXTURE"] = "normal_texture";
	actions[RS::SHADER_CANVAS_ITEM].renames["SCREEN_UV"] = "screen_uv";
	actions[RS::SHADER_CANVAS_ITEM].renames["SCREEN_TEXTURE"] = "screen_texture";
	actions[RS::SHADER_CANVAS_ITEM].renames["SCREEN_PIXEL_SIZE"] = "screen_pixel_size";
	actions[RS::SHADER_CANVAS_ITEM].renames["FRAGCOORD"] = "gl_FragCoord";
	actions[RS::SHADER_CANVAS_ITEM].renames["POINT_COORD"] = "gl_PointCoord";
	actions[RS::SHADER_CANVAS_ITEM].renames["INSTANCE_ID"] = "0";
	actions[RS::SHADER_CANVAS_ITEM].renames["VERTEX_ID"] = "0";

	actions[RS::SHADER_CANVAS_ITEM].renames["LIGHT_VEC"] = "light_vec";
	actions[RS::SHADER_CANVAS_ITEM].renames["LIGHT_HEIGHT"] = "light_height";
	actions[RS::SHADER_CANVAS_ITEM].renames["LIGHT_COLOR"] = "light_color";
	actions[RS::SHADER_CANVAS_ITEM].renames["LIGHT_UV"] = "light_uv";
	actions[RS::SHADER_CANVAS_ITEM].renames["LIGHT"] = "light";
	actions[RS::SHADER_CANVAS_ITEM].renames["SHADOW_COLOR"] = "shadow_color";
	actions[RS::SHADER_CANVAS_ITEM].renames["SHADOW_VEC"] = "shadow_vec";

	actions[RS::SHADER_CANVAS_ITEM].usage_defines["COLOR"] = "#define COLOR_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["MODULATE"] = "#define MODULATE_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["SCREEN_TEXTURE"] = "#define SCREEN_TEXTURE_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["SCREEN_UV"] = "#define SCREEN_UV_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["SCREEN_PIXEL_SIZE"] = "@SCREEN_UV";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["NORMAL"] = "#define NORMAL_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["NORMALMAP"] = "#define NORMALMAP_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["LIGHT"] = "#define USE_LIGHT_SHADER_CODE\n";
	actions[RS::SHADER_CANVAS_ITEM].render_mode_defines["skip_vertex_transform"] = "#define SKIP_TRANSFORM_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["SHADOW_VEC"] = "#define SHADOW_VEC_USED\n";

	// Ported from GLES3

	actions[RS::SHADER_CANVAS_ITEM].usage_defines["sinh"] = "#define SINH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["cosh"] = "#define COSH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["tanh"] = "#define TANH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["asinh"] = "#define ASINH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["acosh"] = "#define ACOSH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["atanh"] = "#define ATANH_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["determinant"] = "#define DETERMINANT_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["transpose"] = "#define TRANSPOSE_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["outerProduct"] = "#define OUTER_PRODUCT_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["round"] = "#define ROUND_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["roundEven"] = "#define ROUND_EVEN_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["inverse"] = "#define INVERSE_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["isinf"] = "#define IS_INF_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["isnan"] = "#define IS_NAN_USED\n";
	actions[RS::SHADER_CANVAS_ITEM].usage_defines["trunc"] = "#define TRUNC_USED\n";

	/** SPATIAL SHADER **/

	actions[RS::SHADER_SPATIAL].renames["WORLD_MATRIX"] = "world_transform";
	actions[RS::SHADER_SPATIAL].renames["INV_CAMERA_MATRIX"] = "camera_inverse_matrix";
	actions[RS::SHADER_SPATIAL].renames["CAMERA_MATRIX"] = "camera_matrix";
	actions[RS::SHADER_SPATIAL].renames["PROJECTION_MATRIX"] = "projection_matrix";
	actions[RS::SHADER_SPATIAL].renames["INV_PROJECTION_MATRIX"] = "projection_inverse_matrix";
	actions[RS::SHADER_SPATIAL].renames["MODELVIEW_MATRIX"] = "modelview";

	actions[RS::SHADER_SPATIAL].renames["VIEW_INDEX"] = "view_index";
	actions[RS::SHADER_SPATIAL].renames["VIEW_MONO_LEFT"] = "0";
	actions[RS::SHADER_SPATIAL].renames["VIEW_RIGHT"] = "1";

	actions[RS::SHADER_SPATIAL].renames["VERTEX"] = "vertex.xyz";
	actions[RS::SHADER_SPATIAL].renames["NORMAL"] = "normal";
	actions[RS::SHADER_SPATIAL].renames["TANGENT"] = "tangent";
	actions[RS::SHADER_SPATIAL].renames["BINORMAL"] = "binormal";
	actions[RS::SHADER_SPATIAL].renames["POSITION"] = "position";
	actions[RS::SHADER_SPATIAL].renames["UV"] = "uv_interp";
	actions[RS::SHADER_SPATIAL].renames["UV2"] = "uv2_interp";
	actions[RS::SHADER_SPATIAL].renames["COLOR"] = "color_interp";
	actions[RS::SHADER_SPATIAL].renames["POINT_SIZE"] = "point_size";
	// gl_InstanceID and VERTEX_ID is not available in OpenGL ES 2.0
	actions[RS::SHADER_SPATIAL].renames["INSTANCE_ID"] = "0";
	actions[RS::SHADER_SPATIAL].renames["VERTEX_ID"] = "0";

	//builtins

	actions[RS::SHADER_SPATIAL].renames["TIME"] = "time";
	actions[RS::SHADER_SPATIAL].renames["VIEWPORT_SIZE"] = "viewport_size";

	actions[RS::SHADER_SPATIAL].renames["FRAGCOORD"] = "gl_FragCoord";
	actions[RS::SHADER_SPATIAL].renames["FRONT_FACING"] = "gl_FrontFacing";
	actions[RS::SHADER_SPATIAL].renames["NORMALMAP"] = "normalmap";
	actions[RS::SHADER_SPATIAL].renames["NORMALMAP_DEPTH"] = "normaldepth";
	actions[RS::SHADER_SPATIAL].renames["ALBEDO"] = "albedo";
	actions[RS::SHADER_SPATIAL].renames["ALPHA"] = "alpha";
	actions[RS::SHADER_SPATIAL].renames["METALLIC"] = "metallic";
	actions[RS::SHADER_SPATIAL].renames["SPECULAR"] = "specular";
	actions[RS::SHADER_SPATIAL].renames["ROUGHNESS"] = "roughness";
	actions[RS::SHADER_SPATIAL].renames["RIM"] = "rim";
	actions[RS::SHADER_SPATIAL].renames["RIM_TINT"] = "rim_tint";
	actions[RS::SHADER_SPATIAL].renames["CLEARCOAT"] = "clearcoat";
	actions[RS::SHADER_SPATIAL].renames["CLEARCOAT_GLOSS"] = "clearcoat_gloss";
	actions[RS::SHADER_SPATIAL].renames["ANISOTROPY"] = "anisotropy";
	actions[RS::SHADER_SPATIAL].renames["ANISOTROPY_FLOW"] = "anisotropy_flow";
	actions[RS::SHADER_SPATIAL].renames["SSS_STRENGTH"] = "sss_strength";
	actions[RS::SHADER_SPATIAL].renames["TRANSMISSION"] = "transmission";
	actions[RS::SHADER_SPATIAL].renames["AO"] = "ao";
	actions[RS::SHADER_SPATIAL].renames["AO_LIGHT_AFFECT"] = "ao_light_affect";
	actions[RS::SHADER_SPATIAL].renames["EMISSION"] = "emission";
	actions[RS::SHADER_SPATIAL].renames["POINT_COORD"] = "gl_PointCoord";
	actions[RS::SHADER_SPATIAL].renames["INSTANCE_CUSTOM"] = "instance_custom";
	actions[RS::SHADER_SPATIAL].renames["SCREEN_UV"] = "screen_uv";
	actions[RS::SHADER_SPATIAL].renames["SCREEN_TEXTURE"] = "screen_texture";
	actions[RS::SHADER_SPATIAL].renames["DEPTH_TEXTURE"] = "depth_texture";
	// Defined in GLES3, but not available in GLES2
	//actions[RS::SHADER_SPATIAL].renames["DEPTH"] = "gl_FragDepth";
	actions[RS::SHADER_SPATIAL].renames["ALPHA_SCISSOR"] = "alpha_scissor";
	actions[RS::SHADER_SPATIAL].renames["OUTPUT_IS_SRGB"] = "SHADER_IS_SRGB";
	actions[RS::SHADER_SPATIAL].renames["NODE_POSITION_WORLD"] = "world_transform[3].xyz";
	actions[RS::SHADER_SPATIAL].renames["CAMERA_POSITION_WORLD"] = "camera_matrix[3].xyz";
	actions[RS::SHADER_SPATIAL].renames["CAMERA_DIRECTION_WORLD"] = "camera_inverse_matrix[3].xyz";
	actions[RS::SHADER_SPATIAL].renames["NODE_POSITION_VIEW"] = "(camera_inverse_matrix * world_transform)[3].xyz";

	//for light
	actions[RS::SHADER_SPATIAL].renames["VIEW"] = "view";
	actions[RS::SHADER_SPATIAL].renames["LIGHT_COLOR"] = "light_color";
	actions[RS::SHADER_SPATIAL].renames["LIGHT"] = "light";
	actions[RS::SHADER_SPATIAL].renames["ATTENUATION"] = "attenuation";
	actions[RS::SHADER_SPATIAL].renames["DIFFUSE_LIGHT"] = "diffuse_light";
	actions[RS::SHADER_SPATIAL].renames["SPECULAR_LIGHT"] = "specular_light";

	actions[RS::SHADER_SPATIAL].usage_defines["TANGENT"] = "#define ENABLE_TANGENT_INTERP\n";
	actions[RS::SHADER_SPATIAL].usage_defines["BINORMAL"] = "@TANGENT";
	actions[RS::SHADER_SPATIAL].usage_defines["RIM"] = "#define LIGHT_USE_RIM\n";
	actions[RS::SHADER_SPATIAL].usage_defines["RIM_TINT"] = "@RIM";
	actions[RS::SHADER_SPATIAL].usage_defines["CLEARCOAT"] = "#define LIGHT_USE_CLEARCOAT\n";
	actions[RS::SHADER_SPATIAL].usage_defines["CLEARCOAT_GLOSS"] = "@CLEARCOAT";
	actions[RS::SHADER_SPATIAL].usage_defines["ANISOTROPY"] = "#define LIGHT_USE_ANISOTROPY\n";
	actions[RS::SHADER_SPATIAL].usage_defines["ANISOTROPY_FLOW"] = "@ANISOTROPY";
	actions[RS::SHADER_SPATIAL].usage_defines["AO"] = "#define ENABLE_AO\n";
	actions[RS::SHADER_SPATIAL].usage_defines["AO_LIGHT_AFFECT"] = "#define ENABLE_AO\n";
	actions[RS::SHADER_SPATIAL].usage_defines["UV"] = "#define ENABLE_UV_INTERP\n";
	actions[RS::SHADER_SPATIAL].usage_defines["UV2"] = "#define ENABLE_UV2_INTERP\n";
	actions[RS::SHADER_SPATIAL].usage_defines["NORMALMAP"] = "#define ENABLE_NORMALMAP\n";
	actions[RS::SHADER_SPATIAL].usage_defines["NORMALMAP_DEPTH"] = "@NORMALMAP";
	actions[RS::SHADER_SPATIAL].usage_defines["COLOR"] = "#define ENABLE_COLOR_INTERP\n";
	actions[RS::SHADER_SPATIAL].usage_defines["INSTANCE_CUSTOM"] = "#define ENABLE_INSTANCE_CUSTOM\n";
	actions[RS::SHADER_SPATIAL].usage_defines["ALPHA_SCISSOR"] = "#define ALPHA_SCISSOR_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["POSITION"] = "#define OVERRIDE_POSITION\n";

	actions[RS::SHADER_SPATIAL].usage_defines["SSS_STRENGTH"] = "#define ENABLE_SSS\n";
	actions[RS::SHADER_SPATIAL].usage_defines["TRANSMISSION"] = "#define TRANSMISSION_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["SCREEN_TEXTURE"] = "#define SCREEN_TEXTURE_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["DEPTH_TEXTURE"] = "#define DEPTH_TEXTURE_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["SCREEN_UV"] = "#define SCREEN_UV_USED\n";

	actions[RS::SHADER_SPATIAL].usage_defines["DIFFUSE_LIGHT"] = "#define USE_LIGHT_SHADER_CODE\n";
	actions[RS::SHADER_SPATIAL].usage_defines["SPECULAR_LIGHT"] = "#define USE_LIGHT_SHADER_CODE\n";

	// Ported from GLES3

	actions[RS::SHADER_SPATIAL].usage_defines["sinh"] = "#define SINH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["cosh"] = "#define COSH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["tanh"] = "#define TANH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["asinh"] = "#define ASINH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["acosh"] = "#define ACOSH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["atanh"] = "#define ATANH_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["determinant"] = "#define DETERMINANT_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["transpose"] = "#define TRANSPOSE_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["outerProduct"] = "#define OUTER_PRODUCT_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["round"] = "#define ROUND_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["roundEven"] = "#define ROUND_EVEN_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["inverse"] = "#define INVERSE_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["isinf"] = "#define IS_INF_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["isnan"] = "#define IS_NAN_USED\n";
	actions[RS::SHADER_SPATIAL].usage_defines["trunc"] = "#define TRUNC_USED\n";

	actions[RS::SHADER_SPATIAL].render_mode_defines["skip_vertex_transform"] = "#define SKIP_TRANSFORM_USED\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["world_vertex_coords"] = "#define VERTEX_WORLD_COORDS_USED\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["ensure_correct_normals"] = "#define ENSURE_CORRECT_NORMALS\n";
	// Defined in GLES3, might not be possible in GLES2 as gl_FrontFacing is not available
	//actions[RS::SHADER_SPATIAL].render_mode_defines["cull_front"] = "#define DO_SIDE_CHECK\n";
	//actions[RS::SHADER_SPATIAL].render_mode_defines["cull_disabled"] = "#define DO_SIDE_CHECK\n";

	GLOBAL_CACHED(render_force_lambert_over_burley, bool, "rendering/quality/shading/force_lambert_over_burley");

	if (!render_force_lambert_over_burley) {
		actions[RS::SHADER_SPATIAL].render_mode_defines["diffuse_burley"] = "#define DIFFUSE_BURLEY\n";
	}

	actions[RS::SHADER_SPATIAL].render_mode_defines["diffuse_oren_nayar"] = "#define DIFFUSE_OREN_NAYAR\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["diffuse_lambert_wrap"] = "#define DIFFUSE_LAMBERT_WRAP\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["diffuse_toon"] = "#define DIFFUSE_TOON\n";

	GLOBAL_CACHED(render_force_blinn_over_ggx, bool, "rendering/quality/shading/force_blinn_over_ggx");

	if (!render_force_blinn_over_ggx) {
		actions[RS::SHADER_SPATIAL].render_mode_defines["specular_schlick_ggx"] = "#define SPECULAR_SCHLICK_GGX\n";
	} else {
		actions[RS::SHADER_SPATIAL].render_mode_defines["specular_schlick_ggx"] = "#define SPECULAR_BLINN\n";
	}

	actions[RS::SHADER_SPATIAL].render_mode_defines["specular_blinn"] = "#define SPECULAR_BLINN\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["specular_phong"] = "#define SPECULAR_PHONG\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["specular_toon"] = "#define SPECULAR_TOON\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["specular_disabled"] = "#define SPECULAR_DISABLED\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["shadows_disabled"] = "#define SHADOWS_DISABLED\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["ambient_light_disabled"] = "#define AMBIENT_LIGHT_DISABLED\n";
	actions[RS::SHADER_SPATIAL].render_mode_defines["shadow_to_opacity"] = "#define USE_SHADOW_TO_OPACITY\n";

	// No defines for particle shaders in GLES2, there are no GPU particles

	vertex_name = "vertex";
	fragment_name = "fragment";
	light_name = "light";
	time_name = "TIME";

	List<String> func_list;

	ShaderLanguage::get_builtin_funcs(&func_list);

	for (List<String>::Element *E = func_list.front(); E; E = E->next()) {
		internal_functions.insert(E->get());
	}
}
