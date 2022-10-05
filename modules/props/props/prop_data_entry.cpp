/*
Copyright (c) 2019-2022 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "prop_data_entry.h"

#include "prop_data.h"

#include "scene/3d/spatial.h"

#include "modules/modules_enabled.gen.h"

#ifdef MODULE_TEXTURE_PACKER_ENABLED
#include "../../texture_packer/texture_packer.h"
#endif

#include "../prop_mesher.h"

Transform PropDataEntry::get_transform() const {
	return _transform;
}
void PropDataEntry::set_transform(const Transform &value) {
	_transform = value;
}

#ifdef MODULE_TEXTURE_PACKER_ENABLED
void PropDataEntry::add_textures_into(Ref<TexturePacker> texture_packer) {
	if (has_method("_add_textures_into"))
		call("_add_textures_into", texture_packer);
}
#endif

bool PropDataEntry::processor_handles(Node *node) {
	return call("_processor_handles", node);
}
void PropDataEntry::processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
	call("_processor_process", prop_data, node, transform);
}
Node *PropDataEntry::processor_get_node_for(const Transform &transform) {
	return call("_processor_get_node_for", transform);
}
bool PropDataEntry::processor_evaluate_children() {
	return call("_processor_evaluate_children");
}

bool PropDataEntry::_processor_handles(Node *node) {
	return false;
}
void PropDataEntry::_processor_process(Ref<PropData> prop_data, Node *node, const Transform &transform) {
}
Node *PropDataEntry::_processor_get_node_for(const Transform &transform) {
	return NULL;
}
bool PropDataEntry::_processor_evaluate_children() {
	return true;
}

PropDataEntry::PropDataEntry() {
}
PropDataEntry::~PropDataEntry() {
}

void PropDataEntry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_transform"), &PropDataEntry::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &PropDataEntry::set_transform);
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM, "transform"), "set_transform", "get_transform");

#ifdef MODULE_TEXTURE_PACKER_ENABLED
	BIND_VMETHOD(MethodInfo("_add_textures_into", PropertyInfo(Variant::OBJECT, "texture_packer", PROPERTY_HINT_RESOURCE_TYPE, "TexturePacker")));

	ClassDB::bind_method(D_METHOD("add_textures_into", "texture_packer"), &PropDataEntry::add_textures_into);
#endif

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "handles"), "_processor_handles"));
	BIND_VMETHOD(MethodInfo("_processor_process",
			PropertyInfo(Variant::OBJECT, "prop_data", PROPERTY_HINT_RESOURCE_TYPE, "PropData"),
			PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"),
			PropertyInfo(Variant::TRANSFORM, "transform")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::OBJECT, "node", PROPERTY_HINT_RESOURCE_TYPE, "Node"), "_processor_get_node_for",
			PropertyInfo(Variant::TRANSFORM, "transform")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "evaluate"), "_processor_evaluate_children"));

	ClassDB::bind_method(D_METHOD("processor_handles", "node"), &PropDataEntry::processor_handles);
	ClassDB::bind_method(D_METHOD("processor_process", "prop_data", "node", "transform"), &PropDataEntry::processor_process);
	ClassDB::bind_method(D_METHOD("processor_get_node_for", "prop_data"), &PropDataEntry::processor_get_node_for);
	ClassDB::bind_method(D_METHOD("processor_evaluate_children"), &PropDataEntry::processor_evaluate_children);

	ClassDB::bind_method(D_METHOD("_processor_handles", "node"), &PropDataEntry::_processor_handles);
	ClassDB::bind_method(D_METHOD("_processor_process", "prop_data", "node", "transform"), &PropDataEntry::_processor_process);
	ClassDB::bind_method(D_METHOD("_processor_get_node_for", "transform"), &PropDataEntry::_processor_get_node_for);
	ClassDB::bind_method(D_METHOD("_processor_evaluate_children"), &PropDataEntry::_processor_evaluate_children);
}
