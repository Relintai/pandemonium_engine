/*************************************************************************/
/*  pdscript_compiler.cpp                                                */
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

#include "pdscript_compiler.h"

#include "pdscript.h"

bool PDScriptCompiler::_is_class_member_property(CodeGen &codegen, const StringName &p_name) {
	if (codegen.function_node && codegen.function_node->_static) {
		return false;
	}

	if (codegen.stack_identifiers.has(p_name)) {
		return false; //shadowed
	}

	return _is_class_member_property(codegen.script, p_name);
}

bool PDScriptCompiler::_is_class_member_property(PDScript *owner, const StringName &p_name) {
	PDScript *scr = owner;
	PDScriptNativeClass *nc = nullptr;
	while (scr) {
		if (scr->native.is_valid()) {
			nc = scr->native.ptr();
		}
		scr = scr->_base;
	}

	ERR_FAIL_COND_V(!nc, false);

	return ClassDB::has_property(nc->get_name(), p_name);
}

void PDScriptCompiler::_set_error(const String &p_error, const PDScriptParser::Node *p_node) {
	if (error != "") {
		return;
	}

	error = p_error;
	if (p_node) {
		err_line = p_node->line;
		err_column = p_node->column;
	} else {
		err_line = 0;
		err_column = 0;
	}
}

bool PDScriptCompiler::_create_unary_operator(CodeGen &codegen, const PDScriptParser::OperatorNode *on, Variant::Operator op, int p_stack_level) {
	ERR_FAIL_COND_V(on->arguments.size() != 1, false);

	int src_address_a = _parse_expression(codegen, on->arguments[0], p_stack_level);
	if (src_address_a < 0) {
		return false;
	}

	codegen.opcodes.push_back(PDScriptFunction::OPCODE_OPERATOR); // perform operator
	codegen.opcodes.push_back(op); //which operator
	codegen.opcodes.push_back(src_address_a); // argument 1
	codegen.opcodes.push_back(src_address_a); // argument 2 (repeated)
	//codegen.opcodes.push_back(PDScriptFunction::ADDR_TYPE_NIL); // argument 2 (unary only takes one parameter)
	return true;
}

bool PDScriptCompiler::_create_binary_operator(CodeGen &codegen, const PDScriptParser::OperatorNode *on, Variant::Operator op, int p_stack_level, bool p_initializer, int p_index_addr) {
	ERR_FAIL_COND_V(on->arguments.size() != 2, false);

	int src_address_a = _parse_expression(codegen, on->arguments[0], p_stack_level, false, p_initializer, p_index_addr);
	if (src_address_a < 0) {
		return false;
	}
	if (src_address_a & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
		p_stack_level++; //uses stack for return, increase stack
	}

	int src_address_b = _parse_expression(codegen, on->arguments[1], p_stack_level, false, p_initializer);
	if (src_address_b < 0) {
		return false;
	}

	codegen.opcodes.push_back(PDScriptFunction::OPCODE_OPERATOR); // perform operator
	codegen.opcodes.push_back(op); //which operator
	codegen.opcodes.push_back(src_address_a); // argument 1
	codegen.opcodes.push_back(src_address_b); // argument 2 (unary only takes one parameter)
	return true;
}

PDScriptDataType PDScriptCompiler::_pdtype_from_datatype(const PDScriptParser::DataType &p_datatype, PDScript *p_owner) const {
	if (!p_datatype.has_type) {
		return PDScriptDataType();
	}

	PDScriptDataType result;
	result.has_type = true;

	switch (p_datatype.kind) {
		case PDScriptParser::DataType::BUILTIN: {
			result.kind = PDScriptDataType::BUILTIN;
			result.builtin_type = p_datatype.builtin_type;
		} break;
		case PDScriptParser::DataType::NATIVE: {
			result.kind = PDScriptDataType::NATIVE;
			result.native_type = p_datatype.native_type;
		} break;
		case PDScriptParser::DataType::SCRIPT: {
			result.kind = PDScriptDataType::SCRIPT;
			result.script_type_ref = Ref<Script>(p_datatype.script_type);
			result.script_type = result.script_type_ref.ptr();
			result.native_type = result.script_type->get_instance_base_type();
		} break;
		case PDScriptParser::DataType::PDSCRIPT: {
			result.kind = PDScriptDataType::PDSCRIPT;
			result.script_type_ref = Ref<Script>(p_datatype.script_type);
			result.script_type = result.script_type_ref.ptr();
			result.native_type = result.script_type->get_instance_base_type();
		} break;
		case PDScriptParser::DataType::CLASS: {
			// Locate class by constructing the path to it and following that path
			PDScriptParser::ClassNode *class_type = p_datatype.class_type;
			List<StringName> names;
			while (class_type->owner) {
				names.push_back(class_type->name);
				class_type = class_type->owner;
			}

			Ref<PDScript> script = Ref<PDScript>(main_script);
			while (names.back()) {
				if (!script->subclasses.has(names.back()->get())) {
					ERR_PRINT("Parser bug: Cannot locate datatype class.");
					result.has_type = false;
					return PDScriptDataType();
				}
				script = script->subclasses[names.back()->get()];
				names.pop_back();
			}

			result.kind = PDScriptDataType::PDSCRIPT;
			result.script_type_ref = Ref<Script>(script);
			result.script_type = result.script_type_ref.ptr();
			result.native_type = script->get_instance_base_type();
		} break;
		default: {
			ERR_PRINT("Parser bug: converting unresolved type.");
			return PDScriptDataType();
		}
	}

	// Only hold strong reference to the script if it's not the owner of the
	// element qualified with this type, to avoid cyclic references (leaks).
	if (result.script_type && result.script_type == p_owner) {
		result.script_type_ref = Ref<Script>();
	}

	return result;
}

int PDScriptCompiler::_parse_assign_right_expression(CodeGen &codegen, const PDScriptParser::OperatorNode *p_expression, int p_stack_level, int p_index_addr) {
	Variant::Operator var_op = Variant::OP_MAX;

	switch (p_expression->op) {
		case PDScriptParser::OperatorNode::OP_ASSIGN_ADD:
			var_op = Variant::OP_ADD;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_SUB:
			var_op = Variant::OP_SUBTRACT;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_MUL:
			var_op = Variant::OP_MULTIPLY;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_DIV:
			var_op = Variant::OP_DIVIDE;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_MOD:
			var_op = Variant::OP_MODULE;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_SHIFT_LEFT:
			var_op = Variant::OP_SHIFT_LEFT;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_SHIFT_RIGHT:
			var_op = Variant::OP_SHIFT_RIGHT;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_AND:
			var_op = Variant::OP_BIT_AND;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_OR:
			var_op = Variant::OP_BIT_OR;
			break;
		case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_XOR:
			var_op = Variant::OP_BIT_XOR;
			break;
		case PDScriptParser::OperatorNode::OP_INIT_ASSIGN:
		case PDScriptParser::OperatorNode::OP_ASSIGN: {
			//none
		} break;
		default: {
			ERR_FAIL_V(-1);
		}
	}

	bool initializer = p_expression->op == PDScriptParser::OperatorNode::OP_INIT_ASSIGN;

	if (var_op == Variant::OP_MAX) {
		return _parse_expression(codegen, p_expression->arguments[1], p_stack_level, false, initializer);
	}

	if (!_create_binary_operator(codegen, p_expression, var_op, p_stack_level, initializer, p_index_addr)) {
		return -1;
	}

	int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
	codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
	codegen.alloc_stack(p_stack_level);
	return dst_addr;
}

