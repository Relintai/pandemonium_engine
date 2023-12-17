

#include "paint_inspector_plugin.h"

#include "../ui/property_inspectors/paint_project_property_inspector.h"
#include "../ui/property_inspectors/paint_project_tools_property_inspector.h"
#include "../ui/property_inspectors/paint_tools_property_inspector.h"

#include "../nodes/paint_node.h"

bool PaintInspectorPlugin::can_handle(Object *p_object) {
	return p_object->is_class("PaintNode");
}
void PaintInspectorPlugin::parse_begin(Object *p_object) {
	PaintNode *paint_node = Object::cast_to<PaintNode>(p_object);

	PaintProjectPropertyInspector *pc = memnew(PaintProjectPropertyInspector);
	pc->set_paint_node(paint_node);
	add_custom_control(pc);

	if (p_object->is_class("PaintCanvas")) {
		PaintToolsPropertyInspector *ptool = memnew(PaintToolsPropertyInspector);
		ptool->set_paint_node(paint_node);
		add_custom_control(ptool);
	}

	if (p_object->is_class("PaintProject")) {
		PaintProjectToolsPropertyInspector *pct = memnew(PaintProjectToolsPropertyInspector);
		pct->set_paint_node(paint_node);
		add_custom_control(pct);
	}
}

PaintInspectorPlugin::PaintInspectorPlugin() {
}

PaintInspectorPlugin::~PaintInspectorPlugin() {
}

void PaintInspectorPlugin::_bind_methods() {
}
