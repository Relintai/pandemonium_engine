
#include "mm_math.h"

#include "../../algos/mm_algos.h"
#include "../../editor/mm_graph_node.h"
#include "../mm_material.h"

Ref<MMNodeUniversalProperty> MMMath::get_image() {
	return image;
}

void MMMath::set_image(const Ref<MMNodeUniversalProperty> &val) {
	image = val;
}

Ref<MMNodeUniversalProperty> MMMath::get_a() {
	return a;
}

void MMMath::set_a(const Ref<MMNodeUniversalProperty> &val) {
	a = val;
}

Ref<MMNodeUniversalProperty> MMMath::get_b() {
	return b;
}

void MMMath::set_b(const Ref<MMNodeUniversalProperty> &val) {
	b = val;
}

Ref<MMNodeUniversalProperty> MMMath::get_output() {
	return output;
}

void MMMath::set_output(const Ref<MMNodeUniversalProperty> &val) {
	output = val;
}

int MMMath::get_operation() const {
	return operation;
}

void MMMath::set_operation(const int val) {
	operation = val;
	set_dirty(true);
	output->do_emit_changed();
}

bool MMMath::get_clamp_result() const {
	return clamp_result;
}

void MMMath::set_clamp_result(const bool val) {
	clamp_result = val;
	set_dirty(true);
	output->do_emit_changed();
}

void MMMath::_init_properties() {
	if (!a.is_valid()) {
		a.instance();
		a->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		a->set_default_value(0);
	}

	a->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	a->set_slot_name(">>>    A    ");
	a->set_value_step(0.01);
	a->set_value_range(Vector2(0, 1));

	if (!b.is_valid()) {
		b.instance();
		b->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		b->set_default_value(0);
	}

	b->set_input_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	b->set_slot_name(">>>    B    ");
	b->set_value_step(0.01);
	b->set_value_range(Vector2(0, 1));

	if (!image.is_valid()) {
		image.instance();
		image->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_IMAGE);
	}

	//image.input_slot_type = MMNodeUniversalProperty.SLOT_TYPE_FLOAT;
	image->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_IMAGE);
	//image.force_override = true;

	if (!output.is_valid()) {
		output.instance();
		output->set_default_type(MMNodeUniversalProperty::DEFAULT_TYPE_FLOAT);
		output->set_default_value(0);
	}

	output->set_output_slot_type(MMNodeUniversalProperty::SLOT_TYPE_UNIVERSAL);
	output->set_slot_name("     Output    >>>");
	output->set_get_value_from_owner(true);

	register_input_property(a);
	register_input_property(b);
	register_output_property(output);
	register_output_property(image);
}

void MMMath::_register_methods(MMGraphNode *mm_graph_node) {
	mm_graph_node->add_slot_label_universal(a);
	mm_graph_node->add_slot_label_universal(b);
	mm_graph_node->add_slot_label_universal(output);
	mm_graph_node->add_slot_texture_universal(image);

	Array arr;
	arr.push_back("A+B");
	arr.push_back("A-B");
	arr.push_back("A*B");
	arr.push_back("A/B");
	arr.push_back("log(A)");
	arr.push_back("log2(A)");
	arr.push_back("pow(A, B)");
	arr.push_back("abs(A)");
	arr.push_back("round(A)");
	arr.push_back("floor(A)");
	arr.push_back("ceil(A)");
	arr.push_back("trunc(A)");
	arr.push_back("fract(A)");
	arr.push_back("min(A, B)");
	arr.push_back("max(A, B)");
	arr.push_back("A<B");
	arr.push_back("cos(A*B)");
	arr.push_back("sin(A*B)");
	arr.push_back("tan(A*B)");
	arr.push_back("sqrt(1-A*A)");

	mm_graph_node->add_slot_enum("get_operation", "set_operation", "Operation", arr);
	mm_graph_node->add_slot_bool("get_clamp_result", "set_clamp_result", "Clamp result");
}