int PDScriptCompiler::_parse_expression(CodeGen &codegen, const PDScriptParser::Node *p_expression, int p_stack_level, bool p_root, bool p_initializer, int p_index_addr) {
	switch (p_expression->type) {
		//should parse variable declaration and adjust stack accordingly...
		case PDScriptParser::Node::TYPE_IDENTIFIER: {
			//return identifier
			//wait, identifier could be a local variable or something else... careful here, must reference properly
			//as stack may be more interesting to work with

			//This could be made much simpler by just indexing "self", but done this way (with custom self-addressing modes) increases performance a lot.

			const PDScriptParser::IdentifierNode *in = static_cast<const PDScriptParser::IdentifierNode *>(p_expression);

			StringName identifier = in->name;

			// TRY STACK!
			if (!p_initializer && codegen.stack_identifiers.has(identifier)) {
				int pos = codegen.stack_identifiers[identifier];
				return pos | (PDScriptFunction::ADDR_TYPE_STACK_VARIABLE << PDScriptFunction::ADDR_BITS);
			}

			// TRY CLASS MEMBER
			if (_is_class_member_property(codegen, identifier)) {
				//get property
				codegen.opcodes.push_back(PDScriptFunction::OPCODE_GET_MEMBER); // perform operator
				codegen.opcodes.push_back(codegen.get_name_map_pos(identifier)); // argument 2 (unary only takes one parameter)
				int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
				codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
				codegen.alloc_stack(p_stack_level);
				return dst_addr;
			}

			//TRY MEMBERS!
			if (!codegen.function_node || !codegen.function_node->_static) {
				// TRY MEMBER VARIABLES!
				//static function
				if (codegen.script->member_indices.has(identifier)) {
					int idx = codegen.script->member_indices[identifier].index;
					return idx | (PDScriptFunction::ADDR_TYPE_MEMBER << PDScriptFunction::ADDR_BITS); //argument (stack root)
				}
			}

			//TRY CLASS CONSTANTS

			PDScript *owner = codegen.script;
			while (owner) {
				PDScript *scr = owner;
				PDScriptNativeClass *nc = nullptr;
				while (scr) {
					if (scr->constants.has(identifier)) {
						//int idx=scr->constants[identifier];
						int idx = codegen.get_name_map_pos(identifier);
						return idx | (PDScriptFunction::ADDR_TYPE_CLASS_CONSTANT << PDScriptFunction::ADDR_BITS); //argument (stack root)
					}
					if (scr->native.is_valid()) {
						nc = scr->native.ptr();
					}
					scr = scr->_base;
				}

				// CLASS C++ Integer Constant

				if (nc) {
					bool success = false;
					int constant = ClassDB::get_integer_constant(nc->get_name(), identifier, &success);
					if (success) {
						Variant key = constant;
						int idx;

						if (!codegen.constant_map.has(key)) {
							idx = codegen.constant_map.size();
							codegen.constant_map[key] = idx;

						} else {
							idx = codegen.constant_map[key];
						}

						return idx | (PDScriptFunction::ADDR_TYPE_LOCAL_CONSTANT << PDScriptFunction::ADDR_BITS); //make it a local constant (faster access)
					}
				}

				owner = owner->_owner;
			}

			if (PDScriptLanguage::get_singleton()->get_global_map().has(identifier)) {
				int idx = PDScriptLanguage::get_singleton()->get_global_map()[identifier];
				return idx | (PDScriptFunction::ADDR_TYPE_GLOBAL << PDScriptFunction::ADDR_BITS); //argument (stack root)
			}

			/* TRY GLOBAL CLASSES */

			if (ScriptServer::is_global_class(identifier)) {
				const PDScriptParser::ClassNode *class_node = codegen.class_node;
				while (class_node->owner) {
					class_node = class_node->owner;
				}

				if (class_node->name == identifier) {
					_set_error("Using own name in class file is not allowed (creates a cyclic reference)", p_expression);
					return -1;
				}

				RES res = ResourceLoader::load(ScriptServer::get_global_class_path(identifier));
				if (res.is_null()) {
					_set_error("Can't load global class " + String(identifier) + ", cyclic reference?", p_expression);
					return -1;
				}

				Variant key = res;
				int idx;

				if (!codegen.constant_map.has(key)) {
					idx = codegen.constant_map.size();
					codegen.constant_map[key] = idx;

				} else {
					idx = codegen.constant_map[key];
				}

				return idx | (PDScriptFunction::ADDR_TYPE_LOCAL_CONSTANT << PDScriptFunction::ADDR_BITS); //make it a local constant (faster access)
			}

#ifdef TOOLS_ENABLED
			if (PDScriptLanguage::get_singleton()->get_named_globals_map().has(identifier)) {
				int idx = codegen.named_globals.find(identifier);
				if (idx == -1) {
					idx = codegen.named_globals.size();
					codegen.named_globals.push_back(identifier);
				}
				return idx | (PDScriptFunction::ADDR_TYPE_NAMED_GLOBAL << PDScriptFunction::ADDR_BITS);
			}
#endif

			//not found, error

			_set_error("Identifier not found: " + String(identifier), p_expression);

			return -1;

		} break;
		case PDScriptParser::Node::TYPE_CONSTANT: {
			//return constant
			const PDScriptParser::ConstantNode *cn = static_cast<const PDScriptParser::ConstantNode *>(p_expression);

			int idx;

			if (!codegen.constant_map.has(cn->value)) {
				idx = codegen.constant_map.size();
				codegen.constant_map[cn->value] = idx;

			} else {
				idx = codegen.constant_map[cn->value];
			}

			return idx | (PDScriptFunction::ADDR_TYPE_LOCAL_CONSTANT << PDScriptFunction::ADDR_BITS); //argument (stack root)

		} break;
		case PDScriptParser::Node::TYPE_SELF: {
			//return constant
			if (codegen.function_node && codegen.function_node->_static) {
				_set_error("'self' not present in static function!", p_expression);
				return -1;
			}
			return (PDScriptFunction::ADDR_TYPE_SELF << PDScriptFunction::ADDR_BITS);
		} break;
		case PDScriptParser::Node::TYPE_ARRAY: {
			const PDScriptParser::ArrayNode *an = static_cast<const PDScriptParser::ArrayNode *>(p_expression);
			Vector<int> values;

			int slevel = p_stack_level;

			for (int i = 0; i < an->elements.size(); i++) {
				int ret = _parse_expression(codegen, an->elements[i], slevel);
				if (ret < 0) {
					return ret;
				}
				if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
					slevel++;
					codegen.alloc_stack(slevel);
				}

				values.push_back(ret);
			}

			codegen.opcodes.push_back(PDScriptFunction::OPCODE_CONSTRUCT_ARRAY);
			codegen.opcodes.push_back(values.size());
			for (int i = 0; i < values.size(); i++) {
				codegen.opcodes.push_back(values[i]);
			}

			int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
			codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
			codegen.alloc_stack(p_stack_level);
			return dst_addr;

		} break;
		case PDScriptParser::Node::TYPE_DICTIONARY: {
			const PDScriptParser::DictionaryNode *dn = static_cast<const PDScriptParser::DictionaryNode *>(p_expression);
			Vector<int> values;

			int slevel = p_stack_level;

			for (int i = 0; i < dn->elements.size(); i++) {
				int ret = _parse_expression(codegen, dn->elements[i].key, slevel);
				if (ret < 0) {
					return ret;
				}
				if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
					slevel++;
					codegen.alloc_stack(slevel);
				}

				values.push_back(ret);

				ret = _parse_expression(codegen, dn->elements[i].value, slevel);
				if (ret < 0) {
					return ret;
				}
				if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
					slevel++;
					codegen.alloc_stack(slevel);
				}

				values.push_back(ret);
			}

			codegen.opcodes.push_back(PDScriptFunction::OPCODE_CONSTRUCT_DICTIONARY);
			codegen.opcodes.push_back(dn->elements.size());
			for (int i = 0; i < values.size(); i++) {
				codegen.opcodes.push_back(values[i]);
			}

			int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
			codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
			codegen.alloc_stack(p_stack_level);
			return dst_addr;

		} break;
		case PDScriptParser::Node::TYPE_CAST: {
			const PDScriptParser::CastNode *cn = static_cast<const PDScriptParser::CastNode *>(p_expression);

			int slevel = p_stack_level;
			int src_addr = _parse_expression(codegen, cn->source_node, slevel);
			if (src_addr < 0) {
				return src_addr;
			}
			if (src_addr & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
				slevel++;
				codegen.alloc_stack(slevel);
			}

			PDScriptDataType cast_type = _pdtype_from_datatype(cn->cast_type);

			switch (cast_type.kind) {
				case PDScriptDataType::BUILTIN: {
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_CAST_TO_BUILTIN);
					codegen.opcodes.push_back(cast_type.builtin_type);
				} break;
				case PDScriptDataType::NATIVE: {
					int class_idx;
					if (PDScriptLanguage::get_singleton()->get_global_map().has(cast_type.native_type)) {
						class_idx = PDScriptLanguage::get_singleton()->get_global_map()[cast_type.native_type];
						class_idx |= (PDScriptFunction::ADDR_TYPE_GLOBAL << PDScriptFunction::ADDR_BITS); //argument (stack root)
					} else {
						_set_error("Invalid native class type '" + String(cast_type.native_type) + "'.", cn);
						return -1;
					}
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_CAST_TO_NATIVE); // perform operator
					codegen.opcodes.push_back(class_idx); // variable type
				} break;
				case PDScriptDataType::SCRIPT:
				case PDScriptDataType::PDSCRIPT: {
					Variant script = cast_type.script_type;
					int idx = codegen.get_constant_pos(script);
					idx |= PDScriptFunction::ADDR_TYPE_LOCAL_CONSTANT << PDScriptFunction::ADDR_BITS; //make it a local constant (faster access)

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_CAST_TO_SCRIPT); // perform operator
					codegen.opcodes.push_back(idx); // variable type
				} break;
				default: {
					_set_error("Parser bug: unresolved data type.", cn);
					return -1;
				}
			}

			codegen.opcodes.push_back(src_addr); // source address
			int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
			codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
			codegen.alloc_stack(p_stack_level);
			return dst_addr;

		} break;
		case PDScriptParser::Node::TYPE_OPERATOR: {
			//hell breaks loose

			const PDScriptParser::OperatorNode *on = static_cast<const PDScriptParser::OperatorNode *>(p_expression);
			switch (on->op) {
				//call/constructor operator
				case PDScriptParser::OperatorNode::OP_PARENT_CALL: {
					ERR_FAIL_COND_V(on->arguments.size() < 1, -1);

					const PDScriptParser::IdentifierNode *in = (const PDScriptParser::IdentifierNode *)on->arguments[0];

					Vector<int> arguments;
					int slevel = p_stack_level;
					for (int i = 1; i < on->arguments.size(); i++) {
						int ret = _parse_expression(codegen, on->arguments[i], slevel);
						if (ret < 0) {
							return ret;
						}
						if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
							slevel++;
							codegen.alloc_stack(slevel);
						}
						arguments.push_back(ret);
					}

					//push call bytecode
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_CALL_SELF_BASE); // basic type constructor

					codegen.opcodes.push_back(codegen.get_name_map_pos(in->name)); //instance
					codegen.opcodes.push_back(arguments.size()); //argument count
					codegen.alloc_call(arguments.size());
					for (int i = 0; i < arguments.size(); i++) {
						codegen.opcodes.push_back(arguments[i]); //arguments
					}

				} break;
				case PDScriptParser::OperatorNode::OP_CALL: {
					if (on->arguments[0]->type == PDScriptParser::Node::TYPE_TYPE) {
						//construct a basic type
						ERR_FAIL_COND_V(on->arguments.size() < 1, -1);

						const PDScriptParser::TypeNode *tn = (const PDScriptParser::TypeNode *)on->arguments[0];
						int vtype = tn->vtype;

						Vector<int> arguments;
						int slevel = p_stack_level;
						for (int i = 1; i < on->arguments.size(); i++) {
							int ret = _parse_expression(codegen, on->arguments[i], slevel);
							if (ret < 0) {
								return ret;
							}
							if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
								slevel++;
								codegen.alloc_stack(slevel);
							}
							arguments.push_back(ret);
						}

						//push call bytecode
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_CONSTRUCT); // basic type constructor
						codegen.opcodes.push_back(vtype); //instance
						codegen.opcodes.push_back(arguments.size()); //argument count
						codegen.alloc_call(arguments.size());
						for (int i = 0; i < arguments.size(); i++) {
							codegen.opcodes.push_back(arguments[i]); //arguments
						}

					} else if (on->arguments[0]->type == PDScriptParser::Node::TYPE_BUILT_IN_FUNCTION) {
						//built in function

						ERR_FAIL_COND_V(on->arguments.size() < 1, -1);

						Vector<int> arguments;
						int slevel = p_stack_level;
						for (int i = 1; i < on->arguments.size(); i++) {
							int ret = _parse_expression(codegen, on->arguments[i], slevel);
							if (ret < 0) {
								return ret;
							}

							if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
								slevel++;
								codegen.alloc_stack(slevel);
							}

							arguments.push_back(ret);
						}

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_CALL_BUILT_IN);
						codegen.opcodes.push_back(static_cast<const PDScriptParser::BuiltInFunctionNode *>(on->arguments[0])->function);
						codegen.opcodes.push_back(on->arguments.size() - 1);
						codegen.alloc_call(on->arguments.size() - 1);
						for (int i = 0; i < arguments.size(); i++) {
							codegen.opcodes.push_back(arguments[i]);
						}

					} else {
						//regular function
						ERR_FAIL_COND_V(on->arguments.size() < 2, -1);

						const PDScriptParser::Node *instance = on->arguments[0];

						if (instance->type == PDScriptParser::Node::TYPE_SELF) {
							//room for optimization
						}

						Vector<int> arguments;
						int slevel = p_stack_level;

						for (int i = 0; i < on->arguments.size(); i++) {
							int ret;

							if (i == 0 && on->arguments[i]->type == PDScriptParser::Node::TYPE_SELF && codegen.function_node && codegen.function_node->_static) {
								//static call to self
								ret = (PDScriptFunction::ADDR_TYPE_CLASS << PDScriptFunction::ADDR_BITS);
							} else if (i == 1) {
								if (on->arguments[i]->type != PDScriptParser::Node::TYPE_IDENTIFIER) {
									_set_error("Attempt to call a non-identifier.", on);
									return -1;
								}
								PDScriptParser::IdentifierNode *id = static_cast<PDScriptParser::IdentifierNode *>(on->arguments[i]);
								ret = codegen.get_name_map_pos(id->name);

							} else {
								ret = _parse_expression(codegen, on->arguments[i], slevel);
								if (ret < 0) {
									return ret;
								}
								if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
									slevel++;
									codegen.alloc_stack(slevel);
								}
							}
							arguments.push_back(ret);
						}

						codegen.opcodes.push_back(p_root ? PDScriptFunction::OPCODE_CALL : PDScriptFunction::OPCODE_CALL_RETURN); // perform operator
						codegen.opcodes.push_back(on->arguments.size() - 2);
						codegen.alloc_call(on->arguments.size() - 2);
						for (int i = 0; i < arguments.size(); i++) {
							codegen.opcodes.push_back(arguments[i]);
						}
					}
				} break;
				case PDScriptParser::OperatorNode::OP_YIELD: {
					ERR_FAIL_COND_V(on->arguments.size() && on->arguments.size() != 2, -1);

					Vector<int> arguments;
					int slevel = p_stack_level;
					for (int i = 0; i < on->arguments.size(); i++) {
						int ret = _parse_expression(codegen, on->arguments[i], slevel);
						if (ret < 0) {
							return ret;
						}
						if ((ret >> PDScriptFunction::ADDR_BITS & PDScriptFunction::ADDR_TYPE_STACK) == PDScriptFunction::ADDR_TYPE_STACK) {
							slevel++;
							codegen.alloc_stack(slevel);
						}
						arguments.push_back(ret);
					}

					//push call bytecode
					codegen.opcodes.push_back(arguments.size() == 0 ? PDScriptFunction::OPCODE_YIELD : PDScriptFunction::OPCODE_YIELD_SIGNAL); // basic type constructor
					for (int i = 0; i < arguments.size(); i++) {
						codegen.opcodes.push_back(arguments[i]); //arguments
					}
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_YIELD_RESUME);
					//next will be where to place the result :)

				} break;

				//indexing operator
				case PDScriptParser::OperatorNode::OP_INDEX:
				case PDScriptParser::OperatorNode::OP_INDEX_NAMED: {
					ERR_FAIL_COND_V(on->arguments.size() != 2, -1);

					int slevel = p_stack_level;
					bool named = (on->op == PDScriptParser::OperatorNode::OP_INDEX_NAMED);

					int from = _parse_expression(codegen, on->arguments[0], slevel);
					if (from < 0) {
						return from;
					}

					int index;
					if (p_index_addr != 0) {
						index = p_index_addr;
					} else if (named) {
						if (on->arguments[0]->type == PDScriptParser::Node::TYPE_SELF && codegen.script && codegen.function_node && !codegen.function_node->_static) {
							PDScriptParser::IdentifierNode *identifier = static_cast<PDScriptParser::IdentifierNode *>(on->arguments[1]);
							const RBMap<StringName, PDScript::MemberInfo>::Element *MI = codegen.script->member_indices.find(identifier->name);

#ifdef DEBUG_ENABLED
							if (MI && MI->get().getter == codegen.function_node->name) {
								String n = static_cast<PDScriptParser::IdentifierNode *>(on->arguments[1])->name;
								_set_error("Must use '" + n + "' instead of 'self." + n + "' in getter.", on);
								return -1;
							}
#endif

							if (MI && MI->get().getter == "") {
								// Faster than indexing self (as if no self. had been used)
								return (MI->get().index) | (PDScriptFunction::ADDR_TYPE_MEMBER << PDScriptFunction::ADDR_BITS);
							}
						}

						index = codegen.get_name_map_pos(static_cast<PDScriptParser::IdentifierNode *>(on->arguments[1])->name);

					} else {
						if (on->arguments[1]->type == PDScriptParser::Node::TYPE_CONSTANT && static_cast<const PDScriptParser::ConstantNode *>(on->arguments[1])->value.get_type() == Variant::STRING) {
							//also, somehow, named (speed up anyway)
							StringName name = static_cast<const PDScriptParser::ConstantNode *>(on->arguments[1])->value;
							index = codegen.get_name_map_pos(name);
							named = true;

						} else {
							//regular indexing
							if (from & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
								slevel++;
								codegen.alloc_stack(slevel);
							}

							index = _parse_expression(codegen, on->arguments[1], slevel);
							if (index < 0) {
								return index;
							}
						}
					}

					codegen.opcodes.push_back(named ? PDScriptFunction::OPCODE_GET_NAMED : PDScriptFunction::OPCODE_GET); // perform operator
					codegen.opcodes.push_back(from); // argument 1
					codegen.opcodes.push_back(index); // argument 2 (unary only takes one parameter)

				} break;
				case PDScriptParser::OperatorNode::OP_AND: {
					// AND operator with early out on failure

					int res = _parse_expression(codegen, on->arguments[0], p_stack_level);
					if (res < 0) {
						return res;
					}
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF_NOT);
					codegen.opcodes.push_back(res);
					int jump_fail_pos = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					res = _parse_expression(codegen, on->arguments[1], p_stack_level);
					if (res < 0) {
						return res;
					}

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF_NOT);
					codegen.opcodes.push_back(res);
					int jump_fail_pos2 = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					codegen.alloc_stack(p_stack_level); //it will be used..
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_TRUE);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
					codegen.opcodes.push_back(codegen.opcodes.size() + 3);
					codegen.opcodes.write[jump_fail_pos] = codegen.opcodes.size();
					codegen.opcodes.write[jump_fail_pos2] = codegen.opcodes.size();
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_FALSE);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					return p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS;

				} break;
				case PDScriptParser::OperatorNode::OP_OR: {
					// OR operator with early out on success

					int res = _parse_expression(codegen, on->arguments[0], p_stack_level);
					if (res < 0) {
						return res;
					}
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF);
					codegen.opcodes.push_back(res);
					int jump_success_pos = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					res = _parse_expression(codegen, on->arguments[1], p_stack_level);
					if (res < 0) {
						return res;
					}

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF);
					codegen.opcodes.push_back(res);
					int jump_success_pos2 = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					codegen.alloc_stack(p_stack_level); //it will be used..
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_FALSE);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
					codegen.opcodes.push_back(codegen.opcodes.size() + 3);
					codegen.opcodes.write[jump_success_pos] = codegen.opcodes.size();
					codegen.opcodes.write[jump_success_pos2] = codegen.opcodes.size();
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_TRUE);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					return p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS;

				} break;
				// ternary operators
				case PDScriptParser::OperatorNode::OP_TERNARY_IF: {
					// x IF a ELSE y operator with early out on failure

					int res = _parse_expression(codegen, on->arguments[0], p_stack_level);
					if (res < 0) {
						return res;
					}
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF_NOT);
					codegen.opcodes.push_back(res);
					int jump_fail_pos = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					res = _parse_expression(codegen, on->arguments[1], p_stack_level);
					if (res < 0) {
						return res;
					}

					codegen.alloc_stack(p_stack_level); //it will be used..
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					codegen.opcodes.push_back(res);
					codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
					int jump_past_pos = codegen.opcodes.size();
					codegen.opcodes.push_back(0);

					codegen.opcodes.write[jump_fail_pos] = codegen.opcodes.size();
					res = _parse_expression(codegen, on->arguments[2], p_stack_level);
					if (res < 0) {
						return res;
					}

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN);
					codegen.opcodes.push_back(p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
					codegen.opcodes.push_back(res);

					codegen.opcodes.write[jump_past_pos] = codegen.opcodes.size();

					return p_stack_level | PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS;

				} break;
				//unary operators
				case PDScriptParser::OperatorNode::OP_NEG: {
					if (!_create_unary_operator(codegen, on, Variant::OP_NEGATE, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_POS: {
					if (!_create_unary_operator(codegen, on, Variant::OP_POSITIVE, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_NOT: {
					if (!_create_unary_operator(codegen, on, Variant::OP_NOT, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_BIT_INVERT: {
					if (!_create_unary_operator(codegen, on, Variant::OP_BIT_NEGATE, p_stack_level)) {
						return -1;
					}
				} break;
				//binary operators (in precedence order)
				case PDScriptParser::OperatorNode::OP_IN: {
					if (!_create_binary_operator(codegen, on, Variant::OP_IN, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_EQUAL: {
					if (!_create_binary_operator(codegen, on, Variant::OP_EQUAL, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_NOT_EQUAL: {
					if (!_create_binary_operator(codegen, on, Variant::OP_NOT_EQUAL, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_LESS: {
					if (!_create_binary_operator(codegen, on, Variant::OP_LESS, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_LESS_EQUAL: {
					if (!_create_binary_operator(codegen, on, Variant::OP_LESS_EQUAL, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_GREATER: {
					if (!_create_binary_operator(codegen, on, Variant::OP_GREATER, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_GREATER_EQUAL: {
					if (!_create_binary_operator(codegen, on, Variant::OP_GREATER_EQUAL, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_ADD: {
					if (!_create_binary_operator(codegen, on, Variant::OP_ADD, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_SUB: {
					if (!_create_binary_operator(codegen, on, Variant::OP_SUBTRACT, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_MUL: {
					if (!_create_binary_operator(codegen, on, Variant::OP_MULTIPLY, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_DIV: {
					if (!_create_binary_operator(codegen, on, Variant::OP_DIVIDE, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_MOD: {
					if (!_create_binary_operator(codegen, on, Variant::OP_MODULE, p_stack_level)) {
						return -1;
					}
				} break;
				//case PDScriptParser::OperatorNode::OP_SHIFT_LEFT: { if (!_create_binary_operator(codegen,on,Variant::OP_SHIFT_LEFT,p_stack_level)) return -1;} break;
				//case PDScriptParser::OperatorNode::OP_SHIFT_RIGHT: { if (!_create_binary_operator(codegen,on,Variant::OP_SHIFT_RIGHT,p_stack_level)) return -1;} break;
				case PDScriptParser::OperatorNode::OP_BIT_AND: {
					if (!_create_binary_operator(codegen, on, Variant::OP_BIT_AND, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_BIT_OR: {
					if (!_create_binary_operator(codegen, on, Variant::OP_BIT_OR, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_BIT_XOR: {
					if (!_create_binary_operator(codegen, on, Variant::OP_BIT_XOR, p_stack_level)) {
						return -1;
					}
				} break;
				//shift
				case PDScriptParser::OperatorNode::OP_SHIFT_LEFT: {
					if (!_create_binary_operator(codegen, on, Variant::OP_SHIFT_LEFT, p_stack_level)) {
						return -1;
					}
				} break;
				case PDScriptParser::OperatorNode::OP_SHIFT_RIGHT: {
					if (!_create_binary_operator(codegen, on, Variant::OP_SHIFT_RIGHT, p_stack_level)) {
						return -1;
					}
				} break;
				//assignment operators
				case PDScriptParser::OperatorNode::OP_ASSIGN_ADD:
				case PDScriptParser::OperatorNode::OP_ASSIGN_SUB:
				case PDScriptParser::OperatorNode::OP_ASSIGN_MUL:
				case PDScriptParser::OperatorNode::OP_ASSIGN_DIV:
				case PDScriptParser::OperatorNode::OP_ASSIGN_MOD:
				case PDScriptParser::OperatorNode::OP_ASSIGN_SHIFT_LEFT:
				case PDScriptParser::OperatorNode::OP_ASSIGN_SHIFT_RIGHT:
				case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_AND:
				case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_OR:
				case PDScriptParser::OperatorNode::OP_ASSIGN_BIT_XOR:
				case PDScriptParser::OperatorNode::OP_INIT_ASSIGN:
				case PDScriptParser::OperatorNode::OP_ASSIGN: {
					ERR_FAIL_COND_V(on->arguments.size() != 2, -1);

					if (on->arguments[0]->type == PDScriptParser::Node::TYPE_OPERATOR && (static_cast<PDScriptParser::OperatorNode *>(on->arguments[0])->op == PDScriptParser::OperatorNode::OP_INDEX || static_cast<PDScriptParser::OperatorNode *>(on->arguments[0])->op == PDScriptParser::OperatorNode::OP_INDEX_NAMED)) {
						// SET (chained) MODE!
#ifdef DEBUG_ENABLED
						if (static_cast<PDScriptParser::OperatorNode *>(on->arguments[0])->op == PDScriptParser::OperatorNode::OP_INDEX_NAMED) {
							const PDScriptParser::OperatorNode *inon = static_cast<PDScriptParser::OperatorNode *>(on->arguments[0]);

							if (inon->arguments[0]->type == PDScriptParser::Node::TYPE_SELF && codegen.script && codegen.function_node && !codegen.function_node->_static) {
								const RBMap<StringName, PDScript::MemberInfo>::Element *MI = codegen.script->member_indices.find(static_cast<PDScriptParser::IdentifierNode *>(inon->arguments[1])->name);
								if (MI && MI->get().setter == codegen.function_node->name) {
									String n = static_cast<PDScriptParser::IdentifierNode *>(inon->arguments[1])->name;
									_set_error("Must use '" + n + "' instead of 'self." + n + "' in setter.", inon);
									return -1;
								}
							}
						}
#endif

						int slevel = p_stack_level;

						PDScriptParser::OperatorNode *op = static_cast<PDScriptParser::OperatorNode *>(on->arguments[0]);

						/* Find chain of sets */

						StringName assign_property;

						List<PDScriptParser::OperatorNode *> chain;

						{
							//create get/set chain
							PDScriptParser::OperatorNode *n = op;
							while (true) {
								chain.push_back(n);
								if (n->arguments[0]->type != PDScriptParser::Node::TYPE_OPERATOR) {
									//check for a built-in property
									if (n->arguments[0]->type == PDScriptParser::Node::TYPE_IDENTIFIER) {
										PDScriptParser::IdentifierNode *identifier = static_cast<PDScriptParser::IdentifierNode *>(n->arguments[0]);
										if (_is_class_member_property(codegen, identifier->name)) {
											assign_property = identifier->name;
										}
									}
									break;
								}
								n = static_cast<PDScriptParser::OperatorNode *>(n->arguments[0]);
								if (n->op != PDScriptParser::OperatorNode::OP_INDEX && n->op != PDScriptParser::OperatorNode::OP_INDEX_NAMED) {
									break;
								}
							}
						}

						/* Chain of gets */

						//get at (potential) root stack pos, so it can be returned
						int prev_pos = _parse_expression(codegen, chain.back()->get()->arguments[0], slevel);
						if (prev_pos < 0) {
							return prev_pos;
						}
						int retval = prev_pos;

						if (retval & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
							slevel++;
							codegen.alloc_stack(slevel);
						}

						Vector<int> setchain;

						if (assign_property != StringName()) {
							// recover and assign at the end, this allows stuff like
							// position.x+=2.0
							// in Node2D
							setchain.push_back(prev_pos);
							setchain.push_back(codegen.get_name_map_pos(assign_property));
							setchain.push_back(PDScriptFunction::OPCODE_SET_MEMBER);
						}

						for (List<PDScriptParser::OperatorNode *>::Element *E = chain.back(); E; E = E->prev()) {
							if (E == chain.front()) { //ignore first
								break;
							}

							bool named = E->get()->op == PDScriptParser::OperatorNode::OP_INDEX_NAMED;
							int key_idx;

							if (named) {
								key_idx = codegen.get_name_map_pos(static_cast<const PDScriptParser::IdentifierNode *>(E->get()->arguments[1])->name);
								//printf("named key %x\n",key_idx);

							} else {
								if (prev_pos & (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS)) {
									slevel++;
									codegen.alloc_stack(slevel);
								}

								PDScriptParser::Node *key = E->get()->arguments[1];
								key_idx = _parse_expression(codegen, key, slevel);
								//printf("expr key %x\n",key_idx);

								//stack was raised here if retval was stack but..
							}

							if (key_idx < 0) { //error
								return key_idx;
							}

							codegen.opcodes.push_back(named ? PDScriptFunction::OPCODE_GET_NAMED : PDScriptFunction::OPCODE_GET);
							codegen.opcodes.push_back(prev_pos);
							codegen.opcodes.push_back(key_idx);
							slevel++;
							codegen.alloc_stack(slevel);
							int dst_pos = (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) | slevel;

							codegen.opcodes.push_back(dst_pos);

							//add in reverse order, since it will be reverted

							setchain.push_back(dst_pos);
							setchain.push_back(key_idx);
							setchain.push_back(prev_pos);
							setchain.push_back(named ? PDScriptFunction::OPCODE_SET_NAMED : PDScriptFunction::OPCODE_SET);

							prev_pos = dst_pos;
						}

						setchain.invert();

						int set_index;
						bool named = false;

						if (op->op == PDScriptParser::OperatorNode::OP_INDEX_NAMED) {
							set_index = codegen.get_name_map_pos(static_cast<const PDScriptParser::IdentifierNode *>(op->arguments[1])->name);
							named = true;
						} else {
							set_index = _parse_expression(codegen, op->arguments[1], slevel + 1);
							named = false;
						}

						if (set_index < 0) { //error
							return set_index;
						}

						if (set_index & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
							slevel++;
							codegen.alloc_stack(slevel);
						}

						int set_value = _parse_assign_right_expression(codegen, on, slevel + 1, named ? 0 : set_index);
						if (set_value < 0) { //error
							return set_value;
						}

						codegen.opcodes.push_back(named ? PDScriptFunction::OPCODE_SET_NAMED : PDScriptFunction::OPCODE_SET);
						codegen.opcodes.push_back(prev_pos);
						codegen.opcodes.push_back(set_index);
						codegen.opcodes.push_back(set_value);

						for (int i = 0; i < setchain.size(); i++) {
							codegen.opcodes.push_back(setchain[i]);
						}

						return retval;

					} else if (on->arguments[0]->type == PDScriptParser::Node::TYPE_IDENTIFIER && _is_class_member_property(codegen, static_cast<PDScriptParser::IdentifierNode *>(on->arguments[0])->name)) {
						//assignment to member property

						int slevel = p_stack_level;

						int src_address = _parse_assign_right_expression(codegen, on, slevel);
						if (src_address < 0) {
							return -1;
						}

						StringName name = static_cast<PDScriptParser::IdentifierNode *>(on->arguments[0])->name;

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_SET_MEMBER);
						codegen.opcodes.push_back(codegen.get_name_map_pos(name));
						codegen.opcodes.push_back(src_address);

						return PDScriptFunction::ADDR_TYPE_NIL << PDScriptFunction::ADDR_BITS;
					} else {
						//REGULAR ASSIGNMENT MODE!!

						int slevel = p_stack_level;

						int dst_address_a = _parse_expression(codegen, on->arguments[0], slevel, false, on->op == PDScriptParser::OperatorNode::OP_INIT_ASSIGN);
						if (dst_address_a < 0) {
							return -1;
						}

						if (dst_address_a & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
							slevel++;
							codegen.alloc_stack(slevel);
						}

						int src_address_b = _parse_assign_right_expression(codegen, on, slevel);
						if (src_address_b < 0) {
							return -1;
						}

						PDScriptDataType assign_type = _pdtype_from_datatype(on->arguments[0]->get_datatype());

						if (assign_type.has_type && !on->datatype.has_type) {
							// Typed assignment
							switch (assign_type.kind) {
								case PDScriptDataType::BUILTIN: {
									codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_TYPED_BUILTIN); // perform operator
									codegen.opcodes.push_back(assign_type.builtin_type); // variable type
									codegen.opcodes.push_back(dst_address_a); // argument 1
									codegen.opcodes.push_back(src_address_b); // argument 2
								} break;
								case PDScriptDataType::NATIVE: {
									int class_idx;
									if (PDScriptLanguage::get_singleton()->get_global_map().has(assign_type.native_type)) {
										class_idx = PDScriptLanguage::get_singleton()->get_global_map()[assign_type.native_type];
										class_idx |= (PDScriptFunction::ADDR_TYPE_GLOBAL << PDScriptFunction::ADDR_BITS); //argument (stack root)
									} else {
										_set_error("Invalid native class type '" + String(assign_type.native_type) + "'.", on->arguments[0]);
										return -1;
									}
									codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_TYPED_NATIVE); // perform operator
									codegen.opcodes.push_back(class_idx); // variable type
									codegen.opcodes.push_back(dst_address_a); // argument 1
									codegen.opcodes.push_back(src_address_b); // argument 2
								} break;
								case PDScriptDataType::SCRIPT:
								case PDScriptDataType::PDSCRIPT: {
									Variant script = assign_type.script_type;
									int idx = codegen.get_constant_pos(script);
									idx |= PDScriptFunction::ADDR_TYPE_LOCAL_CONSTANT << PDScriptFunction::ADDR_BITS; //make it a local constant (faster access)

									codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN_TYPED_SCRIPT); // perform operator
									codegen.opcodes.push_back(idx); // variable type
									codegen.opcodes.push_back(dst_address_a); // argument 1
									codegen.opcodes.push_back(src_address_b); // argument 2
								} break;
								default: {
									ERR_PRINT("Compiler bug: unresolved assign.");

									// Shouldn't get here, but fail-safe to a regular assignment
									codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN); // perform operator
									codegen.opcodes.push_back(dst_address_a); // argument 1
									codegen.opcodes.push_back(src_address_b); // argument 2 (unary only takes one parameter)
								}
							}
						} else {
							// Either untyped assignment or already type-checked by the parser
							codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN); // perform operator
							codegen.opcodes.push_back(dst_address_a); // argument 1
							codegen.opcodes.push_back(src_address_b); // argument 2 (unary only takes one parameter)
						}
						return dst_address_a; //if anything, returns whatever was assigned or correct stack position
					}
				} break;
				case PDScriptParser::OperatorNode::OP_IS: {
					ERR_FAIL_COND_V(on->arguments.size() != 2, false);

					int slevel = p_stack_level;

					int src_address_a = _parse_expression(codegen, on->arguments[0], slevel);
					if (src_address_a < 0) {
						return -1;
					}

					if (src_address_a & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
						slevel++; //uses stack for return, increase stack
					}

					int src_address_b = _parse_expression(codegen, on->arguments[1], slevel);
					if (src_address_b < 0) {
						return -1;
					}

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_EXTENDS_TEST); // perform operator
					codegen.opcodes.push_back(src_address_a); // argument 1
					codegen.opcodes.push_back(src_address_b); // argument 2 (unary only takes one parameter)

				} break;
				case PDScriptParser::OperatorNode::OP_IS_BUILTIN: {
					ERR_FAIL_COND_V(on->arguments.size() != 2, false);
					ERR_FAIL_COND_V(on->arguments[1]->type != PDScriptParser::Node::TYPE_TYPE, false);

					int slevel = p_stack_level;

					int src_address_a = _parse_expression(codegen, on->arguments[0], slevel);
					if (src_address_a < 0) {
						return -1;
					}

					if (src_address_a & PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) {
						slevel++; //uses stack for return, increase stack
					}

					const PDScriptParser::TypeNode *tn = static_cast<const PDScriptParser::TypeNode *>(on->arguments[1]);

					codegen.opcodes.push_back(PDScriptFunction::OPCODE_IS_BUILTIN); // perform operator
					codegen.opcodes.push_back(src_address_a); // argument 1
					codegen.opcodes.push_back((int)tn->vtype); // argument 2 (unary only takes one parameter)
				} break;
				default: {
					ERR_FAIL_V_MSG(0, "Bug in bytecode compiler, unexpected operator #" + itos(on->op) + " in parse tree while parsing expression."); //unreachable code

				} break;
			}

			int dst_addr = (p_stack_level) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
			codegen.opcodes.push_back(dst_addr); // append the stack level as destination address of the opcode
			codegen.alloc_stack(p_stack_level);
			return dst_addr;
		} break;
		//TYPE_TYPE,
		default: {
			ERR_FAIL_V_MSG(-1, "Bug in bytecode compiler, unexpected node in parse tree while parsing expression."); //unreachable code
		} break;
	}
}

Error PDScriptCompiler::_parse_block(CodeGen &codegen, const PDScriptParser::BlockNode *p_block, int p_stack_level, int p_break_addr, int p_continue_addr) {
	codegen.push_stack_identifiers();
	codegen.current_line = p_block->line;

	for (int i = 0; i < p_block->statements.size(); i++) {
		const PDScriptParser::Node *s = p_block->statements[i];

		switch (s->type) {
			case PDScriptParser::Node::TYPE_NEWLINE: {
#ifdef DEBUG_ENABLED
				const PDScriptParser::NewLineNode *nl = static_cast<const PDScriptParser::NewLineNode *>(s);
				codegen.opcodes.push_back(PDScriptFunction::OPCODE_LINE);
				codegen.opcodes.push_back(nl->line);
				codegen.current_line = nl->line;
#endif
			} break;
			case PDScriptParser::Node::TYPE_CONTROL_FLOW: {
				// try subblocks

				const PDScriptParser::ControlFlowNode *cf = static_cast<const PDScriptParser::ControlFlowNode *>(s);

				switch (cf->cf_type) {
					case PDScriptParser::ControlFlowNode::CF_MATCH: {
						PDScriptParser::MatchNode *match = cf->match;

						PDScriptParser::IdentifierNode *id = memnew(PDScriptParser::IdentifierNode);
						id->name = "#match_value";

						// var #match_value
						// copied because there is no _parse_statement :(
						codegen.add_stack_identifier(id->name, p_stack_level++);
						codegen.alloc_stack(p_stack_level);

						PDScriptParser::OperatorNode *op = memnew(PDScriptParser::OperatorNode);
						op->op = PDScriptParser::OperatorNode::OP_ASSIGN;
						op->arguments.push_back(id);
						op->arguments.push_back(match->val_to_match);

						int ret = _parse_expression(codegen, op, p_stack_level);
						if (ret < 0) {
							memdelete(id);
							memdelete(op);
							return ERR_PARSE_ERROR;
						}

						// break address
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(codegen.opcodes.size() + 3);
						int break_addr = codegen.opcodes.size();
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(0); // break addr

						for (int j = 0; j < match->compiled_pattern_branches.size(); j++) {
							PDScriptParser::MatchNode::CompiledPatternBranch branch = match->compiled_pattern_branches[j];

							// jump over continue
							// jump unconditionally
							// continue address
							// compile the condition
							int ret2 = _parse_expression(codegen, branch.compiled_pattern, p_stack_level);
							if (ret2 < 0) {
								memdelete(id);
								memdelete(op);
								return ERR_PARSE_ERROR;
							}

							codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF);
							codegen.opcodes.push_back(ret2);
							codegen.opcodes.push_back(codegen.opcodes.size() + 3);
							int continue_addr = codegen.opcodes.size();
							codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
							codegen.opcodes.push_back(0);

							Error err = _parse_block(codegen, branch.body, p_stack_level, p_break_addr, continue_addr);
							if (err) {
								memdelete(id);
								memdelete(op);
								return ERR_PARSE_ERROR;
							}

							codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
							codegen.opcodes.push_back(break_addr);

							codegen.opcodes.write[continue_addr + 1] = codegen.opcodes.size();
						}

						codegen.opcodes.write[break_addr + 1] = codegen.opcodes.size();

						memdelete(id);
						memdelete(op);

					} break;

					case PDScriptParser::ControlFlowNode::CF_IF: {
						int ret2 = _parse_expression(codegen, cf->arguments[0], p_stack_level, false);
						if (ret2 < 0) {
							return ERR_PARSE_ERROR;
						}

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF_NOT);
						codegen.opcodes.push_back(ret2);
						int else_addr = codegen.opcodes.size();
						codegen.opcodes.push_back(0); //temporary

						Error err = _parse_block(codegen, cf->body, p_stack_level, p_break_addr, p_continue_addr);
						if (err) {
							return err;
						}

						if (cf->body_else) {
							codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
							int end_addr = codegen.opcodes.size();
							codegen.opcodes.push_back(0);
							codegen.opcodes.write[else_addr] = codegen.opcodes.size();

							codegen.opcodes.push_back(PDScriptFunction::OPCODE_LINE);
							codegen.opcodes.push_back(cf->body_else->line);
							codegen.current_line = cf->body_else->line;

							Error err2 = _parse_block(codegen, cf->body_else, p_stack_level, p_break_addr, p_continue_addr);
							if (err2) {
								return err2;
							}

							codegen.opcodes.write[end_addr] = codegen.opcodes.size();
						} else {
							//end without else
							codegen.opcodes.write[else_addr] = codegen.opcodes.size();
						}

					} break;
					case PDScriptParser::ControlFlowNode::CF_FOR: {
						int slevel = p_stack_level;
						int iter_stack_pos = slevel;
						int iterator_pos = (slevel++) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
						int counter_pos = (slevel++) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
						int container_pos = (slevel++) | (PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS);
						codegen.alloc_stack(slevel);

						codegen.push_stack_identifiers();
						codegen.add_stack_identifier(static_cast<const PDScriptParser::IdentifierNode *>(cf->arguments[0])->name, iter_stack_pos);

						int ret2 = _parse_expression(codegen, cf->arguments[1], slevel, false);
						if (ret2 < 0) {
							return ERR_COMPILATION_FAILED;
						}

						//assign container
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSIGN);
						codegen.opcodes.push_back(container_pos);
						codegen.opcodes.push_back(ret2);

						//begin loop
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_ITERATE_BEGIN);
						codegen.opcodes.push_back(counter_pos);
						codegen.opcodes.push_back(container_pos);
						codegen.opcodes.push_back(codegen.opcodes.size() + 4);
						codegen.opcodes.push_back(iterator_pos);
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP); //skip code for next
						codegen.opcodes.push_back(codegen.opcodes.size() + 8);
						//break loop
						int break_pos = codegen.opcodes.size();
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP); //skip code for next
						codegen.opcodes.push_back(0); //skip code for next
						//next loop
						int continue_pos = codegen.opcodes.size();
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_ITERATE);
						codegen.opcodes.push_back(counter_pos);
						codegen.opcodes.push_back(container_pos);
						codegen.opcodes.push_back(break_pos);
						codegen.opcodes.push_back(iterator_pos);

						Error err = _parse_block(codegen, cf->body, slevel, break_pos, continue_pos);
						if (err) {
							return err;
						}

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(continue_pos);
						codegen.opcodes.write[break_pos + 1] = codegen.opcodes.size();

						codegen.pop_stack_identifiers();

					} break;
					case PDScriptParser::ControlFlowNode::CF_WHILE: {
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(codegen.opcodes.size() + 3);
						int break_addr = codegen.opcodes.size();
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(0);
						int continue_addr = codegen.opcodes.size();

						int ret2 = _parse_expression(codegen, cf->arguments[0], p_stack_level, false);
						if (ret2 < 0) {
							return ERR_PARSE_ERROR;
						}
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_IF_NOT);
						codegen.opcodes.push_back(ret2);
						codegen.opcodes.push_back(break_addr);
						Error err = _parse_block(codegen, cf->body, p_stack_level, break_addr, continue_addr);
						if (err) {
							return err;
						}
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(continue_addr);

						codegen.opcodes.write[break_addr + 1] = codegen.opcodes.size();

					} break;
					case PDScriptParser::ControlFlowNode::CF_BREAK: {
						if (p_break_addr < 0) {
							_set_error("'break'' not within loop", cf);
							return ERR_COMPILATION_FAILED;
						}
						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(p_break_addr);

					} break;
					case PDScriptParser::ControlFlowNode::CF_CONTINUE: {
						if (p_continue_addr < 0) {
							_set_error("'continue' not within loop", cf);
							return ERR_COMPILATION_FAILED;
						}

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP);
						codegen.opcodes.push_back(p_continue_addr);

					} break;
					case PDScriptParser::ControlFlowNode::CF_RETURN: {
						int ret2;

						if (cf->arguments.size()) {
							ret2 = _parse_expression(codegen, cf->arguments[0], p_stack_level, false);
							if (ret2 < 0) {
								return ERR_PARSE_ERROR;
							}

						} else {
							ret2 = PDScriptFunction::ADDR_TYPE_NIL << PDScriptFunction::ADDR_BITS;
						}

						codegen.opcodes.push_back(PDScriptFunction::OPCODE_RETURN);
						codegen.opcodes.push_back(ret2);

					} break;
				}
			} break;
			case PDScriptParser::Node::TYPE_ASSERT: {
#ifdef DEBUG_ENABLED
				// try subblocks

				const PDScriptParser::AssertNode *as = static_cast<const PDScriptParser::AssertNode *>(s);

				int ret2 = _parse_expression(codegen, as->condition, p_stack_level, false);
				if (ret2 < 0) {
					return ERR_PARSE_ERROR;
				}

				int message_ret = 0;
				if (as->message) {
					message_ret = _parse_expression(codegen, as->message, p_stack_level + 1, false);
					if (message_ret < 0) {
						return ERR_PARSE_ERROR;
					}
				}

				codegen.opcodes.push_back(PDScriptFunction::OPCODE_ASSERT);
				codegen.opcodes.push_back(ret2);
				codegen.opcodes.push_back(message_ret);
#endif
			} break;
			case PDScriptParser::Node::TYPE_BREAKPOINT: {
#ifdef DEBUG_ENABLED
				// try subblocks
				codegen.opcodes.push_back(PDScriptFunction::OPCODE_BREAKPOINT);
#endif
			} break;
			case PDScriptParser::Node::TYPE_LOCAL_VAR: {
				const PDScriptParser::LocalVarNode *lv = static_cast<const PDScriptParser::LocalVarNode *>(s);

				// since we are using properties now for most class access, allow shadowing of class members to make user's life easier.
				//
				//if (_is_class_member_property(codegen, lv->name)) {
				//	_set_error("Name for local variable '" + String(lv->name) + "' can't shadow class property of the same name.", lv);
				//	return ERR_ALREADY_EXISTS;
				//}

				codegen.add_stack_identifier(lv->name, p_stack_level++);
				codegen.alloc_stack(p_stack_level);

			} break;
			default: {
				//expression
				int ret2 = _parse_expression(codegen, s, p_stack_level, true);
				if (ret2 < 0) {
					return ERR_PARSE_ERROR;
				}
			} break;
		}
	}
	codegen.pop_stack_identifiers();
	return OK;
}

Error PDScriptCompiler::_parse_function(PDScript *p_script, const PDScriptParser::ClassNode *p_class, const PDScriptParser::FunctionNode *p_func, bool p_for_ready) {
	Vector<int> bytecode;
	CodeGen codegen;

	codegen.class_node = p_class;
	codegen.script = p_script;
	codegen.function_node = p_func;
	codegen.stack_max = 0;
	codegen.current_line = 0;
	codegen.call_max = 0;
	codegen.debug_stack = ScriptDebugger::get_singleton() != nullptr;
#ifdef TOOLS_ENABLED
	Vector<StringName> argnames;
	Vector<Variant> default_arg_values;
#endif

	int stack_level = 0;

	if (p_func) {
		for (int i = 0; i < p_func->arguments.size(); i++) {
			// since we are using properties now for most class access, allow shadowing of class members to make user's life easier.
			//
			//if (_is_class_member_property(p_script, p_func->arguments[i])) {
			//	_set_error("Name for argument '" + String(p_func->arguments[i]) + "' can't shadow class property of the same name.", p_func);
			//	return ERR_ALREADY_EXISTS;
			//}

			codegen.add_stack_identifier(p_func->arguments[i], i);
#ifdef TOOLS_ENABLED
			argnames.push_back(p_func->arguments[i]);
#endif
		}
		stack_level = p_func->arguments.size();
	}

	codegen.alloc_stack(stack_level);

	/* Parse initializer -if applies- */

	bool is_initializer = !p_for_ready && !p_func;

	if (is_initializer || (p_func && String(p_func->name) == "_init")) {
		//parse initializer for class members
		if (!p_func && p_class->extends_used && p_script->native.is_null()) {
			//call implicit parent constructor
			codegen.opcodes.push_back(PDScriptFunction::OPCODE_CALL_SELF_BASE);
			codegen.opcodes.push_back(codegen.get_name_map_pos("_init"));
			codegen.opcodes.push_back(0);
			codegen.opcodes.push_back((PDScriptFunction::ADDR_TYPE_STACK << PDScriptFunction::ADDR_BITS) | 0);
		}
		Error err = _parse_block(codegen, p_class->initializer, stack_level);
		if (err) {
			return err;
		}
		is_initializer = true;
	}

	if (p_for_ready || (p_func && String(p_func->name) == "_ready")) {
		//parse initializer for class members
		if (p_class->ready->statements.size()) {
			Error err = _parse_block(codegen, p_class->ready, stack_level);
			if (err) {
				return err;
			}
		}
	}

	/* Parse default argument code -if applies- */

	Vector<int> defarg_addr;
	StringName func_name;

	if (p_func) {
		if (p_func->default_values.size()) {
			codegen.opcodes.push_back(PDScriptFunction::OPCODE_JUMP_TO_DEF_ARGUMENT);
			defarg_addr.push_back(codegen.opcodes.size());
			for (int i = 0; i < p_func->default_values.size(); i++) {
				_parse_expression(codegen, p_func->default_values[i], stack_level, true);
				defarg_addr.push_back(codegen.opcodes.size());
#ifdef TOOLS_ENABLED
				const PDScriptParser::OperatorNode *assign = static_cast<const PDScriptParser::OperatorNode *>(p_func->default_values[i]);
				if (assign->arguments.size() >= 2 && assign->arguments[1]->type == PDScriptParser::Node::TYPE_CONSTANT) {
					const PDScriptParser::ConstantNode *cn = static_cast<const PDScriptParser::ConstantNode *>(assign->arguments[1]);
					default_arg_values.push_back(cn->value);
				} else {
					default_arg_values.push_back(Variant());
				}
#endif
			}

			defarg_addr.invert();
		}

		Error err = _parse_block(codegen, p_func->body, stack_level);
		if (err) {
			return err;
		}

		func_name = p_func->name;
	} else {
		if (p_for_ready) {
			func_name = "_ready";
		} else {
			func_name = "_init";
		}
	}

	codegen.opcodes.push_back(PDScriptFunction::OPCODE_END);

	/*
	if (String(p_func->name)=="") { //initializer func
		pdfunc = &p_script->initializer;
	*/
	//} else { //regular func
	p_script->member_functions[func_name] = memnew(PDScriptFunction);
	PDScriptFunction *pdfunc = p_script->member_functions[func_name];
	//}

	if (p_func) {
		pdfunc->_static = p_func->_static;
		pdfunc->argument_types.resize(p_func->argument_types.size());
		for (int i = 0; i < p_func->argument_types.size(); i++) {
			pdfunc->argument_types.write[i] = _pdtype_from_datatype(p_func->argument_types[i], p_script);
		}
		pdfunc->return_type = _pdtype_from_datatype(p_func->return_type, p_script);
	} else {
		pdfunc->_static = false;
		pdfunc->return_type = PDScriptDataType();
		pdfunc->return_type.has_type = true;
		pdfunc->return_type.kind = PDScriptDataType::BUILTIN;
		pdfunc->return_type.builtin_type = Variant::NIL;
	}

#ifdef TOOLS_ENABLED
	pdfunc->arg_names = argnames;
	pdfunc->default_arg_values = default_arg_values;
#endif
	//constants
	if (codegen.constant_map.size()) {
		pdfunc->_constant_count = codegen.constant_map.size();
		pdfunc->constants.resize(codegen.constant_map.size());
		pdfunc->_constants_ptr = pdfunc->constants.ptrw();
		const Variant *K = nullptr;
		while ((K = codegen.constant_map.next(K))) {
			int idx = codegen.constant_map[*K];
			pdfunc->constants.write[idx] = *K;
		}
	} else {
		pdfunc->_constants_ptr = nullptr;
		pdfunc->_constant_count = 0;
	}
	//global names
	if (codegen.name_map.size()) {
		pdfunc->global_names.resize(codegen.name_map.size());
		pdfunc->_global_names_ptr = &pdfunc->global_names[0];
		for (RBMap<StringName, int>::Element *E = codegen.name_map.front(); E; E = E->next()) {
			pdfunc->global_names.write[E->get()] = E->key();
		}
		pdfunc->_global_names_count = pdfunc->global_names.size();

	} else {
		pdfunc->_global_names_ptr = nullptr;
		pdfunc->_global_names_count = 0;
	}

#ifdef TOOLS_ENABLED
	// Named globals
	if (codegen.named_globals.size()) {
		pdfunc->named_globals.resize(codegen.named_globals.size());
		pdfunc->_named_globals_ptr = pdfunc->named_globals.ptr();
		for (int i = 0; i < codegen.named_globals.size(); i++) {
			pdfunc->named_globals.write[i] = codegen.named_globals[i];
		}
		pdfunc->_named_globals_count = pdfunc->named_globals.size();
	}
#endif

	if (codegen.opcodes.size()) {
		pdfunc->code = codegen.opcodes;
		pdfunc->_code_ptr = &pdfunc->code[0];
		pdfunc->_code_size = codegen.opcodes.size();

	} else {
		pdfunc->_code_ptr = nullptr;
		pdfunc->_code_size = 0;
	}

	if (defarg_addr.size()) {
		pdfunc->default_arguments = defarg_addr;
		pdfunc->_default_arg_count = defarg_addr.size() - 1;
		pdfunc->_default_arg_ptr = &pdfunc->default_arguments[0];
	} else {
		pdfunc->_default_arg_count = 0;
		pdfunc->_default_arg_ptr = nullptr;
	}

	pdfunc->_argument_count = p_func ? p_func->arguments.size() : 0;
	pdfunc->_stack_size = codegen.stack_max;
	pdfunc->_call_size = codegen.call_max;
	pdfunc->name = func_name;
#ifdef DEBUG_ENABLED
	if (ScriptDebugger::get_singleton()) {
		String signature;
		//path
		if (p_script->get_path() != String()) {
			signature += p_script->get_path();
		}
		//loc
		if (p_func) {
			signature += "::" + itos(p_func->body->line);
		} else {
			signature += "::0";
		}

		//function and class

		if (p_class->name) {
			signature += "::" + String(p_class->name) + "." + String(func_name);
		} else {
			signature += "::" + String(func_name);
		}

		pdfunc->profile.signature = signature;
	}
#endif
	pdfunc->_script = p_script;
	pdfunc->source = source;

#ifdef DEBUG_ENABLED

	{
		pdfunc->func_cname = (String(source) + " - " + String(func_name)).utf8();
		pdfunc->_func_cname = pdfunc->func_cname.get_data();
	}

#endif
	if (p_func) {
		pdfunc->_initial_line = p_func->line;
#ifdef TOOLS_ENABLED

		p_script->member_lines[func_name] = p_func->line;
#endif
	} else {
		pdfunc->_initial_line = 0;
	}

	if (codegen.debug_stack) {
		pdfunc->stack_debug = codegen.stack_debug;
	}

	if (is_initializer) {
		p_script->initializer = pdfunc;
	}

	return OK;
}

Error PDScriptCompiler::_parse_class_level(PDScript *p_script, const PDScriptParser::ClassNode *p_class, bool p_keep_state) {
	parsing_classes.insert(p_script);

	if (p_class->owner && p_class->owner->owner) {
		// Owner is not root
		if (!parsed_classes.has(p_script->_owner)) {
			if (parsing_classes.has(p_script->_owner)) {
				_set_error("Cyclic class reference for '" + String(p_class->name) + "'.", p_class);
				return ERR_PARSE_ERROR;
			}
			Error err = _parse_class_level(p_script->_owner, p_class->owner, p_keep_state);
			if (err) {
				return err;
			}
		}
	}

	p_script->native = Ref<PDScriptNativeClass>();
	p_script->base = Ref<PDScript>();
	p_script->_base = nullptr;
	p_script->members.clear();
	p_script->constants.clear();
	for (RBMap<StringName, PDScriptFunction *>::Element *E = p_script->member_functions.front(); E; E = E->next()) {
		memdelete(E->get());
	}
	p_script->member_functions.clear();
	p_script->member_indices.clear();
	p_script->member_info.clear();
	p_script->_signals.clear();
	p_script->initializer = nullptr;

	p_script->tool = p_class->tool;
	p_script->name = p_class->name;

	Ref<PDScriptNativeClass> native;

	PDScriptDataType base_type = _pdtype_from_datatype(p_class->base_type);
	// Inheritance
	switch (base_type.kind) {
		case PDScriptDataType::NATIVE: {
			int native_idx = PDScriptLanguage::get_singleton()->get_global_map()[base_type.native_type];
			native = PDScriptLanguage::get_singleton()->get_global_array()[native_idx];
			ERR_FAIL_COND_V(native.is_null(), ERR_BUG);
			p_script->native = native;
		} break;
		case PDScriptDataType::PDSCRIPT: {
			Ref<PDScript> base = Ref<Script>(base_type.script_type);
			p_script->base = base;
			p_script->_base = base.ptr();
			p_script->member_indices = base->member_indices;

			if (p_class->base_type.kind == PDScriptParser::DataType::CLASS) {
				if (!parsed_classes.has(p_script->_base)) {
					if (parsing_classes.has(p_script->_base)) {
						_set_error("Cyclic class reference for '" + String(p_class->name) + "'.", p_class);
						return ERR_PARSE_ERROR;
					}
					Error err = _parse_class_level(p_script->_base, p_class->base_type.class_type, p_keep_state);
					if (err) {
						return err;
					}
				}
			}
		} break;
		default: {
			_set_error("Parser bug: invalid inheritance.", p_class);
			return ERR_BUG;
		} break;
	}

	for (int i = 0; i < p_class->variables.size(); i++) {
		StringName name = p_class->variables[i].identifier;

		PDScript::MemberInfo minfo;
		minfo.index = p_script->member_indices.size();
		minfo.setter = p_class->variables[i].setter;
		minfo.getter = p_class->variables[i].getter;
		minfo.data_type = _pdtype_from_datatype(p_class->variables[i].data_type, p_script);

		PropertyInfo prop_info = minfo.data_type;
		prop_info.name = name;
		PropertyInfo export_info = p_class->variables[i]._export;

		if (export_info.type != Variant::NIL) {
			if (!minfo.data_type.has_type) {
				prop_info.type = export_info.type;
				prop_info.class_name = export_info.class_name;
			}
			prop_info.hint = export_info.hint;
			prop_info.hint_string = export_info.hint_string;
			prop_info.usage = export_info.usage;
#ifdef TOOLS_ENABLED
			if (p_class->variables[i].default_value.get_type() != Variant::NIL) {
				p_script->member_default_values[name] = p_class->variables[i].default_value;
			}
#endif
		} else {
			prop_info.usage = PROPERTY_USAGE_SCRIPT_VARIABLE;
		}

		p_script->member_info[name] = prop_info;
		p_script->member_indices[name] = minfo;
		p_script->members.insert(name);

#ifdef TOOLS_ENABLED
		p_script->member_lines[name] = p_class->variables[i].line;
#endif
	}

	for (RBMap<StringName, PDScriptParser::ClassNode::Constant>::Element *E = p_class->constant_expressions.front(); E; E = E->next()) {
		StringName name = E->key();

		ERR_CONTINUE(E->get().expression->type != PDScriptParser::Node::TYPE_CONSTANT);

		PDScriptParser::ConstantNode *constant = static_cast<PDScriptParser::ConstantNode *>(E->get().expression);

		p_script->constants.insert(name, constant->value);
#ifdef TOOLS_ENABLED

		p_script->member_lines[name] = E->get().expression->line;
#endif
	}

	for (int i = 0; i < p_class->_signals.size(); i++) {
		StringName name = p_class->_signals[i].name;

		PDScript *c = p_script;

		while (c) {
			if (c->_signals.has(name)) {
				_set_error("Signal '" + name + "' redefined (in current or parent class)", p_class);
				return ERR_ALREADY_EXISTS;
			}

			if (c->base.is_valid()) {
				c = c->base.ptr();
			} else {
				c = nullptr;
			}
		}

		if (native.is_valid()) {
			if (ClassDB::has_signal(native->get_name(), name)) {
				_set_error("Signal '" + name + "' redefined (original in native class '" + String(native->get_name()) + "')", p_class);
				return ERR_ALREADY_EXISTS;
			}
		}

		p_script->_signals[name] = p_class->_signals[i].arguments;
	}

	parsed_classes.insert(p_script);
	parsing_classes.erase(p_script);

	//parse sub-classes

	for (int i = 0; i < p_class->subclasses.size(); i++) {
		StringName name = p_class->subclasses[i]->name;
		Ref<PDScript> &subclass = p_script->subclasses[name];
		PDScript *subclass_ptr = subclass.ptr();

		// Subclass might still be parsing, just skip it
		if (!parsed_classes.has(subclass_ptr) && !parsing_classes.has(subclass_ptr)) {
			Error err = _parse_class_level(subclass_ptr, p_class->subclasses[i], p_keep_state);
			if (err) {
				return err;
			}
		}

#ifdef TOOLS_ENABLED

		p_script->member_lines[name] = p_class->subclasses[i]->line;
#endif

		p_script->constants.insert(name, subclass); //once parsed, goes to the list of constants
	}

	return OK;
}

Error PDScriptCompiler::_parse_class_blocks(PDScript *p_script, const PDScriptParser::ClassNode *p_class, bool p_keep_state) {
	//parse methods

	bool has_initializer = false;
	bool has_ready = false;

	for (int i = 0; i < p_class->functions.size(); i++) {
		if (!has_initializer && p_class->functions[i]->name == "_init") {
			has_initializer = true;
		}
		if (!has_ready && p_class->functions[i]->name == "_ready") {
			has_ready = true;
		}
		Error err = _parse_function(p_script, p_class, p_class->functions[i]);
		if (err) {
			return err;
		}
	}

	//parse static methods

	for (int i = 0; i < p_class->static_functions.size(); i++) {
		Error err = _parse_function(p_script, p_class, p_class->static_functions[i]);
		if (err) {
			return err;
		}
	}

	if (!has_initializer) {
		//create a constructor
		Error err = _parse_function(p_script, p_class, nullptr);
		if (err) {
			return err;
		}
	}

	if (!has_ready && p_class->ready->statements.size()) {
		//create a constructor
		Error err = _parse_function(p_script, p_class, nullptr, true);
		if (err) {
			return err;
		}
	}

#ifdef DEBUG_ENABLED

	//validate instances if keeping state

	if (p_keep_state) {
		for (RBSet<Object *>::Element *E = p_script->instances.front(); E;) {
			RBSet<Object *>::Element *N = E->next();

			ScriptInstance *si = E->get()->get_script_instance();
			if (si->is_placeholder()) {
#ifdef TOOLS_ENABLED
				PlaceHolderScriptInstance *psi = static_cast<PlaceHolderScriptInstance *>(si);

				if (p_script->is_tool()) {
					//re-create as an instance
					p_script->placeholders.erase(psi); //remove placeholder

					PDScriptInstance *instance = memnew(PDScriptInstance);
					instance->base_ref = Object::cast_to<Reference>(E->get());
					instance->members.resize(p_script->member_indices.size());
					instance->script = Ref<PDScript>(p_script);
					instance->owner = E->get();

					//needed for hot reloading
					for (RBMap<StringName, PDScript::MemberInfo>::Element *F = p_script->member_indices.front(); F; F = F->next()) {
						instance->member_indices_cache[F->key()] = F->get().index;
					}
					instance->owner->set_script_instance(instance);

					/* STEP 2, INITIALIZE AND CONSTRUCT */

					Variant::CallError ce;
					p_script->initializer->call(instance, nullptr, 0, ce);

					if (ce.error != Variant::CallError::CALL_OK) {
						//well, tough luck, not goinna do anything here
					}
				}
#endif
			} else {
				PDScriptInstance *gi = static_cast<PDScriptInstance *>(si);
				gi->reload_members();
			}

			E = N;
		}
	}
#endif

	for (int i = 0; i < p_class->subclasses.size(); i++) {
		StringName name = p_class->subclasses[i]->name;
		PDScript *subclass = p_script->subclasses[name].ptr();

		Error err = _parse_class_blocks(subclass, p_class->subclasses[i], p_keep_state);
		if (err) {
			return err;
		}
	}

	p_script->valid = true;
	return OK;
}

void PDScriptCompiler::_make_scripts(PDScript *p_script, const PDScriptParser::ClassNode *p_class, bool p_keep_state) {
	RBMap<StringName, Ref<PDScript>> old_subclasses;

	if (p_keep_state) {
		old_subclasses = p_script->subclasses;
	}

	p_script->subclasses.clear();

	for (int i = 0; i < p_class->subclasses.size(); i++) {
		StringName name = p_class->subclasses[i]->name;

		Ref<PDScript> subclass;
		String fully_qualified_name = p_script->fully_qualified_name + "::" + name;

		if (old_subclasses.has(name)) {
			subclass = old_subclasses[name];
		} else {
			Ref<PDScript> orphan_subclass = PDScriptLanguage::get_singleton()->get_orphan_subclass(fully_qualified_name);
			if (orphan_subclass.is_valid()) {
				subclass = orphan_subclass;
			} else {
				subclass.instance();
			}
		}

		subclass->_owner = p_script;
		subclass->fully_qualified_name = fully_qualified_name;
		p_script->subclasses.insert(name, subclass);

		_make_scripts(subclass.ptr(), p_class->subclasses[i], false);
	}
}

Error PDScriptCompiler::compile(const PDScriptParser *p_parser, PDScript *p_script, bool p_keep_state) {
	err_line = -1;
	err_column = -1;
	error = "";
	parser = p_parser;
	main_script = p_script;
	const PDScriptParser::Node *root = parser->get_parse_tree();
	ERR_FAIL_COND_V(root->type != PDScriptParser::Node::TYPE_CLASS, ERR_INVALID_DATA);

	source = p_script->get_path();

	// The best fully qualified name for a base level script is its file path
	p_script->fully_qualified_name = p_script->path;

	// Create scripts for subclasses beforehand so they can be referenced
	_make_scripts(p_script, static_cast<const PDScriptParser::ClassNode *>(root), p_keep_state);

	p_script->_owner = nullptr;
	Error err = _parse_class_level(p_script, static_cast<const PDScriptParser::ClassNode *>(root), p_keep_state);

	if (err) {
		return err;
	}

	err = _parse_class_blocks(p_script, static_cast<const PDScriptParser::ClassNode *>(root), p_keep_state);

	if (err) {
		return err;
	}

	return OK;
}

String PDScriptCompiler::get_error() const {
	return error;
}
int PDScriptCompiler::get_error_line() const {
	return err_line;
}
int PDScriptCompiler::get_error_column() const {
	return err_column;
}

PDScriptCompiler::PDScriptCompiler() {
}
