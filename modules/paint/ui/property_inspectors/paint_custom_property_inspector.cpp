

#include "paint_custom_property_inspector.h"

#include "../../nodes/paint_node.h"

void PaintCustomPropertyInspector::set_paint_node(PaintNode *paint_node) {
	call("_set_paint_node", paint_node);
}
void PaintCustomPropertyInspector::set_paint_node_bind(Node *paint_node) {
	call("_set_paint_node", paint_node);
}
void PaintCustomPropertyInspector::_set_paint_node(Node *paint_node) {
}

PaintCustomPropertyInspector::PaintCustomPropertyInspector() {
}

PaintCustomPropertyInspector::~PaintCustomPropertyInspector() {
}

void PaintCustomPropertyInspector::_bind_methods() {
	BIND_VMETHOD(MethodInfo("_set_paint_node", PropertyInfo(Variant::OBJECT, "paint_node", PROPERTY_HINT_RESOURCE_TYPE, "PaintNode")));

	ClassDB::bind_method(D_METHOD("set_paint_node", "paint_node"), &PaintCustomPropertyInspector::set_paint_node_bind);
	ClassDB::bind_method(D_METHOD("_set_paint_node", "paint_node"), &PaintCustomPropertyInspector::_set_paint_node);
}