Variant MMMath::_get_property_value(const Vector2 &uv) {
	float af = a->get_value(uv);
	float bf = b->get_value(uv);
	float f = 0;
	//"A+B",;

	if (operation == 0) {
		f = af + bf;
	}
	//"A-B",;
	else if (operation == 1) {
		f = af - bf;
	}
	//"A*B",;
	else if (operation == 2) {
		f = af * bf;
	}
	//"A/B",;
	else if (operation == 3) {
		if (bf == 0) {
			bf = 0.000001;
		}

		f = af / bf;
	}
	//"log(A)",;
	else if (operation == 4) {
		//todo needs to be implemented;
		f = log(af);
	}
	//"log2(A)",;
	else if (operation == 5) {
		//todo needs to be implemented;
		f = log(af);
	}
	//"pow(A, B)",;
	else if (operation == 6) {
		f = pow(af, bf);
	}
	//"abs(A)",;
	else if (operation == 7) {
		f = abs(af);
	}
	//"round(A)",;
	else if (operation == 8) {
		f = round(af);
	}
	//"floor(A)",;
	else if (operation == 9) {
		f = floor(af);
	}
	//"ceil(A)",;
	else if (operation == 10) {
		f = ceil(af);
	}
	//"trunc(A)",;
	else if (operation == 11) {
		f = int(af);
	}
	//"fract(A)",;
	else if (operation == 12) {
		f = MMAlgos::fractf(af);
	}
	//"min(A, B)",;
	else if (operation == 13) {
		f = MIN(af, bf);
	}
	//"max(A, B)",;
	else if (operation == 14) {
		f = MAX(af, bf);
	}
	//"A<B",;
	else if (operation == 15) {
		f = af < bf;
	}
	//"cos(A*B)",;
	else if (operation == 16) {
		f = cos(af * bf);
	}
	//"sin(A*B)",;
	else if (operation == 17) {
		f = sin(af * bf);
	}
	//"tan(A*B)",;
	else if (operation == 18) {
		f = tan(af * bf);
	}
	//"sqrt(1-A*A)";
	else if (operation == 19) {
		f = sqrt(1 - af * af);
	}

	if (clamp_result) {
		f = CLAMP(f, 0, 1);
	}

	return f;
}

void MMMath::_render(const Ref<MMMaterial> &material) {
	Ref<Image> img = render_image(material);
	image->set_value(img);
}

Color MMMath::_get_value_for(const Vector2 &uv, const int pseed) {
	float f = get_property_value(uv);
	return Color(f, f, f, 1);
}

MMMath::MMMath() {
	operation = 0;
	clamp_result = false;
}

MMMath::~MMMath() {
}

void MMMath::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_image"), &MMMath::get_image);
	ClassDB::bind_method(D_METHOD("set_image", "value"), &MMMath::set_image);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "image", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_image", "get_image");

	ClassDB::bind_method(D_METHOD("get_a"), &MMMath::get_a);
	ClassDB::bind_method(D_METHOD("set_a", "value"), &MMMath::set_a);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "a", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_a", "get_a");

	ClassDB::bind_method(D_METHOD("get_b"), &MMMath::get_b);
	ClassDB::bind_method(D_METHOD("set_b", "value"), &MMMath::set_b);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "b", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_b", "get_b");

	ClassDB::bind_method(D_METHOD("get_output"), &MMMath::get_output);
	ClassDB::bind_method(D_METHOD("set_output", "value"), &MMMath::set_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_RESOURCE_TYPE, "MMNodeUniversalProperty"), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("get_operation"), &MMMath::get_operation);
	ClassDB::bind_method(D_METHOD("set_operation", "value"), &MMMath::set_operation);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "operation"), "set_operation", "get_operation");

	ClassDB::bind_method(D_METHOD("get_clamp_result"), &MMMath::get_clamp_result);
	ClassDB::bind_method(D_METHOD("set_clamp_result", "value"), &MMMath::set_clamp_result);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "clamp_result"), "set_clamp_result", "get_clamp_result");
}
