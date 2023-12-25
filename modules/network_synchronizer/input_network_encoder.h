#ifndef INPUT_NETWORK_ENCODER_H
#define INPUT_NETWORK_ENCODER_H

/*************************************************************************/
/*  input_network_encoder.h                                              */
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

#include "core/containers/local_vector.h"
#include "core/object/resource.h"
#include "core/variant/variant.h"
#include "data_buffer.h"

struct NetworkedInputInfo {
	StringName name;
	Variant default_value;
	uint32_t index;
	DataBuffer::DataType data_type;
	DataBuffer::CompressionLevel compression_level;
	real_t comparison_floating_point_precision;
};

class InputNetworkEncoder : public Resource {
	GDCLASS(InputNetworkEncoder, Resource)

public:
	constexpr static uint32_t INDEX_NONE = UINT32_MAX;

private:
	LocalVector<NetworkedInputInfo> input_info;

protected:
	static void _bind_methods();

public:
	uint32_t register_input(
			const StringName &p_name,
			const Variant &p_default_value,
			DataBuffer::DataType p_type,
			DataBuffer::CompressionLevel p_compression_level,
			real_t p_comparison_floating_point_precision = CMP_EPSILON);

	uint32_t find_input_id(const StringName &p_name) const;
	const LocalVector<NetworkedInputInfo> &get_input_info() const;

	void encode(const LocalVector<Variant> &p_inputs, DataBuffer &r_buffer) const;
	void decode(DataBuffer &p_buffer, LocalVector<Variant> &r_inputs) const;
	void reset_inputs_to_defaults(LocalVector<Variant> &r_inputs) const;
	bool are_different(DataBuffer &p_buffer_A, DataBuffer &p_buffer_B) const;
	uint32_t count_size(DataBuffer &p_buffer) const;

	void script_encode(const Array &p_inputs, Object *r_buffer) const;
	Array script_decode(Object *p_buffer) const;
	Array script_get_defaults() const;
	bool script_are_different(Object *p_buffer_A, Object *p_buffer_B) const;
	uint32_t script_count_size(Object *p_buffer) const;
};

#endif
